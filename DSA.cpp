#include "DSA.h"

namespace CampusApp {

JoinRequest::JoinRequest() : groupID(0) {}
JoinRequest::JoinRequest(const Student& student, int groupID)
    : student(student), groupID(groupID) {}

RequestQueue::RequestQueue() : front(0), rear(-1), count(0) {}
bool RequestQueue::isEmpty() const { return count == 0; }
bool RequestQueue::isFull() const { return count == CAPACITY; }

bool RequestQueue::enqueue(const JoinRequest& request) {
    if (isFull()) return false;
    rear = (rear + 1) % CAPACITY;
    arr[rear] = request;
    ++count;
    return true;
}

bool RequestQueue::dequeue(JoinRequest& request) {
    if (isEmpty()) return false;
    request = arr[front];
    front = (front + 1) % CAPACITY;
    --count;
    return true;
}

HashTable::HashTable() { clear(); }

void HashTable::clear() {
    for (int i = 0; i < TABLE_SIZE; ++i) {
        occupied[i] = false;
        keys[i] = 0;
        values[i] = -1;
    }
}

int HashTable::hashFunction(int key) const {
    int result = key % TABLE_SIZE;
    return result < 0 ? result + TABLE_SIZE : result;
}

bool HashTable::insert(int key, int arrayIndex) {
    int index = hashFunction(key);
    for (int attempt = 0; attempt < TABLE_SIZE; ++attempt) {
        if (!occupied[index]) {
            occupied[index] = true;
            keys[index] = key;
            values[index] = arrayIndex;
            return true;
        }
        if (keys[index] == key) return false; // Duplicate ID.
        index = (index + 1) % TABLE_SIZE;     // Collision: next slot.
    }
    return false; // Table is full.
}

int HashTable::search(int key) const {
    int index = hashFunction(key);
    for (int attempt = 0; attempt < TABLE_SIZE; ++attempt) {
        if (!occupied[index]) return -1;
        if (keys[index] == key) return values[index];
        index = (index + 1) % TABLE_SIZE;
    }
    return -1;
}

} // namespace CampusApp
