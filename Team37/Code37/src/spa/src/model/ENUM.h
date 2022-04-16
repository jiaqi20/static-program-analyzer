

#ifndef INC_21S2_CP_SPA_TEAM_37_ENUM_H
#define INC_21S2_CP_SPA_TEAM_37_ENUM_H

enum ExpressionSpecType {
    kFullMatch, // something like "x", "x+y", the RHS of assign should be exactly the expression
    kPartialMatch, // something like _"x"_, _"x*y"_, the RHS of assign contains the expression.
    kAnyMatch // the expression is _, RHS of assign can be anything.
};

enum EntType {
    kStmt,
    kRead,
    kPrint,
    kCall,
    kWhile,
    kIf,
    kAssign,

    kVariable,

    kFixedVar,

    kConstant,

    kProcedure,

    kBooleanType,
    kTupleType,


    kStmtlst,

    kInvalidEnt
};

enum RelationType {
    kFollows,
    kFollowst,
    kParent,
    kParentt,
    kUsess,
    kUsesp,
    kModifiess,
    kModifiesp,
    kCalls,
    kCallst,
    kNext,
    kNextt,
    kAffects,
    kAffectst
};

enum ValueType {
    kSynonym,
    kWildcard,
    kFixedValue,
    kNumber,
    kConstantStr,
    kBoolean,
    kName,
};



#endif //INC_21S2_CP_SPA_TEAM_37_ENUM_H
