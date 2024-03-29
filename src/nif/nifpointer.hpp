#pragma once

#include "nifdata.hpp"
#include "nifreader.hpp"
#include <variant>

template <typename T>
class NifPointer {

private:
	
	union {
		uint64_t index;
		T* pointer;
	};

	void resolve(T* actual);
	uint64_t getIndex(); 

public:

	T* get(); 
	NifPointer(uint64_t index) : index{index} {}
	NifPointer(uint32_t index) : index{static_cast<uint64_t>(index)} {}
	NifPointer() {}
	void resolve(NifData& data);

	void load(NifReader& reader);
};


template <typename T>
T* NifPointer<T>::get() {
	return pointer;
}

template <typename T>
void NifPointer<T>::resolve(T* actual) {
	pointer = actual;
}

template <typename T>
uint64_t NifPointer<T>::getIndex() {
	return index;
}


template <typename T>
void NifPointer<T>::resolve(NifData& data) {
	if(static_cast<uint32_t>(this->index) == -1){
		return;
	}
	this->pointer = reinterpret_cast<T*>(data.getBlock(this->index));
}

template <typename T>
void NifPointer<T>::load(NifReader& reader) {
	uint32_t index;
	reader.read(&index, sizeof(uint32_t), 1);
	this->index = static_cast<uint64_t>(index);
}


template <typename T>
class NifPointerList {
private:
	uint32_t length = 0;
	//bool resolved = false;
	std::vector<std::variant<uint64_t, T*>> pointers;
	/*union {
		uint64_t* indices;
		T** pointers;
	};*/

public:
	T* get(uint32_t index) {return pointers[index];}
	NifPointerList() {}

	void resolve(NifData& data);
	void load(NifReader& reader, uint32_t length);

	T* operator[](uint32_t idx) { return std::get<T*>(pointers[idx]); }

	~NifPointerList() {
		/*if(this->indices != nullptr && this->resolved) {
			delete[] this->indices;
		}*/
	}
};

template <typename T>
void NifPointerList<T>::load(NifReader& reader, uint32_t length) {
	this->length = length;
	std::vector<uint32_t> indices(length);
	reader.read(&indices[0], sizeof(uint32_t), length);
	this->pointers.resize(length);
	//this->indices = new uint64_t[length];
	for(unsigned int i = 0; i < length; i++) {
		//this->indices[i] = static_cast<uint64_t>(indices[i]);
		this->pointers[i] = static_cast<uint64_t>(indices[i]);
	}
}

template <typename T>
void NifPointerList<T>::resolve(NifData& data) {
	for(unsigned int i = 0; i < this->length; i++) {
		if(static_cast<uint32_t>(std::get<uint64_t>(this->pointers[i])) == -1) {
			continue;
		}
		this->pointers[i] = reinterpret_cast<T*>(data.getBlock(std::get<uint64_t>(this->pointers[i])));
	}
	//this->resolved = true;
}