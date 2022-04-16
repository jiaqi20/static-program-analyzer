#ifndef INC_21S2_CP_SPA_TEAM_37_LMT_H
#define INC_21S2_CP_SPA_TEAM_37_LMT_H

#include <unordered_map>
#include <unordered_set>
#include <algorithm>

#include "StatementNumber.h"
#include "./ADT.h"

using namespace std;

// This class is a helper class for Affects: latest modified table
class LMT {
public:
	friend void MergeTwoLMTs(const LMT& lmt1, const LMT& lmt2, LMT& result);

	unordered_map<VarIndex, unordered_set<StatementNumber, hashStatementNumber>*, hash<VarIndex>> table;

	LMT();
	~LMT();
	LMT(const LMT& obj);

	bool operator==(const LMT& obj) const;
	void Add(const VarIndex& vi, const StatementNumber& sn);
	bool IsKeyExist(const VarIndex& vi) const;
	void MergeInto(const LMT& lmt);
	void CopyFrom(const LMT& lmt);
	bool Contains(const LMT& obj) const;
	unordered_set<StatementNumber, hashStatementNumber>* GetLastModifiesStatements(const VarIndex& vi);
};

#endif //INC_21S2_CP_SPA_TEAM_37_LMT_H