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
        Student(int i, std::string n, std::string s1, std::string s2);

        int getID() const;
        std::string getName() const;
        std::string getSkill1() const;
        std::string getSkill2() const;
        void display() const;
    };

    class MemberNode {
    public:
        Student student;
        MemberNode* next;

        MemberNode(Student s);
    };

    class Group {
    private:
        int groupID;
        std::string groupName;
        std::string requiredSkill1;
        std::string requiredSkill2;
        MemberNode* head;

    public:
        Group();
        Group(int id, std::string name, std::string s1, std::string s2);
        ~Group();

        int getID() const;
        std::string getName() const;
        int compatibility(const Student& s) const;
        void addMember(const Student& s);
        void displayGroup() const;
    };

} 

#endif