#include <string>
#include <vector>

#include "catch.hpp"
#include "../src/QueryProcessing/QueryParser.h"
#include "../src/QueryProcessing/QueryParser.cpp"
#include "../src/QueryProcessing/DataRequester.cpp"
#include "../src/model/ConditionResultTable.h"

using namespace std;

TEST_CASE("Table: Test Query - contains Pattern only") {

    SECTION( "Test wildcard  and complete match" ) {
        string test_string = "assign a; variable v; Select a pattern a ( _ , \"v\")";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select a pattern a(_, \"v\") ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Test wildcard  and wildcard" ) {
        string test_string = "assign a; variable v; Select a pattern a ( _ , _)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select a pattern a(_, _) ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Test wildcard  and wildcard" ) {
        string test_string = "assign a; variable v; Select a pattern a ( _ , _\"x+y+z*a\"_)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select a pattern a(_, _\"x+y+z*a\"_) ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Test weird pattern" ) {
        string test_string = "assign a; variable v; Select a pattern a ( _ , \"false\")";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select a pattern a(_, \"false\") ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Test weird pattern that has keyword while" ) {
        string test_string = "assign a; variable v; Select a pattern a ( _ , \"while\")";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select a pattern a(_, \"while\") ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Test weird pattern partial match" ) {
        string test_string = "assign a; variable v; Select a pattern a ( _ , _\"false\"_)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select a pattern a(_, _\"false\"_) ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Test weird pattern that has keyword while partial match" ) {
        string test_string = "assign a; variable v; Select a pattern a ( _ , _\"while\"_)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select a pattern a(_, _\"while\"_) ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Test weird pattern that has number partial match" ) {
        string test_string = "assign a; variable v; Select a pattern a ( _ , \"100000000000000\")";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select a pattern a(_, \"100000000000000\") ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Test weird pattern that has number partial match" ) {
        string test_string = "assign a; variable v; Select a pattern a ( _ , _\"100000000000000\"_)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select a pattern a(_, _\"100000000000000\"_) ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Test weird pattern that super long name" ) {
        string test_string = "assign a; variable v; Select a pattern a ( _ , _\"QWERTYUIOPASDFGHJKLZXCVBNM12345678901234567890zxcvbbbnnmmmmmmmmmmmmasdfghjklqwertyuiop\"_)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select a pattern a(_, _\"QWERTYUIOPASDFGHJKLZXCVBNM12345678901234567890zxcvbbbnnmmmmmmmmmmmmasdfghjklqwertyuiop\"_) ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Test weird pattern that super long name" ) {
        string test_string = "assign a; variable v; Select a pattern a ( _ , \"QWERTYUIOPASDFGHJKLZXCVBNM12345678901234567890zxcvbbbnnmmmmmmmmmmmmasdfghjklqwertyuiop\")";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select a pattern a(_, \"QWERTYUIOPASDFGHJKLZXCVBNM12345678901234567890zxcvbbbnnmmmmmmmmmmmmasdfghjklqwertyuiop\") ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Test weird pattern that has 0" ) {
        string test_string = "assign a; variable v; Select a pattern a ( _ , \"0\")";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select a pattern a(_, \"0\") ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Test weird pattern that has 0 and left parameter is fixed variable" ) {
        string test_string = "assign a; variable v; Select a pattern a ( \"true\" , \"0\")";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select a pattern a(\"true\", \"0\") ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Test weird pattern that has 0 and left parameter is fixed variable1" ) {
        string test_string = "assign a; variable v; Select a pattern a ( \"true\" , \"while\")";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select a pattern a(\"true\", \"while\") ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Test weird pattern that has 0 and left parameter is fixed variable2" ) {
        string test_string = "assign a; variable v; Select a pattern a ( \"true\" , \"kVariable\")";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select a pattern a(\"true\", \"kVariable\") ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Test weird pattern that has 0 and left parameter is fixed variable3" ) {
        string test_string = "assign a; variable v; Select a pattern a ( \"true\" , _\"SELECT\"_)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select a pattern a(\"true\", _\"SELECT\"_) ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Negative: Missing pattern assign" ) {
        string test_string = "assign a; variable v; Select a pattern ( \"true\" , _\"SELECT\"_)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "pattern name is not of a valid ident";
        REQUIRE(expectedResult == result.ToString());
    }
}

TEST_CASE("Table: Test Error Message") {

    SECTION( "Declaration: Invalid Name" ) {
        string test_string = "assign 1a; variable v; Select a pattern a ( _ , \"v\")";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "declaration name first character should be letter";
        REQUIRE(expectedResult == result.GetErrorMessage());
    }

    SECTION( "Declaration: Invalid declaration" ) {
        string test_string = "assignkkkk 1a; variable v; Select a pattern a ( _ , \"v\") ";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "unsupported design entity";
        REQUIRE(expectedResult == result.GetErrorMessage());
    }

    SECTION( "Relation: Unsupported relationship" ) {
        string test_string = "assign a; variable v; Select a such that Heloo(a, b)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "relationship not supported";
        REQUIRE(expectedResult == result.GetErrorMessage());
    }

    SECTION( "Relation: missing that" ) {
        string test_string = "assign a; variable v; Select a such Heloo(a, b)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "missing that after such";
        REQUIRE(expectedResult == result.GetErrorMessage());
    }

    SECTION( "Relation: entity not declared" ) {
        string test_string = "assign a; Select v such Modifies(a, b)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "entity name is not declared";
        REQUIRE(expectedResult == result.GetErrorMessage());
    }

    SECTION( "Relation: entity not declared" ) {
        string test_string = "variable a; Select a pattern a (_, _)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "pattern name is not declared";
        REQUIRE(expectedResult == result.GetErrorMessage());
    }

    SECTION( "Relation: invalid terminator" ) {
        string test_string = "variable v; assign a; Select v pattern a (_, _); ";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "invalid terminator";
        REQUIRE(expectedResult == result.GetErrorMessage());
    }

    SECTION( "Relation: modify left should not be _" ) {
        string test_string = "variable v; assign a; Select v such that Modifies(_, v) ";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "first argument cannot be _";
        REQUIRE(expectedResult == result.GetErrorMessage());
    }

    SECTION( "Relation: uses left should not be _" ) {
        string test_string = "variable v; assign a; Select v such that Uses(_, v) ";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "first argument cannot be _";
        REQUIRE(expectedResult == result.GetErrorMessage());
    }

    SECTION( "Pattern: first argument can only be variable synonym" ) {
        string test_string = "variable v; assign a; if ifs; Select v pattern a (ifs, _) ";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "pattern first argument can only be variable synonym or wildcard or double quote";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Only ; " ) {
        string test_string = ";";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "unsupported design entity";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "weird pattern " ) {
        string test_string = "assign a; Select a pattern a(_a_, _)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "missing valid delim";
        REQUIRE(expectedResult == result.ToString());
    }
}



TEST_CASE("Declaration: Test Weird Declaration") {

    SECTION( "Declaration: All keyboard letter" ) {
        string test_string = "variable QWERTYUIOPASDFGHJKLZXCVBNM; Select QWERTYUIOPASDFGHJKLZXCVBNM";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select QWERTYUIOPASDFGHJKLZXCVBNM ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Declaration: All keyboard letter + number" ) {
        string test_string = "variable QWERTYUIOPASDFGHJKLZXCVBNM1234567890; Select QWERTYUIOPASDFGHJKLZXCVBNM1234567890";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select QWERTYUIOPASDFGHJKLZXCVBNM1234567890 ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Declaration: All keyboard letter + long number" ) {
        string test_string = "variable QWERTYUIOPASDFGHJKLZXCVBNM12345678901234567890; Select QWERTYUIOPASDFGHJKLZXCVBNM12345678901234567890";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select QWERTYUIOPASDFGHJKLZXCVBNM12345678901234567890 ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Declaration: All keyboard letter + long number + letter" ) {
        string test_string = "variable QWERTYUIOPASDFGHJKLZXCVBNM12345678901234567890zxcvbbbnnmmmmmmmmmmmmasdfghjklqwertyuiop; Select QWERTYUIOPASDFGHJKLZXCVBNM12345678901234567890zxcvbbbnnmmmmmmmmmmmmasdfghjklqwertyuiop";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select QWERTYUIOPASDFGHJKLZXCVBNM12345678901234567890zxcvbbbnnmmmmmmmmmmmmasdfghjklqwertyuiop ";
        REQUIRE(expectedResult == result.ToString());
    }
}

TEST_CASE("Table: Test procedure used") {

    SECTION( "Declaration: All keyboard letter" ) {
        string test_string = "stmt s; assign a; variable v; constant c; while w; if ifs; procedure p; Select v such that Modifies(p,v)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select QWERTYUIOPASDFGHJKLZXCVBNM";
        REQUIRE(qp1.GetRelation().getRelationType() == 7);
    }

    SECTION( "Invalid pattern wildcard" ) {
        string test_string = "stmt s, s1; assign a; Select s such that Follows*(s,s1) pattern a(_\"z\"_, _)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "missing valid delim";
        REQUIRE(expectedResult == result.ToString());
    }

}

TEST_CASE("Declaration - statement") {

    SECTION( "Valid statement1" ) {
        string test_string = "stmt s; Select s";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select s ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Valid statement2" ) {
        string test_string = "stmt sssssssssssssssssss; Select sssssssssssssssssss";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select sssssssssssssssssss ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Valid statement3" ) {
        string test_string = "stmt zxcvbnmasdfghjklPOIUYTREWQ123456789; Select zxcvbnmasdfghjklPOIUYTREWQ123456789";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select zxcvbnmasdfghjklPOIUYTREWQ123456789 ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "InValid statement1" ) {
        string test_string = "stmt 1s; Select s";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "declaration name first character should be letter";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "InValid statement2" ) {
        string test_string = "stmt s!s; Select s!s";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "declaration name second character onwards should be letter or digit";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "InValid statement3" ) {
        string test_string = "stmt s,s; Select s,s";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "invalid terminator";
        REQUIRE(expectedResult == result.ToString());
    }
}

TEST_CASE("Declaration - variable") {

    SECTION( "Valid variable1" ) {
        string test_string = "variable s; Select s";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select s ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Valid variable2" ) {
        string test_string = "variable sssssssssssssssssss; Select sssssssssssssssssss";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select sssssssssssssssssss ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Valid variable3" ) {
        string test_string = "variable zxcvbnmasdfghjklPOIUYTREWQ123456789; Select zxcvbnmasdfghjklPOIUYTREWQ123456789";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select zxcvbnmasdfghjklPOIUYTREWQ123456789 ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "InValid variable1" ) {
        string test_string = "variable 1s; Select s";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "declaration name first character should be letter";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "InValid variable2" ) {
        string test_string = "variable s!s; Select s!s";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "declaration name second character onwards should be letter or digit";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "InValid variable3" ) {
        string test_string = "variable s,s; Select s,s";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "invalid terminator";
        REQUIRE(expectedResult == result.ToString());
    }
}

TEST_CASE("Declaration - procedure") {

    SECTION( "Valid procedure1" ) {
        string test_string = "procedure s; Select s";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select s ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Valid procedure2" ) {
        string test_string = "procedure sssssssssssssssssss; Select sssssssssssssssssss";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select sssssssssssssssssss ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Valid procedure3" ) {
        string test_string = "procedure zxcvbnmasdfghjklPOIUYTREWQ123456789; Select zxcvbnmasdfghjklPOIUYTREWQ123456789";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select zxcvbnmasdfghjklPOIUYTREWQ123456789 ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "InValid procedure1" ) {
        string test_string = "procedure 1s; Select s";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "declaration name first character should be letter";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "InValid procedure2" ) {
        string test_string = "procedure s!s; Select s!s";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "declaration name second character onwards should be letter or digit";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "InValid procedure3" ) {
        string test_string = "procedure s,s; Select s,s";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "invalid terminator";
        REQUIRE(expectedResult == result.ToString());
    }
}

TEST_CASE("Declaration - read") {

    SECTION( "Valid read1" ) {
        string test_string = "read s; Select s";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select s ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Valid read2" ) {
        string test_string = "read sssssssssssssssssss; Select sssssssssssssssssss";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select sssssssssssssssssss ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Valid read3" ) {
        string test_string = "read zxcvbnmasdfghjklPOIUYTREWQ123456789; Select zxcvbnmasdfghjklPOIUYTREWQ123456789";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select zxcvbnmasdfghjklPOIUYTREWQ123456789 ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "InValid read1" ) {
        string test_string = "read 1s; Select s";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "declaration name first character should be letter";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "InValid read2" ) {
        string test_string = "read s!s; Select s!s";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "declaration name second character onwards should be letter or digit";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "InValid read3" ) {
        string test_string = "read s,s; Select s,s";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "invalid terminator";
        REQUIRE(expectedResult == result.ToString());
    }
}

TEST_CASE("Declaration - assign") {

    SECTION( "Valid assign1" ) {
        string test_string = "assign s; Select s";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select s ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Valid assign2" ) {
        string test_string = "assign sssssssssssssssssss; Select sssssssssssssssssss";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select sssssssssssssssssss ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Valid assign3" ) {
        string test_string = "assign zxcvbnmasdfghjklPOIUYTREWQ123456789; Select zxcvbnmasdfghjklPOIUYTREWQ123456789";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select zxcvbnmasdfghjklPOIUYTREWQ123456789 ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "InValid assign1" ) {
        string test_string = "assign 1s; Select s";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "declaration name first character should be letter";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "InValid assign2" ) {
        string test_string = "assign s!s; Select s!s";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "declaration name second character onwards should be letter or digit";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "InValid assign3" ) {
        string test_string = "assign s,s; Select s,s";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "invalid terminator";
        REQUIRE(expectedResult == result.ToString());
    }
}

TEST_CASE("Declaration - all") {
    SECTION( "Valid combination1" ) {
        string test_string = "variable v; assign a; stmt s; procedure p; print pn; read r;  Select v";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select v ";
        REQUIRE(expectedResult == result.ToString());
    }
}

TEST_CASE("Relation - Modifies") {
    SECTION( "Test Modifies relationship, statement" ) {
        string test_string = "variable v; assign a; stmt s; Select v such that Modifies (1, v)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select v such that Modifies(1, v) ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Test Modifies relationship, variable" ) {
        string test_string = "variable v; assign a; stmt s; Select v such that Modifies (a, v)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select v such that Modifies(a, v) ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Test Modifies relationship, statement1" ) {
        string test_string = "variable v; assign a; stmt s; Select v such that Modifies (s, v)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select v such that Modifies(s, v) ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Test Modifies relationship, read" ) {
        string test_string = "variable v; assign a; stmt s; read r; Select v such that Modifies (r, v)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select v such that Modifies(r, v) ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Test Modifies relationship, procedure" ) {
        string test_string = "variable v; assign a; stmt s; read r; procedure p; Select v such that Modifies (p, v)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select v such that Modifies(p, v) ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Test Modifies relationship, call" ) {
        string test_string = "variable v; assign a; stmt s; read r; call c; Select v such that Modifies (c, v)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select v such that Modifies(c, v) ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Test Modify wildcard relationship" ) {
        string test_string = "variable v; assign a; stmt s; Select v such that Modifies (1, _)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select v such that Modifies(1, _) ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Test Modify contain fix variable and procedure " ) {
        string test_string = "variable kVariable; assign Select; Select kVariable such that Modifies(\"if\", kVariable)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select kVariable such that Modifies(\"if\", kVariable) ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Test Modify contain while" ) {
        string test_string = "variable kVariable; assign Select; Select kVariable such that Modifies(\"while\", kVariable)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select kVariable such that Modifies(\"while\", kVariable) ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Negative: Test Modify contain wildcard" ) {
        string test_string = "variable kVariable; assign Select; Select kVariable such that Modifies(_, kVariable)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "first argument cannot be _";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Negative: Test Modifies relationship, second parameter needs to be variable" ) {
        string test_string = "variable v; assign a; stmt s; read r; call c; Select v such that Modifies (c, r)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "unsupported parameter type, invalid modify parameter type";
        REQUIRE(expectedResult == result.ToString());
    }
}

TEST_CASE("Relation - Uses") {
    SECTION( "Test Uses relationship, statement" ) {
        string test_string = "variable v; assign a; stmt s; Select v such that Uses (1, v)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select v such that Uses(1, v) ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Test Uses relationship, variable" ) {
        string test_string = "variable v; assign a; stmt s; Select v such that Uses (a, v)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select v such that Uses(a, v) ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Test Uses relationship, statement1" ) {
        string test_string = "variable v; assign a; stmt s; Select v such that Uses (s, v)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select v such that Uses(s, v) ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Test Uses relationship, print" ) {
        string test_string = "variable v; assign a; stmt s; print r; Select v such that Uses (r, v)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select v such that Uses(r, v) ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Test Uses relationship, procedure" ) {
        string test_string = "variable v; assign a; stmt s; read r; procedure p; Select v such that Uses (p, v)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select v such that Uses(p, v) ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Test Uses relationship, call" ) {
        string test_string = "variable v; assign a; stmt s; read r; call c; Select v such that Uses (c, v)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select v such that Uses(c, v) ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Test Uses wildcard relationship" ) {
        string test_string = "variable v; assign a; stmt s; Select v such that Uses (1, _)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select v such that Uses(1, _) ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Test Uses contain fix variable and procedure " ) {
        string test_string = "variable kVariable; assign Select; Select kVariable such that Uses(\"if\", kVariable)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select kVariable such that Uses(\"if\", kVariable) ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Test Uses contain while" ) {
        string test_string = "variable kVariable; assign Select; Select kVariable such that Uses(\"while\", kVariable)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select kVariable such that Uses(\"while\", kVariable) ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Negative: Test Uses contain wildcard" ) {
        string test_string = "variable kVariable; assign Select; Select kVariable such that Uses(_, kVariable)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "first argument cannot be _";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Negative: Test Uses relationship, second parameter needs to be variable" ) {
        string test_string = "variable v; assign a; stmt s; read r; call c; Select v such that Uses (c, r)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "unsupported parameter type, invalid use parameter type";
        REQUIRE(expectedResult == result.ToString());
    }
}

TEST_CASE("Relation - Parent") {
    SECTION( "Test Parent relationship, statement1" ) {
        string test_string = "variable v; assign a; stmt s; Select v such that Parent (1, 2)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select v such that Parent(1, 2) ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Test Parent relationship, statement2" ) {
        string test_string = "variable v; assign a; stmt s, s1; Select v such that Parent (s1, 2)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select v such that Parent(s1, 2) ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Test Parent relationship, statement3" ) {
        string test_string = "variable v; assign a; stmt s, s1, s2; Select v such that Parent (1, s2)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select v such that Parent(1, s2) ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Test Parent relationship, statement4" ) {
        string test_string = "variable v; assign a; stmt s, s1, s2; Select v such that Parent (s1, s2)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select v such that Parent(s1, s2) ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Test Parent relationship, statement5" ) {
        string test_string = "variable v; assign a; stmt s, s1, s2; Select v such that Parent (_, _)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select v such that Parent(_, _) ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Test Parent relationship, statement5" ) {
        string test_string = "variable v; assign a; stmt s, s1, s2; Select v such that Parent (_, 1)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select v such that Parent(_, 1) ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Test Parent relationship, statement5" ) {
        string test_string = "variable v; assign a; stmt s, s1, s2; Select v such that Parent (1, _)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select v such that Parent(1, _) ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Test Parent relationship, statement6" ) {
        string test_string = "variable v; assign a; print p2; Select v such that Parent (1, p2)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select v such that Parent(1, p2) ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Test Parent relationship, statement7" ) {
        string test_string = "variable v; assign a; print p2; read r; Select v such that Parent (1, r)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select v such that Parent(1, r) ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Test Parent relationship, statement8" ) {
        string test_string = "variable v; assign a; print p2; read r; call c; Select v such that Parent (1, c)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select v such that Parent(1, c) ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Test Parent relationship, statement9" ) {
        string test_string = "variable v; assign a; print p2; read r; call c; while w; Select v such that Parent (1, w)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select v such that Parent(1, w) ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Test Parent relationship, statement10" ) {
        string test_string = "variable v; assign a; print p2; read r; call c; if ifs; Select v such that Parent (1, ifs)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select v such that Parent(1, ifs) ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Test Parent relationship, statement11" ) {
        string test_string = "variable v; assign a; stmt s, s1, s2; Select v such that Parent (1, a)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select v such that Parent(1, a) ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Negative: Test Parent relationship, statement1" ) {
        string test_string = "variable v; assign a; stmt s, s1, s2; Select v such that Parent (1, v)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "unsupported parameter type, follow and parent and next parameter type not supported";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Negative: Test Parent relationship, statement3" ) {
        string test_string = "variable v; assign a; print s2; procedure p; Select v such that Parent (1, p)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "unsupported parameter type, follow and parent and next parameter type not supported";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Negative: Test Parent relationship, statement4" ) {
        string test_string = "variable v; assign a; constant cc; stmt s, s1, s2; Select v such that Parent (1, cc)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "unsupported parameter type, follow and parent and next parameter type not supported";
        REQUIRE(expectedResult == result.ToString());
    }
}

TEST_CASE("Relation - Follows") {
    SECTION( "Test Follows relationship, statement1" ) {
        string test_string = "variable v; assign a; stmt s; Select v such that Follows (1, 2)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select v such that Follows(1, 2) ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Test Follows relationship, statement2" ) {
        string test_string = "variable v; assign a; stmt s, s1; Select v such that Follows (s1, 2)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select v such that Follows(s1, 2) ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Test Follows relationship, statement3" ) {
        string test_string = "variable v; assign a; stmt s, s1, s2; Select v such that Follows (1, s2)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select v such that Follows(1, s2) ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Test Follows relationship, statement4" ) {
        string test_string = "variable v; assign a; stmt s, s1, s2; Select v such that Follows (s1, s2)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select v such that Follows(s1, s2) ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Test Follows relationship, statement5" ) {
        string test_string = "variable v; assign a; stmt s, s1, s2; Select v such that Follows (_, _)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select v such that Follows(_, _) ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Test Follows relationship, statement5" ) {
        string test_string = "variable v; assign a; stmt s, s1, s2; Select v such that Follows (_, 1)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select v such that Follows(_, 1) ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Test Follows relationship, statement5" ) {
        string test_string = "variable v; assign a; stmt s, s1, s2; Select v such that Follows (1, _)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select v such that Follows(1, _) ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Test Follows relationship, statement6" ) {
        string test_string = "variable v; assign a; print p2; Select v such that Follows (1, p2)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select v such that Follows(1, p2) ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Test Follows relationship, statement7" ) {
        string test_string = "variable v; assign a; print p2; read r; Select v such that Follows (1, r)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select v such that Follows(1, r) ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Test Follows relationship, statement8" ) {
        string test_string = "variable v; assign a; print p2; read r; call c; Select v such that Follows (1, c)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select v such that Follows(1, c) ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Test Follows relationship, statement9" ) {
        string test_string = "variable v; assign a; print p2; read r; call c; while w; Select v such that Follows (1, w)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select v such that Follows(1, w) ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Test Follows relationship, statement10" ) {
        string test_string = "variable v; assign a; print p2; read r; call c; if ifs; Select v such that Follows (1, ifs)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select v such that Follows(1, ifs) ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Test Follows relationship, statement11" ) {
        string test_string = "variable v; assign a; stmt s, s1, s2; Select v such that Follows (1, a)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select v such that Follows(1, a) ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Negative: Test Follows relationship, statement1" ) {
        string test_string = "variable v; assign a; stmt s, s1, s2; Select v such that Follows (1, v)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "unsupported parameter type, follow and parent and next parameter type not supported";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Negative: Test Follows relationship, statement3" ) {
        string test_string = "variable v; assign a; print s2; procedure p; Select v such that Follows (1, p)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "unsupported parameter type, follow and parent and next parameter type not supported";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Negative: Test Follows relationship, statement4" ) {
        string test_string = "variable v; assign a; constant cc; stmt s, s1, s2; Select v such that Follows (1, cc)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "unsupported parameter type, follow and parent and next parameter type not supported";
        REQUIRE(expectedResult == result.ToString());
    }
}

TEST_CASE("Table: Test Query - contains select only") {
    SECTION( "Test Select Only" ) {
        string test_string = "variable v; assign a; stmt s; Select v";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        qp1.GetLex()->PrintToken();
        Query result = qp1.query();
        string expectedResult = "Select v ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Test Modifies relationship" ) {
        string test_string = "variable v; assign a; stmt s; Select v such that Modifies (1, v)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select v such that Modifies(1, v) ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Test Uses relationship" ) {
        string test_string = "variable v; assign a; call c; Select v such that Uses (1, v)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select v such that Uses(1, v) ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Test Parent relationship" ) {
        string test_string = "variable v; assign a; Select v such that Parent (1, 7)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select v such that Parent(1, 7) ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Test Follow relationship" ) {
        string test_string = "variable v, k; assign a; Select v such that Follows (1, 8)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();

        string expectedResult = "Select v such that Follows(1, 8) ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Test Modify wildcard relationship" ) {
        string test_string = "variable v; assign a; stmt s; Select v such that Modifies (1, _)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select v such that Modifies(1, _) ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Relation: use X " ) {
        string test_string = "variable v; assign a; call c; Select v such that Uses (1, \"x\")";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult ="Select v such that Uses(1, \"x\") ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Relation: use statement and x " ) {
        string test_string = "variable v; assign a; call c; stmt s; Select v such that Uses (s, \"x\")";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult ="Select v such that Uses(s, \"x\") ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Test Select Procedure" ) {
        string test_string = "variable v; assign a; stmt s; procedure p; Select p";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        qp1.GetLex()->PrintToken();
        Query result = qp1.query();
        string expectedResult = "Select p ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "No condition: werid select " ) {
        string test_string = "variable v; assign Select; Select Select ";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select Select ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "No condition: werid variable " ) {
        string test_string = "variable variable; assign Select; Select variable ";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select variable ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "No condition: same as entity type name" ) {
        string test_string = "variable kVariable; assign Select; Select kVariable";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select kVariable ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Relation: use contain fix variable and procedure " ) {
        string test_string = "variable kVariable; assign Select; Select kVariable such that Uses(\"while\", kVariable)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select kVariable such that Uses(\"while\", kVariable) ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Relation: modifes contain fix variable and procedure " ) {
        string test_string = "variable kVariable; assign Select; Select kVariable such that Modifies(\"if\", kVariable)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select kVariable such that Modifies(\"if\", kVariable) ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Relation: parent both _" ) {
        string test_string = "variable v, k; assign a; Select v such that Parent (_, _)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select v such that Parent(_, _) ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Relation: follows both _" ) {
        string test_string = "variable v, k; assign a; Select v such that Follows (_, _)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select v such that Follows(_, _) ";
        REQUIRE(expectedResult == result.ToString());
    }
}

TEST_CASE("Test Relationship: Call") {
    SECTION( "Relation: calls both synonym" ) {
        string test_string = "variable v, k; procedure p, q; assign a; Select p such that Calls(p,q)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select p such that Calls(p, q) ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Relation: calls string + synonym" ) {
        string test_string = "variable v, k; procedure p, q; assign a; Select p such that Calls(\"Third\",q)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select p such that Calls(\"Third\", q) ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Relation: calls both string" ) {
        string test_string = "variable v, k; procedure p, q; assign a; Select p such that Calls(\"Third\",\"Second\")";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select p such that Calls(\"Third\", \"Second\") ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Relation: calls multiple return value, both string" ) {
        string test_string = "variable v, k; procedure p, q; assign a; Select <p,q> such that Calls(\"Third\",\"Second\")";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select < p q > such that Calls(\"Third\", \"Second\") ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Relation: calls multiple return value, string + synonym" ) {
        string test_string = "variable v, k; procedure p, q; assign a; Select <p,q> such that Calls(\"Third\",q)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select < p q > such that Calls(\"Third\", q) ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Relation: calls multiple return value, both string" ) {
        string test_string = "variable v, k; procedure p, q; assign a; Select <p,q> such that Calls(\"Third\",\"Second\")";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select < p q > such that Calls(\"Third\", \"Second\") ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Relation: calls multiple return value + multiple calls + both string + both synonym" ) {
        string test_string = "variable v, k; procedure p, q; assign a; Select <p,q> such that Calls(\"Third\",\"Second\") and Calls(p, q)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select < p q > such that Calls(\"Third\", \"Second\") Calls(p, q) ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Relation: calls multiple return value + multiple calls + string + synonym" ) {
        string test_string = "variable v, k; procedure p, q; assign a; Select <p,q> such that Calls(\"Third\", q) and Calls(p, \"Second\")";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select < p q > such that Calls(\"Third\", q) Calls(p, \"Second\") ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Relation: calls multiple return value + multiple calls two such that + string + synonym" ) {
        string test_string = "variable v, k; procedure p, q; assign a; Select <p,q> such that Calls(\"Third\", q) such that Calls(p, \"Second\")";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select < p q > such that Calls(\"Third\", q) Calls(p, \"Second\") ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Relation: calls single return value + multiple calls and + wildcard + string + synonym" ) {
        string test_string = "variable v, k; procedure p, q; assign a; Select <p, q> such that Calls(_, _) and Calls(p, \"Second\")";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select < p q > such that Calls(_, _) Calls(p, \"Second\") ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Relation: calls single return value + multiple calls and + wildcard + synonym + string + synonym" ) {
        string test_string = "variable v, k; procedure p, q; assign a; Select <p, q> such that Calls(_, p) and Calls(p, \"Second\")";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select < p q > such that Calls(_, p) Calls(p, \"Second\") ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Relation: calls single return value + multiple calls and + syn + wildcard + string + synonym" ) {
        string test_string = "variable v, k; procedure p, q; assign a; Select <p, q> such that Calls(p, _) and Calls(p, \"Second\")";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select < p q > such that Calls(p, _) Calls(p, \"Second\") ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Relation: calls single return value + multiple calls and + syn + wildcard + synonym + wildcard" ) {
        string test_string = "variable v, k; procedure p, q; assign a; Select <p, q> such that Calls(p, _) and Calls(p, _)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select < p q > such that Calls(p, _) Calls(p, _) ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Relation: calls single return value + multiple calls and + syn + wildcard + wildcard + string" ) {
        string test_string = "variable v, k; procedure p, q; assign a; Select <p, q> such that Calls(p, _) and Calls(_, \"Second\")";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select < p q > such that Calls(p, _) Calls(_, \"Second\") ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Negative -> Relation: call invalid use of and" ) {
        string test_string = "variable v, k; procedure p, q; assign a; Select <p,q> such that Calls(\"Third\", q) Calls(p, \"Second\")";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "invalid terminator";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Negative -> Relation: call both parameter are wrong" ) {
        string test_string = "variable v, k; procedure p, q; assign a; Select <p,q> such that Calls(v, k)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "unsupported parameter type, call's parameter should be both procedure";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Negative -> Relation: call one parameter is variable" ) {
        string test_string = "variable v, k; procedure p, q; assign a; Select <p,q> such that Calls(v, p)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "unsupported parameter type, call's parameter should be both procedure";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Negative -> Relation: call one parameter is assign" ) {
        string test_string = "variable v, k; procedure p, q; assign a; Select <p,q> such that Calls(a, p)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "unsupported parameter type, call's parameter should be both procedure";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Negative -> Relation: call one parameter is read" ) {
        string test_string = "variable v, k; procedure p, q; assign a; read re; Select <p,q> such that Calls(p, re)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "unsupported parameter type, call's parameter should be both procedure";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Negative -> Relation: call one parameter is if" ) {
        string test_string = "variable v, k; procedure p, q; assign a; read re; if ifs; Select <p,q> such that Calls(p, ifs)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "unsupported parameter type, call's parameter should be both procedure";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Negative -> Relation: sememtically incorrect call - cyclic call" ) {
        string test_string = "variable v, k; procedure p, q; assign a; read re; if ifs; Select <p,q> such that Calls(p, p)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "cyclic calls in procedure";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Negative -> Relation: sememtically incorrect call- cyclic call_1" ) {
        string test_string = "variable v, k; procedure p, q; assign a; read re; if ifs; Select <p,q> such that Calls(\"first\", \"first\")";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "cyclic calls in procedure";
        REQUIRE(expectedResult == result.ToString());
    }
}

TEST_CASE("Test Relationship: Next") {

    SECTION( "Relation: next both synonym" ) {
        string test_string = "variable v, k; procedure p, q; stmt s, s1, s2; assign a; Select s such that Next(s,s1)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select s such that Next(s, s1) ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Relation: next both number" ) {
        string test_string = "variable v, k; procedure p, q; assign a;stmt s, s1, s2; Select s2 such that Next(1,2)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select s2 such that Next(1, 2) ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Relation: next multiple return value, both integer" ) {
        string test_string = "variable v, k; procedure p, q; assign a;stmt s, s1, s2; Select <s,s1> such that Next(1,2)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select < s s1 > such that Next(1, 2) ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Relation: next multiple return value, integer + synonym" ) {
        string test_string = "variable v, k; procedure p, q; assign a;stmt s, s1, s2; Select <s2,s1> such that Next(s2,3)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select < s2 s1 > such that Next(s2, 3) ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Relation: next multiple return value, both integer" ) {
        string test_string = "variable v, k; procedure p, q; assign a; stmt s, s1, s2; Select <s1,s> such that Next(7,3)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select < s1 s > such that Next(7, 3) ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Relation: next multiple return value + multiple next + both integer + both synonym" ) {
        string test_string = "variable v, k; procedure p, q; assign a;stmt s, s1, s2; Select <s2,s> such that Next(12,9) and Next(s2,s1)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select < s2 s > such that Next(12, 9) Next(s2, s1) ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Relation: next multiple return value + multiple next + integer + synonym" ) {
        string test_string = "variable v, k; procedure p, q; assign a; stmt s, s1, s2; Select <s2,s1> such that Next(s1,9) and Next(18,s2)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select < s2 s1 > such that Next(s1, 9) Next(18, s2) ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Relation: next multiple return value + multiple next two such that + int + synonym" ) {
        string test_string = "variable v, k; procedure p, q; assign a; stmt s, s1, s2; Select <s2,s1> such that Next(s1, 9) such that Next(18, s2)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select < s2 s1 > such that Next(s1, 9) Next(18, s2) ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Relation: next single return value + multiple next and + wildcard + int + synonym" ) {
        string test_string = "variable v, k; procedure p, q; assign a; stmt s, s1, s2; Select <s2, s1> such that Next(_, _) and Next(s1, 13)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select < s2 s1 > such that Next(_, _) Next(s1, 13) ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Relation: next single return value + multiple next and + wildcard + syn + int + synonym" ) {
        string test_string = "variable v, k; procedure p, q; assign a; stmt s, s1, s2; Select <s2, s1> such that Next(_, s2) and Next(s1, 13)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select < s2 s1 > such that Next(_, s2) Next(s1, 13) ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Relation: next single return value + multiple next and + syn +  wildcard + int + synonym" ) {
        string test_string = "variable v, k; procedure p, q; assign a; stmt s, s1, s2; Select <s2, s1> such that Next(s1, _) and Next(s1, 13)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select < s2 s1 > such that Next(s1, _) Next(s1, 13) ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Relation: next single return value + multiple next and + syn +  wildcard + synonym + wildcard" ) {
        string test_string = "variable v, k; procedure p, q; assign a; stmt s, s1, s2; Select <s2, s1> such that Next(s1, _) and Next(s1, _)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select < s2 s1 > such that Next(s1, _) Next(s1, _) ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Relation: next single return value + multiple next and + syn +  wildcard + int + synonym + wildcard" ) {
        string test_string = "variable v, k; procedure p, q; assign a; stmt s, s1, s2; Select <s2, s1> such that Next(_, 13) and Next(s, _)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select < s2 s1 > such that Next(_, 13) Next(s, _) ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Relation: next single return value + multiple next and + assign +  wildcard + int + read + wildcard" ) {
        string test_string = "variable v, k; procedure p, q; assign a; stmt s, s1, s2; if ifs; read re; print pn; call c1; while w; Select <s2, s1> such that Next(_, a) and Next(re, _)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select < s2 s1 > such that Next(_, a) Next(re, _) ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Relation: next single return value + multiple next and + print +  wildcard + int + call + wildcard" ) {
        string test_string = "variable v, k; procedure p, q; assign a; stmt s, s1, s2; if ifs; read re; print pn; call c1; while w; Select <s2, s1> such that Next(pn, a) and Next(_, c1)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select < s2 s1 > such that Next(pn, a) Next(_, c1) ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Relation: next single return value + multiple next and + if +  wildcard + int + while + wildcard" ) {
        string test_string = "variable v, k; procedure p, q; assign a; stmt s, s1, s2; if ifs; read re; print pn; call c1; while w; Select <s2, s1> such that Next(ifs, pn) and Next(w, _)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select < s2 s1 > such that Next(ifs, pn) Next(w, _) ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Negative -> Relation: next invalid use of and" ) {
        string test_string = "variable v, k; procedure p, q; assign a;stmt s, s1, s2; Select <s,s2> such that Next(_, 13) Next(s, _)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "invalid terminator";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Negative -> Relation: next both parameter are wrong" ) {
        string test_string = "variable v, k; procedure p, q; assign a; Select <p,q> such that Next(v, k)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "unsupported parameter type, follow and parent and next parameter type not supported";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Negative -> Relation: next one parameter is variable" ) {
        string test_string = "variable v, k; procedure p, q; stmt s, s1, s2; assign a; Select <p,q> such that Next(v, s1)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "unsupported parameter type, follow and parent and next parameter type not supported";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Negative -> Relation: next one parameter is procedure" ) {
        string test_string = "variable v, k; procedure p, q; stmt s, s1, s2; assign a; Select <p,q> such that Next(s2, p)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "unsupported parameter type, follow and parent and next parameter type not supported";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Negative -> Relation: next one parameter is constant" ) {
        string test_string = "variable v, k; procedure p, q; constant c; assign a;stmt s, s1, s2; Select <p,q> such that Next(c, s1)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "unsupported parameter type, follow and parent and next parameter type not supported";
        REQUIRE(expectedResult == result.ToString());
    }
}

TEST_CASE("Test Return values") {

    SECTION( "Return values: single return value: variable" ) {
        string test_string = "assign a1, a2; procedure p; call c; stmt s, BOOLEAN; variable v; constant c1; Select v.varName";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select v.varName ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Return values: single return value: read" ) {
        string test_string = "assign a1, a2; procedure p; call c; stmt s, BOOLEAN; variable v; read r; constant c1; Select r.varName";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select r.varName ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Return values: single return value: print" ) {
        string test_string = "assign a1, a2; procedure p; call c; stmt s, BOOLEAN; variable v; read r; print pn; constant c1; Select pn.varName";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select pn.varName ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Return values: single return value: constant" ) {
        string test_string = "assign a1, a2; procedure p; call c; stmt s, BOOLEAN; variable v; read r; print pn; constant c1; Select c1.value";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select c1.value ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Return values: single return value: statement" ) {
        string test_string = "assign a1, a2; procedure p; call c; stmt s, BOOLEAN; variable v; read r; print pn; constant c1; Select s.stmt#";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select s.stmt# ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Return values: single return value: read statement" ) {
        string test_string = "assign a1, a2; procedure p; call c; stmt s, BOOLEAN; variable v; read r; print pn; constant c1; Select r.stmt#";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select r.stmt# ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Return values: single return value: print statement" ) {
        string test_string = "assign a1, a2; procedure p; call c; stmt s, BOOLEAN; variable v; read r; print pn; constant c1; Select pn.stmt#";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select pn.stmt# ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Return values: single return value: call statement" ) {
        string test_string = "assign a1, a2; procedure p; call c; stmt s, BOOLEAN; variable v; read r; print pn; constant c1; Select c.stmt#";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select c.stmt# ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Return values: single return value: if statement" ) {
        string test_string = "assign a1, a2; procedure p; call c; stmt s, BOOLEAN; variable v; read r; if ifs; while w; print pn; constant c1; Select ifs.stmt#";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select ifs.stmt# ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Return values: single return value: if statement" ) {
        string test_string = "assign a1, a2; procedure p; call c; stmt s, BOOLEAN; variable v; read r; if ifs; while w; print pn; constant c1; Select w.stmt#";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select w.stmt# ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Return values: single return value: assign statement" ) {
        string test_string = "assign a1, a2; procedure p; call c; stmt s, BOOLEAN; variable v; read r; if ifs; while w; print pn; constant c1; Select a1.stmt#";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select a1.stmt# ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Return values: single return value: call" ) {
        string test_string = "assign a1, a2; procedure p; call c; stmt s, BOOLEAN; constant c1; Select c.procName";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select c.procName ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Return values: single return value: procedure" ) {
        string test_string = "assign a1, a2; procedure p; call c; stmt s, BOOLEAN; constant c1; Select p.procName";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select p.procName ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Return values: multiple return value: mix synonym and attribute 1" ) {
        string test_string = "assign a1, a2; procedure p; call c; stmt s; variable v; read r; if ifs; while w; print pn; constant c1; Select <s, c1.value, pn, w.stmt#>";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select < s c1.value pn w.stmt# > ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Return values: single return value in <>" ) {
        string test_string = "assign a1, a2; procedure p; Select <p.procName>";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select < p.procName > ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Return values: two return value in <> , same type" ) {
        string test_string = "assign a1, a2; procedure p; Select <p.procName, p.procName>";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select < p.procName p.procName > ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Return values: two return value in <> , different entity type, same attribute type" ) {
        string test_string = "assign a1, a2; procedure p; call c; stmt s; Select <p.procName, c.procName>";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select < p.procName c.procName > ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Return values: two return value in <> , different entity type, different attribute type" ) {
        string test_string = "assign a1, a2; procedure p; call c; stmt s; Select <p.procName, s.stmt#>";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select < p.procName s.stmt# > ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Return values: two return value in <> , different entity type, different attribute type 1" ) {
        string test_string = "assign a1, a2; procedure p; call c; stmt s; constant c1; Select <p.procName, c1.value>";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select < p.procName c1.value > ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Return values: multiple return value in <> , different entity type, different attribute type" ) {
        string test_string = "assign a1, a2; procedure p; call c; stmt s; constant c1; Select <p.procName, c1.value, s.stmt#>";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select < p.procName c1.value s.stmt# > ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Return values: multiple return value in <> , different entity type, different attribute type, boolean as design entity" ) {
        string test_string = "assign a1, a2; procedure p; call c; stmt s, BOOLEAN; constant c1; Select <p.procName, c1.value, s.stmt#, BOOLEAN.stmt#>";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select < p.procName c1.value s.stmt# BOOLEAN.stmt# > ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Return values: single return value: boolean" ) {
        string test_string = "assign a1, a2; procedure p; call c; stmt s; variable v; read r; if ifs; while w; print pn; constant c1; Select BOOLEAN";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select BOOLEAN ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Return values: two return value: mix synonym and attribute" ) {
        string test_string = "assign a1, a2; procedure p; call c; stmt s; variable v; read r; if ifs; while w; print pn; constant c1; Select <v, p.procName>";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select < v p.procName > ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Return values: two return value: mix synonym and attribute 1" ) {
        string test_string = "assign a1, a2; procedure p; call c; stmt s; variable v; read r; if ifs; while w; print pn; constant c1; Select <s, c1.value>";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select < s c1.value > ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Return values: multiple return value in <> , different entity type, same attribute type 3" ) {
        string test_string = "assign a1, a2; procedure p; call c; constant c1; stmt s; Select <c1, p.procName, a1, c.procName, s.stmt#>";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select < c1 p.procName a1 c.procName s.stmt# > ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Return values: multiple return value in <> , different entity type, same attribute type 2" ) {
        string test_string = "assign a1, a2; procedure p; call c; constant c1; stmt s, BOOLEAN; Select <BOOLEAN, c1.value, p.procName, a1, c.procName, s.stmt#>";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select < BOOLEAN c1.value p.procName a1 c.procName s.stmt# > ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Negative Return values: multiple boolean" ) {
        string test_string = "assign a1, a2; procedure p; call c; stmt s; variable v; read r; if ifs; while w; print pn; constant c1; Select BOOLEAN BOOLEAN";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "invalid terminator";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Negative Return values: single boolean in <>" ) {
        string test_string = "assign a1, a2; procedure p; call c; stmt s; variable v; read r; if ifs; while w; print pn; constant c1; Select <BOOLEAN>";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "cannot mix boolean type and tuple";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Negative Return values: mix synonym and  boolean in <>" ) {
        string test_string = "assign a1, a2; procedure p; call c; stmt s; variable v; read r; if ifs; while w; print pn; constant c1; Select <v, BOOLEAN>";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "cannot mix boolean type and tuple";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Return values: mix synonym and  boolean in <>" ) {
        string test_string = "assign a1, a2; procedure p; call c; stmt s; variable v, BOOLEAN; read r; if ifs; while w; print pn; constant c1; Select <v, BOOLEAN>";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select < v BOOLEAN > ";
        REQUIRE(expectedResult == result.ToString());
    }
}

TEST_CASE("Test Pattern while") {
    SECTION( "Pattern while all wildcard" ) {
        string test_string = "assign a1, a2; procedure p; call c; stmt s; if ifs; while w; Select c pattern w (_, _) ";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select c pattern w(_, _) ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Pattern while variable " ) {
        string test_string = "assign a1, a2; procedure p; call c; stmt s; variable v; if ifs;while w;  Select c pattern w (v, _) ";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select c pattern w(v, _) ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Pattern while double quote " ) {
        string test_string = "assign a1, a2; procedure p; call c; stmt s; variable v; if ifs;while w;  Select c pattern w (\"first\", _) ";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select c pattern w(\"first\", _) ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Negative: Pattern while assignment " ) {
        string test_string = "assign a1, a2; procedure p; call c; stmt s; variable v; if ifs;while w;  Select c pattern w (a1, _) ";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "pattern first argument can only be variable synonym or wildcard or double quote";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Negative: Pattern while statement " ) {
        string test_string = "assign a1, a2; procedure p; call c; stmt s; variable v; if ifs; while w;  Select c pattern w (s, _) ";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "pattern first argument can only be variable synonym or wildcard or double quote";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Negative: Pattern while call " ) {
        string test_string = "assign a1, a2; procedure p; call c; stmt s; variable v; if ifs;while w;  Select c pattern w (c, _) ";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "pattern first argument can only be variable synonym or wildcard or double quote";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Negative: Pattern while if " ) {
        string test_string = "assign a1, a2; procedure p; call c; stmt s; variable v; if ifs;while w;   Select c pattern w (ifs, _) ";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "pattern first argument can only be variable synonym or wildcard or double quote";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Negative: Pattern while while " ) {
        string test_string = "assign a1, a2; procedure p; call c; stmt s; variable v; if ifs; while w; Select c pattern ifs (w, _) ";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "if second and third argument needs to be wildcard";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Negative: Pattern while print " ) {
        string test_string = "assign a1, a2; procedure p; call c; stmt s; variable v; if ifs; while w; print pn; Select c pattern w (pn, _) ";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "pattern first argument can only be variable synonym or wildcard or double quote";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Negative: Pattern while procedure " ) {
        string test_string = "assign a1, a2; procedure p; call c; stmt s; variable v; if ifs; while w; print pn; Select c pattern w (p, _) ";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "pattern first argument can only be variable synonym or wildcard or double quote";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Negative: Pattern while read " ) {
        string test_string = "assign a1, a2; procedure p; call c; stmt s; variable v; read r; if ifs; while w; print pn; Select c pattern w (r, _) ";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "pattern first argument can only be variable synonym or wildcard or double quote";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Negative: Pattern while second argument is read " ) {
        string test_string = "assign a1, a2; procedure p; call c; stmt s; variable v; read r; if ifs; while w; print pn; Select c pattern w (v, r) ";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "unsupported parameter type, expressionpec second parameter not valid";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Negative: Pattern while second argument is assign " ) {
        string test_string = "assign a1, a2; procedure p; call c; stmt s; variable v; read r; if ifs; while w; print pn; Select c pattern w (v, a1) ";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "unsupported parameter type, expressionpec second parameter not valid";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Negative: Pattern while second argument is procedure " ) {
        string test_string = "assign a1, a2; procedure p; call c; stmt s; variable v; read r; if ifs; while w; print pn; Select c pattern w (v, p) ";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "unsupported parameter type, expressionpec second parameter not valid";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Negative: Pattern while second argument is call " ) {
        string test_string = "assign a1, a2; procedure p; call c; stmt s; variable v; read r; if ifs; while w; print pn; Select c pattern w (v, c) ";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "unsupported parameter type, expressionpec second parameter not valid";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Negative: Pattern while second argument is stmt " ) {
        string test_string = "assign a1, a2; procedure p; call c; stmt s; variable v; read r; if ifs; while w; print pn; Select c pattern w (v, s) ";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "unsupported parameter type, expressionpec second parameter not valid";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Negative: Pattern while second argument is variable " ) {
        string test_string = "assign a1, a2; procedure p; call c; stmt s; variable v; read r; if ifs; while w; print pn; Select c pattern w (v, v) ";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "unsupported parameter type, expressionpec second parameter not valid";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Negative: Pattern while second argument is read " ) {
        string test_string = "assign a1, a2; procedure p; call c; stmt s; variable v; read r; if ifs; while w; print pn; Select c pattern w (v, r) ";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "unsupported parameter type, expressionpec second parameter not valid";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Negative: Pattern while second argument is if " ) {
        string test_string = "assign a1, a2; procedure p; call c; stmt s; variable v; read r; if ifs; while w; print pn; Select c pattern w (v, ifs) ";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "unsupported parameter type, expressionpec second parameter not valid";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Negative: Pattern while second argument is while " ) {
        string test_string = "assign a1, a2; procedure p; call c; stmt s; variable v; read r; if ifs; while w; print pn; Select c pattern w (v, w) ";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "unsupported parameter type, expressionpec second parameter not valid";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Negative: Pattern while second argument is print " ) {
        string test_string = "assign a1, a2; procedure p; call c; stmt s; variable v; read r; if ifs; while w; print pn; Select c pattern w (v, pn) ";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "unsupported parameter type, expressionpec second parameter not valid";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Negative: Pattern while first argument is empty string " ) {
        string test_string = "assign a, a1, a2; while w; if ifs; Select ifs pattern w(\"\" , _)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "it cannot be an empty string";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Negative: Pattern while first argument is empty space " ) {
        string test_string = "assign a, a1, a2; while w; if ifs; Select ifs pattern w(\" \" , _)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "it cannot be an empty string";
        REQUIRE(expectedResult == result.ToString());
    }


    SECTION( "Negative: Pattern while has three parameter " ) {
        string test_string = "assign a1, a2; procedure p; call c; stmt s; variable v; read r; if ifs; while w; print pn; Select c pattern w (v, _, _) ";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "";
        REQUIRE(expectedResult == result.ToString());
    }
}

TEST_CASE("Test Pattern if") {
    SECTION( "Pattern if all wildcard" ) {
        string test_string = "assign a1, a2; procedure p; call c; stmt s; if ifs;  Select c pattern ifs (_, _, _ ) ";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select c pattern ifs(_, _) ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Pattern if variable " ) {
        string test_string = "assign a1, a2; procedure p; call c; stmt s; variable v; if ifs;  Select c pattern ifs (v, _, _ ) ";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select c pattern ifs(v, _) ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Pattern if double quote " ) {
        string test_string = "assign a1, a2; procedure p; call c; stmt s; variable v; if ifs;  Select c pattern ifs (\"first\", _, _ ) ";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select c pattern ifs(\"first\", _) ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Negative: Pattern if assignment " ) {
        string test_string = "assign a1, a2; procedure p; call c; stmt s; variable v; if ifs;  Select c pattern ifs (a1, _, _ ) ";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "pattern first argument can only be variable synonym or wildcard or double quote";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Negative: Pattern if statement " ) {
        string test_string = "assign a1, a2; procedure p; call c; stmt s; variable v; if ifs;  Select c pattern ifs (s, _, _ ) ";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "pattern first argument can only be variable synonym or wildcard or double quote";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Negative: Pattern if call " ) {
        string test_string = "assign a1, a2; procedure p; call c; stmt s; variable v; if ifs;  Select c pattern ifs (c, _, _ ) ";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "pattern first argument can only be variable synonym or wildcard or double quote";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Negative: Pattern if if " ) {
        string test_string = "assign a1, a2; procedure p; call c; stmt s; variable v; if ifs;   Select c pattern ifs (ifs, _, _ ) ";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "pattern first argument can only be variable synonym or wildcard or double quote";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Negative: Pattern if while " ) {
        string test_string = "assign a1, a2; procedure p; call c; stmt s; variable v; if ifs; while w; Select c pattern ifs (w, _, _ ) ";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "pattern first argument can only be variable synonym or wildcard or double quote";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Negative: Pattern if print " ) {
        string test_string = "assign a1, a2; procedure p; call c; stmt s; variable v; if ifs; while w; print pn; Select c pattern ifs (pn, _, _ ) ";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "pattern first argument can only be variable synonym or wildcard or double quote";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Negative: Pattern if procedure " ) {
        string test_string = "assign a1, a2; procedure p; call c; stmt s; variable v; if ifs; while w; print pn; Select c pattern ifs (p, _, _ ) ";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "pattern first argument can only be variable synonym or wildcard or double quote";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Negative: Pattern if read " ) {
        string test_string = "assign a1, a2; procedure p; call c; stmt s; variable v; read r; if ifs; while w; print pn; Select c pattern ifs (r, _, _ ) ";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "pattern first argument can only be variable synonym or wildcard or double quote";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Negative: Pattern if second argument is read " ) {
        string test_string = "assign a1, a2; procedure p; call c; stmt s; variable v; read r; if ifs; while w; print pn; Select c pattern ifs (v, r, _ ) ";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "unsupported parameter type, expressionpec second parameter not valid";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Negative: Pattern if second argument is assign " ) {
        string test_string = "assign a1, a2; procedure p; call c; stmt s; variable v; read r; if ifs; while w; print pn; Select c pattern ifs (v, a1, _ ) ";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "unsupported parameter type, expressionpec second parameter not valid";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Negative: Pattern if second argument is procedure " ) {
        string test_string = "assign a1, a2; procedure p; call c; stmt s; variable v; read r; if ifs; while w; print pn; Select c pattern ifs (v, p, _ ) ";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "unsupported parameter type, expressionpec second parameter not valid";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Negative: Pattern if second argument is call " ) {
        string test_string = "assign a1, a2; procedure p; call c; stmt s; variable v; read r; if ifs; while w; print pn; Select c pattern ifs (v, c, _ ) ";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "unsupported parameter type, expressionpec second parameter not valid";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Negative: Pattern if second argument is stmt " ) {
        string test_string = "assign a1, a2; procedure p; call c; stmt s; variable v; read r; if ifs; while w; print pn; Select c pattern ifs (v, s, _ ) ";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "unsupported parameter type, expressionpec second parameter not valid";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Negative: Pattern if second argument is variable " ) {
        string test_string = "assign a1, a2; procedure p; call c; stmt s; variable v; read r; if ifs; while w; print pn; Select c pattern ifs (v, v, _ ) ";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "unsupported parameter type, expressionpec second parameter not valid";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Negative: Pattern if second argument is read " ) {
        string test_string = "assign a1, a2; procedure p; call c; stmt s; variable v; read r; if ifs; while w; print pn; Select c pattern ifs (v, r, _ ) ";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "unsupported parameter type, expressionpec second parameter not valid";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Negative: Pattern if second argument is if " ) {
        string test_string = "assign a1, a2; procedure p; call c; stmt s; variable v; read r; if ifs; while w; print pn; Select c pattern ifs (v, ifs, _ ) ";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "unsupported parameter type, expressionpec second parameter not valid";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Negative: Pattern if second argument is while " ) {
        string test_string = "assign a1, a2; procedure p; call c; stmt s; variable v; read r; if ifs; while w; print pn; Select c pattern ifs (v, w, _ ) ";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "unsupported parameter type, expressionpec second parameter not valid";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Negative: Pattern if second argument is print " ) {
        string test_string = "assign a1, a2; procedure p; call c; stmt s; variable v; read r; if ifs; while w; print pn; Select c pattern ifs (v, pn, _ ) ";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "unsupported parameter type, expressionpec second parameter not valid";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Negative: Pattern if has only two parameters " ) {
        string test_string = "assign a1, a2; procedure p; call c; stmt s; variable v; read r; if ifs; while w; print pn; Select c pattern ifs (v, _) ";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "if second and third argument needs to be wildcard";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Negative: Pattern if has only two parameters 1" ) {
        string test_string = "assign a1, a2; procedure p; call c; stmt s; variable v; read r; if ifs; while w; print pn; Select c pattern ifs (v, _, ) ";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "unsupported parameter type, relation first parameter not recognized";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Negative: Pattern if third argument is read " ) {
        string test_string = "assign a1, a2; procedure p; call c; stmt s; variable v; read r; if ifs; while w; print pn; Select c pattern ifs (v, _ , r) ";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "pattern first argument can only be variable synonym or wildcard or double quote";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Negative: Pattern if third argument is assign " ) {
        string test_string = "assign a1, a2; procedure p; call c; stmt s; variable v; read r; if ifs; while w; print pn; Select c pattern ifs (v, _, a1) ";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "pattern first argument can only be variable synonym or wildcard or double quote";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Negative: Pattern if third argument is procedure " ) {
        string test_string = "assign a1, a2; procedure p; call c; stmt s; variable v; read r; if ifs; while w; print pn; Select c pattern ifs (v, _, p) ";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "pattern first argument can only be variable synonym or wildcard or double quote";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Negative: Pattern if third argument is call " ) {
        string test_string = "assign a1, a2; procedure p; call c; stmt s; variable v; read r; if ifs; while w; print pn; Select c pattern ifs (v, _, c) ";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "pattern first argument can only be variable synonym or wildcard or double quote";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Negative: Pattern if third argument is stmt " ) {
        string test_string = "assign a1, a2; procedure p; call c; stmt s; variable v; read r; if ifs; while w; print pn; Select c pattern ifs (v, _, s) ";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "pattern first argument can only be variable synonym or wildcard or double quote";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Negative: Pattern if third argument is variable " ) {
        string test_string = "assign a1, a2; procedure p; call c; stmt s; variable v; read r; if ifs; while w; print pn; Select c pattern ifs (v, _, v) ";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "if third argument needs to be wildcard and second argument need to be wildcard";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Negative: Pattern if third argument is read " ) {
        string test_string = "assign a1, a2; procedure p; call c; stmt s; variable v; read r; if ifs; while w; print pn; Select c pattern ifs (v, _, r) ";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "pattern first argument can only be variable synonym or wildcard or double quote";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Negative: Pattern if third argument is if " ) {
        string test_string = "assign a1, a2; procedure p; call c; stmt s; variable v; read r; if ifs; while w; print pn; Select c pattern ifs (v, _, ifs) ";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "pattern first argument can only be variable synonym or wildcard or double quote";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Negative: Pattern if third argument is while " ) {
        string test_string = "assign a1, a2; procedure p; call c; stmt s; variable v; read r; if ifs; while w; print pn; Select c pattern ifs (v, _, w) ";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "pattern first argument can only be variable synonym or wildcard or double quote";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Negative: Pattern if third argument is print " ) {
        string test_string = "assign a1, a2; procedure p; call c; stmt s; variable v; read r; if ifs; while w; print pn; Select c pattern ifs (v, _, pn) ";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "pattern first argument can only be variable synonym or wildcard or double quote";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Negative: Pattern if first argument is empty string " ) {
        string test_string = "assign a, a1, a2; while w; if ifs; Select ifs pattern ifs(\"\" , _ , _)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "it cannot be an empty string";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Negative: Pattern if first argument is empty space " ) {
        string test_string = "assign a, a1, a2; while w; if ifs; Select ifs pattern ifs(\" \" , _ , _)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "it cannot be an empty string";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Negative: Pattern if has more than 3 parameter " ) {
        string test_string = "assign a1, a2; procedure p; call c; stmt s; variable v; read r; if ifs; while w; print pn; Select c pattern w (v, _, _, _) ";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "missing bracket";
        REQUIRE(expectedResult == result.ToString());
    }
}

TEST_CASE("Weird TEST that somehow it cannot run in other place: Mainly next and call") {
    SECTION( "Relation: calls single return value + multiple calls and + string + synonym" ) {
        string test_string = "variable v, k; procedure p, q; assign a; Select p such that Calls(\"Third\", q) and Calls(p, \"Second\")";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select p such that Calls(\"Third\", q) Calls(p, \"Second\") ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Relation: calls single return value + multiple calls two such that + string + synonym" ) {
        string test_string = "variable v, k; procedure p, q; assign a; Select p such that Calls(\"Third\", q) such that Calls(p, \"Second\")";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select p such that Calls(\"Third\", q) Calls(p, \"Second\") ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Relation: next single return value + multiple next two such that + int + synonym" ) {
        string test_string = "variable v, k; procedure p, q; assign a; stmt s, s1, s2; Select s2 such that Next(18, s2) such that Next(28, s)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select s2 such that Next(18, s2) Next(28, s) ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Relation: calls single return value + multiple calls and + wildcard + string + synonym" ) {
        string test_string = "variable v, k; procedure p, q; assign a; Select p such that Calls(_, _) and Calls(p, \"Second\")";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select p such that Calls(_, _) Calls(p, \"Second\") ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Relation: next single return value + multiple next and + int + synonym" ) {
        string test_string = "variable v, k; procedure p, q; assign a;  stmt s, s1, s2; Select s1 such that Next(31, s2) and Next(s1, 13)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select s1 such that Next(31, s2) Next(s1, 13) ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Relation: next single return value + multiple next and + wildcard + int + synonym" ) {
        string test_string = "variable v, k; procedure p, q; assign a; stmt s, s1, s2; Select s2 such that Next(_, _) and Next(s1, 13)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select s2 such that Next(_, _) Next(s1, 13) ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Relation: next integer + synonym" ) {
        string test_string = "variable v, k; procedure p, q; assign a;stmt s, s1, s2; Select s1 such that Next(1,s1)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select s1 such that Next(1, s1) ";
        REQUIRE(expectedResult == result.ToString());
    }
}

TEST_CASE("Test Affects") {

    SECTION( "Affects has two assignment" ) {
        string test_string = "procedure p; call c1; constant c; stmt s; assign a1, a2; Select p such that Affects (a1, a2)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select p such that Affects(a1, a2) ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Affects has two number assignment" ) {
        string test_string = "procedure p; call c1; constant c; stmt s; assign a1, a2; Select p such that Affects (1, 2)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select p such that Affects(1, 2) ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Affects has one number assignment + one synonym" ) {
        string test_string = "procedure p; call c1; constant c; stmt s; assign a1, a2; Select p such that Affects (1, a1)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select p such that Affects(1, a1) ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Affects has one number assignment + one synonym 1" ) {
        string test_string = "procedure p; call c1; constant c; stmt s; assign a1, a2; Select p such that Affects (a2, 10)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select p such that Affects(a2, 10) ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Affects has has left wild card + number" ) {
        string test_string = "procedure p; call c1; constant c; stmt s; assign a1, a2; Select p such that Affects (_, 10)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select p such that Affects(_, 10) ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Affects has has right wild card + number" ) {
        string test_string = "procedure p; call c1; constant c; stmt s; assign a1, a2; Select p such that Affects (72, _)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select p such that Affects(72, _) ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Affects has has left wild card + assign" ) {
        string test_string = "procedure p; call c1; constant c; stmt s; assign a1, a2; Select p such that Affects (a1, 10)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select p such that Affects(a1, 10) ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Affects has has right wild card + number" ) {
        string test_string = "procedure p; call c1; constant c; stmt s; assign a1, a2; Select p such that Affects (72, a2)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select p such that Affects(72, a2) ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Affects has has both wild card" ) {
        string test_string = "procedure p; call c1; constant c; stmt s; assign a1, a2; Select p such that Affects (_, _)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select p such that Affects(_, _) ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Affects has has assignment + statement" ) {
        string test_string = "procedure p; call c1; constant c; stmt s; assign a1, a2; Select p such that Affects (a1, s)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select p such that Affects(a1, s) ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Affects has has statement + assignment" ) {
        string test_string = "procedure p; call c1; constant c; stmt s; assign a1, a2; Select p such that Affects (s, a2)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select p such that Affects(s, a2) ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Affects has has number + statement" ) {
        string test_string = "procedure p; call c1; constant c; stmt s; assign a1, a2; Select p such that Affects (20, s)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select p such that Affects(20, s) ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Affects has has statement + number" ) {
        string test_string = "procedure p; call c1; constant c; stmt s; assign a1, a2; Select p such that Affects (s, 15)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select p such that Affects(s, 15) ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Affects has has number + statement" ) {
        string test_string = "procedure p; call c1; constant c; stmt s; assign a1, a2; Select p such that Affects (20, s)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select p such that Affects(20, s) ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Affects has has statement + wildcard" ) {
        string test_string = "procedure p; call c1; constant c; stmt s; assign a1, a2; Select p such that Affects (s, _)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select p such that Affects(s, _) ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Affects has has wildcard + statement" ) {
        string test_string = "procedure p; call c1; constant c; stmt s; assign a1, a2; Select p such that Affects (_, s)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select p such that Affects(_, s) ";
        REQUIRE(expectedResult == result.ToString());
    }
}

TEST_CASE("Test Affects *") {

    SECTION( "Affects* has two number assignment" ) {
        string test_string = "procedure p; call c1; constant c; stmt s; assign a1, a2; Select p such that Affects* (1, 2)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select p such that Affects*(1, 2) ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Affects* has one number assignment + one synonym" ) {
        string test_string = "procedure p; call c1; constant c; stmt s; assign a1, a2; Select p such that Affects* (1, a1)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select p such that Affects*(1, a1) ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Affects* has one number assignment + one synonym 1" ) {
        string test_string = "procedure p; call c1; constant c; stmt s; assign a1, a2; Select p such that Affects* (a2, 23)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select p such that Affects*(a2, 23) ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Affects* has has left wild card + number" ) {
        string test_string = "procedure p; call c1; constant c; stmt s; assign a1, a2; Select p such that Affects* (_, 10)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select p such that Affects*(_, 10) ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Affects* has has right wild card + number" ) {
        string test_string = "procedure p; call c1; constant c; stmt s; assign a1, a2; Select p such that Affects* (72, _)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select p such that Affects*(72, _) ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Affects has has left wild card + assign" ) {
        string test_string = "procedure p; call c1; constant c; stmt s; assign a1, a2; Select p such that Affects (a1, 10)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select p such that Affects(a1, 10) ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Affects* has has right wild card + number" ) {
        string test_string = "procedure p; call c1; constant c; stmt s; assign a1, a2; Select p such that Affects* (72, a2)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select p such that Affects*(72, a2) ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Affects* has has both wild card" ) {
        string test_string = "procedure p; call c1; constant c; stmt s; assign a1, a2; Select p such that Affects* (_, _)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select p such that Affects*(_, _) ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Affects* has has statement + assignment" ) {
        string test_string = "procedure p; call c1; constant c; stmt s; assign a1, a2; Select p such that Affects* (s, a2)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select p such that Affects*(s, a2) ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Affects* has has number + statement" ) {
        string test_string = "procedure p; call c1; constant c; stmt s; assign a1, a2; Select p such that Affects* (20, s)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select p such that Affects*(20, s) ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Affects* has has statement + number" ) {
        string test_string = "procedure p; call c1; constant c; stmt s; assign a1, a2; Select p such that Affects* (s, 15)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select p such that Affects*(s, 15) ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Affects* has has number + statement" ) {
        string test_string = "procedure p; call c1; constant c; stmt s; assign a1, a2; Select p such that Affects* (20, s)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select p such that Affects*(20, s) ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Affects* has has statement + wildcard" ) {
        string test_string = "procedure p; call c1; constant c; stmt s; assign a1, a2; Select p such that Affects* (s, _)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select p such that Affects*(s, _) ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Affects* has has wildcard + statement" ) {
        string test_string = "procedure p; call c1; constant c; stmt s; assign a1, a2; Select p such that Affects* (_, s)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select p such that Affects*(_, s) ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Affects* has two assignment" ) {
        string test_string = "procedure p; call c1; constant c; stmt s; assign a1, a2; Select p such that Affects* (a1, a2)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select p such that Affects*(a1, a2) ";
        REQUIRE(expectedResult == result.ToString());
    }

}

TEST_CASE("Test With") {

    SECTION( "With IDENT and IDENT" ) {
        string test_string = "procedure p; Select p with \"Third\" = \"Third\"";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select p with \"Third\" = \"Third\" ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "With IDENT contains digit and IDENT contains digit" ) {
        string test_string = "procedure p; Select p with \"Third123\" = \"Third123\"";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select p with \"Third123\" = \"Third123\" ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "With Integer and Integer" ) {
        string test_string = "procedure p; Select p with 456 = 123";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select p with 456 = 123 ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "With long Integer and long Integer" ) {
        string test_string = "procedure p; Select p with 1111111113333333333333333333333333334444456 = 7377361391312442634521313124141321313112313";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select p with 1111111113333333333333333333333333334444456 = 7377361391312442634521313124141321313112313 ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "With AttrREf and AttrREf: same attrRef" ) {
        string test_string = "procedure p; Select p with p.procName = p.procName";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select p with p.procName = p.procName ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "With AttrREf and AttrREf: different attrRef same synonym" ) {
        string test_string = "procedure p, q; Select p with p.procName = q.procName";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select p with p.procName = q.procName ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "With AttrREf and AttrREf: different attrRef different synonym same type" ) {
        string test_string = "procedure p, q; call c; Select p with p.procName = c.procName";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select p with p.procName = c.procName ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "With AttrREf and AttrREf: different attrRef different synonym same type 1" ) {
        string test_string = "procedure p, q; call c; variable v; Select p with p.procName = v.varName";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select p with p.procName = v.varName ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "With AttrREf and AttrREf: different attrRef different synonym same type 2" ) {
        string test_string = "procedure p, q; call c; variable v; read r; Select p with p.procName = r.varName";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select p with p.procName = r.varName ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "With AttrREf and AttrREf: different attrRef different synonym same type 3" ) {
        string test_string = "procedure p, q; call c; variable v; read r; print pn; Select p with p.procName = pn.varName";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select p with p.procName = pn.varName ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "With AttrREf and AttrREf: different attrRef different synonym same type Statement 1" ) {
        string test_string = "procedure p, q; call c; variable v; read r; print pn; stmt s; Select p with s.stmt# = r.stmt#";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select p with s.stmt# = r.stmt# ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "With AttrREf and AttrREf: different attrRef different synonym same type Statement 2" ) {
        string test_string = "procedure p, q; call c; variable v; read r; print pn; stmt s; Select p with s.stmt# = pn.stmt#";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select p with s.stmt# = pn.stmt# ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "With AttrREf and AttrREf: different attrRef different synonym same type Statement 3" ) {
        string test_string = "procedure p, q; call c; variable v; read r; print pn; stmt s; Select p with s.stmt# = c.stmt#";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select p with s.stmt# = c.stmt# ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "With AttrREf and AttrREf: different attrRef different synonym same type Statement 4" ) {
        string test_string = "procedure p, q; call c; variable v; read r; print pn; stmt s; while w; Select p with s.stmt# = w.stmt#";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select p with s.stmt# = w.stmt# ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "With AttrREf and AttrREf: different attrRef different synonym same type Statement 5" ) {
        string test_string = "procedure p, q; call c; variable v; read r; print pn; stmt s; while w; if ifs; Select p with s.stmt# = ifs.stmt#";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select p with s.stmt# = ifs.stmt# ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "With AttrREf and AttrREf: different attrRef different synonym same type Statement 6" ) {
        string test_string = "procedure p, q; call c; variable v; read r; print pn; stmt s; while w; if ifs; assign a; Select p with s.stmt# = a.stmt#";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select p with s.stmt# = a.stmt# ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "With AttrREf and AttrREf: different attrRef different synonym same type Statement 7" ) {
        string test_string = "procedure p, q; call c; variable v; read r; print pn; stmt s; while w; if ifs; assign a; constant c1; Select p with s.stmt# = c1.value";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select p with s.stmt# = c1.value ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "With Ident and AttrREf" ) {
        string test_string = "procedure p, q; call c; variable v; read r; print pn; stmt s; while w; if ifs; assign a; constant c1; Select p with p.procName = \"Third\"";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select p with p.procName = \"Third\" ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "With Ident and AttrREf 1" ) {
        string test_string = "procedure p, q; call c; variable v; read r; print pn; stmt s; while w; if ifs; assign a; constant c1; Select p with c.procName = \"Third\"";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select p with c.procName = \"Third\" ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "With Ident and AttrREf 2" ) {
        string test_string = "procedure p, q; call c; variable v; read r; print pn; stmt s; while w; if ifs; assign a; constant c1; Select p with v.varName = \"Third\"";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select p with v.varName = \"Third\" ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "With Ident and AttrREf 3" ) {
        string test_string = "procedure p, q; call c; variable v; read r; print pn; stmt s; while w; if ifs; assign a; constant c1; Select p with r.varName = \"Third\"";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select p with r.varName = \"Third\" ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "With Ident and AttrREf 4" ) {
        string test_string = "procedure p, q; call c; variable v; read r; print pn; stmt s; while w; if ifs; assign a; constant c1; Select p with pn.varName = \"Third\"";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select p with pn.varName = \"Third\" ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "With AttrRef and Integer 1" ) {
        string test_string = "procedure p, q; call c; variable v; read r; print pn; stmt s; while w; if ifs; assign a; constant c1; Select p with s.stmt# = 10";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select p with s.stmt# = 10 ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "With AttrRef and long Integer 1" ) {
        string test_string = "procedure p, q; call c; variable v; read r; print pn; stmt s; while w; if ifs; assign a; constant c1; Select p with s.stmt# = 1000000000000000000000000000000000000000000000000000000";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select p with s.stmt# = 1000000000000000000000000000000000000000000000000000000 ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "With AttrRef and Integer 2" ) {
        string test_string = "procedure p, q; call c; variable v; read r; print pn; stmt s; while w; if ifs; assign a; constant c1; Select p with r.stmt# = 10";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select p with r.stmt# = 10 ";
        REQUIRE(expectedResult == result.ToString());
    }


    SECTION( "With AttrRef and Integer 3" ) {
        string test_string = "procedure p, q; call c; variable v; read r; print pn; stmt s; while w; if ifs; assign a; constant c1; Select p with pn.stmt# = 10";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select p with pn.stmt# = 10 ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "With AttrRef and Integer 4" ) {
        string test_string = "procedure p, q; call c; variable v; read r; print pn; stmt s; while w; if ifs; assign a; constant c1; Select p with c.stmt# = 10";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select p with c.stmt# = 10 ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "With AttrRef and Integer 5" ) {
        string test_string = "procedure p, q; call c; variable v; read r; print pn; stmt s; while w; if ifs; assign a; constant c1; Select p with w.stmt# = 10";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select p with w.stmt# = 10 ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "With AttrRef and Integer 6" ) {
        string test_string = "procedure p, q; call c; variable v; read r; print pn; stmt s; while w; if ifs; assign a; constant c1; Select p with ifs.stmt# = 10";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select p with ifs.stmt# = 10 ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "With AttrRef and Integer 7" ) {
        string test_string = "procedure p, q; call c; variable v; read r; print pn; stmt s; while w; if ifs; assign a; constant c1; Select p with a.stmt# = 10";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select p with a.stmt# = 10 ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "With AttrRef and Integer 8" ) {
        string test_string = "procedure p, q; call c; variable v; read r; print pn; stmt s; while w; if ifs; assign a; constant c1; Select p with c1.value = 10";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select p with c1.value = 10 ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Negative With: more equals" ) {
        string test_string = "procedure p, q; call c; variable v; read r; print pn; stmt s; Select p with s.stmt# = pn.stmt# = s.stmt#";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "invalid terminator";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Negative With: different attribute type: name and integer 1" ) {
        string test_string = "procedure p, q; call c; variable v; read r; print pn; stmt s; Select p with p.procName = 10";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "two requested type in with clauses should be the same";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Negative With: different attribute type: name and integer 2" ) {
        string test_string = "procedure p, q; call c; variable v; read r; print pn; stmt s; Select p with p.procName = s.stmt#";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "two requested type in with clauses should be the same";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Negative With: different attribute type: name and integer 3" ) {
        string test_string = "procedure p, q; call c; variable v; read r; print pn; stmt s; constant c1; Select p with p.procName = c1.value";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "two requested type in with clauses should be the same";
        REQUIRE(expectedResult == result.ToString());
    }


    SECTION( "Negative With Ident and Integer" ) {
        string test_string = "procedure p, q; call c; variable v; read r; print pn; stmt s; constant c1; Select p with 10 = \"Third\"";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "two requested type in with clauses should be the same";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Negative With both empty string" ) {
        string test_string = "procedure p, q; call c; variable v; read r; print pn; stmt s; constant c1; Select p with \"\" = \"\" ";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "it cannot be an empty string";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Negative With both empty space + empty string" ) {
        string test_string = "procedure p, q; call c; variable v; read r; print pn; stmt s; constant c1; Select p with \" \" = \"\" ";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "it cannot be an empty string";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Negative With empty string + indent" ) {
        string test_string = "procedure p, q; call c; variable v; read r; print pn; stmt s; constant c1; Select p with \"\" = \"Third\" ";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "it cannot be an empty string";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Negative With empty string + integer" ) {
        string test_string = "procedure p, q; call c; variable v; read r; print pn; stmt s; constant c1; Select p with \"\" = 100 ";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "it cannot be an empty string";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Negative With empty string + AttrRef" ) {
        string test_string = "procedure p, q; call c; variable v; read r; print pn; stmt s; constant c1; Select p with \"\" = p.procName ";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "it cannot be an empty string";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Negative With wrong string + AttrRef" ) {
        string test_string = "procedure p, q; call c; variable v; read r; print pn; stmt s; constant c1; Select p with \"(\" = p.procName ";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "declaration name first character should be letter";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Negative With AttrRef + wrong string" ) {
        string test_string = "procedure p, q; call c; variable v; read r; print pn; stmt s; constant c1; Select p with p.procName = \"(\" ";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "declaration name first character should be letter";
        REQUIRE(expectedResult == result.ToString());
    }
}

TEST_CASE("Miscel Test") {
    SECTION( "Miscel Test1" ) {
        string test_string = "stmt s, s1, s2; assign a, a1; while w, w1; if ifs, ifs1; variable v, v1; procedure p, q, p1; constant c, c1; call cl; print pn; read re;\n"
                               "Select w pattern w(_,_)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select w pattern w(_, _) ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Miscel Test2" ) {
        string test_string = "stmt s, s1, s2; assign a, a1; while w, w1; if ifs, ifs1; variable v, v1; procedure p, q, p1; constant c, c1; call cl; print pn; read re; stmt n, n1; Select a pattern a ( \"w\" , \"y * z\")";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select a pattern a(\"w\", \"y*z\") ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Miscel Test3" ) {
        string test_string ="assign a; stmt s; Select BOOLEAN pattern a ( s , \"x\" )";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "pattern first argument can only be variable synonym or wildcard or double quote";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Miscel Test4" ) {
        string test_string ="assign a; Select BOOLEAN such that Modifies ( sdfsdf , \"x\")";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "syntactically incorrect, it is not stmtRef";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Miscel Test5" ) {
        string test_string ="variable v; Select BOOLEAN with v = \"x\"";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "missing valid delim";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Miscel Test6" ) {
        string test_string ="call cl; while w; Select BOOLEAN";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select BOOLEAN ";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Miscel Test7" ) {
        string test_string = "stmt s, s1, s2; assign a, a1; while w, w1; if ifs, ifs1; variable v, v1; procedure p, q, p1; constant c, c1; call cl; print pn; read re;\n"
                               "Select BOOLEAN pattern w(c,_)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "pattern first argument can only be variable synonym or wildcard or double quote";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Miscel Test8" ) {
        string test_string = "assign a, a1, a2; while w; if ifs; Select ifs pattern ifs(\"\" , _ , _)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "it cannot be an empty string";
        REQUIRE(expectedResult == result.ToString());
    }

    SECTION( "Miscel Test9" ) {
        string test_string = "stmt s, s1, s2; assign a, a1; while w, w1; if ifs, ifs1; variable v, v1; procedure p, q, p1; constant c, c1; call cl; print pn; read re;\n"
                               "Select BOOLEAN pattern a(c,_)";        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "pattern first argument can only be variable synonym or wildcard or double quote";
        REQUIRE(expectedResult == result.ToString());
    }
    SECTION( "Miscel Test10" ) {
        string test_string = "stmt s, s1, s2; assign a, a1; while w, w1; if ifs, ifs1; variable v, v1; procedure p, q, p1; constant c, c1; call cl; print pn; read re;\n"
                               "Select <c,  c1> such that Parent*(_,_) and Parent(pn,s)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        string expectedResult = "Select < c c1 > such that Parent*(_, _) Parent(pn, s) ";
        REQUIRE(expectedResult == result.ToString());
    }
}

TEST_CASE("General Test") {
    string test_string = "assign a1, a2; while w1, w2; Select a2 pattern a1 (\"x\", _) and a2 (\"x\", _\"x\"_) such that Affects (a1, a2) and Parent* (w2, a2) and Parent* (w1, w2)";
    string test_string2 = "procedure p; call c; assign a; while w; Select p such that Calls (\"Second\", p) and Parent (w, c) with c.procName = p.procName";
    string test_string3 = "procedure p, q; Select p such that Calls* (_, \"Third\")";
    string test_string4 = "stmt s; Select s such that Follows(_, _)";
    string test_string5 = "stmt s; Select";
    string test_string6 = "procedure p; Select p with p.procName = \"Third\"";
    string test_string7 = "procedure p; call c; Select p with c.procName = p.procName";
    string test_string8 = "procedure p; call c1; constant c; stmt s; Select p with s.stmt# = c.value";
    string test_string9 = "procedure p; call c1; constant c; stmt s; Select p with s.stmt# = 10 and s.stmt# = c.value";
    string test_string10 = "procedure p; call c1; constant c; stmt s; assign a1, a2; Select p with s.stmt# = 10 and such that Affects (a1, a2)";
    string test_string11 = "variable v; Select v";
    string test_string12 = "variable v; Select <v, BOOLEAN>";
    string test_string13 = "variable v; Select <BOOLEAN, BOOLEAN>";
    string test_string14 = "variable v; constant c; Select c with c.value = 929999999999999999999999999999999999999999999999999999999999999999999999";
    string test_string15 = "Select BOOLEAN with 1 = -1";
    string test_string16 = "Select BOOLEAN with \"hello\" = \"he                    llo\" ";
    string test_string17 = "Select BOOLEAN with \"\" = \"\"";
    string test_string18 = "assign a1, a2; Select <a1.stmt#, a2> such that Affects (a1, a2)";
    string test_string19 = "assign a1, a2; procedure p; Select <p.procName>";
    string test_string20 = "assign a1, a2; procedure p; Select p such that Calls(p, p)";
    string test_string21 = "assign a1, a2; procedure p; Select p such that Calls(p, _)";
    string test_string22 = "assign a1, a2; procedure p; Select p such that Calls(_, _)";
    string test_string23 = "assign a1, a2; procedure p; if ifs; while w; Select p such that Calls(_, _) pattern w (_, _)";
    string test_string24 = "assign a1, a2; procedure p; if ifs; while w; Select p such that Calls(_, _) pattern ifs (_, _, _)";
    string test_string25 = "assign a1, a2; procedure p; if ifs; while w; Select p such that Calls(_, _) pattern ifs (_, _, a1)";
    string test_string26 = "assign a, a1, a2; Select a pattern a(_, \"(\")";
    string test_string27 = "assign a, a1, a2; while w; Select w pattern w(\"     circle    \" , _ )";
    string test_string28 = "assign a, a1, a2; while w; if ifs; Select ifs pattern ifs(\" \" , _ , _)";
    string test_string29 = "assign a; variable v; Select a pattern a ( \"true\" , \"0\")";
    string test_string30 = "while w; Select w with \"\" = \"\"";
    string test_string31 = "assign a, a1, a2; while w; if ifs; Select ifs pattern ifs(\"\" , _ , _)";
    string test_string32 = "assign a, a1, a2; while w; if ifs; Select a with \"hello\" = \"hello \" ";
    string test_string33 = "assign a; variable v; Select a pattern a ( _ , _\"x + y + z * a\"_)";
    string test_string34 = "variable v,v2; stmt s; constant c; procedure p; Select <s,c,p,v> such that Uses(p,v) with s.stmt# = c.value";
    string test_string35 = "variable v; stmt v; constant v; procedure v; Select <v,v,v>";
    string test_string36 = "variable v; print pn; Select BOOLEAN such that Affects(pn, _)";
    string test_string37 = "variable v; print pn; assign a; Select BOOLEAN with 1=1";
    string test_string38 = "variable v; print pn; assign a; Select BOOLEAN";
    string test_string39 = "variable v; print pn; assign and; Select BOOLEAN with 7=and.stmt#";
    string test_string40 = "variable v; print pn; assign a1; Select a1 pattern a1 (\"x\", \"_y_\")";
    string test_string41 = "variable v; print pn; stmt s; Select BOOLEAN such that Affects(,s)";
    string test_string42 = "stmt s; Select      s	  such	 that Follows (     s		, 	 17			)";
    string test_string43 = "stmt s; Select s such that Modifies(1, \"z\")";
    string test_string44 = "while w; Select w pattern w (\"x\", \"x\", \"x\")";
    string test_string45 = "if ifs; Select ifs pattern ifs (\"x\", \"x\", \"x\")";
    string test_string46 = "stmt s; Select s such that Affects(, s)";
    string test_string47 = "stmt 1s; Select 1s";
    string test_string48 = "stmt s1; Select s2";
    string test_string49 = "assign a; variable v; Select v a(v, _\"a\"_)";
    string test_string50= "variable v; assign a; stmt s, s1, s2; Select v such that Follows (1, v)";
    string test_string51 = "assign a; variable v; Select v and pattern a(v, _\"a\"_)";
    string test_string52 = "assign a; variable v; Select v and pattern a(v, _\"a\"_)";
    string test_string53 = "variable v; assign a;\n"
                           "Select v such that Uses(a,v) pattern a(_,_\"2\"_)";
    string test_string54 = "assign a;\n"
                           "Select a such that Uses(a, \"c\") pattern a(\"c\", _)";
    string test_string55 = "assign a; variable v; Select v and pattern a(v, _\"a\"_)";
    string test_string56 = "assign a; variable v;\n"
                           "Select a such that Uses(a, _) pattern a(\"c\", _\"c\"_)";
    string test_string57 = "assign a;\n"
                           "Select a such that Modifies(a, \"c\") pattern a(_, _\"b\"_)";
    string test_string58 = "assign a; variable v;\n"
                           "Select a such that Modifies(a, _) pattern a(\"c\", _\"c\"_)";
    string test_string59 = "assign a; stmt s;\n"
                           "Select s such that Parent(s, a) pattern a(_, _\"3\"_)";
    string test_string60 = "while w1; while w; assign a;\n"
                           "Select w1 such that Parent(w, a) pattern a(_, _\"1\"_)";
    string test_string61 = "while w; assign a;\n"
                           "Select w such that Parent(w, a) pattern a(_, _\"1\"_)";
    string test_string62 = "assign a; stmt s;\n"
                           "Select s such that Parent*(s, a) pattern a(_, _\"x + 5\"_)";
    string test_string63 = "stmt s;\n"
                           "        Select BOOLEAN with \"First\" = s.stmt#";
    string test_string64 = "procedure p;\n"
                           "        Select p such that Calls(p, \"Second\")";
    string test_string65 = "variable v; assign a; stmt s; procedure p; print pn; read r;  Select v";
    string test_string66 = "assign a1, a2; procedure p; call c; constant c1; stmt s, BOOLEAN; Select <BOOLEAN, c1.value, a1, c.procName, s.stmt#>";
    string test_string67 = "variable v; assign a; stmt s; procedure p; print pn; read r;  Select v with \"hello\" = \"      he      llo     \"";
    string test_string69 = "variable v; assign a; stmt s; procedure p; print pn; read r;  Select v with \"circle\" = \"      circle    \"";
    string test_string70 = "variable v; assign a; stmt s; procedure p; print pn; read r; while w;  Select w pattern w(\" circle \", _)";
    string test_string72 = "variable v; assign a; stmt s; procedure p; print pn; read r;  Select v with \"\" = \"\"";
    string test_string73 = "variable v; assign a; stmt s; procedure p; print pn; read r; while w;  Select v with v.stmt# = w.stmt#";
    string test_string74 = "variable v; assign a; stmt s; procedure p; print pn; read r; while w;  Select a such that Follows(a,2) pattern a(_, _\"select*while+if-assign / stmt % stmtList\"_)";
    string test_string76 = "procedure s,s1; Select BOOLEAN such that Calls(s, s1) and Calls(s1, s) and Follows(1, 2)";
    string test_string78 = "assign a1; Select a1 pattern a1 (_, \"(x * y - z)\")";
    string test_string80 = "stmt s, s1, s2; assign a, a1; while w, w1; if ifs, ifs1; variable v, v1; procedure p, q, p1; constant c, c1; call cl; print pn; read re; Select BOOLEAN such that Affects*(a, a2) and Affects*(a2, a1) and Affects* (a1, a)";
    string test_string81 = "stmt s, s1, s2; assign a, a1; while w, w1; if ifs, ifs1; variable v, v1; procedure p, q, p1; constant c, c1; call cl; print pn; read re; Select a such that Affects(a, a1) with a1.stmt# = c.value such that Affects(14, 15)";
    string test_string83 = "stmt s; if ifs; assign a; procedure p;\n"
"Select <a, ifs, s> such that Follows(s,24) pattern ifs(\"y\",_,_) pattern a(_,_\"b+c\"_) with p.procName = procC";
    string test_string84 = "stmt s, s1, s2; assign a, a1; while w, w1; if ifs, ifs1; variable v, v1; procedure p, q, p1; constant c, c1; call cl; print pn; read re;\n"
                           "Select <c,  c1> such that Parent*(_,_) and Parent(pn,s)";
    string test_string86 = "stmt s, s1, s2; assign a, a1; while w, w1; if ifs, ifs1; variable v, v1; procedure p, q, p1; constant c, c1; call cl; print pn; read re;\n"
                           "Select BOOLEAN pattern a(c,_)";
    string test_string88 = "assign a, a1, a2; while w; if ifs; Select ifs pattern ifs(\"\" , _ , _)";
    string test_string89 = "stmt s, s1, s2; assign a, a1; while w, w1; if ifs, ifs1; variable v, v1; procedure p, q, p1; constant c, c1; call cl; print pn; read re;\n"
                           "Select BOOLEAN pattern w(c,_)";
    string test_string90 = "stmt s, s1, s2; assign a, a1; while w, w1; if ifs, ifs1; variable v, v1; procedure p, q, p1; constant c, c1; call cl; print pn; read re;\n"
                           "Select w pattern w(_,_)";
    string test_string91 = "stmt s, s1, s2; assign a, a1; while w, w1; if ifs, ifs1; variable v, v1; procedure p, q, p1; constant c, c1; call cl; print pn; read re; stmt n, n1; Select a pattern a ( \"w\" , \"y * z\")";
    string test_string93 ="assign a; stmt s; Select BOOLEAN pattern a ( s , \"x\" )";
    string test_string94 ="assign a; Select BOOLEAN such that Modifies ( sdfsdf , \"x\")";
    string test_string95 ="variable v; Select BOOLEAN with v = \"x\"";
    string test_string97 ="call cl; while w; Select BOOLEAN";
    string test_string98 ="assign a; Select BOOLEAN pattern a(_, \"^2^\")";
    string test_string99 ="assign a; Select BOOLEAN pattern a(_, \"(((((\")";
    string test_string100 ="assign a; Select BOOLEAN pattern a(_, \"(msmsmsms\")";
    string test_string102 ="call c; Select BOOLEAN with c.procName = p.varName";
    QueryLexer ql1 = QueryLexer(test_string102);
    QueryParser qp1 = QueryParser(&ql1);
    Query result = qp1.query();
    REQUIRE(1 == 1);
}