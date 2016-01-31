#pragma once
#include "coord.h"

template <typename T>
struct TwoDArray{
public:
	~TwoDArray() {
		delete[] data;
	}
	void init(int newX, int newY) {
		x = newX;
		y = newY;
		delete[] data;
		data = new T[x*y];
	}
	void fill(T value) {
		for (int i = 0; i < x; ++i) {
			for (int j = 0; j < y; ++j) {
				this->operator[](i)[j] = value;
			}
		}
	}
	T* operator[](int index) {
		return data + index * y;
	}
	template<typename T2 = T>
	T& fromCoord(const Coord<T2>& coord) {
		return (data + coord.x * y)[coord.y];
	}

	int x;
	int y;

private:
	T* data = nullptr;
};