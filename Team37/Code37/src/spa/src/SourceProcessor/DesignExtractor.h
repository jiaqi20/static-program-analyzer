#ifndef INC_21S2_CP_SPA_TEAM_37_DESIGNEXTRACTOR_H
#define INC_21S2_CP_SPA_TEAM_37_DESIGNEXTRACTOR_H

#include <string>
#include <stack>

#include "PKB/PKB.h"
#include "SourceProcessor/ASTNode.h"

using namespace std;

class DesignExtractor
{
private:
	ProgramNode AST;
	PKB *pkb;

	void ExtractVariables();
	void ExtractUsesAssign(int stmt_number, std::string expr, std::string proc_name);
	void ExtractUsesContainer(int stmt_number, std::string condition,  StmtList stmt_list, std::string type, std::string proc_name);
	void ExtractParent(int stmt_number,  StmtList stmt_list);
	void ExtractModifies(int stmt_number,  StmtList stmt_list, std::string proc_name);
	void ExtractFollow( StmtList stmt_list);
	std::stack<VariableNode> ParseExpr(std::string expr);

	int read_idx = 0;
	int print_idx = 1;
	int assign_idx = 2;
	int while_idx = 3;
	int if_idx = 4;
	int call_idx = 5;

public:
	DesignExtractor();
    DesignExtractor(PKB *pkb_input);
	~DesignExtractor();

	// for Source Parser
	bool AddProcedureToAST(int start_stmt_number, int end_stmt_number, std::string name,  StmtList stmt_list);
	AssignNode AddAssignToAST(int stmt_number, std::string var_name, std::string expr, std::string proc_name);
	ReadNode AddReadToAST(int stmt_number, std::string var_name, std::string proc_name);
	WhileNode AddWhileToAST(int stmt_number, std::string conditions,  StmtList stmt_list, std::string proc_name);
	IfNode AddIfToAST(int stmt_number, std::string conditions,  StmtList else_stmt_list,  StmtList then_stmt_list, std::string proc_name);
	PrintNode AddPrintToAST(int stmt_number, std::string var_name, std::string proc_name);
	CallNode AddCallToAST(int stmt_number, std::string call_proc, std::string proc_name);

	bool UpdateCallsStarTable();
	void SetMaxStmtNum(int stmt_number);
	void UpdateFollowsStarTable();
	void UpdateParentStarTable();
	void UpdateNextTable();
	void UpdateNestedModifiesPAndUsesP();
	void UpdateModifiesAndUsesForCall();
	bool CheckNonExistentCall();
	void SetUses(int stmt_number, std::string var_name);
	
	// for PKB
	std::string Rpn(std::string expr);
	bool IsOperator(char c);
	bool IsOperand(char c);
	int Precedence(char op);
	bool EqlOrHigher(char op1, char op2);

	//match substring
	bool KmpSearch(const string& substring, const string& full_string);
	void ComputeLpsArray(const string& substring, int sublen, int* lps);
	int CheckOperatorCount(const string& substring);
	
};

#endif // INC_21S2_CP_SPA_TEAM_37_DESIGNEXTRACTOR_H