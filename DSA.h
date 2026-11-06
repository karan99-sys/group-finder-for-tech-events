#ifndef DSA_H
#define DSA_H

#include "Models.h"
#include <string>

namespace CampusApp {

class JoinRequest {
public:
    Student student;
    int groupID;

    JoinRequest();
    JoinRequest(const Student& student, int groupID);
};

class RequestQueue {
private:
    static const int CAPACITY = 100;
    JoinRequest arr[CAPACITY];
    int front;
    int rear;
    int count;

public:
    RequestQueue();
    bool isEmpty() const;
    bool isFull() const;
    bool enqueue(const JoinRequest& request);
    bool dequeue(JoinRequest& request);
};

// Hash table maps a record ID to its position in the corresponding array.
// Collision handling: linear probing (open addressing).
class HashTable {
private:
    static const int TABLE_SIZE = 2003;
    int keys[TABLE_SIZE];
    int values[TABLE_SIZE];
    bool occupied[TABLE_SIZE];

    int hashFunction(int key) const;

public:
    HashTable();
    void clear();
    bool insert(int key, int arrayIndex);
    int search(int key) const; // Returns array index, or -1 if not found.
};

// File-handling declarations. All definitions are in FileHandling.cpp.
bool loadStudents(const std::string& filename, Student students[], int& count, int capacity);
bool loadGroups(const std::string& filename, Group groups[], int& count, int capacity);
bool saveStudents(const std::string& filename, const Student students[], int count);
bool saveGroups(const std::string& filename, const Group groups[], int count);

} // namespace CampusApp

#endif
