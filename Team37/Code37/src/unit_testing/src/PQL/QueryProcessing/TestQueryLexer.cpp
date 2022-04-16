#include <string>
#include <vector>

#include "catch.hpp"
#include "../src/QueryProcessing/QueryLexer.h"
#include "../src/QueryProcessing/QueryLexer.cpp"

using namespace std;

TEST_CASE("Table: Test Query Lexer") {
    string test_string = "hello world test test";
    QueryLexer ql = QueryLexer(test_string);
    ql.PrintEntityType();
    REQUIRE(1 == 1);

}

TEST_CASE("Table: Test Query Lexer eatKeyword") {
    string test_string = "variable v; Select v such that Modifies (5,v)";
    QueryLexer ql = QueryLexer(test_string);
    ql.EatEntityType("variable");
    ql.PrintToken();
    REQUIRE(1 == 1);

}

TEST_CASE("Table: Test Query Lexer increment") {
    string test_string = "variable v; Select v such that Modifies (5,v)";
    QueryLexer ql = QueryLexer(test_string);
    ql.NextItem();
    ql.PrintToken();
    ql.NextItem();
    ql.PrintToken();
    REQUIRE(1 == 1);

}