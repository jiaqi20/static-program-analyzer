
#include "WithResultTable.h"

WithResultTable::WithResultTable(){

}

WithResultTable::WithResultTable(Ref LHS, Ref RHS) {
    this->lhs = LHS;
    this->rhs = RHS;
}

WithResultTable::WithResultTable(Ref LHS, Ref RHS, vector<pair<Ref, Ref>> listIn) {
    this->lhs = LHS;
    this->rhs = RHS;
    this->list = listIn;
}

Ref* WithResultTable::getLHS() {
    return &lhs;
}

Ref* WithResultTable::getRHS() {
    return &rhs;
}

vector<pair<Ref, Ref>>* WithResultTable::getList() {
    return &list;
}

int WithResultTable::size() {
    return this->list.size();
}

string WithResultTable::ToString() {
    string result = "<arg2: " + lhs.ToString() + " , rhs: " + rhs.ToString() + ", list: ";
    for (auto & iter : list) {
        result += "<" + iter.first.ToString() + ", " + iter.second.ToString() + ">, ";
    }
    return result + ">";
}