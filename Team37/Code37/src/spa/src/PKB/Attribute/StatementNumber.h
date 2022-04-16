#ifndef INC_21S2_CP_SPA_TEAM_37_STATEMENTNUMBER_H
#define INC_21S2_CP_SPA_TEAM_37_STATEMENTNUMBER_H

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class StatementNumber {
private:
    int line;
public:
    StatementNumber(int line);
    ~StatementNumber();
    int GetLine() const;
    bool operator==(const StatementNumber & sn) const;
    bool operator<(const StatementNumber& sn) const;
    size_t HashCode();
    void ToString();
};

struct hashStatementNumber {
public:
    size_t operator() (const StatementNumber& s) const {
        return hash<int>()(s.GetLine());
    }
};

#endif //INC_21S2_CP_SPA_TEAM_37_STATEMENTNUMBER_H