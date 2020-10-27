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
map<ConfigParser::Token, vector<string>> ConfigParser::defaults = {
    {Token::DIRECTORY, {getenv("HOME")}}
};

void ConfigParser::parse(const string &file_name) {
    ifstream fin(file_name);

    if (!fin.is_open()) {
        syslog(LOG_INFO, "Could not open config file %s", file_name.c_str());
        throw "Error while loading config";
    }

    string line;
    while (getline(fin, line)) {
        istringstream iss(line);

        vector<string> words;
        string word;
        while (iss >> word) words.push_back(word);

        if (words.size() == 0 || words.at(0).at(0) == '#') continue;

        //parse logic here
        Token token = grammar.at(words.at(0));
        switch (token) {
        case Token::DIRECTORY:
            if (words.size() != 2) {
                syslog(LOG_INFO, "Error while parsing config file %s: 'directory' has exactly one argument", file_name.c_str());
                fin.close();
                throw "Error while parsing config";
            }
            break;
        }
    }

    syslog(LOG_INFO, "Config file %s parsed successfully", file_name.c_str());
    fin.close();
}

const vector<string> ConfigParser::get(Token key) {
    auto iter = parameters.find(key);
    if (iter == parameters.end()) {
        iter = defaults.find(key);

        if (iter == defaults.end()) throw "No such key in config parser parameters";
    }

    return iter->second;
}