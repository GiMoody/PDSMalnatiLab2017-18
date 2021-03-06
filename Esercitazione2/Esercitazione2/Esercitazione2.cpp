// Esercitazione2.cpp : Questo file contiene la funzione 'main', in cui inizia e termina l'esecuzione del programma.
//


#include "pch.h"
#include "Person.h"
using namespace std;

int main()
{
	// Infinite cycle untille empty line
	string parseString;
	do {
		cout << "Please enter your friend's link: \n";
		getline(cin, parseString);// , ' ');
		string names;
		stringstream out(parseString);

		std::map<string, shared_ptr<Person>> t;

		while (getline(out, names, ' ')) {
			if (names.find(';')) {
				string namea = names.substr(0, names.find(';'));
				string nameb = names.substr(names.find(';') + 1, names.length());
				if (t.find(namea) == t.end()) {
					Person tt(namea);
					shared_ptr<Person> tn = std::make_shared<Person>(tt);
					t.insert(std::pair< string, shared_ptr<Person>>(namea, tn));

				}
				if (t.find(nameb) == t.end()) {
					Person tt(nameb);
					shared_ptr<Person> tn = std::make_shared<Person>(tt);
					t.insert(std::pair< string, shared_ptr<Person>>(nameb, tn));
				}
				t.find(namea)->second->AddFriend(t.find(nameb)->second);
				t.find(nameb)->second->AddFriend(t.find(namea)->second);
			}
		}
		for (auto it = t.begin(); it != t.end(); it++) {
			cout << it->first + ":" << endl;
			for_each(it->second->friends.begin(), it->second->friends.end(), [](weak_ptr<Person> sds) { if (auto f = sds.lock())cout << "\t" + f->name << endl; });

		}
	} while (parseString.length() != 0);
}
