
#ifndef INC_21S2_CP_SPA_TEAM_37_RELATIONRESULTPAIR_H
#define INC_21S2_CP_SPA_TEAM_37_RELATIONRESULTPAIR_H

#include "QType.h"

using namespace std;

class RelationResultPair {
public:
    Ref argument1;
    Ref argument2;
    RelationResultPair(Ref arg1, Ref arg2);
    bool equals(RelationResultPair* other);
    bool operator==(const RelationResultPair & other) const;
    bool arg1Equals(Ref* otherArg1);
    bool arg2Equals(Ref* otherArg2);
    string ToString();
};


#endif //INC_21S2_CP_SPA_TEAM_37_RELATIONRESULTPAIR_H
