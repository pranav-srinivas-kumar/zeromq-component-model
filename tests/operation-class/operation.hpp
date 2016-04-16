/*
 * Operation Class - Initial Design
 * Author: Pranav Srinivas Kumar
 * Date: 2016.04.16
 */

#ifndef OPERATION
#define OPERATION
#include <iostream>
#include <functional>

class Operation {
public:
  Operation(std::string name, 
	    unsigned int priority, 
	    std::function<void ()> operation_function) : 
    name(name),
    priority(priority),
    operation_function(operation_function) {}

  void execute() {
    operation_function();
  }

private:
  std::string name;
  unsigned int priority;
  std::function<void ()> operation_function;
};

#endif
