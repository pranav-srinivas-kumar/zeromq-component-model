/*
 * Subscriber class
 * Author: Pranav Srinivas Kumar
 * Date: 2016.04.16
 */

#ifndef SUBSCRIBER
#define SUBSCRIBER
#include <iostream>
#include <sstream>
#include <zmq.hpp>
#include "operation_queue.hpp"

class Subscriber {
public:
  Subscriber(std::string name, 
	     unsigned int priority, 
	     std::string filter,
	     std::string endpoint, 
	     std::function<void(const std::string&)> operation_function, 
	     Operation_Queue * operation_queue_ptr) : 
    name(name),
    priority(priority),
    filter(filter),
    endpoint(endpoint),
    operation_function(operation_function),
    operation_queue_ptr(operation_queue_ptr) {
    context = new zmq::context_t(2);
    subscriber_socket = new zmq::socket_t(*context, ZMQ_SUB);
    subscriber_socket->connect(endpoint);
    subscriber_socket->setsockopt(ZMQ_SUBSCRIBE, filter.c_str(), filter.length());
  }

  ~Subscriber() {
    subscriber_socket->close();
    delete context;
    delete subscriber_socket;
  }

  void recv() {
    while(true) {
      zmq::message_t received_message; 
      subscriber_socket->recv(&received_message);
      std::string message = std::string(static_cast<char*>(received_message.data()), 
					received_message.size());
      if (message.length() > 0) {
	Operation new_operation(name, priority, std::bind(operation_function, message));
	operation_queue_ptr->enqueue(new_operation);
      }
    }
  }

  std::thread spawn() {
    return std::thread(&Subscriber::recv, this);
  }

  void start() {
    std::thread subscriber_thread = spawn();
    subscriber_thread.detach();
  }

private:
  std::string name;
  unsigned int priority;
  std::string filter;
  std::string endpoint;
  std::function<void(const std::string&)> operation_function;
  Operation_Queue * operation_queue_ptr;
  zmq::context_t * context;
  zmq::socket_t * subscriber_socket;  
};

#endif
