#include "Models.h"

namespace CampusApp {

Student::Student() : id(0) {}

Student::Student(int studentID, const std::string& studentName,
                 const std::string& firstSkill, const std::string& secondSkill)
    : id(studentID), name(studentName),
      skill1(firstSkill), skill2(secondSkill) {}

int Student::getID() const { return id; }
const std::string& Student::getName() const { return name; }
const std::string& Student::getSkill1() const { return skill1; }
const std::string& Student::getSkill2() const { return skill2; }

bool Student::hasSkill(const std::string& skill) const {
    return skill == skill1 || skill == skill2;
}

void Student::display() const {
    std::cout << "ID: " << id
              << " | Name: " << name
              << " | Skills: " << skill1 << ", " << skill2 << '\n';
}

MemberNode::MemberNode(const Student& value)
    : student(value), next(nullptr) {}

Group::Group()
    : id(0), capacity(5), head(nullptr) {}

Group::Group(int groupID, const std::string& groupName,
             const std::string& skill1, const std::string& skill2,
             int maxCapacity)
    : id(groupID), name(groupName), requiredSkill1(skill1),
      requiredSkill2(skill2), capacity(maxCapacity), head(nullptr) {}

void Group::clearMembers() {
    while (head != nullptr) {
        MemberNode* nodeToDelete = head;
        head = head->next;
        delete nodeToDelete;
    }
}

void Group::copyMembers(const MemberNode* source) {
    for (const MemberNode* current = source;
         current != nullptr; current = current->next) {
        addMember(current->student);
    }
}

Group::Group(const Group& other)
    : id(other.id), name(other.name),
      requiredSkill1(other.requiredSkill1),
      requiredSkill2(other.requiredSkill2),
      capacity(other.capacity), head(nullptr) {
    copyMembers(other.head);
}

Group& Group::operator=(const Group& other) {
    if (this != &other) {
        clearMembers();
        id = other.id;
        name = other.name;
        requiredSkill1 = other.requiredSkill1;
        requiredSkill2 = other.requiredSkill2;
        capacity = other.capacity;
        copyMembers(other.head);
    }
    return *this;
}

Group::~Group() {
    clearMembers();
}

int Group::getID() const { return id; }
const std::string& Group::getName() const { return name; }
const std::string& Group::getRequiredSkill1() const { return requiredSkill1; }
const std::string& Group::getRequiredSkill2() const { return requiredSkill2; }
int Group::getCapacity() const { return capacity; }

int Group::memberCount() const {
    int count = 0;
    for (const MemberNode* current = head;
         current != nullptr; current = current->next) {
        ++count;
    }
    return count;
}

int Group::vacancies() const {
    return capacity - memberCount();
}

int Group::compatibility(const Student& student) const {
    int score = 0;
    if (student.hasSkill(requiredSkill1)) score += 50;
    if (student.hasSkill(requiredSkill2)) score += 50;
    return score;
}

bool Group::hasMember(int studentID) const {
    for (const MemberNode* current = head;
         current != nullptr; current = current->next) {
        if (current->student.getID() == studentID) return true;
    }
    return false;
}

void Group::writeMemberIDs(std::ostream& output) const {
    for (const MemberNode* current = head;
         current != nullptr; current = current->next) {
        output << id << '|' << current->student.getID() << '\n';
    }
}

bool Group::addMember(const Student& student) {
    if (vacancies() <= 0 || hasMember(student.getID())) return false;

    MemberNode* newNode = new MemberNode(student);
    if (head == nullptr) {
        head = newNode;
        return true;
    }

    MemberNode* current = head;
    while (current->next != nullptr) current = current->next;
    current->next = newNode;
    return true;
}

bool Group::removeMember(int studentID) {
    MemberNode* previous = nullptr;
    MemberNode* current = head;

    while (current != nullptr && current->student.getID() != studentID) {
        previous = current;
        current = current->next;
    }

    if (current == nullptr) return false;

    if (previous == nullptr) head = current->next;
    else previous->next = current->next;

    delete current;
    return true;
}

void Group::displayGroup() const {
    std::cout << "\nGroup ID: " << id << " | " << name << '\n'
              << "Required skills: " << requiredSkill1
              << ", " << requiredSkill2 << '\n'
              << "Members (" << memberCount() << "/" << capacity << "): ";

    if (head == nullptr) std::cout << "None";
    for (const MemberNode* current = head;
         current != nullptr; current = current->next) {
        std::cout << current->student.getName();
        if (current->next != nullptr) std::cout << " -> ";
    }

    std::cout << "\nVacancies: " << vacancies() << '\n';
}

} 
