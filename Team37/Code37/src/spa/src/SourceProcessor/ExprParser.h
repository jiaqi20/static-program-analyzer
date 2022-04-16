#ifndef INC_21S2_CP_SPA_TEAM_37_EXPRPARSER_H
#define INC_21S2_CP_SPA_TEAM_37_EXPRPARSER_H

#include <list>
#include <stdio.h>
#include <iostream>
#include <map>
#include <set>

#include "Lexer.h"

using namespace lexer;

namespace expr_tokens {

	// OPERATOR
	const static Token kNot { TokenType::kOperator, "!" };
	const static Token kAnd { TokenType::kOperator, "&&" };
	const static Token kOr { TokenType::kOperator, "||" };
	const static Token kPlus { TokenType::kOperator, "+" };
	const static Token kMinus { TokenType::kOperator, "-" };
	const static Token kMultiply { TokenType::kOperator, "*" };
	const static Token kDivide { TokenType::kOperator, "/" };
	const static Token kModulo { TokenType::kOperator, "%" };
	const static Token kEqual { TokenType::kOperator, "==" };
	const static Token kNotEqual { TokenType::kOperator, "!=" };
	const static Token kGreaterThan { TokenType::kOperator, ">" };
	const static Token kGreaterThanEqual { TokenType::kOperator, ">=" };
	const static Token kLesserThan { TokenType::kOperator, "<" };
	const static Token kLesserThanEqual { TokenType::kOperator, "<=" };

	// DELIMITER
	const static Token kLeftParenthesis { TokenType::kDelimiter, "(" };
	const static Token kRightParenthesis { TokenType::kDelimiter, ")" };
}

// sub component in Parser: to parse list of expr tokens into rpn version of tokens list 
namespace expr_parser {
	// precedenceMap <Token, precedenceNumber>
	static std::map<Token, int> precedenceMap = 
	{
		// relational
		std::make_pair(expr_tokens::kOr, 1),
		std::make_pair(expr_tokens::kAnd, 1),
		std::make_pair(expr_tokens::kGreaterThan, 2),
		std::make_pair(expr_tokens::kGreaterThanEqual, 2),
		std::make_pair(expr_tokens::kLesserThan, 2),
		std::make_pair(expr_tokens::kLesserThanEqual, 2),
		std::make_pair(expr_tokens::kEqual, 2),
		std::make_pair(expr_tokens::kNotEqual, 2),
		// arithmetic
		std::make_pair(expr_tokens::kPlus, 3),
		std::make_pair(expr_tokens::kMinus, 3),
		std::make_pair(expr_tokens::kModulo, 4),
		std::make_pair(expr_tokens::kMultiply, 4),
		std::make_pair(expr_tokens::kDivide, 4) 
	};

	// 
	static std::set<Token> relationOperatorTokens =
	{
		expr_tokens::kOr,
		expr_tokens::kAnd,
		expr_tokens::kGreaterThan,
		expr_tokens::kGreaterThanEqual,
		expr_tokens::kLesserThan,
		expr_tokens::kLesserThanEqual,
		expr_tokens::kEqual,
		expr_tokens::kNotEqual
	};

	static std::set<Token> arithmeticOperatorTokens =
	{
		expr_tokens::kPlus,
		expr_tokens::kMinus,
		expr_tokens::kModulo,
		expr_tokens::kMultiply,
		expr_tokens::kDivide
	};

	std::list<Token> ParseExpr(std::list<Token>& tokens);

	// util function
	bool IsRelationOperator(Token token);
	bool IsArithmeticOperator(Token token);
	std::string TokensToString(std::list<Token> tokens);
}

#endif // INC_21S2_CP_SPA_TEAM_37_EXPRPARSER_H