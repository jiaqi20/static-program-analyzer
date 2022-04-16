#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>

#include "PKB/PKB.h"
#include "SourceParser.h"
#include "ExprParser.h"

using namespace std;


// find token, pop it out of list if correct token type 
// else throw error
lexer::Token SourceParser::FindToken(lexer::Token token)
{
	if (tokens.empty()) {
		throw std::logic_error("Source syntax error: trying to find token with value " + token.value + ", but tokens is empty");
	}
	
	// checks for source_tokens::PROCNAME & source_tokens::VARNAME
	bool nameTypeCheck = (token.value.empty() && token.type == lexer::TokenType::kName && tokens.front().type == lexer::TokenType::kName);
	// checks for source_tokens other than PROCNAME and VARNAME
	bool tokenCheck = (tokens.front() == token);
	
	if (tokenCheck || nameTypeCheck) {
		lexer::Token temp = tokens.front();
		tokens.pop_front();
		return temp;
	}
	throw std::logic_error("Source syntax error: Expected token value = " + token.value + " but got " + tokens.front().value);
	return token;
}

void SourceParser::ParseProgram()
{
	if (tokens.empty()) {
		throw std::logic_error("Source syntax error: tokens empty while parsing program");
	}
	while (!tokens.empty()) {
		ParseProcedure();
	}
}

// procedure: 'procedure' proc_name '{' stmtLst '}'
void SourceParser::ParseProcedure()
{
	int firstStmtNum = stmt_counter + 1;

	FindToken(source_tokens::kProcedure);
	current_proc_name = FindToken(source_tokens::kProcName).value;
	FindToken(source_tokens::kLeftBrace);
	std::vector<StmtNode> nodes = ParseStmtLst();
	FindToken(source_tokens::kRightBrace);
	
	int lastStmtNum = stmt_counter;

	if (!dex.AddProcedureToAST(firstStmtNum, lastStmtNum, current_proc_name, nodes)) {
		throw std::logic_error("[SourceParser::Parse] duplicate procedure detected");
	}
}

// stmtLst: stmt+
// Parse stmtLst into a vector of StmtNode
std::vector<StmtNode> SourceParser::ParseStmtLst()
{
	std::vector<StmtNode> nodes;
	
	// loop until right brace
	while (tokens.front() != source_tokens::kRightBrace)
	{
		// last element still no semicolon
		if (tokens.size() == 1)
		{
			throw std::logic_error("Source syntax error in parsing stmt lst");
		}
		StmtNode node = ParseStmt();
		nodes.emplace_back(node);
		
		// after parsing stmt and if tokens empty
		if (tokens.empty())
		{
			throw std::logic_error("Source syntax error: tokens are empty but expect a right brace");
		}
		
	}

	return nodes;
	
}


// read: 'read' var_name ';'
ReadNode SourceParser::ParseRead(int stmtNum)
{

	FindToken(source_tokens::kRead);
	std::string readVarName = FindToken(source_tokens::kVarName).value;
	FindToken(source_tokens::kSemiColon);

    ReadNode readNode = dex.AddReadToAST(stmtNum, readVarName, current_proc_name);

	return readNode; 
}

// print: 'print' var_name';'
PrintNode SourceParser::ParsePrint(int stmtNum)
{

	FindToken(source_tokens::kPrint);
	std::string printVarName = FindToken(source_tokens::kVarName).value;
	FindToken(source_tokens::kSemiColon);
	
	PrintNode node;
	node = dex.AddPrintToAST(stmtNum, printVarName, current_proc_name);

	return node;
}

// call: 'call' proc_name ';'
CallNode SourceParser::ParseCall(int stmtNum)
{
	// SYNTAX GRAMMAR
	FindToken(source_tokens::kCall);
	std::string callProcName = FindToken(source_tokens::kProcName).value;
	FindToken(source_tokens::kSemiColon);
	
	CallNode node;
	node = dex.AddCallToAST(stmtNum, callProcName, current_proc_name);

	return node;
}

// while: 'while' '(' cond_expr ')' '{' stmtLst '}'
WhileNode SourceParser::ParseWhile(int stmtNum) 
{
	// SYNTAX GRAMMAR
	FindToken(source_tokens::kWhile);
	FindToken(source_tokens::kLeftParenthesis);
	std::list<lexer::Token> postfix = ParseWhileCondExpr();
	FindToken(source_tokens::kRightParenthesis);
	FindToken(source_tokens::kLeftBrace);
	std::vector<StmtNode> nodes = ParseStmtLst();
	FindToken(source_tokens::kRightBrace);

	WhileNode whileNode;
	whileNode = dex.AddWhileToAST(stmtNum, expr_parser::TokensToString(postfix), nodes, current_proc_name);

	return whileNode;

}

std::list<lexer::Token> SourceParser::ParseWhileCondExpr()
{
	if (tokens.empty())
	{
		throw std::logic_error("Source syntax error: While condition expression cannot be empty");
	}

	std::list<lexer::Token> condExprTokens;
	// loop until "){"	
	while (!(*(tokens.begin()) == source_tokens::kRightParenthesis &&
		*(std::next(tokens.begin())) == source_tokens::kLeftBrace))
	{
		// loop until last 2 tokens
		if (tokens.size() == 2)
		{
			throw std::logic_error("Source syntax error in parsing while condexpr");
		}
		condExprTokens.emplace_back(tokens.front());
		tokens.pop_front();
	}

	if (condExprTokens.empty())
	{
		throw std::logic_error("Source syntax error: While condition expression cannot be empty");
	}

	return expr_parser::ParseExpr(condExprTokens);
}

// if: 'if' '(' cond_expr ')' 'then' '{' stmtLst '}' 'else' '{' stmtLst '}'
IfNode SourceParser::ParseIf(int stmtNum)
{
	// SYNTAX GRAMMAR
	FindToken(source_tokens::kIf);
	FindToken(source_tokens::kLeftParenthesis);
	std::list<lexer::Token> postfix = ParseIfCondExpr();
	FindToken(source_tokens::kRightParenthesis);
	FindToken(source_tokens::kThen);
	FindToken(source_tokens::kLeftBrace);
	std::vector<StmtNode> thenNodes = ParseStmtLst();
	FindToken(source_tokens::kRightBrace);
	FindToken(source_tokens::kElse);
	FindToken(source_tokens::kLeftBrace);
	std::vector<StmtNode> elseNodes = ParseStmtLst();
	FindToken(source_tokens::kRightBrace);

	IfNode ifNode;
	ifNode = dex.AddIfToAST(stmtNum, expr_parser::TokensToString(postfix), elseNodes, thenNodes, current_proc_name);

	return ifNode;

}

// will call subcomponent: ExprParser for further validation
std::list<lexer::Token> SourceParser::ParseIfCondExpr()
{
	if (tokens.empty())
	{
		throw std::logic_error("Source syntax error: If condition expression cannot be empty");
	}

	std::list<lexer::Token> condExprTokens;
	// loop until "){"
	while (!(*(tokens.begin()) == source_tokens::kRightParenthesis &&
		*(std::next(tokens.begin())) == source_tokens::kThen))
	{
		// loop until last 2 tokens
		if (tokens.size() == 2)
		{
			throw std::logic_error("Source syntax error in parsing if condexpr");
		}
		condExprTokens.emplace_back(tokens.front());
		tokens.pop_front();
	}

	if (condExprTokens.empty())
	{
		throw std::logic_error("Source syntax error: While condition expression cannot be empty");
	}

	return expr_parser::ParseExpr(condExprTokens);
}


// assign: var_name '=' expr ';'
AssignNode SourceParser::ParseAssign(int stmtNum)
{

	auto varName = FindToken(source_tokens::kVarName).value;
	FindToken(source_tokens::kAssign);
	std::list<lexer::Token> postfix = ParseAssignExpr();
	FindToken(source_tokens::kSemiColon);

    AssignNode node = dex.AddAssignToAST(stmtNum, varName, expr_parser::TokensToString(postfix), current_proc_name);

	return node;
}

// extract out the tokens in assign's expr
// will call subcomponent: ExprParser for further validation 
// return postfix of tokens list
std::list<lexer::Token> SourceParser::ParseAssignExpr()
{
	if (tokens.empty()) {
		throw std::logic_error("Source syntax error: rhs assign expression cannot be empty");
	}

	std::list<lexer::Token> exprTokens;

	// extraction
	while (tokens.front() != source_tokens::kSemiColon)
	{
		// last element still no semicolon
		if (tokens.size() == 1)
		{
			throw std::logic_error("Source syntax error in parsing assign expr");
		}
		exprTokens.emplace_back(tokens.front());
		tokens.pop_front();
	}

	std::list<lexer::Token> postfix = expr_parser::ParseExpr(exprTokens);

	// check if include relation operator
	for (Token token : postfix)
	{
		if (expr_parser::IsRelationOperator(token)) {
			throw std::logic_error("Syntax error: Non-arithmetic operators found in assignment expression");
		}
	}

	return postfix;
}

// after end of parsing, do updates on PKB by calling design extractor
void SourceParser::finalise() {
	
	dex.SetMaxStmtNum(GetStmtCounter() - 1);
	dex.UpdateFollowsStarTable();
	dex.UpdateParentStarTable();

	if (!dex.UpdateCallsStarTable()) {
		throw std::logic_error("[SourceParser::Parse] cyclic calls detected");
	}

	dex.UpdateNestedModifiesPAndUsesP();
	dex.UpdateModifiesAndUsesForCall();
	dex.UpdateNextTable();

	if (dex.CheckNonExistentCall()) {
		throw std::logic_error("[SourceParser::Parse] non existent calls detected");
	}
}

SourceParser::SourceParser()
{
	this->stmt_counter = 0;
}

SourceParser::SourceParser(PKB *pkbIn)
{
    dex = DesignExtractor(pkbIn);
    this->stmt_counter = 0;
}

// get current stmt num and increase the counter
int SourceParser::GetStmtCounter()
{
	return ++stmt_counter;
}

// parse and then finalise
void SourceParser::Parse(std::istream& fileStream)
{
	tokens = lexer::Tokenize(fileStream);
	SourceParser::ParseProgram();

	SourceParser::finalise();
};

// stmt: read | print | call | while | if | assign
// check to parse which statement:
// E.g. while = while + while
StmtNode SourceParser::ParseStmt()
{
	int currStmtNum = GetStmtCounter();
	StmtNode node;

	// parse assign stmt
	if (tokens.begin()->type == source_tokens::kVarName.type && *(std::next(tokens.begin())) == source_tokens::kAssign)
	{
		node = ParseAssign(currStmtNum);
	}
	// parse read stmt
	else if (tokens.front() == source_tokens::kRead)
	{
		node = ParseRead(currStmtNum);
	}
	// parse print stmt
	else if (tokens.front() == source_tokens::kPrint)
	{
		node = ParsePrint(currStmtNum);
	}
	// parse call stmt
	else if (tokens.front() == source_tokens::kCall)
	{
		node = ParseCall(currStmtNum);
	}
	// parse while stmt
	else if (tokens.front() == source_tokens::kWhile)
	{
		node = ParseWhile(currStmtNum);
	}
	// parse if stmt
	else if (tokens.front() == source_tokens::kIf)
	{
		node = ParseIf(currStmtNum);
	}
	// throw error
	else
	{
		throw std::logic_error("Source syntax error: not recognised stmt type in ParseStmt()");
	}

	return node;
}


void SourceParser::SetToken(std::list<lexer::Token> t) {
    for (std::list<lexer::Token> ::iterator it = t.begin(); it != t.end(); it++) {
        tokens.emplace_back(*it);
    }
}

void SourceParser::PrintToken() {
    for (std::list<lexer::Token> ::iterator it1 = tokens.begin(); it1 != tokens.end(); it1++) {
        cout << it1->value << endl;
    }
}
void SourceParser::Parse()
{
    ParseProgram();
};
