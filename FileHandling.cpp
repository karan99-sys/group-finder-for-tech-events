#include "DSA.h"

#include <fstream>
#include <sstream>
#include <unordered_map>

namespace CampusApp {

bool loadStudents(const std::string& fileName, std::vector<Student>& students) {
    std::ifstream input(fileName);
    if (!input) return false;

    students.clear();
    std::string line;

    while (std::getline(input, line)) {
        if (line.empty()) continue;

        std::stringstream row(line);
        std::string id, name, skill1, skill2;
        if (!std::getline(row, id, '|') || !std::getline(row, name, '|') ||
            !std::getline(row, skill1, '|') || !std::getline(row, skill2)) {
            continue;
        }

        try {
            students.emplace_back(std::stoi(id), name, skill1, skill2);
        } catch (...) {
            
        }
    }
    return true;
}

bool loadGroups(const std::string& fileName, std::vector<Group>& groups) {
    std::ifstream input(fileName);
    if (!input) return false;

    groups.clear();
    std::string line;

    while (std::getline(input, line)) {
        if (line.empty()) continue;

        std::stringstream row(line);
        std::string id, name, skill1, skill2;
        if (!std::getline(row, id, '|') || !std::getline(row, name, '|') ||
            !std::getline(row, skill1, '|') || !std::getline(row, skill2)) {
            continue;
        }

        try {
            groups.emplace_back(std::stoi(id), name, skill1, skill2, 5);
        } catch (...) {
            
        }
    }
    return true;
}

bool loadMemberships(const std::string& fileName,
                     const std::vector<Student>& students,
                     std::vector<Group>& groups) {
    std::ifstream input(fileName);
    if (!input) return true; 

    std::unordered_map<int, const Student*> studentByID;
    for (const Student& student : students) {
        studentByID[student.getID()] = &student;
    }

    std::string line;
    while (std::getline(input, line)) {
        if (line.empty()) continue;

        std::stringstream row(line);
        std::string groupText, studentText;
        if (!std::getline(row, groupText, '|') || !std::getline(row, studentText)) continue;

        try {
            int groupID = std::stoi(groupText);
            int studentID = std::stoi(studentText);
            auto studentIt = studentByID.find(studentID);
            if (studentIt == studentByID.end()) continue;

            for (Group& group : groups) {
                if (group.getID() == groupID) {
                    
                    if (findStudentGroup(groups, studentID) == -1) {
                        group.addMember(*studentIt->second);
                    }
                    break;
                }
            }
        } catch (...) {
            
        }
    }
    return true;
}

bool saveStudents(const std::string& fileName, const std::vector<Student>& students) {
    std::ofstream output(fileName);
    if (!output) return false;

    for (const Student& student : students) {
        output << student.getID() << '|'
               << student.getName() << '|'
               << student.getSkill1() << '|'
               << student.getSkill2() << '\n';
    }
    return true;
}

bool saveGroups(const std::string& fileName, const std::vector<Group>& groups) {
    std::ofstream output(fileName);
    if (!output) return false;

    for (const Group& group : groups) {
        output << group.getID() << '|'
               << group.getName() << '|'
               << group.getRequiredSkill1() << '|'
               << group.getRequiredSkill2() << '\n';
    }
    return true;
}

bool saveMemberships(const std::string& fileName, const std::vector<Group>& groups) {
    std::ofstream output(fileName);
    if (!output) return false;

    for (const Group& group : groups) {
        group.writeMemberIDs(output);
    }
    return true;
}

} 
