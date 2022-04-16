#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include<set>
#include "StringTokenizer.h"


using namespace std;

StringTokenizer::StringTokenizer(string s) {
    this -> s = s;
}

StringTokenizer::~StringTokenizer() {
}

bool StringTokenizer::TokenizeString(vector<string> &token) {
    stringstream sstream(this -> s); //convert my_string into string stream
    stringstream cur;
    while (sstream.rdbuf()->in_avail() != 0) {
        char c = sstream.get();
        if (IsIgnoredSymbol(c)) {
            if (!cur.str().empty()) {
                token.push_back(cur.str());
                cur.str("");
            }
        }
        else if (IsAcceptedSymbol(c)) {
            if (!cur.str().empty()) {
                token.push_back(cur.str());
                cur.str("");
            }
            token.push_back(string(1, c));
        }
        else {
            cur << c;
        }
    }
    if (!cur.str().empty()) {
        token.push_back(cur.str());
    }

    return true;
}

void StringTokenizer::PrintString(vector<string> &token) {
    for(int i = 0; i < token.size(); i++) {
        cout << token[i] << endl;
    }
}

string StringTokenizer::GetString() {
    return this -> s;
}

StringTokenizer::StringTokenizer() {}

bool StringTokenizer::IsAcceptedSymbol(char c) {
    return accepted_symbols.find(c) != accepted_symbols.end();
}

bool StringTokenizer::IsIgnoredSymbol(char c) {
    return ignored_symbols.find(c) != ignored_symbols.end();
}
