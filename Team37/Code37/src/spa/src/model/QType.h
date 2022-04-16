

#ifndef INC_21S2_CP_SPA_TEAM_37_1_QTYPE_H
#define INC_21S2_CP_SPA_TEAM_37_1_QTYPE_H

#include "string"
#include "ENUM.h"
#include <vector>

using namespace std;

class RefValue {
    ValueType type;
    string name;
public:
    RefValue();
    RefValue(ValueType typeIn);
    RefValue(ValueType typeIn, string nameIn);
    RefValue(ValueType typeIn, int numberIn);
    string GetName();
    int GetValue();
    ValueType GetType();
    void SetName(string& nameIn);
    void SetValue(int& valueIn);
    void SetType(ValueType& typeIn);
    bool equals(RefValue *other);
    bool operator==(const RefValue & other) const;
    bool TypeEquals(const RefValue *other) const;
    string ToString();
    string PrintResult();
};

//There are two kinds of reference. Statement or Entity(variable).
class Ref{
    RefValue name;
    RefValue attributes;
    EntType type;
    // P.PROCNAME = "THIRD"
    // LEFT -> NAME -> P
    // LEFT -> ATTRIBUTE -> PROCNAME
    // RIGHT -> NAME -> THIRD

    // P.PROCNAME = Q.PROCNAME
    // LEFT -> NAME -> P
    // LEFT -> ATTRIBUTE -> PROCNAME
    // RIGHT -> NAME -> THIRD
    vector<Ref> refList;
public:
    Ref();
    Ref(EntType typeIn, RefValue nameIn);
    Ref(vector<Ref> &tuple);
    string ToString();
    string PrintResult();
    string SelectToString();
    string WithToString();
    EntType GetType();
    RefValue* getName();
    RefValue* getAttribute();
    void SetType(EntType typeIn);
    void SetName(RefValue& nameIn);
    void SetAttribute(RefValue& attributeIn);
    void AddRefValue(Ref& nameIn);
    bool IsStatement();
    bool IsVariable();
    bool IsConstant();
    bool IsProcedure();
    bool IsEntity();
    bool IsTuple();
    bool IsBoolean();
    bool equals(Ref *other);
    void EraseRefAttributes();
    bool operator==(const Ref & other) const;
    vector<Ref>* GetTuple();
};

//Relation is formed by the type and two arguments.
class Relation{
    RelationType type;
    Ref argument1;
    Ref argument2;
public:
    RelationType getRelationType();
    Ref* GetArgument1();
    Ref* GetArgument2();
    void SetType(RelationType typeIn);
    void SetArgument1(Ref& arg1In);
    void SetArgument2(Ref& arg2In);
    bool equals(Relation* other);
    string ToString();
};

class ExpressionSpec {
    string expression;
    ExpressionSpecType type;
public:
    ExpressionSpec();
    ExpressionSpec(ExpressionSpecType& typeIn, string& expressionIn);
    string GetExpression();
    ExpressionSpecType GetType();
    bool equals(ExpressionSpec* otherExpression);
    bool operator==(const ExpressionSpec & other) const;
    string ToString();
    void SetExpression(string& expressionIn);
    void SetType(ExpressionSpecType& typeIn);
};

//Pattern is formed by the assign statement, entity reference argument, and expression_spec.
class Pattern {
public:
    Ref arg1; // Must be a synonym of Assign, if or while
    Ref arg2; // can be _, "x", x
    ExpressionSpec expression_spec;
    Ref* GetArg1();
    Ref* GetArg2();
    ExpressionSpec* GetExpression();
    string ToString();
};

class With {
private:
    Ref argument1;
    Ref argument2;

public:
    Ref* GetArgument1();
    Ref* GetArgument2();
    void SetArgument1(Ref& arg1In);
    void SetArgument2(Ref& arg2In);
    string ToString();
};
#endif //INC_21S2_CP_SPA_TEAM_37_1_QTYPE_H
