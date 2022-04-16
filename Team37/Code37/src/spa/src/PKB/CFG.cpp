#include "CFG.h"

CFG::CFG(PKBStorage* storage)
{
	this->storage = storage;

	this->next_star_table = new Table<StatementNumber, StatementNumber, hashStatementNumber, hashStatementNumber>();

	this->affects_table = new Table<StatementNumber, StatementNumber, hashStatementNumber, hashStatementNumber>();

	this->affects_star_table = new Table<StatementNumber, StatementNumber, hashStatementNumber, hashStatementNumber>();
}

CFG::~CFG()
{
	delete next_star_table;
	next_star_table = NULL;
	delete affects_table;
	affects_table = NULL;
	delete affects_star_table;
	affects_star_table = NULL;
	while_stmts = NULL;
	delete outermost_while_stmts;
	outermost_while_stmts = NULL;
}

void CFG::UpdateNextStarOne(Procedure* proc)
{
	StatementNumber sn = StatementNumber(proc->GetStart());
	if (!next_star_table->IsAttribute1Exist(sn) && !next_star_table->IsAttribute2Exist(sn)) {
		GenerateNextStar(proc);
	}
}

void CFG::UpdateNextStarAll()
{
	for (typename unordered_map<ProcedureIndex, Procedure*, hash<size_t>>::iterator it = storage->procedure_table.begin();
		it != storage->procedure_table.end(); ++it) {
		UpdateNextStarOne(it->second);
	}
}

bool CFG::GetNextStar(const StatementNumber& s1, const StatementNumber& s2, Procedure* proc)
{
	UpdateNextStarOne(proc);
	return next_star_table->GetRelation(s1, s2);
}

const set<StatementNumber>* CFG::GetNextStarAfter(const StatementNumber& s, Procedure* proc)
{
	UpdateNextStarOne(proc);
	const set<StatementNumber>* ptr = next_star_table->GetByAttribute1(s);
	return ptr;
}

const set<StatementNumber>* CFG::GetNextStarBefore(const StatementNumber& s, Procedure* proc)
{
	UpdateNextStarOne(proc);
	const set<StatementNumber>* ptr = next_star_table->GetByAttribute2(s);
	return ptr;
}

const set<StatementNumber>* CFG::GetAllNextStarAfter()
{
	UpdateNextStarAll();
	const set<StatementNumber>* ptr = next_star_table->GetAllAttribute1();
	return ptr;
}

const set<StatementNumber>* CFG::GetAllNextStarBefore()
{
	UpdateNextStarAll();
	const set<StatementNumber>* ptr = next_star_table->GetAllAttribute2();
	return ptr;
}

vector<pair<StatementNumber, StatementNumber>>* CFG::GetAllNextStar()
{
	UpdateNextStarAll();
	vector<pair<StatementNumber, StatementNumber>>* ptr = next_star_table->GetAllRelations();
	return ptr;
}

void CFG::GenerateOutermostWhiles()
{
	outermost_while_stmts = new set<StatementNumber>();
	for (typename set<StatementNumber>::iterator it1 = while_stmts->begin(); it1 != while_stmts->end(); ++it1) {
		bool flag = true;
		for (typename set<StatementNumber>::iterator it2 = while_stmts->begin(); it2 != while_stmts->end(); ++it2) {
			if (storage->parent_star_table.GetRelation(*it2, *it1)) {
				flag = false;
				break;
			}
		}
		if (flag) {
			outermost_while_stmts->insert(*it1);
		}
	}
}

void CFG::GenerateNextStar(Procedure* proc)
{
	StatementNumber s = StatementNumber(proc->GetStart());
	DFS(&(storage->next_table), next_star_table, s);
	if (while_stmts == nullptr) {
		this->while_stmts = storage->while_table.GetAllAttribute1();
		GenerateOutermostWhiles();
	}

	for (typename set<StatementNumber>::iterator it = outermost_while_stmts->begin();
		it != outermost_while_stmts->end(); ++it) {
		if (!proc->IsWithin(it->GetLine())) {
			continue;
		}
		const set<StatementNumber>* successors = next_star_table->GetByAttribute1(*it);
		const set<StatementNumber>* children = storage->parent_star_table.GetByAttribute1(*it);
		for (typename set<StatementNumber>::const_iterator it2 = children->begin(); it2 != children->end(); ++it2) {
			next_star_table->AddAllRelations(*it2, *successors);
		}
	}
}

bool CFG::IsAssign(const StatementNumber& sn) 
{
	return storage->assign_table.find(sn) != storage->assign_table.end();
}

bool CFG::IsWhile(const StatementNumber& sn)
{
	return storage->while_table.IsAttribute1Exist(sn);
}

bool CFG::IsIf(const StatementNumber& sn)
{
	return storage->if_table.IsAttribute1Exist(sn);
}

void CFG::GenerateAffects(Procedure* proc)
{
	LMT last_modified_table = LMT();
	GenerateAffects(StatementNumber(proc->GetStart()), last_modified_table);
}

void CFG::GenerateAffects(const StatementNumber& sn, LMT& lmt)
{
	StatementNumber cur = sn;

	while (true) {
		if (IsWhile(cur)) {
			GenerateAffectsForWhile(cur, lmt);
		}
		else if (IsIf(cur)) {
			GenerateAffectsForIf(cur, lmt);
		}
		else if (IsAssign(cur)) {
			GenerateAffectsForAssign(cur, lmt);
		}
		else {
			GenerateAffectsForOthers(cur, lmt);
		}

		const set<StatementNumber>* follower = storage->follow_table.GetByAttribute1(cur);
		if (follower->empty()) {
			return;
		}
		else {
			typename set<StatementNumber>::const_iterator it = follower->begin();
			cur = (*it);
		}
	}
}

void CFG::GenerateAffectsForWhile(const StatementNumber& ws, LMT& before_lmt)
{
	LMT while_lmt(before_lmt);
	const set<StatementNumber>* children = storage->parent_table.GetByAttribute1(ws);
	typename set<StatementNumber>::const_iterator it = children->begin();
	StatementNumber first = *it;
	GenerateAffects(first, while_lmt);
	if (!(before_lmt.Contains(while_lmt))){
		before_lmt.MergeInto(while_lmt);
		GenerateAffectsForWhile(ws, before_lmt);
	}
}

void CFG::GenerateAffectsForIf(const StatementNumber& ifs, LMT& before_lmt)
{
	LMT then_lmt(before_lmt);
	LMT else_lmt(before_lmt);
	pair<StatementNumber, StatementNumber>* p = storage->then_else_table.at(ifs);
	StatementNumber then_first = p->first;
	StatementNumber else_first = p->second;
	GenerateAffects(then_first, then_lmt);
	GenerateAffects(else_first, else_lmt);
	before_lmt.CopyFrom(else_lmt);
	before_lmt.MergeInto(then_lmt);
}

void CFG::GenerateAffectsForAssign(const StatementNumber& cur, LMT& lmt)
{
	const set<VarIndex>* modify_vars = storage->modify_table.GetByAttribute1(cur);
	VarIndex modify_var = *(modify_vars->begin());
	const set<VarIndex>* use_vars = storage->use_table.GetByAttribute1(cur);
	for (typename set<VarIndex>::const_iterator it1 = use_vars->begin(); it1 != use_vars->end(); ++it1) {
		if (!lmt.IsKeyExist(*it1)) {
			continue;
		}
		unordered_set<StatementNumber, hashStatementNumber>* modify_stmts = lmt.GetLastModifiesStatements(*it1);
		for (typename unordered_set<StatementNumber, hashStatementNumber>::const_iterator it2 = modify_stmts->begin();
			it2 != modify_stmts->end(); ++it2) {
			if (IsAssign(*it2)) {
				affects_table->InsertRelation(*it2, cur);
			}
		}
	}
	lmt.Add(modify_var, cur);
}

void CFG::GenerateAffectsForOthers(const StatementNumber& cur, LMT& lmt)
{
	const set<VarIndex>* modify_vars = storage->modify_table.GetByAttribute1(cur);
	for (typename set<VarIndex>::const_iterator it = modify_vars->begin(); it != modify_vars->end(); ++it) {
		lmt.Add(*it, cur);
	}
}

void CFG::UpdateAffectsOne(Procedure* proc)
{
	ProcedureIndex pi = proc->GetIndex();
	if (affects_record.find(pi) == affects_record.end()) {
		GenerateAffects(proc);
		affects_record.insert(pi);
	}
}

void CFG::UpdateAffectsAll()
{
	for (typename unordered_map<ProcedureIndex, Procedure*, hash<size_t>>::iterator it = storage->procedure_table.begin();
		it != storage->procedure_table.end(); ++it) {
		UpdateAffectsOne(it->second);
	}
}

bool CFG::GetAffects(const StatementNumber& sn1, const StatementNumber& sn2, Procedure* proc)
{
	UpdateAffectsOne(proc);
	return affects_table->GetRelation(sn1, sn2);
}

bool CFG::IsAffectsFirstExist(const StatementNumber& sn, Procedure* proc)
{
	UpdateAffectsOne(proc);
	return affects_table->IsAttribute1Exist(sn);
}

bool CFG::IsAffectsSecondExist(const StatementNumber& sn, Procedure* proc)
{
	UpdateAffectsOne(proc);
	return affects_table->IsAttribute2Exist(sn);
}

bool CFG::IsAffectsEmpty()
{
	for (typename unordered_map<ProcedureIndex, Procedure*, hash<size_t>>::iterator it = storage->procedure_table.begin();
		it != storage->procedure_table.end(); ++it) {
		UpdateAffectsOne(it->second);
		if (!affects_table->IsEmpty()) {
			return false;
		}
	}
	return true;
}

const set<StatementNumber>* CFG::GetStatementsAffectedBy(const StatementNumber& sn, Procedure* proc)
{
	UpdateAffectsOne(proc);
	return affects_table->GetByAttribute1(sn);
}

const set<StatementNumber>* CFG::GetStatementsAffects(const StatementNumber& sn, Procedure* proc)
{
	UpdateAffectsOne(proc);
	return affects_table->GetByAttribute2(sn);
}

const set<StatementNumber>* CFG::GetAllStatementsAffects()
{
	UpdateAffectsAll();
	return affects_table->GetAllAttribute1();
}

const set<StatementNumber>* CFG::GetAllStatementsAffected()
{
	UpdateAffectsAll();
	return affects_table->GetAllAttribute2();
}

vector<pair<StatementNumber, StatementNumber>>* CFG::GetAllAffects()
{
	UpdateAffectsAll();
	return affects_table->GetAllRelations();
}

void CFG::GenerateAffectsStar(Procedure* proc)
{
	ProcedureIndex pi = proc->GetIndex();
	if (affects_adjacent_matrices.find(pi) == affects_adjacent_matrices.end()) {
		BuildAffectsAdjMatrix(proc);
	}
	ComputeTransitiveClosure(proc->GetAssigns(), affects_adjacent_matrices[pi], affects_star_table);
}

void CFG::BuildAffectsAdjMatrix(Procedure* proc)
{
	UpdateAffectsOne(proc);
	if (!proc->IsAssignFinished()) {
		storage->GenerateAssignStatementsForProc(proc);
	}
	vector<StatementNumber> assigns = proc->GetAssigns();
	int len = assigns.size();
	vector<vector<bool>> adj(len, vector<bool>(len));
	BuildAdjacentMatrix(assigns, affects_table, adj);
	affects_adjacent_matrices[proc->GetIndex()] = adj;
}

void CFG::UpdateAffectsStarOne(Procedure* proc)
{
	ProcedureIndex pi = proc->GetIndex();
	if (affects_star_record.find(pi) == affects_star_record.end()) {
		GenerateAffectsStar(proc);
		affects_star_record.insert(pi);
	}
}

void CFG::UpdateAffectsStarAll()
{
	for (typename unordered_map<ProcedureIndex, Procedure*, hash<size_t>>::iterator it = storage->procedure_table.begin();
		it != storage->procedure_table.end(); ++it) {
		UpdateAffectsStarOne(it->second);
	}
}

bool CFG::GetAffectsStar(const StatementNumber& sn1, const StatementNumber& sn2, Procedure* proc)
{
	UpdateAffectsStarOne(proc);
	return affects_star_table->GetRelation(sn1, sn2);
}

const set<StatementNumber>* CFG::GetStatementsAffectedStarBy(const StatementNumber& sn, Procedure* proc)
{
	UpdateAffectsStarOne(proc);
	return affects_star_table->GetByAttribute1(sn);
}

const set<StatementNumber>* CFG::GetStatementsAffectsStar(const StatementNumber& sn, Procedure* proc)
{
	UpdateAffectsStarOne(proc);
	return affects_star_table->GetByAttribute2(sn);
}

const set<StatementNumber>* CFG::GetAllStatementsAffectsStar()
{
	UpdateAffectsStarAll();
	return affects_star_table->GetAllAttribute1();
}

const set<StatementNumber>* CFG::GetAllStatementsAffectedStar()
{
	UpdateAffectsStarAll();
	return affects_star_table->GetAllAttribute2();
}

vector<pair<StatementNumber, StatementNumber>>* CFG::GetAllAffectsStar()
{
	UpdateAffectsStarAll();
	return affects_star_table->GetAllRelations();
}

void CFG::ClearCache()
{
	delete next_star_table;
	next_star_table = new Table<StatementNumber, StatementNumber, hashStatementNumber, hashStatementNumber>();
	delete affects_table;
	affects_table = new Table<StatementNumber, StatementNumber, hashStatementNumber, hashStatementNumber>();
	affects_record.clear();
	delete affects_star_table;
	affects_star_table = new Table<StatementNumber, StatementNumber, hashStatementNumber, hashStatementNumber>();
	affects_star_record.clear();
}
