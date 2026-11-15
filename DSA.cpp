#include "DSA.h"

#include <algorithm>
#include <functional>



namespace CampusApp {

JoinRequest::JoinRequest() : groupID(0) {}

JoinRequest::JoinRequest(const Student& value, int requestedGroupID)
    : student(value), groupID(requestedGroupID) {}

RequestQueue::RequestQueue() : front(0), rear(-1), count(0) {}

bool RequestQueue::isEmpty() const { return count == 0; }
bool RequestQueue::isFull() const { return count == CAPACITY; }

bool RequestQueue::contains(int studentID) const {
    for (int offset = 0; offset < count; ++offset) {
        int index = (front + offset) % CAPACITY;
        if (data[index].student.getID() == studentID) return true;
    }
    return false;
}

bool RequestQueue::enqueue(const JoinRequest& request) {
    if (isFull()) return false;
    rear = (rear + 1) % CAPACITY;
    data[rear] = request;
    ++count;
    return true;
}

bool RequestQueue::dequeue(JoinRequest& request) {
    if (isEmpty()) return false;
    request = data[front];
    front = (front + 1) % CAPACITY;
    --count;
    return true;
}

RequestStack::RequestStack() : top(-1) {}
bool RequestStack::isEmpty() const { return top == -1; }
bool RequestStack::isFull() const { return top == CAPACITY - 1; }

bool RequestStack::push(const JoinRequest& request) {
    if (isFull()) return false;
    data[++top] = request;
    return true;
}

bool RequestStack::pop(JoinRequest& request) {
    if (isEmpty()) return false;
    request = data[top--];
    return true;
}

HashTable::HashTable() { clear(); }

void HashTable::clear() {
    for (int i = 0; i < SIZE; ++i) {
        used[i] = false;
        keys[i] = 0;
        values[i] = -1;
    }
}

int HashTable::hash(int key) const {
    return (key % SIZE + SIZE) % SIZE;
}

bool HashTable::insert(int key, int value) {
    int index = hash(key);

    for (int checked = 0; checked < SIZE; ++checked) {
        if (!used[index] || keys[index] == key) {
            used[index] = true;
            keys[index] = key;
            values[index] = value;
            return true;
        }
        index = (index + 1) % SIZE; // Linear probing.
    }
    return false;
}

int HashTable::search(int key) const {
    int index = hash(key);

    for (int checked = 0; checked < SIZE; ++checked) {
        if (!used[index]) return -1;
        if (keys[index] == key) return values[index];
        index = (index + 1) % SIZE;
    }
    return -1;
}

void selectionSortGroupsByVacancies(std::vector<Group>& groups) {
    for (std::size_t i = 0; i < groups.size(); ++i) {
        std::size_t bestIndex = i;

        for (std::size_t j = i + 1; j < groups.size(); ++j) {
            if (groups[j].vacancies() > groups[bestIndex].vacancies()) {
                bestIndex = j;
            }
        }

        if (bestIndex != i) std::swap(groups[i], groups[bestIndex]);
    }
}

int findStudentGroup(const std::vector<Group>& groups, int studentID) {
    for (std::size_t i = 0; i < groups.size(); ++i) {
        if (groups[i].hasMember(studentID)) return static_cast<int>(i);
    }
    return -1;
}

void CompatibilityGraph::build(const std::vector<Student>& students,
                               const std::vector<Group>& groups) {
    const int studentCount = static_cast<int>(students.size());
    adjacencyList.assign(students.size() + groups.size(), {});

    for (int studentIndex = 0; studentIndex < studentCount; ++studentIndex) {
        for (std::size_t groupIndex = 0; groupIndex < groups.size(); ++groupIndex) {
            if (groups[groupIndex].compatibility(students[studentIndex]) > 0) {
                int groupVertex = studentCount + static_cast<int>(groupIndex);
                adjacencyList[studentIndex].push_back(groupVertex);
                adjacencyList[groupVertex].push_back(studentIndex);
            }
        }
    }
}

void CompatibilityGraph::showBFS(int studentIndex,
                                 const std::vector<Student>& students,
                                 const std::vector<Group>& groups) const {
    if (studentIndex < 0 || studentIndex >= static_cast<int>(students.size())) return;

    std::vector<bool> visited(adjacencyList.size(), false);
    std::queue<int> pendingVertices;
    pendingVertices.push(studentIndex);
    visited[studentIndex] = true;
    bool found = false;

    std::cout << "BFS compatible groups for "
              << students[studentIndex].getName() << ":\n";

    while (!pendingVertices.empty()) {
        int current = pendingVertices.front();
        pendingVertices.pop();

        for (int neighbour : adjacencyList[current]) {
            if (visited[neighbour]) continue;
            visited[neighbour] = true;
            pendingVertices.push(neighbour);

            if (neighbour >= static_cast<int>(students.size())) {
                int groupIndex = neighbour - static_cast<int>(students.size());
                if (groups[groupIndex].compatibility(students[studentIndex]) > 0) {
                    std::cout << "- " << groups[groupIndex].getName()
                              << " (" << groups[groupIndex].compatibility(students[studentIndex])
                              << "% match)\n";
                    found = true;
                }
            }
        }
    }

    if (!found) std::cout << "No compatible groups found.\n";
}

void CompatibilityGraph::showDFS(int studentIndex,
                                 const std::vector<Student>& students,
                                 const std::vector<Group>& groups) const {
    if (studentIndex < 0 || studentIndex >= static_cast<int>(students.size())) return;

    std::vector<bool> visited(adjacencyList.size(), false);
    bool found = false;

    std::function<void(int)> depthFirstVisit = [&](int current) {
        visited[current] = true;

        for (int neighbour : adjacencyList[current]) {
            if (visited[neighbour]) continue;
            visited[neighbour] = true;

            if (neighbour >= static_cast<int>(students.size())) {
                int groupIndex = neighbour - static_cast<int>(students.size());
                if (groups[groupIndex].compatibility(students[studentIndex]) > 0) {
                    std::cout << "- " << groups[groupIndex].getName()
                              << " (" << groups[groupIndex].compatibility(students[studentIndex])
                              << "% match)\n";
                    found = true;
                }
            }

            depthFirstVisit(neighbour);
        }
    };

    std::cout << "DFS compatible groups for "
              << students[studentIndex].getName() << ":\n";
    depthFirstVisit(studentIndex);
    if (!found) std::cout << "No compatible groups found.\n";
}

} // namespace CampusApp
