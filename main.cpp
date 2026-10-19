#include <iostream>
#include "Models.h"
#include "DSA.h"

using namespace std;
using namespace CampusApp;

Student* findStudent(Student students[], int size, int id) {
    for (int i = 0; i < size; i++) {
        if (students[i].getID() == id) {
            return &students[i];
        }
    }
    return NULL;
}

Group* findGroup(Group groups[], int size, int id) {
    for (int i = 0; i < size; i++) {
        if (groups[i].getID() == id) {
            return &groups[i];
        }
    }
    return NULL;
}

int main() {
    Student students[3] = {
        Student(101, "Aaditya", "C++", "DSA"),
        Student(102, "Karan", "Python", "AI"),
        Student(103, "Amit", "HTML", "CSS")
    };

    Group groups[3] = {
        Group(1, "Code Masters", "C++", "DSA"),
        Group(2, "AI Warriors", "Python", "AI"),
        Group(3, "Web Wizards", "HTML", "CSS")
    };

    RequestQueue requests;

    int choice;

    do {
        cout << "\n==============================" << endl;
        cout << "     CAMPUS GROUP FINDER" << endl;
        cout << "==============================" << endl;
        cout << "1. View Students" << endl;
        cout << "2. View Groups" << endl;
        cout << "3. Find Best Group" << endl;
        cout << "4. Send Join Request" << endl;
        cout << "5. Process Next Request" << endl;
        cout << "6. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1) {
            cout << "\n--- Students ---" << endl;
            for (int i = 0; i < 3; i++) {
                students[i].display();
            }
        }
        else if (choice == 2) {
            cout << "\n--- Groups ---" << endl;
            for (int i = 0; i < 3; i++) {
                groups[i].displayGroup();
            }
        }
        else if (choice == 3) {
            int studentID;
            cout << "Enter Student ID: ";
            cin >> studentID;

            Student* student = findStudent(students, 3, studentID);

            if (student == NULL) {
                cout << "Student not found." << endl;
                continue;
            }

            int bestScore = -1;
            int bestGroupIndex = -1;

            cout << "\nCompatibility Scores:" << endl;

            for (int i = 0; i < 3; i++) {
                int score = groups[i].compatibility(*student);

                cout << groups[i].getName()
                     << " = " << score << "%" << endl;

                if (score > bestScore) {
                    bestScore = score;
                    bestGroupIndex = i;
                }
            }

            cout << "\nBest Group: "
                 << groups[bestGroupIndex].getName()
                 << " (" << bestScore << "% match)" << endl;
        }
        else if (choice == 4) {
            int studentID;
            int groupID;

            cout << "Enter Student ID: ";
            cin >> studentID;

            Student* student = findStudent(students, 3, studentID);

            if (student == NULL) {
                cout << "Student not found." << endl;
                continue;
            }

            cout << "Enter Group ID: ";
            cin >> groupID;

            Group* group = findGroup(groups, 3, groupID);

            if (group == NULL) {
                cout << "Group not found." << endl;
                continue;
            }

            JoinRequest request(*student, groupID);
            requests.enqueue(request);
        }
        else if (choice == 5) {
            if (requests.isEmpty()) {
                cout << "No pending requests." << endl;
                continue;
            }

            JoinRequest request = requests.dequeue();

            cout << "\nRequest from: "
                 << request.student.getName() << endl;
            cout << "Requested Group ID: "
                 << request.groupID << endl;

            char answer;
            cout << "Accept request? (y/n): ";
            cin >> answer;

            if (answer == 'y' || answer == 'Y') {
                Group* group = findGroup(groups, 3, request.groupID);

                if (group != NULL) {
                    group->addMember(request.student);
                    cout << "Request accepted. Student added to group." << endl;
                }
            } else {
                cout << "Request rejected." << endl;
            }
        }
        else if (choice == 6) {
            cout << "Exiting program." << endl;
        }
        else {
            cout << "Invalid choice." << endl;
        }

    } while (choice != 6);

    return 0;
}