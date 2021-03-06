#include "pch.h"
#include "Cache.h"

Solution func(const std::vector<int> &v);
std::vector<int> op(int i, int j, const std::vector<int> &original, OpPoints op);
void comb(int pos, int i,std::vector<int> vec);

// Global variables
std::map<Problem, Solution> cache;
std::vector<std::vector<int>> combinations;
int size_fin;

int main() {
	std::fstream f;
	std::vector<int> input(7,0);
	size_fin = input.size();
	f.open("Report.txt", std::ofstream::out, std::ofstream::trunc);
	
	// Creazione combinazione
    // Create the sets of sorted integer with number between 0 and 9
	comb(input.size()-1, 0, input);
	std::sort(combinations.begin(), combinations.end());
	
	// Inizio esecuzione
    // Start execution
	for (auto it = combinations.begin(); it != combinations.end(); it++) {
		Solution solution = func(*it);

		// Salvataggio risultato migliore su file
        // Save on file the best solution for that combination
		if (solution.isSovle()) {
			f << "Sequence: ";
			for (auto t = it->begin(); t != it->end(); t++)
				f << *t << " ";
			f << " Points:" << solution.getPoints() << "  Ops:";
			f << solution.printInstruction() << std::endl;
		}
	}
	f.close();
}

/*
* Create all the combinations: recursive operation
*/
void comb(int pos, int i, std::vector<int> vec) {
	combinations.push_back(vec);
	
	if (pos < 0)
		return;
	for (int j = 0; j < 9; j++) {
		vec[pos] = j + 1;
		if(pos == vec.size() -1)
			comb(pos - 1, 0, vec);
		if (pos + 1 < vec.size() && vec[pos] <= vec[pos + 1])
			comb(pos - 1, 0, vec);
	}
}

/*
* Recursive operation that solve the problem
*/
Solution func(const std::vector<int> &v) {
	if (cache.find(v) != cache.end())
		return cache[v];

	std::priority_queue<Solution> s;
	if (v.size() == 1 && v[0] == 0) {
        //  Valore default se operazione andata a buon fine
        //  Added default value if operation ended up fine
		s.push(Solution(Instruction(-1, -1, SUM), v, 0)); 
	}
	else {
		for (int m = 0; m < v.size() - 1; m++) {
			for (int k = m + 1; k < v.size(); k++) {

				// Caso somma
				std::vector<int> tmp = op(m, k, v, SUM);
				Solution sol = func(tmp);
				if (sol.isSovle()) {
					sol.setVector(tmp);
					sol.addBack(Instruction(v[m], v[k], SUM));
					s.push(sol);
				}

				// Caso sottrazione
				if ((v[k] - v[m]) >= 0) {
					std::vector<int> tmp = op(m, k, v, SUB);
					Solution sol = func(tmp);
					if (sol.isSovle()) {
						sol.setVector(tmp);
						sol.addBack(Instruction(v[m], v[k], SUB));
						s.push(sol);
					}
					// Caso moltiplicazione
					if (v[k] != 0 && v[m] != 0) {
						std::vector<int> tmp = op(m, k, v, MUL);
						Solution sol = func(tmp);
						if (sol.isSovle()) {
							sol.setVector(tmp);
							sol.addBack(Instruction(v[m], v[k], MUL));
							s.push(sol);
						}
						// Caso divisione
						if (v[k] != 0 && v[m] != 0 && v[k] % v[m] == 0) {
							std::vector<int> tmp = op(m, k, v, DIV);
							Solution sol = func(tmp);
							if (sol.isSovle()) {
								sol.setVector(tmp);
								sol.addBack(Instruction(v[m], v[k], DIV));
								s.push(sol);
							}
						}
					}
				}
			}
		}
	}

	if (s.empty()) {
		s.push(Solution(Instruction(-1, -1, SUM), v, -1));
	}
	cache.insert(std::make_pair(v, s.top()));
	return s.top();
}

/*
* Execute one operation between the two number and return a new list of number (new problem)
*/
std::vector<int> op(int i, int j, const std::vector<int> &original, OpPoints op) {
	int v1 = original[i], v2 = original[j];
	int s;
	std::vector<int> result(original);
	result.erase(result.begin() + j);
	result.erase(result.begin() + i);
	switch (op)
	{
	case SUM:
		s = v1 + v2;
		break;
	case SUB:
		s = v2 - v1;
		break;
	case MUL:
		s = v1 * v2;
		break;
	case DIV:
		s = v2 / v1;
		break;
	}
	result.push_back(s);
	std::sort(result.begin(), result.end());
	return std::move(result);
}
