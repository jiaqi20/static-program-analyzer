
#include "RelationResultTable.h"
#include "iostream"

RelationResultTable::RelationResultTable() {

}
RelationResultTable::RelationResultTable(RelationType typeIn, Ref queryingArg1, Ref queryingArg2, vector<RelationResultPair> listIn) {
    type = typeIn;
    list = listIn;
    queryingArgument1 = queryingArg1;
    queryingArgument2 = queryingArg2;
}

RelationResultTable::RelationResultTable(RelationType typeIn, Ref queryingArg1, Ref queryingArg2) {
    type = typeIn;
    queryingArgument1 = queryingArg1;
    queryingArgument2 = queryingArg2;
}

void RelationResultTable::add(RelationResultPair newData) {
    list.push_back(newData);
}

vector<RelationResultPair>* RelationResultTable::getList(){
    return &list;
}

Ref* RelationResultTable::getQueryingArg1() {
    return &queryingArgument1;
}

Ref* RelationResultTable::getQueryingArg2() {
    return &queryingArgument2;
}

bool RelationResultTable::containsPair(RelationResultPair* pair) {
    for (auto & iter : list) {
        if (iter.equals(pair)){
            return true;
        }
    }
    return false;
}

bool RelationResultTable::containsArg1(Ref* arg) {
    for (auto & iter : list) {
        if (iter.arg1Equals(arg)){
            return true;
        }
    }
    return false;
}

bool RelationResultTable::containsArg1JustValue(Ref* arg) {
    for (auto & iter : list) {
        if (iter.argument1.getName()->equals(arg->getName())){
            return true;
        }
    }
    return false;
}

bool RelationResultTable::containsArg2(Ref* arg) {
    for (auto & iter : list) {
        if (iter.arg2Equals(arg)){
            return true;
        }
    }
    return false;
}

bool RelationResultTable::containsArg2JustValue(Ref* arg) {
    for (auto & iter : list) {
        if (iter.argument2.getName()->equals(arg->getName())){
            return true;
        }
    }
    return false;
}

bool RelationResultTable::equalsArg1(Ref* checkingSelected) {
    return queryingArgument1.equals(checkingSelected);
}

bool RelationResultTable::equalsArg2(Ref* checkingSelected) {
    return queryingArgument2.equals(checkingSelected);
}

int RelationResultTable::size() {
    return list.size();
}

string RelationResultTable::ToString() {
    string result = "<arg1: " + queryingArgument1.ToString() + ", arg2: " + queryingArgument2.ToString() + " list: ";
    for(auto & iter : list) {
        result += iter.ToString() + ", ";
    }
    return result + ">";
}


