#ifndef INC_21S2_CP_SPA_TEAM_37_SOURCEPARSER_H
#define INC_21S2_CP_SPA_TEAM_37_SOURCEPARSER_H

#include <string>
#include <vector>

#include "ASTNode.h"
#include "Lexer.h"
#include "DesignExtractor.h"

using namespace std;
using namespace lexer;

namespace source_tokens {
	 
	const static Token kProcedure { TokenType::kName, "procedure" };
	const static Token kRead { TokenType::kName, "read" };
	const static Token kPrint { TokenType::kName, "print" };
	const static Token kCall { TokenType::kName, "call" };
	const static Token kWhile { TokenType::kName, "while" };
	const static Token kIf { TokenType::kName, "if" };
	const static Token kThen { TokenType::kName, "then" };
	const static Token kElse { TokenType::kName, "else" };

	// OPERATOR
	const static Token kAssign { TokenType::kOperator, "=" };

	// DELIMITER
	const static Token kLeftBrace { TokenType::kDelimiter, "{" };
	const static Token kRightBrace { TokenType::kDelimiter, "}" };
	const static Token kLeftParenthesis { TokenType::kDelimiter, "(" };
	const static Token kRightParenthesis { TokenType::kDelimiter, ")" };
	const static Token kSemiColon { TokenType::kDelimiter, ";" };

	// Procedure NAME, Variable NAME
	const static Token kProcName { TokenType::kName, "" };
	const static Token kVarName { TokenType::kName, "" };
}; // namespace SourceTokens

class SourceParser
{
private:

	// DesignExtractor object
	DesignExtractor dex;

	// storing all the tokens from Lexer
	std::list<Token> tokens;

	// a counter to save current stmt number
	int stmt_counter;

	// a variable to save current procedure name
	std::string current_proc_name;

	Token FindToken(Token token);
	
	void ParseProgram();

    void ParseProcedure();

    std::vector<StmtNode> ParseStmtLst();

	ReadNode ParseRead(int stmtNum);
	PrintNode ParsePrint(int stmtNum);
	CallNode ParseCall(int stmtNum);
	
	WhileNode ParseWhile(int stmtNum);
	std::list<Token> ParseWhileCondExpr();

	IfNode ParseIf(int stmtNum);
	std::list<Token> ParseIfCondExpr();
	
	AssignNode ParseAssign(int stmtNum);
	std::list<Token> ParseAssignExpr();

	void finalise();

public:
	SourceParser();
    SourceParser(PKB *pkbIN);

	int GetStmtCounter();
	void Parse(std::istream& fileStream);
	
	StmtNode ParseStmt();

	// for testing purposes
    void Parse();
    void SetToken(std::list<Token> tokens);
    void PrintToken();

};

#endif //INC_21S2_CP_SPA_TEAM_37_SOURCEPARSER_H