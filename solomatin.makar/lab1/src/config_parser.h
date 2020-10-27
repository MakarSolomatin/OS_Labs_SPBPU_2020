#ifndef CONFIG_PARSER_H
#define CONFIG_PARSER_H

#include <map>
#include <vector>
#include <string>

using namespace std;
class ConfigParser {
public:
    enum class Token { DIRECTORY }; // grammar token
    void parse(const string &file_name); // parse config file, throws exceptions
    const vector<string> get(Token key); // returns null if key not exist

private:
    static map<string, Token> grammar; // grammar of config file
    static map<Token, vector<string>> defaults;
    map<Token, vector<string>> parameters;

    string fieldName(Token token) {
        for (auto const &entry : grammar) {
            if (entry.second == token) return entry.first;
        }
        return "undefined field";
    }
};

#endif /*CONFIG_PARSER_H*/
