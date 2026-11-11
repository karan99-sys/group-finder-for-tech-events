// All text-file handling is kept in this ONE implementation file.
// Function declarations are placed in DSA.h to avoid creating another header.
#include "DSA.h"
#include <fstream>
#include <sstream>
#include <string>

namespace CampusApp {

bool loadStudents(const std::string& filename, Student students[],
                  int& count, int capacity) {
    std::ifstream file(filename.c_str());
    if (!file) return false;

    count = 0;
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        if (count >= capacity) break;

        std::stringstream row(line);
        std::string idText, name, skill1, skill2;
        if (!std::getline(row, idText, '|') ||
            !std::getline(row, name, '|') ||
            !std::getline(row, skill1, '|') ||
            !std::getline(row, skill2)) continue;

        try {
            int id = std::stoi(idText);
            if (id <= 0) continue;
            students[count] = Student(id, name, skill1, skill2);
            ++count;
        } catch (...) {
            // Skip malformed lines rather than stopping the complete load.
        }
    }
    return true;
}

bool loadGroups(const std::string& filename, Group groups[],
                int& count, int capacity) {
    std::ifstream file(filename.c_str());
    if (!file) return false;

    count = 0;
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        if (count >= capacity) break;

        std::stringstream row(line);
        std::string idText, name, skill1, skill2;
        if (!std::getline(row, idText, '|') ||
            !std::getline(row, name, '|') ||
            !std::getline(row, skill1, '|') ||
            !std::getline(row, skill2)) continue;

        try {
            int id = std::stoi(idText);
            if (id <= 0) continue;
            groups[count] = Group(id, name, skill1, skill2);
            ++count;
        } catch (...) {
            // Skip malformed lines.
        }
    }
    return true;
}

bool saveStudents(const std::string& filename, const Student students[], int count) {
    std::ofstream file(filename.c_str());
    if (!file) return false;
    for (int i = 0; i < count; ++i) {
        file << students[i].getID() << '|'
             << students[i].getName() << '|'
             << students[i].getSkill1() << '|'
             << students[i].getSkill2() << '\n';
    }
    return true;
}

bool saveGroups(const std::string& filename, const Group groups[], int count) {
    std::ofstream file(filename.c_str());
    if (!file) return false;
    for (int i = 0; i < count; ++i) {
        file << groups[i].getID() << '|'
             << groups[i].getName() << '|'
             << groups[i].getRequiredSkill1() << '|'
             << groups[i].getRequiredSkill2() << '\n';
    }
    return true;
}

} // namespace CampusApp
