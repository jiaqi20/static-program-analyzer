#ifndef INC_21S2_CP_SPA_TEAM_37_1_STRINGTOKENIZER_H
#define INC_21S2_CP_SPA_TEAM_37_1_STRINGTOKENIZER_H
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include<set>
#include<unordered_map>
#include <functional>



using namespace std;

class StringTokenizer {
private:
    string s;
    set<char> accepted_symbols{',', ';', '(', ')', '<', '>', '"', '+', '-', '*', '/', '_', '%', '.', '='};
    set<char> ignored_symbols{' ', '\n', '\r', '\t'};
    bool IsAcceptedSymbol(char c);
    bool IsIgnoredSymbol(char c);

public:
    StringTokenizer(string s);

    StringTokenizer();

    ~StringTokenizer();
    bool TokenizeString(vector<string> &token);
    void PrintString(vector<string> &token);
    string GetString();
};

#endif //INC_21S2_CP_SPA_TEAM_37_1_STRINGTOKENIZER_H