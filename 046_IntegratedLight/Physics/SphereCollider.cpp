#include "stdafx.h"
#include "SphereCollider.h"
#include "./Objects/GameModel.h"

SphereCollider::SphereCollider(GameModel * model)
	: Collider(model)
{
	shape = eSphere;
}

SphereCollider::~SphereCollider()
{
}