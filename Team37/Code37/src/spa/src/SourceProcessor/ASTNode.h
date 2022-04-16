#ifndef INC_21S2_CP_SPA_TEAM_37_ASTNODE_H
#define INC_21S2_CP_SPA_TEAM_37_ASTNODE_H

#include <string>
#include <vector>
#include <variant>

using namespace std;

class ASTNode;

class ProgramNode;

class ProcedureNode;

class PrintNode;
class ReadNode;
class AssignNode;
class WhileNode;
class IfNode;
class CallNode; // future iteration

class VariableNode;
class ConstantNode;
class OperatorNode;

using StmtNode = std::variant<ReadNode, PrintNode, AssignNode, WhileNode, IfNode, CallNode>;
using StmtList = std::vector<StmtNode>;

using AST = ProgramNode();

/**
 * ASTNode.
 */
class ASTNode
{
public:
	ASTNode();
	~ASTNode();

    int stmt_number;

	void SetstmtNumber(int stmt_number) {
		this->stmt_number = stmt_number;
	}

	int GetStmtNumber() {
		return this->stmt_number;
	}

	virtual bool operator==(const ASTNode& ast_node) const = 0;
	virtual std::string ToString() = 0;

	StmtList stmt_list;
	StmtList then_stmt_list;
	StmtList else_stmt_list;
	std::string expr;
	std::string conditions;
};

/**
 * ASTNode for a SIMPLE program.
 */ 
class ProgramNode : public ASTNode 
{
public:
	std::vector<ProcedureNode> procedure_list;
	ProgramNode() = default;
	ProgramNode(int stmt_number, std::vector<ProcedureNode> procedure_list);

	void add(ProcedureNode& proc_node)
	{
		procedure_list.push_back(proc_node);
	}

	bool operator==(const ASTNode& ast_node) const override;
	std::string ToString() override;
};

/**
 * ASTNode for variables in a SIMPLE program.
 */
class VariableNode : public ASTNode
{
public:
	std::string name;
	VariableNode() = default;
	VariableNode(int stmt_number, std::string name);
	bool operator==(const ASTNode& ast_node) const override;
	std::string ToString() override;
};

/**
 * ASTNode for procedures in a SIMPLE program.
 */
class ProcedureNode : public ASTNode
{
public:
	std::string name;
	StmtList stmt_list;
	ProcedureNode() = default;
	ProcedureNode(int stmt_number, std::string name, StmtList stmt_list);
	void add(StmtNode& stmt_node)
	{
		stmt_list.push_back(stmt_node);
	}
	bool operator==(const ASTNode& ast_node) const override;
	std::string ToString() override;
};

/**
 * ASTNode for print statement.
 */
class PrintNode : public ASTNode
{
public:
	VariableNode variable;
	PrintNode() = default;
	PrintNode(int stmt_number, VariableNode variable);
	bool operator==(const ASTNode& ast_ode) const override;
	std::string ToString() override;
};

/**
 * ASTNode for read statement.
 */
class ReadNode : public ASTNode
{
public:
	VariableNode variable;
	ReadNode() = default;
	ReadNode(int stmt_number, VariableNode variable);
	bool operator==(const ASTNode& ast_ode) const override;
	std::string ToString() override;
};

/**
 * ASTNode for assign statement.
 */
class AssignNode : public ASTNode
{
public:
	VariableNode variable;

	std::string expr;

	AssignNode() = default;
	AssignNode(int stmt_number, VariableNode variable, std::string expr);
	bool operator==(const ASTNode& ast_node) const override;
	std::string ToString() override;
};

/**
 * ASTNode for while statement.
 */
class WhileNode : public ASTNode
{
public:
	VariableNode variable;
	std::string conditions;
	StmtList stmt_list;
	WhileNode() = default;
	WhileNode(int stmt_number, std::string conditions, StmtList stmt_list);
	bool operator==(const ASTNode& ast_node) const override;
	std::string ToString() override;
};

/**
 * ASTNode for if statement.
 */
class IfNode : public ASTNode
{
public:
	VariableNode variable;
	std::string conditions;
	StmtList else_stmt_list;
	StmtList then_stmt_list;
	IfNode() = default;
	IfNode(int stmt_number, std::string conditions, StmtList else_stmt_list, StmtList then_stmt_list);
	bool operator==(const ASTNode& ast_node) const override;
	std::string ToString() override;
};

/**
 * ASTNode for call statement.
 */
class CallNode : public ASTNode
{
public:
	VariableNode variable;
	std::string procedure_name;
	CallNode() = default;
	CallNode(int stmt_number, std::string procedure_name);
	bool operator==(const ASTNode& ast_node) const override;
	std::string ToString() override;
};

/**
 * ASTNode for constants in a SIMPLE program.
 */
class ConstantNode : public ASTNode
{
public:
	int value;
	ConstantNode() = default;
	ConstantNode(int stmt_number, int value);
	bool operator==(const ASTNode& ast_node) const override;
	std::string ToString() override;
};

/**
 * ASTNode for operators in a SIMPLE program.
 */
class OperatorNode : public ASTNode
{
public:
	std::string symbol;
	OperatorNode() = default;
	OperatorNode(int stmt_number, std::string symbol);
	bool operator==(const ASTNode& ast_node) const override;
	std::string ToString() override;
};

#endif // INC_21S2_CP_SPA_TEAM_37_ASTNODE_H
