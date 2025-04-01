#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "skip_list.h"

class CommandParser {
public:
    static void ParseCommand(const std::string& input, std::vector<std::string>& tokens) {
        tokens.clear();
        std::istringstream iss(input);
        std::string token;
        while (iss >> token) {
            tokens.push_back(token);
        }
    }
};

class OvOStore {
private:
    SkipList<std::string, std::less<>> skipList;

public:
    bool Set(const std::string& value, const std::string& key) {
        std::string combined = key + ":" + value;
        return skipList.Insert(combined);
    }

    bool Get(const std::string& key, std::string* value) {
        std::string combined;
        std::vector<std::string> result;
        skipList.GetRange(key + " ", key + "~", &result);
        if (!result.empty()) {
            size_t space_pos = result[0].find(':');
            if (space_pos != std::string::npos) {
                *value = result[0].substr(space_pos + 1);
                return true;
            }
        }
        return false;
    }

    bool Del(const std::string& key) {
        std::vector<std::string> result;
        skipList.GetRange(key + " ", key + "~", &result);
        if (!result.empty()) {
            return skipList.Remove(result[0]);
        }
        return false;
    }

    void Range(const std::string& lower, const std::string& upper, std::vector<std::pair<std::string, std::string>>* result) {
        std::vector<std::string> combined_result;
        skipList.GetRange(lower + " ", upper + "~", &combined_result);
        result->clear();
        for (const auto& combined : combined_result) {
            size_t space_pos = combined.find(':');
            if (space_pos != std::string::npos) {
                result->emplace_back(combined.substr(0, space_pos), combined.substr(space_pos + 1));
            }
        }
    }
};

int main() {
    OvOStore redis;
    std::string input;
    std::cout << "Welcome to OvOStore\n";
    std::cout << ">>> ";

    while (std::getline(std::cin, input)) {
        std::vector<std::string> tokens;
        CommandParser::ParseCommand(input, tokens);

        if (tokens.empty()) {
            std::cout << ">>> ";
            continue;
        }

        std::string command = tokens[0];
        std::transform(command.begin(), command.end(), command.begin(), ::toupper);

        if (command == "SET") {
            if (tokens.size() < 3) {
                std::cout << "ERROR: SET command requires a key and a value\n";
            } else {
                std::string value = tokens[1];
                std::string key;
                for (size_t i = 2; i < tokens.size(); ++i) {
                    if (i > 2) key += " ";
                    key += tokens[i];
                }
                if (redis.Set(value, key)) {
                    std::cout << "OK\n";
                } else {
                    std::cout << "ERROR: Key already exists\n";
                }
            }
        } else if (command == "GET") {
            if (tokens.size() != 2) {
                std::cout << "ERROR: GET command requires a key\n";
            } else {
                std::string value;
                if (redis.Get(tokens[1], &value)) {
                    std::cout << value << "\n";
                } else {
                    std::cout << "nil\n";
                }
            }
        } else if (command == "DEL") {
            if (tokens.size() != 2) {
                std::cout << "ERROR: DEL command requires a key\n";
            } else {
                if (redis.Del(tokens[1])) {
                    std::cout << "OK\n";
                } else {
                    std::cout << "ERROR: Key not found\n";
                }
            }
        } else if (command == "RANGE") {
            if (tokens.size() != 3) {
                std::cout << "ERROR: RANGE command requires lower and upper bounds\n";
            } else {
                std::vector<std::pair<std::string, std::string>> result;
                redis.Range(tokens[1], tokens[2], &result);
                std::cout << "Found " << result.size() << " key-value pairs:\n";
                for (const auto& pair : result) {
                    std::cout << pair.first << " -> " << pair.second << "\n";
                }
            }
        } else if (command == "EXIT") {
            break;
        } else {
            std::cout << "ERROR: Unknown command\n";
        }

        std::cout << ">>> ";
    }

    std::cout << "Goodbye!\n";
    return 0;
}