#pragma once
#include "libraries.hpp"

namespace utils //inludes utils(utilities) namespace
{
	inline float hireTimeInSeconds() //inline function to reduce overhead
	{
		float t = SDL_GetTicks(); //declare variable equal to ticks
		t *= 0.001f;

		return t; //returns ticks
	}
}