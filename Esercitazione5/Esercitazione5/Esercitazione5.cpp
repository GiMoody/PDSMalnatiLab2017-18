// Esercitazione5.cpp : Questo file contiene la funzione 'main', in cui inizia e termina l'esecuzione del programma.
//

#include "pch.h"
#include "message_loop.h"


int main()
{
	message_loop ml;
	ml.startup();
	auto t1 = std::make_shared< std::packaged_task<void()> >([]() { std::cout << "second message\n"; throw 1; });
	auto t3 = std::make_shared< std::packaged_task<void()> >([]() { std::cout << "third message\n"; });
	auto t2 = std::make_shared< std::packaged_task<void()> >([&ml, t3]() { std::cout << "first message\n"; ml.post_delayed(t3, std::chrono::milliseconds(1500)); });
	ml.post_delayed(t1, std::chrono::milliseconds(1000));
	ml.post_message(t2);
	auto f1 = t1->get_future();
	try { f1.get(); } catch (...) { std::cout << "Exception caught in t1\n"; }
	std::this_thread::sleep_for(std::chrono::milliseconds(5000));
	ml.shutdown();
	return 0;
}
