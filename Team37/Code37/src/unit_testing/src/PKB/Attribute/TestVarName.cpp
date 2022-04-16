#include <string>

#include "../src/PKB/Attribute/VarName.h"
#include "../src/PKB/Attribute/VarName.cpp"

#include "catch.hpp"

using namespace std;

TEST_CASE("VarName: Test Validity of Name") {
    VarName validName = VarName("test");
    VarName invalidName = VarName("");

    SECTION( "Valid name is indeed valid" ) {
        REQUIRE(validName.IsValid());
    }

    SECTION( "Invalid Index is indeed invalid" ) {
        REQUIRE(!invalidName.IsValid());
    }
}

TEST_CASE("VarName: Test Get variable name") {
    VarName validNameTestOne = VarName("TestOne");
    VarName invalidName = VarName("");
    SECTION( "Valid variable name can indeed get the correct name" ) {
        REQUIRE(validNameTestOne.GetName() == "TestOne");
    }
    SECTION( "Invalid name cannot get the name" ) {
        REQUIRE(invalidName.GetName() == "");
    }
}

TEST_CASE("VarName: Test operator ==") {
    VarName validNameTestOne = VarName("TestOne");
    VarName validNameTestTwo= VarName("TestTwo");
    VarName validNameAnotherTestOne = VarName("TestOne");
    SECTION( "Two different name are indeed different" ) {
        REQUIRE(!(validNameTestOne == validNameTestTwo));
    }

    SECTION( "Two same name are indeed the same" ) {
        REQUIRE(validNameTestOne == validNameAnotherTestOne);
    }
}

TEST_CASE("VarName: Test operator <") {
    VarName validNameTestOne = VarName("TestOne");
    VarName validNameUestTwo= VarName("UestOne");
    VarName validNameAnotherTestOne = VarName("TestOne");
    SECTION( "Smaller name are indeed smaller than the bigger name" ) {
        REQUIRE(validNameTestOne < validNameUestTwo);
    }

    SECTION( "Bigger name are indeed bigger than the small name" ) {
        REQUIRE(!(validNameUestTwo < validNameTestOne));
    }

    SECTION( "Same value name are indeed same as other same name" ) {
        REQUIRE(!(validNameTestOne < validNameAnotherTestOne));
    }
}