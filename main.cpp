#include <iostream>

#include "deque.h"

int main () {
	MyDeque <int> vec;
	for(int i = 0; i < 10; i++) {
		vec.push_back(i);
	}
	for(int i = 0; i < 5; i++) {
		vec.pop_front();
	}
	for(int i = 0; i < 5; i++) {
		vec.push_front(i);
	}
	for(int val : vec) {
		std::cout << val << ' ';
	}
	return 0;
}
