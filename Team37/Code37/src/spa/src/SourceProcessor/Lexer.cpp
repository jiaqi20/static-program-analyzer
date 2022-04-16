#include <istream>
#include <cctype> /* isalnum, isdigit, isspace, ispunct */

#include "Lexer.h"

using namespace std;
using namespace lexer;

namespace {

	// Return Token with NAME type.
	// NAME: LETTER ( LETTER | DIGIT )*
	// LETTER: A-Z | a-z
	// DIGIT: 0-9
	Token GetTokenName(std::istream& stream)
	{
		std::string str;

		// loop all LETTER or DIGIT
		while (std::isalnum(stream.peek())) {
			str += char(stream.get());
		}
		return { TokenType::kName, str };
	}

	// Return Token with INTEGER type.
	// INTEGER: DIGIT+
	// DIGIT: 0-9
    Token GetTokenInteger(std::istream& stream)
	{
		std::string str;

		// loop all DIGIT
		while (std::isdigit(stream.peek())) {
			str += char(stream.get());
		}
		return { TokenType::kInteger, str };
	}

	// Return Token with DELIMITER/OPERATOR type
	// DELIMITER: { } ( ) ; " , . # _
	// OPERATOR: + - * / % = == ! != > >= < <= && ||
    Token GetTokenPunct(std::istream& stream)
	{
		TokenType type;
		std::string value;
		
		switch (stream.peek()) {
		case '{':
		case '}':
		case '(':
		case ')':
		case ';':
		case '"':
		case ',':
		case '.':
		case '#':
		case '_':
			type = TokenType::kDelimiter;
			value += char(stream.get());
			break;
		case '+':
		case '-':
		case '*':
		case '/':
		case '%':
			type = TokenType::kOperator;
			value += char(stream.get());
			break;
		case '!':
		case '>':
		case '<':
		case '=':
			type = TokenType::kOperator;
			value += char(stream.get());
			if (stream.peek() == '=') 
			{
				value += char(stream.get());
			}
			break;
		case '&':
		case '|':
			type = TokenType::kOperator;
			value += char(stream.get());
			if (stream.peek() == value[0]) 
			{
				value += char(stream.get());
			}
			else 
			{
				throw std::logic_error("Error in lexer: could not find && or || operator, encountered: " + char(stream.peek()));
			}
			break;
		default:
			throw std::logic_error("Error in lexer: unknown punctuation encountered: " + char(stream.peek()));
		}
		return { type, value };
	}

	// remove spaces from the stream
	void RemoveSpace(std::istream& stream) 
	{	
		while (std::isspace(stream.peek())) {
			char(stream.get());
		}
	}
}


namespace lexer {
	std::list<Token> Tokenize(std::istream& fileStream)
	{
		std::list<Token> tokens;

		while (fileStream.peek() != EOF)
		{			
			// do lexing
			if (std::isalpha(fileStream.peek())) {
				tokens.emplace_back(GetTokenName(fileStream));
			}
			else if (std::isdigit(fileStream.peek())) {
				tokens.emplace_back(GetTokenInteger(fileStream));
			}
			else if (std::ispunct(fileStream.peek())) {
				tokens.emplace_back(GetTokenPunct(fileStream));
			}
			else if (std::isspace(fileStream.peek())) {
				RemoveSpace(fileStream);
			}
			else {
				throw std::logic_error("Error in lexer: unknown character encountered: " + char(fileStream.peek()));
			}
		}
		return tokens;
	}

}