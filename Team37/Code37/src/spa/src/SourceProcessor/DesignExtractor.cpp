
#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <cstring>

#include "DesignExtractor.h"
#include "PKB/PKB.h"
#include "PKB/Attribute/VarName.h"

using namespace std;

DesignExtractor::DesignExtractor() {
}

DesignExtractor::~DesignExtractor() {

}

DesignExtractor::DesignExtractor(PKB* pkb_input) {
    this->pkb = pkb_input;
}

bool DesignExtractor::AddProcedureToAST(int start_stmt_number, int end_stmt_number, std::string name, StmtList stmt_list)
{
    if (!pkb->InsertProcedureName(ProcedureName(name))) {
        return false;
    }

    ExtractFollow(stmt_list);

    pkb->SetProcedure(ProcedureName(name), start_stmt_number, end_stmt_number);

    ProcedureNode pn = ProcedureNode(start_stmt_number, name, stmt_list);

    AST.add(pn);
    return true;
}

// Source parser call this after end of parsing to update calls* relation 
// return true if no cyclic calls else false
bool DesignExtractor::UpdateCallsStarTable() {
    bool isValidCalls = pkb->InitCallsStarTable();
    return isValidCalls;
}

// Source parser call this after end of parsing to check for non existent call
// return true if there is non existent calls else false
bool DesignExtractor::CheckNonExistentCall() {
    bool isNonExistentCall = pkb->IsNonExistentCallStmtExist();
    return isNonExistentCall;
}

void DesignExtractor::SetMaxStmtNum(int stmt_number)
{
    pkb->SetMaxStmtNumber(stmt_number);

}

// Source parser call this before update next table
void DesignExtractor::UpdateFollowsStarTable() {
    pkb->InitFollowsStarTable();
}

// Source parser call this before update next table
void DesignExtractor::UpdateParentStarTable() {
    pkb->InitParentStarTable();
}

// Source parser call this after end of parsing to update modifiesP and usesP such that
// parent procedure will have the modifiesP and usesP relations of its children procedure (call stmts)
// Handles procedure p; Modifies(p, v)
void DesignExtractor::UpdateNestedModifiesPAndUsesP()
{
    pkb->UpdateNestedModifiesP();
    pkb->UpdateNestedUsesP();
}

// Source parser call this after end of parsing to update modifies and uses for call statements
// Handles call c; Modifies(c, v)
void DesignExtractor::UpdateModifiesAndUsesForCall()
{
    pkb->UpdateUsesForCallStatements();
    pkb->UpdateModifiesForCallStatements();
}

// Called after InitfollowStarTable
void DesignExtractor::UpdateNextTable()
{
    pkb->InitNextTable();
}

void DesignExtractor::SetUses(int stmt_number, std::string var_name)
{
    pkb->InsertUses(StatementNumber(stmt_number), VarName(var_name));
}

AssignNode DesignExtractor::AddAssignToAST(int stmt_number, std::string var_name, std::string expr, std::string proc_name)
{
    StatementNumber stmt = StatementNumber(stmt_number);
    VarName vn = VarName(var_name);
    ProcedureName pn = ProcedureName(proc_name);

    pkb->InsertModifies(stmt, vn);

    pkb->InsertModifiesP(pn, vn);

    pkb->InsertVariable(vn);

    ExtractUsesAssign(stmt_number, expr, proc_name);
 
    pkb->InsertAssign(stmt, expr);

    return AssignNode(stmt_number, VariableNode(stmt_number, var_name), expr);
}

ReadNode DesignExtractor::AddReadToAST(int stmt_number, std::string var_name, std::string proc_name)
{
    StatementNumber stmt = StatementNumber(stmt_number);
    VarName vn = VarName(var_name);
    ProcedureName pn = ProcedureName(proc_name);

    pkb->InsertModifies(stmt, vn);

    pkb->InsertModifiesP(pn, vn);

    pkb->InsertVariable(vn);

    pkb->InsertReadStatement(stmt, vn);

    return ReadNode(stmt_number, VariableNode(stmt_number, var_name));
}

WhileNode DesignExtractor::AddWhileToAST(int stmt_number, std::string conditions, StmtList stmt_list, std::string proc_name)
{
    ExtractParent(stmt_number, stmt_list);

    ExtractModifies(stmt_number, stmt_list, proc_name);

    ExtractFollow(stmt_list);

    ExtractUsesContainer(stmt_number, conditions, stmt_list, "while", proc_name);

    return WhileNode(stmt_number, conditions, stmt_list);
}

IfNode DesignExtractor::AddIfToAST(int stmt_number, std::string conditions, StmtList else_stmt_list, StmtList then_stmt_list, std::string proc_name)
{
    ExtractParent(stmt_number, then_stmt_list);
    ExtractParent(stmt_number, else_stmt_list);

    ExtractModifies(stmt_number, then_stmt_list, proc_name);
    ExtractModifies(stmt_number, else_stmt_list, proc_name);

    ExtractFollow(then_stmt_list);
    ExtractFollow(else_stmt_list);

    ExtractUsesContainer(stmt_number, conditions, then_stmt_list, "if", proc_name);
    ExtractUsesContainer(stmt_number, conditions, else_stmt_list, "if", proc_name);

    return IfNode(stmt_number, conditions, else_stmt_list, then_stmt_list);
}

CallNode DesignExtractor::AddCallToAST(int stmt_number, std::string call_proc, std::string proc_name)
{
    StatementNumber stmt = StatementNumber(stmt_number);

    pkb->InsertCallStatement(stmt, call_proc);

    pkb->InsertCalls(proc_name, call_proc);

    return CallNode(stmt_number, proc_name);
}

PrintNode DesignExtractor::AddPrintToAST(int stmt_number, std::string var_name, std::string proc_name)
{
    StatementNumber stmt = StatementNumber(stmt_number);
    VarName vn = VarName(var_name);
    ProcedureName pn = ProcedureName(proc_name);

    pkb->InsertUses(stmt, vn);

    pkb->InsertUsesP(pn, vn);

    pkb->InsertVariable(vn);

    pkb->InsertPrintStatement(stmt, vn);

    return PrintNode(stmt_number, VariableNode(stmt_number, var_name));
}

void DesignExtractor::ExtractVariables()
{
    return;
}

void DesignExtractor::ExtractUsesAssign(int stmt_number, std::string expr, std::string proc_name) {
    for (int i = 0; i < expr.length(); i = i + 1) {
        std::string all_c = "";
        char c = expr[i];
        if (c == '|') {
            continue;
        }
        if (c != ' ' && (c >= '0' && c <= '9') && ((all_c[0] >= '0' && all_c[0] <= '9') || all_c == "")) {
            while (i < expr.length() && c != '|') {
                all_c = all_c + c;
                i = i + 1;
                c = expr[i];
            }
            pkb->InsertConstant(StatementNumber(stmt_number), Constant(all_c));
            continue;
        }
        if (IsOperator(c)) {
            continue;
        }
        
        if (c != ' ') {

            while (((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9')) && c != '|') {
                all_c = all_c + c;
                i = i + 1;
                c = expr[i];
            }
            pkb->InsertUses(StatementNumber(stmt_number), VarName(all_c));

            pkb->InsertUsesP(ProcedureName(proc_name), VarName(all_c));

            pkb->InsertVariable(VarName(all_c));
        }
    }
}

void DesignExtractor::ExtractUsesContainer(int stmt_number, std::string condition, StmtList stmt_list, std::string type, std::string proc_name)
{
    for (int i = 0; i < condition.length(); i = i + 1) {
        string allC = "";
        char c = condition[i];
        if (c == '|') {
            continue;
        }
        if (c != ' ' && c >= '0' && c <= '9' && ((allC[0] >= '0' && allC[0] <= '9') || allC == "")) {
            while (i < condition.length() && c != '|') {
                allC = allC + c;
                i = i + 1;
                c = condition[i];
            }
            pkb->InsertConstant(StatementNumber(stmt_number), Constant(allC));

            if (type == "while") {

                pkb->InsertWhileStatement(StatementNumber(stmt_number), VarName());
            }
            else if (type == "if") {
                pkb->InsertIfStatement(StatementNumber(stmt_number), VarName());
            }
        }
        else if (c != ' ' && (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9')) {
            while (((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9')) && c != '|') {
                allC = allC + c;
                i = i + 1;
                c = condition[i];
            }
            pkb->InsertUses(StatementNumber(stmt_number), VarName(allC));

            pkb->InsertUsesP(ProcedureName(proc_name), VarName(allC));

            if (type == "while") {
                pkb->InsertWhileStatement(StatementNumber(stmt_number), VarName(allC));

            }
            else if (type == "if") {
                pkb->InsertIfStatement(StatementNumber(stmt_number), VarName(allC));
            }
        }
        else {
            continue;
        }
    }

    for (const auto& stmt : stmt_list) {
        //print
        if (stmt.index() == print_idx) {
            int stmt_num = std::visit([](const auto& s) { return s.stmt_number; }, stmt);
            std::string var_name = std::visit([](const auto& s) { return s.variable.name; }, stmt);
            pkb->InsertUses(StatementNumber(stmt_number), VarName(var_name));
            pkb->InsertUses(StatementNumber(stmt_num), VarName(var_name));
            pkb->InsertUsesP(ProcedureName(proc_name), VarName(var_name));
        }
        //assign
        if (stmt.index() == assign_idx) {
            int stmt_num = std::visit([](const auto& s) { return s.stmt_number; }, stmt);
            std::string expr = std::visit([](const auto& s) { return s.expr; }, stmt);
            ExtractUsesAssign(stmt_number, expr, ProcedureName(proc_name));
            ExtractUsesAssign(stmt_num, expr, ProcedureName(proc_name));
        }
        //while
        if (stmt.index() == while_idx) {
            int stmt_num = std::visit([](const auto& s) { return s.stmt_number; }, stmt);
            StmtList stmt_list = std::visit([](const auto& s) { return s.stmt_list; }, stmt);
            std::string conditions = std::visit([](const auto& s) { return s.conditions; }, stmt);
            ExtractUsesContainer(stmt_number, conditions, stmt_list, "none", ProcedureName(proc_name));
            ExtractUsesContainer(stmt_num, conditions, stmt_list, "while", ProcedureName(proc_name));
        }
        //if
        if (stmt.index() == if_idx) {
            int stmt_num = std::visit([](const auto& s) { return s.stmt_number; }, stmt);
            std::string conditions = std::visit([](const auto& s) { return s.conditions; }, stmt);
            StmtList then_stmt_list = std::visit([](const auto& s) { return s.then_stmt_list; }, stmt);
            StmtList else_stmt_list = std::visit([](const auto& s) { return s.else_stmt_list; }, stmt);

            ExtractUsesContainer(stmt_num, conditions, then_stmt_list, "if", ProcedureName(proc_name));
            ExtractUsesContainer(stmt_number, conditions, then_stmt_list, "none", ProcedureName(proc_name));
            ExtractUsesContainer(stmt_num, conditions, else_stmt_list, "if", ProcedureName(proc_name));
            ExtractUsesContainer(stmt_number, conditions, else_stmt_list, "none", ProcedureName(proc_name));
        }
    }
}

void DesignExtractor::ExtractParent(int stmt_number, StmtList stmt_list)
{
    for (const auto& stmt : stmt_list) {
        int stmt_num;
        stmt_num = std::visit([](const auto& s) { return s.stmt_number; }, stmt);

        pkb->InsertParent(stmt_number, StatementNumber(stmt_num));
    }
}

void DesignExtractor::ExtractModifies(int stmt_number, StmtList stmt_list, std::string proc_name)
{
    for (const auto& stmt : stmt_list) {

        // read or assign
        if (stmt.index() == read_idx || stmt.index() == assign_idx) {
            int stmt_num = std::visit([](const auto& s) { return s.stmt_number; }, stmt);
            std::string var_name = std::visit([](const auto& s) { return s.variable.name; }, stmt);
            pkb->InsertModifies(StatementNumber(stmt_number), VarName(var_name));
            pkb->InsertModifies(StatementNumber(stmt_num), VarName(var_name));
            pkb->InsertModifiesP(ProcedureName(proc_name), VarName(var_name));
        }
        //while
        if (stmt.index() == while_idx) {
            int stmt_num = std::visit([](const auto& s) { return s.stmt_number; }, stmt);
            StmtList stmt_list = std::visit([](const auto& s) { return s.stmt_list; }, stmt);
            ExtractModifies(stmt_number, stmt_list, ProcedureName(proc_name));
            ExtractModifies(stmt_num, stmt_list, ProcedureName(proc_name));
        }
        //if
        if (stmt.index() == if_idx) {
            int stmt_num = std::visit([](const auto& s) { return s.stmt_number; }, stmt);
            StmtList then_stmt_list = std::visit([](const auto& s) { return s.then_stmt_list; }, stmt);
            StmtList else_stmt_list = std::visit([](const auto& s) { return s.else_stmt_list; }, stmt);
            ExtractModifies(stmt_number, then_stmt_list, ProcedureName(proc_name));
            ExtractModifies(stmt_num, then_stmt_list, ProcedureName(proc_name));
            ExtractModifies(stmt_number, else_stmt_list, ProcedureName(proc_name));
            ExtractModifies(stmt_num, else_stmt_list, ProcedureName(proc_name));
        }
    }
    return;
}

void DesignExtractor::ExtractFollow(StmtList stmt_list) {

    int count = 0;
    int firstStmtNum = 0;
    int secondStmtNum = 0;
    for (const auto& stmt : stmt_list) {
        if (count == 0) {
            firstStmtNum = std::visit([](const auto& s) { return s.stmt_number; }, stmt);
            count++;
            continue;
        }

        count++;

        secondStmtNum = std::visit([](const auto& s) { return s.stmt_number; }, stmt);

        pkb->InsertFollows(StatementNumber(firstStmtNum), StatementNumber(secondStmtNum));
        firstStmtNum = secondStmtNum;
    }
    return;
}

/**
 * Parse Expr for assignment.
 * Using Pratt Algorithm to parse.
 * Shunting Yard Algorithm to evaluate.
 */
std::stack<VariableNode> DesignExtractor::ParseExpr(std::string expr)
{
    std::stack<VariableNode> expr_stack;

    return expr_stack;
}

// convert string into reverse polish notation 
std::string DesignExtractor::Rpn(std::string expr) {
    std::stack <char> S;
    std::string postfix = "";
    char ch;
    if (expr.find('_') != string::npos) {
        return "_";
    }
    size_t open_bracket = std::count(expr.begin(), expr.end(), '(');
    size_t close_bracket = std::count(expr.begin(), expr.end(), ')');

    if (open_bracket != close_bracket) {
        return "_";
    }
    S.push('(');
    expr += ')';

    for (int i = 0; i < expr.length(); i++)
    {
        ch = expr[i];
        if (ch == ' ')
            continue;
        else if (ch == '(')
            S.push(ch);
        else if (IsOperand(ch))
            postfix += ch;
        else if (IsOperator(ch))
        {
            if (postfix != "" && !IsOperator(postfix[postfix.length() - 1])) {
                postfix = postfix + '|';
            }
            while (!S.empty() && EqlOrHigher(S.top(), ch))
            {
                postfix += S.top();
                S.pop();
            }
            S.push(ch);
        }
        else if (ch == ')')
        {
            postfix = postfix + '|';
            while (!S.empty() && S.top() != '(')
            {
                postfix += S.top();
                S.pop();
            }
            if (!S.empty()) {
                S.pop();
            }
        } else {
            return "_";
        }
    }
    
    for (int i = 0; i < postfix.length(); i++)
    {
        if (postfix[i] == '|' && i < postfix.length() - 1) {
            if (i == 0) {
                continue;
            }
            if ((IsOperator(postfix[i - 1]) && IsOperator(postfix[i + 1])) || (postfix[i + 1] == '|') || (postfix[i - 1] == '|')) {
                postfix.erase(i, 1);
            }
        }
        else if (i != 0) {
            if (postfix[i] == '|' && postfix[i - 1] == '|') {
                postfix.erase(i, 1);
            }
        }
    }
    for (int i = postfix.length() - 1; i > -1; i--) {
        if (postfix[i] == '|') {
        }
        else if (IsOperator(postfix[i])) {
            postfix = postfix.substr(0, i + 1);
            break;
        }
    }
    return postfix;
}

bool DesignExtractor::IsOperator(char c)
{
    if (c == '+' || c == '-' || c == '*' || c == '/' || c == '%')
        return true;
    return false;
}

bool DesignExtractor::IsOperand(char c)
{
    if (c >= 'A' && c <= 'Z')
        return true;
    if (c >= 'a' && c <= 'z')
        return true;
    if (c >= '0' && c <= '9')
        return true;
    return false;
}

int DesignExtractor::Precedence(char op)
{
    if (op == '+' || op == '-')
        return 1;
    if (op == '*' || op == '/' || op == '%')
        return 2;
    return 0;
}

bool DesignExtractor::EqlOrHigher(char op1, char op2)
{
    int p1 = Precedence(op1);
    int p2 = Precedence(op2);
    if (p1 == p2)
    {
        return true;
    }
    return  (p1 > p2 ? true : false);
}

// preprocessing substring array
// lps[i] means the longest proper prefix of pat[0..i] 
// which is also a suffix of pat[0..i]
void DesignExtractor::ComputeLpsArray(const string& substring, int sublen, int* lps)
{
    int len = 0;

    lps[0] = 0;

    int i = 1;
    while (i < sublen) {
        if (substring[i] == substring[len]) {
            len++;
            lps[i] = len;
            i++;
        }
        else
        {
            if (len != 0) {
                len = lps[len - 1];
            }
            else
            {
                lps[i] = 0;
                i++;
            }
        }
    }
}

// implementation of KMP pattern searching algorithm
bool DesignExtractor::KmpSearch(const string& substring, const string& full_string)
{
    int sublen = substring.length();
    int fulllen = full_string.length();

    if (sublen == 1 || sublen > fulllen) {
        return false;
    }

    int count = 0;
    for (int i = 0; i < sublen; i++) {
        if (substring[i] == '|' || IsOperator(substring[i])) {
            count++;
        }
    }

    if (count == 2) {
        return false;
    }

    int* lps = new int[sublen];

    ComputeLpsArray(substring, sublen, lps);

    int i = 0;
    int j = 0;
    while (i < fulllen) {
        if (substring[j] == full_string[i]) {
            j++;
            i++;
        }
        if (j == sublen - 1 && substring[j] == '|' && IsOperator(substring[j-1])) {
            return true;
        }
        if (j == sublen) {
            if (i - j == 0) {
                return true;
            }

            if (IsOperand(full_string[i - j - 1])) {
                return false;
            }
            return true;
        }
        else if (i < fulllen && substring[j] != full_string[i]) {
            if (j != 0) {
                j = lps[j - 1];
            }
            else {
                i = i + 1;
            }
        }
    }
    delete[] lps;
    return false;
}

int DesignExtractor::CheckOperatorCount(const string& substring) {
    int count = 0;
    for (int i = 0; i < substring.length(); i++) {
        if (IsOperator(substring[i])) {
            count++;
        }
    }

    return count;
}