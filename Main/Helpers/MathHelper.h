#pragma once

#include <cmath>

class MathHelper
{
public:
	template <class T>
	static float log2(T f);

	template <class T>
	static T pow2(T e);

	template <class T>
	static T clamp(T value, T min, T max);

	template <class T>
	static T saturate(T value);

	template <class T>
	static T round(T value);

	template <class T>
	static T lerp(T a, T b, T s);
};


template <class T>
T MathHelper::clamp(T value, T min, T max)
{
	return (value < min) ? min : (value > max) ? max : value;
}


template <class T>
T MathHelper::pow2(T e)
{
	return (T)std::pow(2, (float)e);
}


template <class T>
float MathHelper::log2(T n)
{
	return std::log((float)n) / std::log(2.0f);
}


template <class T>
T MathHelper::round(T value)
{
	return std::floor(value + 0.5f);
}


template <class T>
static T MathHelper::lerp(T a, T b, T s)
{
	return a + s * (b - a);
}


template <class T>
static T MathHelper::saturate(T value)
{
	return clamp(value, (T)0, (T)1);
}