#include "PKB.h"
#include "SourceProcessor/DesignExtractor.h"

using namespace std;

PKB::PKB() {
    this->storage = PKBStorage();
    this->cfg = new CFG(&storage);
}

PKB::~PKB() {
    delete cfg;
    cfg = NULL;
}

Procedure* PKB::WithinSameProc(const StatementNumber& sn1, const StatementNumber& sn2) {
    Procedure* proc1 = GetResideProcedure(sn1);
    Procedure* proc2 = GetResideProcedure(sn2);
    if (proc1 == nullptr || proc2 == nullptr || proc1->GetIndex() != proc2->GetIndex()) {
        return nullptr;
    }
    else {
        return proc1;
    }
}

// APIs for Modify

bool PKB::GetModifies(const StatementNumber& sn, const VarName& vn) {
    return storage.GetRelationStmtVar(storage.modify_table, sn, vn);
}

bool PKB::IsModifiesStatementExist(const StatementNumber& sn) {
    return storage.modify_table.IsAttribute1Exist(sn);
}

bool PKB::IsModifiesVarExist(const VarName& vn) {
    return storage.IsVarExist(storage.modify_table, vn);
}

bool PKB::IsModifiesEmpty() {
    return storage.modify_table.IsEmpty();
}

set<VarName> PKB::GetModifiesByStatement(const StatementNumber& no) {
    return storage.GetVarsByStmt(storage.modify_table, no);
}

set<StatementNumber> PKB::GetModifiesByVar(const VarName& vn) {
    return storage.GetStmtsByVar(storage.modify_table, vn);
}

set<StatementNumber> PKB::GetAllModifiesStatements() {
    return *storage.modify_table.GetAllAttribute1();
}

set<VarName> PKB::GetAllModifiesVars() {
    return storage.GetAllVars(storage.modify_table);
}

vector<pair<StatementNumber, VarName>> PKB::GetAllModifies() {
    return storage.GetAllStmtVarPairs(storage.modify_table);
}

bool PKB::InsertModifies(const StatementNumber& no, const VarName& vn) {
    InsertVariable(vn);
    return storage.InsertStmtVarRelation(storage.modify_table, no, vn);
}

void PKB::UpdateModifiesForCallStatements() {
    vector<pair<StatementNumber, ProcedureIndex>> call_stmts = *storage.call_table.GetAllRelations();
    for (typename vector<pair<StatementNumber, ProcedureIndex>>::const_iterator it = call_stmts.begin();
        it != call_stmts.end(); ++it) {
        set<VarIndex> var_indexes = *storage.modifyP_table.GetByAttribute1(it->second);
        storage.modify_table.AddAllRelations(it->first, var_indexes);
        set<StatementNumber> parents = GetStatementsParentedByStar(it->first);
        if (parents.empty()) {
            continue;
        }
        for (typename set<StatementNumber>::iterator it2 = parents.begin(); it2 != parents.end(); ++it2) {
            storage.modify_table.AddAllRelations(*it2, var_indexes);
        }
    }
}

// API for ModifiesP

bool PKB::GetModifiesP(const ProcedureName& pn, const VarName& vn) {
    return storage.GetRelationProcVar(storage.modifyP_table, pn, vn);
}

set<ProcedureName> PKB::GetModifiesPByVar(const VarName& vn) {
    return storage.GetProcsByVar(storage.modifyP_table, vn);
}

set<VarName> PKB::GetModifiesPByProcedure(const ProcedureName& pn) {
    return storage.GetVarsByProc(storage.modifyP_table, pn);
}

set<ProcedureName> PKB::GetAllModifiesProcedures() {
    return storage.GetAllProcs(storage.modifyP_table);
}

set<VarName> PKB::GetAllModifiesPVars() {
    return storage.GetAllVars(storage.modifyP_table);
}

vector<pair<ProcedureName, VarName>> PKB::GetAllModifiesP() {
    return storage.GetAllProcVarPairs(storage.modifyP_table);
}

bool PKB::InsertModifiesP(const ProcedureName& pn, const VarName& vn) {
    InsertVariable(vn);
    return storage.InsertProcVarRelation(storage.modifyP_table, pn, vn);
}

bool PKB::IsModifiesProcedureExist(const ProcedureName& pn)
{
    return storage.IsProcExist(storage.modifyP_table, pn);
}

bool PKB::IsModifiesPVarExist(const VarName& vn)
{
    return storage.IsVarExist(storage.modifyP_table, vn);
}

bool PKB::IsModifiesPEmpty()
{
    return storage.modifyP_table.IsEmpty();
}

void PKB::UpdateNestedModifiesP() {
    unordered_set<ProcedureIndex> visited;
    ComputeIndirectRelation(storage.calls_table, storage.modifyP_table, visited);
}

// APIs for Uses

bool PKB::GetUses(const StatementNumber& sn, const VarName& vn) {
    return storage.GetRelationStmtVar(storage.use_table, sn, vn);
}

bool PKB::IsUsesStatementExist(const StatementNumber& sn) {
    return storage.use_table.IsAttribute1Exist(sn);
}

bool PKB::IsUsesEmpty() {
    return storage.use_table.IsEmpty();
}

set<VarName> PKB::GetUsesByStatement(const StatementNumber& no) {
    return storage.GetVarsByStmt(storage.use_table, no);
}

set<StatementNumber> PKB::GetUsesByVar(const VarName& vn) {
    return storage.GetStmtsByVar(storage.use_table, vn);
}

set<StatementNumber> PKB::GetAllUsesStatements() {
    return *storage.use_table.GetAllAttribute1();
}

set<VarName> PKB::GetAllUsesVars() {
    return storage.GetAllVars(storage.use_table);
}

vector<pair<StatementNumber, VarName>> PKB::GetAllUses() {
    return storage.GetAllStmtVarPairs(storage.use_table);
}

bool PKB::InsertUses(const StatementNumber& no, const VarName& vn) {
    InsertVariable(vn);
    return storage.InsertStmtVarRelation(storage.use_table, no, vn);
}

bool PKB::IsUsesVarExist(const VarName& vn) {
    return storage.IsVarExist(storage.use_table, vn);
}


void PKB::UpdateUsesForCallStatements() {
    vector<pair<StatementNumber, ProcedureIndex>> call_stmts = *storage.call_table.GetAllRelations();
    for (typename vector<pair<StatementNumber, ProcedureIndex>>::const_iterator it = call_stmts.begin();
        it != call_stmts.end(); ++it) {
        set<VarIndex> var_indexes = *storage.useP_table.GetByAttribute1(it->second);
        storage.use_table.AddAllRelations(it->first, var_indexes);
        set<StatementNumber> parents = GetStatementsParentedByStar(it->first);
        if (parents.empty()) {
            continue;
        }
        for (typename set<StatementNumber>::iterator it2 = parents.begin(); it2 != parents.end(); ++it2) {
            storage.use_table.AddAllRelations(*it2, var_indexes);
        }
    }
}

// API for UsesP

bool PKB::GetUsesP(const ProcedureName& pn, const VarName& vn) {
    return storage.GetRelationProcVar(storage.useP_table, pn, vn);
}

bool PKB::IsUsesProcedureExist(const ProcedureName& pn) {
    return storage.IsProcExist(storage.useP_table, pn);
}

bool PKB::IsUsesPVarExist(const VarName& vn) {
    return storage.IsVarExist(storage.useP_table, vn);
}

bool PKB::IsUsesPEmpty() {
    return storage.useP_table.IsEmpty();
}

set<ProcedureName> PKB::GetUsesPByVar(const VarName& vn) {
    return storage.GetProcsByVar(storage.useP_table, vn);
}

set<VarName> PKB::GetUsesPByProcedure(const ProcedureName& pn) {
    return storage.GetVarsByProc(storage.useP_table, pn);
}

set<ProcedureName> PKB::GetAllUsesProcedures() {
    return storage.GetAllProcs(storage.useP_table);
}

set<VarName> PKB::GetAllUsesPVars() {
    return storage.GetAllVars(storage.useP_table);
}

vector<pair<ProcedureName, VarName>> PKB::GetAllUsesP() {
    return storage.GetAllProcVarPairs(storage.useP_table);
}

bool PKB::InsertUsesP(const ProcedureName& pn, const VarName& vn) {
    InsertVariable(vn);
    return storage.InsertProcVarRelation(storage.useP_table, pn, vn);
}

void PKB::UpdateNestedUsesP() {
    unordered_set<ProcedureIndex> visited;
    ComputeIndirectRelation(storage.calls_table, storage.useP_table, visited);
}

// APIs for Follow

bool PKB::GetFollows(const StatementNumber& sn1, const StatementNumber& sn2) {
    return storage.follow_table.GetRelation(sn1, sn2);
}

bool PKB::IsFollowsArg1Exist(const StatementNumber& sn) {
    return storage.follow_table.IsAttribute1Exist(sn);
}

bool PKB::IsFollowsArg2Exist(const StatementNumber& sn) {
    return storage.follow_table.IsAttribute2Exist(sn);
}

bool PKB::IsFollowsEmpty() {
    return storage.follow_table.IsEmpty();
}

set<StatementNumber> PKB::GetStatementsFollows(const StatementNumber& sn) {
    return *storage.follow_table.GetByAttribute1(sn);
}

set<StatementNumber> PKB::GetStatementsFollowedBy(const StatementNumber& sn) {
    return *storage.follow_table.GetByAttribute2(sn);
}

set<StatementNumber> PKB::GetAllFollowsFirst() {
    return *storage.follow_table.GetAllAttribute1();
}

set<StatementNumber> PKB::GetAllFollowsSecond() {
    return *storage.follow_table.GetAllAttribute2();
}

vector<pair<StatementNumber, StatementNumber>> PKB::GetAllFollows() {
    return *storage.follow_table.GetAllRelations();
}

bool PKB::InsertFollows(const StatementNumber& sn1, const StatementNumber& sn2) {
    return storage.follow_table.InsertRelation(sn1, sn2);
}

// API for Follow*
// Call this after all insertFollow have been finished
void PKB::InitFollowsStarTable() {
    ComputeTransitiveClosureForDAG(storage.follow_table, storage.follow_star_table);
}
bool PKB::GetFollowsStar(const StatementNumber& sn1, const StatementNumber& sn2) {
    return storage.follow_star_table.GetRelation(sn1, sn2);
}

bool PKB::IsFollowsStarArg1Exist(const StatementNumber& sn) {
    return storage.follow_star_table.IsAttribute1Exist(sn);
}

bool PKB::IsFollowsStarArg2Exist(const StatementNumber& sn) {
    return storage.follow_star_table.IsAttribute2Exist(sn);
}

bool PKB::IsFollowsStarEmpty() {
    return storage.follow_star_table.IsEmpty();
}

set<StatementNumber> PKB::GetStatementsFollowsStar(const StatementNumber& sn) {
    return *storage.follow_star_table.GetByAttribute1(sn);
}

set<StatementNumber> PKB::GetStatementsFollowedByStar(const StatementNumber& sn) {
    return *storage.follow_star_table.GetByAttribute2(sn);
}

set<StatementNumber> PKB::GetAllFollowsStarFirst() {
    return *storage.follow_star_table.GetAllAttribute1();
}

set<StatementNumber> PKB::GetAllFollowsStarSecond() {
    return *storage.follow_star_table.GetAllAttribute2();
}

vector<pair<StatementNumber, StatementNumber>> PKB::GetAllFollowsStar() {
    return *storage.follow_star_table.GetAllRelations();
}


// APIs for Parent

bool PKB::GetParent(const StatementNumber& sn, const StatementNumber& sn1) {
    return storage.parent_table.GetRelation(sn, sn1);
}

bool PKB::IsParentArg1Exist(const StatementNumber& sn) {
    return storage.parent_table.IsAttribute1Exist(sn);
}

bool PKB::IsParentArg2Exist(const StatementNumber& sn) {
    return storage.parent_table.IsAttribute2Exist(sn);
}

bool PKB::IsParentEmpty() {
    return storage.parent_table.IsEmpty();
}

set<StatementNumber> PKB::GetStatementsParent(const StatementNumber& sn) {
    return *storage.parent_table.GetByAttribute1(sn);
}

set<StatementNumber> PKB::GetStatementsParentedBy(const StatementNumber& sn) {
    return *storage.parent_table.GetByAttribute2(sn);
}

set<StatementNumber> PKB::GetAllParentFirst() {
    return *storage.parent_table.GetAllAttribute1();
}

set<StatementNumber> PKB::GetAllParentSecond() {
    return *storage.parent_table.GetAllAttribute2();
}

vector<pair<StatementNumber, StatementNumber>> PKB::GetAllParent() {
    return *storage.parent_table.GetAllRelations();
}

bool PKB::InsertParent(const StatementNumber& no, const StatementNumber& sn1) {
    return storage.parent_table.InsertRelation(no, sn1);
}

// API for Parent*
// Call this after all insertParent have been finished
void PKB::InitParentStarTable() {
    ComputeTransitiveClosureForDAG(storage.parent_table, storage.parent_star_table);
}
bool PKB::GetParentStar(const StatementNumber& sn1, const StatementNumber& sn2) {
    return storage.parent_star_table.GetRelation(sn1, sn2);
}

bool PKB::IsParentStarArg1Exist(const StatementNumber& sn) {
    return storage.parent_star_table.IsAttribute1Exist(sn);
}

bool PKB::IsParentStarArg2Exist(const StatementNumber& sn) {
    return storage.parent_star_table.IsAttribute2Exist(sn);
}

bool PKB::IsParentStarEmpty() {
    return storage.parent_star_table.IsEmpty();
}

set<StatementNumber> PKB::GetStatementsParentStar(const StatementNumber& sn) {
    return *storage.parent_star_table.GetByAttribute1(sn);
}

set<StatementNumber> PKB::GetStatementsParentedByStar(const StatementNumber& sn) {
    return *storage.parent_star_table.GetByAttribute2(sn);
}

set<StatementNumber> PKB::GetAllParentStarFirst() {
    return *storage.parent_star_table.GetAllAttribute1();
}

set<StatementNumber> PKB::GetAllParentStarSecond() {
    return *storage.parent_star_table.GetAllAttribute2();
}

vector<pair<StatementNumber, StatementNumber>> PKB::GetAllParentStar() {
    return *storage.parent_star_table.GetAllRelations();
}

// API for variable table
bool PKB::InsertVariable(const VarName& vn) {
    VarIndex vi = storage.var_name_hasher(vn);
    bool flag = storage.var_table.find(vi) == storage.var_table.end();
    if (flag) {
        storage.var_table[vi] = vn;
    }
    return flag;
}

set<VarName> PKB::GetAllVarNames() {
    set<VarName> result;
    for (typename unordered_map<VarIndex, VarName, hash<size_t>>::iterator it = storage.var_table.begin();
        it != storage.var_table.end(); ++it) {
        result.insert(it->second);
    }
    return result;
}

// API for statement number
void PKB::SetMaxStmtNumber(int max) {
    storage.max_stmt_number = max;
}

int PKB::GetMaxStmtNumber() const {
    return storage.max_stmt_number;
}

// API for constant
bool PKB::IsConstantInStmt(const StatementNumber& sn, const Constant c) {
    return storage.constant_table.GetRelation(sn, c);
}

set<StatementNumber> PKB::GetStatementsByConstant(const Constant c) {
    return *storage.constant_table.GetByAttribute2(c);
}
set<Constant> PKB::GetConstantsByStatement(const StatementNumber& sn) {
    return *storage.constant_table.GetByAttribute1(sn);
}

set<Constant> PKB::GetAllConstants() {
    return *storage.constant_table.GetAllAttribute2();
}

bool PKB::InsertConstant(const StatementNumber& sn, const Constant c) {
    return storage.constant_table.InsertRelation(sn, c);
}

// API for while statement table

void PKB::GetAllStatementWithControlVars(Table<StatementNumber, VarIndex, hashStatementNumber, hash<size_t>>& table,
    set<StatementNumber>& temp) {
    for (typename set<StatementNumber>::iterator it = temp.begin(); it != temp.end();) {
        set<VarIndex> indexs = *table.GetByAttribute1(*it);
        bool flag = true;
        for (typename set<VarIndex>::iterator it2 = indexs.begin(); it2 != indexs.end(); ++it2) {
            flag = flag && (*it2) == storage.invalidIndex;
        }
        if (flag) {
            temp.erase(it++);
        }
        else {
            it++;
        }
    }
}

bool PKB::InsertWhileStatement(const StatementNumber& sn, const VarName& vn) {
    if (vn.IsValid()) {
        InsertVariable(vn);
    }
    return storage.InsertStmtVarRelation(storage.while_table, sn, vn);
}

set<StatementNumber> PKB::GetAllWhileStatements() {
    return *storage.while_table.GetAllAttribute1();
}

set<StatementNumber> PKB::GetAllWhileStatementsWithControlVar() {
    set<StatementNumber> temp = *storage.while_table.GetAllAttribute1();
    GetAllStatementWithControlVars(storage.while_table, temp);
    return temp;
}

set<VarName> PKB::GetWhileControlVarsByStatement(const StatementNumber& sn) {
    return storage.GetValidVarsByStmt(storage.while_table, sn);
}

set<StatementNumber> PKB::GetWhileStatementsByVar(const VarName& vn) {
    if (!vn.IsValid()) {
        return set<StatementNumber>();
    }
    return storage.GetStmtsByVar(storage.while_table, vn);
}

vector<pair<StatementNumber, VarName>> PKB::GetAllWhile() {
    return storage.GetAllStmtVarPairs(storage.while_table);
}

bool PKB::IsWhile(const StatementNumber& sn) {
    return storage.while_table.IsAttribute1Exist(sn);
}

bool PKB::IsWhileControlVar(const VarName& vn) {
    if (!vn.IsValid()) {
        return false;
    }
    return storage.IsVarExist(storage.while_table, vn);
}

bool PKB::IsWhileEmpty() {
    return storage.while_table.IsEmpty();
}

// API for if statement
bool PKB::InsertIfStatement(const StatementNumber& sn, const VarName& vn) {
    if (vn.IsValid()) {
        InsertVariable(vn);
    }
    return storage.InsertStmtVarRelation(storage.if_table, sn, vn);
}

set<StatementNumber> PKB::GetAllIfStatements() {
    return *storage.if_table.GetAllAttribute1();
}

set<StatementNumber> PKB::GetAllIfStatementsWithControlVar() {
    set<StatementNumber> temp = *storage.if_table.GetAllAttribute1();
    GetAllStatementWithControlVars(storage.if_table, temp);
    return temp;
}

set<VarName> PKB::GetIfControlVarsByStatement(const StatementNumber& sn) {
    return storage.GetValidVarsByStmt(storage.if_table, sn);
}

set<StatementNumber> PKB::GetIfStatementsByVar(const VarName& vn) {
    return storage.GetStmtsByVar(storage.if_table, vn);
}

vector<pair<StatementNumber, VarName>> PKB::GetAllIf() {
    return storage.GetAllStmtVarPairs(storage.if_table);
}

bool PKB::IsIf(const StatementNumber& sn) {
    return storage.if_table.IsAttribute1Exist(sn);
}

bool PKB::IsIfControlVar(const VarName& vn) {
    if (!vn.IsValid()) {
        return false;
    }
    return storage.IsVarExist(storage.if_table, vn);
}

bool PKB::IsIfEmpty() {
    return storage.if_table.IsEmpty();
}

// API for read statement
bool PKB::InsertReadStatement(const StatementNumber& sn, const VarName& vn) {
    InsertVariable(vn);
    return storage.InsertStmtVarRelation(storage.read_table, sn, vn);
}

set<StatementNumber> PKB::GetReadStatementsByVar(const VarName& vn) {
    return storage.GetStmtsByVar(storage.read_table, vn);
}

set<VarName> PKB::GetReadVarByStatement(const StatementNumber& sn) {
    return storage.GetVarsByStmt(storage.read_table, sn);
}

set<StatementNumber> PKB::GetAllReadStatements() {
    return *storage.read_table.GetAllAttribute1();
}

set<VarName> PKB::GetAllReadVariables() {
    return storage.GetAllVars(storage.read_table);
}

vector<pair<StatementNumber, VarName>> PKB::GetAllReads() {
    return storage.GetAllStmtVarPairs(storage.read_table);
}

bool PKB::IsRead(const StatementNumber& sn) {
    return storage.read_table.IsAttribute1Exist(sn);
}

bool PKB::IsVarRead(const VarName& vn) {
    return storage.IsVarExist(storage.read_table, vn);
}

bool PKB::IsReadEmpty() {
    return storage.read_table.IsEmpty();
}

// API for print statement
bool PKB::InsertPrintStatement(const StatementNumber& sn, const VarName& vn) {
    InsertVariable(vn);
    return storage.InsertStmtVarRelation(storage.print_table, sn, vn);
}

set<StatementNumber> PKB::GetPrintStatementsByVar(const VarName& vn) {
    return storage.GetStmtsByVar(storage.print_table, vn);
}

set<VarName> PKB::GetPrintedVarByStatement(const StatementNumber& sn) {
    return storage.GetVarsByStmt(storage.print_table, sn);
}

set<StatementNumber> PKB::GetAllPrintStatements() {
    return *storage.print_table.GetAllAttribute1();
}

set<VarName> PKB::GetAllPrintedVariables() {
    return storage.GetAllVars(storage.print_table);
}

vector<pair<StatementNumber, VarName>> PKB::GetAllPrints() {
    return storage.GetAllStmtVarPairs(storage.print_table);
}

bool PKB::IsPrint(const StatementNumber& sn) {
    return storage.print_table.IsAttribute1Exist(sn);
}

bool PKB::IsVarPrint(const VarName& vn) {
    return storage.IsVarExist(storage.print_table, vn);
}

bool PKB::IsPrintEmpty() {
    return storage.print_table.IsEmpty();
}


// API for call statement
bool PKB::InsertCallStatement(const StatementNumber& sn, const ProcedureName& pn) {
    ProcedureIndex pi = storage.procedure_name_hasher(pn);
    return storage.call_table.InsertRelation(sn, pi);
}

set<StatementNumber> PKB::GetCallStatementsByProcedure(const ProcedureName& pn) {
    ProcedureIndex pi = storage.procedure_name_hasher(pn);
    return *storage.call_table.GetByAttribute2(pi);
}

set<ProcedureName> PKB::GetCalledProcedureByStatement(const StatementNumber& sn) {
    set<ProcedureName> res;
    storage.GetProcedureNameSet(*storage.call_table.GetByAttribute1(sn), res);
    return res;
}

set<StatementNumber> PKB::GetAllCallStatements() {
    return *storage.call_table.GetAllAttribute1();
}

set<ProcedureName> PKB::GetAllCalledProcedures() {
    set<ProcedureName> res;
    storage.GetProcedureNameSet(*storage.call_table.GetAllAttribute2(), res);
    return res;
}

vector<pair<StatementNumber, ProcedureName>> PKB::GetAllCallWithProc() {
    vector<pair<StatementNumber, ProcedureName>> res;
    storage.TransformProIndexesToProNamesWithStmt(*storage.call_table.GetAllRelations(), res);
    return res;
}

bool PKB::IsProcCalled(const ProcedureName& pn) {
    ProcedureIndex pi = storage.procedure_name_hasher(pn);
    return storage.call_table.IsAttribute2Exist(pi);
}

bool PKB::IsCall(const StatementNumber& sn) {
    return storage.call_table.IsAttribute1Exist(sn);
}

bool PKB::IsCallEmpty() {
    return storage.call_table.IsEmpty();
}

bool PKB::IsNonExistentCallStmtExist() {
    const set<ProcedureIndex>* all_called_procs = storage.call_table.GetAllAttribute2();
    for (typename set<ProcedureIndex>::const_iterator it = all_called_procs->begin(); it != all_called_procs->end(); ++it) {
        if (!storage.IsProcExist(*it)) {
            return true;
        }
    }
    return false;
}

// API for procedure
bool PKB::InsertProcedureName(const ProcedureName& pn) {
    ProcedureIndex pi = storage.procedure_name_hasher(pn);
    bool flag = storage.procedure_table.find(pi) == storage.procedure_table.end();
    if (flag) {
        Procedure* ptr = new Procedure(pn);
        storage.procedure_table[pi] = ptr;
    }
    return flag;
}

set<ProcedureName> PKB::GetAllProcedureNames() {
    set<ProcedureName> res;
    for (typename unordered_map<ProcedureIndex, Procedure*, hash<size_t>>::iterator it = storage.procedure_table.begin();
        it != storage.procedure_table.end(); ++it) {
        res.insert(it->second->GetName());
    }
    return res;
}

void PKB::SetProcedure(const ProcedureName& pn, int start, int end) {
    ProcedureIndex pi = storage.procedure_name_hasher(pn);
    storage.procedure_table[pi]->SetDetails(start, end);
}

Procedure* PKB::GetResideProcedure(const StatementNumber& sn) {
    return storage.GetResideProcedure(sn);
}

// API for Calls relation

bool PKB::GetCalls(const ProcedureName& pn1, const ProcedureName& pn2) {
    return storage.GetRelationProcs(storage.calls_table, pn1, pn2);
}

bool PKB::IsCallerExist(const ProcedureName& pn) {
    return storage.IsProc1Exist(storage.calls_table, pn);
}

bool PKB::IsCalleeExist(const ProcedureName& pn) {
    return storage.IsProc2Exist(storage.calls_table, pn);
}

bool PKB::IsCallsEmpty() {
    return storage.calls_table.IsEmpty();
}

set<ProcedureName> PKB::GetProcedureNamesCalls(const ProcedureName& pn) {
    return storage.GetProc1(storage.calls_table, pn);
}

set<ProcedureName> PKB::GetProcedureNamesCalledBy(const ProcedureName& pn) {
    return storage.GetProc2(storage.calls_table, pn);
}

set<ProcedureName> PKB::GetAllCallsFirst() {
    return storage.GetAllProc1s(storage.calls_table);
}

set<ProcedureName> PKB::GetAllCallsSecond() {
    return storage.GetAllProc2s(storage.calls_table);
}

vector<pair<ProcedureName, ProcedureName>> PKB::GetAllCalls() {
    return storage.GetAllProcPairs(storage.calls_table);
}

bool PKB::InsertCalls(const ProcedureName& pn1, const ProcedureName& pn2) {
    return storage.InsertProcsRelation(storage.calls_table, pn1, pn2);
}

// API for Calls* relation

bool PKB::InitCallsStarTable() {
    return DFSNoCycle(storage.calls_table, storage.calls_star_table);
}

bool PKB::GetCallsStar(const ProcedureName& pn1, const ProcedureName& pn2) {
    return storage.GetRelationProcs(storage.calls_star_table, pn1, pn2);
}

bool PKB::IsCallerStarExist(const ProcedureName& pn) {
    return storage.IsProc1Exist(storage.calls_star_table, pn);
}

bool PKB::IsCalleeStarExist(const ProcedureName& pn) {
    return storage.IsProc2Exist(storage.calls_star_table, pn);
}

bool PKB::IsCallsStarEmpty() {
    return storage.calls_star_table.IsEmpty();
}

set<ProcedureName> PKB::GetProcedureNamesCallsStar(const ProcedureName& pn) {
    return storage.GetProc1(storage.calls_star_table, pn);
}

set<ProcedureName> PKB::GetProcedureNamesCalledStarBy(const ProcedureName& pn) {
    return storage.GetProc2(storage.calls_star_table, pn);
}

set<ProcedureName> PKB::GetAllCallsStarFirst() {
    return storage.GetAllProc1s(storage.calls_star_table);
}

set<ProcedureName> PKB::GetAllCallsStarSecond() {
    return storage.GetAllProc2s(storage.calls_star_table);
}

vector<pair<ProcedureName, ProcedureName>> PKB::GetAllCallsStar() {
    return storage.GetAllProcPairs(storage.calls_star_table);
}

// API for assign table
bool PKB::InsertAssign(const StatementNumber& sn, const RPN& rpn) {
    if (storage.assign_table.find(sn) != storage.assign_table.end()) {
        return false;
    }
    else {
        storage.assign_table[sn] = rpn;
        return true;
    }
}

RPN PKB::GetAssignByStatement(const StatementNumber& sn) {
    if (storage.assign_table.find(sn) == storage.assign_table.end()) {
        return "";
    }
    else {
        return storage.assign_table[sn];
    }
}

// This one is expensive
// Need to loop through all the expressions
vector<StatementNumber> PKB::GetStatementsBySameExpr(const string& expr) {
    vector<StatementNumber> result;
    if (expr.empty()) {
        return result;
    }
    DesignExtractor dex = DesignExtractor();
    string rpn = dex.Rpn(expr);

    for (typename unordered_map<StatementNumber, string>::iterator it = storage.assign_table.begin();
        it != storage.assign_table.end(); ++it) {
        string cur = it->second;

        if (rpn.compare(cur) == 0) {
            result.push_back(it->first);
        }
    }

    return result;
}

// This one is expensive
// Need to loop through all the expressions
vector<StatementNumber> PKB::GetStatementsBySubExpr(const string& expr) {
    vector<StatementNumber> result;
    if (expr.empty()) {
        return result;
    }
    DesignExtractor dex = DesignExtractor();
    DesignExtractor comparator = DesignExtractor();
    string rpn = dex.Rpn(expr);

    for (typename unordered_map<StatementNumber, string>::iterator it = storage.assign_table.begin();
        it != storage.assign_table.end(); ++it) {
        string cur = it->second;

        if (comparator.KmpSearch(rpn, cur)) {
            result.push_back(it->first);
        }
    }

    return result;
}

vector<StatementNumber> PKB::GetAllAssignStatements() {
    storage.GenerateAssignStatements();
    return storage.assign_statements;
}

vector<pair<StatementNumber, RPN>> PKB::GetAllAssigns() {
    vector<pair<StatementNumber, RPN>> result(storage.assign_table.begin(), storage.assign_table.end());
    return result;
}

bool PKB::IsAssign(const StatementNumber& sn) {
    return storage.assign_table.find(sn) != storage.assign_table.end();
}

// API for next_table

bool PKB::GetNext(const StatementNumber& sn1, const StatementNumber& sn2) {
    return storage.next_table.GetRelation(sn1, sn2);
}

bool PKB::IsNextFirstExist(const StatementNumber& sn) {
    return storage.next_table.IsAttribute1Exist(sn);
}

bool PKB::IsNextSecondExist(const StatementNumber& sn) {
    return storage.next_table.IsAttribute2Exist(sn);
}

bool PKB::IsNextEmpty() {
    return storage.next_table.IsEmpty();
}

set<StatementNumber> PKB::GetStatementsNextAfter(const StatementNumber& sn) {
    return *storage.next_table.GetByAttribute1(sn);
}

set<StatementNumber> PKB::GetStatementsNextBefore(const StatementNumber& sn) {
    return *storage.next_table.GetByAttribute2(sn);
}

set<StatementNumber> PKB::GetAllStatementsNextAfter() {
    return *storage.next_table.GetAllAttribute1();
}

set<StatementNumber> PKB::GetAllStatementsNextBefore() {
    return *storage.next_table.GetAllAttribute2();
}

vector<pair<StatementNumber, StatementNumber>> PKB::GetAllNext() {
    return *storage.next_table.GetAllRelations();
}

void PKB::InitNextTable() {
    storage.GenerateThenElseStatements();
    for (typename unordered_map<ProcedureIndex, Procedure*, hash<size_t>>::iterator it = storage.procedure_table.begin();
        it != storage.procedure_table.end(); ++it) {
        GenerateNext(StatementNumber(it->second->GetStart()));
    }
}

// private function for next_table
void PKB::GenerateNext(const StatementNumber& start) {
    vector<StatementNumber> temp;
    GenerateNext(start, temp);
}

void PKB::GenerateNext(const StatementNumber& start, vector<StatementNumber>& last_stmts) {
    StatementNumber cur = start;

    while (true) {
        vector<StatementNumber> if_last_stmts;
        vector<StatementNumber> while_last_stmts;
        if (IsWhile(cur)) {
            GenerateNextForWhile(cur, while_last_stmts);
        }
        else if (IsIf(cur)) {
            GenerateNextForIf(cur, if_last_stmts);
        }

        set<StatementNumber> follower = GetStatementsFollows(cur);
        if (follower.empty()) {
            if (if_last_stmts.empty()) {
                last_stmts.push_back(cur);
            }
            else {
                last_stmts.insert(last_stmts.end(), if_last_stmts.begin(), if_last_stmts.end());
            }
            return;
        }
        else {
            typename set<StatementNumber>::iterator it = follower.begin();
            if (if_last_stmts.empty()) {
                storage.next_table.InsertRelation(cur, *it);
            }
            else {
                for (typename vector<StatementNumber>::const_iterator it1 = if_last_stmts.begin();
                    it1 != if_last_stmts.end(); ++it1) {
                    storage.next_table.InsertRelation(*it1, *it);
                }
            }
            cur = (*it);
        }
    }
}

void PKB::GenerateNextForWhile(const StatementNumber& ws, vector<StatementNumber>& last_stmts) {
    set<StatementNumber> children = GetStatementsParent(ws);
    // Since children is sorted by accesendingly, so begin() is the first element
    // at least one stmt in stmt list according to grammar
    typename set<StatementNumber>::iterator it = children.begin();
    StatementNumber first = *it;
    storage.next_table.InsertRelation(ws, first);
    GenerateNext(first, last_stmts);
    for (typename vector<StatementNumber>::const_iterator it = last_stmts.begin();
        it != last_stmts.end(); ++it) {
        storage.next_table.InsertRelation(*it, ws);
    }
}

void PKB::GenerateNextForIf(const StatementNumber& ifs, vector<StatementNumber>& last_stmts) {
    pair<StatementNumber, StatementNumber>* p = storage.then_else_table.at(ifs);
    StatementNumber then_first = p->first;
    StatementNumber else_first = p->second;
    storage.next_table.InsertRelation(ifs, then_first);
    GenerateNext(then_first, last_stmts);
    storage.next_table.InsertRelation(ifs, else_first);
    GenerateNext(else_first, last_stmts);
}

// API for Next*

bool PKB::GetNextStar(const StatementNumber& sn1, const StatementNumber& sn2) {
    Procedure* proc1 = WithinSameProc(sn1, sn2);
    if (proc1 == nullptr) {
        return false;
    }
    else {
        return cfg->GetNextStar(sn1, sn2, proc1);
    }
}

bool PKB::IsNextStarFirstExist(const StatementNumber& sn) {
    return IsNextFirstExist(sn);
}

bool PKB::IsNextStarSecondExist(const StatementNumber& sn) {
    return IsNextSecondExist(sn);
}

bool PKB::IsNextStarEmpty() {
    return IsNextEmpty();
}

set<StatementNumber> PKB::GetStatementsNextStarAfter(const StatementNumber& sn) {
    Procedure* proc = GetResideProcedure(sn);
    if (proc == nullptr) {
        return set<StatementNumber>();
    }
    else {
        return *(cfg->GetNextStarAfter(sn, proc));
    }
}

set<StatementNumber> PKB::GetStatementsNextStarBefore(const StatementNumber& sn) {
    Procedure* proc = GetResideProcedure(sn);
    if (proc == nullptr) {
        return set<StatementNumber>();
    }
    else {
        return *(cfg->GetNextStarBefore(sn, proc));
    }
}

set<StatementNumber> PKB::GetAllStatementsNextStarAfter() {
    return *(cfg->GetAllNextStarAfter());
}

set<StatementNumber> PKB::GetAllStatementsNextStarBefore() {
    return *(cfg->GetAllNextStarBefore());
}

vector<pair<StatementNumber, StatementNumber>> PKB::GetAllNextStar() {
    return *(cfg->GetAllNextStar());
}

bool PKB::GetAffects(const StatementNumber& sn1, const StatementNumber& sn2) {
    if (!(IsAssign(sn1) && IsAssign(sn2))) {
        return false;
    }
    Procedure* proc1 = WithinSameProc(sn1, sn2);
    if (proc1 == nullptr) {
        return false;
    }
    else {
        return cfg->GetAffects(sn1, sn2, proc1);
    }
}

bool PKB::IsAffectsFirstExist(const StatementNumber& sn) {
    if (!IsAssign(sn)) {
        return false;
    }
    Procedure* proc = GetResideProcedure(sn);
    if (proc == nullptr) {
        return false;
    }
    else {
        return cfg->IsAffectsFirstExist(sn, proc);
    }
}

bool PKB::IsAffectsSecondExist(const StatementNumber& sn) {
    if (!IsAssign(sn)) {
        return false;
    }
    Procedure* proc = GetResideProcedure(sn);
    if (proc == nullptr) {
        return false;
    }
    else {
        return cfg->IsAffectsSecondExist(sn, proc);
    }
}

bool PKB::IsAffectsEmpty() {
    return cfg->IsAffectsEmpty();
}

set<StatementNumber> PKB::GetStatementsAffectedBy(const StatementNumber& sn) {
    if (!IsAssign(sn)) {
        return set<StatementNumber>();
    }
    Procedure* proc = GetResideProcedure(sn);
    if (proc == nullptr) {
        return set<StatementNumber>();
    }
    else {
        return *(cfg->GetStatementsAffectedBy(sn, proc));
    }
}

set<StatementNumber> PKB::GetStatementsAffects(const StatementNumber& sn) {
    if (!IsAssign(sn)) {
        return set<StatementNumber>();
    }
    Procedure* proc = GetResideProcedure(sn);
    if (proc == nullptr) {
        return set<StatementNumber>();
    }
    else {
        return *(cfg->GetStatementsAffects(sn, proc));
    }
}

set<StatementNumber> PKB::GetAllStatementsAffects() {
    return *(cfg->GetAllStatementsAffects());
}

set<StatementNumber> PKB::GetAllStatementsAffected() {
    return *(cfg->GetAllStatementsAffected());
}

vector<pair<StatementNumber, StatementNumber>> PKB::GetAllAffects() {
    return *(cfg->GetAllAffects());
}

// API for affects star

bool PKB::GetAffectsStar(const StatementNumber& sn1, const StatementNumber& sn2) {
    if (!(IsAssign(sn1) && IsAssign(sn2))) {
        return false;
    }
    Procedure* proc1 = WithinSameProc(sn1, sn2);
    if (proc1 == nullptr) {
        return false;
    }
    else {
        return cfg->GetAffectsStar(sn1, sn2, proc1);
    }
}

bool PKB::IsAffectsStarFirstExist(const StatementNumber& sn) {
    return IsAffectsFirstExist(sn);
}

bool PKB::IsAffectsStarSecondExist(const StatementNumber& sn) {
    return IsAffectsSecondExist(sn);
}

bool PKB::IsAffectsStarEmpty() {
    return IsAffectsEmpty();
}

set<StatementNumber> PKB::GetStatementsAffectedStarBy(const StatementNumber& sn) {
    if (!IsAssign(sn)) {
        return set<StatementNumber>();
    }
    Procedure* proc = GetResideProcedure(sn);
    if (proc == nullptr) {
        return set<StatementNumber>();
    }
    else {
        return *(cfg->GetStatementsAffectedStarBy(sn, proc));
    }
}

set<StatementNumber> PKB::GetStatementsAffectsStar(const StatementNumber& sn) {
    if (!IsAssign(sn)) {
        return set<StatementNumber>();
    }
    Procedure* proc = GetResideProcedure(sn);
    if (proc == nullptr) {
        return set<StatementNumber>();
    }
    else {
        return *(cfg->GetStatementsAffectsStar(sn, proc));
    }
}

set<StatementNumber> PKB::GetAllStatementsAffectsStar() {
    return *(cfg->GetAllStatementsAffectsStar());
}

set<StatementNumber> PKB::GetAllStatementsAffectedStar() {
    return *(cfg->GetAllStatementsAffectedStar());
}

vector<pair<StatementNumber, StatementNumber>> PKB::GetAllAffectsStar() {
    return *(cfg->GetAllAffectsStar());
}