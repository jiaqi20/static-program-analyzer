#include <string>
#include <fstream>
#include <list>
#include <iostream>

#include "../src/model/ConditionResultTable.h"
#include "../src/model/ConditionResultTable.cpp"

#include "catch.hpp"

using namespace std;

TEST_CASE("Test Condition Table 1") {

    ConditionResultTable createData;
    list<Ref> statementCol;

    Ref ref1;
    createData.CreateDataSet(ref1, "s1");
    statementCol.push_back(ref1);

    int number_of_row = 4;
    for (int i = 1; i < number_of_row; i = i + 1) {
        EntType et2 = kStmt;
        ValueType type2 = kSynonym;
        string name2 = to_string(i);
        RefValue rf2 = RefValue(type2, name2);
        Ref ref2 = Ref(et2, rf2);
        statementCol.push_back(ref2);
    }


    list<Ref> statementCol2;
    EntType et3 = kStmt;
    ValueType type3 = kSynonym;
    string name3 = "s2";
    RefValue rf3 = RefValue(type3, name3);
    Ref ref3 = Ref(et3, rf3);
    statementCol2.push_back(ref3);

    for (int i = 1; i < number_of_row; i = i + 1) {
        EntType et2 = kStmt;
        ValueType type2 = kSynonym;
        string name2 = to_string(i);
        RefValue rf2 = RefValue(type2, name2);
        Ref ref2 = Ref(et2, rf2);
        statementCol2.push_back(ref2);
    }

    SECTION("Test Add col") {
        ConditionResultTable crt;
        list<list<Ref>> &input_table = crt.GetTable();
        crt.AddColDirectToTable(input_table, statementCol);
        crt.AddColDirectToTable(input_table, statementCol2);

        list<Ref> statementCol3;
        EntType et3 = kStmt;
        ValueType type3 = kSynonym;
        string name3 = "s3";
        RefValue rf3 = RefValue(type3, name3);
        Ref ref3 = Ref(et3, rf3);
        statementCol3.push_back(ref3);

        for (int i = 1; i < number_of_row; i = i + 1) {
            EntType et2 = kStmt;
            ValueType type2 = kSynonym;
            string name2 = to_string(i);
            RefValue rf2 = RefValue(type2, name2);
            Ref ref2 = Ref(et2, rf2);
            statementCol3.push_back(ref2);
        }
        crt.AddCol(statementCol3);
        string expected = "[s1, 1, 2, 3, 1, 2, 3, 1, 2, 3, ]\n"
                          "[s2, 1, 2, 3, 1, 2, 3, 1, 2, 3, ]\n"
                          "[s3, 1, 1, 1, 2, 2, 2, 3, 3, 3, ]\n"
                          "";
        REQUIRE(expected == crt.PrintCustomTable(crt.GetTable()));
    }

    SECTION("Test Delete col by ref") {
        ConditionResultTable crt;
        list<list<Ref>> &input_table1 = crt.GetTable();
        crt.AddColDirectToTable(input_table1, statementCol);
        crt.AddColDirectToTable(input_table1, statementCol2);

        EntType et = kStmt;
        ValueType type = kSynonym;
        string name = "s2";
        RefValue rf = RefValue(type, name);
        Ref ref = Ref(et, rf);
        list<list<Ref>> &input_table = crt.GetTable();
        crt.DeleteColByRef(ref, input_table);
        string expected = "[s1, 1, 2, 3, ]\n"
                          "";
        REQUIRE(expected == crt.PrintCustomTable(crt.GetTable()));
    }

    SECTION("Test Delete col by index") {
        ConditionResultTable crt;
        list<list<Ref>> &input_table1 = crt.GetTable();
        crt.AddColDirectToTable(input_table1, statementCol);
        crt.AddColDirectToTable(input_table1, statementCol2);

        list<list<Ref>> &input_table = crt.GetTable();
        crt.DeleteColByIndex(1, input_table);
        string expected = "[s1, 1, 2, 3, ]\n"
                          "";
        REQUIRE(expected == crt.PrintCustomTable(crt.GetTable()));
    }

    SECTION("Test Delete row") {
        ConditionResultTable crt;
        list<list<Ref>> &input_table1 = crt.GetTable();
        crt.AddColDirectToTable(input_table1, statementCol);
        crt.AddColDirectToTable(input_table1, statementCol2);

        list<list<Ref>> &input_table = crt.GetTable();
        crt.DeleteRow(2, input_table);
        string expected = "[s1, 1, 3, ]\n"
                          "[s2, 1, 3, ]\n"
                          "";
        REQUIRE(expected == crt.PrintCustomTable(crt.GetTable()));
    }

    SECTION("Test Read row") {
        ConditionResultTable crt;
        list<list<Ref>> &input_table1 = crt.GetTable();
        crt.AddColDirectToTable(input_table1, statementCol);
        crt.AddColDirectToTable(input_table1, statementCol2);

        list<list<Ref>> input_table = crt.GetTable();
        list<Ref> list_ref = crt.ReadRow(2, input_table);
        string result = "";
        for (list<Ref>::iterator it = list_ref.begin(); it != list_ref.end(); it++) {
            result = result + it->ToString() + "   ";
        }
        string expected = "2   2   ";
        REQUIRE(expected == result);
    }

    SECTION("Test Read col") {
        ConditionResultTable crt;
        list<list<Ref>> &input_table1 = crt.GetTable();
        crt.AddColDirectToTable(input_table1, statementCol);
        crt.AddColDirectToTable(input_table1, statementCol2);

        EntType et = kStmt;
        ValueType type = kSynonym;
        string name = "s2";
        RefValue rf = RefValue(type, name);
        Ref ref = Ref(et, rf);

        list<Ref> list_ref = crt.ReadCol(ref);
        string result = "";
        for (list<Ref>::iterator it = list_ref.begin(); it != list_ref.end(); it++) {
            result = result + it->ToString() + "   ";
        }
        string expected = "s2   1   2   3   ";
        REQUIRE(expected == result);
    }

    SECTION("Test Extract Element") {
        ConditionResultTable crt;
        list<list<Ref>> &input_table1 = crt.GetTable();
        crt.AddColDirectToTable(input_table1, statementCol);
        crt.AddColDirectToTable(input_table1, statementCol2);

        EntType et = kStmt;
        ValueType type = kSynonym;
        string name = "s2";
        RefValue rf = RefValue(type, name);
        Ref ref = Ref(et, rf);

        Ref list_ref = crt.ExtractElement(2, ref);
        string expected = "2";
        REQUIRE(expected == list_ref.ToString());
    }

    SECTION("Test Add row") {
        ConditionResultTable crt;
        list<list<Ref>> &input_table1 = crt.GetTable();
        crt.AddColDirectToTable(input_table1, statementCol);
        crt.AddColDirectToTable(input_table1, statementCol2);


        list<Ref> statementCol3;
        EntType et5 = kStmt;
        ValueType type5 = kSynonym;
        string name5 = "7";
        RefValue rf5 = RefValue(type5, name5);
        Ref ref5 = Ref(et5, rf5);
        statementCol3.push_back(ref1);
        statementCol3.push_back(ref5);

        list<Ref> statementCol4;
        EntType et6 = kStmt;
        ValueType type6 = kSynonym;
        string name6 = "8";
        RefValue rf6 = RefValue(type6, name6);
        Ref ref6 = Ref(et6, rf6);
        statementCol4.push_back(ref3);
        statementCol4.push_back(ref6);


        list<list<Ref>> statementCol5;
        statementCol5.push_back(statementCol3);
        statementCol5.push_back(statementCol4);
        crt.AddRow(statementCol5);
        string expected = "[s1, 1, 2, 3, 7, ]\n"
                          "[s2, 1, 2, 3, 8, ]\n"
                          "";
        REQUIRE(expected == crt.PrintCustomTable(crt.GetTable()));
    }

    SECTION("Test Add Table") {
        ConditionResultTable crt;
        list<list<Ref>> &input_table1 = crt.GetTable();
        crt.AddColDirectToTable(input_table1, statementCol);
        crt.AddColDirectToTable(input_table1, statementCol2);

        list<Ref> statementCol3;
        EntType et3 = kStmt;
        ValueType type3 = kSynonym;
        string name3 = "s3";
        RefValue rf3 = RefValue(type3, name3);
        Ref ref3 = Ref(et3, rf3);
        statementCol3.push_back(ref3);

        for (int i = 1; i < number_of_row; i = i + 1) {
            EntType et2 = kStmt;
            ValueType type2 = kSynonym;
            string name2 = to_string(i);
            RefValue rf2 = RefValue(type2, name2);
            Ref ref2 = Ref(et2, rf2);
            statementCol3.push_back(ref2);
        }

        list<Ref> statementCol4;
        EntType et4 = kStmt;
        ValueType type4 = kSynonym;
        string name4 = "s4";
        RefValue rf4 = RefValue(type4, name4);
        Ref ref4 = Ref(et4, rf4);
        statementCol4.push_back(ref4);

        for (int i = 1; i < number_of_row; i = i + 1) {
            EntType et2 = kStmt;
            ValueType type2 = kSynonym;
            string name2 = to_string(i);
            RefValue rf2 = RefValue(type2, name2);
            Ref ref2 = Ref(et2, rf2);
            statementCol4.push_back(ref2);
        }


        list<list<Ref>> input_table;
        input_table.push_back(statementCol3);
        input_table.push_back(statementCol4);


        crt.AddCrossProductTable(input_table);


        string expected = "[s1, 1, 2, 3, 1, 2, 3, 1, 2, 3, ]\n"
                          "[s2, 1, 2, 3, 1, 2, 3, 1, 2, 3, ]\n"
                          "[s3, 1, 1, 1, 2, 2, 2, 3, 3, 3, ]\n"
                          "[s4, 1, 1, 1, 2, 2, 2, 3, 3, 3, ]\n"
                          "";
        REQUIRE(expected == crt.PrintCustomTable(crt.GetTable()));
    }

    SECTION("Test Combine: Common Attribute") {
        list<Ref> Uses;
        Ref ref11;
        createData.CreateDataSet(ref11, "s3");
        Uses.push_back(ref11);

        Ref ref12;
        createData.CreateDataSet(ref12, "5");
        Uses.push_back(ref12);

        Ref ref13;
        createData.CreateDataSet(ref13, "5");
        Uses.push_back(ref13);

        Ref ref14;
        createData.CreateDataSet(ref14, "7");
        Uses.push_back(ref14);

        Ref ref15;
        createData.CreateDataSet(ref15, "8");
        Uses.push_back(ref15);

        Ref ref16;
        createData.CreateDataSet(ref16, "2");
        Uses.push_back(ref16);


        list<Ref> Uses1;
        Ref ref17;
        createData.CreateDataSet(ref17, "v1");
        Uses1.push_back(ref17);

        Ref ref18;
        createData.CreateDataSet(ref18, "y");
        Uses1.push_back(ref18);

        Ref ref19;
        createData.CreateDataSet(ref19, "z");
        Uses1.push_back(ref19);

        Ref ref20;
        createData.CreateDataSet(ref20, "x");
        Uses1.push_back(ref20);

        Ref ref21;
        createData.CreateDataSet(ref21, "y");
        Uses1.push_back(ref21);

        Ref ref22;
        createData.CreateDataSet(ref22, "y");
        Uses1.push_back(ref22);

        ConditionResultTable crt;
        list<list<Ref>> &input_table1 = crt.GetTable();
        crt.AddColDirectToTable(input_table1, Uses);
        crt.AddColDirectToTable(input_table1, Uses1);


        list<Ref> Modifies;
        Ref ref31;
        createData.CreateDataSet(ref31, "s3");
        Modifies.push_back(ref31);

        Ref ref32;
        createData.CreateDataSet(ref32, "6");
        Modifies.push_back(ref32);

        Ref ref33;
        createData.CreateDataSet(ref33, "5");
        Modifies.push_back(ref33);

        Ref ref34;
        createData.CreateDataSet(ref34, "7");
        Modifies.push_back(ref34);


        ConditionResultTable crt1;
        list<list<Ref>> &input_table2 = crt1.GetTable();
        crt1.AddColDirectToTable(input_table2, Modifies);

        crt.combine(input_table2);

        string expected = "[s3, 5, 5, 7, ]\n"
                          "[v1, y, z, x, ]\n"
                          "";
        REQUIRE(expected == crt.PrintCustomTable(crt.GetTable()));
    }

    SECTION("Test Combine: Distinct Attribute") {
        list<Ref> Uses;
        Ref ref11;
        createData.CreateDataSet(ref11, "s3");
        Uses.push_back(ref11);

        Ref ref12;
        createData.CreateDataSet(ref12, "5");
        Uses.push_back(ref12);

        Ref ref13;
        createData.CreateDataSet(ref13, "5");
        Uses.push_back(ref13);

        Ref ref14;
        createData.CreateDataSet(ref14, "7");
        Uses.push_back(ref14);

        Ref ref15;
        createData.CreateDataSet(ref15, "8");
        Uses.push_back(ref15);

        Ref ref16;
        createData.CreateDataSet(ref16, "2");
        Uses.push_back(ref16);


        list<Ref> Uses1;
        Ref ref17;
        createData.CreateDataSet(ref17, "v1");
        Uses1.push_back(ref17);

        Ref ref18;
        createData.CreateDataSet(ref18, "y");
        Uses1.push_back(ref18);

        Ref ref19;
        createData.CreateDataSet(ref19, "z");
        Uses1.push_back(ref19);

        Ref ref20;
        createData.CreateDataSet(ref20, "x");
        Uses1.push_back(ref20);

        Ref ref21;
        createData.CreateDataSet(ref21, "y");
        Uses1.push_back(ref21);

        Ref ref22;
        createData.CreateDataSet(ref22, "y");
        Uses1.push_back(ref22);

        ConditionResultTable crt;
        list<list<Ref>> &input_table1 = crt.GetTable();
        crt.AddColDirectToTable(input_table1, Uses);
        crt.AddColDirectToTable(input_table1, Uses1);


        list<Ref> Modifies;
        Ref ref31;
        createData.CreateDataSet(ref31, "s3");
        Modifies.push_back(ref31);

        Ref ref32;
        createData.CreateDataSet(ref32, "6");
        Modifies.push_back(ref32);

        Ref ref33;
        createData.CreateDataSet(ref33, "5");
        Modifies.push_back(ref33);

        Ref ref34;
        createData.CreateDataSet(ref34, "7");
        Modifies.push_back(ref34);


        ConditionResultTable crt1;
        list<list<Ref>> &input_table2 = crt1.GetTable();
        crt1.AddColDirectToTable(input_table2, Modifies);

        crt.combine(input_table2);

        list<Ref> Follows;
        Ref ref41;
        createData.CreateDataSet(ref41, "s1");
        Follows.push_back(ref41);

        Ref ref42;
        createData.CreateDataSet(ref42, "6");
        Follows.push_back(ref42);

        Ref ref43;
        createData.CreateDataSet(ref43, "3");
        Follows.push_back(ref43);

        Ref ref44;
        createData.CreateDataSet(ref44, "10");
        Follows.push_back(ref44);

        Ref ref45;
        createData.CreateDataSet(ref45, "8");
        Follows.push_back(ref45);


        list<Ref> Follows1;
        Ref ref46;
        createData.CreateDataSet(ref46, "s2");
        Follows1.push_back(ref46);

        Ref ref49;
        createData.CreateDataSet(ref49, "20");
        Follows1.push_back(ref49);

        Ref ref50;
        createData.CreateDataSet(ref50, "4");
        Follows1.push_back(ref50);

        Ref ref47;
        createData.CreateDataSet(ref47, "15");
        Follows1.push_back(ref47);

        Ref ref48;
        createData.CreateDataSet(ref48, "11");
        Follows1.push_back(ref48);

        ConditionResultTable crt2;
        list<list<Ref>> &input_table3 = crt2.GetTable();
        crt2.AddColDirectToTable(input_table3, Follows);
        crt2.AddColDirectToTable(input_table3, Follows1);

        crt.combine(input_table3);

        string expected = "[s3, 5, 5, 7, 5, 5, 7, 5, 5, 7, 5, 5, 7, ]\n"
                          "[v1, y, z, x, y, z, x, y, z, x, y, z, x, ]\n"
                          "[s1, 6, 6, 6, 3, 3, 3, 10, 10, 10, 8, 8, 8, ]\n"
                          "[s2, 20, 20, 20, 4, 4, 4, 15, 15, 15, 11, 11, 11, ]\n"
                          "";
        REQUIRE(expected == crt.PrintCustomTable(crt.GetTable()));
    }

    SECTION("Test Combine: Distinct Attribute + Parent") {
        list<Ref> Uses;
        Ref ref11;
        createData.CreateDataSet(ref11, "s3");
        Uses.push_back(ref11);

        Ref ref12;
        createData.CreateDataSet(ref12, "5");
        Uses.push_back(ref12);

        Ref ref13;
        createData.CreateDataSet(ref13, "5");
        Uses.push_back(ref13);

        Ref ref14;
        createData.CreateDataSet(ref14, "7");
        Uses.push_back(ref14);

        Ref ref15;
        createData.CreateDataSet(ref15, "8");
        Uses.push_back(ref15);

        Ref ref16;
        createData.CreateDataSet(ref16, "2");
        Uses.push_back(ref16);


        list<Ref> Uses1;
        Ref ref17;
        createData.CreateDataSet(ref17, "v1");
        Uses1.push_back(ref17);

        Ref ref18;
        createData.CreateDataSet(ref18, "y");
        Uses1.push_back(ref18);

        Ref ref19;
        createData.CreateDataSet(ref19, "z");
        Uses1.push_back(ref19);

        Ref ref20;
        createData.CreateDataSet(ref20, "x");
        Uses1.push_back(ref20);

        Ref ref21;
        createData.CreateDataSet(ref21, "y");
        Uses1.push_back(ref21);

        Ref ref22;
        createData.CreateDataSet(ref22, "y");
        Uses1.push_back(ref22);

        ConditionResultTable crt;
        list<list<Ref>> &input_table1 = crt.GetTable();
        crt.AddColDirectToTable(input_table1, Uses);
        crt.AddColDirectToTable(input_table1, Uses1);


        list<Ref> Modifies;
        Ref ref31;
        createData.CreateDataSet(ref31, "s3");
        Modifies.push_back(ref31);

        Ref ref32;
        createData.CreateDataSet(ref32, "6");
        Modifies.push_back(ref32);

        Ref ref33;
        createData.CreateDataSet(ref33, "5");
        Modifies.push_back(ref33);

        Ref ref34;
        createData.CreateDataSet(ref34, "7");
        Modifies.push_back(ref34);


        ConditionResultTable crt1;
        list<list<Ref>> &input_table2 = crt1.GetTable();
        crt1.AddColDirectToTable(input_table2, Modifies);

        crt.combine(input_table2);

        list<Ref> Follows;
        Ref ref41;
        createData.CreateDataSet(ref41, "s1");
        Follows.push_back(ref41);

        Ref ref42;
        createData.CreateDataSet(ref42, "6");
        Follows.push_back(ref42);

        Ref ref43;
        createData.CreateDataSet(ref43, "3");
        Follows.push_back(ref43);

        Ref ref44;
        createData.CreateDataSet(ref44, "10");
        Follows.push_back(ref44);

        Ref ref45;
        createData.CreateDataSet(ref45, "8");
        Follows.push_back(ref45);


        list<Ref> Follows1;
        Ref ref46;
        createData.CreateDataSet(ref46, "s2");
        Follows1.push_back(ref46);

        Ref ref49;
        createData.CreateDataSet(ref49, "20");
        Follows1.push_back(ref49);

        Ref ref50;
        createData.CreateDataSet(ref50, "4");
        Follows1.push_back(ref50);

        Ref ref47;
        createData.CreateDataSet(ref47, "15");
        Follows1.push_back(ref47);

        Ref ref48;
        createData.CreateDataSet(ref48, "11");
        Follows1.push_back(ref48);

        ConditionResultTable crt2;
        list<list<Ref>> &input_table3 = crt2.GetTable();
        crt2.AddColDirectToTable(input_table3, Follows);
        crt2.AddColDirectToTable(input_table3, Follows1);

        crt.combine(input_table3);

        list<Ref> Parent;
        Ref ref51;
        createData.CreateDataSet(ref51, "s3");
        Parent.push_back(ref51);

        Ref ref52;
        createData.CreateDataSet(ref52, "5");
        Parent.push_back(ref52);

        Ref ref53;
        createData.CreateDataSet(ref53, "5");
        Parent.push_back(ref53);

        Ref ref54;
        createData.CreateDataSet(ref54, "2");
        Parent.push_back(ref54);

        Ref ref55;
        createData.CreateDataSet(ref55, "2");
        Parent.push_back(ref55);

        Ref ref56;
        createData.CreateDataSet(ref56, "7");
        Parent.push_back(ref56);


        list<Ref> Parent1;
        Ref ref57;
        createData.CreateDataSet(ref57, "s1");
        Parent1.push_back(ref57);

        Ref ref58;
        createData.CreateDataSet(ref58, "6");
        Parent1.push_back(ref58);

        Ref ref59;
        createData.CreateDataSet(ref59, "8");
        Parent1.push_back(ref59);

        Ref ref60;
        createData.CreateDataSet(ref60, "7");
        Parent1.push_back(ref60);

        Ref ref61;
        createData.CreateDataSet(ref61, "3");
        Parent1.push_back(ref61);

        Ref ref62;
        createData.CreateDataSet(ref62, "6");
        Parent1.push_back(ref62);

        ConditionResultTable crt3;
        list<list<Ref>> &input_table4 = crt3.GetTable();
        crt3.AddColDirectToTable(input_table4, Parent);
        crt3.AddColDirectToTable(input_table4, Parent1);

        crt.combine(input_table4);

        string expected = "[s3, 5, 5, 5, 5, 7, ]\n"
                          "[v1, y, y, z, z, x, ]\n"
                          "[s1, 6, 8, 6, 8, 6, ]\n"
                          "[s2, 20, 11, 20, 11, 20, ]\n"
                          "";
        REQUIRE(expected == crt.PrintCustomTable(crt.GetTable()));
    }

    SECTION("Test Combine: Distinct Attribute + Parent + Another uses") {
        list<Ref> Uses;
        Ref ref11;
        createData.CreateDataSet(ref11, "s3");
        Uses.push_back(ref11);

        Ref ref12;
        createData.CreateDataSet(ref12, "5");
        Uses.push_back(ref12);

        Ref ref13;
        createData.CreateDataSet(ref13, "5");
        Uses.push_back(ref13);

        Ref ref14;
        createData.CreateDataSet(ref14, "7");
        Uses.push_back(ref14);

        Ref ref15;
        createData.CreateDataSet(ref15, "8");
        Uses.push_back(ref15);

        Ref ref16;
        createData.CreateDataSet(ref16, "2");
        Uses.push_back(ref16);


        list<Ref> Uses1;
        Ref ref17;
        createData.CreateDataSet(ref17, "v1");
        Uses1.push_back(ref17);

        Ref ref18;
        createData.CreateDataSet(ref18, "y");
        Uses1.push_back(ref18);

        Ref ref19;
        createData.CreateDataSet(ref19, "z");
        Uses1.push_back(ref19);

        Ref ref20;
        createData.CreateDataSet(ref20, "x");
        Uses1.push_back(ref20);

        Ref ref21;
        createData.CreateDataSet(ref21, "y");
        Uses1.push_back(ref21);

        Ref ref22;
        createData.CreateDataSet(ref22, "y");
        Uses1.push_back(ref22);

        ConditionResultTable crt;
        list<list<Ref>> &input_table1 = crt.GetTable();
        crt.AddColDirectToTable(input_table1, Uses);
        crt.AddColDirectToTable(input_table1, Uses1);


        list<Ref> Modifies;
        Ref ref31;
        createData.CreateDataSet(ref31, "s3");
        Modifies.push_back(ref31);

        Ref ref32;
        createData.CreateDataSet(ref32, "6");
        Modifies.push_back(ref32);

        Ref ref33;
        createData.CreateDataSet(ref33, "5");
        Modifies.push_back(ref33);

        Ref ref34;
        createData.CreateDataSet(ref34, "7");
        Modifies.push_back(ref34);


        ConditionResultTable crt1;
        list<list<Ref>> &input_table2 = crt1.GetTable();
        crt1.AddColDirectToTable(input_table2, Modifies);

        crt.combine(input_table2);

        list<Ref> Follows;
        Ref ref41;
        createData.CreateDataSet(ref41, "s1");
        Follows.push_back(ref41);

        Ref ref42;
        createData.CreateDataSet(ref42, "6");
        Follows.push_back(ref42);

        Ref ref43;
        createData.CreateDataSet(ref43, "3");
        Follows.push_back(ref43);

        Ref ref44;
        createData.CreateDataSet(ref44, "10");
        Follows.push_back(ref44);

        Ref ref45;
        createData.CreateDataSet(ref45, "8");
        Follows.push_back(ref45);


        list<Ref> Follows1;
        Ref ref46;
        createData.CreateDataSet(ref46, "s2");
        Follows1.push_back(ref46);

        Ref ref49;
        createData.CreateDataSet(ref49, "20");
        Follows1.push_back(ref49);

        Ref ref50;
        createData.CreateDataSet(ref50, "4");
        Follows1.push_back(ref50);

        Ref ref47;
        createData.CreateDataSet(ref47, "15");
        Follows1.push_back(ref47);

        Ref ref48;
        createData.CreateDataSet(ref48, "11");
        Follows1.push_back(ref48);

        ConditionResultTable crt2;
        list<list<Ref>> &input_table3 = crt2.GetTable();
        crt2.AddColDirectToTable(input_table3, Follows);
        crt2.AddColDirectToTable(input_table3, Follows1);

        crt.combine(input_table3);

        list<Ref> Parent;
        Ref ref51;
        createData.CreateDataSet(ref51, "s3");
        Parent.push_back(ref51);

        Ref ref52;
        createData.CreateDataSet(ref52, "5");
        Parent.push_back(ref52);

        Ref ref53;
        createData.CreateDataSet(ref53, "5");
        Parent.push_back(ref53);

        Ref ref54;
        createData.CreateDataSet(ref54, "2");
        Parent.push_back(ref54);

        Ref ref55;
        createData.CreateDataSet(ref55, "2");
        Parent.push_back(ref55);

        Ref ref56;
        createData.CreateDataSet(ref56, "7");
        Parent.push_back(ref56);


        list<Ref> Parent1;
        Ref ref57;
        createData.CreateDataSet(ref57, "s1");
        Parent1.push_back(ref57);

        Ref ref58;
        createData.CreateDataSet(ref58, "6");
        Parent1.push_back(ref58);

        Ref ref59;
        createData.CreateDataSet(ref59, "8");
        Parent1.push_back(ref59);

        Ref ref60;
        createData.CreateDataSet(ref60, "7");
        Parent1.push_back(ref60);

        Ref ref61;
        createData.CreateDataSet(ref61, "3");
        Parent1.push_back(ref61);

        Ref ref62;
        createData.CreateDataSet(ref62, "6");
        Parent1.push_back(ref62);

        ConditionResultTable crt3;
        list<list<Ref>> &input_table4 = crt3.GetTable();
        crt3.AddColDirectToTable(input_table4, Parent);
        crt3.AddColDirectToTable(input_table4, Parent1);

        crt.combine(input_table4);

        list<Ref> Usess3;
        Ref ref71;
        createData.CreateDataSet(ref71, "s2");
        Usess3.push_back(ref71);

        Ref ref72;
        createData.CreateDataSet(ref72, "11");
        Usess3.push_back(ref72);

        Ref ref73;
        createData.CreateDataSet(ref73, "20");
        Usess3.push_back(ref73);

        Ref ref74;
        createData.CreateDataSet(ref74, "4");
        Usess3.push_back(ref74);

        Ref ref75;
        createData.CreateDataSet(ref75, "20");
        Usess3.push_back(ref75);

        Ref ref76;
        createData.CreateDataSet(ref76, "11");
        Usess3.push_back(ref76);

        Ref ref77;
        createData.CreateDataSet(ref77, "10");
        Usess3.push_back(ref77);

        list<Ref> Usess4;
        Ref ref78;
        createData.CreateDataSet(ref78, "v1");
        Usess4.push_back(ref78);

        Ref ref79;
        createData.CreateDataSet(ref79, "y");
        Usess4.push_back(ref79);

        Ref ref80;
        createData.CreateDataSet(ref80, "x");
        Usess4.push_back(ref80);

        Ref ref81;
        createData.CreateDataSet(ref81, "x");
        Usess4.push_back(ref81);

        Ref ref85;
        createData.CreateDataSet(ref85, "y");
        Usess4.push_back(ref85);

        Ref ref86;
        createData.CreateDataSet(ref86, "z");
        Usess4.push_back(ref86);

        Ref ref87;
        createData.CreateDataSet(ref87, "z");
        Usess4.push_back(ref87);


        ConditionResultTable crt4;
        list<list<Ref>> &input_table5 = crt4.GetTable();
        crt4.AddColDirectToTable(input_table5, Usess3);
        crt4.AddColDirectToTable(input_table5, Usess4);

        crt.combine(input_table5);

        string expected = "[s3, 5, 5, 5, 7, ]\n"
                          "[v1, y, y, z, x, ]\n"
                          "[s1, 6, 8, 8, 6, ]\n"
                          "[s2, 20, 11, 11, 20, ]\n"
                          "";
        REQUIRE(expected == crt.PrintCustomTable(crt.GetTable()));
    }

    SECTION("Test Combine: Combine table with cross product") {
        list<Ref> Uses;
        Ref ref11;
        createData.CreateDataSet(ref11, "s");
        Uses.push_back(ref11);

        Ref ref12;
        createData.CreateDataSet(ref12, "1");
        Uses.push_back(ref12);

        Ref ref13;
        createData.CreateDataSet(ref13, "1");
        Uses.push_back(ref13);

        Ref ref14;
        createData.CreateDataSet(ref14, "1");
        Uses.push_back(ref14);

        Ref ref15;
        createData.CreateDataSet(ref15, "1");
        Uses.push_back(ref15);


        list<Ref> Uses1;
        Ref ref17;
        createData.CreateDataSet(ref17, "v");
        Uses1.push_back(ref17);

        Ref ref18;
        createData.CreateDataSet(ref18, "else");
        Uses1.push_back(ref18);

        Ref ref19;
        createData.CreateDataSet(ref19, "normal");
        Uses1.push_back(ref19);

        Ref ref20;
        createData.CreateDataSet(ref20, "sum");
        Uses1.push_back(ref20);

        Ref ref21;
        createData.CreateDataSet(ref21, "a2");
        Uses1.push_back(ref21);

        ConditionResultTable crt;
        list<list<Ref>> &input_table1 = crt.GetTable();
        crt.AddColDirectToTable(input_table1, Uses);
        crt.AddColDirectToTable(input_table1, Uses1);


        list<Ref> Modifies;
        Ref ref31;
        createData.CreateDataSet(ref31, "a");
        Modifies.push_back(ref31);

        Ref ref32;
        createData.CreateDataSet(ref32, "17");
        Modifies.push_back(ref32);

        Ref ref33;
        createData.CreateDataSet(ref33, "6");
        Modifies.push_back(ref33);


        list<Ref> Modifies1;
        Ref ref34;
        createData.CreateDataSet(ref34, "v");
        Modifies1.push_back(ref34);

        Ref ref35;
        createData.CreateDataSet(ref35, "else");
        Modifies1.push_back(ref35);

        Ref ref36;
        createData.CreateDataSet(ref36, "else");
        Modifies1.push_back(ref36);

        ConditionResultTable crt1;
        list<list<Ref>> &input_table2 = crt1.GetTable();
        crt1.AddColDirectToTable(input_table2, Modifies);
        crt1.AddColDirectToTable(input_table2, Modifies1);

        crt.combine(input_table2);

        string expected = "[s, 1, 1, ]\n"
                          "[v, else, else, ]\n"
                          "[a, 17, 6, ]\n"
                          "";
        REQUIRE(expected == crt.PrintCustomTable(crt.GetTable()));
    }

    SECTION("Test Combine: Combine table with cross product1") {
        list<Ref> Uses;
        Ref ref11;
        createData.CreateDataSet(ref11, "a");
        Uses.push_back(ref11);

        Ref ref12;
        createData.CreateDataSet(ref12, "1");
        Uses.push_back(ref12);

        list<Ref> Uses1;
        Ref ref17;
        createData.CreateDataSet(ref17, "v");
        Uses1.push_back(ref17);

        Ref ref18;
        createData.CreateDataSet(ref18, "exact");
        Uses1.push_back(ref18);


        ConditionResultTable crt;
        list<list<Ref>> &input_table1 = crt.GetTable();
        crt.AddColDirectToTable(input_table1, Uses);
        crt.AddColDirectToTable(input_table1, Uses1);


        list<Ref> Modifies;
        Ref ref31;
        createData.CreateDataSet(ref31, "p");
        Modifies.push_back(ref31);

        Ref ref32;
        createData.CreateDataSet(ref32, "bonus");
        Modifies.push_back(ref32);

        list<Ref> Modifies1;
        Ref ref33;
        createData.CreateDataSet(ref33, "a");
        Modifies1.push_back(ref33);

        Ref ref34;
        createData.CreateDataSet(ref34, "1");
        Modifies1.push_back(ref34);

        ConditionResultTable crt1;
        list<list<Ref>> &input_table2 = crt1.GetTable();
        crt1.AddColDirectToTable(input_table2, Modifies);
        crt1.AddColDirectToTable(input_table2, Modifies1);

        crt.combine(input_table2);


        string expected = "[a, 1, ]\n"
                          "[v, exact, ]\n"
                          "[p, bonus, ]\n"
                          "";
        REQUIRE(expected == crt.PrintCustomTable(crt.GetTable()));
    }
}

TEST_CASE("Test Select Distinct Ref from Table") {
    ConditionResultTable createData;

    SECTION("Test Combine: Combine table with cross product first") {
        list<Ref> Uses;
        Ref ref11;
        createData.CreateDataSet(ref11, "a");
        Uses.push_back(ref11);

        Ref ref12;
        createData.CreateDataSet(ref12, "1");
        Uses.push_back(ref12);

        Ref ref13;
        createData.CreateDataSet(ref13, "1");
        Uses.push_back(ref13);

        Ref ref14;
        createData.CreateDataSet(ref14, "2");
        Uses.push_back(ref14);

        Ref ref15;
        createData.CreateDataSet(ref15, "2");
        Uses.push_back(ref15);

        Ref ref16;
        createData.CreateDataSet(ref16, "3");
        Uses.push_back(ref16);

        list<Ref> Uses1;
        Ref ref17;
        createData.CreateDataSet(ref17, "v");
        Uses1.push_back(ref17);

        Ref ref18;
        createData.CreateDataSet(ref18, "exact");
        Uses1.push_back(ref18);


        Ref ref19;
        createData.CreateDataSet(ref19, "exact");
        Uses1.push_back(ref19);

        Ref ref20;
        createData.CreateDataSet(ref20, "hey");
        Uses1.push_back(ref20);


        Ref ref21;
        createData.CreateDataSet(ref21, "hey");
        Uses1.push_back(ref21);

        Ref ref22;
        createData.CreateDataSet(ref22, "huh");
        Uses1.push_back(ref22);

        ConditionResultTable crt;
        list<list<Ref>> &input_table1 = crt.GetTable();
        crt.AddColDirectToTable(input_table1, Uses);
        crt.AddColDirectToTable(input_table1, Uses1);


        list<Ref> Modifies;
        Ref ref31;
        createData.CreateDataSet(ref31, "p");
        Modifies.push_back(ref31);

        Ref ref32;
        createData.CreateDataSet(ref32, "bonus");
        Modifies.push_back(ref32);

        Ref ref41;
        createData.CreateDataSet(ref41, "bonus");
        Modifies.push_back(ref41);

        Ref ref42;
        createData.CreateDataSet(ref42, "bonus");
        Modifies.push_back(ref42);

        list<Ref> Modifies1;
        Ref ref33;
        createData.CreateDataSet(ref33, "a");
        Modifies1.push_back(ref33);

        Ref ref34;
        createData.CreateDataSet(ref34, "1");
        Modifies1.push_back(ref34);

        Ref ref35;
        createData.CreateDataSet(ref35, "2");
        Modifies1.push_back(ref35);

        Ref ref36;
        createData.CreateDataSet(ref36, "3");
        Modifies1.push_back(ref36);

        ConditionResultTable crt1;
        list<list<Ref>> &input_table2 = crt1.GetTable();
        crt1.AddColDirectToTable(input_table2, Modifies);
        crt1.AddColDirectToTable(input_table2, Modifies1);

        crt.combine(input_table2);
        string expected = "[a, 1, 1, 2, 2, 3, ]\n"
                          "[v, exact, exact, hey, hey, huh, ]\n"
                          "[p, bonus, bonus, bonus, bonus, bonus, ]\n"
                          "";
        REQUIRE(expected == crt.PrintCustomTable(crt.GetTable()));
    }

    SECTION("Test Combine: Combine table with cross product, select one column") {
        list<Ref> Uses;
        Ref ref11;
        createData.CreateDataSet(ref11, "a");
        Uses.push_back(ref11);

        Ref ref12;
        createData.CreateDataSet(ref12, "1");
        Uses.push_back(ref12);

        Ref ref13;
        createData.CreateDataSet(ref13, "1");
        Uses.push_back(ref13);

        Ref ref14;
        createData.CreateDataSet(ref14, "2");
        Uses.push_back(ref14);

        Ref ref15;
        createData.CreateDataSet(ref15, "2");
        Uses.push_back(ref15);

        Ref ref16;
        createData.CreateDataSet(ref16, "3");
        Uses.push_back(ref16);

        list<Ref> Uses1;
        Ref ref17;
        createData.CreateDataSet(ref17, "v");
        Uses1.push_back(ref17);

        Ref ref18;
        createData.CreateDataSet(ref18, "exact");
        Uses1.push_back(ref18);


        Ref ref19;
        createData.CreateDataSet(ref19, "exact");
        Uses1.push_back(ref19);

        Ref ref20;
        createData.CreateDataSet(ref20, "hey");
        Uses1.push_back(ref20);


        Ref ref21;
        createData.CreateDataSet(ref21, "hey");
        Uses1.push_back(ref21);

        Ref ref22;
        createData.CreateDataSet(ref22, "huh");
        Uses1.push_back(ref22);

        ConditionResultTable crt;
        list<list<Ref>> &input_table1 = crt.GetTable();
        crt.AddColDirectToTable(input_table1, Uses);
        crt.AddColDirectToTable(input_table1, Uses1);


        list<Ref> Modifies;
        Ref ref31;
        createData.CreateDataSet(ref31, "p");
        Modifies.push_back(ref31);

        Ref ref32;
        createData.CreateDataSet(ref32, "bonus");
        Modifies.push_back(ref32);

        Ref ref41;
        createData.CreateDataSet(ref41, "bonus");
        Modifies.push_back(ref41);

        Ref ref42;
        createData.CreateDataSet(ref42, "bonus");
        Modifies.push_back(ref42);

        list<Ref> Modifies1;
        Ref ref33;
        createData.CreateDataSet(ref33, "a");
        Modifies1.push_back(ref33);

        Ref ref34;
        createData.CreateDataSet(ref34, "1");
        Modifies1.push_back(ref34);

        Ref ref35;
        createData.CreateDataSet(ref35, "2");
        Modifies1.push_back(ref35);

        Ref ref36;
        createData.CreateDataSet(ref36, "3");
        Modifies1.push_back(ref36);

        ConditionResultTable crt1;
        list<list<Ref>> &input_table2 = crt1.GetTable();
        crt1.AddColDirectToTable(input_table2, Modifies);
        crt1.AddColDirectToTable(input_table2, Modifies1);

        crt.combine(input_table2);

        list<Ref> col_needed;
        col_needed.push_back(ref11);
        list<list<Ref>> output_table = crt.SelectDistinctRef(col_needed, crt.GetTable());

        string expected = "[a, 1, 2, 3, ]\n"
                          "";
        REQUIRE(expected == crt.PrintCustomTable(output_table));
    }

    SECTION("Test Combine: Combine table with cross product, select two column") {
        list<Ref> Uses;
        Ref ref11;
        createData.CreateDataSet(ref11, "a");
        Uses.push_back(ref11);

        Ref ref12;
        createData.CreateDataSet(ref12, "1");
        Uses.push_back(ref12);

        Ref ref13;
        createData.CreateDataSet(ref13, "1");
        Uses.push_back(ref13);

        Ref ref14;
        createData.CreateDataSet(ref14, "2");
        Uses.push_back(ref14);

        Ref ref15;
        createData.CreateDataSet(ref15, "2");
        Uses.push_back(ref15);

        Ref ref16;
        createData.CreateDataSet(ref16, "3");
        Uses.push_back(ref16);

        list<Ref> Uses1;
        Ref ref17;
        createData.CreateDataSet(ref17, "v");
        Uses1.push_back(ref17);

        Ref ref18;
        createData.CreateDataSet(ref18, "exact");
        Uses1.push_back(ref18);


        Ref ref19;
        createData.CreateDataSet(ref19, "exact");
        Uses1.push_back(ref19);

        Ref ref20;
        createData.CreateDataSet(ref20, "hey");
        Uses1.push_back(ref20);


        Ref ref21;
        createData.CreateDataSet(ref21, "hey");
        Uses1.push_back(ref21);

        Ref ref22;
        createData.CreateDataSet(ref22, "huh");
        Uses1.push_back(ref22);

        ConditionResultTable crt;
        list<list<Ref>> &input_table1 = crt.GetTable();
        crt.AddColDirectToTable(input_table1, Uses);
        crt.AddColDirectToTable(input_table1, Uses1);


        list<Ref> Modifies;
        Ref ref31;
        createData.CreateDataSet(ref31, "p");
        Modifies.push_back(ref31);

        Ref ref32;
        createData.CreateDataSet(ref32, "bonus");
        Modifies.push_back(ref32);

        Ref ref41;
        createData.CreateDataSet(ref41, "bonus");
        Modifies.push_back(ref41);

        Ref ref42;
        createData.CreateDataSet(ref42, "bonus");
        Modifies.push_back(ref42);

        list<Ref> Modifies1;
        Ref ref33;
        createData.CreateDataSet(ref33, "a");
        Modifies1.push_back(ref33);

        Ref ref34;
        createData.CreateDataSet(ref34, "1");
        Modifies1.push_back(ref34);

        Ref ref35;
        createData.CreateDataSet(ref35, "2");
        Modifies1.push_back(ref35);

        Ref ref36;
        createData.CreateDataSet(ref36, "3");
        Modifies1.push_back(ref36);

        ConditionResultTable crt1;
        list<list<Ref>> &input_table2 = crt1.GetTable();
        crt1.AddColDirectToTable(input_table2, Modifies);
        crt1.AddColDirectToTable(input_table2, Modifies1);

        crt.combine(input_table2);

        list<Ref> col_needed;
        col_needed.push_back(ref11);
        col_needed.push_back(ref17);
        list<list<Ref>> output_table = crt.SelectDistinctRef(col_needed, crt.GetTable());

        string expected = "[a, 1, 2, 3, ]\n"
                          "[v, exact, hey, huh, ]\n"
                          "";
        REQUIRE(expected == crt.PrintCustomTable(output_table));
    }


    SECTION("Test Combine: Combine table with cross product, select two column random value") {
        list<Ref> Uses;
        Ref ref11;
        createData.CreateDataSet(ref11, "a");
        Uses.push_back(ref11);

        Ref ref12;
        createData.CreateDataSet(ref12, "1");
        Uses.push_back(ref12);

        Ref ref13;
        createData.CreateDataSet(ref13, "7");
        Uses.push_back(ref13);

        Ref ref14;
        createData.CreateDataSet(ref14, "3");
        Uses.push_back(ref14);

        Ref ref15;
        createData.CreateDataSet(ref15, "1");
        Uses.push_back(ref15);

        Ref ref16;
        createData.CreateDataSet(ref16, "7");
        Uses.push_back(ref16);

        list<Ref> Uses1;
        Ref ref17;
        createData.CreateDataSet(ref17, "v");
        Uses1.push_back(ref17);

        Ref ref18;
        createData.CreateDataSet(ref18, "hey");
        Uses1.push_back(ref18);


        Ref ref19;
        createData.CreateDataSet(ref19, "what");
        Uses1.push_back(ref19);

        Ref ref20;
        createData.CreateDataSet(ref20, "huh");
        Uses1.push_back(ref20);


        Ref ref21;
        createData.CreateDataSet(ref21, "hey");
        Uses1.push_back(ref21);

        Ref ref22;
        createData.CreateDataSet(ref22, "huh");
        Uses1.push_back(ref22);

        ConditionResultTable crt;
        list<list<Ref>> &input_table1 = crt.GetTable();
        crt.AddColDirectToTable(input_table1, Uses);
        crt.AddColDirectToTable(input_table1, Uses1);


        list<Ref> Modifies;
        Ref ref31;
        createData.CreateDataSet(ref31, "p");
        Modifies.push_back(ref31);

        Ref ref32;
        createData.CreateDataSet(ref32, "bonus");
        Modifies.push_back(ref32);

        Ref ref41;
        createData.CreateDataSet(ref41, "bonus");
        Modifies.push_back(ref41);

        Ref ref42;
        createData.CreateDataSet(ref42, "bonus");
        Modifies.push_back(ref42);

        list<Ref> Modifies1;
        Ref ref33;
        createData.CreateDataSet(ref33, "a");
        Modifies1.push_back(ref33);

        Ref ref34;
        createData.CreateDataSet(ref34, "7");
        Modifies1.push_back(ref34);

        Ref ref35;
        createData.CreateDataSet(ref35, "1");
        Modifies1.push_back(ref35);

        Ref ref36;
        createData.CreateDataSet(ref36, "3");
        Modifies1.push_back(ref36);

        ConditionResultTable crt1;
        list<list<Ref>> &input_table2 = crt1.GetTable();
        crt1.AddColDirectToTable(input_table2, Modifies);
        crt1.AddColDirectToTable(input_table2, Modifies1);

        crt.combine(input_table2);


        list<Ref> col_needed;
        col_needed.push_back(ref11);
        col_needed.push_back(ref17);
        list<list<Ref>> output_table = crt.SelectDistinctRef(col_needed, crt.GetTable());

        string expected = "[a, 1, 7, 3, 7, ]\n"
                          "[v, hey, what, huh, huh, ]\n"
                          "";
        REQUIRE(expected == crt.PrintCustomTable(output_table));
    }

    SECTION("Test Combine: Combine table with cross product, select two column random value + duplicates") {
        list<Ref> Uses;
        Ref ref11;
        createData.CreateDataSet(ref11, "a");
        Uses.push_back(ref11);

        Ref ref12;
        createData.CreateDataSet(ref12, "1");
        Uses.push_back(ref12);

        Ref ref13;
        createData.CreateDataSet(ref13, "7");
        Uses.push_back(ref13);

        Ref ref14;
        createData.CreateDataSet(ref14, "3");
        Uses.push_back(ref14);

        Ref ref15;
        createData.CreateDataSet(ref15, "1");
        Uses.push_back(ref15);

        Ref ref16;
        createData.CreateDataSet(ref16, "7");
        Uses.push_back(ref16);

        list<Ref> Uses1;
        Ref ref17;
        createData.CreateDataSet(ref17, "v");
        Uses1.push_back(ref17);

        Ref ref18;
        createData.CreateDataSet(ref18, "hey");
        Uses1.push_back(ref18);


        Ref ref19;
        createData.CreateDataSet(ref19, "what");
        Uses1.push_back(ref19);

        Ref ref20;
        createData.CreateDataSet(ref20, "huh");
        Uses1.push_back(ref20);


        Ref ref21;
        createData.CreateDataSet(ref21, "hey");
        Uses1.push_back(ref21);

        Ref ref22;
        createData.CreateDataSet(ref22, "huh");
        Uses1.push_back(ref22);

        ConditionResultTable crt;
        list<list<Ref>> &input_table1 = crt.GetTable();
        crt.AddColDirectToTable(input_table1, Uses);
        crt.AddColDirectToTable(input_table1, Uses1);


        list<Ref> Modifies;
        Ref ref31;
        createData.CreateDataSet(ref31, "p");
        Modifies.push_back(ref31);

        Ref ref32;
        createData.CreateDataSet(ref32, "bala");
        Modifies.push_back(ref32);

        Ref ref41;
        createData.CreateDataSet(ref41, "extra");
        Modifies.push_back(ref41);

        Ref ref42;
        createData.CreateDataSet(ref42, "yessss");
        Modifies.push_back(ref42);

        list<Ref> Modifies1;
        Ref ref33;
        createData.CreateDataSet(ref33, "a");
        Modifies1.push_back(ref33);

        Ref ref34;
        createData.CreateDataSet(ref34, "7");
        Modifies1.push_back(ref34);

        Ref ref35;
        createData.CreateDataSet(ref35, "7");
        Modifies1.push_back(ref35);

        Ref ref36;
        createData.CreateDataSet(ref36, "3");
        Modifies1.push_back(ref36);

        ConditionResultTable crt1;
        list<list<Ref>> &input_table2 = crt1.GetTable();
        crt1.AddColDirectToTable(input_table2, Modifies);
        crt1.AddColDirectToTable(input_table2, Modifies1);

        crt.combine(input_table2);


        list<Ref> col_needed;
        col_needed.push_back(ref11);
        col_needed.push_back(ref17);
        list<list<Ref>> output_table = crt.SelectDistinctRef(col_needed, crt.GetTable());

        string expected = "[a, 7, 3, 7, ]\n"
                          "[v, what, huh, huh, ]\n"
                          "";
        REQUIRE(expected == crt.PrintCustomTable(output_table));
    }

    SECTION("Test Combine: Distinct Attribute more rows") {
        list<Ref> Uses;
        Ref ref11;
        createData.CreateDataSet(ref11, "s3");
        Uses.push_back(ref11);

        Ref ref12;
        createData.CreateDataSet(ref12, "5");
        Uses.push_back(ref12);

        Ref ref13;
        createData.CreateDataSet(ref13, "5");
        Uses.push_back(ref13);

        Ref ref14;
        createData.CreateDataSet(ref14, "7");
        Uses.push_back(ref14);

        Ref ref15;
        createData.CreateDataSet(ref15, "8");
        Uses.push_back(ref15);

        Ref ref16;
        createData.CreateDataSet(ref16, "2");
        Uses.push_back(ref16);


        list<Ref> Uses1;
        Ref ref17;
        createData.CreateDataSet(ref17, "v1");
        Uses1.push_back(ref17);

        Ref ref18;
        createData.CreateDataSet(ref18, "y");
        Uses1.push_back(ref18);

        Ref ref19;
        createData.CreateDataSet(ref19, "z");
        Uses1.push_back(ref19);

        Ref ref20;
        createData.CreateDataSet(ref20, "x");
        Uses1.push_back(ref20);

        Ref ref21;
        createData.CreateDataSet(ref21, "y");
        Uses1.push_back(ref21);

        Ref ref22;
        createData.CreateDataSet(ref22, "y");
        Uses1.push_back(ref22);

        ConditionResultTable crt;
        list<list<Ref>> &input_table1 = crt.GetTable();
        crt.AddColDirectToTable(input_table1, Uses);
        crt.AddColDirectToTable(input_table1, Uses1);


        list<Ref> Modifies;
        Ref ref31;
        createData.CreateDataSet(ref31, "s3");
        Modifies.push_back(ref31);

        Ref ref32;
        createData.CreateDataSet(ref32, "6");
        Modifies.push_back(ref32);

        Ref ref33;
        createData.CreateDataSet(ref33, "5");
        Modifies.push_back(ref33);

        Ref ref34;
        createData.CreateDataSet(ref34, "7");
        Modifies.push_back(ref34);


        ConditionResultTable crt1;
        list<list<Ref>> &input_table2 = crt1.GetTable();
        crt1.AddColDirectToTable(input_table2, Modifies);

        crt.combine(input_table2);

        list<Ref> Follows;
        Ref ref41;
        createData.CreateDataSet(ref41, "s1");
        Follows.push_back(ref41);

        Ref ref42;
        createData.CreateDataSet(ref42, "6");
        Follows.push_back(ref42);

        Ref ref43;
        createData.CreateDataSet(ref43, "3");
        Follows.push_back(ref43);

        Ref ref44;
        createData.CreateDataSet(ref44, "10");
        Follows.push_back(ref44);

        Ref ref45;
        createData.CreateDataSet(ref45, "8");
        Follows.push_back(ref45);


        list<Ref> Follows1;
        Ref ref46;
        createData.CreateDataSet(ref46, "s2");
        Follows1.push_back(ref46);

        Ref ref49;
        createData.CreateDataSet(ref49, "20");
        Follows1.push_back(ref49);

        Ref ref50;
        createData.CreateDataSet(ref50, "4");
        Follows1.push_back(ref50);

        Ref ref47;
        createData.CreateDataSet(ref47, "15");
        Follows1.push_back(ref47);

        Ref ref48;
        createData.CreateDataSet(ref48, "11");
        Follows1.push_back(ref48);

        ConditionResultTable crt2;
        list<list<Ref>> &input_table3 = crt2.GetTable();
        crt2.AddColDirectToTable(input_table3, Follows);
        crt2.AddColDirectToTable(input_table3, Follows1);

        crt.combine(input_table3);

        list<Ref> col_needed;
        col_needed.push_back(ref31);
        col_needed.push_back(ref41);
        list<list<Ref>> output_table = crt.SelectDistinctRef(col_needed, crt.GetTable());
        string expected = "[s3, 5, 7, 5, 7, 5, 7, 5, 7, ]\n"
                          "[s1, 6, 6, 3, 3, 10, 10, 8, 8, ]\n"
                          "";
        REQUIRE(expected == crt.PrintCustomTable(output_table));
    }

    SECTION("Test Combine: Common Attribute complicated") {
        list<Ref> Uses;
        Ref ref11;
        createData.CreateDataSet(ref11, "s3");
        Uses.push_back(ref11);

        Ref ref12;
        createData.CreateDataSet(ref12, "5");
        Uses.push_back(ref12);

        Ref ref13;
        createData.CreateDataSet(ref13, "5");
        Uses.push_back(ref13);

        Ref ref14;
        createData.CreateDataSet(ref14, "7");
        Uses.push_back(ref14);

        Ref ref15;
        createData.CreateDataSet(ref15, "8");
        Uses.push_back(ref15);

        Ref ref16;
        createData.CreateDataSet(ref16, "2");
        Uses.push_back(ref16);


        list<Ref> Uses1;
        Ref ref17;
        createData.CreateDataSet(ref17, "v1");
        Uses1.push_back(ref17);

        Ref ref18;
        createData.CreateDataSet(ref18, "y");
        Uses1.push_back(ref18);

        Ref ref19;
        createData.CreateDataSet(ref19, "z");
        Uses1.push_back(ref19);

        Ref ref20;
        createData.CreateDataSet(ref20, "x");
        Uses1.push_back(ref20);

        Ref ref21;
        createData.CreateDataSet(ref21, "y");
        Uses1.push_back(ref21);

        Ref ref22;
        createData.CreateDataSet(ref22, "y");
        Uses1.push_back(ref22);

        ConditionResultTable crt;
        list<list<Ref>> &input_table1 = crt.GetTable();
        crt.AddColDirectToTable(input_table1, Uses);
        crt.AddColDirectToTable(input_table1, Uses1);


        list<Ref> Modifies;
        Ref ref31;
        createData.CreateDataSet(ref31, "s3");
        Modifies.push_back(ref31);

        Ref ref32;
        createData.CreateDataSet(ref32, "6");
        Modifies.push_back(ref32);

        Ref ref33;
        createData.CreateDataSet(ref33, "5");
        Modifies.push_back(ref33);

        Ref ref34;
        createData.CreateDataSet(ref34, "7");
        Modifies.push_back(ref34);


        ConditionResultTable crt1;
        list<list<Ref>> &input_table2 = crt1.GetTable();
        crt1.AddColDirectToTable(input_table2, Modifies);

        crt.combine(input_table2);
        crt.RemoveDuplicatesInTable(crt.GetTable());

        list<Ref> col_needed;
        col_needed.push_back(ref31);
        list<list<Ref>> output_table = crt.SelectDistinctRef(col_needed, crt.GetTable());
        string expected = "[s3, 5, 7, ]\n"
                          "";
        REQUIRE(expected == crt.PrintCustomTable(output_table));
    }
}