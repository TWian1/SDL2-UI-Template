#pragma once
template<typename T>
struct vec2 {
	T x, y;
	vec2(T x = T(), T y = T()) : x(x), y(y) {}};

template<typename T>
struct vec3 {
	T x, y, z;
	vec3(T x = T(), T y = T(), T z = T()) : x(x), y(y), z(z) {}};