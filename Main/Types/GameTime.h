#pragma once

struct GameTime
{
	GameTime() : elapsed(0), total(0) {}
	GameTime(float elapsed, float total) : elapsed(elapsed), total(total) {}

	float elapsed;
	float total;
};