#ifndef INC_21S2_CP_SPA_TEAM_37_PROCEDURE_H
#define INC_21S2_CP_SPA_TEAM_37_PROCEDURE_H

#include <string>
#include <vector>
#include "./ADT.h"
#include "StatementNumber.h"

using namespace std;

class Procedure {
private:
	ProcedureName name = "";
	int start = 0;
	int end = 0;
	ProcedureIndex index;
	bool finished_assign = false;
	vector<StatementNumber> assigns;

public:
	Procedure() {}
	Procedure(const ProcedureName& name);
	Procedure(const ProcedureName& name, int start, int end);
	void SetDetails(int start, int end);
	ProcedureName GetName();
	ProcedureName* GetNamePointer();
	bool IsWithin(int n);
	int GetStart();
	int GetEnd();
	ProcedureIndex GetIndex();
	bool IsAssignFinished() const;
	void AddAssign(StatementNumber);
	const vector<StatementNumber>& GetAssigns() const;
};

struct hashProcedureName {
public:
	size_t operator() (const ProcedureName& pn) const {
		return std::hash<std::string>()(pn);
	}
};

#endif //INC_21S2_CP_SPA_TEAM_37_PROCEDURE_H