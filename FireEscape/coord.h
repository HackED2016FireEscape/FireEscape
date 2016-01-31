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
	// Can put coordinate operations here for convenience
};