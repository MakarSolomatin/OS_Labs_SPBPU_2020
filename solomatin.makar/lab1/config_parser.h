#ifndef CONFIG_PARSER_H
#define CONFIG_PARSER_H

#include <map>
#include <string>

using namespace std;
// class responsible for parsing config file
class ConfigParser {
    map<string, string> parameters;
    static map<string, string> grammar; // grammar of config file

public:
    bool parse(const string &file_name); // parse config file
    const char * get(const string &key); // returns null if key not exist
};

#endif /*CONFIG_PARSER_H*/
