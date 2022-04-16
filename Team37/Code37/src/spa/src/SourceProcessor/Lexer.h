#ifndef INC_21S2_CP_SPA_TEAM_37_LEXER_H
#define INC_21S2_CP_SPA_TEAM_37_LEXER_H

#include <list>
#include <string>

// Lexer component to return SourceParser a list of token by calling tokenize()
namespace lexer {
	enum class TokenType {kName, kInteger, kDelimiter, kOperator};

	struct Token 
	{
		const TokenType type;
		const std::string value;

		friend bool operator == (const Token& lhs, const Token& rhs) {
			return lhs.type == rhs.type && lhs.value == rhs.value;
		}
		
		friend bool operator != (const Token& lhs, const Token& rhs) {
			return !(lhs == rhs);
		}

		friend bool operator < (const Token& lhs, const Token& rhs) {
			return lhs.type != rhs.type ? lhs.type < rhs.type : lhs.value < rhs.value;
		}

	};

	// convert fileStream into a list of Lexer::Token
	std::list<Token> Tokenize(std::istream &fileStream);
}

#endif  // INC_21S2_CP_SPA_TEAM_37_LEXER_H