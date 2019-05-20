#include "Person.h"

void Person::AddFriend(std::shared_ptr<Person> &f) {
	this->friends.insert(std::weak_ptr<Person>(f));

}
