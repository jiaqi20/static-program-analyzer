#include <string>
#include <vector>

#include "catch.hpp"
#include "../src/model/QType.h"

using namespace std;

TEST_CASE("Test RefValue") {
    RefValue testingOnlyType = RefValue(kWildcard);
    RefValue testingNumber = RefValue(kNumber, 1);
    RefValue testingString = RefValue(kSynonym, "x");
    SECTION("test getName") {
        REQUIRE(testingString.GetName() == "x");
    }
    SECTION("test getValue") {
        REQUIRE(testingNumber.GetValue() == 1);
    }
    SECTION("test GetType") {
        REQUIRE(testingNumber.GetType() == kNumber);
    }
    SECTION("test setName") {
        RefValue temp = RefValue(kFixedValue, "y");
        string newName = "z";
        temp.SetName(newName);
        REQUIRE(temp.GetName() == "z");
    }
    SECTION("test setValue") {
        RefValue temp = RefValue(kNumber, 1);
        int newValue = 2;
        temp.SetValue(newValue);
        REQUIRE(temp.GetValue() == 2);
    }
    SECTION("test setType") {
        RefValue temp = RefValue(kWildcard);
        ValueType newType = kNumber;
        temp.SetType(newType);
        REQUIRE(temp.GetType() == kNumber);
    }
    SECTION("test Equals") {
        RefValue temp = RefValue(kSynonym, "x");
        REQUIRE(temp.equals(&testingString));
        REQUIRE(!temp.equals(&testingOnlyType));
    }
    SECTION("test ==") {
        RefValue temp = RefValue(kSynonym, "x");
        REQUIRE(temp == testingString);
        REQUIRE(!(temp == testingOnlyType));
    }
    SECTION("test typeEquals") {
        RefValue temp = RefValue(kWildcard);
        REQUIRE(temp.TypeEquals(&testingOnlyType));
    }
    SECTION("test ToString") {
        RefValue temp = RefValue(kFixedValue, "y");
        REQUIRE(testingOnlyType.ToString() == "_");
        REQUIRE(testingNumber.ToString() == "1");
        REQUIRE(testingString.ToString() == "x");
        REQUIRE(temp.ToString() == "\"y\"");
    }
}

TEST_CASE("Test Ref") {
    RefValue testingOnlyType = RefValue(kWildcard);
    RefValue testingNumber = RefValue(kNumber, 1);
    RefValue testingString = RefValue(kSynonym, "x");
    RefValue testingFixedValue = RefValue(kFixedValue, "x");
    Ref testingWildCard = Ref(kVariable, testingOnlyType);
    Ref testingVariable = Ref(kVariable, testingString);
    Ref testingConstant = Ref(kConstant, testingNumber);
    Ref testingStatement = Ref(kStmt, testingNumber);
    Ref testingProcedure = Ref(kProcedure, testingFixedValue);
    SECTION("test ToString") {
        REQUIRE(testingWildCard.ToString() == "_");
        REQUIRE(testingVariable.ToString() == "x");
        REQUIRE(testingConstant.ToString() == "1");
        REQUIRE(testingStatement.ToString() == "1");
        REQUIRE(testingProcedure.ToString() == "\"x\"");
    }
    SECTION("test GetType") {
        REQUIRE(testingWildCard.GetType() == kVariable);
        REQUIRE(testingVariable.GetType() == kVariable);
        REQUIRE(testingConstant.GetType() == kConstant);
        REQUIRE(testingStatement.GetType() == kStmt);
        REQUIRE(testingProcedure.GetType() == kProcedure);
    }
    SECTION("test getName") {
        REQUIRE(testingWildCard.getName()->equals(&testingOnlyType));
        REQUIRE(testingVariable.getName()->equals(&testingString));
        REQUIRE(testingConstant.getName()->equals(&testingNumber));
        REQUIRE(testingStatement.getName()->equals(&testingNumber));
        REQUIRE(testingProcedure.getName()->equals(&testingFixedValue));
    }
    SECTION("test setType") {
        Ref temp = Ref(kStmt, testingNumber);
        EntType newType = kAssign;
        temp.SetType(newType);
        REQUIRE(temp.GetType() == newType);
    }
    SECTION("test setName") {
        Ref temp = Ref(kStmt, testingNumber);
        RefValue newValue = RefValue(kNumber, 2);
        temp.SetName(newValue);
        REQUIRE(temp.getName()->equals(&newValue));
    }
    SECTION("test isStatement") {
        REQUIRE(testingProcedure.IsStatement() != true);
        REQUIRE(testingStatement.IsStatement() == true);
    }
    SECTION("test isVariable") {
        REQUIRE(testingVariable.IsVariable() == true);
        REQUIRE(testingStatement.IsVariable() == false);
    }
    SECTION("test isConstant") {
        REQUIRE(testingConstant.IsConstant() == true);
        REQUIRE(testingStatement.IsConstant() == false);
    }
    SECTION("test isProcedure") {
        REQUIRE(testingProcedure.IsProcedure() == true);
        REQUIRE(testingStatement.IsProcedure() == false);
    }
    SECTION("test equals") {
        Ref temp = Ref(kStmt, testingNumber);
        REQUIRE(temp.equals(&testingStatement));
        REQUIRE(!temp.equals(&testingProcedure));
    }
    SECTION("test ==") {
        Ref temp = Ref(kStmt, testingNumber);
        REQUIRE(temp == testingStatement);
        REQUIRE(!(temp == testingProcedure));
    }
}

TEST_CASE("Test Relation") {

    SECTION("test initialize") {
        Relation testingModifies = Relation();
        testingModifies.SetType(kModifiess);
        Ref statement = Ref(kStmt, RefValue(kNumber, 1));
        Ref variable = Ref(kVariable, RefValue(kSynonym, "x"));
        testingModifies.SetArgument1(statement);
        testingModifies.SetArgument2(variable);
        REQUIRE(testingModifies.ToString() == "Modifies(1, x)");
    }
}

TEST_CASE("Test Pattern") {
    Pattern testingPattern;
    Ref statement = Ref(kAssign, RefValue(kSynonym, "a"));
    testingPattern.arg1 = statement;
    Ref variable = Ref(kVariable, RefValue(kSynonym, "x"));
    testingPattern.arg2 = variable;
    ExpressionSpecType type = kPartialMatch;
    string exp = "x";
    ExpressionSpec expression = ExpressionSpec(type, exp);
    testingPattern.expression_spec = expression;
    SECTION("test getSynassign") {
        REQUIRE(testingPattern.GetArg1()->equals(&statement));
    }
    SECTION("test getLHS"){
        REQUIRE(testingPattern.GetArg2()->equals(&variable));
    }
    SECTION("test getExpression") {
        REQUIRE(testingPattern.GetExpression()->equals(&expression));
    }
    SECTION("test ToString") {
        REQUIRE(testingPattern.ToString() == "pattern a(x, _\"x\"_)");
    }
}

TEST_CASE("Test ExpressionSpec") {
    ExpressionSpecType type = kPartialMatch;
    string exp = "x";
    ExpressionSpec expression = ExpressionSpec(type, exp);
    SECTION("test getExpression") {
        REQUIRE(expression.GetExpression() == "x");
    }
    SECTION("test GetType") {
        REQUIRE(expression.GetType() == kPartialMatch);
    }
    SECTION("test ToString") {
        REQUIRE(expression.ToString() == "_\"x\"_");
    }
    SECTION("test equals") {
        ExpressionSpecType type = kPartialMatch;
        string exp = "x";
        ExpressionSpec temp1 = ExpressionSpec(type, exp);
        ExpressionSpecType type2 = kPartialMatch;
        string exp2 = "y";
        ExpressionSpec temp2 = ExpressionSpec(type2, exp2);
        REQUIRE(temp1.equals(&expression));
        REQUIRE(!temp1.equals(&temp2));
    }
}