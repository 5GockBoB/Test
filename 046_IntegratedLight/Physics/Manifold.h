#pragma once

class Manifold
{
public:
	static bool Solve(Collider* a, Collider* b);
	static bool Solve(GameModel* a, GameModel* b);
};