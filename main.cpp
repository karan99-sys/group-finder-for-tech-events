#include "DSA.h"

#include <algorithm>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

using namespace CampusApp;
using namespace std;

namespace {
const string STUDENT_FILE = "students.txt";
const string GROUP_FILE = "groups.txt";
const string MEMBERSHIP_FILE = "members.txt";

void rebuildIndexes(const vector<Student>& students,
                    const vector<Group>& groups,
                    HashTable& studentIndex,
                    HashTable& groupIndex) {
    studentIndex.clear();
    groupIndex.clear();

    for (size_t i = 0; i < students.size(); ++i) {
        studentIndex.insert(students[i].getID(), static_cast<int>(i));
    }
    for (size_t i = 0; i < groups.size(); ++i) {
        groupIndex.insert(groups[i].getID(), static_cast<int>(i));
    }
}

bool readInteger(const string& prompt, int& value) {
    cout << prompt;
    if (cin >> value) return true;

    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Invalid input. Please enter a number.\n";
    return false;
}

void showStudents(const vector<Student>& students) {
    cout << "\n========== STUDENT DIRECTORY ==========\n";
    for (const Student& student : students) student.display();
}

void showGroups(const vector<Group>& groups) {
    cout << "\n========== AVAILABLE GROUPS ==========\n";
    for (const Group& group : groups) group.displayGroup();
}

void showMenu() {
    cout << "\n============== TEAM NEXUS ==============\n"
         << "1. View all students\n"
         << "2. View all groups\n"
         << "3. Search student by ID \n"
         << "4. Find compatible groups\n"
         << "5. Explore compatible groups \n"
         << "6. Sort groups by vacancies\n"
         << "7. Send join request \n"
         << "8. Process next request\n"
         << "9. Undo latest accepted membership\n"
         << "0. Save and exit\n"
         << "Choose an option: ";
}

bool saveAll(const vector<Student>& students, const vector<Group>& groups) {
    bool studentsSaved = saveStudents(STUDENT_FILE, students);
    bool groupsSaved = saveGroups(GROUP_FILE, groups);
    bool membershipsSaved = saveMemberships(MEMBERSHIP_FILE, groups);
    return studentsSaved && groupsSaved && membershipsSaved;
}
} // namespace

int main() {
    vector<Student> students;
    vector<Group> groups;

    if (!loadStudents(STUDENT_FILE, students)) {
        cerr << "Cannot open students.txt. Keep it in the program folder.\n";
        return 1;
    }
    if (!loadGroups(GROUP_FILE, groups)) {
        cerr << "Cannot open groups.txt. Keep it in the program folder.\n";
        return 1;
    }
    if (!loadMemberships(MEMBERSHIP_FILE, students, groups)) {
        cerr << "Could not load saved memberships.\n";
        return 1;
    }

    HashTable studentIndex;
    HashTable groupIndex;
    rebuildIndexes(students, groups, studentIndex, groupIndex);

    RequestQueue pendingRequests;
    RequestStack acceptedHistory;
    CompatibilityGraph compatibilityGraph;

    cout << "Welcome to Team Nexus!\n"
         << "Loaded " << students.size() << " students and "
         << groups.size() << " groups.\n";

    int choice = -1;
    do {
        showMenu();
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Please enter a valid menu number.\n";
            continue;
        }

        if (choice == 1) {
            showStudents(students);
        }
        else if (choice == 2) {
            showGroups(groups);
        }
        else if (choice == 3) {
            int studentID;
            if (!readInteger("Enter student ID: ", studentID)) continue;

            int studentPosition = studentIndex.search(studentID);
            if (studentPosition == -1) cout << "Student not found.\n";
            else students[studentPosition].display();
        }
        else if (choice == 4 || choice == 5) {
            int studentID;
            if (!readInteger("Enter student ID: ", studentID)) continue;

            int studentPosition = studentIndex.search(studentID);
            if (studentPosition == -1) {
                cout << "Student not found.\n";
                continue;
            }

            
            compatibilityGraph.build(students, groups);
            if (choice == 4) {
                compatibilityGraph.showBFS(studentPosition, students, groups);
            } else {
                compatibilityGraph.showDFS(studentPosition, students, groups);
            }
        }
        else if (choice == 6) {
            selectionSortGroupsByVacancies(groups);
            rebuildIndexes(students, groups, studentIndex, groupIndex);
            cout << "Groups sorted by vacancies (highest first).\n";
            for (const Group& group : groups) {
                cout << group.getID() << " | " << group.getName()
                     << " | Vacancies: " << group.vacancies() << '\n';
            }
        }
        else if (choice == 7) {
            int studentID, groupID;
            if (!readInteger("Enter student ID: ", studentID)) continue;

            int studentPosition = studentIndex.search(studentID);
            if (studentPosition == -1) {
                cout << "Student not found.\n";
                continue;
            }

            if (findStudentGroup(groups, studentID) != -1) {
                cout << "This student is already a member of a group.\n";
                continue;
            }
            if (pendingRequests.contains(studentID)) {
                cout << "This student already has a pending join request.\n";
                continue;
            }

            if (!readInteger("Enter group ID: ", groupID)) continue;
            int groupPosition = groupIndex.search(groupID);
            if (groupPosition == -1) {
                cout << "Group not found.\n";
                continue;
            }

            Group& selectedGroup = groups[groupPosition];
            if (selectedGroup.vacancies() <= 0) {
                cout << "This group is full.\n";
                continue;
            }

            int match = selectedGroup.compatibility(students[studentPosition]);
            if (match == 0) {
                cout << "No matching skills. You can still choose another group.\n";
                continue;
            }

            JoinRequest request(students[studentPosition], groupID);
            if (pendingRequests.enqueue(request)) {
                cout << "Request added to the queue. Compatibility: "
                     << match << "%\n";
            } else {
                cout << "Request queue is full. Try again later.\n";
            }
        }
        else if (choice == 8) {
            JoinRequest request;
            if (!pendingRequests.dequeue(request)) {
                cout << "There are no pending requests.\n";
                continue;
            }

            cout << request.student.getName() << " wants to join group ID "
                 << request.groupID << ". Accept? (y/n): ";
            char answer;
            cin >> answer;

            if (answer != 'y' && answer != 'Y') {
                cout << "Request rejected.\n";
                continue;
            }

            int groupPosition = groupIndex.search(request.groupID);
            if (groupPosition == -1) {
                cout << "The requested group no longer exists.\n";
                continue;
            }
            if (findStudentGroup(groups, request.student.getID()) != -1) {
                cout << "Request denied: student has already joined another group.\n";
                continue;
            }
            if (acceptedHistory.isFull()) {
                cout << "Undo history is full. Cannot accept another request.\n";
                continue;
            }

            Group& targetGroup = groups[groupPosition];
            if (!targetGroup.addMember(request.student)) {
                cout << "Could not accept: group may be full or student already exists.\n";
                continue;
            }

            
            if (!acceptedHistory.push(request)) {
                targetGroup.removeMember(request.student.getID());
                cout << "Could not store undo record; membership was rolled back.\n";
                continue;
            }

            cout << "Request accepted. " << request.student.getName()
                 << " joined " << targetGroup.getName() << ".\n";
            if (!saveAll(students, groups)) {
                cout << "Warning: could not save all data files.\n";
            }
        }
        else if (choice == 9) {
            JoinRequest lastAccepted;
            if (!acceptedHistory.pop(lastAccepted)) {
                cout << "There is no accepted membership to undo in this session.\n";
                continue;
            }

            int groupPosition = groupIndex.search(lastAccepted.groupID);
            if (groupPosition != -1 &&
                groups[groupPosition].removeMember(lastAccepted.student.getID())) {
                cout << "Undo successful. " << lastAccepted.student.getName()
                     << " has been removed from "
                     << groups[groupPosition].getName() << ".\n";
                if (!saveAll(students, groups)) {
                    cout << "Warning: could not save all data files.\n";
                }
            } else {
                cout << "Could not undo this membership.\n";
            }
        }
        else if (choice == 0) {
            if (saveAll(students, groups)) {
                cout << "All data saved. Goodbye!\n";
            } else {
                cout << "Warning: one or more files could not be saved.\n";
            }
        }
        else {
            cout << "Invalid option. Choose from 0 to 9.\n";
        }

    } while (choice != 0);

    return 0;
}
