#pragma once
#include "pch.h"
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <map>
#include <fstream>
#include <queue>

/********************************************************************************************
* Esercitazione 3 2017/2018 - PDS                                                           *
*   Given a sorted list of N integer number, generate a N-1 list in which                   *
*   the new value is obtained by add, subtract, multiply or divide two number               *
*   inside the list.                                                                        *
*   The new N-1 sorted list contains the result of the mathematical operation and all the   *
*   non-used value.                                                                         *
*                                                                                           *
*   Each list has a score calcuate in this way:                                             *
*       - Current score = previous score + result of the math operation * Constant value    *
*       The constant value change depending on the type of operation                        *
*               (SUM = 2, SUB = 1 , MUL = 4, DIV = 3)                                       *
*       There are some constraint on how the operation has to be performed:                 *
*           -> A-B > 0                                                                      *
*           -> A*B <=> A,B > 0                                                              *
*           -> A/B <=> A,B > 0 & A%B = 0                                                    *
*           -> Sums are always possible.                                                    *
*                                                                                           *
*   The program has the objective to find all the sorted combination of 7 number between 0  *
*   and 9 with final value equal to zero and maximum score.                                 *
*********************************************************************************************/

// Operation's point
enum OpPoints
{
	SUM = 2,
	SUB = 1,
	MUL = 4,
	DIV = 3
};

// Struct that containts the two operator of a generic mathematical operation
// and the gained points
struct Instruction {
public:
	int op1, op2;
	OpPoints op;

	Instruction(int o1, int o2, OpPoints p) : op1(o1), op2(o2), op(p) {};
};

/*
* Solution is a class that for a given problem (the sorted list) save a solution 
*/
class Solution {
    // Solution private variables
	std::vector<Instruction> instructions;
	std::vector<int> problem;
	int total_points;

public:
    /**
    * Default Constructor
    */
	Solution() : total_points(-1) {};

    /**
    * Constructor with an initial solution
    * @param i: Initial instruction
    * @param v: Problem to solve (list of sorted integer)
    * @param p: Current gained points
    */
	Solution(Instruction i, std::vector<int> v, int p) : problem(v), total_points(p) { instructions.push_back(i); };

    /**
    * Minor operator overloading, it compare two solution based on the value of the total points
    * @param s: Solution to compare
    */
	inline bool operator<(const Solution &s) const {
		return total_points < s.total_points;
	}

    /*
    * Add the new instruction to the solution and calculate the points
    * @param i: New instruction
    */
	void addBack(Instruction i) {
		instructions.push_back(i);
		switch (i.op) {
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

    /*
    * For the current solution calculate the total points
    */
	void calculatePoints() {
		total_points = 0;
		for (auto it = instructions.begin(); it != instructions.end(); it++) {
			if (it->op1 == -1 && it->op2 == -1) continue;
			switch (it->op) {
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

    /*
    * Print the current solution with each operation executed
    */
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

    /*
    * Check if the current solution is empty
    */
	bool isEmpty() { return instructions.size() > 0 ? false : true; }
    
    /*
    * Check if the current solution is solved
    */
    bool isSovle() { return total_points >= 0 ? true : false; }

    /*
    * Set the problem to the current solution
    * @param v: problem to solve (list of sorted integer)
    */
	void setVector(std::vector<int> v) { problem = v; }

    /*
    * Return the total current points for this solution
    */
	int getPoints() { return total_points; }
	
};


typedef std::vector<int> Problem;
