#include <string>
#include <fstream>
#include <list>
#include <iostream>

#include "../src/SourceProcessor/ExprParser.h"

#include "catch.hpp"

using namespace std;
using namespace lexer;

TEST_CASE("expr_parser::parseExpr") {

    Token token1 = { TokenType::kName, "x" };
    Token token2 = { TokenType::kName, "y" };
    Token token3 = { TokenType::kName, "z" };

    Token token4 = { TokenType::kOperator, "==" };
    Token token5 = { TokenType::kOperator, "*" };
    Token token6 = { TokenType::kOperator, "+" };
    Token token7 = { TokenType::kOperator, "(" };
    Token token8 = { TokenType::kOperator, ")" };

    Token token9 = { TokenType::kInteger, "100" };
    Token token10 = { TokenType::kInteger, "200" };
    Token token11 = { TokenType::kInteger, "abc123" };

    SECTION("basic")
    {
        std::list<Token> tokens;
        tokens.emplace_back(token1);
        tokens.emplace_back(token6);
        tokens.emplace_back(token2); // x + y

        std::list<Token> rpnTokens = expr_parser::ParseExpr(tokens);
        std::string result = "x|y|+";

        REQUIRE(expr_parser::TokensToString(rpnTokens) == result);

    }

    SECTION("complex")
    {
        std::list<Token> tokens;
        tokens.emplace_back(token11);
        tokens.emplace_back(token6);
        tokens.emplace_back(token2); 
        tokens.emplace_back(token5); 
        tokens.emplace_back(token1); // abc123 + y * x

        std::list<Token> rpnTokens = expr_parser::ParseExpr(tokens);
        std::string result = "abc123|y|x|*+";

        REQUIRE(expr_parser::TokensToString(rpnTokens) == result);

    }

}

TEST_CASE("expr_parser::isArithmeticOperator") {

    Token notArithmeticToken = { TokenType::kOperator, "==" };
    Token arithmeticToken = { TokenType::kOperator, "+" };

    SECTION("is or not")
    {
        REQUIRE(expr_parser::IsArithmeticOperator(arithmeticToken));
        REQUIRE(!expr_parser::IsArithmeticOperator(notArithmeticToken));
    }

}

TEST_CASE("expr_parser::isRelationOperator") {

    Token notRelationToken = { TokenType::kOperator, "+" };
    Token relationToken = { TokenType::kOperator, "<=" };

    SECTION("is or not")
    {
        REQUIRE(expr_parser::IsRelationOperator(relationToken));
        REQUIRE(!expr_parser::IsRelationOperator(notRelationToken));
    }

}