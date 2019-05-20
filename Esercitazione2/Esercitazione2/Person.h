#pragma once
#include "pch.h"

/**********************************
* Esercitazione 2 2017/2018 - PDS *
***********************************/

/**
* Person is a class that descrive a person and its set of friends.
*/
class Person {
	public:
		// Data members
		std::string name;
		std::set<std::weak_ptr<Person>, std::owner_less<std::weak_ptr<Person>>> friends;
	
		/**
		* Default and only Constructor
		* @param n: Person name
		*/
		Person(std::string n) : name(n) {};

		/**
		* Add a friend to the Person's list
		* @param f: shared pointer of the Person
		*/
		void AddFriend(std::shared_ptr<Person> f) {
			this->friends.insert(std::weak_ptr<Person>(f));
		}

};