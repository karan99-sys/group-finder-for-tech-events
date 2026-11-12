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
    std::string getName() const;
    std::string getSkill1() const;
    std::string getSkill2() const;
    void display() const;
};

struct MemberNode {
    Student student;
    MemberNode* next;

    explicit MemberNode(const Student& student);
};

class Group {
private:
    int groupID;
    std::string groupName;
    std::string requiredSkill1;
    std::string requiredSkill2;
    MemberNode* head;

    void clearMembers();
    void copyMembers(const MemberNode* source);

public:
    Group();
    Group(int id, const std::string& name,
          const std::string& skill1, const std::string& skill2);
    Group(const Group& other);
    Group& operator=(const Group& other);
    ~Group();

    int getID() const;
    std::string getName() const;
    std::string getRequiredSkill1() const;
    std::string getRequiredSkill2() const;

    int compatibility(const Student& student) const;
    void addMember(const Student& student);
    void displayGroup() const;
};

} // namespace CampusApp

#endif
