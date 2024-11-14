#include <iostream>
#include <string>
#include <bits/stdc++.h>

using namespace std;

class Node {
public:
  string name;
  int size;
  int startAddress;
  Node *next;
  bool isFree;

  Node(string name, int size) {
    isFree = false;
    this->name = name;
    this->size = size;
    this->next = NULL;
  }
};

void display(Node *head) {
  Node *temp;
  temp = head;
  cout << "\nMemory status:\n";
  while (temp != NULL) {
    cout << "[PID: " << temp->name << ", Size:" << temp->size << ", Start:" << temp->startAddress << ", End:" << (temp->startAddress + temp->size) << "]";
    temp = temp->next;
    if (temp != NULL)
      cout << ", ";
  }
  cout << endl;
}

void mergeFreeBlocks(Node *&head) {
    Node *curr = head;
    while (curr != NULL) {
        if (curr->name == "free") {
            int accumulatedSize = curr->size;
            Node *runner = curr->next;
            while (runner != NULL && runner->name == "free") {
                accumulatedSize += runner->size;
                curr->next = runner->next;
                delete runner;
                runner = curr->next;
            }
            curr->size = accumulatedSize;
        }
        curr = curr->next;
    }
}

void deallocation(Node *&head) {
    cout << "Enter program name to deallocate: ";
    string target;
    cin >> target;

    Node *current = head;
    while (current != NULL) {
        if (current->name == target) {
            current->name = "free";
            mergeFreeBlocks(head);
            display(head);
            return;
        }
        current = current->next;
    }
    cout << "Program not found.\n";
}

void compaction(Node *&head) {
    if (!head || head->next == nullptr) {
        cout << "Compaction done!\n";
        return;
    }

    Node *current = head;
    int accumulatedFreeMemory = 0;

    // If the first node is free
    if (current->name == "free") {
        accumulatedFreeMemory += current->size;
        head = current->next;
        delete current;  
        current = head;
    }

    Node *prev = current;
    current = current->next;

    while (current != nullptr) {
        if (current->name == "free") {
            accumulatedFreeMemory += current->size;

            // If the free node is at the end
            if (current->next == nullptr) {
                prev->next = nullptr;
                delete current;
                break;
            }

            // Skip over the free node by linking prev to current->next
            prev->next = current->next;
            delete current;
            current = prev->next;
        } else {
            prev = current;
            current = current->next;
        }
    }

    //  reset  start addresses
    Node *temp = head;
    int addressCounter = 0;

    while (temp != nullptr) {
        temp->startAddress = addressCounter;
        addressCounter += temp->size;
        temp = temp->next;
    }

    // Add a new large free block at the end 
    if (accumulatedFreeMemory > 0) {
        Node *newFreeBlock = new Node("free", accumulatedFreeMemory);
        newFreeBlock->startAddress = addressCounter;

        Node *last = head;
        while (last->next != nullptr) {
            last = last->next;
        }
        last->next = newFreeBlock;
        newFreeBlock->next = nullptr;
    }

    display(head);
    cout << "\nCompaction Done!\n";
}

void allocation(Node *&head) {
    cout << "Enter program name: ";
    string name;
    cin >> name;
    cout << "Enter size: ";
    int size;
    cin >> size;

    Node* largestBlock = NULL;
    Node* curr = head;
    int largestSize= INT_MIN;
    while(curr!=NULL){
        if(curr->name == "free" && curr->size >= size){
            if(curr->size > largestSize){
                largestSize = curr->size;
                largestBlock = curr;
            }
        }
        curr = curr->next;
    }

    if(largestBlock == NULL){
        cout << "Memory not available\n";
        return;
    }

    if(largestBlock->size == size){
        largestBlock->name = name;
    } else {
        Node* newNode = new Node(name, size);
        newNode->startAddress = largestBlock->startAddress;
        newNode->next = largestBlock;
        newNode->next->startAddress += size;
        newNode->next->size -= size;

        if(largestBlock == head){
            head = newNode;
        } else {
            Node* prev = head;
            while(prev->next != largestBlock){
                prev = prev->next;
            }
            prev->next = newNode;
        }
    }
    display(head);
}

int main() {
  Node *TotalMemory = new Node("free", 1000);
  TotalMemory->startAddress = 0;
  Node *head = TotalMemory;

  display(head);

  int choice;
  
  while (true) {
    cout << "\n1. Allocate Memory \n2. Deallocate Memory\n3. Compact Memory\n4. Display Memory\n5. Exit\n";
    cout << "Enter your choice: ";
    cin >> choice;

    switch (choice) {
    case 1:
      allocation(head);  
      break;
    case 2:
      deallocation(head);
      break;
    case 3:
      compaction(head);
      break;
    case 4:
      display(head);
      break;
    case 5:
      cout << "Exiting Code\n";
      return 0;
    default:
      cout << "Invalid choice. Try again.\n";
    }
  }
  return 0;
}
