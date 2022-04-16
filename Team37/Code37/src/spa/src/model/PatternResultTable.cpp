
#include "PatternResultTable.h"
#include "iostream"

PatternResultTable::PatternResultTable() {}

PatternResultTable::PatternResultTable(Ref queryingAssignIn, Ref queryingLHSIn, vector<PatternResultElement> listIn) {
    querying_arg1 = queryingAssignIn;
    querying_arg2 = queryingLHSIn;
    list = listIn;
}

PatternResultTable::PatternResultTable(Ref queryingAssignIn, Ref queryingLHSIn) {
    querying_arg1 = queryingAssignIn;
    querying_arg2 = queryingLHSIn;
}

void PatternResultTable::add(PatternResultElement newData) {
    list.push_back(newData);
}

vector<PatternResultElement>* PatternResultTable::getList() {
    return &list;
}

Ref* PatternResultTable::getQueryingAssign() {
    return &querying_arg1;
}

Ref* PatternResultTable::getQueryingLHS() {
    return &querying_arg2;
}

ExpressionSpec* PatternResultTable::getQueryingExpression() {
    return &querying_expression;
}

bool PatternResultTable::ContainsAssignJustValue(Ref* checkingAssign) {
    for (auto & iter : list) {
        if (iter.AssignEquals(checkingAssign)) {
            return true;
        }
    }
    return false;
}

bool PatternResultTable::containsAssignJustValue(Ref *checkingAssign) {
    for (auto & iter : list) {
        if (iter.assign_stmt.getName()->equals(checkingAssign->getName())) {
            return true;
        }
    }
    return false;
}

bool PatternResultTable::ContainsLHS(Ref* checkingLHS) {
    for (auto & iter : list) {
        if (iter.LhsEquals(checkingLHS)) {
            return true;
        }
    }
    return false;
}

bool PatternResultTable::ContainsElement(PatternResultElement* element) {
    for (auto & iter : list) {
        if (iter.equals(element)) {
            return true;
        }
    }
    return false;
}

bool PatternResultTable::EqualsAssign(Ref* checking) {
    return querying_arg1.equals(checking);
}

bool PatternResultTable::EqualsLHS(Ref* checking) {
    return querying_arg2.equals(checking);
}

bool PatternResultTable::EqualsExpression(ExpressionSpec *checking) {
    return querying_expression.equals(checking);
}

int PatternResultTable::size() {
    return list.size();
}

string PatternResultTable::ToString() {
    string result = "<assign: " + querying_arg1.ToString() + " arg2: " + querying_arg2.ToString() + " expression: " + querying_expression.ToString() + " list: ";
    for (auto &iter : list) {
        result += iter.ToString() + ", ";
    }
    return result + ">";
}