#ifndef INC_21S2_CP_SPA_TEAM_37_PKB_H
#define INC_21S2_CP_SPA_TEAM_37_PKB_H

#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <functional>

#include "PKB/Attribute/StatementNumber.h"
#include "./Attribute/ADT.h"
#include "Attribute/VarName.h"
#include "Attribute/Procedure.h"
#include "Storage/Table.h"
#include "Storage/PKBStorage.h"
#include "CFG.h"

using namespace std;

class PKB {
private:

    // Stores PKB tables
    PKBStorage storage;

    // Stores a pointer to the CFG
    CFG* cfg;

    void GetAllStatementWithControlVars(Table<StatementNumber, VarIndex, hashStatementNumber, hash<size_t>>& table,
        set<StatementNumber>& temp);

    Procedure* WithinSameProc(const StatementNumber& sn1, const StatementNumber& sn2);

    // private function for next_table
    void GenerateNext(const StatementNumber& start);
    void GenerateNext(const StatementNumber& start, vector<StatementNumber>& last_stmts);
    void GenerateNextForWhile(const StatementNumber& ws, vector<StatementNumber>& last_stmts);
    void GenerateNextForIf(const StatementNumber& ifs, vector<StatementNumber>& last_stmts);

public:
    PKB();
    ~PKB();

    // API for Modifies

    // Gets if Modifies(sn, vn) is true
    bool GetModifies(const StatementNumber& sn, const VarName& vn);
    // Gets if Modifies(sn, _) is true, sn is the given stmt
    bool IsModifiesStatementExist(const StatementNumber& sn);
    // Gets if Modifies(_, vn) is true, vn is the given var
    bool IsModifiesVarExist(const VarName& vn);
    // Gets if modifies relation is empty
    bool IsModifiesEmpty();
    // Gets a set of statements that modify variable vn
    set<StatementNumber> GetModifiesByVar(const VarName& vn);
    // Gets a set of variables that are modified by statement sn
    set<VarName> GetModifiesByStatement(const StatementNumber& sn);
    // Gets all statements such that Modifies(stmt, _)
    set<StatementNumber> GetAllModifiesStatements();
    // Gets all variables such that ModifiesS(_, var)
    set<VarName> GetAllModifiesVars();
    // Gets all Modifies(stmt#, var) in the form of a vector of pair<stmt#, var>
    vector<pair<StatementNumber, VarName>> GetAllModifies();
    // Inserts a Modifies relation (sn, vn), returns true if insert successfully, false if the relation already existed
    bool InsertModifies(const StatementNumber& sn, const VarName& vn);
    // Updates Modifies for call statements
    void UpdateModifiesForCallStatements();

    // API for ModifiesP

    // Get if ModifiesP(pn, vn) is true
    bool GetModifiesP(const ProcedureName& pn, const VarName& vn);
    // Gets if ModifiesP(pn, _) is true, sn is the given proc
    bool IsModifiesProcedureExist(const ProcedureName& pn);
    // Gets if ModifiesP(_, vn) is true, vn is the given var
    bool IsModifiesPVarExist(const VarName& vn);
    // Gets if modifies relationis empty
    bool IsModifiesPEmpty();
    // Get a set of procedures that modifies variable vn
    set<ProcedureName> GetModifiesPByVar(const VarName& vn);
    // Gets a set of variables that are modified by procedure pn
    set<VarName> GetModifiesPByProcedure(const ProcedureName& pn);
    // Gets all procedure names such that Modifies(proc, _)
    set<ProcedureName> GetAllModifiesProcedures();
    // Gets all variables such that ModifiesP(_, var)
    set<VarName> GetAllModifiesPVars();
    // Gets all ModifiesP(procedure, var) in the form of a vector of pair<procedure, var>
    vector<pair<ProcedureName, VarName>> GetAllModifiesP();
    // Inserts a Modifies relation (pn, vn), returns true if insert successfully, false if the relation already existed
    bool InsertModifiesP(const ProcedureName& pn, const VarName& vn);
    // Update nested ModifiesP based on Calls
    void UpdateNestedModifiesP();

    // API for Use

    // Gets if Uses(sn, vn) is true
    bool GetUses(const StatementNumber& sn, const VarName& vn);
    // Gets if Uses(sn, _) is true, sn is the given stmt
    bool IsUsesStatementExist(const StatementNumber& sn);
    // Gets if Uses(_, vn) is true, sn is the given stmt
    bool IsUsesVarExist(const VarName& vn);
    // Gets if uses relation is empty
    bool IsUsesEmpty();
    // Gets a set of statements that use variable vn
    set<StatementNumber> GetUsesByVar(const VarName& vn);
    // Gets a set of variables that are used by statement sn
    set<VarName> GetUsesByStatement(const StatementNumber& sn);
    // Gets all Uses(stmt#, var) in the form of a vector of pair<stmt#, var>
    vector<pair<StatementNumber, VarName>> GetAllUses();
    // Gets all statements such that Uses(stmt, _)
    set<StatementNumber> GetAllUsesStatements();
    // Gets all variables such that Uses(_, var)
    set<VarName> GetAllUsesVars();
    // Inserts a uses relation (sn, vn), returns true if insert successfully, false if the relation already existed
    bool InsertUses(const StatementNumber& no, const VarName& vn);
    // Updates Uses for call statements
    void UpdateUsesForCallStatements();

    // API for UsesP

    // Get if UsesP(pn, vn) is true
    bool GetUsesP(const ProcedureName& pn, const VarName& vn);
    // Gets if UsesP(pn, _) is true, sn is the given proc
    bool IsUsesProcedureExist(const ProcedureName& pn);
    // Gets if UsesP(_, vn) is true, vn is the given var
    bool IsUsesPVarExist(const VarName& vn);
    // Gets if usesP relationis empty
    bool IsUsesPEmpty();
    // Get a set of procedures that uses variable vn
    set<ProcedureName> GetUsesPByVar(const VarName& vn);
    // Gets a set of variables that are used by procedure pn
    set<VarName> GetUsesPByProcedure(const ProcedureName& pn);
    // Gets all procedures such that Uses(proc, _)
    set<ProcedureName> GetAllUsesProcedures();
    // Gets all variables such that UsesP(_, var)
    set<VarName> GetAllUsesPVars();
    // Gets all Uses(procedure, var) in the form of a vector of pair<procedure, var>
    vector<pair<ProcedureName, VarName>> GetAllUsesP();
    // Inserts a Uses relation (pn, vn), returns true if insert successfully, false if the relation already existed
    bool InsertUsesP(const ProcedureName& pn, const VarName& vn);
    // Update nested UsesP based on Calls
    void UpdateNestedUsesP();

    // API for Follow

    // Gets if Follows(sn1, sn2) is true
    bool GetFollows(const StatementNumber& sn1, const StatementNumber& sn2);
    // Get if Follows(sn, _) is true. sn is a given stmt
    bool IsFollowsArg1Exist(const StatementNumber& sn);
    // Get if Follows(_, sn) is true, sn is a given stmt
    bool IsFollowsArg2Exist(const StatementNumber& sn);
    // Get if Follows is empty
    bool IsFollowsEmpty();
    // Gets stmt such that Follow(sn, stmt) holds, sn and stmt are StatementNumber, i.e. stmt follows sn directly
    set<StatementNumber> GetStatementsFollows(const StatementNumber& sn);
    // Gets stmt such that Follow(stmt, sn) holds, sn and stmt are StatementNumber, i.e. sn follows stmt directly
    set<StatementNumber> GetStatementsFollowedBy(const StatementNumber& sn);
    // Gets all statements such that Follows(stmt, _)
    set<StatementNumber> GetAllFollowsFirst();
    // Gets all statements such that Follows(_, stmt)
    set<StatementNumber> GetAllFollowsSecond();
    // Gets all Follows(stmt#, stmt#) in the form of a vector of pair<stmt#, stmt#>
    vector<pair<StatementNumber, StatementNumber>> GetAllFollows();
    // Inserts a follows relation (sn1, sn2), returns true if insert successfully, false if the relation already existed
    bool InsertFollows(const StatementNumber& sn1, const StatementNumber& sn2);

    // API for Follow*

    // Initializes the Follows* realtions by computing the transtive closure of Follows
    // Calls this after all insertFollow have been finished
    void InitFollowsStarTable();
    // Gets if Follows*(sn1, sn2) is true
    bool GetFollowsStar(const StatementNumber& sn1, const StatementNumber& sn2);
    // Get if Follows*(sn, _) is true. sn is a given stmt
    bool IsFollowsStarArg1Exist(const StatementNumber& sn);
    // Get if Follows*(_, sn) is true, sn is a given stmt
    bool IsFollowsStarArg2Exist(const StatementNumber& sn);
    // Get if Follows* is empty
    bool IsFollowsStarEmpty();
    // Gets stmt such that Follow*(sn, stmt) holds, sn and stmt are StatementNumber, i.e. stmt follows sn directly or indirectly
    set<StatementNumber> GetStatementsFollowsStar(const StatementNumber& sn);
    // Gets stmt such that Follow*(stmt, sn) holds, sn and stmt are StatementNumber, i.e. stmt follows sn directly or indirectly
    set<StatementNumber> GetStatementsFollowedByStar(const StatementNumber& sn);
    // Gets all statements such that Follows*(stmt, _)
    set<StatementNumber> GetAllFollowsStarFirst();
    // Gets all statements such that Follows*(_, stmt)
    set<StatementNumber> GetAllFollowsStarSecond();
    // Gets all Follows*(stmt#, stmt#) in the form of a vector of pair<stmt#, stmt#>
    vector<pair<StatementNumber, StatementNumber>> GetAllFollowsStar();

    // API for Parent

    // Gets if Parent(sn1, sn2) is true
    bool GetParent(const StatementNumber& sn1, const StatementNumber& sn2);
    // Get if Parent(sn, _) is true. sn is a given stmt
    bool IsParentArg1Exist(const StatementNumber& sn);
    // Get if Parent(_, sn) is true, sn is a given stmt
    bool IsParentArg2Exist(const StatementNumber& sn);
    // Get if Parent is empty
    bool IsParentEmpty();
    // Gets stmt such that Parent(sn, stmt) holds, sn and stmt are StatementNumber, i.e. stmt nested in sn directly
    set<StatementNumber> GetStatementsParent(const StatementNumber& sn);
    // Gets stmt such that Parent(stmt, sn) holds, sn and stmt are StatementNumber, i.e. sn nested in stmt directly
    set<StatementNumber> GetStatementsParentedBy(const StatementNumber& sn);
    // Gets all statements such that Parent(stmt, _)
    set<StatementNumber> GetAllParentFirst();
    // Gets all statements such that Parent(_, stmt)
    set<StatementNumber> GetAllParentSecond();
    // Gets all Parent(stmt#, stmt#) in the form of a vector of pair<stmt#, stmt#>
    vector<pair<StatementNumber, StatementNumber>> GetAllParent();
    // Inserts a parent relation (sn1, sn2), returns true if insert successfully, false if the relation already existed
    bool InsertParent(const StatementNumber& sn1, const StatementNumber& sn2);

    // API for Parent*

    // Initializes the Parent* realtions by computing the transtive closure of Parent
    // Calls this after all insertParent have been finished
    void InitParentStarTable();
    // Gets if Parent*(sn1, sn2) is true
    bool GetParentStar(const StatementNumber& sn1, const StatementNumber& sn2);
    // Get if Parent*(sn, _) is true. sn is a given stmt
    bool IsParentStarArg1Exist(const StatementNumber& sn);
    // Get if Parent*(_, sn) is true, sn is a given stmt
    bool IsParentStarArg2Exist(const StatementNumber& sn);
    // Get if Parent* is empty
    bool IsParentStarEmpty();
    // Gets stmt such that Parent*(sn, stmt) holds, sn and stmt are StatementNumber, i.e. stmt nested in sn directly or indirectly
    set<StatementNumber> GetStatementsParentStar(const StatementNumber& sn);
    // Gets stmt such that Parent*(stmt, sn) holds, sn and stmt are StatementNumber, i.e. sn nested in stmt directly or indirectly
    set<StatementNumber> GetStatementsParentedByStar(const StatementNumber& sn);
    // Gets all statements such that Parent*(stmt, _)
    set<StatementNumber> GetAllParentStarFirst();
    // Gets all statements such that Parent*(_, stmt)
    set<StatementNumber> GetAllParentStarSecond();
    // Gets all Parent*(stmt#, stmt#) in the form of a vector of pair<stmt#, stmt#>
    vector<pair<StatementNumber, StatementNumber>> GetAllParentStar();

    // API for Calls relation

    // Gets if Calls(pn1, pn2) is true
    bool GetCalls(const ProcedureName& pn1, const ProcedureName& pn2);
    // Get if Calls(pn, _) is true
    bool IsCallerExist(const ProcedureName& pn);
    // Get if Calls(_, pn) is true
    bool IsCalleeExist(const ProcedureName& pn);
    // Get if Calls is empty
    bool IsCallsEmpty();
    // Gets procedure_names such that Calls(pn, procedure_name) holds, i.e. pn calls procedure_name directly
    set<ProcedureName> GetProcedureNamesCalls(const ProcedureName& pn);
    // Gets procedure_names such that Calls(procedure_name, pn) holds, i.e. pn is called by procedure_name directly
    set<ProcedureName> GetProcedureNamesCalledBy(const ProcedureName& pn);
    // Gets all procedures such that Calls(proc, _)
    set<ProcedureName> GetAllCallsFirst();
    // Gets all procedures such that Calls(_, proc)
    set<ProcedureName> GetAllCallsSecond();
    // Gets all Calls(procedure_name, procedure_name) in the form of a vector of pair<procedure_name, procedure_name>
    vector<pair<ProcedureName, ProcedureName>> GetAllCalls();
    // Inserts a calls relation (pn1, pn2), returns true if insert successfully, false if the relation already existed
    bool InsertCalls(const ProcedureName& pn1, const ProcedureName& pn2);

    // API for Calls* relation

    // Initializes the Calls* realtions by computing the transtive closure of Calls
    // Calls this after all InsertCalls have been finished
    // Returns true if no cyclic call, otherwise false
    bool InitCallsStarTable();
    // Gets if Calls*(pn1, pn2) is true
    bool GetCallsStar(const ProcedureName& pn1, const ProcedureName& pn2);
    // Get if Calls*(pn, _) is true
    bool IsCallerStarExist(const ProcedureName& pn);
    // Get if Calls*(_, pn) is true
    bool IsCalleeStarExist(const ProcedureName& pn);
    // Get if Calls* is empty
    bool IsCallsStarEmpty();
    // Gets procedure_names such that Calls*(pn, procedure_name) holds, i.e. pn calls procedure_name directly or indirectly
    set<ProcedureName> GetProcedureNamesCallsStar(const ProcedureName& pn);
    // Gets procedure_names such that Calls*(procedure_name, pn) holds, i.e. pn is called by procedure_name directly or indirectly
    set<ProcedureName> GetProcedureNamesCalledStarBy(const ProcedureName& pn);
    // Gets all procedures such that Calls*(proc, _)
    set<ProcedureName> GetAllCallsStarFirst();
    // Gets all procedures such that Calls*(_, proc)
    set<ProcedureName> GetAllCallsStarSecond();
    // Gets all Calls*(procedure_name, procedure_name) in the form of a vector of pair<procedure_name, procedure_name>
    vector<pair<ProcedureName, ProcedureName>> GetAllCallsStar();

    // API for constant

    // Gets if the constant c is in the statement specified by sn
    bool IsConstantInStmt(const StatementNumber& sn, const Constant c);
    // Gets all the stmt# that have constant c
    set<StatementNumber> GetStatementsByConstant(const Constant c);
    // Gets all constants in statement specified by sn
    set<Constant> GetConstantsByStatement(const StatementNumber& sn);
    // Gets all constants in the source code
    set<Constant> GetAllConstants();
    // Inserts a constant c with the statement sn that it is in, 
    // returns true if insert successfully, false if the relation already existed
    bool InsertConstant(const StatementNumber& sn, const Constant c);

    // API for variable table
    // Inserts a variable, return true if insert successfully, false if the variable already existed
    bool InsertVariable(const VarName& vn);
    // Gets a set of all variables in the source code
    set<VarName> GetAllVarNames();

    // API for statement number

    // Stores the maximum statement number of the source code
    void SetMaxStmtNumber(int max);
    // Gets the maximum statement number of the source code
    int GetMaxStmtNumber() const;

    // API for while statement table

    // Inserts a while statement with its control variables, 
    // returns true if insert successfully, false if the while already existed
    // If the whileStatment has control variable, parameter v should be VarName(control variable)
    // else parameter v should be VarName()
    bool InsertWhileStatement(const StatementNumber& sn, const VarName& v);
    // Gets a set of while statements no matter if has control variable or not
    set<StatementNumber> GetAllWhileStatements();
    // Gets a set of while statements that have control variable(s)
    set<StatementNumber> GetAllWhileStatementsWithControlVar();
    // Gets a set of control variables of the while statement specified by sn
    set<VarName> GetWhileControlVarsByStatement(const StatementNumber& sn);
    // Gets a set of while stmt# that uses variable v as their control variable
    // Can not find the while statement without control variable. e.g. while(1==1)
    set<StatementNumber> GetWhileStatementsByVar(const VarName& v);
    // Gets a vector of <while_stmt, control_var>, if the control_var is a constant, the second arg is ""
    vector<pair<StatementNumber, VarName>> GetAllWhile();
    // Gets if the stmt# sn is a while statement. If so, returns true, otherwise returns false
    bool IsWhile(const StatementNumber& sn);
    // Gets if the given var vn is used as a control variable in while
    bool IsWhileControlVar(const VarName& vn);
    // Gets if while statements is empty
    bool IsWhileEmpty();

    // API for if statement

    // Inserts a if statement with its control variables, 
    // returns true if insert successfully, false if the if statement already existed
    // If the ifStatment has control variable, parameter v should be VarName(control variable)
    // else parameter v should be VarName()
    bool InsertIfStatement(const StatementNumber& sn, const VarName& v);
    // Gets a set of if statements no matter if has control variable or not
    set<StatementNumber> GetAllIfStatements();
    // Gets a set of if statements that have control variable(s)
    set<StatementNumber> GetAllIfStatementsWithControlVar();
    // Gets a set of control variables of the if statement specified by sn
    set<VarName> GetIfControlVarsByStatement(const StatementNumber& sn);
    // Gets a set of if stmt# that uses variable v as their control variable
    // Can not find the while statement without control variable. e.g. while(1==1)
    set<StatementNumber> GetIfStatementsByVar(const VarName& v);
    // Gets a vector of <if_stmt, control_var>, if the control_var is a constant, the second arg is ""
    vector<pair<StatementNumber, VarName>> GetAllIf();
    // Gets if the stmt# sn is a if statement. If so, returns true, otherwise returns false
    bool IsIf(const StatementNumber& sn);
    // Gets if the given var vn is used as a control variable in if
    bool IsIfControlVar(const VarName& vn);
    // Gets if if statements is empty
    bool IsIfEmpty();

    // API for read statement

    // Inserts a read statement sn, returns true if insert successfully, false if the read statement already existed
    bool InsertReadStatement(const StatementNumber& sn, const VarName& vn);
    // Gets read statements by variable, i.e., read r, r.varName = vn
    set<StatementNumber> GetReadStatementsByVar(const VarName& vn);
    // Gets variables read by the read statement
    set<VarName> GetReadVarByStatement(const StatementNumber& sn);
    // Gets all read statements in the source code
    set<StatementNumber> GetAllReadStatements();
    // Gets all read variables
    set<VarName> GetAllReadVariables();
    // Gets all read
    vector<pair<StatementNumber, VarName>> GetAllReads();
    // Gets if the given statement is read
    bool IsRead(const StatementNumber& sn);
    // Gets if the given variable is read
    bool IsVarRead(const VarName& vn);
    // Gets if read is empty
    bool IsReadEmpty();

    // API for print statement

    // Inserts a print statement sn, returns true if insert successfully, false if the print statement already existed
    bool InsertPrintStatement(const StatementNumber& sn, const VarName& vn);
    // Gets print statements by variable, i.e., print pn, pn.varName = vn
    set<StatementNumber> GetPrintStatementsByVar(const VarName& vn);
    // Gets variables print by the print statement
    set<VarName> GetPrintedVarByStatement(const StatementNumber& sn);
    // Gets all print statements in the source code
    set<StatementNumber> GetAllPrintStatements();
    // Gets all print variables
    set<VarName> GetAllPrintedVariables();
    // Gets all print
    vector<pair<StatementNumber, VarName>> GetAllPrints();
    // Gets if the given statement is print
    bool IsPrint(const StatementNumber& sn);
    // Gets if the given variable isprint
    bool IsVarPrint(const VarName& vn);
    // Gets if print is empty
    bool IsPrintEmpty();

    // API for call statement

    // Inserts a call statement sn, returns true if insert successfully, false if the call statement exists
    bool InsertCallStatement(const StatementNumber& sn, const ProcedureName& vn);
    // Gets call statements by procedure name, i.e., call cl, cl.procName = vn
    set<StatementNumber> GetCallStatementsByProcedure(const ProcedureName& pn);
    // Gets procedure called by call statement sn
    set<ProcedureName> GetCalledProcedureByStatement(const StatementNumber& sn);
    // Gets all call statements in the source code
    set<StatementNumber> GetAllCallStatements();
    // Gets all procedures called by others
    set<ProcedureName> GetAllCalledProcedures();
    // Get all call
    vector<pair<StatementNumber, ProcedureName>> GetAllCallWithProc();
    // Get if the procedure pn is in a call statement
    bool IsProcCalled(const ProcedureName& pn);
    // Get if a statement stmt is a call statement
    bool IsCall(const StatementNumber& sn);
    // Get if call statements table empty
    bool IsCallEmpty();
    // Get if there is a call statement calls non-existent procedure
    bool IsNonExistentCallStmtExist();

    // API for procedure

    // Inserts a procedure pn, returns true if insert successfully, false if the procedure already existed
    bool InsertProcedureName(const ProcedureName& pn);
    // Set the start line number and end line number of procedure pn
    void SetProcedure(const ProcedureName& pn, int start, int end);
    // Gets all procedures in the source code
    set<ProcedureName> GetAllProcedureNames();
    // Gets the the procedure where the given stmt resides
    Procedure* GetResideProcedure(const StatementNumber& sn);

    // API for assign_table

    // Inserts an assign statement sn and the rpn of RHS the assign statement sn,
    // returns true if insert successfully, false if the assign already existed
    bool InsertAssign(const StatementNumber& sn, const RPN& rpn);
    // Gets the rpn of RHS of the given stmt# sn
    RPN GetAssignByStatement(const StatementNumber& sn);
    // Gets a vector of assign stmt# whose RHS is the same as the given expr
    // This function is expensive, it need to loop through all the expressions
    vector<StatementNumber> GetStatementsBySameExpr(const RPN& expr);
    // Gets a vector of assign stmt# whose RHS contains the given expr
    // This function is expensive, it need to loop through all the expressions
    vector<StatementNumber> GetStatementsBySubExpr(const RPN& expr);
    // Gets a vector of all assign stmt#
    vector<StatementNumber> GetAllAssignStatements();
    // Gets a vector of assign stmt# and the rpns of RHS in the form of pair<stmt#, rpn>
    vector<pair<StatementNumber, RPN>> GetAllAssigns();
    // Gets if the given statement is assignment
    bool IsAssign(const StatementNumber& sn);

    // API for next_table

    // Gets if Next(sn1, sn2) is true
    bool GetNext(const StatementNumber& sn1, const StatementNumber& sn2);
    // Gets if Next(sn, _) is true
    bool IsNextFirstExist(const StatementNumber& sn);
    // Gets if Next(_, sn) is true
    bool IsNextSecondExist(const StatementNumber& sn);
    // Get of Next table is empty
    bool IsNextEmpty();
    // Gets stmt such that Next(sn, stmt) holds, sn and stmt are StatementNumber, i.e. stmt is excuted after sn directly
    set<StatementNumber> GetStatementsNextAfter(const StatementNumber& sn);
    // Gets stmt such that Next(stmt, sn) holds, sn and stmt are StatementNumber, i.e. stmt is excuted before sn directly
    set<StatementNumber> GetStatementsNextBefore(const StatementNumber& sn);
    // Generate Next relation based on Follows, Parent, Follows*, while_table, if_table
    void InitNextTable();
    // Gets all statements sn such that Next(sn, _) holds
    set<StatementNumber> GetAllStatementsNextAfter();
    // Gets all statements sn such that Next(_, sn) holds
    set<StatementNumber> GetAllStatementsNextBefore();
    // Gets all Next(stmt#, stmt#) in the form of a vector of pair<stmt#, stmt#>
    vector<pair<StatementNumber, StatementNumber>> GetAllNext();

    // API for next_star

    // Gets if Next*(sn1, sn2) is true
    bool GetNextStar(const StatementNumber& sn1, const StatementNumber& sn2);
    // Gets if Next*(sn, _) is true
    bool IsNextStarFirstExist(const StatementNumber& sn);
    // Gets if Next*(_, sn) is true
    bool IsNextStarSecondExist(const StatementNumber& sn);
    // Get of Next* table is empty
    bool IsNextStarEmpty();
    // Gets stmt such that Next*(sn, stmt) holds, sn and stmt are StatementNumber, i.e. stmt is excuted after sn directly or indirectly
    set<StatementNumber> GetStatementsNextStarAfter(const StatementNumber& sn);
    // Gets stmt such that Next*(stmt, sn) holds, sn and stmt are StatementNumber, i.e. stmt is excuted before sn directly or indirectly
    set<StatementNumber> GetStatementsNextStarBefore(const StatementNumber& sn);
    // Gets all statements sn such that Next*(sn, _) holds
    set<StatementNumber> GetAllStatementsNextStarAfter();
    // Gets all statements sn such that Next*(_, sn) holds
    set<StatementNumber> GetAllStatementsNextStarBefore();
    // Gets all Next*(stmt#, stmt#) in the form of a vector of pair<stmt#, stmt#>
    vector<pair<StatementNumber, StatementNumber>> GetAllNextStar();

    // API for affects

    // Gets if Affects(sn1, sn2) is true
    bool GetAffects(const StatementNumber& sn1, const StatementNumber& sn2);
    // Gets if Affects(sn, _) is true
    bool IsAffectsFirstExist(const StatementNumber& sn);
    // Gets if Affects(_, sn) is true
    bool IsAffectsSecondExist(const StatementNumber& sn);
    // Gets if Affects is empty
    bool IsAffectsEmpty();
    // Gets stmt such that Affects(sn, stmt) holds, sn is input, sn and stmt are StatementNumber, i.e. stmt is affected by sn directly
    set<StatementNumber> GetStatementsAffectedBy(const StatementNumber& sn);
    // Gets stmt such that Affects(stmt, sn) holds, sn is input, sn and stmt are StatementNumber, i.e. stmt affects sn
    set<StatementNumber> GetStatementsAffects(const StatementNumber& sn);
    // Gets all statements sn such that Affects(sn, _) holds
    set<StatementNumber> GetAllStatementsAffects();
    // Gets all statements sn such that Affects(_, sn) holds
    set<StatementNumber> GetAllStatementsAffected();
    // Gets all Affects(stmt#, stmt#) in the form of a vector of pair<stmt#, stmt#>
    vector<pair<StatementNumber, StatementNumber>> GetAllAffects();

    // API for affects star

    // Gets if Affects*(sn1, sn2) is true
    bool GetAffectsStar(const StatementNumber& sn1, const StatementNumber& sn2);
    // Gets if Affects*(sn, _) is true
    bool IsAffectsStarFirstExist(const StatementNumber& sn);
    // Gets if Affects*(_, sn) is true
    bool IsAffectsStarSecondExist(const StatementNumber& sn);
    // Gets if Affects* is empty
    bool IsAffectsStarEmpty();
    // Gets stmt such that Affects*(sn, stmt) holds, sn is input, sn and stmt are StatementNumber, i.e. stmt is affected by sn directly or indirectly
    set<StatementNumber> GetStatementsAffectedStarBy(const StatementNumber& sn);
    // Gets stmt such that Affects*(stmt, sn) holds, sn is input, sn and stmt are StatementNumber, i.e. stmt affects sn directly or indirectly
    set<StatementNumber> GetStatementsAffectsStar(const StatementNumber& sn);
    // Gets all statements sn such that Affects*(sn, _) holds
    set<StatementNumber> GetAllStatementsAffectsStar();
    // Gets all statements sn such that Affects*(_, sn) holds
    set<StatementNumber> GetAllStatementsAffectedStar();
    // Gets all Affects(stmt#, stmt#) in the form of a vector of pair<stmt#, stmt#>
    vector<pair<StatementNumber, StatementNumber>> GetAllAffectsStar();

    // API for CFG to clear cache
    void ClearCache() {
        this->cfg->ClearCache();
    }

    // API for print test
    void PrintAllRelations(vector<pair<StatementNumber, VarName>>& v) {
        for (vector<pair<StatementNumber, VarName>>::iterator it = v.begin(); it != v.end(); ++it) {
            cout << '(' << (*it).first.GetLine() << ',' << (*it).second.GetName() << ')' << endl;
        }
    }

    void PrintAllRelations(vector<pair<StatementNumber, StatementNumber>>& v) {
        for (vector<pair<StatementNumber, StatementNumber>>::iterator it = v.begin(); it != v.end(); ++it) {
            cout << '(' << (*it).first.GetLine() << ',' << (*it).second.GetLine() << ')' << endl;
        }
    }

    void PrintAllRelations(vector<pair<ProcedureName, VarName>>& v) {
        for (vector<pair<ProcedureName, VarName>>::iterator it = v.begin(); it != v.end(); ++it) {
            cout << '(' << (*it).first << ',' << (*it).second.GetName() << ')' << endl;
        }
    }

    void PrintAllRelations(vector<pair<ProcedureName, ProcedureName>>& v) {
        for (vector<pair<ProcedureName, ProcedureName>>::iterator it = v.begin(); it != v.end(); ++it) {
            cout << '(' << (*it).first << ',' << (*it).second << ')' << endl;
        }
    }

    void PrintAllRelations(vector<pair<StatementNumber, ProcedureName>>& v) {
        for (vector<pair<StatementNumber, ProcedureName>>::iterator it = v.begin(); it != v.end(); ++it) {
            cout << '(' << (*it).first.GetLine() << ',' << (*it).second << ')' << endl;
        }
    }

    void PrintAllProcedures() {
        cout << "PKB Procedures:" << endl;
        for (typename unordered_map<ProcedureIndex, Procedure*>::iterator it = storage.procedure_table.begin();
            it != storage.procedure_table.end(); ++it) {
            cout << it->first << " Procdure: " << it->second->GetName() << " from " << it->second->GetStart() << " to " << it->second->GetEnd() << " with index " << it->second->GetIndex() << endl;
        }
    }
};

#endif // INC_21S2_CP_SPA_TEAM_37_PKB_H