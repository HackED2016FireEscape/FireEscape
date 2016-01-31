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
	T& fromCoord(Coord<T2> coord) {
		if (coord.x < 0) {
			coord.x = 0;
		}
		if (coord.x > x - 1) {
			coord.x = x - 1;
		}

		if (coord.y < 0) {
			coord.y = 0;
		}
		if (coord.y > y - 1) {
			coord.y = y - 1;
		}

		return (data + coord.x * y)[coord.y];
	}

	int x;
	int y;

private:
	T* data = nullptr;
};