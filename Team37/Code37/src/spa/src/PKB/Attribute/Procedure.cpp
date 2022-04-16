#include "Procedure.h"

Procedure:: Procedure(const ProcedureName& name, int start, int end) {
	this->name = name;
	this->start = start;
	this->end = end;
	this->index = hashProcedureName()(name);
}

Procedure:: Procedure(const ProcedureName& name) {
	this->name = name;
	this->index = hashProcedureName()(name);
}

void Procedure::SetDetails(int start, int end) {
	this->start = start;
	this->end = end;
}

ProcedureName Procedure::GetName() {
	return this->name;
}

ProcedureName* Procedure::GetNamePointer() {
	return &(this->name);
}

bool Procedure::IsWithin(int n)
{
	return n >= start && n <= end;
}

int Procedure::GetStart() {
	return this->start;  
}

int Procedure::GetEnd() {
	return this->end;
}

ProcedureIndex Procedure::GetIndex() {
	return this->index;
}

bool Procedure::IsAssignFinished() const {
	return this->finished_assign;
}

void Procedure::AddAssign(StatementNumber s) {
	this->assigns.push_back(s);
}

const vector<StatementNumber>& Procedure::GetAssigns() const {
	return assigns;
}