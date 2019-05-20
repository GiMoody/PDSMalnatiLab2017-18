// Esercitazione4.cpp : Questo file contiene la funzione 'main', in cui inizia e termina l'esecuzione del programma.

#include "pch.h"
#include "Cache.h"

#define N 16

Solution func(const std::vector<int> &v);
std::vector<int> op(int i, int j, const std::vector<int> &original, OpPoints op);
void comb(int pos, int i, std::vector<int> vec);
std::vector<std::string> thread_func(size_t start, size_t end);

//std::map<Problem, Solution> cache;
//cache_t<Problem, Solution> cache(N);
sub_caches<cache_t<Problem, Solution>> cache(N);
std::vector<std::vector<int>> combinations;
int size_fin;

int main()
{
	std::fstream f;
	std::vector<int> input(7, 0);
	std::vector<std::future<std::vector<std::string>>> future_vector;
	size_fin = input.size();
	// f.open("Report.txt", std::ofstream::out, std::ofstream::trunc);

	 // Creazione combinazione
	comb(input.size() - 1, 0, input);
	std::sort(combinations.begin(), combinations.end());
	size_t max_thread = std::thread::hardware_concurrency();
	int n_iter = combinations.size() / max_thread;

	size_t executed = 0;

	// Inizio esecuzione
	for (size_t i = 0; i < max_thread - 1; i++) {
		std::future<std::vector<std::string>> f = std::async(thread_func, executed, executed + n_iter);
		future_vector.push_back(std::move(f));
		executed += n_iter;
	}
	std::vector<std::string> sol = thread_func(executed, combinations.size());
	std::set<std::string> sol_string;
	sol_string.insert(sol.begin(), sol.end());


	for (auto t = future_vector.begin(); t != future_vector.end(); t++) {
		sol = t->get();
		sol_string.insert(sol.begin(), sol.end());
	}

	f.open("Report.txt", std::ofstream::out, std::ofstream::trunc);
	for (auto it = sol_string.begin(); it != sol_string.end(); it++)
		f << *it;
	f.close();
	std::cout << "Total solution: " << sol_string.size() << std::endl;
}

std::vector<std::string> thread_func(size_t start, size_t end) {
	std::vector<std::string> sol;
	for (size_t i = start; i < end; i++) {
		Solution solution = func(combinations[i]);

		// Salvataggio risultato migliore su file
		if (solution.isSovle()) {
			std::stringstream s;
			s << "Sequence: ";
			for (auto t = combinations[i].begin(); t != combinations[i].end(); t++)
				s << *t << " ";
			s << " Points:" << solution.getPoints() << "  Ops:";
			s << solution.printInstruction();
			sol.push_back(s.str());
		}
	}
	return sol;
}


void comb(int pos, int i, std::vector<int> vec) {
	combinations.push_back(vec);

	if (pos < 0)
		return;
	for (int j = 0; j < 9; j++) {
		vec[pos] = j + 1;
		if (pos == vec.size() - 1)
			comb(pos - 1, 0, vec);
		if (pos + 1 < vec.size() && vec[pos] <= vec[pos + 1])
			comb(pos - 1, 0, vec);
	}
}

Solution func(const std::vector<int> &v) {
	//if (cache.find(v) != cache.end())
	//    return cache[v];
	Solution sol;
	if (cache.get(v, sol))
		return sol;

	std::priority_queue<Solution> s;
	if (v.size() == 1 && v[0] == 0) {
		s.push(Solution(Instruction(-1, -1, SUM), v, 0)); //  Valore default se operazione andata a buon fine

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
	//cache.insert(std::make_pair(v, s.top()));
	cache.put(v, s.top());
	return s.top();
}

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