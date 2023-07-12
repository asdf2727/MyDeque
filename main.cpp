#include <iostream>

#include "deque.h"

int main () {
	MyDeque <int> vec;
	for(int i = 0; i < 100; i++) {
		vec.push_back(i);
	}
	for(int i = 0; i < 90; i++) {
		vec.pop_back();
	}
	for(int i = 0; i < 90; i++) {
		vec.push_front(i + 100);
	}
	for(int i = 0; i < 80; i++) {
		vec.pop_front();
	}
	for(int val : vec) {
		std::cout << val << ' ';
	}
	return 0;
}
