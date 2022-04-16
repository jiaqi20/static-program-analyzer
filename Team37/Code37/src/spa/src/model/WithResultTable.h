
#ifndef SPA_WITHRESULTTABLE_H
#define SPA_WITHRESULTTABLE_H

#include <string>
#include "QType.h"

using namespace std;

class WithResultTable {
    Ref lhs;
    Ref rhs;
    vector<pair<Ref, Ref>> list;
public:
    WithResultTable();
    WithResultTable(Ref LHS, Ref RHS);
    WithResultTable(Ref LHS, Ref RHS, vector<pair<Ref, Ref>> listIn);
    Ref* getLHS();
    Ref* getRHS();
    vector<pair<Ref, Ref>>* getList();
    int size();
    string ToString();
};


#endif //SPA_WITHRESULTTABLE_H
