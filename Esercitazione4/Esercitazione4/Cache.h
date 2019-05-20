#pragma once
#include "pch.h"
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <future>
#include <map>
#include <map>
#include <set>
#include <fstream>
#include <sstream>
#include <queue>

/********************************************************************************************
* Esercitazione 4 2017/2018 - PDS (Same as Esercitazione 3 but multithread )				        *
*********************************************************************************************/
enum OpPoints
{
	SUM = 2,
	SUB = 1,
	MUL = 4,
	DIV = 3
};

struct Instruction {
public:
	int op1, op2;
	OpPoints op;

	Instruction(int o1, int o2, OpPoints p) : op1(o1), op2(o2), op(p) {};
};

class Solution {
	std::vector<Instruction> instructions;
	std::vector<int> problem;
	int total_points;
public:
	Solution() : total_points(-1) {};
	Solution(Instruction i, std::vector<int> v, int p) : problem(v), total_points(p) { instructions.push_back(i); };


	inline bool operator<(const Solution &s) const {
		return total_points < s.total_points;
	}


	void addBack(Instruction i) {
		instructions.push_back(i);
		switch (i.op)
		{
		case SUM:
			total_points += (i.op1 + i.op2)*i.op;
			break;
		case SUB:
			total_points += (i.op2 - i.op1)*i.op;
			break;
		case MUL:
			total_points += (i.op1 * i.op2)*i.op;
			break;
		case DIV:
			total_points += (i.op2 / i.op1)*i.op;
			break;
		}
	}
	void calculatePoints() {
		total_points = 0;
		for (auto it = instructions.begin(); it != instructions.end(); it++) {
			if (it->op1 == -1 && it->op2 == -1) continue;
			switch (it->op)
			{
			case SUM:
				total_points += (it->op1 + it->op2)*it->op;
				break;
			case SUB:
				total_points += (it->op2 - it->op1)*it->op;
				break;
			case MUL:
				total_points += (it->op1 * it->op2)*it->op;
				break;
			case DIV:
				total_points += (it->op2 / it->op1)*it->op;
				break;
			}
		}
	};
	std::string printInstruction() {
		std::string print;
		for (auto it = instructions.begin(); it != instructions.end(); it++) {
			if (it->op1 == -1 && it->op2 == -1) continue;
			switch (it->op) {
			case SUM:
				print += std::to_string(it->op1) + "+" + std::to_string(it->op2);
				break;
			case SUB:
				print += std::to_string(it->op2) + "-" + std::to_string(it->op1);
				break;
			case MUL:
				print += std::to_string(it->op1) + "*" + std::to_string(it->op2);
				break;
			case DIV:
				print += std::to_string(it->op2) + "/" + std::to_string(it->op1);
				break;
			default:
				break;
			}
			if (it != instructions.end() - 1)
				print += " - ";
		}
		print += "\n";
		return print;
	}
	bool isEmpty() { return instructions.size() > 0 ? false : true; }
	bool isSovle() { return total_points >= 0 ? true : false; }

	void setVector(std::vector<int> v) { problem = v; }
	int  getPoints() { return total_points; }

};
typedef std::vector<int> Problem;

template<class cache_t>
class sub_caches {
private:
	size_t n;
	const int M = 16;
	std::vector<cache_t> subs;

	size_t hash(const Problem& p) {
		size_t i = 0;
		for (auto it = p.begin(); it != p.end(); it++)
			i += *it;
		return i % n;
	}

public:
	sub_caches(size_t d) : n(d) {

		subs.resize(d);
	};

	bool get(const Problem& p, Solution& s) {
		size_t index = hash(p);
		return  subs[index].get(p, s);
	}

	void put(const Problem& p, const Solution& s) {
		size_t index = hash(p);
		return subs[index].put(p, s);
	}
};

// Punto 2: Modifica struttura dati es3 per essere thread safe
template<typename Problem, typename Solution>
class cache_t {
public:
	cache_t() {};
	cache_t(const cache_t<Problem, Solution> &s) {
		cache = s.cache;
	}
	cache_t& operator=(const cache_t<Problem, Solution>& rhs) {
		if (this != &rhs) {
			cache = rhs.cache;
		}
		return *this;
	}

	bool get(const Problem& p, Solution& s) {
		std::lock_guard<std::mutex> l(mx);
		if (cache.find(p) != cache.end()) {
			s = cache[p];
			return true;
		}
		return false;
	}

	void put(const Problem& p, const Solution& s) {
		std::unique_lock<std::mutex> l(mx);
		if (cache.find(p) == cache.end())
			cache[p] = s;
	}

private:
	std::map<Problem, Solution> cache;
	std::mutex mx;  
};
