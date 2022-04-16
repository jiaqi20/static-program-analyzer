
#ifndef INC_21S2_CP_SPA_TEAM_37_PATTERNRESULTTABLE_H
#define INC_21S2_CP_SPA_TEAM_37_PATTERNRESULTTABLE_H

#include "ENUM.h"
#include "vector"
#include "QType.h"
#include "PatternResultElement.h"

using namespace std;

class PatternResultTable {
    Ref querying_arg1;
    Ref querying_arg2;
    ExpressionSpec querying_expression;
    vector<PatternResultElement> list;
public:
    PatternResultTable();
    PatternResultTable(Ref queryingAssignIn, Ref queryingLHSIn, vector<PatternResultElement> listIn);
    PatternResultTable(Ref queryingAssignIn, Ref queryingLHSIn);
    void add(PatternResultElement newData);
    vector<PatternResultElement>* getList();
    Ref* getQueryingAssign();
    Ref* getQueryingLHS();
    ExpressionSpec* getQueryingExpression();
    bool ContainsAssignJustValue(Ref* checkingAssign);
    bool containsAssignJustValue(Ref* checkingAssign);
    bool ContainsLHS(Ref* checkingLHS);
    bool ContainsElement(PatternResultElement* element);
    bool EqualsAssign(Ref* checking);
    bool EqualsLHS(Ref* checking);
    bool EqualsExpression(ExpressionSpec* checking);
    int size();
    string ToString();
};


#endif //INC_21S2_CP_SPA_TEAM_37_PATTERNRESULTTABLE_H
