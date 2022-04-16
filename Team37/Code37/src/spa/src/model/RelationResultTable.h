
#ifndef INC_21S2_CP_SPA_TEAM_37_RELATIONRESULTTABLE_H
#define INC_21S2_CP_SPA_TEAM_37_RELATIONRESULTTABLE_H


#include "ENUM.h"
#include "vector"
#include "algorithm"
#include "QType.h"
#include "RelationResultPair.h"

using namespace std;

class RelationResultTable {
    RelationType type;
    Ref queryingArgument1;
    Ref queryingArgument2;
    vector<RelationResultPair> list;
public:
    RelationResultTable();
    RelationResultTable(RelationType typeIn, Ref queryingArg1, Ref queryingArg2, vector<RelationResultPair> listIn);
    RelationResultTable(RelationType typeIn, Ref queryingArg1, Ref queryingArg2);
    void add(RelationResultPair newData);
    vector<RelationResultPair>* getList();
    Ref* getQueryingArg1();
    Ref* getQueryingArg2();
    bool containsArg1(Ref* arg);
    bool containsArg1JustValue(Ref* arg);
    bool containsArg2(Ref* arg);
    bool containsArg2JustValue(Ref* arg);
    bool containsPair(RelationResultPair* pair);
    bool equalsArg1(Ref* checkingSelected);
    bool equalsArg2(Ref* checkingSelected);
    int size();
    string ToString();
};


#endif //INC_21S2_CP_SPA_TEAM_37_RELATIONRESULTTABLE_H
