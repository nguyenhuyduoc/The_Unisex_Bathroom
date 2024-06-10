#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
// #include "draw.cpp"

struct Person {
    int id;
    std::string type;
    bool is_waiting;
};

std::vector<std::string> read_actions_from_file(const std::string& filename) {
    std::vector<std::string> actions;
    std::ifstream file(filename);
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty()) {
            actions.push_back(line);
        }
    }
    return actions;
}