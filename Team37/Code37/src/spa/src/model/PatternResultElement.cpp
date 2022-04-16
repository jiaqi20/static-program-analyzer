
#include "PatternResultElement.h"

PatternResultElement::PatternResultElement(Ref assignIn, Ref LHSIn, ExpressionSpec expressionIn) {
    assign_stmt = assignIn;
    lhs = LHSIn;
    expression = expressionIn;
}

bool PatternResultElement::equals(PatternResultElement* other) {
    return other->AssignEquals(&assign_stmt) && other->LhsEquals(&lhs) && other->ExpressionEquals(&expression);
}

bool PatternResultElement::operator==(const PatternResultElement & other) const {
    return this->assign_stmt == other.assign_stmt && this->lhs == other.lhs && this->expression == other.expression;
}

bool PatternResultElement::AssignEquals(Ref* otherAssign) {
    return otherAssign->equals(&assign_stmt);
}
bool PatternResultElement::LhsEquals(Ref* otherLHS) {
    return otherLHS->equals(&lhs);
}

bool PatternResultElement::ExpressionEquals(ExpressionSpec* otherExpression) {
    return otherExpression->equals(&expression);
}

string PatternResultElement::ToString() {
    return "<" + assign_stmt.ToString() + ", " + lhs.ToString() + ", " + expression.ToString() + ">";
}