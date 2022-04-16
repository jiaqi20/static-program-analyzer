#include <string>

#include "PKB/Attribute/StatementNumber.h"
#include "PKB/Attribute/StatementNumber.cpp"

#include "catch.hpp"

using namespace std;

TEST_CASE("StatementNumber: Test Get statement number") {
    StatementNumber validStatementOne = StatementNumber(1);
    SECTION( "Valid statement number can indeed get the correct statement number" ) {
        REQUIRE(validStatementOne.GetLine() == 1);
    }
}

TEST_CASE("StatementNumber: Test operator ==") {
    StatementNumber validStatementOne = StatementNumber(1);
    StatementNumber validStatementTwo = StatementNumber(2);
    StatementNumber validStatementAnotherOne = StatementNumber(1);
    SECTION( "Two different name are indeed different" ) {
        REQUIRE(!(validStatementOne == validStatementTwo));
    }

    SECTION( "Two same name are indeed the same" ) {
        REQUIRE(validStatementOne == validStatementAnotherOne);
    }
}