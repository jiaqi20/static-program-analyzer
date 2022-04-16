
#ifndef INC_21S2_CP_SPA_TEAM_37_PATTERNRESULTELEMENT_H
#define INC_21S2_CP_SPA_TEAM_37_PATTERNRESULTELEMENT_H


#include "QType.h"

class PatternResultElement {
public:
    Ref assign_stmt;
    Ref lhs;
    ExpressionSpec expression;
    PatternResultElement(Ref assignIn, Ref LHSIn, ExpressionSpec expressionIn);
    bool equals(PatternResultElement* other);
    bool operator==(const PatternResultElement & other) const;
    bool AssignEquals(Ref* otherAssign);
    bool LhsEquals(Ref* otherLHS);
    bool ExpressionEquals(ExpressionSpec* otherExpression);
    string ToString();
};


#endif //INC_21S2_CP_SPA_TEAM_37_PATTERNRESULTELEMENT_H
