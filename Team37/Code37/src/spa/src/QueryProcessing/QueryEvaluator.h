//
// This class will receive the query order and the data needed, then decide the answer.
// The answer will be passed to result render class later by the processor.
//

#ifndef INC_21S2_CP_SPA_TEAM_37_1_QUERYRESULTDECIDER_H
#define INC_21S2_CP_SPA_TEAM_37_1_QUERYRESULTDECIDER_H


#include "model/QueryResult.h"
#include "model/Query.h"
#include "model/ConditionResultTable.h"
#include "model/RelationResultTable.h"
#include "model/PatternResultTable.h"
#include "model/WithResultTable.h"
#include "PKB/PKB.h"
#include <set>

class QueryEvaluator {
    PKB *pkb;
     vector<string> s_refVectorToStringVector(vector<Ref> *refVector);

     list<list<Ref>> s_relationResultTableTo2DList(RelationResultTable table);
     list<list<Ref>> s_patternResultTableTo2DList(PatternResultTable table);
     list<list<Ref>> s_withResultTableTo2DList(WithResultTable table);
     vector<Ref> s_generateTupleFromPossibleValues(vector<Ref> *tuple, vector<vector<Ref>> *dataList, vector<bool> *existInfo, size_t conditionResultTableSize);
     vector<Ref> s_removeFirstElementOfRefList(list<Ref> refList);

     bool CheckVariableInClauses(Ref &ref, vector<Relation> &r, vector<Pattern> &p, vector<With> &w);
     bool NeedProcessAttribute(Ref *synonym);
     void ReorderClauses(vector<Relation> &r, vector<Pattern> &p, vector<With> &w, map<string, list<string>> &clauses_type_with_pos, map<string, list<string>> &pattern_syn, list<string> &unassigned_clauses);
     void UpdateHashMap(map<string, list<string>> &hm, string &arg_one, int &count, string &clause_type);
     void OrderHashMap(map<string, list<string>> &reorder_map, map<string, int> &hm, int &max_block);
     void BuildOrderedMap(map<string, int> &hm, list<string> &current_list, int &counter);
     map<int, list<string>> BuildEvaluationSequence(map<string, int> &ordered_sequence, map<int, list<string>> &build_into_block, int &counter, list<string> &unassigned_clauses);


     bool ContainsAllReturnValues(map<string, list<string>> &reorder_map, Ref &current_ref, set<string> &clauses_contains_return_values, map<string, list<string>> &pattern_syn);
     void CollectUniqueRefValue(vector<Ref> &ref_list, set<string> &return_value_set);
     void GetImportantBlockNumber(map<string, int> &ordered_sequence, set<string> &clauses_contains_return_values, list<int> &block_number_present);
     void MapReturnValues(map<string, list<string>> &reorder_map, set<string> &return_value_set, int &number_of_presence_in_clauses, set<string> &clauses_contains_return_values);
     ConditionResultTable getFalseConditionResultTable();
     string s_printRefVector(vector<Ref> *vector);
     string s_printRefTable(vector<vector<Ref>> *table);
     string s_printReorderMap(map<string, list<string>> &reorder_map);
     string s_printEvaluationSequence(map<int, list<string>> &build_into_block);
     string s_printBlockNumber(list<int> &block_number_present);
     string s_printStringList(const list<string>& strs);
     vector<Relation> s_pickRelations(vector<Relation> *relationList, list<string> indexs);
     vector<Pattern> s_pickPatterns(vector<Pattern> *patternList, list<string> indexs);
     vector<With> s_pickWiths(vector<With> *withList, list<string> indexs);
     bool s_blockContainsSelected(list<string> *blockIndexs, set<string> *clausesWithSelected);
     vector<Ref> s_generateResultForNoConditionTupleQuery(Query *query, PKB *pkb);
     vector<Ref> s_generateResultForNoConditionEntityQuery(Query *query, PKB *pkb);
     vector<Ref> s_generateResultForEntityQueryWithConditions(Query *query, ConditionResultTable *conditionResultTable, PKB *pkb);
     vector<Ref> s_generateResultForTupleQueryWithConditions(Query *query, ConditionResultTable *conditionResultTable, PKB *pkb);
     bool s_isValidConditionResultTable(ConditionResultTable *conditionResultTable);
     ConditionResultTable s_combineResultValues(Query *query, vector<list<list<Ref>>> *resultValues);
     void GetShortTableIndex(int &shortestTableIndex, vector<bool> &combined, vector<list<list<Ref>>> *resultValues);
     void GetFlag(vector<Ref>* tuple, int &i, bool &flag, vector<vector<Ref>>* dataList, Ref &checking, vector<vector<Ref>> &combinedResult);
     void CollectPossibleValues(int &i, vector<vector<Ref>>* dataList, vector<vector<Ref>> &combinedResult, vector<Ref> &resultList, size_t &resultSize);
     void IterateCombineResult(vector<vector<Ref>> &combinedResult, vector<vector<Ref>>* dataList, int &i);
     void IterativelyOrganizeTheElement(map<string, list<string>>& reorder_map, set<string> &founded_block, map<string, int>& hm, int& counter);
     void CollectSelectedTuple(Query *query, ConditionResultTable &conditionResultTable, vector<ConditionResultTable> &resultTablePool);
     void FindShortestTableIndex(vector<ConditionResultTable> &afterPickNeeded, vector<bool> &combined, ConditionResultTable &conditionResultTable);

public:
    QueryEvaluator(PKB *pkbIn);
    QueryResult Evaluate(Query *query);
    QueryResult EvaluateQueryWithNoCondition(Query *query);
    QueryResult EvaluateQueryWithConditions(Query *query);
    QueryResult DecideResult(Query *query, ConditionResultTable *conditionResultTable);
    ConditionResultTable GenerateConditionResultTable(Query *query);
    ConditionResultTable GetConditionResultTableWithOptimization(Query *query);
    ConditionResultTable GetConditionResultTableInSequence(Query *query);
    ConditionResultTable GetConditionResultTableByLists(Query *query, vector<Relation> *relationList, vector<Pattern> *patternList, vector<With> *withList);
     string PrintBuildSequence(Query *query);
     string PrintOrderSequence(map<string, int> &ordered_sequence);
};


#endif //INC_21S2_CP_SPA_TEAM_37_1_QUERYRESULTDECIDER_H