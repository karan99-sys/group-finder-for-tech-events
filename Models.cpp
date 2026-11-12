#include "Models.h"

namespace CampusApp {

Student::Student() : id(0), name(""), skill1(""), skill2("") {}

Student::Student(int id, const std::string& name,
                 const std::string& skill1, const std::string& skill2)
    : id(id), name(name), skill1(skill1), skill2(skill2) {}

int Student::getID() const { return id; }
std::string Student::getName() const { return name; }
std::string Student::getSkill1() const { return skill1; }
std::string Student::getSkill2() const { return skill2; }

void Student::display() const {
    std::cout << "ID: " << id << " | Name: " << name
              << " | Skills: " << skill1 << ", " << skill2 << '\n';
}

MemberNode::MemberNode(const Student& student)
    : student(student), next(nullptr) {}

Group::Group()
    : groupID(0), groupName(""), requiredSkill1(""),
      requiredSkill2(""), head(nullptr) {}

Group::Group(int id, const std::string& name,
             const std::string& skill1, const std::string& skill2)
    : groupID(id), groupName(name), requiredSkill1(skill1),
      requiredSkill2(skill2), head(nullptr) {}

void Group::clearMembers() {
    MemberNode* current = head;
    while (current != nullptr) {
        MemberNode* nextNode = current->next;
        delete current;
        current = nextNode;
    }
    head = nullptr;
}

void Group::copyMembers(const MemberNode* source) {
    while (source != nullptr) {
        addMember(source->student);
        source = source->next;
    }
}

Group::Group(const Group& other)
    : groupID(other.groupID), groupName(other.groupName),
      requiredSkill1(other.requiredSkill1),
      requiredSkill2(other.requiredSkill2), head(nullptr) {
    copyMembers(other.head);
}

Group& Group::operator=(const Group& other) {
    if (this != &other) {
        clearMembers();
        groupID = other.groupID;
        groupName = other.groupName;
        requiredSkill1 = other.requiredSkill1;
        requiredSkill2 = other.requiredSkill2;
        copyMembers(other.head);
    }
    return *this;
}

Group::~Group() { clearMembers(); }

int Group::getID() const { return groupID; }
std::string Group::getName() const { return groupName; }
std::string Group::getRequiredSkill1() const { return requiredSkill1; }
std::string Group::getRequiredSkill2() const { return requiredSkill2; }

int Group::compatibility(const Student& student) const {
    int score = 0;
    if (student.getSkill1() == requiredSkill1 ||
        student.getSkill2() == requiredSkill1) score += 50;
    if (student.getSkill1() == requiredSkill2 ||
        student.getSkill2() == requiredSkill2) score += 50;
    return score;
}

void Group::addMember(const Student& student) {
    // Avoid adding the same student to this group more than once.
    MemberNode* current = head;
    while (current != nullptr) {
        if (current->student.getID() == student.getID()) return;
        current = current->next;
    }

    MemberNode* newNode = new MemberNode(student);
    if (head == nullptr) {
        head = newNode;
        return;
    }

    current = head;
    while (current->next != nullptr) current = current->next;
    current->next = newNode;
}

void Group::displayGroup() const {
    std::cout << "\nGroup ID: " << groupID
              << "\nGroup Name: " << groupName
              << "\nRequired Skills: " << requiredSkill1
              << ", " << requiredSkill2
              << "\nMembers: ";

    if (head == nullptr) {
        std::cout << "No members";
    } else {
        const MemberNode* current = head;
        while (current != nullptr) {
            std::cout << current->student.getName();
            if (current->next != nullptr) std::cout << " -> ";
            current = current->next;
        }
    }
    std::cout << '\n';
}

} // namespace CampusApp
