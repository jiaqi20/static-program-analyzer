
#include <string>
#include <vector>
#include "QueryLexer.h"
#include "QueryParser.h"

using namespace std;

QueryParser::QueryParser(QueryLexer *lexer1) {
    this -> lex = lexer1;
}

QueryParser::~QueryParser() {
}

void QueryParser::SetMessage(string &s) {
    this -> error_message = s;
}

bool QueryParser::IsBooleanValue(string s) {
    return s == boolean_value;
}

bool ContainsOnlyDigit(string s) {
    int strLen=0;

    int counter = 0;
    for(int i = 0;s[i]!='\0';i++)
    {
        strLen++;
    }
    for(int i = 0;i < strLen;i++)
    {
        if(s[i] >= '0' && s[i] <= '9')
        {
            counter++;
        }
    }
    return strLen == counter;
}

int QueryParser::ParseVariableContainsEntity(string &name) {
    EntType pre_t = ref.GetType();
    EntType t = entity_type[name];
    ref.SetType(t);
    RefValue rV = RefValue();
    ValueType vType;
    string sValue = name;
    int iValue;
    vType = kSynonym;
    iValue = -1;
    rV.SetType(vType);
    rV.SetName(sValue);
    ref.SetName(rV);
    if (GetLex()->MatchDelims(".")) {
        RemoveDelim(".");
        int isValid = VerifyWithParameterType(ref);
        if (isValid == -1) {
            string m = "the attributes names in return values is invalid";
            SetMessage(m);
            return -1;
        }
    }
    ref.AddRefValue(ref);
    if (ref.getAttribute()->GetName() != "" && pre_t == kTupleType) {
        ref.EraseRefAttributes();
    }
    if (pre_t == kTupleType) {
        ref.SetType(pre_t);
    }
    return 1;
}

int QueryParser::ParseSelectVariableName(string &name) {
    name = GetLex()->EatVariable();
    if (ContainEntity(name)) {
        int output = ParseVariableContainsEntity(name);
        if (output == -1) {
            return -1;
        }
    }  else if (IsBooleanValue(name)) {
        EntType t = kBooleanType;
        ref.SetType(t);
        RefValue rV = RefValue();
        ValueType vType = kBoolean;
        rV.SetType(vType);
        ref.SetName(rV);
        ref.AddRefValue(ref);
    } else if (name == "") {
        string m = "entity cannot be empty";
        SetMessage(m);
        return -1;
    } else if (!ContainEntity(name)) {
        string m = "entity name is not declared";
        SetMessage(m);
        return -1;
    }
    return 1;
}

void QueryParser::SetInvalidRef(Ref &argOne) {
    EntType t = kInvalidEnt;
    argOne.SetType(t);
    string sName = "";
    RefValue rV = RefValue();
    rV.SetName(sName);
    argOne.SetName(rV);
    return;
}

int QueryParser::CollectStringParameter(Ref &argOne) {
    string s = *GetLex()->GetCurrentPosition();
    GetLex()->EatString(s);
    RefValue rV = RefValue();
    ValueType vType;
    string sName;
    if (GetLex()->MatchString(s)) {
        EntType t = kInvalidEnt;
        argOne.SetType(t);
        sName = "";
        string m = "it cannot be an empty string";
        SetMessage(m);
        return -1;
    } else {
        sName = GetLex()->EatVariable();
    }
    int iValue;
    vType = kFixedValue;
    iValue = -1;
    rV.SetType(vType);
    rV.SetName(sName);
    argOne.SetName(rV);
    if (GetLex()->MatchString(s)) {
        GetLex()->EatString(s);
    } else {
        string m = "missing enclosing inverted comma";
        SetMessage(m);
        return -1;
    }
    EntType t = kFixedVar;
    argOne.SetType(t);
    return 1;
}

int QueryParser::CollectExpressionParameter(ExpressionSpec &argOne) {
    string s = *GetLex()->GetCurrentPosition();
    GetLex()->EatString(s);
    string sValue = "";
    ExpressionSpecType t = kFullMatch;
    while (!GetLex()->IsEnd() && !GetLex()->MatchString(s)) {
        sValue.append(*GetLex()->GetCurrentPosition());
        GetLex()->NextItem();
    }
    string parsed_express = DesignExtractor().Rpn(sValue);

    if (parsed_express == "_") {
        string m = "pattern has invalid expression";
        SetMessage(m);
        return -1;
    }
    argOne.SetExpression(sValue);
    argOne.SetType(t);
    if (GetLex()->MatchString(s)) {
        GetLex()->EatString(s);
    } else {
        string m = "missing inverted comma";
        SetMessage(m);
        return -1;
    }
    return 1;
}

int QueryParser::CollectIntParameter(Ref &argOne) {
    EntType t = kStmt;
    argOne.SetType(t);
    RefValue rV = RefValue();
    ValueType vType;
    int iValue;
    vType = kNumber;
    string sName = *GetLex()->GetCurrentPosition();
    if  (sName.size() > 8) {
        iValue = -1;
    } else {
        iValue = stoi(*GetLex()->GetCurrentPosition());
    }
    rV.SetType(vType);
    rV.SetName(sName);
    argOne.SetName(rV);
    GetLex()->NextItem();
    return 1;
}

bool QueryParser::ContainEntity(string var) {
    unordered_map<string, EntType>::iterator it = entity_type.find(var);
    return it != entity_type.end();
}

int QueryParser::CollectVariableParamter(Ref &argOne) {
    EntType t = entity_type[*GetLex()->GetCurrentPosition()];
    argOne.SetType(t);
    RefValue rV = RefValue();
    ValueType vType;
    string sName = *GetLex()->GetCurrentPosition();
    int iValue;
    vType = kSynonym;
    iValue = -1;
    rV.SetType(vType);
    rV.SetName(sName);
    argOne.SetName(rV);
    GetLex()->NextItem();
    return 1;
}

int QueryParser::CollectWildCardParameter(Ref &argOne) {
    if (relation_parameter.getRelationType() == kParentt || relation_parameter.getRelationType() == kParent
        || relation_parameter.getRelationType() == kFollows || relation_parameter.getRelationType() == kFollowst
        || relation_parameter.getRelationType() == kNext || relation_parameter.getRelationType() == kNextt) {
        EntType t = kStmt;
        argOne.SetType(t);
    } else if (relation_parameter.getRelationType() == kCalls || relation_parameter.getRelationType() == kCallst) {
        EntType t = kProcedure;
        argOne.SetType(t);
    } else if (relation_parameter.getRelationType() == kAffects || relation_parameter.getRelationType() == kAffectst) {
        EntType t = kAssign;
        argOne.SetType(t);
    } else {
        EntType t = kVariable;
        argOne.SetType(t);
    }
    RefValue rV = RefValue();
    ValueType vType;
    string sName = "";
    int iValue;
    vType = kWildcard;
    iValue = -1;
    rV.SetType(vType);
    argOne.SetName(rV);
    GetLex()->NextItem();
    return 1;
}

int QueryParser::CollectProcedure(Ref &argOne) {
    string s = *GetLex()->GetCurrentPosition();
    GetLex()->EatString(s);
    if (relation_parameter.getRelationType() == kUsess) {
        RelationType rType;
        rType = kUsesp;
        relation_parameter.SetType(rType);
    } else if (relation_parameter.getRelationType() == kModifiess) {
        RelationType rType;
        rType = kModifiesp;
        relation_parameter.SetType(rType);
    }
    EntType t = kProcedure;
    argOne.SetType(t);
    RefValue rV = RefValue();
    ValueType vType;
    string sName = GetLex()->EatVariable();
    int iValue;
    vType = kFixedValue;
    iValue = -1;
    rV.SetType(vType);
    rV.SetName(sName);
    argOne.SetName(rV);
    if (GetLex()->MatchString(s)) {
        GetLex()->EatString(s);
    } else {
        string m = "missing enclosing inverted comma";
        SetMessage(m);
        return -1;
    }
    return 1;
}

int QueryParser::RemoveLeftBracket() {
    if (GetLex()->MatchBracket("(")) {
        GetLex()->EatBracket("(");
    } else {
        string m = "missing bracket";
        SetMessage(m);
        return -1;
    }
    return 1;
}

int QueryParser::CollectRelationFirstParameter(Ref& argOne) {
    int isValid = -1;
    if (GetLex()->MatchString("\"") || GetLex()->MatchString("\'")) {
        isValid = CollectProcedure(argOne);
    } else if (ContainsOnlyDigit(*GetLex()->GetCurrentPosition())) {
        isValid = CollectIntParameter(argOne);
    } else if (ContainEntity(*GetLex()->GetCurrentPosition())) {
        isValid = CollectVariableParamter(argOne);
    } else if (GetLex()->MatchWildCard(*GetLex()->GetCurrentPosition())) {
        isValid = CollectWildCardParameter(argOne);
    } else if (CheckValidityOfDeclarationName(*GetLex()->GetCurrentPosition())) {
        isValid = 1;
        sem_valid = false;
        SetInvalidRef(argOne);
        GetLex()->NextItem();
    } else {
        string m = "unsupported parameter type, relation first parameter not recognized";
        SetMessage(m);
        return -1;
    }
    return isValid;
}

int QueryParser::RemoveDelim(string d) {

    if (GetLex()->MatchDelims(d)) {
        GetLex()->EatDelim(d) ;
        GetLex()->NextItem();
    } else {
        string m = "missing valid delim";
        SetMessage(m);
        return -1;
    }
    return 1;
}

int QueryParser::CollectRelationSecondParameter(Ref &argOne) {
    int isValid = -1;
    if (GetLex()->MatchString("\"") || GetLex()->MatchString("\'")) {
        if (relation_parameter.getRelationType() == kCalls || relation_parameter.getRelationType() == kCallst) {
            isValid = CollectProcedure(argOne);
        } else {
            isValid = CollectStringParameter(argOne);
        }
    } else if (ContainsOnlyDigit(*GetLex()->GetCurrentPosition())) {
        isValid = CollectIntParameter(argOne);
    } else if (ContainEntity(*GetLex()->GetCurrentPosition())) {
        isValid = CollectVariableParamter(argOne);
    } else if (GetLex()->MatchWildCard(*GetLex()->GetCurrentPosition())) {
        isValid = CollectWildCardParameter(argOne);
    } else if (CheckValidityOfDeclarationName(*GetLex()->GetCurrentPosition())) {
        isValid = 1;
        sem_valid = false;
        SetInvalidRef(argOne);
        GetLex()->NextItem();
    } else {
        string m = "unsupported parameter type, relation second parameter not recognized";
        SetMessage(m);
        return -1;
    }
    return isValid;
}

int QueryParser::RemoveRightBracket() {
    if (GetLex()->MatchBracket(")")) {
        GetLex()->EatBracket(")");
    } else {
        string m = "missing bracket";
        SetMessage(m);
        return -1;
    }
    return 1;
}
int QueryParser::CollectRelationParameter(Ref& argOne, Ref& argTwo) {
    int rBracket = RemoveLeftBracket();
    if (rBracket == -1) {
        return -1;
    }

    int getLeft = CollectRelationFirstParameter(argOne);
    if (getLeft == -1) {
        return -1;
    }

    int rDelim = RemoveDelim(",");
    if (rDelim == -1) {
        return -1;
    }
    int getRight = CollectRelationSecondParameter(argTwo);
    if (getRight == -1) {

        return -1;
    }
    int rRBracket = RemoveRightBracket();
    if (rRBracket == -1) {

        return -1;
    }
    return 1;
}

void QueryParser::SetUseType(RelationType &rType) {
    rType = kUsess;
    GetLex()->EatRelation("Uses");
}

void QueryParser::SetModifiesType(RelationType &rType) {
    rType = kModifiess;
    GetLex()->EatRelation("Modifies");
}

void QueryParser::SetFollowsType(RelationType &rType) {
    GetLex()->EatRelation("Follows");
    if (*GetLex()->GetCurrentPosition() == "*") {
        rType = kFollowst;
        GetLex()->NextItem();
    } else {
        rType = kFollows;
    }
}

void QueryParser::SetParentsType(RelationType &rType) {
    GetLex()->EatRelation("Parent");
    if (*GetLex()->GetCurrentPosition() == "*") {
        rType = kParentt;
        GetLex()->NextItem();
    } else {
        rType = kParent;
    }
}

void QueryParser::SetCallsType(RelationType &rType) {
    GetLex()->EatRelation("Calls");
    if (*GetLex()->GetCurrentPosition() == "*") {
        rType = kCallst;
        GetLex()->NextItem();
    } else {
        rType = kCalls;
    }
}

void QueryParser::SetNextsType(RelationType &rType) {
    GetLex()->EatRelation("Next");
    if (*GetLex()->GetCurrentPosition() == "*") {
        rType = kNextt;
        GetLex()->NextItem();
    } else {
        rType = kNext;
    }
}

void QueryParser::SetAffectsType(RelationType &rType) {
    GetLex()->EatRelation("Affects");
    if (*GetLex()->GetCurrentPosition() == "*") {
        rType = kAffectst;
        GetLex()->NextItem();
    } else {
        rType = kAffects;
    }
}

int QueryParser::QueryRelation(Relation &r) {
    RelationType rType;
    Ref argumentOne;
    Ref argumentTwo;
    if (GetLex()->MatchRelation("Uses")) {
        SetUseType(rType);
    } else if (GetLex()->MatchRelation("Modifies")) {
        SetModifiesType(rType);
    } else if (GetLex()->MatchRelation("Follows")) {
        SetFollowsType(rType);
    } else if (GetLex()->MatchRelation("Parent")) {
        SetParentsType(rType);
    } else if (GetLex()->MatchRelation("Calls")) {
        SetCallsType(rType);
    } else if (GetLex()->MatchRelation("Next")) {
        SetNextsType(rType);
    } else if (GetLex()->MatchRelation("Affects")) {
        SetAffectsType(rType);
    } else {
        string m = "relationship not supported";
        SetMessage(m);
        return -1;
    }
    r.SetType(rType);
    int rP = CollectRelationParameter(argumentOne, argumentTwo);
    if (rP == -1) {
        return -1;
    } else {
        r.SetArgument1(argumentOne);
        r.SetArgument2(argumentTwo);
        return 1;
    }
}

int QueryParser::CollectPartialMatch(ExpressionSpec &argOne) {
    string s = *GetLex()->GetCurrentPosition();
    GetLex()->EatString(s);
    string sValue = "";
    ExpressionSpecType t = kPartialMatch;
    while (!GetLex()->IsEnd() && !GetLex()->MatchString(s)) {
        sValue.append(*GetLex()->GetCurrentPosition());
        GetLex()->NextItem();
    }

    string parsed_express = DesignExtractor().Rpn(sValue);

    if (parsed_express == "_") {
        string m = "pattern has invalid expression";
        SetMessage(m);
        return -1;
    }

    argOne.SetExpression(sValue);
    argOne.SetType(t);
    if (GetLex()->MatchString(s)) {
        GetLex()->EatString(s);
    } else {
        string m = "missing inverted comma";
        SetMessage(m);
        return -1;
    }
    return 1;
}

int QueryParser::CollectUnrestricted(ExpressionSpec& argOne) {
    GetLex()->NextItem();
    if (GetLex()->MatchString("\"") || GetLex()->MatchString("\"")) {
        int res = CollectPartialMatch(argOne);
        if (res == -1) {
            return -1;
        }

        if (GetLex()->MatchWildCard(*GetLex()->GetCurrentPosition())) {
            GetLex()->NextItem();
            return 1;
        } else {
            string m = "missing underscore";
            SetMessage(m);
            return -1;
        }
    } else {
        string name = "";
        ExpressionSpecType t = kAnyMatch;
        argOne.SetType(t);
        argOne.SetExpression(name);
        return 1;
    }
}

int QueryParser::CollectUnrestricted(Ref& argOne) {
    GetLex()->NextItem();
        EntType t = kVariable;
    argOne.SetType(t);
        RefValue rV = RefValue();
        ValueType vType;
        string sValue = "";
        int iValue;
        vType = kWildcard;
        iValue = -1;
    rV.SetType(vType);
    rV.SetName(sValue);
    argOne.SetName(rV);
        return 1;
}

int QueryParser::CollectPatternFirstParameter(Ref &argOne) {
    int isValid = -1;
    if (GetLex()->MatchString("\"") || GetLex()->MatchString("\'")) {
        isValid = CollectStringParameter(argOne);
    } else if (GetLex()->MatchWildCard(*GetLex()->GetCurrentPosition())) {
        isValid = CollectUnrestricted(argOne);
    } else if (ContainEntity(*GetLex()->GetCurrentPosition())) {
        EntType t = entity_type[*GetLex()->GetCurrentPosition()];
        isValid = CollectVariableParamter(argOne);
        if (t != kVariable) {
            string m  = "pattern first argument can only be variable synonym or wildcard or double quote";
            SetMessage(m);
            return -1;
        }
    } else if (CheckValidityOfDeclarationName(*GetLex()->GetCurrentPosition()) == 1) {
        sem_valid = false;
        GetLex()->NextItem();
        isValid = 1;
    } else {
        string m  = "unsupported parameter type, relation first parameter not recognized";
        SetMessage(m);
        return -1;
    }
    return isValid;
}

int QueryParser::CollectPatternSecondParameter(ExpressionSpec &argOne) {
    int isValid = -1;
    if (GetLex()->MatchString("\"") || GetLex()->MatchString("\'")) {
        isValid = CollectExpressionParameter(argOne);
    } else if (GetLex()->MatchWildCard(*GetLex()->GetCurrentPosition())) {
        isValid = CollectUnrestricted(argOne);
    }  else {
        string m  = "unsupported parameter type, expressionpec second parameter not valid";
        SetMessage(m);
        return -1;
    }
    return isValid;
}

int QueryParser::CollectPatternParameter(Ref &argOne, ExpressionSpec &argTwo, Ref& argThree, EntType &et) {
    int rBracket = RemoveLeftBracket();
    if (rBracket == -1) {
        return -1;
    }
    int pLeftParameter = CollectPatternFirstParameter(argOne);
    if (pLeftParameter == -1) {
        sem_valid = false;
    }
    pLeftParameter = IsEntRef(argOne);
    if (pLeftParameter == -1) {
        return -1;
    }
    int rDelim = RemoveDelim(",");
    if (rDelim == -1) {
        return -1;
    }
    int pRightParameter = CollectPatternSecondParameter(argTwo);
    if (pRightParameter == -1) {
        return -1;
    }
    if (GetLex()->MatchDelims(",")) {
        int rDelim_2 = RemoveDelim(",");
        if (rDelim_2 == -1) {
            return -1;
        }
        int pRightParameter_2 = CollectPatternFirstParameter(argThree);
        if (pRightParameter_2 == -1) {
            return -1;
        }
        if (argThree.getName()->GetType() != kWildcard || argTwo.GetType() != kAnyMatch) {
            string m  = "if third argument needs to be wildcard and second argument need to be wildcard";
            SetMessage(m);
            sem_valid = false;
            return -1;
        }
        if (et != kIf) {
            sem_valid = false;
        }
    }
    int rRBracket = RemoveRightBracket();
    if (rRBracket == -1) {
        return -1;
    }
    return 1;
}

int QueryParser::VerifyIfAndWhilePattern(ExpressionSpec &argTwo, Ref &argThree, EntType &et) {
    if (et == kIf) {
        if (argThree.getName()->GetType() != kWildcard || argTwo.GetType() != kAnyMatch) {
            string m  = "if second and third argument needs to be wildcard";
            SetMessage(m);
            sem_valid = false;
        }
    } else if (et == kWhile) {
        if (argTwo.GetType() != kAnyMatch) {
            string m  = "while second needs to be wildcard";
            SetMessage(m);
            sem_valid = false;
        }
    }
    return 1;
}

int QueryParser::QueryPattern(Pattern &r) {
    ExpressionSpec ExpSpe = ExpressionSpec();
    Ref argumentOne;
    Ref argumentThree;
    EntType et = r.arg1.GetType();
    int rP = CollectPatternParameter(argumentOne, ExpSpe, argumentThree, et);

    if (rP == -1) {
        return -1;
    }
    int a = VerifyIfAndWhilePattern(ExpSpe, argumentThree, et);
    if (a == -1) {
        return -1;
    } else {
        r.arg2 = argumentOne;
        r.expression_spec = ExpSpe;
        return 1;
    }
}

int QueryParser::RemoveCondition() {
    GetLex()->EatClauses("such");
    if (GetLex()->MatchCondition("that")) {
        GetLex()->EatCondition("that");
        return 1;
    } else {
        string m = "missing that after such";
        SetMessage(m);
        return -1;
    }
}

bool QueryParser::VerifyPatternName(string p) {
    unordered_map<string, EntType>:: iterator it = entity_type.find(p);
    if (it == entity_type.end()) {
        return false;
    } else {
        return entity_type[p] == kAssign || entity_type[p] == kWhile || entity_type[p] == kIf;
    }
}

bool QueryParser::IsStatement(EntType &et) {

    if (et == kStmt || et == kRead || et == kPrint || et == kAssign || et == kCall || et == kWhile || et == kIf) {
        return true;
    } else {
        return false;
    }
}

bool QueryParser::IsValidUse(EntType &et) {
    if (et == kStmt || et == kVariable || et == kAssign || et == kPrint || et == kIf || et == kWhile || et == kCall || et == kProcedure) {
        return true;
    } else {
        return false;
    }
}

bool QueryParser::IsValidModify(EntType &et) {
    if (et == kStmt || et == kVariable || et == kAssign || et == kRead || et == kIf || et == kWhile || et == kCall || et == kProcedure) {
        return true;
    } else {
        return false;
    }
}

bool QueryParser::VerifyModifyAndUseLeftArgument(Ref* argOne) {
    RefValue* rValue = argOne->getName();
    ValueType vType = rValue->GetType();
    if (vType == kWildcard) {
        string m = "first argument cannot be _";
        SetMessage(m);
        return false;
    }
    return true;
}

int QueryParser::VerifyFollowAndParentAndNext(EntType &argOne, EntType &argTwo) {
    if (IsStatement(argOne) && IsStatement(argTwo)) {
        return 1;
    } else {

        string m = "unsupported parameter type, follow and parent and next parameter type not supported";
        SetMessage(m);
        return -1;
    }
}

bool QueryParser::IsProcedure(EntType &et) {
    return et == kProcedure;
}

bool QueryParser::IsAssign(EntType &et) {
    return et == kStmt || et == kAssign;
}

int QueryParser::VerifyCall(EntType &argOne, EntType &argTwo) {
    if (IsProcedure(argOne) && IsProcedure(argTwo)) {
        return 1;
    } else {

        string m = "unsupported parameter type, call's parameter should be both procedure";
        SetMessage(m);
        return -1;
    }
}

int QueryParser::VerifyAffects(EntType &argOne, EntType &argTwo) {
    if (IsAssign(argOne) && IsAssign(argTwo)) {
        return 1;
    } else {
        string m = "unsupported parameter type, affect's parameter should be both assignment";
        SetMessage(m);
        return -1;
    }
}

int QueryParser::VerifyUses(EntType &argOne, EntType &argTwo, Ref* a) {
    if (relation_parameter.getRelationType() == kUsess && argOne == kProcedure) {
        RelationType rType;
        rType = kUsesp;
        relation_parameter.SetType(rType);
    }

    if (IsValidUse(argOne) && (argTwo == kVariable || argTwo == kFixedVar)) {
        if (argOne == kVariable) {
            bool isValid = VerifyModifyAndUseLeftArgument(a);
            if (!isValid) {

                return -1;
            }
        }
        return 1;
    } else {

        string m = "unsupported parameter type, invalid use parameter type";
        SetMessage(m);
        return -1;
    }
}

int QueryParser::VerifyModify(EntType &argOneEntType, EntType &argTwoEntType, Ref *a) {
    if (relation_parameter.getRelationType() == kModifiess && argOneEntType == kProcedure) {
        RelationType rType;
        rType = kModifiesp;
        relation_parameter.SetType(rType);
    }
    if (IsValidModify(argOneEntType) && (argTwoEntType == kVariable || argTwoEntType == kFixedVar)) {
        if (argOneEntType == kVariable) {
            bool isValid = VerifyModifyAndUseLeftArgument(a);
            if (!isValid) {

                return -1;
            }
        }
        return 1;
    } else {

        string m = "unsupported parameter type, invalid modify parameter type";
        SetMessage(m);
        return -1;
    }
}

int QueryParser::IsStmtRef(Ref &argOne) {
    ValueType vt = argOne.getName()->GetType();
    if (vt == kSynonym || vt == kWildcard || vt == kNumber || argOne.GetType() == kInvalidEnt) {
        return 1;
    } else {
        string m = "syntactically incorrect, it is not stmtRef";
        SetMessage(m);
        return -1;
    }
}

int QueryParser::VerifyWhetherBothParameterAreStmtRef(Ref &argOne, Ref &argTwo) {
    if (IsStmtRef(argOne) == 1 && IsStmtRef(argTwo) == 1) {
        return 1;
    } else {
        string m = "one of the parameter is not syntactically correct, one of the parameter is not stmt ref";
        SetMessage(m);
        return -1;
    }
}

int QueryParser::IsEntRef(Ref &argOne) {
    ValueType vt = argOne.getName()->GetType();
    EntType et = argOne.GetType();
    if (vt == kSynonym || vt == kWildcard || vt == kFixedValue || et == kStmt || et == kRead
    || et == kPrint || et == kWhile || et == kIf || et == kAssign || et == kVariable || et == kConstant || et == kProcedure ||
    et == kCall) {
        return 1;
    } else {
        return -1;
    }
}

int QueryParser::VerifyWhetherBothParameterAreEntRef(Ref &argOne, Ref &argTwo) {
    if (IsEntRef(argOne) == 1 && IsEntRef(argTwo) == 1) {
        return 1;
    } else {
        string m = "one of the parameter is not syntactically correct, one of the parameter is not ent ref";
        SetMessage(m);
        return -1;
    }
}

int QueryParser::VerifyOneStmtRefAndOneEntRef(Ref &argOne, Ref &argTwo) {
    if ((IsEntRef(argOne) == 1 && IsStmtRef(argTwo) == 1) || (IsEntRef(argTwo) == 1 && IsStmtRef(argOne) == 1)) {
        return 1;
    } else {
        string m = "one of the parameter need to be Stmt ref and the other need to be ent ref";
        SetMessage(m);
        return -1;
    }
}

int QueryParser::VerifySuchCondition(Relation &r) {
    RelationType rt = r.getRelationType();
    Ref* argOne = r.GetArgument1();
    EntType argOneEntType = argOne->GetType();
    Ref* argTwo = r.GetArgument2();
    EntType argTwoEntType = argTwo->GetType();
    int isValid = -1;
    if (rt == kFollowst || rt == kFollows || rt == kParent || rt == kParentt || rt == kNext || rt == kNextt) {
        isValid = VerifyFollowAndParentAndNext(argOneEntType, argTwoEntType);
    } else if (rt == kUsess || rt == kUsesp) {
        isValid = VerifyUses(argOneEntType, argTwoEntType, argOne);
    } else if (rt == kModifiesp || rt == kModifiess) {
        isValid = VerifyModify(argOneEntType, argTwoEntType, argOne);
    } else if (rt == kCalls || rt == kCallst) {
        isValid = VerifyCall(argOneEntType, argTwoEntType);
        if (argOneEntType == kProcedure && argTwoEntType == kProcedure) {
            string empty = "";
            string first = argOne->getName()->GetName();
            string second = argTwo->getName()->GetName();
            if (first == second && first != empty) {
                string m = "cyclic calls in procedure";
                SetMessage(m);
                isValid = -1;
            }
        }
    } else if (rt == kAffects || rt == kAffectst) {
        isValid = VerifyAffects(argOneEntType, argTwoEntType);
    } else {
        string m = "unsupported entity type";
        SetMessage(m);
        return -1;
    }
    if (isValid == -1) {
        sem_valid = false;
    }
    if (rt == kFollowst || rt == kFollows || rt == kParent || rt == kParentt || rt == kNext
    || rt == kNextt ||   rt == kAffects || rt == kAffectst) {
        isValid = VerifyWhetherBothParameterAreStmtRef(*argOne, *argTwo);
    } else if (rt == kUsess || rt == kModifiess) {
        isValid = VerifyOneStmtRefAndOneEntRef(*argOne, *argTwo);
    } else if (rt == kUsesp || rt == kModifiesp || rt == kCalls || rt == kCallst) {
        isValid = VerifyWhetherBothParameterAreEntRef(*argOne, *argTwo);
    }
    return isValid;
}

void QueryParser::AddPattern(Pattern &p) {
    this->list_pattern_parameter.push_back(p);
}

void QueryParser::AddRelation(Relation &r) {
    this->list_relation_parameter.push_back(r);
}

int QueryParser::HandleRelation(Relation &r) {
    if (GetLex() -> IsEnd()) {
        string m = "relation missing before reaching the end";
        SetMessage(m);
        return -1;
    }
    int rReRes = QueryRelation(r);
    if (rReRes == -1) {
        return -1;
    }

    int vRelation = VerifySuchCondition(r);
    if (vRelation == -1) {
        sem_valid = false;
        return -1;
    }
    AddRelation(r);
    return 1;
}

int QueryParser::HandlePattern(Pattern &p) {
    if (GetLex() -> IsEnd()) {
        string m = "pattern missing before reaching the end";
        SetMessage(m);
        return -1;
    }
    bool correctPattern = VerifyPatternName(*GetLex()->GetCurrentPosition());
    if (!correctPattern) {
        sem_valid = false;
        string m = "pattern name is not declared";
        SetMessage(m);
    }
    int isValidName = CheckValidityOfDeclarationName(*GetLex()->GetCurrentPosition());
    if (isValidName == -1) {
        string m = "pattern name is not of a valid ident";
        SetMessage(m);
        return -1;
    }
    if (ContainEntity(*GetLex()->GetCurrentPosition())) {
        EntType t = entity_type[*GetLex()->GetCurrentPosition()];
        p.arg1.SetType(t);
        RefValue rV = RefValue();
        ValueType vType;
        string sValue = *GetLex()->GetCurrentPosition();
        int iValue;
        vType = kSynonym;
        iValue = -1;
        rV.SetType(vType);
        rV.SetName(sValue);
        p.arg1.SetName(rV);
    } else {
        sem_valid = false;
        string m = "the entity name is not declared";
        SetMessage(m);
    }
    GetLex()->NextItem();
    int qPattRes = QueryPattern(p);
    if (qPattRes == -1) {
        return -1;
    }
    AddPattern(p);
    return 1;
}

int QueryParser::IsValidStmt(EntType &et, Ref &argOne) {
    if (et != kStmt && et != kRead && et != kPrint && et != kCall &&
        et != kWhile && et != kIf && et != kAssign) {
        sem_valid = false;
        string m = "requested parameter should not have stmt# attribute";
        SetMessage(m);
        if (argOne.getName()->GetType() != kSynonym) {
            return -1;
        }
    }
    return 1;
}

int QueryParser::IsValidProc(EntType &et, Ref &argOne) {
    if (et != kProcedure && et != kCall) {
        string m = "requested parameter should not have procName attribute";
        SetMessage(m);
        sem_valid = false;
        if (argOne.getName()->GetType() != kSynonym) {
            return -1;
        }
    }
    return 1;
}

int QueryParser::IsValidVar(EntType &et, Ref &argOne) {
    if (et != kVariable && et != kRead && et != kPrint) {
        string m = "requested parameter should not have varName attribute";
        SetMessage(m);
        sem_valid = false;
        if (argOne.getName()->GetType() != kSynonym) {
            return -1;
        }
    }
    return 1;
}

int QueryParser::IsValidValue(EntType &et, Ref &argOne) {
    if (et != kConstant) {
        string m = "requested parameter should not have value attribute";
        SetMessage(m);
        sem_valid = false;
        if (argOne.getName()->GetType() != kSynonym) {
            return -1;
        }
    }
    return 1;
}

int QueryParser::VerifyWithParameterType(Ref &argOne) {
    if (GetLex() -> IsEnd()) {
        string m = "parameter values missing before reaching the end";
        SetMessage(m);

        return -1;
    }
    string variable = GetLex()->EatVariable();
    EntType et = argOne.GetType();
    if (variable == statement_number_value) {
        int stmt = IsValidStmt(et, argOne);
        if (stmt == -1) {
            return -1;
        }
    } else if (variable == procname_value) {
        int proc = IsValidProc(et, argOne);
        if (proc == -1) {
            return -1;
        }
    } else if (variable == var_name_value) {
        int var = IsValidVar(et, argOne);
        if (var == -1) {
            return -1;
        }
    } else if (variable == value_value ) {
        int va = IsValidValue(et, argOne);
        if (va == -1) {
            return -1;
        }
    } else {
        string m = "the attributes is not supported";
        SetMessage(m);
        return -1;
    }
    RefValue* rf = argOne.getAttribute();
    if (variable == procname_value || variable == var_name_value) {
        ValueType vt = kName;
        rf->SetType(vt);
    } else if (variable == statement_number_value || variable == value_value ) {
        ValueType vt = kNumber;
        rf->SetType(vt);
    }
    rf->SetName(variable);
    argOne.SetAttribute(*rf);
    return 1;
}

int QueryParser::HandleWithNegativeNumber(Ref &argOne, int &isValid) {
    GetLex() -> NextItem();
    if (ContainsOnlyDigit(*GetLex() -> GetCurrentPosition())) {
        isValid = CollectIntParameter(argOne);
        int iValue;
        string sName = argOne.getName()->GetName();
        if  (sName.size() > 8) {
            iValue = -1;
        } else {
            iValue = 0 - argOne.getName()->GetValue();
        }
        string final_string = "-" + sName;
        argOne.getName()->SetName(final_string);
        ValueType vt = kNumber;
        argOne.getAttribute()->SetType(vt);
    } else {
        string m = "negative number express wrongly";
        SetMessage(m);
        return -1;
    }
    return 1;
}

int QueryParser::HandleWithEntity(Ref &argOne, int &isValid) {
    isValid = CollectVariableParamter(argOne);
    if (isValid == -1) {
        return -1;
    }
    int rDelim = RemoveDelim(".");
    if (rDelim == -1) {
        return -1;
    }
    isValid = VerifyWithParameterType(argOne);
    if (isValid == -1) {
        sem_valid = false;
    }
    return 1;
}

int QueryParser::HandleWithIdent(Ref &argOne, int &isValid) {
    sem_valid = false;
    CollectVariableParamter(argOne);

    int rDelim = RemoveDelim(".");
    if (rDelim == -1) {
        return -1;
    }

    VerifyWithParameterType(argOne);

    isValid = 1;
    return 1;
}

int QueryParser::CollectWithTwoParameter(Ref &argOne) {
    if (GetLex() -> IsEnd()) {
        string m = "parameter missing before reaching the end";
        SetMessage(m);
        return -1;
    }
    int isValid = -1;
    if (*GetLex() -> GetCurrentPosition() == negative_sign) {
        int negative = HandleWithNegativeNumber(argOne, isValid);
        if (negative == -1) {
            return -1;
        }
    } else if (ContainsOnlyDigit(*GetLex() -> GetCurrentPosition())) {
        isValid = CollectIntParameter(argOne);
        ValueType vt = kNumber;
        argOne.getAttribute()->SetType(vt);
    } else if (ContainEntity(*GetLex() -> GetCurrentPosition())) {
        int entityWith = HandleWithEntity(argOne, isValid);
        if (entityWith == -1) {
            return -1;
        }
    } else if (GetLex()->MatchString("\"") || GetLex()->MatchString("\'")) {
        isValid = CollectStringParameter(argOne);
        if (isValid != -1) {
            string s = argOne.getName()->GetName();
            isValid = CheckValidityOfDeclarationName(s);
        }
        ValueType vt = kName;
        argOne.getAttribute()->SetType(vt);
    } else if (CheckValidityOfDeclarationName(*GetLex() -> GetCurrentPosition()) == 1) {
        int ident = HandleWithIdent(argOne, isValid);
        if (ident == -1) {
            return -1;
        }
    } else {
        string m = "variable is not declared";
        SetMessage(m);
    }
    return isValid;
}

int QueryParser::CollectWithParameter(Ref &argOne, Ref &argTwo) {
    int getLeft = CollectWithTwoParameter(argOne);
    if (getLeft == -1) {
        return -1;
    }

    int rDelim = RemoveDelim("=");
    if (rDelim == -1) {
        return -1;
    }

    int getRight = CollectWithTwoParameter(argTwo);
    if (getRight == -1) {
        return -1;
    }

    return 1;
}

int QueryParser::QueryWith(With &w) {
    Ref aOne;
    Ref aTwo;
    int rW = CollectWithParameter(aOne, aTwo);
    if (rW == -1) {
        return -1;
    } else {
        if (aOne.getAttribute()->GetType() != aTwo.getAttribute() ->GetType()) {
            sem_valid = false;
            string m = "two requested type in with clauses should be the same";
            SetMessage(m);
        }
        w.SetArgument1(aOne);
        w.SetArgument2(aTwo);
        return 1;
    }
}

void QueryParser::AddWith(With &w) {
    this->list_with_parameter.push_back(w);
}

int QueryParser::HandleWith(With &w) {
    if (GetLex() -> IsEnd()) {
        string m = "relation missing before reaching the end";
        SetMessage(m);
        return -1;
    }

    int rReWith = QueryWith(w);
    if (rReWith == -1) {
        return -1;
    }
    AddWith(w);
    has_with = true;
    return 1;
}

int QueryParser::HandleAndConnector(Pattern &p, Relation &r, With &w, string &clause) {
    GetLex() -> EatConnector();
    if (clause == "") {
        return -1;
    } else if (clause == "relation") {
        int hRelation = HandleRelation(r);
        if (hRelation == -1) {
            has_relation = false;
            return -1;
        }
    } else if (clause == "pattern") {
        int hPattern = HandlePattern(p);
        if (hPattern == -1) {
            has_pattern = false;
            return -1;
        }
    } else if (clause == "with") {
        int hWith = HandleWith(w);
        if (hWith == -1) {
            has_with = false;
            return -1;
        }
    }
    return 1;
}

int QueryParser::QueryClause(Pattern &p, Relation &r, With &w) {
    string clause = "";
    while (GetLex()->GetCurrentPosition() != GetLex()->GetStringToken().end()) {
        if (GetLex()->MatchClauses("such")) {
            int removeRes = RemoveCondition();
            if (removeRes == -1) {
                return -1;
            }
            int hRelation = HandleRelation(r);
            if (hRelation == -1) {
                return -1;
            }
            clause = "relation";
            has_relation = true;
        } else if (GetLex()->MatchClauses("pattern")) {
            GetLex()->EatClauses("pattern");
            int hPattern = HandlePattern(p);
            if (hPattern == -1) {
                return -1;
            }
            clause = "pattern";
            has_pattern = true;
        } else if (GetLex()->MatchConnector()) {
            int output = HandleAndConnector(p, r, w, clause);
            if (output == -1) {
                return -1;
            }
        } else if (GetLex()->MatchClauses("with")) {
            clause = "with";
            GetLex()->EatClauses("with");
            int hWith = HandleWith(w);
            if (hWith == -1) {
                return -1;
            }
            has_with = true;
        } else {
            string m = "invalid terminator";
            SetMessage(m);
            return -1;
        }
        if (clause != "with") {
            GetLex()->NextItem();
        }
    }
    return 1;
}

int QueryParser::ParseSelectMultipleVariableName() {
    GetLex() ->EatBracket("<");
    string name = "";
    while (!GetLex() ->MatchBracket(">")) {
        int pV = ParseSelectVariableName(name);
        if (pV == -1) {
            return -1;
        }
        if (!GetLex() ->MatchBracket(">")) {
            if (GetLex() ->MatchBracket(",")) {
                GetLex() ->EatBracket(",");
            } else {
                string m = "missing , between the variable names";
                SetMessage(m);
                return -1;
            }
        }
    }
    GetLex() ->EatBracket(">");
    if (name == "") {
        string m = "need to has some return values";
        SetMessage(m);
        return -1;
    }
    return 1;
}

int QueryParser::VerifyReturnValues() {
    vector<Ref> refList = *ref.GetTuple();
    EntType et = ref.GetType();
    if (et == kBooleanType) {
        if (refList.size() > 1) {
            string m = "can only have one boolean in the select";
            SetMessage(m);
            return -1;
        }
    } else {
        for (vector<Ref> ::iterator it1 = refList.begin(); it1 != refList.end(); ++it1) {
            ValueType currentVT = it1->getName()->GetType();
            if (currentVT == kBoolean) {
                string m = "cannot mix boolean type and tuple";
                SetMessage(m);
                return -1;
            }
        }
    }
    return 1;
}

int QueryParser::QuerySelect() {
    string name;
    GetLex()->EatClauses("Select");
    if (GetLex() -> IsEnd()) {

        string errorMessage = "reaching the end";
        SetMessage(errorMessage);
        return -1;
    }
    if (GetLex() ->MatchBracket("<")) {
        EntType t = kTupleType;
        ref.SetType(t);
        int pMV = ParseSelectMultipleVariableName();
        if (pMV == -1) {

            return -1;
        }
        ref.SetType(t);
    } else {
        int pV = ParseSelectVariableName(name);
        if (pV == -1) {

            return -1;
        }
    }
    int checkReturnValues = VerifyReturnValues();
    if (checkReturnValues == -1) {
        return -1;
    }

    int queryClRes = QueryClause(this->pattern_parameter, this->relation_parameter, this-> with_parameter);
    if (queryClRes == -1) {
        return -1;
    }
    return queryClRes;
}

int QueryParser::ParseEntity(EntType& et) {
    if (GetLex()->MatchEntityType("stmt")) {
        et = kStmt;
        GetLex()->EatEntityType("stmt");
    } else if (GetLex()->MatchEntityType("procedure")) {
        et = kProcedure;
        GetLex()->EatEntityType("procedure");
    } else if (GetLex()->MatchEntityType("stmtLst")) {
        et = kStmtlst;
        GetLex()->EatEntityType("stmtLst");
    } else if (GetLex()->MatchEntityType("read")) {
        et = kRead;
        GetLex()->EatEntityType("read");
    } else if (GetLex()->MatchEntityType("print")) {
        et = kPrint;
        GetLex()->EatEntityType("print");
    } else if (GetLex()->MatchEntityType("assign")) {
        et = kAssign;
        GetLex()->EatEntityType("assign");
    } else if (GetLex()->MatchEntityType("call")) {
        et = kCall;
        GetLex()->EatEntityType("call");
    } else if (GetLex()->MatchEntityType("while")) {
        et = kWhile;
        GetLex()->EatEntityType("while");
    } else if (GetLex()->MatchEntityType("if")) {
        et = kIf;
        GetLex()->EatEntityType("if");
    } else if (GetLex()->MatchEntityType("variable")) {
        et = kVariable;
        GetLex()->EatEntityType("variable");
    } else if (GetLex()->MatchEntityType("constant")) {
        et = kConstant;
        GetLex()->EatEntityType("constant");
    } else {
        string errorMessage = "unsupported design entity";
        SetMessage(errorMessage);
        return -1;
    }
    return 1;
}


int QueryParser::CheckValidityOfDeclarationName(string &inputS) {
    int strLen = inputS.length();
    for (int i = 0; i < strLen; i = i + 1) {
        char c = inputS[i];
        if (i == 0) {
            if (('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z')) {
                continue;
            } else {
                string m  = "declaration name first character should be letter";
                SetMessage(m);
                return -1;
            }
        } else {
            if (('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || (c >= '0' && c <= '9')) {
                continue;
            } else {
                string m  = "declaration name second character onwards should be letter or digit";
                SetMessage(m);
                return -1;
            }
        }
    }
    return 1;
}


int QueryParser::ParseVariableName(unordered_map<string, EntType> &eT, EntType ett) {
    string name = GetLex()->EatVariable();
    int isValidName = CheckValidityOfDeclarationName(name);
    if (isValidName == -1) {
        return -1;
    }
    if (name == "") {
        string s = "the entity name cannot be empty";
        SetMessage(s);
        return -1;
    } else if (ContainEntity(name)) {
        sem_valid = false;
        string s = "the entity name has been declared";
        SetMessage(s);
    }  else {
        eT[name] = ett;
        if (GetLex()->MatchBracket(*GetLex()->GetCurrentPosition())) {
            GetLex()->EatBracket(",");
            return ParseVariableName(eT, ett);
        }
        return 1;
    }
}

int QueryParser::ParseSeparation() {
    if (GetLex()->MatchDelims(";")) {
        GetLex()->EatDelim(";");
        return 1;
    } else {
        string m = "missing ; at the end of declaration";
        SetMessage(m);
        return -1;
    }
}

int QueryParser::QueryDeclaration() {
    EntType type;
    string name;
    int pE = ParseEntity(type);
    if (pE == -1) {
        return -1;
    }

    int pV = ParseVariableName(entity_type, type);

    if (pV == -1) {
        return -1;
    }
    int pS = ParseSeparation();

    if (pS == -1) {
        return -1;
    }

    return 1;
}

string QueryParser::PrintEntity() {
    unordered_map<string, EntType>::iterator it = entity_type.begin();
    string s = " ";
    while (it != entity_type.end())
    {
        string name = it->first;
        EntType type = it->second;
        s = s + "name is " + " and the entity type is  " + all_entity[type] + "\n";
        it++;
    }
    return s;
}

Query QueryParser::query() {
    if (!IsValidTokenizedString()) {
        string s = "invalid tokenized string, it contains space in the input string";
        SetMessage(s);
        return Query(false, error_message);
    }
    while (GetLex()->GetCurrentPosition() != GetLex()->GetStringToken().end()) {
        if (GetLex()->MatchClauses("Select")) {
            int  querySeRes = QuerySelect();
            if (querySeRes == -1) {
                return Query(&ref, false, false, error_message);
            }
        } else {
            int  queryDeRes = QueryDeclaration();
            if (queryDeRes == -1) {
                return Query(&ref, false, false, error_message);
            }
        }
        GetLex()->NextItem();
    }
    if (has_pattern && has_relation && has_with) {
        return Query(&ref, &list_relation_parameter, &list_pattern_parameter, &list_with_parameter, sem_valid, error_message);
    } else if (has_pattern && has_with) {
        return Query(&ref, &list_pattern_parameter, &list_with_parameter, sem_valid, error_message);
    } else if (has_relation && has_with) {
        return Query(&ref, &list_relation_parameter, &list_with_parameter, sem_valid, error_message);
    } else if (has_pattern && has_relation) {
        return Query(&ref, &list_relation_parameter, &list_pattern_parameter, sem_valid, error_message);
    } else if (has_pattern) {
        return Query(&ref, &list_pattern_parameter, sem_valid, error_message);
    } else if (has_relation) {
        return Query(&ref, &list_relation_parameter, sem_valid, error_message);
    } else if (has_with) {
        return Query(&ref, &list_with_parameter, sem_valid, error_message);
    } else {
        return Query(&ref, sem_valid, error_message);
    }
}

QueryLexer* QueryParser::GetLex() {
    return lex;
}

string QueryParser::GetErrorMessage() {
    return this->error_message;
}

Relation QueryParser::GetRelation() {
    return this->relation_parameter;
}

bool QueryParser::IsValidTokenizedString() {
    return GetLex()->IsValidTokenized();
}