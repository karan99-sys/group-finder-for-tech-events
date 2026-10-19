#include "Models.h"
#include "DSA.h"

namespace CampusApp {

    
    Student::Student() {
        id = 0;
        name = "";
        skill1 = "";
        skill2 = "";
    }

    Student::Student(int i, std::string n, std::string s1, std::string s2) {
        this->id = i;
        this->name = n;
        this->skill1 = s1;
        this->skill2 = s2;
    }

    int Student::getID() const {
        return id;
    }

    std::string Student::getName() const {
        return name;
    }

    std::string Student::getSkill1() const {
        return skill1;
    }

    std::string Student::getSkill2() const {
        return skill2;
    }

    void Student::display() const {
        std::cout << "ID: " << id
                  << " | Name: " << name
                  << " | Skills: " << skill1 << ", " << skill2 << std::endl;
    }

    MemberNode::MemberNode(Student s) {
        student = s;
        next = NULL;
    }

    
    Group::Group() {
        groupID = 0;
        groupName = "";
        requiredSkill1 = "";
        requiredSkill2 = "";
        head = NULL;
    }

    Group::Group(int id, std::string name, std::string s1, std::string s2) {
        groupID = id;
        groupName = name;
        requiredSkill1 = s1;
        requiredSkill2 = s2;
        head = NULL;
    }

    int Group::getID() const {
        return groupID;
    }

    std::string Group::getName() const {
        return groupName;
    }

    int Group::compatibility(const Student& s) const {
        int score = 0;

        if (s.getSkill1() == requiredSkill1 || s.getSkill2() == requiredSkill1) {
            score += 50;
        }

        if (s.getSkill1() == requiredSkill2 || s.getSkill2() == requiredSkill2) {
            score += 50;
        }

        return score;
    }

    void Group::addMember(const Student& s) {
        MemberNode* newNode = new MemberNode(s);

        if (head == NULL) {
            head = newNode;
        } else {
            MemberNode* temp = head;
            while (temp->next != NULL) {
                temp = temp->next;
            }
            temp->next = newNode;
        }
    }

    void Group::displayGroup() const {
        std::cout << "\nGroup ID: " << groupID << std::endl;
        std::cout << "Group Name: " << groupName << std::endl;
        std::cout << "Required Skills: " << requiredSkill1 << ", " << requiredSkill2 << std::endl;
        std::cout << "Members: ";

        MemberNode* temp = head;

        if (temp == NULL) {
            std::cout << "No members";
        } else {
            while (temp != NULL) {
                std::cout << temp->student.getName();
                if (temp->next != NULL) {
                    std::cout << " -> ";
                }
                temp = temp->next;
            }
        }

        std::cout << std::endl;
    }

    Group::~Group() {
        MemberNode* temp = head;
        while (temp != NULL) {
            MemberNode* nextNode = temp->next;
            delete temp;
            temp = nextNode;
        }
    }

    
    JoinRequest::JoinRequest() {
        groupID = 0;
    }

    JoinRequest::JoinRequest(const Student& s, int gid) {
        student = s;
        groupID = gid;
    }

    RequestQueue::RequestQueue() {
        front = 0;
        rear = -1;
    }

    bool RequestQueue::isEmpty() const {
        return front > rear;
    }

    bool RequestQueue::isFull() const {
        return rear == 49;
    }

    void RequestQueue::enqueue(const JoinRequest& request) {
        if (isFull()) {
            std::cout << "Request queue is full." << std::endl;
            return;
        }

        rear++;
        arr[rear] = request;
        std::cout << "Join request sent successfully." << std::endl;
    }

    JoinRequest RequestQueue::dequeue() {
        if (isEmpty()) {
            return JoinRequest();
        }

        JoinRequest request = arr[front];
        front++;
        return request;
    }

} 