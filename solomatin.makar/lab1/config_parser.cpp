#include <map>
#include <syslog.h>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include "config_parser.h"

using namespace std;
map<string, string> ConfigParser::grammar = {
    {}
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
    }

    string parametersString;
    for (auto const &entry : parameters) {
        parametersString += (entry.first + ": " + entry.second + ";").c_str();
    }

    syslog(LOG_INFO, "Parsed %s file with parameters: %s", file_name.c_str(), parametersString.c_str());
    return true;
}

const char * ConfigParser::get(const string &key) {
    auto iter = parameters.find(key);
    if (iter == parameters.end()) return nullptr;

    return iter->second.c_str();
}
