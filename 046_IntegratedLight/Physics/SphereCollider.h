#pragma once
#include "Collider.h"

class SphereCollider : public Collider
{
public:
	SphereCollider(class GameModel* model);
	~SphereCollider();
};