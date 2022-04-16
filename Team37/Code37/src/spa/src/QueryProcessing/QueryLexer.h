#ifndef INC_21S2_CP_SPA_TEAM_37_QueryLexer_H
#define INC_21S2_CP_SPA_TEAM_37_QueryLexer_H
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <unordered_map>
#include <functional>
#include "StringTokenizer.h"


using namespace std;

class QueryLexer {

private:
    string s;
    StringTokenizer tok;
    set<string> entity_type;
    set<string> delims;
    set<string> select;
    set<string> pattern;
    set<string> condition_symbol;
    set<string> relation;
    set<string> bracket;
    set<string> clauses;
    set<string> connector;
    vector<string> string_token;
    set<string> string_symbol;
    vector<string> ::iterator current_position;
    bool valid_tokenized_string = true;

public:
    QueryLexer(string s);
    QueryLexer();
    ~QueryLexer();

    void InitEntityType();
    void InitStringSymbol();
    void InitClauseSymbol();
    void InitDelims();
    void InitCondition();
    void InitBracket();
    void InitRelation();
    void InitConnector();
    bool MatchEntityType(string inputKeyword);
    bool MatchDelims(string inputDelims);
    bool MatchCondition(string condition);
    bool MatchRelation(string re);
    bool MatchBracket(string b);
    bool MatchString(string s);
    bool MatchClauses(string c);
    bool MatchConnector();
    bool MatchWildCard(string wildcard);
    bool IsEnd();
    bool IsValidTokenized();
    string EatEntityType(string input);
    string EatDelim(string inputDelim);
    string EatCondition(string condition);
    string EatBracket(string b);
    string EatString(string s);
    string EatClauses(string c);
    string EatConnector();
    string EatRelation(string re);
    string NextItem();
    string EatVariable();
    string PrintRelation();
    string PrintEntityType();
    string PrintCondition();
    string PrintDelims();
    string PrintToken();
    string PrintBracket();
    string PrintStringSymbol();
    vector<string>& GetStringToken();
    vector<string> ::iterator GetCurrentPosition();
};

#endif //INC_21S2_CP_SPA_TEAM_37_QueryLexer_H