#include "Query.h"
#include "QType.h"
#include <vector>

RefValue::RefValue() {}

RefValue::RefValue(ValueType typeIn) {
    type = typeIn;
}

RefValue::RefValue(ValueType typeIn, string nameIn) {
    type = typeIn;
    name = nameIn;
}

RefValue::RefValue(ValueType typeIn, int numberIn) {
    type = typeIn;
    name = to_string(numberIn);
}

ValueType RefValue::GetType() {
    return type;
}

string RefValue::GetName() {
    return name;
}

int RefValue::GetValue() {
    if (type == kNumber) {
        return stoi(name);
    } else {
        return -1;
    }
}

void RefValue::SetValue(int &valueIn) {
    name = to_string(valueIn);
}

void RefValue::SetType(ValueType& typeIn) {
    type = typeIn;
}

void RefValue::SetName(string& nameIn) {
    name = nameIn;
}

string RefValue::ToString() {
    if (type == kSynonym || type == kConstantStr || type == kNumber || type == kName) {
        return name;
    } else if (type == kWildcard) {
        return "_";
    } else if (type == kFixedValue) {
        return "\"" + name + "\"";
    } else {
        return "INVALID_VAL";
    }
}

string RefValue::PrintResult() {
    if (type == kSynonym || type == kConstantStr || type == kFixedValue || type == kName || type == kNumber) {
        return name;
    } else if (type == kWildcard) {
        return "_";
    } else {
        return "INVALID_VAL";
    }
}

bool RefValue::TypeEquals(const RefValue *other) const {
    return type == (other->type);
}

bool RefValue::equals(RefValue *other) {
    if (TypeEquals(other)) {
        if (type == kWildcard) {
            return true;
        } else if (type == kSynonym || type == kFixedValue || type == kConstantStr || type == kNumber) {
            return (other->name) == name;
        }
    } else {
        return false;
    }
}

bool RefValue::operator==(const RefValue & other) const {
    if (TypeEquals(&other)) {
        if (type == kWildcard) {
            return true;
        } else if (type == kSynonym || type == kFixedValue || type == kConstantStr || type == kNumber) {
            return (other.name) == name;
        }
    } else {
        return false;
    }
}

Ref::Ref() {}

Ref::Ref(EntType typeIn, RefValue nameIn) {
    type = typeIn;
    name = nameIn;
}

Ref::Ref(vector<Ref> &tuple) {
    this->type = kTupleType;
    this->refList = tuple;
}

string Ref::ToString() {
    if (IsTuple()) {
        string result = "<";
        for (auto & iter : refList) {
            result += iter.ToString() + ", ";
        }
        return result + ">";
    } else {
        if (attributes.GetName() == "") {
             return name.ToString();
        } else {
            return name.ToString() + "." + attributes.ToString();
        }

    }
}

void Ref::EraseRefAttributes() {
    string name = "";
    attributes.SetName(name);
}

string Ref::PrintResult() {
    if (IsTuple()) {
        string result = "";
        auto iterRef = refList.begin();
        while (iterRef != refList.end()) {
            result += iterRef->PrintResult();
            iterRef++;
            if (iterRef != refList.end()) {
                result += " ";
            }
        }
        return result;
    } else {
        return name.PrintResult();
    }
}

string Ref::SelectToString() {
    string select = "< ";
    EntType et1 = kTupleType;

    if (getName()->GetType() == kBoolean) {
        return "BOOLEAN";
    }
    if (GetType() == kTupleType) {
        for (auto& it : refList) {
            select = select + it.ToString() + " ";
        }
        select = select + ">";
        return select;
    } else {
        if (attributes.GetName() == "") {
            return name.ToString();
        } else {
            return name.ToString() + "." + attributes.ToString();
        }
    }
}

string Ref::WithToString() {
    if (attributes.ToString() != "") {
        return name.ToString() + "." + attributes.ToString();
    } else {
        return name.ToString();
    }
}

EntType Ref::GetType() {
    return type;
}

RefValue* Ref::getName() {
    return &name;
}

RefValue* Ref::getAttribute() {
    return &attributes;
}

void Ref::SetType(EntType typeIn) {
    type = typeIn;
}

void Ref::SetName(RefValue& nameIn) {
    name = nameIn;
}

void Ref::SetAttribute(RefValue &attributeIn) {
    attributes = attributeIn;
}

bool Ref::IsStatement() {
    return type == kStmt
        || type == kRead
        || type == kPrint
        || type == kCall
        || type == kWhile
        || type == kIf
        || type == kAssign;
}

bool Ref::IsVariable() {
    return type == kVariable;
}

bool Ref::IsConstant() {
    return type == kConstant;
}

bool Ref::IsProcedure() {
    return type == kProcedure;
}

bool Ref::IsEntity() {
    return IsConstant() || IsStatement() || IsProcedure() || IsVariable();
}

bool Ref::IsTuple() {
    return type == kTupleType;
}

bool Ref::IsBoolean() {
    return type == kBooleanType;
}

bool Ref::equals(Ref *other) {
    return other->GetType() == type && other->getName()->equals(&name);
}

bool Ref::operator==(const Ref & other) const {
    return this->type == other.type && this->name == other.name;
}

RelationType Relation::getRelationType() {
    return type;
}

Ref* Relation::GetArgument1() {
    return &argument1;
}

Ref* Relation::GetArgument2() {
    return &argument2;
}

void Relation::SetType(RelationType typeIn) {
    type = typeIn;
}

void Relation::SetArgument1(Ref& arg1In) {
    argument1 = arg1In;
}

void Relation::SetArgument2(Ref& arg2In) {
    argument2 = arg2In;
}

bool Relation::equals(Relation *other) {
    return getRelationType() == other->getRelationType() && argument1.equals(other->GetArgument1()) && argument2.equals(
            other->GetArgument2());
}

string Relation::ToString() {
    if (type == kFollows) {
        return "Follows(" + argument1.ToString() + ", " + argument2.ToString() + ")";
    } else if (type == kFollowst) {
        return "Follows*(" + argument1.ToString() + ", " + argument2.ToString() + ")";
    } else if (type == kParent) {
        return "Parent(" + argument1.ToString() + ", " + argument2.ToString() + ")";
    } else if (type == kParentt){
        return "Parent*(" + argument1.ToString() + ", " + argument2.ToString() + ")";
    } else if (type == kUsess || type == kUsesp) {
        return "Uses(" + argument1.ToString() + ", " + argument2.ToString() + ")";
    } else if (type == kModifiess || type == kModifiesp) {
        return "Modifies(" + argument1.ToString() + ", " + argument2.ToString() + ")";
    } else if (type == kCalls) {
        return "Calls(" + argument1.ToString() + ", " + argument2.ToString() + ")";
    } else if (type == kCallst) {
        return "Calls*(" + argument1.ToString() + ", " + argument2.ToString() + ")";
    } else if (type == kNext) {
        return "Next(" + argument1.ToString() + ", " + argument2.ToString() + ")";
    } else if (type == kNextt) {
        return "Next*(" + argument1.ToString() + ", " + argument2.ToString() + ")";
    } else if (type == kAffects) {
        return "Affects(" + argument1.ToString() + ", " + argument2.ToString() + ")";
    } else if (type == kAffectst) {
        return "Affects*(" + argument1.ToString() + ", " + argument2.ToString() + ")";
    } else {
        return "INVALID_RELATION";
    }
}

string ExpressionSpec::GetExpression() {
    return this->expression;
}
string ExpressionSpec::ToString() {
    if (type == kFullMatch) {
        return "\"" + GetExpression() + "\"";
    } else if (type == kPartialMatch) {
        return "_\"" + GetExpression() + "\"_";
    } else if (type == kAnyMatch) {
        return "_";
    } else {
        return "INVALID EXPRESSIONSPEC";
    }
}

ExpressionSpec::ExpressionSpec(ExpressionSpecType &typeIn, string &expressionIn) {
    expression = expressionIn;
    type = typeIn;
}

ExpressionSpec::ExpressionSpec() {
}

void ExpressionSpec::SetExpression(string &expressionIn) {
    expression = expressionIn;
}

void ExpressionSpec::SetType(ExpressionSpecType &typeIn) {
    type = typeIn;
}

ExpressionSpecType ExpressionSpec::GetType() {
    return type;
}

bool ExpressionSpec::equals(ExpressionSpec* otherExpression) {
    return (type == otherExpression->GetType()) && (expression == otherExpression->GetExpression());
}

bool ExpressionSpec::operator==(const ExpressionSpec & other) const {
    return this->type == other.type && this->expression == other.expression;
}

Ref* Pattern::GetArg1() {
    return &arg1;
}

Ref* Pattern::GetArg2() {
    return &arg2;
}

ExpressionSpec* Pattern::GetExpression() {
    return &expression_spec;
}

string Pattern::ToString() {
    return "pattern " + arg1.ToString() + "(" + arg2.ToString() + ", " + expression_spec.ToString() + ")";
}

vector<Ref>* Ref::GetTuple() {
    return &refList;
}

void Ref::AddRefValue(Ref &nameIn) {
    this->refList.push_back(nameIn);
}

Ref* With::GetArgument1() {
    return &argument1;
}

Ref* With::GetArgument2() {
    return &argument2;
}

void With::SetArgument1(Ref& arg1In) {
    argument1 = arg1In;
}

void With::SetArgument2(Ref& arg2In) {
    argument2 = arg2In;
}

string With::ToString() {
    return "with " + argument1.WithToString() + " = " + argument2.WithToString();
}