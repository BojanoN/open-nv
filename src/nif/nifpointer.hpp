#pragma once
#include "nifdata.hpp"



template <typename T>
class NifPointer {

private:
	union {
		uint64_t index;
		T* pointer;
	};

	void resolve(T* actual);
	uint64_t getIndex(); 

	friend class NifData;

public:

	T* get(); 
	NifPointer(uint64_t index) : index{index} {}

};


template <typename T>
T* NifPointer::get() {
	return pointer;
}

template <typename T>
void NifPointer::resolve(T* actual) {
	pointer = actual;
}

template <typename T>
uint64_t NifPointer::getIndex() {
	return index;
}
