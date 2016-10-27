#pragma once

#include <iostream>
using namespace std;

/// Vetor 2d =]
struct Vec2 {
	float x, y;

	/// Ctor, com parâmetros
	Vec2 (float x = 0, float y = 0) : x (x), y (y) {}
	/// Operações básicas
	Vec2 operator+ (const Vec2 & outro) const {
		return Vec2 (x + outro.x, y + outro.y);
	}
	Vec2 operator- (const Vec2 & outro) const {
		return Vec2 (x - outro.x, y - outro.y);
	}
	Vec2 & operator+= (const Vec2 & outro) {
		x += outro.x;
		y += outro.y;
		return *this;
	}
	Vec2 & operator-= (const Vec2 & outro) {
		x -= outro.x;
		y -= outro.y;
		return *this;
	}
};

ostream& operator<< (ostream& os, const Vec2 & v) {
	os << '(' << v.x << ',' << v.y << ')';
	return os;
}
