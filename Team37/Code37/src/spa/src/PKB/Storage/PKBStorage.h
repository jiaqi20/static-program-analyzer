#ifndef INC_21S2_CP_SPA_TEAM_37_PKBSTORAGE_H
#define INC_21S2_CP_SPA_TEAM_37_PKBSTORAGE_H


#include <unordered_map>

#include "Table.h"
#include "../Attribute/ADT.h"
#include "../Attribute/Procedure.h"
#include "../Attribute/VarName.h"
#include "PKB/Attribute/StatementNumber.h"

using namespace std;

class PKBStorage {
private:
    friend class PKB;
    friend class CFG;

    // Stores modifies relation Modifies(stmt#, var)
    Table<StatementNumber, VarIndex, hashStatementNumber,
        hash<VarIndex>> modify_table;

    // Stores modifiesP relation Modifies(procedure_name, var)
    Table<ProcedureIndex, VarIndex, hash<ProcedureIndex>, hash<VarIndex>> modifyP_table;

    // Stores uses relation Uses(stmt#, var)
    Table<StatementNumber, VarIndex, hashStatementNumber,
        hash<VarIndex>> use_table;

    // Stores usesP relation Modifies(procedure_name, var)
    Table<ProcedureIndex, VarIndex, hash<ProcedureIndex>, hash<VarIndex>> useP_table;

    // Stores follows relation Follows(stmt#, stmt)
    Table<StatementNumber, StatementNumber, hashStatementNumber,
        hashStatementNumber> follow_table;

    // Stores parent relation Parent(stmt#, stmt)
    Table<StatementNumber, StatementNumber, hashStatementNumber,
        hashStatementNumber> parent_table;

    // Stores follows* relation Follows*(stmt#, stmt)
    Table<StatementNumber, StatementNumber, hashStatementNumber,
        hashStatementNumber> follow_star_table;

    // Stores parent* relation Parent*(stmt#, stmt)
    Table<StatementNumber, StatementNumber, hashStatementNumber,
        hashStatementNumber> parent_star_table;

    // Stores cosntant in the source program and which stmt it appears
    Table<StatementNumber, Constant, hashStatementNumber, hash<Constant>> constant_table;

    // Stores <stmt# of while, set of control variables of while>
    Table<StatementNumber, VarIndex, hashStatementNumber, hash<VarIndex>> while_table;

    // Stores <stmt# of if, set of control variables of if>
    Table<StatementNumber, VarIndex, hashStatementNumber, hash<VarIndex>> if_table;

    // Stores the the first stmt of then clause and else clause of if stmt
    unordered_map<StatementNumber, pair<StatementNumber, StatementNumber>*, hashStatementNumber> then_else_table;

    // Stores calls relation Calls(procedure_name, procedure_name)
    Table<ProcedureIndex, ProcedureIndex, hash<ProcedureIndex>, hash<ProcedureIndex>> calls_table;

    // Stores calls* relation Calls*(procedure_name, procedure_name)
    Table<ProcedureIndex, ProcedureIndex, hash<ProcedureIndex>, hash<ProcedureIndex>> calls_star_table;

    // Stores next relation Next(stmt#, stmt#)
    Table<StatementNumber, StatementNumber, hashStatementNumber, hashStatementNumber> next_table;

    // Stores all the stmt# of read
    Table<StatementNumber, VarIndex, hashStatementNumber, hash<size_t>> read_table;

    // Stores all the stmt# of print
    Table<StatementNumber, VarIndex, hashStatementNumber, hash<size_t>> print_table;

    // Stores all the stmt# of call
    Table<StatementNumber, ProcedureIndex, hashStatementNumber, hash<size_t>> call_table;

    // Stores all the variables in source code and its hash index
    unordered_map<VarIndex, VarName, hash<size_t>> var_table;

    // Stores all the procedures in source code and its hash index
    unordered_map<ProcedureIndex, Procedure*, hash<size_t>> procedure_table;

    // Stores the rpn of RHS of an assignment
    // key is statement number, value is rpn of the RHS, see DesignExtractor for details of rpn
    unordered_map<StatementNumber, RPN, hashStatementNumber> assign_table;

    vector<StatementNumber> assign_statements;

    int max_stmt_number = 0;

    hashVarName var_name_hasher = hashVarName();
    VarIndex invalidIndex = var_name_hasher(VarName::INVALID_NAME());

    hashProcedureName procedure_name_hasher = hashProcedureName();

    // private function for var_table
    VarName GetVarName(VarIndex vi) const;
    void GetVarNameSet(const set<VarIndex>& varIndexes, set<VarName>& varNames);
    void GetValidVarNameSet(const set<VarIndex>& varIndexes, set<VarName>& varNames);
    void TransformVarIndextoVarName(const vector<pair<StatementNumber, VarIndex>>& source,
        vector<pair<StatementNumber, VarName>>& res) const;

    // private function for procedure_table
    bool IsProcExist(ProcedureIndex pi);
    ProcedureName* GetProcedureName(ProcedureIndex pi);
    void GetProcedureNameSet(const set<ProcedureIndex>& pindexes, set<ProcedureName>& pnames);
    void TransformIndexesToNames(const vector<pair<ProcedureIndex, VarIndex>>& source,
        vector<pair<ProcedureName, VarName>>& res);
    void TransformProIndexesToProNames(const vector<pair<ProcedureIndex, ProcedureIndex>>& source,
        vector<pair<ProcedureName, ProcedureName>>& res);
    void TransformProIndexesToProNamesWithStmt(const vector<pair<StatementNumber, ProcedureIndex>>& source,
        vector<pair<StatementNumber, ProcedureName>>& res);


public:
    PKBStorage();
    ~PKBStorage();
    Procedure* GetResideProcedure(const StatementNumber&);
    void GenerateThenElseStatements();
    void GenerateAssignStatements();
    void GenerateAssignStatementsForProc(Procedure* proc);

    // API for Table<stmt, var>
    bool GetRelationStmtVar(const Table<StatementNumber, VarIndex, hashStatementNumber, hash<VarIndex>>& table,
        const StatementNumber& sn, const VarName& vn) const;

    set<StatementNumber> GetStmtsByVar(const Table<StatementNumber, VarIndex, hashStatementNumber, hash<VarIndex>>& table,
        const VarName& vn) const;

    set<VarName> GetVarsByStmt(const Table<StatementNumber, VarIndex, hashStatementNumber, hash<VarIndex>>& table,
        const StatementNumber& sn);

    set<VarName> GetValidVarsByStmt(const Table<StatementNumber, VarIndex, hashStatementNumber, hash<VarIndex>>& table,
        const StatementNumber& sn);

    set<VarName> GetAllVars(Table<StatementNumber, VarIndex, hashStatementNumber, hash<VarIndex>>& table);

    vector<pair<StatementNumber, VarName>> GetAllStmtVarPairs(const Table<StatementNumber, VarIndex, hashStatementNumber, hash<VarIndex>>& table) const;

    bool InsertStmtVarRelation(Table<StatementNumber, VarIndex, hashStatementNumber, hash<VarIndex>>& table,
        const StatementNumber& sn, const VarName& vn);

    bool IsVarExist(const Table<StatementNumber, VarIndex, hashStatementNumber, hash<VarIndex>>& table, const VarName& vn) const;

    // API for Table<proc, var>
    bool GetRelationProcVar(const Table<ProcedureIndex, VarIndex, hash<ProcedureIndex>, hash<VarIndex>>& table,
        const ProcedureName& pn, const VarName& vn) const;

    set<ProcedureName> GetProcsByVar(const Table<ProcedureIndex, VarIndex, hash<ProcedureIndex>, hash<VarIndex>>& table,
        const VarName& vn);

    set<VarName> GetVarsByProc(const Table<ProcedureIndex, VarIndex, hash<ProcedureIndex>, hash<VarIndex>>& table,
        const ProcedureName& pn);

    set<ProcedureName> GetAllProcs(Table<ProcedureIndex, VarIndex, hash<ProcedureIndex>, hash<VarIndex>>& table);

    set<VarName> GetAllVars(Table<ProcedureIndex, VarIndex, hash<ProcedureIndex>, hash<VarIndex>>& table);

    vector<pair<ProcedureName, VarName>> GetAllProcVarPairs(const Table<ProcedureIndex, VarIndex, hash<ProcedureIndex>, hash<VarIndex>>& table);

    bool InsertProcVarRelation(Table<ProcedureIndex, VarIndex, hash<ProcedureIndex>, hash<VarIndex>>& table,
        const ProcedureName& pn, const VarName& vn);

    bool IsVarExist(const Table<ProcedureIndex, VarIndex, hash<ProcedureIndex>, hash<VarIndex>>& table, const VarName& vn) const;

    bool IsProcExist(const Table<ProcedureIndex, VarIndex, hash<ProcedureIndex>, hash<VarIndex>>& table, const ProcedureName& pn) const;

    // API for Table<proc, proc>
    bool GetRelationProcs(const Table<ProcedureIndex, ProcedureIndex, hash<ProcedureIndex>, hash<ProcedureIndex>>& table,
        const ProcedureName& pn1, const ProcedureName& pn2);

    set<ProcedureName> GetProc1(const Table<ProcedureIndex, ProcedureIndex, hash<ProcedureIndex>, hash<ProcedureIndex>>& table,
        const ProcedureName& pn);

    set<ProcedureName> GetProc2(const Table<ProcedureIndex, ProcedureIndex, hash<ProcedureIndex>, hash<ProcedureIndex>>& table,
        const ProcedureName& pn);

    set<ProcedureName> GetAllProc1s(Table<ProcedureIndex, ProcedureIndex, hash<ProcedureIndex>, hash<ProcedureIndex>>& table);

    set<ProcedureName> GetAllProc2s(Table<ProcedureIndex, ProcedureIndex, hash<ProcedureIndex>, hash<ProcedureIndex>>& table);

    vector<pair<ProcedureName, ProcedureName>> GetAllProcPairs(const Table<ProcedureIndex, ProcedureIndex, hash<ProcedureIndex>, hash<ProcedureIndex>>& table);

    bool InsertProcsRelation(Table<ProcedureIndex, ProcedureIndex, hash<ProcedureIndex>, hash<ProcedureIndex>>& table,
        const ProcedureName& pn1, const ProcedureName& pn2);

    bool IsProc1Exist(const Table<ProcedureIndex, ProcedureIndex, hash<ProcedureIndex>, hash<ProcedureIndex>>& table,
        const ProcedureName& pn);

    bool IsProc2Exist(const Table<ProcedureIndex, ProcedureIndex, hash<ProcedureIndex>, hash<ProcedureIndex>>& table,
        const ProcedureName& pn);
};

#endif // INC_21S2_CP_SPA_TEAM_37_PKBSTORAGE_H