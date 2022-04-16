

#include "QueryEvaluator.h"
#include "DataRequester.h"
#include "model/WithResultTable.h"

QueryEvaluator::QueryEvaluator(PKB* pkbIn) {
    this->pkb = pkbIn;
}

QueryResult QueryEvaluator::Evaluate(Query* query) {
    
    QueryResult result;
    if (!query->GetRelationList()->empty() || !query->GetPatternList()->empty() || !query->GetWithList()->empty()) {
        result = EvaluateQueryWithConditions(query);
    }
    else {
        result = EvaluateQueryWithNoCondition(query);
    }  
    return result;
}

QueryResult QueryEvaluator::EvaluateQueryWithNoCondition(Query* query) {
    
    if (query->GetSelectedEnt()->IsBoolean()) {
        return { {"TRUE"} };
    }
    else if (query->GetSelectedEnt()->IsTuple()) {
        vector<Ref> resultList = s_generateResultForNoConditionTupleQuery(query, pkb);
        return { s_refVectorToStringVector(&resultList) };
    }
    else {
        vector<Ref> resultList = s_generateResultForNoConditionEntityQuery(query, pkb);
        return { s_refVectorToStringVector(&resultList) };
    }
    return {};
}

vector<Ref> QueryEvaluator::s_generateResultForNoConditionTupleQuery(Query *query, PKB *pkb){
    auto dataRequester = DataRequester(pkb);
    vector<vector<Ref>> tuplePossibleValueTable;
    vector<bool> existInTable;
    for (auto& iter : *query->GetSelectedEnt()->GetTuple()) {
        existInTable.push_back(false);
        vector<Ref> temp = dataRequester.GetAllSelectedReference(iter);
        if (NeedProcessAttribute(&iter)) {
            tuplePossibleValueTable.push_back(
                    dataRequester.GetAttributeValue(temp, *(iter.getAttribute()), iter.GetType()));
        }
        else {
            tuplePossibleValueTable.push_back(temp);
        }
    }
    return s_generateTupleFromPossibleValues(query->GetSelectedEnt()->GetTuple(), &tuplePossibleValueTable,
                                             &existInTable, 0);
}

vector<Ref> QueryEvaluator::s_generateResultForNoConditionEntityQuery(Query *query, PKB *pkb) {
    auto dataRequester = DataRequester(pkb);
    vector<Ref> selectedAllReferenceList = dataRequester.GetAllSelectedReference(*(query->GetSelectedEnt()));
    Ref* selectedRef = query->GetSelectedEnt();
    if (NeedProcessAttribute(selectedRef)) {
        return dataRequester.GetAttributeValue(selectedAllReferenceList, *selectedRef->getAttribute(),
                                                     selectedRef->GetType());
    }
    return selectedAllReferenceList;
}

QueryResult QueryEvaluator::EvaluateQueryWithConditions(Query* query) {

    ConditionResultTable conditionResultTable = GenerateConditionResultTable(query);
    if (conditionResultTable.columnSize() > 0 && conditionResultTable.recordSize() == 0) {
        
        if (query->GetSelectedEnt()->IsBoolean()) {
            return { {"FALSE"} };
        }
        else {
            vector<Ref> temp;
            return { s_refVectorToStringVector(&temp) };
        }
    }
    return DecideResult(query, &conditionResultTable);
}

QueryResult QueryEvaluator::DecideResult(Query *query, ConditionResultTable *conditionResultTable) {
    vector<Ref> resultList;

    if (query->GetSelectedEnt()->IsBoolean()) {
        if (s_isValidConditionResultTable(conditionResultTable)) {
            return {{"TRUE"}};
        } else {
            return {{"FALSE"}};
        }
    }
    else if (query->GetSelectedEnt()->IsEntity()){
        resultList = s_generateResultForEntityQueryWithConditions(query, conditionResultTable, pkb);
    }
    else if (query->GetSelectedEnt()->IsTuple()) {
        resultList = s_generateResultForTupleQueryWithConditions(query, conditionResultTable, pkb);
    }
    return { s_refVectorToStringVector(&resultList) };
}

vector<Ref> QueryEvaluator::s_generateResultForEntityQueryWithConditions(Query *query, ConditionResultTable *conditionResultTable, PKB *pkb) {
    auto dataRequester = DataRequester(pkb);
    vector<Ref> resultList;
    if (query->GetSelectedEnt()->IsEntity() && conditionResultTable->hasColumn(query->GetSelectedEnt())) {
        
        vector<Ref> colTemp = s_removeFirstElementOfRefList(conditionResultTable->ReadCol(*query->GetSelectedEnt()));
        vector<Ref> results;
        if (NeedProcessAttribute(query->GetSelectedEnt())) {
            resultList = dataRequester.GetAttributeValue(colTemp, *query->GetSelectedEnt()->getAttribute(),
                                                         query->GetSelectedEnt()->GetType());
        }
        else {
            resultList = colTemp;
        }
    }
    else if (query->GetSelectedEnt()->IsEntity() && !conditionResultTable->hasColumn(query->GetSelectedEnt())) {
        
        if (conditionResultTable->recordSize() > 0 || (conditionResultTable->columnSize() == 0)) {
            resultList = s_generateResultForNoConditionEntityQuery(query, pkb);
        }
    }
    return resultList;
}

vector<Ref> QueryEvaluator::s_generateResultForTupleQueryWithConditions(Query *query, ConditionResultTable *conditionResultTable, PKB *pkb) {
    auto dataRequester = DataRequester(pkb);
    vector<Ref> resultList;
    
    vector<vector<Ref>> tuplePossibleValueTable;
    vector<bool> existInTable;
    for (auto iter : *query->GetSelectedEnt()->GetTuple()) {
        vector<Ref> possibleValues;
        if (conditionResultTable->hasColumn(&iter)) {
            
            existInTable.push_back(true);
            possibleValues = s_removeFirstElementOfRefList(conditionResultTable->ReadCol(iter));
        } else {
            existInTable.push_back(false);
            possibleValues = dataRequester.GetAllSelectedReference(iter);
        }
        if (NeedProcessAttribute(&iter)) {
            tuplePossibleValueTable.emplace_back(dataRequester.GetAttributeValue(possibleValues, *iter.getAttribute(), iter.GetType()));
        }
        else {
            tuplePossibleValueTable.emplace_back(possibleValues);
        }
    }
    resultList = s_generateTupleFromPossibleValues(query->GetSelectedEnt()->GetTuple(),
                                                   &tuplePossibleValueTable, &existInTable,
                                                   conditionResultTable->recordSize());
    return resultList;
}

vector<string> QueryEvaluator::s_refVectorToStringVector(vector<Ref>* refVector) {
    vector<string> resultList;
    for (auto& iter : *(refVector)) {
        resultList.emplace_back(iter.PrintResult());
    }
    return resultList;
}

string QueryEvaluator::s_printRefVector(vector<Ref>* vec) {
    string result = "[";
    for (auto& iter : *vec) {
        result += iter.ToString() + ", ";
    }
    return result + "]";
}

list<list<Ref>> QueryEvaluator::s_relationResultTableTo2DList(RelationResultTable table) {
    list<list<Ref>> resultList;
    vector<RelationResultPair>* resultPairList = table.getList();
    if (table.getQueryingArg1()->getName()->GetType() == kSynonym) {
        list<Ref> tempList;
        tempList.push_back(*table.getQueryingArg1());
        for (auto& iter : *resultPairList) {
            tempList.push_back(iter.argument1);
        }
        resultList.push_back(tempList);
    }
    if (table.getQueryingArg2()->getName()->GetType() == kSynonym) {
        list<Ref> tempList;
        tempList.push_back(*table.getQueryingArg2());
        for (auto& iter : *resultPairList) {
            tempList.push_back(iter.argument2);
        }
        resultList.push_back(tempList);
    }
    return resultList;
}

list<list<Ref>> QueryEvaluator::s_patternResultTableTo2DList(PatternResultTable table) {
    list<list<Ref>> resultList;
    vector<PatternResultElement>* resultPairList = table.getList();
    if (table.getQueryingAssign()->getName()->GetType() == kSynonym) {
        list<Ref> tempList;
        tempList.push_back(*table.getQueryingAssign());
        for (auto& iter : *resultPairList) {
            tempList.push_back(iter.assign_stmt);
        }
        resultList.push_back(tempList);
    }
    if (table.getQueryingLHS()->getName()->GetType() == kSynonym) {
        list<Ref> tempList;
        tempList.push_back(*table.getQueryingLHS());
        for (auto& iter : *resultPairList) {
            tempList.push_back(iter.lhs);
        }
        resultList.push_back(tempList);
    }
    return resultList;
}

list<list<Ref>> QueryEvaluator::s_withResultTableTo2DList(WithResultTable table) {
    list<list<Ref>> resultList;
    vector<pair<Ref, Ref>>* resultPairList = table.getList();
    if (table.getLHS()->getName()->GetType() == kSynonym) {
        list<Ref> tempList;
        tempList.push_back(*table.getLHS());
        for (auto& iter : *resultPairList) {
            tempList.push_back(iter.first);
        }
        resultList.push_back(tempList);
    }
    if (table.getRHS()->getName()->GetType() == kSynonym && !table.getRHS()->equals(table.getLHS())) {
        list<Ref> tempList;
        tempList.push_back(*table.getRHS());
        for (auto& iter : *resultPairList) {
            tempList.push_back(iter.second);
        }
        resultList.push_back(tempList);
    }
    return resultList;
}

void QueryEvaluator::GetFlag(vector<Ref>* tuple, int &i, bool &flag, vector<vector<Ref>>* dataList, Ref &checking, vector<vector<Ref>> &combinedResult) {
    for (int j = 0; j < i; ++j) {
        if ((tuple->begin() + j)->equals(&checking)) {
            vector<Ref> temp;
            for (auto& iter : *(dataList->begin() + j)) {
                temp.push_back(iter);
            }
            for (auto& iter : temp) {
                (combinedResult.begin() + i)->push_back(iter);
            }
            flag = true;
            break;
        }
    }
}

void QueryEvaluator::IterateCombineResult(vector<vector<Ref>> &combinedResult, vector<vector<Ref>>* dataList, int &i) {
    auto combinedResultIter = combinedResult.begin();
    while (combinedResultIter != combinedResult.end()) {
        vector<Ref> holding;
        for (auto& iter : *combinedResultIter) {
            holding.push_back(iter);
        }
        for (int j = 1; j < (dataList->begin() + i)->size(); ++j) {
            for (auto& iter : holding) {
                combinedResultIter->push_back(iter);
            }
        }
        combinedResultIter++;
    }
}

void QueryEvaluator::CollectPossibleValues(int &i, vector<vector<Ref>>* dataList, vector<vector<Ref>> &combinedResult, vector<Ref> &resultList, size_t &resultSize) {
    vector<Ref> temp;
    if (resultSize <= 0) {
        for (auto& iter : *(dataList->begin() + i)) {
            temp.push_back(iter);
        }
        resultSize = temp.size();
    }
    else {
        for (auto& iter : *(dataList->begin() + i)) {
            for (int j = 0; j < resultSize; ++j) {
                temp.push_back(iter);
            }
        }

        IterateCombineResult(combinedResult, dataList, i);

        resultSize = temp.size();
    }
    for (auto& iter : temp) {
        (combinedResult.begin() + i)->push_back(iter);
    }
}

vector<Ref> QueryEvaluator::s_generateTupleFromPossibleValues(vector<Ref>* tuple, vector<vector<Ref>>* dataList,
    vector<bool>* existInfo, size_t conditionResultTableSize) {

    vector<Ref> resultList;
    vector<vector<Ref>> combinedResult;
    size_t resultSize = 0;
    for (int i = 0; i < tuple->size(); ++i) {
        vector<Ref> temp;
        if (*(existInfo->begin() + i)) {
            for (auto& iter : *(dataList->begin() + i)) {
                temp.push_back(iter);
            }
        }
        combinedResult.push_back(temp);
    }
    resultSize = conditionResultTableSize;
    for (int i = 0; i < tuple->size(); ++i) {
        Ref checking = *(tuple->begin() + i);
        if (!*(existInfo->begin() + i)) {
            bool flag = false;
            GetFlag(tuple, i, flag, dataList, checking, combinedResult);
            if (!flag) {
                CollectPossibleValues(i, dataList, combinedResult, resultList, resultSize);
            }
        }
    }
    for (int i = 0; i < combinedResult.begin()->size(); ++i) {
        vector<Ref> temp;
        for (auto& iter : combinedResult) {
            temp.push_back(*(iter.begin() + i));
        }
        resultList.emplace_back(temp);
    }

    return resultList;
}

vector<Ref> QueryEvaluator::s_removeFirstElementOfRefList(list<Ref> refList) {
    vector<Ref> resultVector;
    auto iter = refList.begin();
    iter++;
    while (iter != refList.end()) {
        resultVector.push_back(*iter);
        iter++;
    }
    return resultVector;
}

string QueryEvaluator::s_printRefTable(vector<vector<Ref>>* table) {
    string result;
    if (!table->empty()) {
        for (int i = 0; i < table->size(); ++i) {
            vector<Ref> row = *(table->begin() + i);
            result += s_printRefVector(&row) + "\n";
        }
    }
    return result;
}

bool QueryEvaluator::NeedProcessAttribute(Ref* synonym) {
    return (synonym->GetType() == kRead && synonym->getAttribute()->GetName() == "varName")
        || (synonym->GetType() == kPrint && synonym->getAttribute()->GetName() == "varName")
        || (synonym->GetType() == kCall && synonym->getAttribute()->GetName() == "procName");
}

void QueryEvaluator::UpdateHashMap(map<string, list<string>>& hm, string& arg_one, int& count, string& clause_type) {
    auto clause_map = hm.find(arg_one);
    string to_find = clause_type + to_string(count);
    if (clause_map != hm.end()) {
        list<string> value = clause_map->second;
        auto list_iter = find(value.begin(), value.end(), to_find);
        if (list_iter != value.end()) {
            return;
        }
        else {
            value.push_back(to_find);
            hm[arg_one] = value;
        }
    }
    else {
        list<string> value;
        value.push_back(to_find);
        hm[arg_one] = value;
    }
    return;
}

void QueryEvaluator::ReorderClauses(vector<Relation>& r, vector<Pattern>& p, vector<With>& w,
    map<string, list<string>>& hm, map<string, list<string>>& pattern_syn, list<string> &unassigned_clauses) {

    int relation_count = 0;
    for (auto & it1 : r) {
        string arg_one = it1.GetArgument1()->getName()->GetName();
        ValueType vt1 = it1.GetArgument1()->getName()->GetType();
        string arg_two = it1.GetArgument2()->getName()->GetName();
        ValueType vt2 = it1.GetArgument2()->getName()->GetType();
        string c_type = "r";
        if (vt1 == kSynonym) {
            UpdateHashMap(hm, arg_one, relation_count, c_type);
        }
        if (vt2 == kSynonym) {
            UpdateHashMap(hm, arg_two, relation_count, c_type);
        }
        if (vt1 != kSynonym && vt2 != kSynonym) {
            string s = "r" + to_string(relation_count);
            unassigned_clauses.push_back(s);
        }
        relation_count = relation_count + 1;
    }

    int pattern_count = 0;
    for (auto & it1 : p) {
        string arg_one = it1.GetArg2()->getName()->GetName();
        ValueType vt1 = it1.GetArg2()->getName()->GetType();
        string arg_syn = it1.GetArg1()->getName()->GetName();
        string c_type = "p";
        if (vt1 == kSynonym) {
            UpdateHashMap(hm, arg_one, pattern_count, c_type);
        }
        UpdateHashMap(hm, arg_syn, pattern_count, c_type);
        UpdateHashMap(pattern_syn, arg_syn, pattern_count, c_type);

        pattern_count = pattern_count + 1;
    }

    int with_count = 0;
    for (auto & it1 : w) {
        string arg_one = it1.GetArgument1()->getName()->GetName();
        ValueType vt1 = it1.GetArgument1()->getName()->GetType();
        string arg_two = it1.GetArgument2()->getName()->GetName();
        ValueType vt2 = it1.GetArgument2()->getName()->GetType();
        string c_type = "w";

        if (vt1 == kSynonym) {
            UpdateHashMap(hm, arg_one, with_count, c_type);
        }
        if (vt2 == kSynonym) {
            UpdateHashMap(hm, arg_two, with_count, c_type);
        }

        if (vt1 != kSynonym && vt2 != kSynonym) {
            string s = "w" + to_string(with_count);
            unassigned_clauses.push_back(s);
        }

        with_count = with_count + 1;
    }
    return;
}


bool QueryEvaluator::CheckVariableInClauses(Ref& ref, vector<Relation>& r, vector<Pattern>& p, vector<With>& w) {

    vector<Ref>* ref_list = ref.GetTuple();
    for (auto & it : *ref_list) {
        string synonym = it.getName()->GetName();
        EntType et_syn = it.GetType();
        for (auto & it1 : r) {
            string arg_one = it1.GetArgument1()->getName()->GetName();
            EntType arg_one_type = it1.GetArgument1()->GetType();
            string arg_two = it1.GetArgument2()->getName()->GetName();
            EntType arg_two_type = it1.GetArgument2()->GetType();
            if ((synonym == arg_one && et_syn == arg_one_type) || (synonym == arg_two && et_syn == arg_two_type)) {
                return true;
            }
        }

        for (auto & it2 : p) {
            string arg_one = it2.GetArg1()->getName()->GetName();
            if (synonym == arg_one) {
                return true;
            }
        }

        for (auto & it3 : w) {
            string arg_one = it3.GetArgument1()->getName()->GetName();
            string arg_two = it3.GetArgument2()->getName()->GetName();
            if (synonym == arg_one || synonym == arg_two) {
                return true;
            }
        }
    }
    return false;
}

void QueryEvaluator::BuildOrderedMap(map<string, int>& hm, list<string>& current_list, int& counter) {
    for (const auto& current_key : current_list) {
        hm[current_key] = counter;
    }
}

void QueryEvaluator::IterativelyOrganizeTheElement(map<string, list<string>>& reorder_map, set<string> &founded_block, map<string, int>& hm, int& counter) {
    bool has_common = true;

    while (has_common) {
        bool found_common = false;
        for (auto & it : reorder_map) {
            string key = it.first;
            list<string> current_list = it.second;
            if (founded_block.find(key) != founded_block.end()) {
                continue;
            }
            int block_number = -1;
            bool has_same = false;
            for (const auto& current_key : current_list) {
                auto clause_map = hm.find(current_key);
                if (clause_map != hm.end()) {
                    block_number = clause_map->second;
                    has_same = true;
                    found_common = true;
                    founded_block.insert(key);
                    break;
                }
            }
            if (has_same) {
                BuildOrderedMap(hm, current_list, block_number);
            }
        }
        has_common = found_common;
    }
    if (founded_block.size() < reorder_map.size()) {

        for (auto & it : reorder_map) {

            string key = it.first;
            list<string> current_list = it.second;
            if (founded_block.find(key) != founded_block.end()) {
                continue;
            }
            founded_block.insert(key);
            BuildOrderedMap(hm, current_list, counter);
            counter = counter + 1;
            break;
        }
    }
}

void QueryEvaluator::OrderHashMap(map<string, list<string>>& reorder_map, map<string, int>& hm, int& counter) {
    set<string> founded_block;
    while (founded_block.size() < reorder_map.size()) {
        if (counter == 0) {
            auto it1 = reorder_map.begin();
            string key = it1->first;
            list<string> current_list = it1->second;
            founded_block.insert(key);
            BuildOrderedMap(hm, current_list, counter);
            counter = counter + 1;
            continue;
        } else {
            IterativelyOrganizeTheElement(reorder_map, founded_block, hm, counter);
        }
    }
}

map<int, list<string>>
QueryEvaluator::BuildEvaluationSequence(map<string, int>& ordered_sequence, map<int, list<string>>& build_into_block,
    int& counter, list<string> &unassigned_clauses) {
    for (auto & it : ordered_sequence) {
        int block_number = it.second;
        string key = it.first;
        auto current_block_number = build_into_block.find(block_number);
        if (current_block_number != build_into_block.end()) {
            list<string> current_list = build_into_block[block_number];
            current_list.push_back(key);
            build_into_block[block_number] = current_list;
        }
        else {
            list<string> current_list;
            current_list.push_back(key);
            build_into_block[block_number] = current_list;
        }
    }
    counter = counter + 1;
    for (const auto& cur_s : unassigned_clauses) {
        list<string> current_list;
        current_list.push_back(cur_s);
        build_into_block[counter] = current_list;
        counter = counter + 1;
    }
    return build_into_block;
}

string QueryEvaluator::s_printEvaluationSequence(map<int, list<string>>& build_into_block) {
    string result = "Current result lists are: \n ";
    for (auto & it : build_into_block) {
        int block_number = it.first;
        list<string> all_list = it.second;
        result += "block" + to_string(block_number) + ": ";
        for (const auto& current_s : all_list) {
            result += current_s + " ";
        }
        result += " \n ";
    }
    return result;
}

string QueryEvaluator::s_printReorderMap(map<string, list<string>>& reorder_map) {
    size_t map_size = reorder_map.size();
    string result = "Current reorder map size is: " + to_string(map_size) + " \n";
    result = result + "Current reorder map are: \n ";
    for (auto & it : reorder_map) {
        string s = it.first;
        list<string> all_list = it.second;
        result += "current synonym " + s + ": ";
        for (const auto& current_s : all_list) {
            result += current_s + " ";
        }
        result += " \n ";
    }
    return result;
}

string QueryEvaluator::s_printBlockNumber(list<int>& block_number_present) {
    size_t map_size = block_number_present.size();
    string result = "Current block size is: " + to_string(map_size) + " \n";
    result = result + "Current block number are: \n ";
    for (int s : block_number_present) {
        result += "block" + to_string(s) + " ";
    }
    return result;
}

string QueryEvaluator::PrintBuildSequence(Query *query) {
    vector<Relation> *relationConditions = query->GetRelationList();
    vector<Pattern> *patternConditions = query->GetPatternList();
    vector<With> *withConditions = query->GetWithList();
    Ref *current_ref = query->GetSelectedEnt();

    map<string, list<string>> reorder_map; // string: "with", "pattern", "such that" ; int: position in the clauses
    map<string, list<string>> pattern_syn;
    list<string> unassigned_clauses;
    ReorderClauses(*relationConditions, *patternConditions, *withConditions, reorder_map, pattern_syn, unassigned_clauses);
    string reorder_map_output = s_printReorderMap(reorder_map);

    map<string, int> ordered_sequence;
    map<int, list<string>> build_into_block; // block number and list of string
    set<string> clauses_contains_return_values;
    list<int> block_number_present; // number
    int max_block = 0;
    OrderHashMap(reorder_map, ordered_sequence, max_block);
    BuildEvaluationSequence(ordered_sequence, build_into_block, max_block, unassigned_clauses);
    ContainsAllReturnValues(reorder_map, *current_ref, clauses_contains_return_values, pattern_syn);
    GetImportantBlockNumber(ordered_sequence, clauses_contains_return_values, block_number_present);

    string block_number = s_printBlockNumber(block_number_present);

    string result = s_printEvaluationSequence(build_into_block);
    return result;
}

void QueryEvaluator::CollectUniqueRefValue(vector<Ref>& ref_list, set<string>& return_value_set) {
    for (auto & it : ref_list) {
        string ref_name = it.getName()->GetName();
        return_value_set.insert(ref_name);
    }
}

void QueryEvaluator::MapReturnValues(map<string, list<string>>& reorder_map, set<string>& return_value_set,
    int& number_of_presence_in_clauses, set<string>& clauses_contains_return_values) {
    for (auto & it : reorder_map) {
        if (return_value_set.empty()) {
            break;
        }
        string current_syn = it.first;
        list<string> current_clauses = it.second;
        if (return_value_set.find(current_syn) != return_value_set.end()) {
            number_of_presence_in_clauses = number_of_presence_in_clauses + 1;
            return_value_set.erase(current_syn);
            string first_clauses = current_clauses.front();
            clauses_contains_return_values.insert(first_clauses);
        }
    }
}

bool QueryEvaluator::ContainsAllReturnValues(map<string, list<string>>& reorder_map, Ref& current_ref,
    set<string>& clauses_contains_return_values,
    map<string, list<string>>& pattern_syn) {
    set<string> return_value_set;
    vector<Ref> ref_list = *current_ref.GetTuple();
    CollectUniqueRefValue(ref_list, return_value_set);
    size_t values_size = return_value_set.size();
    int number_of_presence_in_clauses = 0;
    MapReturnValues(reorder_map, return_value_set, number_of_presence_in_clauses, clauses_contains_return_values);
    MapReturnValues(pattern_syn, return_value_set, number_of_presence_in_clauses, clauses_contains_return_values);
    return values_size == number_of_presence_in_clauses;
}

void QueryEvaluator::GetImportantBlockNumber(map<string, int>& ordered_sequence,
    set<string>& clauses_contains_return_values,
    list<int>& block_number_present) {
    for (const auto& clauses : clauses_contains_return_values) {
        int block_number = ordered_sequence[clauses];
        block_number_present.push_back(block_number);
    }
}

ConditionResultTable QueryEvaluator::GenerateConditionResultTable(Query* query) {
    vector<Relation>* relationConditions = query->GetRelationList();
    vector<Pattern>* patternConditions = query->GetPatternList();
    vector<With>* withConditions = query->GetWithList();
    ConditionResultTable conditionResultTable;
    bool useOptimization = (relationConditions->size() + patternConditions->size() + withConditions->size()) > 1;
    if (useOptimization) {
        conditionResultTable = GetConditionResultTableWithOptimization(query);
    } else {
        conditionResultTable = GetConditionResultTableInSequence(query);
    }
    return conditionResultTable;
}

void QueryEvaluator::FindShortestTableIndex(vector<ConditionResultTable> &afterPickNeeded, vector<bool> &combined, ConditionResultTable &conditionResultTable) {
    int shortestTableIndex = -1;
    for (int j = 0; j < afterPickNeeded.size(); ++j) {
        if (!*(combined.begin() + j)) {
            if (shortestTableIndex == -1 ||
                (afterPickNeeded.begin() + shortestTableIndex)->recordSize() > (afterPickNeeded.begin() + j)->recordSize()) {
                shortestTableIndex = j;
            }
        }
    }
    conditionResultTable.combine((afterPickNeeded.begin() + shortestTableIndex)->GetTable());
    *(combined.begin() + shortestTableIndex) = true;
}

void QueryEvaluator::CollectSelectedTuple(Query *query, ConditionResultTable &conditionResultTable, vector<ConditionResultTable> &resultTablePool) {
    vector<ConditionResultTable> afterPickNeeded;
    for (auto & resultTable : resultTablePool) {
        ConditionResultTable tempTable;
        list<list<Ref>> temp;
        for (auto & tableColumn : resultTable.GetTable()) {
            Ref tableHeader = *tableColumn.begin();
            if (std::count(query->GetSelectedEnt()->GetTuple()->begin(), query->GetSelectedEnt()->GetTuple()->end(), tableHeader) > 0) {
                temp.push_back(resultTable.ReadCol(tableHeader));
            }
        }
        tempTable.combine(temp);
        afterPickNeeded.emplace_back(tempTable);
    }

    vector<bool> combined;
    for (int i = 0; i < afterPickNeeded.size(); ++i) {
        combined.emplace_back(false);
    }
    for (int i = 0; i < afterPickNeeded.size(); ++i) {
        FindShortestTableIndex(afterPickNeeded, combined, conditionResultTable);
    }
}

ConditionResultTable QueryEvaluator::GetConditionResultTableWithOptimization(Query *query) {
    auto dataRequester = DataRequester(pkb);
    vector<Relation>* relationConditions = query->GetRelationList();
    vector<Pattern>* patternConditions = query->GetPatternList();
    vector<With>* withConditions = query->GetWithList();
    Ref* current_ref = query->GetSelectedEnt();

    ConditionResultTable conditionResultTable = ConditionResultTable();
    ConditionResultTable falseResultTable = getFalseConditionResultTable();

    bool contains_dependency = CheckVariableInClauses(*current_ref, *relationConditions, *patternConditions,
                                                      *withConditions);

    map<string, list<string>> pattern_syn;
    map<string, list<string>> reorder_map; // string: "with", "pattern", "such that" ; int: position in the clauses
    list<string> unassigned_clauses;
    ReorderClauses(*relationConditions, *patternConditions, *withConditions, reorder_map, pattern_syn,
                   unassigned_clauses);
    map<string, int> ordered_sequence;
    map<int, list<string>> build_into_block; // block number and list of clauses
    set<string> clauses_contains_return_values;
    list<int> block_number_present; // return value in block number
    int max_block = 0;
    OrderHashMap(reorder_map, ordered_sequence, max_block);
    BuildEvaluationSequence(ordered_sequence, build_into_block, max_block, unassigned_clauses);
    ContainsAllReturnValues(reorder_map, *current_ref, clauses_contains_return_values, pattern_syn);
    GetImportantBlockNumber(ordered_sequence, clauses_contains_return_values, block_number_present);

    vector<ConditionResultTable> resultTablePool;
    for (auto & iter: build_into_block) {

        vector<Relation> blockRelationConditions = s_pickRelations(relationConditions, iter.second);
        vector<Pattern> blockPatternConditions = s_pickPatterns(patternConditions, iter.second);
        vector<With> blockWithConditions = s_pickWiths(withConditions, iter.second);
        
        if (s_blockContainsSelected(&iter.second, &clauses_contains_return_values)) {
            
            ConditionResultTable blockResultTable = GetConditionResultTableByLists(query, &blockRelationConditions, &blockPatternConditions, &blockWithConditions);
            if ((blockResultTable.columnSize() > 0 && blockResultTable.recordSize() == 0)) {
                return falseResultTable;
            }
            resultTablePool.emplace_back(blockResultTable);
        } else {
            if ((blockRelationConditions.size() + blockPatternConditions.size() + blockWithConditions.size()) > 1) {

                ConditionResultTable combinedBlockResult = GetConditionResultTableByLists(query, &blockRelationConditions, &blockPatternConditions, &blockWithConditions);
                if ((combinedBlockResult.columnSize() > 0 && combinedBlockResult.recordSize() == 0)) {
                    return falseResultTable;
                }
            } else {
                if ((blockRelationConditions.size() == 1 && !dataRequester.validateRelation(*blockRelationConditions.begin())) ||
                    (blockPatternConditions.size() == 1 && !dataRequester.validatePattern(*blockPatternConditions.begin())) ||
                    (blockWithConditions.size() == 1 && !dataRequester.validateWith(*blockWithConditions.begin()))) {
                    return falseResultTable;
                }
            }
        }
    }

    if (query->GetSelectedEnt()->IsBoolean()){
        return {};
    } else if (query->GetSelectedEnt()->IsEntity()) {
        if (!resultTablePool.empty() && resultTablePool.begin()->hasColumn(query->GetSelectedEnt())) {
            list<list<Ref>> tempTable;
            tempTable.emplace_back(resultTablePool.begin()->ReadCol(*query->GetSelectedEnt()));
            conditionResultTable.combine(tempTable);
        }
    } else if (query->GetSelectedEnt()->IsTuple()) {
        CollectSelectedTuple(query, conditionResultTable, resultTablePool);
    }
    return conditionResultTable;
}

ConditionResultTable QueryEvaluator::GetConditionResultTableInSequence(Query *query) {
    vector<Relation>* relationConditions = query->GetRelationList();
    vector<Pattern>* patternConditions = query->GetPatternList();
    vector<With>* withConditions = query->GetWithList();
    return GetConditionResultTableByLists(query, relationConditions, patternConditions, withConditions);
}

ConditionResultTable QueryEvaluator::GetConditionResultTableByLists(Query *query, vector<Relation> *relationList, vector<Pattern> *patternList, vector<With> *withList) {
    auto dataRequester = DataRequester(pkb);
    ConditionResultTable falseResultTable = getFalseConditionResultTable();

    vector<list<list<Ref>>> tempResultListPool;
    for (auto& iter : *(relationList)) {
        RelationResultTable relationResultTable = dataRequester.GetRelationTable(iter);

        if (relationResultTable.size() == 0) {
            return falseResultTable;
        }
        else {
            tempResultListPool.emplace_back(s_relationResultTableTo2DList(relationResultTable));
        }
    }
    for (auto& iter : *(patternList)) {
        PatternResultTable patternResultTable = dataRequester.GetPatternTable(iter);

        if (patternResultTable.size() == 0) {
            return falseResultTable;
        }
        else {
            tempResultListPool.emplace_back(s_patternResultTableTo2DList(patternResultTable));
        }
    }
    for (auto& iter : *(withList)) {
        WithResultTable withResultTable = dataRequester.GetWithTable(iter);

        if (withResultTable.size() == 0) {
            return falseResultTable;
        }
        else {
            tempResultListPool.emplace_back(s_withResultTableTo2DList(withResultTable));
        }
    }
    return s_combineResultValues(query, &tempResultListPool);
}


ConditionResultTable QueryEvaluator::getFalseConditionResultTable() {
    ConditionResultTable falseResultTable = ConditionResultTable();
    list<Ref> invalidValue;
    invalidValue.emplace_back(kBooleanType, RefValue(kSynonym, "INVALID"));
    falseResultTable.AddCol(invalidValue);
    return falseResultTable;
}

bool QueryEvaluator::s_isValidConditionResultTable(ConditionResultTable *conditionResultTable) {
    return conditionResultTable->recordSize() > 0 || (conditionResultTable->columnSize() == 0);
}

void QueryEvaluator::GetShortTableIndex(int &shortestTableIndex, vector<bool> &combined, vector<list<list<Ref>>> *resultValues) {
    for (int j = 0; j < resultValues->size(); ++j) {
        if (!*(combined.begin() + j)) {
            if (shortestTableIndex == -1 ||
                (resultValues->begin() + shortestTableIndex)->begin()->size() >
                (resultValues->begin() + j)->begin()->size()) {
                shortestTableIndex = j;
            }
        }
    }
}

ConditionResultTable QueryEvaluator::s_combineResultValues(Query *query, vector<list<list<Ref>>> *resultValues) {
    ConditionResultTable result;
    vector<bool> combined;
    for (int i = 0; i < resultValues->size(); ++i) {
        combined.emplace_back(false);
    }

    for (int i = 0; i < resultValues->size(); ++i) {
        int shortestTableIndex = -1;
        GetShortTableIndex(shortestTableIndex, combined, resultValues);
        list<list<Ref>> needToCombine = *(resultValues->begin() + shortestTableIndex);
        list<list<Ref>> leaveSynonym;
        for (auto column : needToCombine) {
            if (column.begin()->getName()->GetType() == kSynonym) {
                leaveSynonym.push_back(column);
            }
        }
        vector<Relation>* relationConditions = query->GetRelationList();
        vector<Pattern>* patternConditions = query->GetPatternList();
        vector<With>* withConditions = query->GetWithList();
        bool useOptimization = (relationConditions->size() + patternConditions->size() + withConditions->size()) > 1;
        if (leaveSynonym.size() == 1 && useOptimization) {
            ConditionResultTable().RemoveDuplicatesInTable(leaveSynonym);
        }
        result.combine(leaveSynonym);
        *(combined.begin() + shortestTableIndex) = true;
    }
    return result;
}


string QueryEvaluator::s_printStringList(const list<string>& strs) {
    string result;
    for (auto& iter : strs) {
        result += iter;
    }
    return result;
}

vector<Relation> QueryEvaluator::s_pickRelations(vector<Relation> *relationList, list<string> indexs) {
    vector<Relation> result;
    for (auto & iter : indexs) {
        if (iter.at(0) == 'r') {
            int position = stoi(iter.substr(1, iter.size() - 1));
            result.emplace_back(*(relationList->begin() + position));
        }
    }
    return result;
}

vector<Pattern> QueryEvaluator::s_pickPatterns(vector<Pattern> *patternList, list<string> indexs) {
    vector<Pattern> result;
    for (auto & iter : indexs) {
        if (iter.at(0) == 'p') {
            int position = stoi(iter.substr(1, iter.size() - 1));
            result.emplace_back(*(patternList->begin() + position));
        }
    }
    return result;
}

vector<With> QueryEvaluator::s_pickWiths(vector<With> *withList, list<string> indexs) {
    vector<With> result;
    for (auto & iter : indexs) {
        if (iter.at(0) == 'w') {
            int position = stoi(iter.substr(1, iter.size() - 1));
            result.emplace_back(*(withList->begin() + position));
        }
    }
    return result;
}

bool QueryEvaluator::s_blockContainsSelected(list<string> *blockIndexs, set<string> *clausesWithSelected) {
    for (auto & iter : *blockIndexs) {
        if (clausesWithSelected->count(iter) > 0) {
            return true;
        }
    }
    return false;
}
