/*
 * Timer Class
 * Author: Pranav Srinivas Kumar
 * Date: 2016.04.13
 */

#ifndef TIMER
#define TIMER
#include <iostream>
#include <string>
#include <chrono>
#include <ratio>
#include <thread>
#include "operation_queue.hpp"

class Timer {
public:
  Timer(std::string name, unsigned int priority, long long period, 
	std::function<void()> operation_function, 
	Operation_Queue * operation_queue_ptr) : 
    name(name), 
    priority(priority),
    period(std::chrono::nanoseconds(period)), 
    operation_function(operation_function),
    operation_queue_ptr(operation_queue_ptr) {}

  void operation() {
    while(true) {
      auto start = std::chrono::high_resolution_clock::now();
      while(std::chrono::duration_cast<std::chrono::nanoseconds>
      (std::chrono::high_resolution_clock::now() - start) < period) {}
      auto expiry = std::chrono::high_resolution_clock::now();

      Operation new_operation(name, priority, operation_function);
      operation_queue_ptr->enqueue(new_operation);
    }    
  }

  std::thread spawn() {
    return std::thread(&Timer::operation, this);
  }

  void start() {
    std::thread timer_thread = spawn();
    timer_thread.detach();
  }

private:
  std::string name;
  unsigned int priority;
  std::function<void()> operation_function;
  std::chrono::duration<long long, std::ratio<1, 1000000000>> period;
  Operation_Queue * operation_queue_ptr;
};

#endif
