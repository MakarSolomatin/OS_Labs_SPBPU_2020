#ifndef CONFIG_PARSER_H
#define CONFIG_PARSER_H

#include <map>
#include <string>

using namespace std;
// class responsible for parsing config file
class ConfigParser {
public:
    enum class Token { DIRECTORY }; // grammar token
    bool parse(const string &file_name); // parse config file
    const char * get(Token key); // returns null if key not exist

private:
    static map<string, Token> grammar; // grammar of config file
    map<Token, string> parameters;

    string fieldName(Token token) {
        for (auto const &entry : grammar) {
            if (entry.second == token) return entry.first;
        }
        return "undefined field";
    }
};

#endif /*CONFIG_PARSER_H*/
