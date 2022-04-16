#ifndef INC_21S2_CP_SPA_TEAM_37_QueryParser_H
#define INC_21S2_CP_SPA_TEAM_37_QueryParser_H
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include<set>
#include<unordered_map>
#include<map>
#include <functional>
#include "StringTokenizer.h"
#include "QueryLexer.h"
#include  "../PKB/Attribute/VarName.h"
#include  "../PKB/Attribute/VarName.h"
#include  "../model/Query.h"
#include "../model/QType.h"
#include "../model/ENUM.h"
#include "../SourceProcessor/DesignExtractor.h"

using namespace std;

class QueryParser {

private:
    QueryLexer *lex;
    set<string> variable_set;
    Relation relation;
    EntType entity;
    RefValue name;
    Ref ref;
    unordered_map<string, EntType> entity_type;
    string boolean_value = "BOOLEAN";
    string procname_value = "procName";
    string statement_number_value = "stmt#";
    string var_name_value = "varName";
    string value_value = "value";
    string negative_sign = "-";
    string all_entity[12] = {"kStmt",
                             "kRead",
                             "kPrint",
                             "kCall",
                             "kWhile",
                             "kIf",
                             "kAssign",
                             "kVariable",
                             "kConstant",
                             "kProcedure",
                             "kStmtlst",
                             "kInvalidEnt"
    };

    Relation relation_parameter;
    Pattern pattern_parameter;
    With with_parameter;
    bool sem_valid = true;
    bool has_relation = false;
    bool has_pattern = false;
    bool has_with = false;
    string error_message;
    vector<Relation> list_relation_parameter;
    vector<Pattern> list_pattern_parameter;
    vector<With> list_with_parameter;

    bool ContainEntity(string var);
    string PrintEntity();
    int QuerySelect();
    int QueryDeclaration();
    int ParseEntity(EntType& et);
    int ParseVariableName(unordered_map<string, EntType>& eT, EntType ett);
    int ParseSelectVariableName(string& name);
    int ParseSeparation();
    int QueryClause(Pattern& p, Relation& r, With& w);
    int QueryRelation(Relation& r );
    int QueryPattern(Pattern& r );
    int QueryWith(With& w);
    int RemoveCondition();
    int CollectRelationParameter(Ref& argOne, Ref& argTwo);
    int CollectWithParameter(Ref& argOne, Ref& argTwo);
    int CollectStringParameter(Ref& argOne);
    int CollectExpressionParameter(ExpressionSpec& argOne);
    int CollectIntParameter(Ref& argOne);
    int CollectWildCardParameter(Ref& argOne);
    int CollectVariableParamter(Ref& argOne);
    int HandleRelation(Relation &r);
    int HandlePattern(Pattern& p);
    int HandleWith(With& w);
    bool VerifyPatternName(string p);
    int CollectPatternParameter(Ref& argOne, ExpressionSpec& argTwo, Ref& argThree, EntType &et);
    int CollectUnrestricted(ExpressionSpec& argOne);
    int CollectUnrestricted(Ref& argOne);
    int CollectProcedure(Ref& argOne);
    int CollectPartialMatch(ExpressionSpec& argOne);
    int CheckValidityOfDeclarationName(string &inputS);
    int VerifySuchCondition(Relation &r);
    int VerifyWithParameterType(Ref& argOne);
    int VerifyIfAndWhilePattern(ExpressionSpec& argTwo, Ref& argThree, EntType &et);
    bool IsStatement(EntType &et);
    bool IsProcedure(EntType &et);
    bool IsValidUse(EntType &et);
    bool IsValidModify(EntType &et);
    bool IsAssign(EntType &et);
    bool IsValidTokenizedString();
    void SetMessage(string &s);
    void SetInvalidRef(Ref& argOne);
    int ParseVariableContainsEntity(string &name);
    void SetUseType(RelationType &rType);
    void SetModifiesType(RelationType &rType);
    void SetFollowsType(RelationType &rType);
    void SetParentsType(RelationType &rType);
    void SetCallsType(RelationType &rType);
    void SetNextsType(RelationType &rType);
    void SetAffectsType(RelationType &rType);
    int IsValidStmt(EntType &et, Ref &argOne);
    int IsValidProc(EntType &et, Ref &argOne);
    int IsValidVar(EntType &et, Ref &argOne);
    int IsValidValue(EntType &et, Ref &argOne);
    int HandleWithNegativeNumber(Ref &argOne, int &isValid);
    int HandleWithEntity(Ref &argOne, int &isValid);
    int HandleWithIdent(Ref &argOne, int &isValid);
    int HandleAndConnector(Pattern &p, Relation &r, With &w, string &clause);
    int RemoveLeftBracket();
    int CollectRelationFirstParameter(Ref& argOne);
    int CollectWithTwoParameter(Ref& argOne);
    int RemoveDelim(string d);
    int CollectRelationSecondParameter(Ref& argOne);
    int RemoveRightBracket();
    int CollectPatternFirstParameter(Ref& argOne);
    int CollectPatternSecondParameter(ExpressionSpec& argOne);
    int VerifyFollowAndParentAndNext(EntType& argOne, EntType& argTwo);
    int VerifyCall(EntType& argOne, EntType& argTwo);
    int VerifyAffects(EntType& argOne, EntType& argTwo);
    int VerifyUses(EntType& argOne, EntType& argTwo, Ref* a);
    int VerifyModify(EntType &argOneEntType, EntType &argTwoEntType, Ref* a);
    int VerifyWhetherBothParameterAreStmtRef(Ref& argOne, Ref& argTwo);
    int IsStmtRef(Ref& argOne);
    int IsEntRef(Ref& argOne);
    int VerifyWhetherBothParameterAreEntRef(Ref& argOne, Ref& argTwo);
    int VerifyOneStmtRefAndOneEntRef(Ref& argOne, Ref& argTwo);
    string GetErrorMessage();
    bool VerifyModifyAndUseLeftArgument(Ref* argOne);
    int ParseSelectMultipleVariableName();
    bool IsBooleanValue(string s);
    void AddRelation(Relation &r);
    void AddPattern(Pattern &p);
    void AddWith(With &w);
    int VerifyReturnValues();

public:
    QueryParser(QueryLexer *lexer1);
    ~QueryParser();
    Query query();
    Relation GetRelation();
    QueryLexer* GetLex();
};

#endif //INC_21S2_CP_SPA_TEAM_37_QueryParser_H