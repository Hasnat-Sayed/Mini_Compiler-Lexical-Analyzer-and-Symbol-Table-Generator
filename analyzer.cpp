#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <cctype>
#include <sstream>
#include <iomanip>
#include <algorithm>
using namespace std;

struct Symbol {
    string name, type;
    string address;
};

vector<string> keywords = {
    "int", "float", "char", "double", "if", "else", "for", "while", "do",
    "switch", "case", "return", "void", "static", "const", "break"
};

vector<string> operators = {
    "+", "-", "*", "/", "%", "=", "==", "!=", ">", "<", ">=", "<=", "&&", "||"
};

unordered_map<string, Symbol> symbolTable;
unordered_set<string> printed;
int addressCounter = 4096;

bool isKeyword(const string& s) {
    return find(keywords.begin(), keywords.end(), s) != keywords.end();
}

bool isValidIdentifier(const string& s) {
    if (!isalpha(s[0]) && s[0] != '_') return false;
    for (char c : s)
        if (!isalnum(c) && c != '_') return false;
    return true;
}

bool isOperator(const string& s) {
    return find(operators.begin(), operators.end(), s) != operators.end();
}

string escapeJSON(const string& s) {
    string out;
    for (char c : s) {
        if (c == '\\') out += "\\\\";
        else if (c == '"') out += "\\\"";
        else if (c == '\n') out += "\\n";
        else if (c == '\r') out += "\\r";
        else if (c == '\t') out += "\\t";
        else out += c;
    }
    return out;
}

string removeComments(const string& code) {
    string result;
    bool single = false, multi = false;
    for (size_t i = 0; i < code.size(); i++) {
        if (single && code[i] == '\n') single = false, result += '\n';
        else if (multi && code[i] == '*' && code[i + 1] == '/') multi = false, i++;
        else if (!single && !multi && code[i] == '/' && code[i + 1] == '/') single = true, i++;
        else if (!single && !multi && code[i] == '/' && code[i + 1] == '*') multi = true, i++;
        else if (!single && !multi) result += code[i];
    }
    return result;
}

string toJSON(const string& code) {
    string token, output;
    bool inString = false;
    vector<pair<string, string>> tokens;
    vector<Symbol> symbols;
    istringstream stream(code);
    string line, full;
    while (getline(stream, line)) full += line + '\n';

    for (size_t i = 0; i < full.size(); i++) {
        char ch = full[i];

        if (ch == '"') {
            inString = !inString;
            token += ch;
            if (!inString) {
                tokens.push_back({token, "string"});
                printed.insert(token);
                token.clear();
            }
            continue;
        }

        if (inString) {
            token += ch;
            continue;
        }

        if (isdigit(ch)) {
            token += ch;
            while (i + 1 < full.size() && (isdigit(full[i + 1]) || full[i + 1] == '.')) {
                if (full[i + 1] == '.' && token.find('.') != string::npos) break;
                token += full[++i];
            }
            tokens.push_back({token, "number"});
            printed.insert(token);
            token.clear();
            continue;
        }

        if (isspace(ch) || string(";,(){}[]").find(ch) != string::npos) {
            if (!token.empty()) {
                if (isKeyword(token)) tokens.push_back({token, "keyword"});
                else if (isValidIdentifier(token)) {
                    size_t j = i;
                    while (j < full.size() && isspace(full[j])) j++;
                    if (j < full.size() && full[j] == '(') tokens.push_back({token, "function"});
                    else {
                        tokens.push_back({token, "identifier"});
                        if (!symbolTable.count(token)) {
                            symbolTable[token] = {token, "identifier", "0x" + to_string(addressCounter)};
                            addressCounter += 16;
                        }
                    }
                } else tokens.push_back({token, "unknown"});
                printed.insert(token);
                token.clear();
            }
            if (!isspace(ch)) {
                string sym(1, ch);
                tokens.push_back({sym, "symbol"});
                printed.insert(sym);
            }
            continue;
        }

        if (ispunct(ch)) {
            string op(1, ch);
            if (i + 1 < full.size() && isOperator(op + full[i + 1])) {
                op += full[++i];
                tokens.push_back({op, "operator"});
            } else if (isOperator(op)) tokens.push_back({op, "operator"});
            else token += ch;
            printed.insert(op);
            continue;
        }

        token += ch;
    }

    if (!token.empty()) {
        if (isKeyword(token)) tokens.push_back({token, "keyword"});
        else if (isValidIdentifier(token)) {
            tokens.push_back({token, "identifier"});
            if (!symbolTable.count(token))
                symbolTable[token] = {token, "identifier", "0x" + to_string(addressCounter)};
        } else tokens.push_back({token, "unknown"});
        printed.insert(token);
    }

    output = "{\n  \"tokens\": [\n";
    for (size_t i = 0; i < tokens.size(); i++) {
        auto [val, type] = tokens[i];
        output += "    { \"value\": \"" + escapeJSON(val) + "\", \"type\": \"" + type + "\" }";
        if (i + 1 < tokens.size()) output += ",";
        output += "\n";
    }
    output += "  ],\n  \"symbolTable\": [\n";
    size_t i = 0;
    for (auto& [name, sym] : symbolTable) {
        output += "    { \"name\": \"" + escapeJSON(sym.name) + "\", \"type\": \"" + escapeJSON(sym.type) + "\", \"address\": \"" + sym.address + "\" }";
        if (++i < symbolTable.size()) output += ",";
        output += "\n";
    }
    output += "  ]\n}";
    return output;
}

int main() {
    string input, line;
    while (getline(cin, line)) input += line + '\n';
    input = removeComments(input);
    cout << toJSON(input) << endl;
    return 0;
}
