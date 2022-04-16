#include "ExprParser.h"

using namespace lexer;
using namespace std;


// parse expr tokens and return a list of rpn tokens
// RPN = Reverse Polish Notation
std::list<Token> expr_parser::ParseExpr(std::list<Token>& tokens)
{
	// After !, before and after: && and ||, there are parenthesis
	for (std::list<Token>::iterator it = tokens.begin(); it != tokens.end(); ++it)
	{
		// check after !, &&, ||
		if ((*it) == expr_tokens::kOr || (*it) == expr_tokens::kAnd || (*it) == expr_tokens::kNot)
		{
			if (std::next(it) == tokens.end() || *std::next(it) != expr_tokens::kLeftParenthesis) 
			{
				// throw error
				throw std::logic_error("Syntax error on expression: " + it->value + " not followed by (");
			}
		}

		// check before &&, ||
		if ((*it) == expr_tokens::kAnd || (*it) == expr_tokens::kOr) 
		{
			if (std::prev(it) == tokens.begin() || *std::prev(it) != expr_tokens::kRightParenthesis) 
			{
				throw std::logic_error("Syntax error on expression: " + it->value + " not preceded by )");
			}
		}

	}

	// shunting-yard algorithm:
	// stack of operators including parenthesis
	std::list<Token> stack;
	// rpn of results
	std::list<Token> queueResult;
	std::list<Token> tokenInputs = tokens;

	//while tokens not empty
	while (!tokenInputs.empty())
	{
		// read a token
		Token input = tokenInputs.front();
		tokenInputs.pop_front();

		// add number/name into the back of queueresult
		if (input.type == TokenType::kInteger || input.type == TokenType::kName) 
		{
			queueResult.push_back(input);
		} 
		// add not operator into stack
		else if (input == expr_tokens::kNot) 
		{
			stack.push_back(input);
		}
		// add left parenthesis into stack
		else if (input == expr_tokens::kLeftParenthesis)
		{
			stack.push_back(input);
		}
		// add operators other than not/leftparenthesis operator
		else if (precedenceMap.count(input))
		{
			// While (operator at the top of the stack is not a parenthesis) and (
			// (function is at the top of the stack) or (operator at the top of the stack has greater or equal precedence))
			while (!stack.empty() && stack.back() != expr_tokens::kLeftParenthesis &&
				(stack.back() == expr_tokens::kNot || precedenceMap[stack.back()] >= precedenceMap[input])) {		
				
				// Pop the operator stack into the queue
				queueResult.push_back(stack.back());
				stack.pop_back();
			}			
			// Push operator into the operator stack
			stack.push_back(input);
		}
		// handle right parenthesis
		else if (input == expr_tokens::kRightParenthesis)
		{
			// loop until leftparenthesis: pop_back stack and push_back result
			while (!stack.empty() && stack.back() != expr_tokens::kLeftParenthesis) {
				queueResult.push_back(stack.back());
				stack.pop_back();
			}
			
			// no leftparenthesis, then throw parenthesis mismatch error
			if (stack.empty()) {
				throw std::logic_error("Syntax error: Expression contains mismatched parentheses");
			}

			// Discard the left parentheses
			if (stack.back() == expr_tokens::kLeftParenthesis) {
				stack.pop_back();
			}
		}
		else {
			throw std::logic_error("Syntax error: unrecognised token while parsing expression");
		}
	}
	// pop the operator stack
	while (!stack.empty())
	{
		Token op = stack.back();
		stack.pop_back();
		
		if (op == expr_tokens::kLeftParenthesis) {
			throw std::logic_error("Syntax error: Expression contains mismatched parentheses");
		}
		queueResult.push_back(op);
	}
	return queueResult;
}

bool expr_parser::IsRelationOperator(Token token)
{
	return relationOperatorTokens.find(token) != relationOperatorTokens.end();
}

bool expr_parser::IsArithmeticOperator(Token token)
{
	return arithmeticOperatorTokens.find(token) != arithmeticOperatorTokens.end();;
}

std::string expr_parser::TokensToString(std::list<Token> tokens)
{
	std::string str;

	// append to str
	while (!tokens.empty())
	{
		Token token = tokens.front();

		// str output has to depend on design extractor::rpn
		if (token.type == TokenType::kInteger || token.type == TokenType::kName)
		{
			str.append(token.value + "|");
			tokens.pop_front();
		}
		else {
			str.append(token.value);
			tokens.pop_front();
		}

		
	}
	return str;
}
