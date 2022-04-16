
#include "RelationResultPair.h"

RelationResultPair::RelationResultPair(Ref arg1, Ref arg2) {
    argument1 = arg1;
    argument2 = arg2;
}

bool RelationResultPair::equals(RelationResultPair* other) {
    return other->arg1Equals(&argument1) && other->arg2Equals(&argument2);
}

bool RelationResultPair::operator==(const RelationResultPair & other) const {
    return this->argument1 == other.argument1 && this->argument2 == other.argument2;
}

bool RelationResultPair::arg1Equals(Ref* otherArg1) {
    return argument1.equals(otherArg1);
}

bool RelationResultPair::arg2Equals(Ref* otherArg2) {
    return argument2.equals(otherArg2);
}

string RelationResultPair::ToString() {
    return "<" + argument1.ToString() + ", " + argument2.ToString() + ">";
}
