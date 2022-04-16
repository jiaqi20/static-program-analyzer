#include <iostream>
#include <string>
using namespace std;

#include "VarName.h"

VarName::VarName() {
    this->name = "";
}

VarName::VarName(string name) {
    this->name = name;
}

VarName::VarName(const VarName& obj) {
    name = obj.name;
}

VarName::~VarName() {

}

bool VarName::IsValid() const {
    return !this->name.empty();
}

string VarName::GetName() const {
    if (IsValid()) {
        return this->name;
    } else {
        return std::string();
    }
}

const VarName VarName::INVALID_NAME() {
    return VarName("");
}

void VarName::ToString() {
    cout << "Variable Name is: " << this -> name << endl;
}

bool VarName::operator==(const VarName& other) const {
    return this->name == other.GetName();
}

bool VarName::operator<(const VarName& other) const {
    return this->name < other.GetName();
}