#pragma once

template <typename T>
struct Coord {
	T x;
	T y;

	Coord<T> operator+=(const Coord<T>& other) {
		x += other.x;
		y += other.y;
		return *this;
	}
	Coord<T> operator-=(const Coord<T>& other) {
		x -= other.x;
		y -= other.y;
		return *this;
	}
	Coord<T> operator+(const Coord<T>& other) {
		return{
			x + other.x,
			y + other.y
		};
	}
	bool operator<(const Coord<T>& other) {
		return x < other.x;
	}
	bool operator==(const Coord<T>& other) {
		return x == other.x && y == other.y;
	}
	bool operator!=(const Coord<T>& other) {
		return x != other.x && y != other.y;
	}
	// Can put coordinate operations here for convenience
};