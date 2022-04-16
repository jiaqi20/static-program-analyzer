#include "PKBStorage.h"

PKBStorage::PKBStorage()
{
}

PKBStorage::~PKBStorage()
{
    for (typename unordered_map<StatementNumber, pair<StatementNumber, StatementNumber>*>::iterator it = then_else_table.begin();
        it != then_else_table.end();) {
        delete it->second;
        it->second = NULL;
        then_else_table.erase(it++);
    }
}

VarName PKBStorage::GetVarName(VarIndex vi) const {
    if (vi == invalidIndex) {
        return VarName("");
    }
    else {
        return var_table.at(vi);
    }
}

void PKBStorage::GetVarNameSet(const set<VarIndex>& varIndexes, set<VarName>& varNames) {
    for (typename set<VarIndex>::iterator it = varIndexes.begin(); it != varIndexes.end(); ++it) {
        VarName cur = GetVarName(*it);
        varNames.insert(cur);
    }
}

void PKBStorage::GetValidVarNameSet(const set<VarIndex>& varIndexes, set<VarName>& varNames) {
    for (typename set<VarIndex>::iterator it = varIndexes.begin(); it != varIndexes.end(); ++it) {
        VarName cur = GetVarName(*it);
        if (cur.IsValid()) {
            varNames.insert(cur);
        }
    }
}

void PKBStorage::TransformVarIndextoVarName(const vector<pair<StatementNumber, VarIndex>>& source,
    vector<pair<StatementNumber, VarName>>& res) const {
    for (typename vector<pair<StatementNumber, VarIndex>>::const_iterator it = source.begin(); it != source.end(); ++it) {
        VarName vn = GetVarName(it->second);
        res.push_back(pair(it->first, vn));
    }
}

ProcedureName* PKBStorage::GetProcedureName(ProcedureIndex pi) {
    return procedure_table[pi]->GetNamePointer();
}

bool PKBStorage::IsProcExist(ProcedureIndex pi)
{
    return !(procedure_table.find(pi) == procedure_table.end());
}

void PKBStorage::GetProcedureNameSet(const set<ProcedureIndex>& pindexes, set<ProcedureName>& pnames) {
    for (typename set<ProcedureIndex>::iterator it = pindexes.begin(); it != pindexes.end(); ++it) {
        ProcedureName cur = *(GetProcedureName(*it));
        pnames.insert(cur);
    }
}

void PKBStorage::TransformIndexesToNames(const vector<pair<ProcedureIndex, VarIndex>>& source,
    vector<pair<ProcedureName, VarName>>& res) {
    for (typename vector<pair<ProcedureIndex, VarIndex>>::const_iterator it = source.begin(); it != source.end(); ++it) {
        ProcedureName pn = *(GetProcedureName(it->first));
        VarName vn = GetVarName(it->second);
        res.push_back(pair(pn, vn));
    }
}

void PKBStorage::TransformProIndexesToProNames(const vector<pair<ProcedureIndex, ProcedureIndex>>& source,
    vector<pair<ProcedureName, ProcedureName>>& res) {
    for (typename vector<pair<ProcedureIndex, ProcedureIndex>>::const_iterator it = source.begin(); it != source.end(); ++it) {
        ProcedureName pn1 = *(GetProcedureName(it->first));
        ProcedureName pn2 = *(GetProcedureName(it->second));
        res.push_back(pair(pn1, pn2));
    }
}

void PKBStorage::TransformProIndexesToProNamesWithStmt(const vector<pair<StatementNumber, ProcedureIndex>>& source,
    vector<pair<StatementNumber, ProcedureName>>& res) {
    for (typename vector<pair<StatementNumber, ProcedureIndex>>::const_iterator it = source.begin(); it != source.end(); ++it) {
        ProcedureName pn2 = *(GetProcedureName(it->second));
        res.push_back(pair(it->first, pn2));
    }
}

Procedure* PKBStorage::GetResideProcedure(const StatementNumber& sn)
{
    if (sn.GetLine() > max_stmt_number) {
        return nullptr;
    }
    for (typename unordered_map<ProcedureIndex, Procedure*, hash<size_t>>::iterator it = procedure_table.begin();
        it != procedure_table.end(); ++it) {
        if (it->second->IsWithin(sn.GetLine())) {
            return it->second;
        }
    }
    return nullptr;
}

void PKBStorage::GenerateThenElseStatements()
{
    const set<StatementNumber>* ifs = if_table.GetAllAttribute1();
    for (typename set<StatementNumber>::const_iterator it = ifs->begin(); it != ifs->end(); ++it) {
        const set<StatementNumber>* children = parent_table.GetByAttribute1(*it);
        typename set<StatementNumber>::const_iterator it2 = children->begin();
        StatementNumber then_first = *it2;
        ++it2;
        while (it2 != children->end()) {
            if (!follow_star_table.GetRelation(then_first, *it2)) {
                break;
            }
            ++it2;
        }
        StatementNumber else_first = *it2;

        then_else_table[*it] = new pair<StatementNumber, StatementNumber>(then_first, else_first);
    }
}

void PKBStorage::GenerateAssignStatements()
{
    if (assign_table.empty() || !assign_statements.empty()) {
        return;
    }
    for (typename unordered_map<StatementNumber, string>::iterator it = assign_table.begin();
        it != assign_table.end(); ++it) {
        assign_statements.push_back(it->first);
    }
}

void PKBStorage::GenerateAssignStatementsForProc(Procedure* proc)
{
    if (proc->IsAssignFinished()) {
        return;
    }
    GenerateAssignStatements();
    for (typename vector<StatementNumber>::const_iterator it = assign_statements.begin(); it != assign_statements.end(); ++it) {
        if (proc->IsWithin(it->GetLine())) {
            proc->AddAssign(*it);
        }
    }
}

bool PKBStorage::GetRelationStmtVar(const Table<StatementNumber, VarIndex, hashStatementNumber, hash<VarIndex>>& table, 
    const StatementNumber& sn, const VarName& vn) const
{
    VarIndex vi = var_name_hasher(vn);
    return table.GetRelation(sn, vi);
}

set<StatementNumber> PKBStorage::GetStmtsByVar(const Table<StatementNumber, VarIndex, hashStatementNumber, hash<VarIndex>>& table, 
    const VarName& vn) const
{
    VarIndex vi = var_name_hasher(vn);
    return *table.GetByAttribute2(vi);
}

set<VarName> PKBStorage::GetVarsByStmt(const Table<StatementNumber, VarIndex, hashStatementNumber, hash<VarIndex>>& table, const StatementNumber& sn)
{
    set<VarName> result;
    set<VarIndex> temp = *table.GetByAttribute1(sn);
    GetVarNameSet(temp, result);
    return result;
}

set<VarName> PKBStorage::GetValidVarsByStmt(const Table<StatementNumber, VarIndex, hashStatementNumber, hash<VarIndex>>& table, const StatementNumber& sn)
{
    set<VarName> result;
    GetValidVarNameSet(*table.GetByAttribute1(sn), result);
    return result;
}

set<VarName> PKBStorage::GetAllVars(Table<StatementNumber, VarIndex, hashStatementNumber, hash<VarIndex>>& table)
{
    set<VarName> result;
    set<VarIndex> temp = *table.GetAllAttribute2();
    GetVarNameSet(temp, result);
    return result;
}

vector<pair<StatementNumber, VarName>> PKBStorage::GetAllStmtVarPairs(const Table<StatementNumber, VarIndex, hashStatementNumber, hash<VarIndex>>& table) const
{
    vector<pair<StatementNumber, VarName>> result;
    vector<pair<StatementNumber, VarIndex>>* temp = table.GetAllRelations();
    TransformVarIndextoVarName(*temp, result);
    return result;
}

bool PKBStorage::InsertStmtVarRelation(Table<StatementNumber, VarIndex, hashStatementNumber, hash<VarIndex>>& table, const StatementNumber& sn, const VarName& vn)
{
    VarIndex vi = var_name_hasher(vn);
    return table.InsertRelation(sn, vi);
}

bool PKBStorage::IsVarExist(const Table<StatementNumber, VarIndex, hashStatementNumber, hash<VarIndex>>& table, const VarName& vn) const
{
    VarIndex vi = var_name_hasher(vn);
    return table.IsAttribute2Exist(vi);
}

bool PKBStorage::GetRelationProcVar(const Table<ProcedureIndex, VarIndex, hash<ProcedureIndex>, hash<VarIndex>>& table, const ProcedureName& pn, const VarName& vn) const
{
    ProcedureIndex pi = procedure_name_hasher(pn);
    VarIndex vi = var_name_hasher(vn);
    return table.GetRelation(pi, vi);
}

set<ProcedureName> PKBStorage::GetProcsByVar(const Table<ProcedureIndex, VarIndex, hash<ProcedureIndex>, hash<VarIndex>>& table, const VarName& vn)
{
    VarIndex vi = var_name_hasher(vn);
    set<ProcedureName> res;
    GetProcedureNameSet(*table.GetByAttribute2(vi), res);
    return res;
}

set<VarName> PKBStorage::GetVarsByProc(const Table<ProcedureIndex, VarIndex, hash<ProcedureIndex>, hash<VarIndex>>& table, const ProcedureName& pn)
{
    ProcedureIndex pi = procedure_name_hasher(pn);
    set<VarName> res;
    GetVarNameSet(*table.GetByAttribute1(pi), res);
    return res;
}

set<ProcedureName> PKBStorage::GetAllProcs(Table<ProcedureIndex, VarIndex, hash<ProcedureIndex>, hash<VarIndex>>& table)
{
    set<ProcedureName> res;
    const set<ProcedureIndex>* temp = table.GetAllAttribute1();
    GetProcedureNameSet(*temp, res);
    return res;
}

set<VarName> PKBStorage::GetAllVars(Table<ProcedureIndex, VarIndex, hash<ProcedureIndex>, hash<VarIndex>>& table) {
    set<VarName> result;
    set<VarIndex> temp = *table.GetAllAttribute2();
    GetVarNameSet(temp, result);
    return result;
}

vector<pair<ProcedureName, VarName>> PKBStorage::GetAllProcVarPairs(const Table<ProcedureIndex, VarIndex, hash<ProcedureIndex>, hash<VarIndex>>& table)
{
    vector<pair<ProcedureName, VarName>> res;
    TransformIndexesToNames(*table.GetAllRelations(), res);
    return res;
}

bool PKBStorage::InsertProcVarRelation(Table<ProcedureIndex, VarIndex, hash<ProcedureIndex>, hash<VarIndex>>& table, const ProcedureName& pn, const VarName& vn)
{
    ProcedureIndex pi = procedure_name_hasher(pn);
    VarIndex vi = var_name_hasher(vn);
    return table.InsertRelation(pi, vi);
}

bool PKBStorage::IsProcExist(const Table<ProcedureIndex, VarIndex, hash<ProcedureIndex>, hash<VarIndex>>& table, const ProcedureName& pn) const
{
    ProcedureIndex pi = procedure_name_hasher(pn);
    return table.IsAttribute1Exist(pi);
}

bool PKBStorage::IsVarExist(const Table<ProcedureIndex, VarIndex, hash<ProcedureIndex>, hash<VarIndex>>& table, const VarName& vn) const {
    VarIndex vi = var_name_hasher(vn);
    return table.IsAttribute2Exist(vi);
}

bool PKBStorage::GetRelationProcs(const Table<ProcedureIndex, ProcedureIndex, hash<ProcedureIndex>, hash<ProcedureIndex>>& table, const ProcedureName& pn1, const ProcedureName& pn2)
{
    ProcedureIndex pi1 = procedure_name_hasher(pn1);
    ProcedureIndex pi2 = procedure_name_hasher(pn2);
    return table.GetRelation(pi1, pi2);
}

set<ProcedureName> PKBStorage::GetProc1(const Table<ProcedureIndex, ProcedureIndex, hash<ProcedureIndex>, hash<ProcedureIndex>>& table, const ProcedureName& pn)
{
    ProcedureIndex pi = procedure_name_hasher(pn);
    set<ProcedureName> res;
    GetProcedureNameSet(*table.GetByAttribute1(pi), res);
    return res;
}

set<ProcedureName> PKBStorage::GetProc2(const Table<ProcedureIndex, ProcedureIndex, hash<ProcedureIndex>, hash<ProcedureIndex>>& table, const ProcedureName& pn)
{
    ProcedureIndex pi = procedure_name_hasher(pn);
    set<ProcedureName> res;
    GetProcedureNameSet(*table.GetByAttribute2(pi), res);
    return res;
}

set<ProcedureName> PKBStorage::GetAllProc1s(Table<ProcedureIndex, ProcedureIndex, hash<ProcedureIndex>, hash<ProcedureIndex>>& table)
{
    set<ProcedureName> res;
    GetProcedureNameSet(*table.GetAllAttribute1(), res);
    return res;
}

set<ProcedureName> PKBStorage::GetAllProc2s(Table<ProcedureIndex, ProcedureIndex, hash<ProcedureIndex>, hash<ProcedureIndex>>& table)
{
    set<ProcedureName> res;
    GetProcedureNameSet(*table.GetAllAttribute2(), res);
    return res;
}

vector<pair<ProcedureName, ProcedureName>> PKBStorage::GetAllProcPairs(const Table<ProcedureIndex, ProcedureIndex, hash<ProcedureIndex>, hash<ProcedureIndex>>& table)
{
    vector<pair<ProcedureName, ProcedureName>> res;
    TransformProIndexesToProNames(*table.GetAllRelations(), res);
    return res;
}

bool PKBStorage::InsertProcsRelation(Table<ProcedureIndex, ProcedureIndex, hash<ProcedureIndex>, hash<ProcedureIndex>>& table, const ProcedureName& pn1, const ProcedureName& pn2)
{
    ProcedureIndex pi1 = procedure_name_hasher(pn1);
    ProcedureIndex pi2 = procedure_name_hasher(pn2);
    return table.InsertRelation(pi1, pi2);
}

bool PKBStorage::IsProc1Exist(const Table<ProcedureIndex, ProcedureIndex, hash<ProcedureIndex>, hash<ProcedureIndex>>& table, const ProcedureName& pn)
{
    ProcedureIndex pi = procedure_name_hasher(pn);
    return table.IsAttribute1Exist(pi);
}

bool PKBStorage::IsProc2Exist(const Table<ProcedureIndex, ProcedureIndex, hash<ProcedureIndex>, hash<ProcedureIndex>>& table, const ProcedureName& pn)
{
    ProcedureIndex pi = procedure_name_hasher(pn);
    return table.IsAttribute2Exist(pi);
}
