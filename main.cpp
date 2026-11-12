#include <iostream>
#include <limits>
#include <string>
#include "Models.h"
#include "DSA.h"

using namespace std;
using namespace CampusApp;

const int MAX_STUDENTS = 500;
const int MAX_GROUPS = 200;
const string STUDENT_FILE = "students.txt";
const string GROUP_FILE = "groups.txt";

void rebuildHashTables(Student students[], int studentCount,
                       Group groups[], int groupCount,
                       HashTable& studentHash, HashTable& groupHash) {
    studentHash.clear();
    groupHash.clear();
    for (int i = 0; i < studentCount; ++i)
        studentHash.insert(students[i].getID(), i);
    for (int i = 0; i < groupCount; ++i)
        groupHash.insert(groups[i].getID(), i);
}

Student* findStudent(Student students[], const HashTable& table, int id) {
    int index = table.search(id);
    return index == -1 ? nullptr : &students[index];
}

Group* findGroup(Group groups[], const HashTable& table, int id) {
    int index = table.search(id);
    return index == -1 ? nullptr : &groups[index];
}

void showStudents(const Student students[], int count) {
    cout << "\n========== STUDENTS (" << count << ") ==========\n";
    for (int i = 0; i < count; ++i) students[i].display();
}

void showGroups(const Group groups[], int count) {
    cout << "\n========== GROUPS (" << count << ") ==========\n";
    for (int i = 0; i < count; ++i) groups[i].displayGroup();
}

int main() {
    Student students[MAX_STUDENTS];
    Group groups[MAX_GROUPS];
    int studentCount = 0;
    int groupCount = 0;

    if (!loadStudents(STUDENT_FILE, students, studentCount, MAX_STUDENTS)) {
        cout << "Could not open " << STUDENT_FILE << ". Keep it beside the executable.\n";
        return 1;
    }
    if (!loadGroups(GROUP_FILE, groups, groupCount, MAX_GROUPS)) {
        cout << "Could not open " << GROUP_FILE << ". Keep it beside the executable.\n";
        return 1;
    }

    HashTable studentHash;
    HashTable groupHash;
    rebuildHashTables(students, studentCount, groups, groupCount,
                      studentHash, groupHash);

    RequestQueue requests;
    int choice = 0;

    cout << "Loaded " << studentCount << " students and " << groupCount << " groups.\n";

    do {
        cout << "\n=================================\n"
             << "       CAMPUS GROUP FINDER\n"
             << "=================================\n"
             << "1. View Students\n"
             << "2. View Groups\n"
             << "3. Find Best Group (Hash Search)\n"
             << "4. Send Join Request (Hash Search)\n"
             << "5. Process Next Request\n"
             << "6. Add Student\n"
             << "7. Add Group\n"
             << "8. Save Data\n"
             << "9. Exit\n"
             << "Enter choice: ";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Please enter a valid number.\n";
            continue;
        }

        if (choice == 1) {
            showStudents(students, studentCount);
        }
        else if (choice == 2) {
            showGroups(groups, groupCount);
        }
        else if (choice == 3) {
            int studentID;
            cout << "Enter Student ID: ";
            cin >> studentID;
            Student* student = findStudent(students, studentHash, studentID);
            if (student == nullptr) {
                cout << "Student not found.\n";
                continue;
            }
            if (groupCount == 0) {
                cout << "No groups are available.\n";
                continue;
            }

            int bestScore = -1;
            int bestIndex = -1;
            cout << "\nCompatibility Scores:\n";
            for (int i = 0; i < groupCount; ++i) {
                int score = groups[i].compatibility(*student);
                cout << groups[i].getName() << " = " << score << "%\n";
                if (score > bestScore) {
                    bestScore = score;
                    bestIndex = i;
                }
            }
            cout << "\nBest Group: " << groups[bestIndex].getName()
                 << " (" << bestScore << "% match)\n";
        }
        else if (choice == 4) {
            int studentID, groupID;
            cout << "Enter Student ID: ";
            cin >> studentID;
            Student* student = findStudent(students, studentHash, studentID);
            if (student == nullptr) {
                cout << "Student not found.\n";
                continue;
            }

            cout << "Enter Group ID: ";
            cin >> groupID;
            Group* group = findGroup(groups, groupHash, groupID);
            if (group == nullptr) {
                cout << "Group not found.\n";
                continue;
            }

            if (requests.enqueue(JoinRequest(*student, groupID)))
                cout << "Join request added to the queue.\n";
            else
                cout << "Request queue is full.\n";
        }
        else if (choice == 5) {
            JoinRequest request;
            if (!requests.dequeue(request)) {
                cout << "No pending requests.\n";
                continue;
            }

            cout << "\nRequest from: " << request.student.getName()
                 << " (ID " << request.student.getID() << ")\n"
                 << "Requested Group ID: " << request.groupID << '\n';
            char answer;
            cout << "Accept request? (y/n): ";
            cin >> answer;

            if (answer == 'y' || answer == 'Y') {
                Group* group = findGroup(groups, groupHash, request.groupID);
                if (group != nullptr) {
                    group->addMember(request.student);
                    cout << "Request accepted. Student added to group.\n";
                } else {
                    cout << "Group no longer exists.\n";
                }
            } else {
                cout << "Request rejected.\n";
            }
        }
        else if (choice == 6) {
            if (studentCount >= MAX_STUDENTS) {
                cout << "Student storage is full.\n";
                continue;
            }
            int id;
            string name, skill1, skill2;
            cout << "Enter new Student ID: ";
            cin >> id;
            if (studentHash.search(id) != -1) {
                cout << "This ID already exists.\n";
                continue;
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Enter name: "; getline(cin, name);
            cout << "Enter first skill: "; getline(cin, skill1);
            cout << "Enter second skill: "; getline(cin, skill2);

            students[studentCount] = Student(id, name, skill1, skill2);
            studentHash.insert(id, studentCount);
            ++studentCount;
            cout << "Student added. Choose Save Data to store it permanently.\n";
        }
        else if (choice == 7) {
            if (groupCount >= MAX_GROUPS) {
                cout << "Group storage is full.\n";
                continue;
            }
            int id;
            string name, skill1, skill2;
            cout << "Enter new Group ID: ";
            cin >> id;
            if (groupHash.search(id) != -1) {
                cout << "This ID already exists.\n";
                continue;
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Enter group name: "; getline(cin, name);
            cout << "Enter first required skill: "; getline(cin, skill1);
            cout << "Enter second required skill: "; getline(cin, skill2);

            groups[groupCount] = Group(id, name, skill1, skill2);
            groupHash.insert(id, groupCount);
            ++groupCount;
            cout << "Group added. Choose Save Data to store it permanently.\n";
        }
        else if (choice == 8) {
            bool a = saveStudents(STUDENT_FILE, students, studentCount);
            bool b = saveGroups(GROUP_FILE, groups, groupCount);
            cout << (a && b ? "All student and group data saved.\n"
                            : "Error while saving one or more files.\n");
        }
        else if (choice == 9) {
            saveStudents(STUDENT_FILE, students, studentCount);
            saveGroups(GROUP_FILE, groups, groupCount);
            cout << "Data saved. Exiting program.\n";
        }
        else {
            cout << "Invalid choice.\n";
        }
    } while (choice != 9);

    return 0;
}
