#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include "pch.h"
#include <iostream>
#include <string>
#include <memory>
#include <future>
#include <chrono>
#include <mutex>
#include <shared_mutex>
#include <condition_variable>
#include <vector>
#include <queue>
#include <atomic>
#include <ctime>
#include <iomanip>
#include <stdlib.h>

/**********************************
* Esercitazione 5 2017/2018 - PDS *
***********************************/

class message_loop {
	std::thread thread_queue;
	std::priority_queue<std::pair<unsigned long long, std::shared_ptr<std::packaged_task<void()>>>,
						std::vector<std::pair<unsigned long long, std::shared_ptr<std::packaged_task<void()>>>>,
						std::greater<std::pair<unsigned long long, std::shared_ptr<std::packaged_task<void()>>>>> message_queue;

	std::atomic<bool> close_thread, is_terminated;
	std::mutex m;

	void thread_cycle() {
		while (!close_thread.load()) {
			std::lock_guard<std::mutex> lc(m);
			if (!message_queue.empty()) {
				std::pair<unsigned long, std::shared_ptr<std::packaged_task<void()>>> task = message_queue.top();
				unsigned long now = std::chrono::system_clock::now().time_since_epoch().count();
				try {
					if (now >= task.first) {
						message_queue.pop();
						std::thread t(std::ref(*task.second.get()));
						t.detach();
					}
				}
				catch (...) {
					throw;
				}
			}
		}
		is_terminated.store(true);
	}

public:
	message_loop() { is_terminated.store(true); close_thread.store(true); };
	void startup() {
		if (is_terminated.load()) {
			close_thread.store(false);
			is_terminated.store(false);
			std::thread t([this]() {this->thread_cycle(); });
			thread_queue = std::move(t);
		}
	}

	void shutdown() {
		if (!close_thread.load()) {
			close_thread.store(true);
			std::unique_lock<std::mutex> lc(m);
			while (!message_queue.empty())
				message_queue.pop();
			
			lc.unlock();
			thread_queue.join();
			
		}
		else
			if(is_terminated.load())
				throw std::exception("The loop is no initialized!");
			else
				throw std::exception("The loop has already start the shutdown!");
	}

	void post_message(std::shared_ptr<std::packaged_task<void()>> pt) {
		std::lock_guard<std::mutex> lc(m);
		if (!close_thread.load()) {
			auto time = std::chrono::system_clock::now().time_since_epoch();
			message_queue.push(std::make_pair(time.count(), pt));
		}
		else
			throw std::exception("No loop is active at the moment");
	}
	
	// Chiamato da post task
	void post_delayed(std::shared_ptr<std::packaged_task<void()>> pt, std::chrono::milliseconds delay) {
		std::lock_guard<std::mutex> lc(m);
		if (!close_thread.load()) {
			auto time = std::chrono::system_clock::now().time_since_epoch() + delay;
			message_queue.push(std::make_pair(time.count(), pt));
		}
		else
			throw std::exception("No loop is active at the moment");
	}
	
	
	~message_loop() {
		close_thread.store(true);
		if(thread_queue.joinable())
			thread_queue.join();
	}
};
	
