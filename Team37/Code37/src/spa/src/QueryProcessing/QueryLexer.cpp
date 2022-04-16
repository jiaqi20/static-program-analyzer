
#include <string>
#include <vector>
#include<set>
#include "QueryLexer.h"
#include "StringTokenizer.h"


using namespace std;

QueryLexer::QueryLexer(string s) {
    this -> s = s;
    this -> tok = StringTokenizer(s);
    bool tokenize = tok.TokenizeString(this->string_token);
    valid_tokenized_string = tokenize;
    this-> current_position = string_token.begin();
    InitEntityType();
    InitDelims();
    InitCondition();
    InitRelation();
    InitBracket();
    InitStringSymbol();
    InitConnector();
    InitClauseSymbol();
}

QueryLexer::~QueryLexer() {
}

bool QueryLexer::IsValidTokenized() {
    return valid_tokenized_string;
}

void QueryLexer::InitClauseSymbol() {
    this->clauses.insert("Select");
    this->clauses.insert("such");
    this->clauses.insert("pattern");
    this->clauses.insert("with");
}

bool QueryLexer::MatchClauses(string c) {
    if (IsEnd()) {
        return false;
    }
    if (this->clauses.find(*this->current_position) != this->clauses.end()) {
        return *this->current_position == c;
    } else {
        return false;
    }
}

string QueryLexer::EatClauses(string s) {
    if (!MatchClauses(s)) {
        return "bad syntax: clauses not found";
    } else {
        NextItem();
    }
    return "clauses eaten";
}

bool QueryLexer::MatchConnector() {
    if (IsEnd()) {
        return false;
    }
    if (this->connector.find(*this->current_position) != this->connector.end()) {
        return true;
    } else {
        return false;
    }
}

string QueryLexer::EatConnector() {
    if (!MatchConnector()) {
        return "bad syntax: connector and not found";
    } else {
        NextItem();
    }
    return "connector and eaten";
}

void QueryLexer::InitConnector() {
    this->connector.insert("and");
}

void QueryLexer::InitEntityType() {
    this->entity_type.insert("procedure");
    this->entity_type.insert("stmt");
    this->entity_type.insert("stmtLst");
    this->entity_type.insert("read");
    this->entity_type.insert("print");
    this->entity_type.insert("assign");
    this->entity_type.insert("call");
    this->entity_type.insert("while");
    this->entity_type.insert("if");
    this->entity_type.insert("variable");
    this->entity_type.insert("constant");
}

string QueryLexer::PrintEntityType() {
    string s = "The keywords are: ";
    for (set<string> ::iterator it1 = entity_type.begin(); it1 != entity_type.end(); it1++) {
        s = s + *it1 + ", ";
    }
    return s + " ";
}

QueryLexer::QueryLexer() {}

bool QueryLexer::MatchEntityType(string inputKeyword) {
    if (IsEnd()) {
        return false;
    }
    set<string>::iterator pos = this->entity_type.find(inputKeyword);
    if (pos != this->entity_type.end()) {
        return *this->current_position == inputKeyword;
    } else {
        return false;
    }
}

string QueryLexer::NextItem() {
    if (this->current_position == this->string_token.end()) {
        return "it is the end";
    } else {
        current_position++;
    }
    return "proceed to next";
}

string QueryLexer::EatEntityType(string input) {
    if (!MatchEntityType(input)) {
        return "bad syntax: entity type not matched";
    } else {
        NextItem();
    }
    return "entity type eaten";
}

string QueryLexer::PrintToken() {
    string s = "Current tokens are: ";
    for (vector<string> ::iterator it1 = current_position; it1 != string_token.end(); it1++) {
        s = s + *it1 + ", ";
    }
    s = s + " ";
    return s;
}

string QueryLexer::EatVariable() {
    if (this->current_position == this->string_token.end()) {
        return "";
    } else {
        string var = *this->current_position;
        NextItem();
        return var;
    }
}

vector<string>& QueryLexer::GetStringToken() {
    return this->string_token;
}

vector<string> ::iterator QueryLexer::GetCurrentPosition() {
    return current_position;
}


void QueryLexer::InitDelims() {
    this->delims.insert(";");
    this->delims.insert(",");
    this->delims.insert(".");
    this->delims.insert("=");
}

string QueryLexer::PrintDelims() {
    string s = "The delimiters are: ";
    for (set<string> ::iterator it1 = delims.begin(); it1 != delims.end(); it1++) {
        s = s + *it1 + ", ";
    }
    s = s + " ";
    return s;
}

bool QueryLexer::MatchDelims(string inputDelims) {
    if (IsEnd()) {
        return false;
    }
    if (this->delims.find(inputDelims) != this->delims.end()) {
        return *this->current_position == inputDelims;
    } else {
        return false;
    }
}

string QueryLexer::EatDelim(string inputDelim) {
    if (!MatchDelims(inputDelim)) {
        return "bad syntax: delim not matched";
    } else {
    }
    return "delim eaten";
}

void QueryLexer::InitStringSymbol() {
    this->string_symbol.insert("\"");
    this->string_symbol.insert("\'");
}

string QueryLexer::PrintStringSymbol() {
    string s = "The delimiters are: " ;
    for (set<string> ::iterator it1 = string_symbol.begin(); it1 != string_symbol.end(); it1++) {
        s = s + *it1 + ", ";
    }
    s = s + " ";
    return s;
}

bool QueryLexer::MatchString(string s) {
    if (IsEnd()) {
        return false;
    }
    if (this->string_symbol.find(s) != this->string_symbol.end()) {
        return *this->current_position == s;
    } else {
        return false;
    }
}

string QueryLexer::EatString(string s) {
    if (!MatchString(s)) {
        return "bad syntax: delim not matched";
    } else {
        NextItem();
    }
    return "string eaten";
}

void QueryLexer::InitCondition() {
    this->condition_symbol.insert("that");
}

string QueryLexer::PrintCondition() {
    string s = "The conditions symbol are: " ;
    for (set<string> ::iterator it1 = condition_symbol.begin(); it1 != condition_symbol.end(); it1++) {
        s = s + *it1 + ", ";
    }
    return s + " ";
}

bool QueryLexer::MatchCondition(string condition) {
    if (IsEnd()) {
        return false;
    }
    if (this->condition_symbol.find(*this->current_position) != this->condition_symbol.end()) {
        return *this->current_position == condition;
    } else {
        return false;
    }
}

string QueryLexer::EatCondition(string condition) {
    if (!MatchCondition(condition)) {
        return "bad syntax: condition not matched";
    } else {
        NextItem();
    }
    return "condition eaten";
}

void QueryLexer::InitRelation() {
    this->relation.insert("Modifies");
    this->relation.insert("Uses");
    this->relation.insert("Parent");
    this->relation.insert("Parent*");
    this->relation.insert("Follows");
    this->relation.insert("Follows*");
    this->relation.insert("Calls");
    this->relation.insert("Calls*");
    this->relation.insert("Next");
    this->relation.insert("Next*");
    this->relation.insert("Affects");
    this->relation.insert("Affects*");
}

string QueryLexer::PrintRelation() {
    string s = "The relations are: " ;
    for (set<string> ::iterator it1 = relation.begin(); it1 != relation.end(); it1++) {
        s = s + *it1 + ", ";
    }
    return s + " ";
}

bool QueryLexer::MatchRelation(string re) {
    if (IsEnd()) {
        return false;
    }
    if (this->relation.find(*this->current_position) != this->relation.end()) {
        return *this->current_position == re;
    } else {
        return false;
    }
}

string QueryLexer::EatRelation(string re) {
    if (!MatchRelation(re)) {
        return "bad syntax: relation not found";
    } else {
        string relationship = *this->current_position;
        NextItem();
        return relationship;
    }
}

void QueryLexer::InitBracket() {
    this->bracket.insert("(");
    this->bracket.insert(")");
    this->bracket.insert(",");
    this->bracket.insert("<");
    this->bracket.insert(">");
}

string QueryLexer::PrintBracket() {
    string s = "The brackets are: " ;
    for (set<string> ::iterator it1 = bracket.begin(); it1 != bracket.end(); it1++) {
        s = s + *it1 + ", ";
    }
    return s + " ";
}

bool QueryLexer::MatchBracket(string b) {
    if (IsEnd()) {
        return false;
    }
    if (this->bracket.find(*this->current_position) != this->bracket.end()) {
        return *this->current_position == b;
    } else {
        return false;
    }
}

string QueryLexer::EatBracket(string b) {
    if (!MatchBracket(b)) {
        return "bad syntax: relation not found";
    } else {
        if (b != ")") {
            NextItem();
        }
    }
    return "bracket eaten";
}

bool QueryLexer::MatchWildCard(string wildcard) {
    if (IsEnd()) {
        return false;
    }
    return wildcard == "_";
}

bool QueryLexer::IsEnd() {
    return this->current_position == string_token.end();
}