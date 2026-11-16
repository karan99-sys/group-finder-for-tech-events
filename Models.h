#ifndef MODELS_H
#define MODELS_H

#include <iostream>
#include <string>

namespace CampusApp {

class Student {
private:
    int id;
    std::string name;
    std::string skill1;
    std::string skill2;

public:
    Student();
    Student(int id, const std::string& name,
            const std::string& skill1, const std::string& skill2);

    int getID() const;
    const std::string& getName() const;
    const std::string& getSkill1() const;
    const std::string& getSkill2() const;

    bool hasSkill(const std::string& skill) const;
    void display() const;
};


struct MemberNode {
    Student student;
    MemberNode* next;

    explicit MemberNode(const Student& student);
};

class Group {
private:
    int id;
    std::string name;
    std::string requiredSkill1;
    std::string requiredSkill2;
    int capacity;
    MemberNode* head;

    void clearMembers();
    void copyMembers(const MemberNode* source);

public:
    Group();
    Group(int id, const std::string& name,
          const std::string& skill1, const std::string& skill2,
          int capacity = 5);

    
    Group(const Group& other);
    Group& operator=(const Group& other);
    ~Group();

    int getID() const;
    const std::string& getName() const;
    const std::string& getRequiredSkill1() const;
    const std::string& getRequiredSkill2() const;
    int getCapacity() const;

    int memberCount() const;
    int vacancies() const;
    int compatibility(const Student& student) const;

    bool hasMember(int studentID) const;
    void writeMemberIDs(std::ostream& output) const;
    bool addMember(const Student& student);
    bool removeMember(int studentID);
    void displayGroup() const;
};

} // namespace CampusApp

#endif
