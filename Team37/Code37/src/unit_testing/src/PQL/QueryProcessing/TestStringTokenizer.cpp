#include <string>
#include <vector>

#include "catch.hpp"
#include "../src/QueryProcessing/StringTokenizer.h"
#include "../src/QueryProcessing/StringTokenizer.cpp"

using namespace std;

TEST_CASE("Table: Test Tokenizer") {
    SECTION("test 1") {
        string test_string = "variable v; Select v such that Modifies (5, v)";
        StringTokenizer a = StringTokenizer(test_string);
        vector<string> tokens;
        a.TokenizeString(tokens);
        vector<string> expectedTokens{ "variable", "v", ";", "Select", "v", "such", "that", "Modifies",
        "(", "5", ",","v", ")" };
        REQUIRE(tokens == expectedTokens);
    }

    SECTION("test 2") {
        string test_string = "variable v; assign a;\n\rSelect v such that Modifies (a, v) pattern a(_, v)";
        StringTokenizer a = StringTokenizer(test_string);
        vector<string> tokens;
        a.TokenizeString(tokens);
        vector<string> expectedTokens{ "variable", "v", ";", "assign", "a", ";","Select", "v", "such", "that",
            "Modifies", "(", "a", ",","v", ")", "pattern", "a", "(", "_", ",", "v", ")" };
        REQUIRE(tokens == expectedTokens);
    }

    SECTION("test 3") {
        string test_string = "stmt s;\nSelect s such that Modifies (s, \"x\")";
        StringTokenizer a = StringTokenizer(test_string);
        vector<string> tokens;
        a.TokenizeString(tokens);
        vector<string> expectedTokens{ "stmt", "s", ";", "Select", "s", "such", "that",
            "Modifies", "(", "s", ",","\"","x","\"", ")"};
        REQUIRE(tokens == expectedTokens);
    }

    SECTION("test 4") {
        string test_string = "assign a; while w;\n"
                             "Select w such that Parent* (w, a) pattern a (\"count\", _)";
        StringTokenizer a = StringTokenizer(test_string);
        vector<string> tokens;
        a.TokenizeString(tokens);
        vector<string> expectedTokens{ "assign", "a", ";", "while", "w", ";", "Select", "w", "such", "that",
                                       "Parent", "*", "(", "w", ",", "a", ")", "pattern", "a", "(", "\"",
                                        "count","\"",",", "_", ")"};
        REQUIRE(tokens == expectedTokens);
    }

    SECTION("test 5") {
        string test_string = "assign a;\n Select a pattern a(\"x\", _\"x+y *z / 5- 1\"_)";
        StringTokenizer a = StringTokenizer(test_string);
        vector<string> tokens;
        a.TokenizeString(tokens);
        vector<string> expectedTokens{ "assign", "a", ";", "Select", "a", "pattern", "a", "(", "\"",
                                        "x","\"",",", "_", "\"", "x", "+", "y", "*", "z", "/", "5", "-", "1", "\"", "_", ")" };
        REQUIRE(tokens == expectedTokens);
    }

    SECTION("test 6") {
        string test_string = "variable v; Select v";
        StringTokenizer a = StringTokenizer(test_string);
        vector<string> tokens;
        a.TokenizeString(tokens);
        vector<string> expectedTokens{ "variable", "v", ";", "Select", "v"};
        REQUIRE(tokens == expectedTokens);
    }

    SECTION("test 7") {
        string test_string = "variable v; stmt s; Select s";
        StringTokenizer a = StringTokenizer(test_string);
        vector<string> tokens;
        a.TokenizeString(tokens);
        vector<string> expectedTokens{ "variable", "v", ";","stmt", "s", ";", "Select", "s" };
        REQUIRE(tokens == expectedTokens);
    }

    SECTION("test 8") {
        string test_string = "assign a;\n Select a pattern a(\"x\", _\"x%y *z % 5- 1\"_)";
        StringTokenizer a = StringTokenizer(test_string);
        vector<string> tokens;
        a.TokenizeString(tokens);
        vector<string> expectedTokens{ "assign", "a", ";", "Select", "a", "pattern", "a", "(", "\"",
                                        "x","\"",",", "_", "\"", "x", "%", "y", "*", "z", "%", "5", "-", "1", "\"", "_", ")" };
        REQUIRE(tokens == expectedTokens);
    }
}