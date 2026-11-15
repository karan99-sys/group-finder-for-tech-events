#ifndef DSA_H
#define DSA_H

#include "Models.h"
#include <string>
#include <vector>

namespace CampusApp {

struct JoinRequest {
    Student student;
    int groupID;

    JoinRequest();
    JoinRequest(const Student& student, int groupID);
};

class RequestQueue {
private:
    static const int CAPACITY = 100;
    JoinRequest data[CAPACITY];
    int front;
    int rear;
    int count;

public:
    RequestQueue();
    bool isEmpty() const;
    bool isFull() const;
    bool contains(int studentID) const;
    bool enqueue(const JoinRequest& request);
    bool dequeue(JoinRequest& request);
};

class RequestStack {
private:
    static const int CAPACITY = 100;
    JoinRequest data[CAPACITY];
    int top;

public:
    RequestStack();
    bool isEmpty() const;
    bool isFull() const;
    bool push(const JoinRequest& request);
    bool pop(JoinRequest& request);
};

class HashTable {
private:
    static const int SIZE = 2003;
    int keys[SIZE];
    int values[SIZE];
    bool used[SIZE];
    int hash(int key) const;

public:
    HashTable();
    void clear();
    bool insert(int key, int value);
    int search(int key) const;
};

void selectionSortGroupsByVacancies(std::vector<Group>& groups);

int findStudentGroup(const std::vector<Group>& groups, int studentID);


class CompatibilityGraph {
private:
    std::vector<std::vector<int>> adjacencyList;

public:
    void build(const std::vector<Student>& students,
               const std::vector<Group>& groups);
    void showBFS(int studentIndex,
                 const std::vector<Student>& students,
                 const std::vector<Group>& groups) const;
    void showDFS(int studentIndex,
                 const std::vector<Student>& students,
                 const std::vector<Group>& groups) const;
};


bool loadStudents(const std::string& fileName, std::vector<Student>& students);
bool loadGroups(const std::string& fileName, std::vector<Group>& groups);
bool loadMemberships(const std::string& fileName,
                     const std::vector<Student>& students,
                     std::vector<Group>& groups);
bool saveStudents(const std::string& fileName, const std::vector<Student>& students);
bool saveGroups(const std::string& fileName, const std::vector<Group>& groups);
bool saveMemberships(const std::string& fileName, const std::vector<Group>& groups);

} // namespace CampusApp

#endif
