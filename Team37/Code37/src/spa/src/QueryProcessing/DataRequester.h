

#ifndef INC_21S2_CP_SPA_TEAM_37_1_DATAREQUESTER_H
#define INC_21S2_CP_SPA_TEAM_37_1_DATAREQUESTER_H


#include "../PKB/Attribute/VarName.h"
#include "PKB/Attribute/StatementNumber.h"
#include "../PKB/PKB.h"
#include "../model/QType.h"
#include "../model/Query.h"
#include "../model/RelationResultTable.h"
#include "../model/PatternResultTable.h"
#include "../model/WithResultTable.h"
#include <string>
#include <set>
#include <vector>

using namespace std;

class DataRequester {
    PKB *PKBInstance;
    static vector<int> s_refVectorToIntVector(vector<Ref> refVector);
    static vector<Ref> s_statementNumberToRef(vector<StatementNumber> statements, EntType statementType);
    static vector<Ref> s_statementNumberToRef(set<StatementNumber> statements, EntType statementType);
    static string s_refVectorToString(vector<Ref> refList);
    static vector<Ref> s_combination(vector<vector<Ref>> *dataList, int start);

    vector<Ref> GetAllStmtRefByType(EntType type);
    vector<Ref> GetAllVariable();
    vector<Ref> GetAllConstant();
    vector<Ref> GetAllProcedure();
    RelationResultTable GetModifiesSUsesSTable(Relation relation);
    RelationResultTable GetModifiesPUsesPTable(Relation relation);
    RelationResultTable GetFollowsParentsTable(Relation relation);
    RelationResultTable GetCallsTable(Relation relation);
    RelationResultTable GetNextTable(Relation relation);
    RelationResultTable GetAffectTable(Relation relation);

    vector<RelationResultPair> GetAllRelationByType(RelationType type, EntType arg1Type, EntType arg2Type);
    vector<RelationResultPair> GetRelationSameArg(RelationType type, EntType argType);
    vector<RelationResultPair> GetRelationArgByArg(RelationType type, bool getArg1, Ref gettingBy, EntType gettingArgType);

    vector<pair<Ref, Ref>> GetWithArgByArg(Ref* firstArg, Ref* secondArg);
    vector<pair<Ref, Ref>> GetWithSynBySyn(Ref* firstArg, Ref* secondArg);
    pair<Ref, Ref> GetWithArgBySynHelper(EntType type, ValueType value, string name, int number);

    vector<pair<Ref, Ref>> GetWithSynBySynComplexInt(Ref* firstArg, Ref* secondArg);
    pair<Ref, Ref> GetWithComplexIntHelper(EntType type_first, EntType type_sec, ValueType value_first, ValueType value_sec, int numIn, string str);

    vector<pair<Ref, Ref>> GetWithSynBySynComplexName(Ref* firstArg, Ref* secondArg);
    pair<Ref, Ref> GetWithComplexNameHelper(EntType type_first, EntType type_sec, ValueType value_first, ValueType value_sec, int numIn, int numSec, string str);

    set<int> StatementNumberToInt(set<StatementNumber> stmts);

    set<StatementNumber> GetStatementNumberFromPKB(EntType entType);
    set<StatementNumber> GetStatementNumberFromPKBByName(EntType entType, string name);
    set<VarName> GetVarNamesFromPKB(EntType entType);
    set<ProcedureName> GetProcNamesFromPKB(EntType entType);
    set<VarName> GetVarNamesFromPkbByLine(EntType entType, int stmtNum);

    bool CheckRelationStands(Relation relation);
    PatternResultTable GetPatternResultTableForWhileIfSyn(EntType &patternStmtType, vector<Ref> &statementList, Pattern &pattern);
    void CollectElementListForSynAndWhile(vector<Ref> &statementList, EntType &patternStmtType, vector<PatternResultElement> &elementList);
    void CollectElementListForFixedValue(EntType &patternStmtType, vector<PatternResultElement> &elementList, Pattern &pattern);
    void CollectPatternResultListForSyn(vector<PatternResultElement> &patternResultList, vector<Ref> &statementList, Pattern &pattern);
    void CollectPatternResultListForFixedValue(vector<PatternResultElement> &patternResultList, vector<Ref> &statementList, Pattern &pattern, bool &flag);
    RelationResultTable GetModifyKNumberTable(Relation &relation, RelationType &relationType);
    RelationResultTable GetModifySynWildcard(Relation &relation, RelationType &relationType);
    RelationResultTable GetModifyFixedValue(Relation &relation, RelationType &relationType);
    RelationResultTable GetModifykSynonymTable(Relation &relation, RelationType &relationType);
    RelationResultTable GetModifyUsePKNumberTable(Relation &relation, RelationType &relationType);
    RelationResultTable GetModifyUsePkFixedValueTable(Relation &relation, RelationType &relationType);
    RelationResultTable GetFollowParentSynWCTable(Relation &relation, RelationType &relationType);
    RelationResultTable GetFollowParentKNumberTable(Relation &relation, RelationType &relationType);
    RelationResultTable GetFollowParentKNumberWCTable(Relation &relation, RelationType &relationType);
    RelationResultTable GetFollowParentSynKNumberTable(Relation &relation, RelationType &relationType);
    RelationResultTable GetNextSynWC(ValueType &secondArgType,  RelationType &relationType, ValueType &firstArgType, Ref* firstArg, Ref* secondArg);
    RelationResultTable GetNextKNumberWC(ValueType &secondArgType,  RelationType &relationType, ValueType &firstArgType, Ref* firstArg, Ref* secondArg, Relation &relation);
    void CollectResultListForOthers(RelationType &type, EntType &arg1Type, EntType &arg2Type, vector<RelationResultPair> &resultList);
    void CollectResultListForUsesModifes(RelationType &type, EntType &arg1Type, EntType &arg2Type, vector<RelationResultPair> &resultList);
    void CollectResultListForUsesModifesProcedure(RelationType &type, EntType &arg1Type, EntType &arg2Type, vector<RelationResultPair> &resultList);
    void CollectResultListForCalls(RelationType &type, EntType &arg1Type, EntType &arg2Type, vector<RelationResultPair> &resultList);
    void CollectRelationOtherResultList(RelationType &type, bool &getArg1, Ref &gettingBy, EntType &gettingArgType, vector<RelationResultPair> &resultList);
    void CollectRelationUseModifyResultList(RelationType &type, bool &getArg1, Ref &gettingBy, EntType &gettingArgType, vector<RelationResultPair> &resultList);
    void CollectRelationUseModifyProcedureResultList(RelationType &type, bool &getArg1, Ref &gettingBy, EntType &gettingArgType, vector<RelationResultPair> &resultList);
    void CollectRelationCallResultList(RelationType &type, bool &getArg1, Ref &gettingBy, EntType &gettingArgType, vector<RelationResultPair> &resultList);
    static void CombineEmptyTable(vector<Ref> &resultList, vector<Ref> &holding);
    static void CombineNoneEmptyTable(vector<Ref> &resultList, vector<Ref> &holding, vector<Ref> &rest);
    void CollectWithPandCall(set<ProcedureName> &procs, set<ProcedureName> &callProcs, EntType &secondArgEntType, EntType &firstArgEntType, vector<pair<Ref, Ref>> &temp_list);
    void CollectWithPandVar(set<ProcedureName>& procs, set<VarName>& vars, EntType& secondArgEntType, EntType& firstArgEntType, vector<pair<Ref, Ref>>& temp_list);
    void CollectWithCallandP(set<ProcedureName>& callProcs, set<ProcedureName>& procs, EntType& secondArgEntType, EntType& firstArgEntType, vector<pair<Ref, Ref>>& temp_list);
    void CollectWithPandVarForCall(set<ProcedureName>& procs, set<VarName>& vars, EntType& secondArgEntType, EntType& firstArgEntType, vector<pair<Ref, Ref>>& temp_list);
    void CollectWithVandVForProc(set<VarName>& firstVars, set<VarName>& secondVars, EntType& secondArgEntType, EntType& firstArgEntType, vector<pair<Ref, Ref>>& temp_list);
    void CollectWithVandVForRP(set<VarName>& firstVars, set<VarName>& secondVars, EntType& secondArgEntType, EntType& firstArgEntType, vector<pair<Ref, Ref>>& temp_list);
    void CollectWithVarandPForCall(set<VarName>& vars, set<ProcedureName>& procs, EntType& secondArgEntType, EntType& firstArgEntType, vector<pair<Ref, Ref>>& temp_list);
    void CollectWithVandVForVar(set<VarName>& var, set<VarName>& vars, EntType& secondArgEntType, EntType& firstArgEntType, vector<pair<Ref, Ref>>& temp_list);

public:
    DataRequester(PKB *pkb);
    vector<Ref> GetAllSelectedReference(Ref selectedRef);
    vector<Ref> GetAssignTableByPattern(Pattern pattern);
    RelationResultTable GetRelationTable(Relation relation);
    PatternResultTable GetPatternTable(Pattern pattern);
    WithResultTable GetWithTable(With withCondition);
    vector<Ref> GetAttributeValue(vector<Ref>& RefList, RefValue attributes, EntType type);
    bool validateRelation(Relation relation);
    bool validatePattern(Pattern pattern);
    bool validateWith(With withCondition);
};


#endif //INC_21S2_CP_SPA_TEAM_37_1_DATAREQUESTER_H
