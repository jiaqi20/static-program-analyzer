
#include "DataRequester.h"

DataRequester::DataRequester(PKB* pkb) {
    this->PKBInstance = pkb;
}

vector<Ref> DataRequester::GetAllVariable() {
    set<VarName> variableList = PKBInstance->GetAllVarNames();
    vector<Ref> resultList;
    for (auto& iter : variableList) {
        resultList.emplace_back(kVariable, RefValue(kFixedValue, iter.GetName()));
    }
    return resultList;
}

vector<Ref> DataRequester::GetAllStmtRefByType(EntType type) {
    vector<Ref> resultList;
    switch (type) {
    case kStmt: {
        int max_stmt_number = PKBInstance->GetMaxStmtNumber();
        for (int i = 1; i <= max_stmt_number; ++i) {
            resultList.emplace_back(kStmt, RefValue(kNumber, i));
        }
    }
              break;
    case kRead:
        resultList = s_statementNumberToRef(PKBInstance->GetAllReadStatements(), kRead);
        break;
    case kPrint:
        resultList = s_statementNumberToRef(PKBInstance->GetAllPrintStatements(), kPrint);
        break;
    case kCall:
        resultList = s_statementNumberToRef(PKBInstance->GetAllCallStatements(), kCall);
        break;
    case kWhile:
        resultList = s_statementNumberToRef(PKBInstance->GetAllWhileStatements(), kWhile);
        break;
    case kIf:
        resultList = s_statementNumberToRef(PKBInstance->GetAllIfStatements(), kIf);
        break;
    case kAssign:
        resultList = s_statementNumberToRef(PKBInstance->GetAllAssignStatements(), kAssign);
        break;
    default:
        break;
    }
    return resultList;
}

vector<Ref> DataRequester::GetAllConstant() {
    vector<Ref> resultList;
    set<Constant> constantSet = PKBInstance->GetAllConstants();
    for (auto& iter : constantSet) {
        resultList.emplace_back(kConstant, RefValue(kConstantStr, iter));
    }
    return resultList;
}

vector<Ref> DataRequester::GetAllProcedure() {
    vector<Ref> resultList;
    set<ProcedureName> procedureNames = PKBInstance->GetAllProcedureNames();
    for (auto& iter : procedureNames) {
        resultList.emplace_back(kProcedure, RefValue(kFixedValue, iter));
    }
    return resultList;
}

vector<Ref> DataRequester::GetAllSelectedReference(Ref selectedRef) {
    if (selectedRef.IsVariable()) {
        return GetAllVariable();
    }
    else if (selectedRef.IsStatement()) {
        return GetAllStmtRefByType(selectedRef.GetType());
    }
    else if (selectedRef.IsConstant()) {
        return GetAllConstant();
    }
    else if (selectedRef.IsProcedure()) {
        return GetAllProcedure();
    }
    else {
    }
}

RelationResultTable DataRequester::GetRelationTable(Relation relation) {
    RelationType type = relation.getRelationType();
    if (type == kFollows || type == kFollowst || type == kParent || type == kParentt) {
        return GetFollowsParentsTable(relation);
    }
    else if (type == kUsess || type == kModifiess) {
        RelationResultTable result = GetModifiesSUsesSTable(relation);
        return result;
    }
    else if (type == kUsesp || type == kModifiesp) {
        return GetModifiesPUsesPTable(relation);
    }
    else if (type == kCalls || type == kCallst) {
        return GetCallsTable(relation);
    }
    else if (type == kNext || type == kNextt) {
        return GetNextTable(relation);
    }
    else if (type == kAffects || type == kAffectst) {
        return GetAffectTable(relation);
    }
    else {
    }
}

vector<Ref> DataRequester::GetAssignTableByPattern(Pattern pattern) {
    if (pattern.GetExpression()->GetType() == kFullMatch) {
        return s_statementNumberToRef(PKBInstance->GetStatementsBySameExpr(pattern.GetExpression()->GetExpression()),
            kAssign);
    }
    else if (pattern.GetExpression()->GetType() == kPartialMatch) {
        return s_statementNumberToRef(PKBInstance->GetStatementsBySubExpr(pattern.GetExpression()->GetExpression()),
            kAssign);
    }
    else if (pattern.GetExpression()->GetType() == kAnyMatch) {
        return GetAllStmtRefByType(kAssign);
    }
}

void DataRequester::CollectElementListForSynAndWhile(vector<Ref> &statementList, EntType &patternStmtType, vector<PatternResultElement> &elementList) {
    for (auto& iter : statementList) {
        set<VarName> controlVariables = patternStmtType == kWhile ?
                                        PKBInstance->GetWhileControlVarsByStatement(iter.getName()->GetValue())
                                                                  : PKBInstance->GetIfControlVarsByStatement(iter.getName()->GetValue());

        for (auto& variable : controlVariables) {
            elementList.push_back(
                    PatternResultElement(iter, { kVariable, {kFixedValue, variable.GetName()} }, {}));
        }
    }
}

void DataRequester::CollectElementListForFixedValue(EntType &patternStmtType,
                                                    vector<PatternResultElement> &elementList, Pattern &pattern) {
    VarName requestedControlVar = { pattern.GetArg2()->getName()->GetName() };
    set<StatementNumber> stmtNumbers = patternStmtType == kWhile ?
                                       PKBInstance->GetWhileStatementsByVar(requestedControlVar)
                                                                 : PKBInstance->GetIfStatementsByVar(requestedControlVar);

    for (auto& stmt : stmtNumbers) {
        Ref stmtRef;
        if (patternStmtType == kWhile) {
            stmtRef = Ref(kWhile, RefValue(kNumber, stmt.GetLine()));
        }
        else {
            stmtRef = Ref(kIf, RefValue(kNumber, stmt.GetLine()));
        }

        elementList.push_back(PatternResultElement(stmtRef,
                                                   { kVariable, {kFixedValue, requestedControlVar.GetName()} }, {}));
    }
}

PatternResultTable DataRequester::GetPatternResultTableForWhileIfSyn(EntType &patternStmtType, vector<Ref> &statementList, Pattern &pattern) {
    if (pattern.GetArg2()->getName()->GetType() == kSynonym ||
        pattern.GetArg2()->getName()->GetType() == kWildcard) {
        vector<PatternResultElement> elementList;
        CollectElementListForSynAndWhile(statementList, patternStmtType, elementList);
        return { *(pattern.GetArg1()), *(pattern.GetArg2()), elementList };
    }
    else if (pattern.GetArg2()->getName()->GetType() == kFixedValue) {
        vector<PatternResultElement> elementList;
        CollectElementListForFixedValue(patternStmtType, elementList, pattern);
        return { *(pattern.GetArg1()), *(pattern.GetArg2()), elementList };
    }
}

void DataRequester::CollectPatternResultListForSyn(vector<PatternResultElement> &patternResultList, vector<Ref> &statementList, Pattern &pattern) {
    for (auto& iter : statementList) {
        set<VarName> modified = PKBInstance->GetModifiesByStatement(iter.getName()->GetValue());
        patternResultList.emplace_back(Ref(kAssign, RefValue(kNumber, iter.getName()->GetValue())),
                                       Ref(kVariable, RefValue(kFixedValue, modified.begin()->GetName())),
                                       *(pattern.GetExpression()));
    }
}

void DataRequester::CollectPatternResultListForFixedValue(vector<PatternResultElement> &tempList,
                                                          vector<Ref> &statementList, Pattern &pattern, bool &flag) {
    for (auto& iter : statementList) {
        set<VarName> modified = PKBInstance->GetModifiesByStatement(iter.getName()->GetValue());
        if (modified.begin()->GetName() == pattern.GetArg2()->getName()->GetName()) {
            tempList.emplace_back(Ref(kAssign, RefValue(kNumber, iter.getName()->GetValue())),
                                  *(pattern.GetArg2()), *(pattern.GetExpression()));
            flag = true;
        }
    }
}

PatternResultTable DataRequester::GetPatternTable(Pattern pattern) {
    EntType patternStmtType = pattern.GetArg1()->GetType();
    if (patternStmtType == kWhile || patternStmtType == kIf) {
        vector<Ref> statementList = GetAllStmtRefByType(patternStmtType);
        if (pattern.GetArg1()->getName()->GetType() == kSynonym) {
            PatternResultTable pattern_result_table_for_while_if_synonym = GetPatternResultTableForWhileIfSyn(patternStmtType, statementList, pattern);
            return pattern_result_table_for_while_if_synonym;
        }
        return {};
    }
    else if (patternStmtType == kAssign) {
        vector<Ref> statementList = GetAssignTableByPattern(pattern);
        if (pattern.GetArg1()->getName()->GetType() == kSynonym) {
            if (pattern.GetArg2()->getName()->GetType() == kWildcard || pattern.arg2.getName()->GetType() == kSynonym) {
                vector<PatternResultElement> patternResultList;
                CollectPatternResultListForSyn(patternResultList, statementList, pattern);
                return { *(pattern.GetArg1()), *(pattern.GetArg2()), patternResultList };
            }
            else if (pattern.GetArg2()->getName()->GetType() == kFixedValue) {
                bool flag = false;
                vector<PatternResultElement> tempList;
                CollectPatternResultListForFixedValue(tempList, statementList, pattern, flag);
                if (flag) {
                    return { *(pattern.GetArg1()), *(pattern.GetArg2()), tempList };
                }
                else {
                    return { *(pattern.GetArg1()), *(pattern.GetArg2()) };
                }
            }
        }
    }
    return {};
}

RelationResultTable DataRequester::GetModifySynWildcard(Relation &relation, RelationType &relationType) {
    if (relationType == kModifiess) {
        return {relationType, *(relation.GetArgument1()), *(relation.GetArgument2()),
                GetRelationArgByArg(kModifiess, false, *relation.GetArgument1(), kVariable)};
    }
    else if (relationType == kUsess) {
        return {relationType, *(relation.GetArgument1()), *(relation.GetArgument2()),
                GetRelationArgByArg(kUsess, false, *relation.GetArgument1(), kVariable)};
    }
}

RelationResultTable DataRequester::GetModifyFixedValue(Relation &relation, RelationType &relationType) {
    if (relationType == kModifiess) {
        if (CheckRelationStands(relation)) {
            vector<RelationResultPair> templist;
            templist.emplace_back(*(relation.GetArgument1()), *(relation.GetArgument2()));
            return RelationResultTable(relationType, *(relation.GetArgument1()), *(relation.GetArgument2()),
                                       templist);
        }
        else {
            return RelationResultTable(relationType, *(relation.GetArgument1()), *(relation.GetArgument2()));
        }
    }
    else if (relationType == kUsess) {
        if (CheckRelationStands(relation)) {
            vector<RelationResultPair> templist;
            templist.push_back(RelationResultPair(*(relation.GetArgument1()), *(relation.GetArgument2())));
            return RelationResultTable(relationType, *(relation.GetArgument1()), *(relation.GetArgument2()),
                                       templist);
        }
        else {
            return RelationResultTable(relationType, *(relation.GetArgument1()), *(relation.GetArgument2()));
        }
    }
}

RelationResultTable DataRequester::GetModifyKNumberTable(Relation &relation, RelationType &relationType) {
    if (relation.GetArgument2()->getName()->GetType() == kSynonym ||
        relation.GetArgument2()->getName()->GetType() == kWildcard) {
        RelationResultTable modify_syn_wildcard = GetModifySynWildcard(relation, relationType);
        return modify_syn_wildcard;
    }
    else if (relation.GetArgument2()->getName()->GetType() == kFixedValue) {
        RelationResultTable modify_fixed_value = GetModifyFixedValue(relation, relationType);
        return modify_fixed_value;
    }
}

RelationResultTable DataRequester::GetModifykSynonymTable(Relation &relation, RelationType &relationType) {
    if (relation.GetArgument2()->getName()->GetType() == kSynonym ||
        relation.GetArgument2()->getName()->GetType() == kWildcard) {
        if (relationType == kModifiess) {
            return RelationResultTable(relationType, *(relation.GetArgument1()), *(relation.GetArgument2()),
                                       GetAllRelationByType(kModifiess, relation.GetArgument1()->GetType(),
                                                            relation.GetArgument2()->GetType()));
        }
        else if (relationType == kUsess) {
            return RelationResultTable(relationType, *(relation.GetArgument1()), *(relation.GetArgument2()),
                                       GetAllRelationByType(kUsess, relation.GetArgument1()->GetType(),
                                                            relation.GetArgument2()->GetType()));
        }
        else {

        }
    }
    else if (relation.GetArgument2()->getName()->GetType() == kFixedValue) {
        if (relationType == kModifiess) {
            return RelationResultTable(relationType, *(relation.GetArgument1()), *(relation.GetArgument2()),
                                       GetRelationArgByArg(kModifiess, true, *(relation.GetArgument2()),
                                                           relation.GetArgument1()->GetType()));
        }
        else if (relationType == kUsess) {
            return RelationResultTable(relationType, *(relation.GetArgument1()), *(relation.GetArgument2()),
                                       GetRelationArgByArg(kUsess, true, *(relation.GetArgument2()),
                                                           relation.GetArgument1()->GetType()));
        }
    }
}

RelationResultTable DataRequester::GetModifiesSUsesSTable(Relation relation) {
    RelationType relationType = relation.getRelationType();
    if (relation.GetArgument1()->getName()->GetType() == kNumber) {
        RelationResultTable modify_kNumber_table = GetModifyKNumberTable(relation, relationType);
        return modify_kNumber_table;
    }
    else if (relation.GetArgument1()->getName()->GetType() == kSynonym) {
        RelationResultTable modify_kSynonym_table = GetModifykSynonymTable(relation, relationType);
        return modify_kSynonym_table;
    }
}

RelationResultTable DataRequester::GetModifyUsePKNumberTable(Relation &relation, RelationType &relationType) {
    if (relation.GetArgument2()->getName()->GetType() == kSynonym ||
        relation.GetArgument2()->getName()->GetType() == kWildcard) {
        if (relationType == kModifiesp) {
            return RelationResultTable(relationType, *(relation.GetArgument1()), *(relation.GetArgument2()),
                                       GetAllRelationByType(kModifiesp, relation.GetArgument1()->GetType(),
                                                            relation.GetArgument2()->GetType()));
        }
        else if (relationType == kUsesp) {
            return RelationResultTable(relationType, *(relation.GetArgument1()), *(relation.GetArgument2()),
                                       GetAllRelationByType(kUsesp, relation.GetArgument1()->GetType(),
                                                            relation.GetArgument2()->GetType()));
        }
    }
    else if (relation.GetArgument2()->getName()->GetType() == kFixedValue) {
        if (relationType == kModifiesp) {
            return RelationResultTable(relationType, *(relation.GetArgument1()), *(relation.GetArgument2()),
                                       GetRelationArgByArg(kModifiesp, true, *relation.GetArgument2(), kProcedure));
        }
        else if (relationType == kUsesp) {
            return RelationResultTable(relationType, *(relation.GetArgument1()), *(relation.GetArgument2()),
                                       GetRelationArgByArg(kUsesp, true, *relation.GetArgument2(), kProcedure));
        }
    }
}

RelationResultTable DataRequester::GetModifyUsePkFixedValueTable(Relation &relation, RelationType &relationType) {
    if (relation.GetArgument2()->getName()->GetType() == kSynonym ||
        relation.GetArgument2()->getName()->GetType() == kWildcard) {
        if (relationType == kModifiesp) {
            return RelationResultTable(relationType, *(relation.GetArgument1()), *(relation.GetArgument2()),
                                       GetRelationArgByArg(kModifiesp, false, *(relation.GetArgument1()),
                                                           kVariable));
        }
        else if (relationType == kUsesp) {
            return RelationResultTable(relationType, *(relation.GetArgument1()), *(relation.GetArgument2()),
                                       GetRelationArgByArg(kUsesp, false, *(relation.GetArgument1()), kVariable));
        }
    }
    else if (relation.GetArgument2()->getName()->GetType() == kFixedValue) {
        if (relationType == kModifiesp) {
            if (CheckRelationStands(relation)) {
                vector<RelationResultPair> tempList;
                tempList.push_back(RelationResultPair(*(relation.GetArgument1()), *(relation.GetArgument2())));
                return RelationResultTable(relationType, *(relation.GetArgument1()), *(relation.GetArgument2()),
                                           tempList);
            }
            else {
                return RelationResultTable(relationType, *(relation.GetArgument1()), *(relation.GetArgument2()));
            }
        }
        else if (relationType == kUsesp) {
            if (CheckRelationStands(relation)) {
                vector<RelationResultPair> tempList;
                tempList.push_back(RelationResultPair(*(relation.GetArgument1()), *(relation.GetArgument2())));
                return RelationResultTable(relationType, *(relation.GetArgument1()), *(relation.GetArgument2()),
                                           tempList);
            }
            else {
                return RelationResultTable(relationType, *(relation.GetArgument1()), *(relation.GetArgument2()));
            }
        }
    }
}

void DataRequester::CollectWithVarandPForCall(set<VarName>& vars, set<ProcedureName>& procs, EntType& secondArgEntType, EntType& firstArgEntType, vector<pair<Ref, Ref>>& temp_list) {
    for (auto& iter1 : vars) {
        for (auto& iter2 : procs) {
            if (iter1.GetName() != iter2) {
                continue;
            }
            set<StatementNumber> stmtLine = GetStatementNumberFromPKBByName(secondArgEntType, iter2);
            for (auto& s : stmtLine) {
                temp_list.push_back(GetWithComplexNameHelper(firstArgEntType, secondArgEntType, kFixedValue, kNumber, s.GetLine(), -1, iter2));
            }
        }
    }
}

RelationResultTable DataRequester::GetModifiesPUsesPTable(Relation relation) {
    RelationType relationType = relation.getRelationType();
    if (relation.GetArgument1()->getName()->GetType() == kSynonym) {
        RelationResultTable modify_use_p_kSynonym_table = GetModifyUsePKNumberTable(relation, relationType);
        return modify_use_p_kSynonym_table;
    }
    else if (relation.GetArgument1()->getName()->GetType() == kFixedValue) {
        RelationResultTable modify_use_p_kFixedValue_table = GetModifyUsePkFixedValueTable(relation, relationType);
        return modify_use_p_kFixedValue_table;
    }
}

RelationResultTable DataRequester::GetFollowParentSynWCTable(Relation &relation, RelationType &relationType) {
    if (relation.GetArgument1()->getName()->GetType() != kWildcard && relation.GetArgument1()->equals(
            relation.GetArgument2())) {
        return RelationResultTable(relationType, *(relation.GetArgument1()), *(relation.GetArgument2()));
    }
    if (relationType == kFollows) {
        return RelationResultTable(relationType, *(relation.GetArgument1()), *(relation.GetArgument2()),
                                   GetAllRelationByType(kFollows, relation.GetArgument1()->GetType(),
                                                        relation.GetArgument2()->GetType()));
    }
    else if (relationType == kParent) {
        return RelationResultTable(relationType, *(relation.GetArgument1()), *(relation.GetArgument2()),
                                   GetAllRelationByType(kParent, relation.GetArgument1()->GetType(),
                                                        relation.GetArgument2()->GetType()));
    }
    else if (relationType == kFollowst) {
        return RelationResultTable(relationType, *(relation.GetArgument1()), *(relation.GetArgument2()),
                                   GetAllRelationByType(kFollowst, relation.GetArgument1()->GetType(),
                                                        relation.GetArgument2()->GetType()));
    }
    else if (relationType == kParentt) {
        return RelationResultTable(relationType, *(relation.GetArgument1()), *(relation.GetArgument2()),
                                   GetAllRelationByType(kParentt, relation.GetArgument1()->GetType(),
                                                        relation.GetArgument2()->GetType()));
    }
}

RelationResultTable DataRequester::GetFollowParentKNumberTable(Relation &relation, RelationType &relationType) {
    if (relationType == kFollows) {
        return RelationResultTable(relationType, *(relation.GetArgument1()), *(relation.GetArgument2()),
                                   GetRelationArgByArg(kFollows, true, *(relation.GetArgument2()),
                                                       relation.GetArgument1()->GetType()));
    }
    else if (relationType == kParent) {
        return RelationResultTable(relationType, *(relation.GetArgument1()), *(relation.GetArgument2()),
                                   GetRelationArgByArg(kParent, true, *(relation.GetArgument2()),
                                                       relation.GetArgument1()->GetType()));
    }
    else if (relationType == kFollowst) {
        return RelationResultTable(relationType, *(relation.GetArgument1()), *(relation.GetArgument2()),
                                   GetRelationArgByArg(kFollowst, true, *(relation.GetArgument2()),
                                                       relation.GetArgument1()->GetType()));
    }
    else if (relationType == kParentt) {
        return RelationResultTable(relationType, *(relation.GetArgument1()), *(relation.GetArgument2()),
                                   GetRelationArgByArg(kParentt, true, *(relation.GetArgument2()),
                                                       relation.GetArgument1()->GetType()));
    }
}

RelationResultTable DataRequester::GetFollowParentKNumberWCTable(Relation &relation, RelationType &relationType) {
    if (relationType == kFollows) {
        return RelationResultTable(relationType, *(relation.GetArgument1()), *(relation.GetArgument2()),
                                   GetRelationArgByArg(kFollows, false, *(relation.GetArgument1()),
                                                       relation.GetArgument2()->GetType()));
    }
    else if (relationType == kParent) {
        return RelationResultTable(relationType, *(relation.GetArgument1()), *(relation.GetArgument2()),
                                   GetRelationArgByArg(kParent, false, *(relation.GetArgument1()),
                                                       relation.GetArgument2()->GetType()));
    }
    else if (relationType == kFollowst) {
        return RelationResultTable(relationType, *(relation.GetArgument1()), *(relation.GetArgument2()),
                                   GetRelationArgByArg(kFollowst, false, *(relation.GetArgument1()),
                                                       relation.GetArgument2()->GetType()));
    }
    else if (relationType == kParentt) {
        return RelationResultTable(relationType, *(relation.GetArgument1()), *(relation.GetArgument2()),
                                   GetRelationArgByArg(kParentt, false, *(relation.GetArgument1()),
                                                       relation.GetArgument2()->GetType()));
    }
}

RelationResultTable DataRequester::GetFollowParentSynKNumberTable(Relation &relation, RelationType &relationType) {
    if (relationType == kFollows) {
        if (CheckRelationStands(relation)) {
            vector<RelationResultPair> templist;
            templist.emplace_back(*(relation.GetArgument1()), *(relation.GetArgument2()));
            return RelationResultTable(relationType, *(relation.GetArgument1()), *(relation.GetArgument2()),
                                       templist);
        }
        else {
            return RelationResultTable(relationType, *(relation.GetArgument1()), *(relation.GetArgument2()));
        }
    } else if (relationType == kParent) {
        if (CheckRelationStands(relation)) {
            vector<RelationResultPair> templist;
            templist.emplace_back(*(relation.GetArgument1()), *(relation.GetArgument2()));
            return RelationResultTable(relationType, *(relation.GetArgument1()), *(relation.GetArgument2()),
                                       templist);
        }
        else {
            return RelationResultTable(relationType, *(relation.GetArgument1()), *(relation.GetArgument2()));
        }
    } else if (relationType == kFollowst) {
        if (CheckRelationStands(relation)) {
            vector<RelationResultPair> templist;
            templist.emplace_back(*(relation.GetArgument1()), *(relation.GetArgument2()));
            return RelationResultTable(relationType, *(relation.GetArgument1()), *(relation.GetArgument2()),
                                       templist);
        }
        else {
            return RelationResultTable(relationType, *(relation.GetArgument1()), *(relation.GetArgument2()));
        }
    } else if (relationType == kParentt) {
        if (CheckRelationStands(relation)) {
            vector<RelationResultPair> templist;
            templist.emplace_back(*(relation.GetArgument1()), *(relation.GetArgument2()));
            return RelationResultTable(relationType, *(relation.GetArgument1()), *(relation.GetArgument2()),
                                       templist);
        }
        else {
            return RelationResultTable(relationType, *(relation.GetArgument1()), *(relation.GetArgument2()));
        }
    }
}

void DataRequester::CollectWithPandCall(set<ProcedureName>& procs, set<ProcedureName>& callProcs, EntType& secondArgEntType, EntType& firstArgEntType, vector<pair<Ref, Ref>>& temp_list) {
    for (auto& iter1 : procs) {
        for (auto& iter2 : callProcs) {
            if (iter1 != iter2) {
                continue;
            }
            set<StatementNumber> stmtLine = GetStatementNumberFromPKBByName(secondArgEntType, iter2);
            for (auto& s : stmtLine) {
                temp_list.push_back(
                    GetWithComplexNameHelper(firstArgEntType, secondArgEntType, kFixedValue, kNumber, s.GetLine(), -1, iter1));
            }
        }
    }
}

RelationResultTable DataRequester::GetFollowsParentsTable(Relation relation) {
    RelationType relationType = relation.getRelationType();
    if (relation.GetArgument1()->getName()->GetType() == kSynonym ||
        relation.GetArgument1()->getName()->GetType() == kWildcard) {
        if (relation.GetArgument2()->getName()->GetType() == kSynonym ||
            relation.GetArgument2()->getName()->GetType() == kWildcard) {
            RelationResultTable Follow_Parent_kSynonym_kWildcard_table = GetFollowParentSynWCTable(relation, relationType);
            return Follow_Parent_kSynonym_kWildcard_table;
        }
        else if (relation.GetArgument2()->getName()->GetType() == kNumber) {
            RelationResultTable Follow_Parent_KNumber_table = GetFollowParentKNumberTable(relation, relationType);
            return Follow_Parent_KNumber_table;
        }
    }
    else if (relation.GetArgument1()->getName()->GetType() == kNumber) {
        if (relation.GetArgument2()->getName()->GetType() == kSynonym ||
            relation.GetArgument2()->getName()->GetType() == kWildcard) {
            RelationResultTable Follow_Parent_kSynonym_kWildcard_kNumber_table = GetFollowParentKNumberWCTable(relation, relationType);
            return Follow_Parent_kSynonym_kWildcard_kNumber_table;
        }

        else if (relation.GetArgument2()->getName()->GetType() == kNumber) {
            RelationResultTable Follow_Parent_kNumber_table = GetFollowParentSynKNumberTable(relation, relationType);
            return Follow_Parent_kNumber_table;
        }
    }
}

RelationResultTable DataRequester::GetCallsTable(Relation relation) {
    RelationType type = relation.getRelationType();
    if (relation.GetArgument1()->getName()->GetType() == kSynonym ||
        relation.GetArgument1()->getName()->GetType() == kWildcard) {
        if (relation.GetArgument2()->getName()->GetType() == kSynonym ||
            relation.GetArgument2()->getName()->GetType() == kWildcard) {
            return RelationResultTable(type, *(relation.GetArgument1()), *(relation.GetArgument2()),
                GetAllRelationByType(type, relation.GetArgument1()->GetType(),
                    relation.GetArgument2()->GetType()));
        }
        else if (relation.GetArgument2()->getName()->GetType() == kFixedValue) {
            return RelationResultTable(type, *(relation.GetArgument1()), *(relation.GetArgument2()),
                GetRelationArgByArg(type, true, *(relation.GetArgument2()),
                    relation.GetArgument1()->GetType()));
        }
    }
    else if (relation.GetArgument1()->getName()->GetType() == kFixedValue) {
        if (relation.GetArgument2()->getName()->GetType() == kSynonym ||
            relation.GetArgument2()->getName()->GetType() == kWildcard) {
            return RelationResultTable(type, *(relation.GetArgument1()), *(relation.GetArgument2()),
                GetRelationArgByArg(type, false, *(relation.GetArgument1()),
                    relation.GetArgument2()->GetType()));
        }
        else if (relation.GetArgument2()->getName()->GetType() == kFixedValue) {
            if (CheckRelationStands(relation)) {
                vector<RelationResultPair> tempList;
                tempList.emplace_back(*(relation.GetArgument1()), *(relation.GetArgument2()));
                return RelationResultTable(type, *(relation.GetArgument1()), *(relation.GetArgument2()), tempList);
            }
            else {
                return RelationResultTable(type, *(relation.GetArgument1()), *(relation.GetArgument2()));
            }
        }
    }
}

RelationResultTable DataRequester::GetNextSynWC(ValueType &secondArgType, RelationType &relationType, ValueType &firstArgType, Ref* firstArg, Ref* secondArg) {
    if (secondArgType == kSynonym || secondArgType == kWildcard) {

        if (firstArgType != kWildcard && firstArg->equals(secondArg) && relationType == kNext) {
            return RelationResultTable(relationType, *(firstArg), *(secondArg));
        }

        if (firstArgType != kWildcard && firstArg->equals(secondArg) && relationType == kNextt) {
            return RelationResultTable(relationType, *(firstArg), *(secondArg),
                                       GetRelationSameArg(relationType, firstArg->GetType()));
        }

        if (relationType == kNext || relationType == kNextt) {
            return RelationResultTable(relationType, *(firstArg), *(secondArg),
                                       GetAllRelationByType(relationType, firstArg->GetType(), secondArg->GetType()));
        }
    }
    else if (secondArgType == kNumber) {
        if (relationType == kNext || relationType == kNextt) {
            return RelationResultTable(relationType, *(firstArg), *(secondArg),
                                       GetRelationArgByArg(relationType, true, *(secondArg), firstArg->GetType()));
        }
    }
}

RelationResultTable DataRequester::GetNextKNumberWC(ValueType &secondArgType, RelationType &relationType,
                                                    ValueType &firstArgType, Ref* firstArg, Ref* secondArg, Relation &relation) {
    if (secondArgType == kSynonym || secondArgType == kWildcard) {
        if (relationType == kNext || relationType == kNextt) {
            return RelationResultTable(relationType, *(firstArg), *(secondArg),
                                       GetRelationArgByArg(relationType, false, *(firstArg), secondArg->GetType()));
        }
    }
    else if (secondArgType == kNumber) {
        if (relationType == kNext || relationType == kNextt) {
            if (CheckRelationStands(relation)) {
                vector<RelationResultPair> templist;
                templist.push_back(RelationResultPair(*(firstArg), *(secondArg)));
                return RelationResultTable(relationType, *(firstArg), *(secondArg), templist);
            }
            else {
                return RelationResultTable(relationType, *(firstArg), *(secondArg));
            }
        }
    }
}

RelationResultTable DataRequester::GetNextTable(Relation relation) {
    RelationType relationType = relation.getRelationType();
    Ref* firstArg = relation.GetArgument1();
    Ref* secondArg = relation.GetArgument2();

    ValueType firstArgType = firstArg->getName()->GetType();
    ValueType secondArgType = secondArg->getName()->GetType();

    if (firstArgType == kSynonym || firstArgType == kWildcard) {
        RelationResultTable Next_KSynonym_WC_Table = GetNextSynWC(secondArgType, relationType, firstArgType, firstArg, secondArg);
        return Next_KSynonym_WC_Table;
    }
    else if (firstArgType == kNumber) {
        RelationResultTable Next_KNumber_Table = GetNextKNumberWC(secondArgType, relationType, firstArgType, firstArg, secondArg, relation);
        return Next_KNumber_Table;
    }
}

void DataRequester::CollectResultListForOthers(RelationType &type, EntType &arg1Type, EntType &arg2Type, vector<RelationResultPair> &resultList) {
    vector<pair<StatementNumber, StatementNumber> > returnedList;
    switch (type) {
        case kFollows:
            returnedList = PKBInstance->GetAllFollows();
            break;
        case kFollowst:
            returnedList = PKBInstance->GetAllFollowsStar();
            break;
        case kParent:
            returnedList = PKBInstance->GetAllParent();
            break;
        case kParentt:
            returnedList = PKBInstance->GetAllParentStar();
            break;
        case kNext:
            returnedList = PKBInstance->GetAllNext();
            break;
        case kNextt:
            returnedList = PKBInstance->GetAllNextStar();
            break;
        case kAffects:
            returnedList = PKBInstance->GetAllAffects();
            break;
        case kAffectst:
            returnedList = PKBInstance->GetAllAffectsStar();
            break;
        default:
            break;
    }

    vector<Ref> arg1TypeList = GetAllStmtRefByType(arg1Type);
    vector<Ref> arg2TypeList = GetAllStmtRefByType(arg2Type);
    for (auto& iter : returnedList) {
        Ref tempArg1 = Ref(arg1Type, RefValue(kNumber, iter.first.GetLine()));
        Ref tempArg2 = Ref(arg2Type, RefValue(kNumber, iter.second.GetLine()));

        if (count(arg1TypeList.begin(), arg1TypeList.end(), tempArg1) > 0 &&
            count(arg2TypeList.begin(), arg2TypeList.end(), tempArg2) > 0) {
            resultList.emplace_back(tempArg1, tempArg2);
        }
    }
}

void DataRequester::CollectResultListForUsesModifes(RelationType &type, EntType &arg1Type, EntType &arg2Type, vector<RelationResultPair> &resultList) {
    vector<pair<StatementNumber, VarName> > returnedList;
    switch (type) {
        case kUsess:
            returnedList = PKBInstance->GetAllUses();
            break;
        case kModifiess:
            returnedList = PKBInstance->GetAllModifies();
            break;
        default:
            break;
    }
    vector<Ref> stmtTypeList = GetAllStmtRefByType(arg1Type);
    for (auto& iter : returnedList) {
        Ref tempStmt = Ref(arg1Type, RefValue(kNumber, iter.first.GetLine()));
        Ref tempVar = Ref(kVariable, RefValue(kFixedValue, iter.second.GetName()));
        if (count(stmtTypeList.begin(), stmtTypeList.end(), tempStmt) > 0) {
            resultList.emplace_back(tempStmt, tempVar);
        }
    }
}

void DataRequester::CollectResultListForUsesModifesProcedure(RelationType &type, EntType &arg1Type, EntType &arg2Type, vector<RelationResultPair> &resultList) {
    vector<pair<ProcedureName, VarName> > returnedList;
    switch (type) {
        case kUsesp:
            returnedList = PKBInstance->GetAllUsesP();
            break;
        case kModifiesp:
            returnedList = PKBInstance->GetAllModifiesP();
            break;
        default:
            break;
    }
    for (auto& iter : returnedList) {
        Ref tempProc = Ref(kProcedure, RefValue(kFixedValue, iter.first));
        Ref tempVar = Ref(kVariable, RefValue(kFixedValue, iter.second.GetName()));
        resultList.emplace_back(tempProc, tempVar);
    }
}

void DataRequester::CollectResultListForCalls(RelationType &type, EntType &arg1Type, EntType &arg2Type, vector<RelationResultPair> &resultList) {
    vector<pair<ProcedureName, ProcedureName> > returnedList;
    switch (type) {
        case kCalls:
            returnedList = PKBInstance->GetAllCalls();
            break;
        case kCallst:
            returnedList = PKBInstance->GetAllCallsStar();
            break;
        default:
            break;
    }
    for (auto& iter : returnedList) {
        Ref tempProc1 = Ref(kProcedure, RefValue(kFixedValue, iter.first));
        Ref tempProc2 = Ref(kProcedure, RefValue(kFixedValue, iter.second));
        resultList.emplace_back(tempProc1, tempProc2);
    }
}

vector<RelationResultPair> DataRequester::GetAllRelationByType(RelationType type, EntType arg1Type, EntType arg2Type) {
    vector<RelationResultPair> resultList;
    if (type == kFollows || type == kFollowst || type == kParent || type == kParentt
        || type == kNext || type == kNextt || type == kAffects || type == kAffectst) {
        CollectResultListForOthers(type, arg1Type, arg2Type, resultList);
    }
    else if (type == kUsess || type == kModifiess) {
        CollectResultListForUsesModifes(type, arg1Type, arg2Type, resultList);
    }
    else if (type == kUsesp || type == kModifiesp) {
        CollectResultListForUsesModifesProcedure(type, arg1Type, arg2Type, resultList);
    }
    else if (type == kCalls || type == kCallst) {
        CollectResultListForCalls(type, arg1Type, arg2Type, resultList);
    }
    return resultList;
}

vector<RelationResultPair> DataRequester::GetRelationSameArg(RelationType type, EntType argType) {
    vector<RelationResultPair> resultList;

    if (type == kAffects || type == kAffectst) {

        vector<Ref> argTypeList = GetAllStmtRefByType(argType);

        for (auto& iter : argTypeList) {
            StatementNumber sn = iter.getName()->GetValue();

            bool isTrueRelation = false;
            switch (type) {
            case kAffects:
                isTrueRelation = PKBInstance->GetAffects(sn, sn);
                break;
            case kAffectst:
                isTrueRelation = PKBInstance->GetAffectsStar(sn, sn);
                break;
            default:
                break;
            }
            if (isTrueRelation) {
                Ref tempArg = Ref(argType, RefValue(kNumber, sn.GetLine()));
                resultList.emplace_back(tempArg, tempArg);
            }
        }
    } else if (type == kNextt) {
        vector<Ref> argTypeList = GetAllStmtRefByType(argType);
        for (auto& iter : argTypeList) {
            StatementNumber sn = iter.getName()->GetValue();
            if (PKBInstance->GetNextStar(sn, sn)) {
                Ref temp = {argType, {kNumber, sn.GetLine()}};
                resultList.emplace_back(temp, temp);
            }
        }
    }

    return resultList;
}

void DataRequester::CollectRelationOtherResultList(RelationType &type, bool &getArg1, Ref &gettingBy, EntType &gettingArgType, vector<RelationResultPair> &resultList) {
    StatementNumber gettingByLine = StatementNumber(gettingBy.getName()->GetValue());
    set<StatementNumber> returnedSet;
    switch (type) {
        case kFollows:
            returnedSet = getArg1 ? PKBInstance->GetStatementsFollowedBy(gettingByLine) : PKBInstance->GetStatementsFollows(gettingByLine);
            break;
        case kFollowst:
            returnedSet = getArg1 ? PKBInstance->GetStatementsFollowedByStar(gettingByLine) : PKBInstance->GetStatementsFollowsStar(gettingByLine);
            break;
        case kParent:
            returnedSet = getArg1 ? PKBInstance->GetStatementsParentedBy(gettingByLine) : PKBInstance->GetStatementsParent(gettingByLine);
            break;
        case kParentt:
            returnedSet = getArg1 ? PKBInstance->GetStatementsParentedByStar(gettingByLine) : PKBInstance->GetStatementsParentStar(gettingByLine);
            break;
        case kNext:
            returnedSet = getArg1 ? PKBInstance->GetStatementsNextBefore(gettingByLine) : PKBInstance->GetStatementsNextAfter(gettingByLine);
            break;
        case kNextt:
            returnedSet = getArg1 ? PKBInstance->GetStatementsNextStarBefore(gettingByLine) : PKBInstance->GetStatementsNextStarAfter(gettingByLine);
            break;
        case kAffects:
            returnedSet = getArg1 ? PKBInstance->GetStatementsAffects(gettingByLine) : PKBInstance->GetStatementsAffectedBy(gettingByLine);
            break;
        case kAffectst:
            returnedSet = getArg1 ? PKBInstance->GetStatementsAffectsStar(gettingByLine) : PKBInstance->GetStatementsAffectedStarBy(gettingByLine);
            break;
        default:
            break;
    }
    vector<Ref> gettingArgTypeList = GetAllStmtRefByType(gettingArgType);
    for (auto& iter : returnedSet) {
        Ref temp = Ref(gettingArgType, RefValue(kNumber, iter.GetLine()));
        if (count(gettingArgTypeList.begin(), gettingArgTypeList.end(), temp) > 0) {
            if (getArg1) {
                resultList.emplace_back(temp, gettingBy);
            }
            else {
                resultList.emplace_back(gettingBy, temp);
            }
        }
    }
}

void DataRequester::CollectRelationUseModifyResultList(RelationType &type, bool &getArg1, Ref &gettingBy, EntType &gettingArgType, vector<RelationResultPair> &resultList) {
    if (getArg1) {
        VarName gettingByVar = VarName(gettingBy.getName()->GetName());
        set<StatementNumber> returnedSet = (type == kUsess) ? PKBInstance->GetUsesByVar(gettingByVar)
                                                            : PKBInstance->GetModifiesByVar(gettingByVar);
        vector<Ref> stmtTypeList = GetAllStmtRefByType(gettingArgType);
        for (auto& iter : returnedSet) {
            Ref temp = Ref(gettingArgType, RefValue(kNumber, iter.GetLine()));
            if (count(stmtTypeList.begin(), stmtTypeList.end(), temp) > 0) {
                resultList.emplace_back(temp, gettingBy);
            }
        }
    } else {
        StatementNumber gettingByStmt = StatementNumber(gettingBy.getName()->GetValue());
        set<VarName> returnedSet = (type == kUsess) ? PKBInstance->GetUsesByStatement(gettingByStmt)
                                                    : PKBInstance->GetModifiesByStatement(gettingByStmt);
        for (auto& iter : returnedSet) {
            resultList.emplace_back(gettingBy, Ref(kVariable, RefValue(kFixedValue, iter.GetName())));
        }
    }
}

void DataRequester::CollectRelationUseModifyProcedureResultList(RelationType &type, bool &getArg1, Ref &gettingBy, EntType &gettingArgType, vector<RelationResultPair> &resultList) {
    if (getArg1) {
        VarName gettingByVar = VarName(gettingBy.getName()->GetName());
        set<ProcedureName> returnedSet = (type == kUsesp) ? PKBInstance->GetUsesPByVar(gettingByVar)
                                                          : PKBInstance->GetModifiesPByVar(gettingByVar);
        for (auto& iter : returnedSet) {
            resultList.emplace_back(Ref(kProcedure, RefValue(kFixedValue, iter)), gettingBy);
        }
    }
    else {
        ProcedureName gettingByProcedure = ProcedureName(gettingBy.getName()->GetName());
        set<VarName> returnedSet = (type == kUsesp) ? PKBInstance->GetUsesPByProcedure(gettingByProcedure)
                                                    : PKBInstance->GetModifiesPByProcedure(gettingByProcedure);
        for (auto& iter : returnedSet) {
            resultList.emplace_back(gettingBy, Ref(kVariable, RefValue(kFixedValue, iter.GetName())));
        }
    }
}

void DataRequester::CollectRelationCallResultList(RelationType &type, bool &getArg1, Ref &gettingBy, EntType &gettingArgType, vector<RelationResultPair> &resultList) {
    ProcedureName gettingByProc = ProcedureName(gettingBy.getName()->GetName());
    set<ProcedureName> returnedSet;
    if (getArg1) {
        returnedSet = (type == kCalls) ? PKBInstance->GetProcedureNamesCalledBy(gettingByProc)
                                       : PKBInstance->GetProcedureNamesCalledStarBy(gettingByProc);
        for (auto& iter : returnedSet) {
            resultList.emplace_back(Ref(kProcedure, RefValue(kFixedValue, iter)), gettingBy);
        }
    }
    else {
        returnedSet = (type == kCalls) ? PKBInstance->GetProcedureNamesCalls(gettingByProc)
                                       : PKBInstance->GetProcedureNamesCallsStar(gettingByProc);
        for (auto& iter : returnedSet) {
            resultList.emplace_back(gettingBy, Ref(kProcedure, RefValue(kFixedValue, iter)));
        }
    }

}

vector<RelationResultPair>
DataRequester::GetRelationArgByArg(RelationType type, bool getArg1, Ref gettingBy, EntType gettingArgType) {
    vector<RelationResultPair> resultList;
    if (type == kFollows || type == kFollowst || type == kParent || type == kParentt
        || type == kNext || type == kNextt || type == kAffects || type == kAffectst) {
        CollectRelationOtherResultList(type, getArg1, gettingBy, gettingArgType, resultList);
    }
    else if (type == kUsess || type == kModifiess) {
        CollectRelationUseModifyResultList(type, getArg1, gettingBy, gettingArgType, resultList);
    }
    else if (type == kUsesp || type == kModifiesp) {
        CollectRelationUseModifyProcedureResultList(type, getArg1, gettingBy, gettingArgType, resultList);
    }
    else if (type == kCalls || type == kCallst) {
        CollectRelationCallResultList(type, getArg1, gettingBy, gettingArgType, resultList);
    }
    return resultList;
}

bool DataRequester::CheckRelationStands(Relation relation) {
    RelationType type = relation.getRelationType();
    bool result = false;
    if (type == kFollows || type == kFollowst || type == kParent || type == kParentt
        || type == kNext || type == kNextt || type == kAffects || type == kAffectst) {
        StatementNumber arg1 = StatementNumber(relation.GetArgument1()->getName()->GetValue());
        StatementNumber arg2 = StatementNumber(relation.GetArgument2()->getName()->GetValue());
        switch (type) {
        case kFollows:
            result = PKBInstance->GetFollows(arg1, arg2);
            break;
        case kFollowst:
            result = PKBInstance->GetFollowsStar(arg1, arg2);
            break;
        case kParent:
            result = PKBInstance->GetParent(arg1, arg2);
            break;
        case kParentt:
            result = PKBInstance->GetParentStar(arg1, arg2);
            break;
        case kNext:
            result = PKBInstance->GetNext(arg1, arg2);
            break;
        case kNextt:
            result = PKBInstance->GetNextStar(arg1, arg2);
            break;
        case kAffects:
            result = PKBInstance->GetAffects(arg1, arg2);
            break;
        case kAffectst:
            result = PKBInstance->GetAffectsStar(arg1, arg2);
            break;
        default:
            break;
        }
    }
    else if (type == kUsess || type == kModifiess) {
        StatementNumber arg1 = StatementNumber(relation.GetArgument1()->getName()->GetValue());
        VarName arg2 = VarName(relation.GetArgument2()->getName()->GetName());
        switch (type) {
        case kUsess:
            result = PKBInstance->GetUses(arg1, arg2);
            break;
        case kModifiess:
            result = PKBInstance->GetModifies(arg1, arg2);
            break;
        default:
            break;
        }
    }
    else if (type == kUsesp || type == kModifiesp) {
        ProcedureName arg1 = ProcedureName(relation.GetArgument1()->getName()->GetName());
        VarName arg2 = VarName(relation.GetArgument2()->getName()->GetName());
        switch (type) {
        case kUsesp:
            result = PKBInstance->GetUsesP(arg1, arg2);
            break;
        case kModifiesp:
            result = PKBInstance->GetModifiesP(arg1, arg2);
            break;
        default:
            break;
        }
    }
    else if (type == kCalls || type == kCallst) {
        ProcedureName arg1 = ProcedureName(relation.GetArgument1()->getName()->GetName());
        ProcedureName arg2 = ProcedureName(relation.GetArgument2()->getName()->GetName());
        switch (type) {
        case kCalls:
            result = PKBInstance->GetCalls(arg1, arg2);
            break;
        case kCallst:
            result = PKBInstance->GetCallsStar(arg1, arg2);
            break;
        default:
            break;
        }

    }
    return result;
}

WithResultTable DataRequester::GetWithTable(With withCondition) {
    Ref* firstArg = withCondition.GetArgument1();
    Ref* secondArg = withCondition.GetArgument2();

    ValueType firstArgType = firstArg->getName()->GetType();
    ValueType secondArgType = secondArg->getName()->GetType();

    string firstArgValue = firstArg->getName()->GetName();
    string secondArgValue = secondArg->getName()->GetName();

    EntType firstArgEntType = firstArg->GetType();
    EntType secondArgEntType = secondArg->GetType();

    ValueType firstAttributeType = firstArg->getAttribute()->GetType();
    ValueType secondAttributeType = secondArg->getAttribute()->GetType();

    if (firstArgType == kSynonym) {
        if (secondArgType == kSynonym) {
            if (firstArgEntType == secondArgEntType) {
                return WithResultTable(*(withCondition.GetArgument1()), *(withCondition.GetArgument2()),
                    GetWithSynBySyn(firstArg, secondArg));
            }
            else if (firstAttributeType == kNumber && secondAttributeType == kNumber) {
                return WithResultTable(*(withCondition.GetArgument1()), *(withCondition.GetArgument2()),
                    GetWithSynBySynComplexInt(firstArg, secondArg));
            }
            else if (firstAttributeType == kName && secondAttributeType == kName) {
                return WithResultTable(*(withCondition.GetArgument1()), *(withCondition.GetArgument2()),
                    GetWithSynBySynComplexName(firstArg, secondArg));
            }
        }
        else if (secondArgType == kNumber || secondArgType == kFixedValue) {
            return WithResultTable(*(withCondition.GetArgument1()), *(withCondition.GetArgument2()),
                GetWithArgByArg(firstArg, secondArg));
        }
    }
    else if (firstArgType == kNumber) {
        if (secondArgType == kNumber) {
            if (firstArgValue == secondArgValue) {
                vector<pair<Ref, Ref>> temp_list;
                temp_list.emplace_back(*(withCondition.GetArgument1()), *withCondition.GetArgument2());
                return WithResultTable(*(withCondition.GetArgument1()), *(withCondition.GetArgument2()), temp_list);
            }
        }
        else if (secondArgType == kSynonym) {
            return WithResultTable(*(withCondition.GetArgument2()), *(withCondition.GetArgument1()),
                GetWithArgByArg(secondArg, firstArg));
        }

    }
    else if (firstArgType == kFixedValue) {
        if (secondArgType == kFixedValue) {
            if (firstArgValue == secondArgValue) {
                vector<pair<Ref, Ref>> temp_list;
                temp_list.emplace_back(*(withCondition.GetArgument1()), *(withCondition.GetArgument2()));
                return WithResultTable(*(withCondition.GetArgument1()), *(withCondition.GetArgument2()), temp_list);
            }
        }
        else if (secondArgType == kSynonym) {
            return WithResultTable(*(withCondition.GetArgument2()), *(withCondition.GetArgument1()),
                GetWithArgByArg(secondArg, firstArg));
        }
    }
    return {};
}

vector<int> DataRequester::s_refVectorToIntVector(vector<Ref> refVector) {
    vector<int> resultList;
    for (auto& iter : refVector) {
        if (iter.getName()->GetType() == kNumber) {
            resultList.push_back(iter.getName()->GetValue());
        }
        else {
            resultList.push_back(-1);
        }
    }
    return resultList;
}

vector<Ref> DataRequester::s_statementNumberToRef(vector<StatementNumber> statements, EntType statementType) {
    vector<Ref> resultList;
    for (auto& iter : statements) {
        resultList.emplace_back(statementType, RefValue(kNumber, iter.GetLine()));
    }
    return resultList;
}

vector<Ref> DataRequester::s_statementNumberToRef(set<StatementNumber> statements, EntType statementType) {
    vector<Ref> resultList;
    for (auto& iter : statements) {
        resultList.emplace_back(statementType, RefValue(kNumber, iter.GetLine()));
    }
    return resultList;
}

string DataRequester::s_refVectorToString(vector<Ref> refList) {
    string result;
    for (auto& iter : refList) {
        result += iter.ToString() + ", ";
    }
    return result;
}

RelationResultTable DataRequester::GetAffectTable(Relation relation) {
    RelationType relationType = relation.getRelationType();
    Ref* firstArg = relation.GetArgument1();
    Ref* secondArg = relation.GetArgument2();

    ValueType firstArgType = firstArg->getName()->GetType();
    ValueType secondArgType = secondArg->getName()->GetType();

    if (firstArgType == kSynonym || firstArgType == kWildcard) {
        if (secondArgType == kSynonym || secondArgType == kWildcard) {
            if (firstArgType != kWildcard && firstArg->equals(secondArg)) {
                return RelationResultTable(relationType, *(firstArg), *(secondArg),
                    GetRelationSameArg(relationType, firstArg->GetType()));
            }

            if (relationType == kAffects || relationType == kAffectst) {
                return RelationResultTable(relationType, *(firstArg), *(secondArg),
                    GetAllRelationByType(relationType, firstArg->GetType(),
                        secondArg->GetType()));
            }
            else {
            }
        }
        else if (secondArgType == kNumber) {
            if (relationType == kAffects || relationType == kAffectst) {
                return RelationResultTable(relationType, *(firstArg), *(secondArg),
                    GetRelationArgByArg(relationType, true, *(secondArg), firstArg->GetType()));
            }
            else {
            }
        }
    }
    else if (firstArgType == kNumber) {
        if (secondArgType == kSynonym || secondArgType == kWildcard) {
            if (relationType == kAffects || relationType == kAffectst) {
                return RelationResultTable(relationType, *(firstArg), *(secondArg),
                    GetRelationArgByArg(relationType, false, *(firstArg), secondArg->GetType()));
            }
            else {
            }
        }
        else if (secondArgType == kNumber) {
            if (relationType == kAffects || relationType == kAffectst) {
                if (CheckRelationStands(relation)) {
                    vector<RelationResultPair> templist;
                    templist.emplace_back(*(firstArg), *(secondArg));
                    return RelationResultTable(relationType, *(firstArg), *(secondArg), templist);
                }
                else {
                    return RelationResultTable(relationType, *(firstArg), *(secondArg));
                }
            }
        }
    }
}

void DataRequester::CollectWithCallandP(set<ProcedureName>& callProcs, set<ProcedureName>& procs, EntType& secondArgEntType, EntType& firstArgEntType, vector<pair<Ref, Ref>>& temp_list) {
    for (auto& iter1 : callProcs) {
        for (auto& iter2 : procs) {
            if (iter1 != iter2) {
                continue;
            }
            set<StatementNumber> stmtLine = GetStatementNumberFromPKBByName(firstArgEntType, iter1);
            for (auto& s : stmtLine) {
                temp_list.push_back(
                    GetWithComplexNameHelper(firstArgEntType, secondArgEntType, kNumber, kFixedValue, s.GetLine(), -1, iter1));
            }
        }
    }
}

void DataRequester::CombineEmptyTable(vector<Ref> &resultList, vector<Ref> &holding) {
    for (auto& iterHolding : holding) {
        Ref temp;
        temp.SetType(kTupleType);
        temp.GetTuple()->push_back(iterHolding);
        resultList.emplace_back(temp);
    }
}

void DataRequester::CombineNoneEmptyTable(vector<Ref> &resultList, vector<Ref> &holding, vector<Ref> &rest) {
    for (auto& iterHolding : holding) {
        for (auto& iterRest : rest) {
            Ref temp;
            temp.SetType(kTupleType);
            temp.GetTuple()->push_back(iterHolding);
            for (auto& iter : *iterRest.GetTuple()) {
                temp.GetTuple()->push_back(iter);
            }
            resultList.emplace_back(temp);
        }
    }
}

vector<Ref> DataRequester::s_combination(vector<vector<Ref>>* dataList, int start) {
    vector<Ref> resultList;
    if (start < dataList->size()) {
        vector<Ref> rest = s_combination(dataList, start + 1);
        vector<Ref> holding = *(dataList->begin() + start);
        if (rest.empty()) {
            CombineEmptyTable(resultList, holding);
        }
        else {
            CombineNoneEmptyTable(resultList, holding, rest);
        }
    }
    return resultList;
}

pair<Ref, Ref> DataRequester::GetWithComplexIntHelper(EntType type_first, EntType type_sec, ValueType value_first, ValueType value_sec,
    int numIn, string str) {
    Ref first;
    Ref second;

    if (type_first == kConstant) {
        first = Ref(type_first, RefValue(value_sec, str));
        second = Ref(type_sec, RefValue(value_first, numIn));
    }
    else if (type_sec == kConstant) {
        first = Ref(type_first, RefValue(value_first, numIn));
        second = Ref(type_sec, RefValue(value_sec, str));
    }
    else {
        first = Ref(type_first, RefValue(value_first, numIn));
        second = Ref(type_sec, RefValue(value_sec, numIn));
    }
    return pair<Ref, Ref>(first, second);
}

vector<pair<Ref, Ref>> DataRequester::GetWithSynBySynComplexInt(Ref* firstArg, Ref* secondArg) {
    EntType firstArgEntType = firstArg->GetType();
    EntType secondArgEntType = secondArg->GetType();
    vector<pair<Ref, Ref>> temp_list;

    if ((firstArgEntType == kStmt &&
        (secondArgEntType == kRead || secondArgEntType == kPrint || secondArgEntType == kCall ||
            secondArgEntType == kWhile ||
            secondArgEntType == kIf || secondArgEntType == kAssign)) ||
        ((firstArgEntType == kRead || firstArgEntType == kPrint || firstArgEntType == kCall ||
            firstArgEntType == kWhile || firstArgEntType == kIf || firstArgEntType == kAssign) &&
            secondArgEntType == kStmt)) {

        StatementNumber maxStmt = PKBInstance->GetMaxStmtNumber();
        set<StatementNumber> stmts;

        if (firstArgEntType == kStmt) {
            stmts = GetStatementNumberFromPKB(secondArgEntType);
        }
        else {
            stmts = GetStatementNumberFromPKB(firstArgEntType);
        }

        for (auto& iter : stmts) {
            if (maxStmt.GetLine() >= iter.GetLine()) {
                temp_list.push_back(
                    GetWithComplexIntHelper(firstArgEntType, secondArgEntType, kNumber, kNumber, iter.GetLine(),
                        ""));
            }
        }
    }
    else if ((firstArgEntType == kRead &&
        (secondArgEntType == kPrint || secondArgEntType == kCall || secondArgEntType == kWhile ||
            secondArgEntType == kIf || secondArgEntType == kAssign)) ||
        (firstArgEntType == kPrint &&
            (secondArgEntType == kRead || secondArgEntType == kCall || secondArgEntType == kWhile ||
                secondArgEntType == kIf || secondArgEntType == kAssign)) ||
        (firstArgEntType == kCall &&
            (secondArgEntType == kPrint || secondArgEntType == kRead || secondArgEntType == kWhile ||
                secondArgEntType == kIf || secondArgEntType == kAssign)) ||
        (firstArgEntType == kWhile &&
            (secondArgEntType == kPrint || secondArgEntType == kCall || secondArgEntType == kRead ||
                secondArgEntType == kIf || secondArgEntType == kAssign)) ||
        (firstArgEntType == kIf &&
            (secondArgEntType == kPrint || secondArgEntType == kCall || secondArgEntType == kWhile ||
                secondArgEntType == kRead || secondArgEntType == kAssign)) ||
        (firstArgEntType == kAssign &&
            (secondArgEntType == kPrint || secondArgEntType == kCall || secondArgEntType == kWhile ||
                secondArgEntType == kRead || secondArgEntType == kIf))) {

        set<StatementNumber> firstStmt = GetStatementNumberFromPKB(firstArgEntType);
        set<StatementNumber> secondStmt = GetStatementNumberFromPKB(secondArgEntType);

        for (auto& iter1 : firstStmt) {
            auto iter2 = *(secondStmt.begin());
            if (iter1.GetLine() == iter2.GetLine()) {
                temp_list.push_back(
                    GetWithComplexIntHelper(firstArgEntType, secondArgEntType, kNumber, kNumber, iter1.GetLine(),
                        ""));
            }
        }
    }
    else if ((firstArgEntType == kConstant && secondArgEntType == kStmt) ||
        (firstArgEntType == kStmt && secondArgEntType == kConstant)) {
        set<Constant> constants = PKBInstance->GetAllConstants();
        StatementNumber maxStmt = PKBInstance->GetMaxStmtNumber();

        for (int i = 1; i <= maxStmt.GetLine(); i++) {
            for (auto& iter : constants) {
                if (to_string(i) == iter) {
                    temp_list.push_back(
                        GetWithComplexIntHelper(firstArgEntType, secondArgEntType, kNumber, kConstantStr, i,
                            to_string(i)));
                }
            }
        }
    }
    else if ((firstArgEntType == kConstant &&
        (secondArgEntType == kPrint || secondArgEntType == kCall || secondArgEntType == kWhile ||
            secondArgEntType == kRead || secondArgEntType == kIf || secondArgEntType == kAssign)) ||
        (secondArgEntType == kConstant &&
            (firstArgEntType == kPrint || firstArgEntType == kCall || firstArgEntType == kWhile ||
                firstArgEntType == kRead || firstArgEntType == kIf || firstArgEntType == kAssign ||
                firstArgEntType == kCall))) {

        set<Constant> constants = PKBInstance->GetAllConstants();
        set<StatementNumber> stmts;
        if (firstArgEntType == kConstant) {
            stmts = GetStatementNumberFromPKB(secondArgEntType);
        }
        else {
            stmts = GetStatementNumberFromPKB(firstArgEntType);
        }

        for (auto& iter1 : constants) {
            for (auto& iter2 : stmts) {
                if (iter1 == to_string(iter2.GetLine())) {
                    temp_list.push_back(
                        GetWithComplexIntHelper(firstArgEntType, secondArgEntType, kNumber, kConstantStr,
                            stoi(iter1), iter1));
                }
            }
        }
    }
    return temp_list;
}

set<int> DataRequester::StatementNumberToInt(set<StatementNumber> stmts) {
    set<int> stmtNumbers;
    for (auto& iter : stmts) {
        stmtNumbers.insert(iter.GetLine());
    }
    return stmtNumbers;
}

void DataRequester::CollectWithVandVForRP(set<VarName>& firstVars, set<VarName>& secondVars, EntType& secondArgEntType, EntType& firstArgEntType, vector<pair<Ref, Ref>>& temp_list) {
    for (auto& iter1 : firstVars) {
        for (auto& iter2 : secondVars) {
            if (iter1.GetName() != iter2.GetName()) {
                continue;
            }
            set<StatementNumber> stmtLine = GetStatementNumberFromPKBByName(firstArgEntType, iter1.GetName());
            set<StatementNumber> stmtLine2 = GetStatementNumberFromPKBByName(secondArgEntType, iter1.GetName());
            for (auto& s : stmtLine) {
                for (auto& s2 : stmtLine2) {
                    temp_list.push_back(GetWithComplexNameHelper(firstArgEntType, secondArgEntType, kNumber, kNumber, s.GetLine(), s2.GetLine(), ""));
                }
            }
        }
    }
}

vector<pair<Ref, Ref>> DataRequester::GetWithArgByArg(Ref* firstArg, Ref* secondArg) {
    string secondArgValue = secondArg->getName()->GetName();
    EntType firstArgEntType = firstArg->GetType();
    ValueType firstAttributeType = firstArg->getAttribute()->GetType();
    vector<pair<Ref, Ref>> temp_list;
    set<StatementNumber> statementNumbers = GetStatementNumberFromPKB(firstArgEntType);

    if (firstArgEntType == kStmt) {
        StatementNumber maxStmt = PKBInstance->GetMaxStmtNumber();
        for (int i = 1; i <= maxStmt.GetLine(); i++) {
            if (to_string(i) == secondArgValue) {
                temp_list.push_back(GetWithArgBySynHelper(firstArgEntType, kNumber, "", stoi(secondArgValue)));
            }
        }
    }
    else if (firstArgEntType == kRead || firstArgEntType == kPrint) {
        for (auto& iter : statementNumbers) {
            set<VarName> varNames = GetVarNamesFromPkbByLine(firstArgEntType, iter.GetLine());
            auto var = *(varNames.begin());
            if ((firstAttributeType == kNumber && to_string(iter.GetLine()) == secondArgValue) ||
                (firstAttributeType == kName && var.GetName() == secondArgValue)) {
                temp_list.push_back(GetWithArgBySynHelper(firstArgEntType, kNumber, "", iter.GetLine()));
            }
        }
    }
    else if (firstArgEntType == kCall) {
        for (auto& iter : statementNumbers) {
            set<ProcedureName> callProc = PKBInstance->GetCalledProcedureByStatement(iter.GetLine());
            auto proc = *(callProc.begin());
            if ((firstAttributeType == kNumber && to_string(iter.GetLine()) == secondArgValue) ||
                (firstAttributeType == kName && proc == secondArgValue)) {
                temp_list.push_back(GetWithArgBySynHelper(firstArgEntType, kNumber, "", iter.GetLine()));
            }
        }
    }
    else if (firstArgEntType == kWhile || firstArgEntType == kIf || firstArgEntType == kAssign) {
        for (auto& iter : statementNumbers) {
            if (to_string(iter.GetLine()) == secondArgValue) {
                temp_list.push_back(GetWithArgBySynHelper(firstArgEntType, kNumber, "", iter.GetLine()));
            }
        }
    }
    else if (firstArgEntType == kProcedure) {
        set<ProcedureName> procs = PKBInstance->GetAllProcedureNames();
        for (auto& iter : procs) {
            if (iter == secondArgValue) {
                temp_list.push_back(GetWithArgBySynHelper(firstArgEntType, kFixedValue, iter, -1));
            }
        }
    }
    else if (firstArgEntType == kVariable) {
        set<VarName> varNames = PKBInstance->GetAllVarNames();
        for (auto& iter : varNames) {
            if (iter.GetName() == secondArgValue) {
                temp_list.push_back(GetWithArgBySynHelper(firstArgEntType, kFixedValue, iter.GetName(), -1));
            }
        }
    }
    else if (firstArgEntType == kConstant) {
        set<Constant> constants = PKBInstance->GetAllConstants();
        for (auto& iter : constants) {
            if (iter == secondArgValue) {
                temp_list.push_back(GetWithArgBySynHelper(firstArgEntType, kConstantStr, iter, -1));
                break;
            }
        }
    }
    return temp_list;
}

pair<Ref, Ref> DataRequester::GetWithArgBySynHelper(EntType type, ValueType value, string name, int number) {
    Ref first;
    if (name != "") {
        first = Ref(type, RefValue(value, name));

    }
    else if (number != -1) {
        first = Ref(type, RefValue(value, number));
    }

    return pair<Ref, Ref>(first, first);
}

void DataRequester::CollectWithPandVar(set<ProcedureName>& procs, set<VarName>& vars, EntType& secondArgEntType, EntType& firstArgEntType, vector<pair<Ref, Ref>>& temp_list) {
    for (auto& iter1 : procs) {
        for (auto& iter2 : vars) {
            if (iter1 == iter2.GetName()) {
                temp_list.push_back(
                    GetWithComplexNameHelper(firstArgEntType, secondArgEntType, kFixedValue, kFixedValue, -1, -1, iter1));
            }
        }
    }
}

vector<pair<Ref, Ref>> DataRequester::GetWithSynBySyn(Ref* firstArg, Ref* secondArg) {
    EntType firstArgEntType = firstArg->GetType();
    EntType secondArgEntType = secondArg->GetType();
    vector<pair<Ref, Ref>> temp_list;

    if (firstArgEntType == kProcedure && secondArgEntType == kProcedure) {
        set<ProcedureName> procs = PKBInstance->GetAllProcedureNames();
        for (auto& iter : procs) {
            temp_list.push_back(GetWithArgBySynHelper(firstArgEntType, kFixedValue, iter, -1));
        }
    }
    else if (firstArgEntType == kStmt) {
        StatementNumber maxStmt = PKBInstance->GetMaxStmtNumber();
        for (int i = 1; i <= maxStmt.GetLine(); i++) {
            temp_list.push_back(GetWithArgBySynHelper(firstArgEntType, kNumber, "", i));
        }
    }
    else if (firstArgEntType == kVariable) {
        set<VarName> varNames = PKBInstance->GetAllVarNames();
        for (auto& iter : varNames) {
            temp_list.push_back(GetWithArgBySynHelper(firstArgEntType, kFixedValue, iter.GetName(), -1));
        }
    }
    else if (firstArgEntType == kConstant) {
        set<Constant> constants = PKBInstance->GetAllConstants();
        for (auto& iter : constants) {
            temp_list.push_back(GetWithArgBySynHelper(firstArgEntType, kConstantStr, iter, -1));
        }
    }
    else {
        set<StatementNumber> statementNumbers = GetStatementNumberFromPKB(firstArgEntType);
        for (auto& iter : statementNumbers) {
            temp_list.push_back(GetWithArgBySynHelper(firstArgEntType, kNumber, "", iter.GetLine()));
        }
    }
    return temp_list;
}

set<StatementNumber> DataRequester::GetStatementNumberFromPKB(EntType entType) {
    set<StatementNumber> list;
    switch (entType) {
    case (kRead):
        list = PKBInstance->GetAllReadStatements();
        break;
    case (kPrint):
        list = PKBInstance->GetAllPrintStatements();
        break;
    case (kCall):
        list = PKBInstance->GetAllCallStatements();
        break;
    case (kWhile):
        list = PKBInstance->GetAllWhileStatements();
        break;
    case (kIf):
        list = PKBInstance->GetAllIfStatements();
        break;
    case (kAssign):
        for (StatementNumber s : PKBInstance->GetAllAssignStatements()) {
            list.insert(s);
        }
        break;
    }
    return list;
}

set<StatementNumber> DataRequester::GetStatementNumberFromPKBByName(EntType entType, string name) {
    set<StatementNumber> list;
    switch (entType) {
    case (kRead):
        list = PKBInstance->GetReadStatementsByVar(name);
        break;
    case (kPrint):
        list = PKBInstance->GetPrintStatementsByVar(name);
        break;
    case (kCall):
        list = PKBInstance->GetCallStatementsByProcedure(name);
        break;
    }
    return list;
}

set<VarName> DataRequester::GetVarNamesFromPKB(EntType entType) {
    set<VarName> list;
    switch (entType) {
    case (kRead):
        list = PKBInstance->GetAllReadVariables();
        break;
    case (kPrint):
        list = PKBInstance->GetAllPrintedVariables();
        break;
    case (kVariable):
        list = PKBInstance->GetAllVarNames();
        break;
    }
    return list;
}

void DataRequester::CollectWithVandVForProc(set<VarName>& firstVars, set<VarName>& secondVars, EntType& secondArgEntType, EntType& firstArgEntType, vector<pair<Ref, Ref>>& temp_list) {
    for (auto& iter1 : firstVars) {
        for (auto& iter2 : secondVars) {
            if (iter1.GetName() != iter2.GetName()) {
                continue;
            }
            set<StatementNumber> stmtLine = GetStatementNumberFromPKBByName(firstArgEntType, iter1.GetName());
            for (auto& s : stmtLine) {
                temp_list.push_back(GetWithComplexNameHelper(firstArgEntType, secondArgEntType, kNumber, kFixedValue, s.GetLine(), -1, iter1.GetName()));
            }
        }
    }
}

set<ProcedureName> DataRequester::GetProcNamesFromPKB(EntType entType) {
    set<ProcedureName> list;
    switch (entType) {
    case (kProcedure):
        list = PKBInstance->GetAllProcedureNames();
        break;
    case (kCall):
        list = PKBInstance->GetAllCalledProcedures();
        break;
    }
    return list;
}

set<VarName> DataRequester::GetVarNamesFromPkbByLine(EntType entType, int stmtNum) {
    set<VarName> list;
    switch (entType) {
    case (kRead):
        list = PKBInstance->GetReadVarByStatement(stmtNum);
        break;
    case (kPrint):
        list = PKBInstance->GetPrintedVarByStatement(stmtNum);
        break;
    }
    return list;
}

pair<Ref, Ref> DataRequester::GetWithComplexNameHelper(EntType type_first, EntType type_sec, ValueType value_first, ValueType value_sec, int numIn, int numSec, string str) {
    Ref first;
    Ref second;
    if (value_first == kNumber && value_sec == kNumber) {
        first = Ref(type_first, RefValue(value_first, numIn));
        second = Ref(type_sec, RefValue(value_sec, numSec));
    }
    else if (value_first == kFixedValue && value_sec == kFixedValue) {
        first = Ref(type_first, RefValue(value_first, str));
        second = Ref(type_sec, RefValue(value_sec, str));
    }
    else if (value_first == kFixedValue && value_sec == kNumber) {
        first = Ref(type_first, RefValue(value_first, str));
        second = Ref(type_sec, RefValue(value_sec, numIn));
    }
    else if (value_first == kNumber && value_sec == kFixedValue) {
        first = Ref(type_first, RefValue(value_first, numIn));
        second = Ref(type_sec, RefValue(value_sec, str));
    }
    return pair<Ref, Ref>(first, second);
}

void DataRequester::CollectWithPandVarForCall(set<ProcedureName>& procs, set<VarName>& vars, EntType& secondArgEntType, EntType& firstArgEntType, vector<pair<Ref, Ref>>& temp_list) {
    for (auto& iter1 : procs) {
        for (auto& iter2 : vars) {
            if (iter1 != iter2.GetName()) {
                continue;
            }
             set<StatementNumber> stmtLine = GetStatementNumberFromPKBByName(firstArgEntType, iter1);
             for (auto& s : stmtLine) {
                temp_list.push_back(
                    GetWithComplexNameHelper(firstArgEntType, secondArgEntType, kNumber, kFixedValue, s.GetLine(), -1, iter1));
             }
        }
    }
}

vector<pair<Ref, Ref>> DataRequester::GetWithSynBySynComplexName(Ref* firstArg, Ref* secondArg) {
    EntType firstArgEntType = firstArg->GetType();
    EntType secondArgEntType = secondArg->GetType();
    vector<pair<Ref, Ref>> temp_list;

    if (firstArgEntType == kProcedure && secondArgEntType == kCall) {
        set<ProcedureName> procs = GetProcNamesFromPKB(firstArgEntType);
        set<ProcedureName> callProcs = GetProcNamesFromPKB(secondArgEntType);
        CollectWithPandCall(procs, callProcs, secondArgEntType, firstArgEntType, temp_list);
    }
    else if (firstArgEntType == kProcedure && (secondArgEntType == kVariable || secondArgEntType == kRead || secondArgEntType == kPrint)) {
        set<ProcedureName> procs = GetProcNamesFromPKB(firstArgEntType);
        set<VarName> vars = GetVarNamesFromPKB(secondArgEntType);
        CollectWithPandVar(procs, vars, secondArgEntType, firstArgEntType, temp_list);
    }
    else if (firstArgEntType == kCall && secondArgEntType == kProcedure) {
        set<ProcedureName> callProcs = GetProcNamesFromPKB(firstArgEntType);
        set<ProcedureName> procs = GetProcNamesFromPKB(secondArgEntType);
        CollectWithCallandP(callProcs, procs, secondArgEntType, firstArgEntType, temp_list);
    }
    else if (firstArgEntType == kCall && (secondArgEntType == kVariable || secondArgEntType == kRead || secondArgEntType == kPrint)) {
        set<ProcedureName> procs = GetProcNamesFromPKB(firstArgEntType);
        set<VarName> var = GetVarNamesFromPKB(secondArgEntType);
        CollectWithPandVarForCall(procs, var, secondArgEntType, firstArgEntType, temp_list);
    }
    else if ((firstArgEntType == kRead && (secondArgEntType == kProcedure || secondArgEntType == kCall)) ||
        (firstArgEntType == kPrint && (secondArgEntType == kProcedure || secondArgEntType == kCall)) ||
        (firstArgEntType == kVariable && (secondArgEntType == kProcedure))) {
        set<VarName> vars = GetVarNamesFromPKB(firstArgEntType);
        set<ProcedureName> procs = GetProcNamesFromPKB(secondArgEntType);
        CollectWithPandVar(procs, vars, secondArgEntType, firstArgEntType, temp_list);
    }
    else if ((firstArgEntType == kRead && secondArgEntType == kVariable) || (firstArgEntType == kPrint && secondArgEntType == kVariable)) {
        set<VarName> firstVars = GetVarNamesFromPKB(firstArgEntType);
        set<VarName> secondVars = GetVarNamesFromPKB(secondArgEntType);
        CollectWithVandVForProc(firstVars, secondVars, secondArgEntType, firstArgEntType, temp_list);
    }
    else if ((firstArgEntType == kRead && secondArgEntType == kPrint) || (firstArgEntType == kPrint && secondArgEntType == kRead)) {
        set<VarName> firstVars = GetVarNamesFromPKB(firstArgEntType);
        set<VarName> secondVars = GetVarNamesFromPKB(secondArgEntType);
        CollectWithVandVForRP(firstVars, secondVars, secondArgEntType, firstArgEntType, temp_list);
        
    }
    else if (firstArgEntType == kVariable && secondArgEntType == kCall) {
        set<VarName> vars = GetVarNamesFromPKB(firstArgEntType);
        set<ProcedureName> procs = GetProcNamesFromPKB(secondArgEntType);
        CollectWithVarandPForCall(vars, procs, secondArgEntType, firstArgEntType, temp_list);
    }
    else if (firstArgEntType == kVariable && (secondArgEntType == kPrint || secondArgEntType == kRead)) {
        set<VarName> vars = GetVarNamesFromPKB(firstArgEntType);
        set<VarName> var = GetVarNamesFromPKB(secondArgEntType);
        CollectWithVandVForVar(var, vars, secondArgEntType, firstArgEntType, temp_list);
    }
    return temp_list;
}

void DataRequester::CollectWithVandVForVar(set<VarName>& var, set<VarName>& vars, EntType& secondArgEntType, EntType& firstArgEntType, vector<pair<Ref, Ref>>& temp_list) {
    for (auto& iter1 : var) {
        for (auto& iter2 : vars) {
            if (iter1.GetName() != iter2.GetName()) {
                continue;
            }
            set<StatementNumber> stmtLine = GetStatementNumberFromPKBByName(secondArgEntType, iter2.GetName());;
            for (auto& s : stmtLine) {
                temp_list.push_back(
                    GetWithComplexNameHelper(firstArgEntType, secondArgEntType, kFixedValue, kNumber, s.GetLine(), -1, iter1.GetName()));
            }
        }
    }
}

vector<Ref> DataRequester::GetAttributeValue(vector<Ref>& RefList, RefValue attributes, EntType type) {
    vector<Ref> resultList;
    if (type == kRead && attributes.GetName() == "varName") {
        for (auto& iter : RefList) {
            VarName modifiedVar = *(PKBInstance->GetModifiesByStatement(iter.getName()->GetValue()).begin());
            resultList.push_back({ kVariable, {kFixedValue, modifiedVar.GetName()} });
        }
    }
    else if (type == kPrint && attributes.GetName() == "varName") {
        for (auto& iter : RefList) {
            VarName modifiedVar = *(PKBInstance->GetUsesByStatement(iter.getName()->GetValue()).begin());
            resultList.push_back({ kVariable, {kFixedValue, modifiedVar.GetName()} });
        }
    }
    else if (type == kCall && attributes.GetName() == "procName") {
        for (auto& iter : RefList) {
            ProcedureName calledProcedure = *(PKBInstance->GetCalledProcedureByStatement(
                iter.getName()->GetValue()).begin());
            resultList.push_back({ kProcedure, {kFixedValue, calledProcedure} });
        }
    }
    return resultList;
}

bool DataRequester::validateRelation(Relation relation) {
    if ((relation.GetArgument1()->getName()->GetType() == kWildcard && relation.GetArgument2()->getName()->GetType() == kWildcard)) {
        bool valid = false;
        switch (relation.getRelationType()) {
            case kFollows:
                valid = !PKBInstance->IsFollowsEmpty();
                break;
            case kFollowst:
                valid = !PKBInstance->IsFollowsStarEmpty();
                break;
            case kParent:
                valid = !PKBInstance->IsParentEmpty();
                break;
            case kParentt:
                valid = !PKBInstance->IsParentStarEmpty();
                break;
            case kUsess:
                valid = !PKBInstance->IsUsesEmpty();
                break;
            case kUsesp:
                valid = !PKBInstance->IsUsesPEmpty();
                break;
            case kModifiess:
                valid = !PKBInstance->IsModifiesEmpty();
                break;
            case kModifiesp:
                valid = !PKBInstance->IsModifiesPEmpty();
                break;
            case kCalls:
                valid = !PKBInstance->IsCallsEmpty();
                break;
            case kCallst:
                valid = !PKBInstance->IsCallsStarEmpty();
                break;
            case kNext:
                valid = !PKBInstance->IsNextEmpty();
                break;
            case kNextt:
                valid = !PKBInstance->IsNextStarEmpty();
                break;
            case kAffects:
                valid = !PKBInstance->IsAffectsEmpty();
                break;
            case kAffectst:
                valid = !PKBInstance->IsAffectsStarEmpty();
                break;
            default:
                break;
        }
        return valid;
    } else {
        RelationResultTable result = GetRelationTable(relation);
        int size = result.size();
        return size > 0;
    }
}

bool DataRequester::validatePattern(Pattern pattern) {
    return GetPatternTable(pattern).size() > 0;
}

bool DataRequester::validateWith(With withCondition) {
    return GetWithTable(withCondition).size() > 0;
}
