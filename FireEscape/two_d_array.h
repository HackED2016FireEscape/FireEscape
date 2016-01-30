#pragma once

template <typename T>
struct TwoDArray{
public:
	~TwoDArray() {
		delete[] data;
	}
	void init(unsigned newX, unsigned newY) {
		x = newX;
		y = newY;
		delete[] data;
		data = new T[x*y];
	}
	void fill(T value) {
		for (unsigned i = 0; i < x; ++i) {
			for (unsigned j = 0; j < y; ++j) {
				this->operator[](i)[j] = value;
			}
		}
	}
	T* operator[](unsigned index) {
		return data + index * y;
	}

	unsigned x;
	unsigned y;

private:
	T* data;
};