#pragma once
#include "libraries.hpp"

namespace utils
{
	inline float hireTimeInSeconds()
	{
		float t = SDL_GetTicks();
		t *= 0.001f;

		return t;
	}
}