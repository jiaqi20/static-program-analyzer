#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

#include "StatementNumber.h"

StatementNumber::StatementNumber(int line) {
    this->line = line;
}

StatementNumber::~StatementNumber() {

}

int StatementNumber::GetLine() const {
    return this->line;
}

bool StatementNumber::operator==(const StatementNumber& sn) const {
    return line == sn.line;
};

bool StatementNumber::operator<(const StatementNumber& sn) const {
    return line < sn.line;
}

size_t StatementNumber::HashCode() {
    return hash<int>()(this->line);
}

void StatementNumber::ToString() {
    cout << "Statement number is: " << this->line << endl;
}
