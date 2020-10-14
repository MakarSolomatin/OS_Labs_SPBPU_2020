#include <map>
#include <syslog.h>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include "config_parser.h"

using namespace std;
map<string, ConfigParser::Token> ConfigParser::grammar = {
    {"diretory", Token::DIRECTORY}
};

bool ConfigParser::parse(const string &file_name) {
    ifstream fin(file_name);

    if (!fin.is_open()) {
        syslog(LOG_INFO, "Could not open %s file", file_name.c_str());
        return false;
    }

    string line;
    while (getline(fin, line)) {
        istringstream iss(line);

        vector<string> words;
        string word;
        while (iss >> word) words.push_back(word);

        if (words.size() == 0 || words.at(0).at(0) == '#') continue;

        //parse logic here
        if (words.at(0) == "directory") {
            if (words.size() != 2) {
                syslog(LOG_INFO, "Error while parsing %s: 'directory' has exactly one argument", file_name.c_str());
                fin.close();
                return false;
            }

            parameters[Token::DIRECTORY] = words.at(1);
        }
    }

    string parametersString;
    for (auto const &entry : parameters) {
        parametersString += (fieldName(entry.first) + ": " + entry.second + ";").c_str();
    }

    syslog(LOG_INFO, "Parsed %s file with parameters: %s", file_name.c_str(), parametersString.c_str());
    fin.close();
    return true;
}

const char * ConfigParser::get(Token key) {
    auto iter = parameters.find(key);
    if (iter == parameters.end()) return nullptr;

    return iter->second.c_str();
}
