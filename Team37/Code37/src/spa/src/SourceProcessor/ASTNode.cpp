
#include <string>
#include <vector>
#include "ASTNode.h"

using namespace std;


ASTNode::ASTNode() {

}

ASTNode::~ASTNode() {

}

bool ASTNode::operator==(const ASTNode& node) const 
{
	return this->stmt_number == node.stmt_number;
}

/**
* Program Node
*/
ProgramNode::ProgramNode(int stmt_number, std::vector<ProcedureNode> procedure_list)
{
	this->stmt_number = stmt_number;
	this->procedure_list = procedure_list;
}

bool ProgramNode::operator==(const ASTNode& node) const 
{
	auto casted_node = dynamic_cast<const ProgramNode*>(&node);
	return this->procedure_list == casted_node->procedure_list;
}

std::string ProgramNode::ToString() 
{
	std::string str = "(ProgramNode ";
	for (auto& proc : this->procedure_list) {
		str += proc.ToString();
		str += " ";
	}
	str += ")";
	return str;
}

/**
* Procedure Node
*/
ProcedureNode::ProcedureNode(int stmt_number, std::string name, StmtList stmt_list)
{
	this->stmt_number = stmt_number;
	this->name = name;
	this->stmt_list = stmt_list;
}

bool ProcedureNode::operator==(const ASTNode& node) const
{
	auto casted_node = dynamic_cast<const ProcedureNode*>(&node);
	return (this->name == casted_node->name) && (this->stmt_list == casted_node->stmt_list);
}

std::string ProcedureNode::ToString() 
{
	std::string str = "(ProcedureNode " + this->name + " ";

	for (auto& stmt : this->stmt_list) {
		//str += stmt.ToString();
	}
	str += ")";
	return str;
}

/**
* Print Node
*/
PrintNode::PrintNode(int stmt_number, VariableNode variable)
{
	this->stmt_number = stmt_number;
	this->variable = variable;
}

bool PrintNode::operator==(const ASTNode& node) const
{
	auto casted_node = dynamic_cast<const PrintNode*>(&node);
	return this->variable == casted_node->variable;
}

std::string PrintNode::ToString() 
{
	return "(PrintNode " + this->variable.ToString() + ")";
}

/**
* Read Node
*/
ReadNode::ReadNode(int stmt_number, VariableNode variable)
{
	this->stmt_number = stmt_number;
	this->variable = variable;
}

bool ReadNode::operator==(const ASTNode& node) const
{
	auto casted_node = dynamic_cast<const ReadNode*>(&node);
	return this->variable == casted_node->variable;
}

std::string ReadNode::ToString() 
{
	return "(ReadNode " + this->variable.ToString() + ")";
}

/**
* Assign Node
*/
AssignNode::AssignNode(int stmt_number, VariableNode variable, std::string expr)
{
	this->stmt_number = stmt_number;
	this->variable = variable;
	this->expr = expr;
}

bool AssignNode::operator==(const ASTNode& node) const
{
	auto casted_node = dynamic_cast<const AssignNode*>(&node);
	return (this->variable == casted_node->variable);
}

std::string AssignNode::ToString() 
{
	std::string str = "(AssignNode " + this->variable.ToString() + " ";
	str += " = ";
	//str += this->expr;
	str += ")";
	return str;
}

/**
* While Node
*/
WhileNode::WhileNode(int stmt_number, std::string conditions, StmtList stmt_list)
{
	this->stmt_number = stmt_number;
	this->conditions = conditions;
	this->stmt_list = stmt_list;
}

bool WhileNode::operator==(const ASTNode& node) const
{
	auto casted_node = dynamic_cast<const WhileNode*>(&node);
	return (this->conditions == casted_node->conditions) && (this->stmt_list == casted_node->stmt_list);
}

std::string WhileNode::ToString() 
{
	std::string str = "(WhileNode " + this->conditions + " ";

	for (auto& stmt : this->stmt_list) {

		//str += stmt.ToString();
	}
	str += ")";
	return str;
}

/**
* If Node
*/
IfNode::IfNode(int stmt_number, std::string conditions, StmtList else_stmt_list, StmtList then_stmt_list)
{
	this->stmt_number = stmt_number;
	this->conditions = conditions;
	this->else_stmt_list = else_stmt_list;
	this->then_stmt_list = then_stmt_list;
}

bool IfNode::operator==(const ASTNode& node) const
{
	auto casted_node = dynamic_cast<const IfNode*>(&node);
	return (this->conditions == casted_node->conditions) && (this->else_stmt_list == casted_node->else_stmt_list)
		&& (this->then_stmt_list == casted_node->then_stmt_list);
}

std::string IfNode::ToString() 
{
	std::string str = "(IfNode " + this->conditions + " " + "\n then: \n";

	for (auto& stmt : this->then_stmt_list) {
		//str += stmt.ToString();
	}
	str += "\n else:\n";
	for (auto& stmt : this->else_stmt_list) {

		//str += stmt.ToString();
	}
	str += ")";
	return str;
}

/**
* Call Node
*/
CallNode::CallNode(int stmt_number, std::string procedure_name)
{
	this->stmt_number = stmt_number;
	this->procedure_name = procedure_name;
}

bool CallNode::operator==(const ASTNode& node) const
{
	auto casted_node = dynamic_cast<const CallNode*> (&node);
	return (this->procedure_name == casted_node->procedure_name);
}

std::string CallNode::ToString()
{
	return "(CallNode " + this->procedure_name + ")";
}

/**
* Variable Node
*/
VariableNode::VariableNode(int stmt_number, std::string name)
{
	this->stmt_number = stmt_number;
	this->name = name;
}

bool VariableNode::operator==(const ASTNode& node) const
{
	auto casted_node = dynamic_cast<const VariableNode*>(&node);
	return this->name == casted_node->name;
}

std::string VariableNode::ToString()
{
	return "(VariableNode " + this->name + ")";
}

/**
* Constant Node
*/
ConstantNode::ConstantNode(int stmt_number, int value)
{
	this->stmt_number = stmt_number;
	this->value = value;
}

bool ConstantNode::operator==(const ASTNode& node) const
{
	auto casted_node = dynamic_cast<const ConstantNode*>(&node);
	return this->value == casted_node->value;
}

std::string ConstantNode::ToString()
{
	return "(ConstantNode " + std::to_string(this->value) + ")";
}

/**
* Operator Node
*/
OperatorNode::OperatorNode(int stmt_number, std::string symbol)
{
	this->stmt_number = stmt_number;
	this->symbol = symbol;
}

bool OperatorNode::operator==(const ASTNode& node) const 
{
	auto casted_node = dynamic_cast<const OperatorNode*>(&node);
	return this->symbol == casted_node->symbol;
}

std::string OperatorNode::ToString()
{
	return "(OperatorNode " + this->symbol + ")";
}
