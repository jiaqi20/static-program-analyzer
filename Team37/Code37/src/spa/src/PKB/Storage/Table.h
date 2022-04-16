#ifndef INC_21S2_CP_SPA_TEAM_37_TABLE_H
#define INC_21S2_CP_SPA_TEAM_37_TABLE_H

#include <stdio.h>
#include <iostream>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <algorithm>

using namespace std;

template<class Attribute1,
	class Attribute2,
	class Hash1 = hash<Attribute1>,
	class Hash2 = hash<Attribute2>
>
class Table
{
	template<class Attribute, class Hash>
	friend void ComputeTransitiveClosureForDAG(Table<Attribute, Attribute, Hash, Hash>& origin,
		Table<Attribute, Attribute, Hash, Hash>& target);

	template<class Attribute, class Hash>
	friend void ComputeTransitiveClosureForDAG(Table<Attribute, Attribute, Hash, Hash>& origin,
		Table<Attribute, Attribute, Hash, Hash>& target, const Attribute& init);

	template<class Attribute3,
		class Attribute4,
		class Hash3,
		class Hash4
	>
	friend void ComputeIndirectRelation(Table<Attribute3, Attribute3, Hash3, Hash3>& connector,
		Table<Attribute3, Attribute4, Hash3, Hash4>& relation, unordered_set<Attribute3, Hash3>& visited);

	template<class Attribute3,
		class Attribute4,
		class Hash3,
		class Hash4
	>
	friend void ComputeIndirectRelationHelper(Table<Attribute3, Attribute3, Hash3, Hash3>& connector,
		Table<Attribute3, Attribute4, Hash3, Hash4>& relation, unordered_set<Attribute3, Hash3>& visited,
		const Attribute3& init);

	template<class Attribute, class Hash>
	friend void DFS(Table<Attribute, Attribute, Hash, Hash>* origin,
		Table<Attribute, Attribute, Hash, Hash>* target, const Attribute& init);

	template<class Attribute, class Hash>
	friend void DFS(Table<Attribute, Attribute, Hash, Hash>* origin,
		Table<Attribute, Attribute, Hash, Hash>* target, const Attribute& init, unordered_set<Attribute, Hash>& visited);

	template<class Attribute, class Hash>
	friend bool DFSNoCycle(Table<Attribute, Attribute, Hash, Hash>& origin,
		Table<Attribute, Attribute, Hash, Hash>& target);

	template<class Attribute, class Hash>
	friend bool DFSNoCycleHelper(Table<Attribute, Attribute, Hash, Hash>* origin,
		Table<Attribute, Attribute, Hash, Hash>* target, const Attribute& init, unordered_map<Attribute, int, Hash>& visited);

	// Compute transitive closure of a graph (probably with cycles)
	// Implement Floyd Warshall

	template<class Attribute, class Hash>
	friend void ComputeTransitiveClosure(const vector<Attribute>& vertices, const vector<vector<bool>>& adj,
		Table<Attribute, Attribute, Hash, Hash>* target);

	template<class Attribute, class Hash>
	friend void BuildAdjacentMatrix(const vector<Attribute>& vertices, Table<Attribute, Attribute, Hash, Hash>* origin,
		vector<vector<bool>>& result);

private:
	// map1: key is attribute1, value is a list of attribute2
	unordered_map <Attribute1, set<Attribute2>*, Hash1> map1;
	// map2: key is attribute2, value is a list of attribute1
	unordered_map <Attribute2, set<Attribute1>*, Hash2> map2;

	set<Attribute1> all1;

	bool all1_flag = false;

	set<Attribute2> all2;

	bool all2_flag = false;
	
	set<Attribute1> empty1;

	set<Attribute2> empty2;

public:
	Table();
	~Table();
	bool InsertRelation(const Attribute1& attribute1, const Attribute2& attribute2);
	void AddAllRelations(const Attribute1& attribute1, const set<Attribute2>& attribute2s);
	bool GetRelation(const Attribute1& attribute1, const Attribute2& attribute2) const;
	const set<Attribute2>* GetByAttribute1(const Attribute1& key) const;
	const set<Attribute1>* GetByAttribute2(const Attribute2& key) const;
	vector<pair<Attribute1, Attribute2>>* GetAllRelations() const;
	const set<Attribute1>* GetAllAttribute1();
	const set<Attribute2>* GetAllAttribute2();
	bool IsAttribute1Exist(const Attribute1& a) const;
	bool IsAttribute2Exist(const Attribute2& a) const;
	bool IsArg1Empty() const;
	bool IsArg2Empty() const;
	bool IsEmpty() const;

	const set<Attribute1>* EmptySet1() const {
		return &empty1;
	}
	const set<Attribute2>* EmptySet2() const {
		return &empty2;
	}
};
template<class Attribute1,
	class Attribute2,
	class Hash1,
	class Hash2
>
Table<Attribute1, Attribute2, Hash1, Hash2>::Table()
{
}

template<class Attribute1,
	class Attribute2,
	class Hash1,
	class Hash2
>
Table<Attribute1, Attribute2, Hash1, Hash2>::~Table()
{
	for (typename unordered_map<Attribute1, set<Attribute2>*, Hash1>::iterator it1 = map1.begin(); it1 != map1.end();) {
		delete it1->second;
		it1->second = NULL;
		map1.erase(it1++);
	}
	for (typename unordered_map<Attribute2, set<Attribute1>*, Hash2>::iterator it2 = map2.begin(); it2 != map2.end();) {
		delete it2->second;
		it2->second = NULL;
		map2.erase(it2++);
	}
}

template<class Attribute1,
	class Attribute2,
	class Hash1
>
bool updateMap(unordered_map <Attribute1, set<Attribute2>*, Hash1>& map,
	const Attribute1& attribute1,
	const Attribute2& attribute2) {
	typename unordered_map<Attribute1, set<Attribute2>*, Hash1>::iterator itr1 = map.find(attribute1);
	if (itr1 == map.end()) {
		set<Attribute2>* valueSet = new set<Attribute2>();
		valueSet->insert(attribute2);
		map.insert(pair<Attribute1, set<Attribute2>*>(attribute1, valueSet));
	}
	else {
		set<Attribute2>* valueSet = (*itr1).second;
		typename set<Attribute2>::iterator itr2 = valueSet->find(attribute2);
		if (itr2 != valueSet->end()) {
			return false;
		}
		else {
			valueSet->insert(attribute2);
			map[attribute1] = valueSet;
		}
	}

	return true;
}

template<class Attribute1,
	class Attribute2,
	class Hash1,
	class Hash2
>
bool Table<Attribute1, Attribute2, Hash1, Hash2>::
InsertRelation(const Attribute1& attribute1, const Attribute2& attribute2)
{
	bool insertOne = updateMap(map1, attribute1, attribute2);
	if (!insertOne) {
		return false;
	}

	bool insertTwo = updateMap(map2, attribute2, attribute1);
	if (!insertTwo) {
		return false;
	}
	return true;
}

template<class Attribute1,
	class Attribute2,
	class Hash1,
	class Hash2
>
void Table<Attribute1, Attribute2, Hash1, Hash2>::
AddAllRelations(const Attribute1& attribute1, const set<Attribute2>& attribute2s) {
	for (typename set<Attribute2>::iterator sit = attribute2s.begin(); sit != attribute2s.end(); ++sit) {
		updateMap(map2, (*sit), attribute1);
	}
	if (map1.find(attribute1) == map1.end()) {
		set<Attribute2>* cur = new set<Attribute2>(attribute2s);
		map1[attribute1] = cur;
	}
	else {
		set<Attribute2>* cur = map1[attribute1];
		cur->insert(attribute2s.begin(), attribute2s.end());
	}
}

template<class Attribute1,
	class Attribute2,
	class Hash1,
	class Hash2
>
bool Table<Attribute1, Attribute2, Hash1, Hash2>::
GetRelation(const Attribute1& attribute1, const Attribute2& attribute2) const
{
	typename unordered_map<Attribute1, set<Attribute2>*, Hash1>::const_iterator itr1 = map1.find(attribute1);
	if (itr1 == map1.end()) {
		return false;
	}
	else {
		set<Attribute2>* valueSet = (*itr1).second;
		return valueSet->find(attribute2) != valueSet->end();
	}
}


template<class Attribute1,
	class Attribute2,
	class Hash1
>
const set<Attribute2>*
getAttribute(const unordered_map<Attribute1, set<Attribute2>*, Hash1>& map, const Attribute1& key)
{
	typename unordered_map<Attribute1, set<Attribute2>*, Hash1>::const_iterator itr1 = map.find(key);
	if (itr1 == map.end()) {
		return nullptr;
	}
	else {
		return (*itr1).second;
	}
}

template<class Attribute1,
	class Attribute2,
	class Hash1,
	class Hash2
>
const set<Attribute2>* Table<Attribute1, Attribute2, Hash1, Hash2>::
GetByAttribute1(const Attribute1& key) const
{
	const set<Attribute2>* setPtr = getAttribute(map1, key);
	if (setPtr == nullptr) {
		return &empty2;
	}
	else {
		return setPtr;
	}
}

template<class Attribute1,
	class Attribute2,
	class Hash1,
	class Hash2
>
const set<Attribute1>* Table<Attribute1, Attribute2, Hash1, Hash2>::
GetByAttribute2(const Attribute2& key) const
{
	const set<Attribute1>* setPtr = getAttribute(map2, key);
	if (setPtr == nullptr) {
		return &empty1;
	}
	else {
		return setPtr;
	}
}

template<class Attribute1,
	class Attribute2,
	class Hash1,
	class Hash2
>
vector<pair<Attribute1, Attribute2>>* Table<Attribute1, Attribute2, Hash1, Hash2>::GetAllRelations() const
{
	vector<pair<Attribute1, Attribute2>>* result = new vector<pair<Attribute1, Attribute2>>();
	for (typename unordered_map<Attribute1, set<Attribute2>*, Hash1>::const_iterator it = map1.begin(); 
		it != map1.end(); ++it) {
		Attribute1 key = it->first;
		set<Attribute2>* valueSet = it->second;
		for (typename set<Attribute2>::const_iterator sit = valueSet->begin(); sit != valueSet->end(); ++sit) {
			result->push_back(pair<Attribute1, Attribute2>(key, *(sit)));
		}
	}
	return result;
}

template<class Attribute1,
	class Attribute2,
	class Hash1
>
void getAllAttribute(const unordered_map <Attribute1, set<Attribute2>*, Hash1>& map, set<Attribute1>& res) {
	for (typename unordered_map<Attribute1, set<Attribute2>*, Hash1>::const_iterator it = map.begin();
		it != map.end(); ++it) {
		Attribute1 key = it->first;
		res.insert(key);
	}
}

template<class Attribute1,
	class Attribute2,
	class Hash1,
	class Hash2
>
const set<Attribute1>* Table<Attribute1, Attribute2, Hash1, Hash2>::GetAllAttribute1() {
	if (!all1_flag) {
		getAllAttribute(map1, all1);
		all1_flag = true;
	}
	return &all1;
}

template<class Attribute1,
	class Attribute2,
	class Hash1,
	class Hash2
>
const set<Attribute2>* Table<Attribute1, Attribute2, Hash1, Hash2>::GetAllAttribute2() {
	if (!all2_flag) {
		getAllAttribute(map2, all2);
		all2_flag = false;
	}
	return &all2;
}

template<class Attribute1,
	class Attribute2,
	class Hash1,
	class Hash2
>
bool Table<Attribute1, Attribute2, Hash1, Hash2>::IsEmpty() const {
	return map1.empty() && map2.empty();
}

template<class Attribute1,
	class Attribute2,
	class Hash1,
	class Hash2
>
bool Table<Attribute1, Attribute2, Hash1, Hash2>::IsArg1Empty() const {
	return map1.empty();
}

template<class Attribute1,
	class Attribute2,
	class Hash1,
	class Hash2
>
bool Table<Attribute1, Attribute2, Hash1, Hash2>::IsArg2Empty() const {
	return map2.empty();
}

template<class Attribute, class Hash>
void ComputeTransitiveClosureForDAG(Table<Attribute, Attribute, Hash, Hash>& origin,
	Table<Attribute, Attribute, Hash, Hash>& target) {
	const set<Attribute>* keys = origin.GetAllAttribute1();
	for (typename set<Attribute>::const_iterator it1 = keys->begin(); it1 != keys->end(); ++it1) {
		if (target.map1.find(*it1) == target.map1.end()) {
			ComputeTransitiveClosureForDAG(origin, target, (*it1));
		}
	}
}

template<class Attribute, class Hash>
void ComputeTransitiveClosureForDAG(Table<Attribute, Attribute, Hash, Hash>& origin,
	Table<Attribute, Attribute, Hash, Hash>& target, const Attribute& init) {
	if (target.map1.find(init) != target.map1.end()) {
		return;
	}
	set<Attribute> values = *(origin.GetByAttribute1(init));
	if (!values.empty()) {
		target.AddAllRelations(init, values);
		for (typename set<Attribute>::iterator it2 = values.begin(); it2 != values.end(); ++it2) {
			ComputeTransitiveClosureForDAG(origin, target, *it2);
		}
		for (typename set<Attribute>::iterator it2 = values.begin(); it2 != values.end(); ++it2) {
			set<Attribute> newValues = *(target.GetByAttribute1(*it2));
			target.AddAllRelations(init, newValues);
		}
	}
}

template<class Attribute3,
	class Attribute4,
	class Hash3 = hash<Attribute3>,
	class Hash4 = hash<Attribute4>
>
void ComputeIndirectRelation(Table<Attribute3, Attribute3, Hash3, Hash3>& connector,
	Table<Attribute3, Attribute4, Hash3, Hash4>& relation, unordered_set<Attribute3, Hash3>& visited) {
	for (typename unordered_map<Attribute3, set<Attribute4>*, Hash3>::const_iterator it = connector.map1.begin(); 
		it != connector.map1.end(); ++it) {
		if (visited.find(it->first) == visited.end()) {
			ComputeIndirectRelationHelper(connector, relation, visited, it->first);
		}
	}
}

template<class Attribute1,
	class Attribute2,
	class Hash1,
	class Hash2
>
void ComputeIndirectRelationHelper(Table<Attribute1, Attribute1, Hash1, Hash1>& connector,
	Table<Attribute1, Attribute2, Hash1, Hash2>& relation, unordered_set<Attribute1, Hash1>& visited,
	const Attribute1& init) {
	if (visited.find(init) != visited.end()) {
		return;
	}

	const set<Attribute1>* successors = connector.GetByAttribute1(init);
	if (successors->empty()) {
		visited.insert(init);
		return;
	}

	for (typename set<Attribute1>::const_iterator it1 = successors->begin(); it1 != successors->end(); ++it1) {
		ComputeIndirectRelationHelper(connector, relation, visited, (*it1));
	}
	for (typename set<Attribute1>::const_iterator it1 = successors->begin(); it1 != successors->end(); ++it1) {
		relation.AddAllRelations(init, *(relation.GetByAttribute1(*it1)));
	}
	visited.insert(init);
}

template<class Attribute1,
	class Attribute2,
	class Hash1
>
bool IsExist(const unordered_map<Attribute1, set<Attribute2>*, Hash1>& map, const Attribute1& a) {
	return map.find(a) != map.end();
}

template<class Attribute1,
	class Attribute2,
	class Hash1,
	class Hash2
>
bool Table<Attribute1, Attribute2, Hash1, Hash2>::IsAttribute1Exist(const Attribute1& a) const {
	return map1.find(a) != map1.end();
}

template<class Attribute1,
	class Attribute2,
	class Hash1,
	class Hash2
>
bool Table<Attribute1, Attribute2, Hash1, Hash2>::IsAttribute2Exist(const Attribute2& a) const {
	return map2.find(a) != map2.end();
}

template<class Attribute, class Hash>
void DFS(Table<Attribute, Attribute, Hash, Hash>* origin,
	Table<Attribute, Attribute, Hash, Hash>* target, 
	const Attribute& init) {
	unordered_set<Attribute, Hash> visited;
	DFS(origin, target, init, visited);
}

template<class Attribute, class Hash>
void DFS(Table<Attribute, Attribute, Hash, Hash>* origin,
	Table<Attribute, Attribute, Hash, Hash>* target, const Attribute& init, unordered_set<Attribute, Hash>& visited) {
	if (visited.find(init) != visited.end()) {
		return;
	}

	const set<Attribute>* successors = origin->GetByAttribute1(init);
	visited.insert(init);
	if (successors->empty()) {
		return;
	}
	else {
		target->AddAllRelations(init, *successors);
	}

	for (typename set<Attribute>::const_reverse_iterator itr = successors->rbegin(); itr != successors->rend(); ++itr) {
		DFS(origin, target, *itr, visited);
	}

	for (typename set<Attribute>::const_reverse_iterator itr = successors->rbegin(); itr != successors->rend(); ++itr) {
		set<Attribute> indirect_successors = *(target->GetByAttribute1(*itr));
		target->AddAllRelations(init, indirect_successors);
	}
}

template<class Attribute, class Hash>
bool DFSNoCycle(Table<Attribute, Attribute, Hash, Hash>& origin,
	Table<Attribute, Attribute, Hash, Hash>& target) {
	const set<Attribute>* keys = origin.GetAllAttribute1();
	for (typename set<Attribute>::const_iterator it1 = keys->begin(); it1 != keys->end(); ++it1) {
		if (target.map1.find(*it1) != target.map1.end()) {
			continue;
		}
		unordered_map<Attribute, int, Hash> visited;
		if (!DFSNoCycleHelper(origin, target, (*it1), visited)) {
			return false;
		}
	}
	return true;
}

template<class Attribute, class Hash>
bool DFSNoCycleHelper(Table<Attribute, Attribute, Hash, Hash>& origin,
	Table<Attribute, Attribute, Hash, Hash>& target, const Attribute& init, unordered_map<Attribute, int, Hash>& visited) {
	// not in visited: hasn't start, visited = 0: during traversal, visited = 1: finished
	if (visited.find(init) != visited.end() && visited[init] == 0) {
		return false;
	}

	const set<Attribute>* successors = origin.GetByAttribute1(init);
	visited[init] = 0;
	if (successors->empty()) {
		visited[init] = 1;
		return true;
	}
	else {
		target.AddAllRelations(init, *successors);
	}

	for (typename set<Attribute>::const_iterator it = successors->begin(); it != successors->end(); ++it) {
		if (!DFSNoCycleHelper(origin, target, *it, visited)) {
			return false;
		}
	}
	visited[init] = 1;

	for (typename set<Attribute>::const_iterator it = successors->begin(); it != successors->end(); ++it) {
		set<Attribute> indirect_successors = *(target.GetByAttribute1(*it));
		target.AddAllRelations(init, indirect_successors);
	}

	return true;
}

template<class Attribute, class Hash>
void ComputeTransitiveClosure(vector<Attribute>& vertices, Table<Attribute, Attribute, Hash, Hash>& origin,
	Table<Attribute, Attribute, Hash, Hash>& target) {
	int len = vertices.size();
	unordered_map<Attribute, int, Hash> indices;
	for (int i = 0; i < len; ++i) {
		indices[vertices[i]] = i;
	}
	bool** dp = new bool* [len];
	for (int i = 0; i < len; ++i) {
		dp[i] = new bool[len];
		for (int j = 0; j < len; ++j) {
			dp[i][j] = false;
		}
	}

	for (int i = 0; i < len; ++i) {
		Attribute start = vertices[i];
		set<Attribute>* ends = origin.GetByAttribute1(start);
		for (typename set<Attribute>::const_iterator it = ends->begin(); it != ends->end(); ++it) {
			int j = indices[*it];
			dp[i][j] = true;
		}
	}

	for (int k = 0; k < len; ++k) {
		for (int i = 0; i < len; ++i) {
			for (int j = 0; j < len; ++j) {
				dp[i][j] = dp[i][j] || (dp[i][k] && dp[k][j]);
			}
		}
	}

	for (int i = 0; i < len; ++i) {
		for (int j = 0; j < len; ++j) {
			target.InsertRelation(vertices[i], vertices[j]);
		}
	}

	for (int i = 0; i < len; ++i) {
		delete[] dp[i];
		dp[i] = NULL;
	}
	delete[] dp;
	dp = NULL;
}

template<class Attribute, class Hash>
void ComputeTransitiveClosure(const vector<Attribute>& vertices, const vector<vector<bool>>& adj,
	Table<Attribute, Attribute, Hash, Hash>* target)
{
	int len = vertices.size();
	bool** dp = new bool* [len];
	for (int i = 0; i < len; ++i) {
		dp[i] = new bool[len];
		for (int j = 0; j < len; ++j) {
			dp[i][j] = adj[i][j];
		}
	}

	for (int k = 0; k < len; ++k) {
		for (int i = 0; i < len; ++i) {
			for (int j = 0; j < len; ++j) {
				dp[i][j] = dp[i][j] || (dp[i][k] && dp[k][j]);
			}
		}
	}

	for (int i = 0; i < len; ++i) {
		for (int j = 0; j < len; ++j) {
			if (dp[i][j]) {
				target->InsertRelation(vertices[i], vertices[j]);
			}
		}
	}

	for (int i = 0; i < len; ++i) {
		delete[] dp[i];
		dp[i] = NULL;
	}
	delete[] dp;
	dp = NULL;
}

template<class Attribute, class Hash>
void BuildAdjacentMatrix(const vector<Attribute>& vertices, Table<Attribute, Attribute, Hash, Hash>* origin,
	vector<vector<bool>>& result) {
	int len = vertices.size();
	unordered_map<Attribute, int, Hash> indices;
	for (int i = 0; i < len; ++i) {
		indices[vertices[i]] = i;
	}

	for (int i = 0; i < len; ++i) {
		for (int j = 0; j < len; ++j) {
			result[i][j] = false;
		}
	}

	for (int i = 0; i < len; ++i) {
		Attribute start = vertices[i];
		const set<Attribute>* ends = origin->GetByAttribute1(start);
		for (typename set<Attribute>::const_iterator it = ends->begin(); it != ends->end(); ++it) {
			int j = indices[*it];
			result[i][j] = true;
		}
	}
}

#endif //INC_21S2_CP_SPA_TEAM_37_TABLE_H