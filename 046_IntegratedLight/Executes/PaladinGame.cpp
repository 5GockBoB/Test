#include "stdafx.h"
#include "PaladinGame.h"
#include "./Viewer/Orbit.h"
#include "./Objects/GameModel.h"
#include "./Objects/Paladin.h"
#include "./Objects/Vanguard.h"
#include "./Objects/MeshPlane.h"
#include "./Physics/Manifold.h"
#include "./Model/ModelClip.h"

PaladinGame::PaladinGame(ExecuteValues* values)
	: Execute(values)
{
	paladin = new Paladin();
	SAFE_DELETE(values->MainCamera);
	values->MainCamera = new Orbit(paladin->PositionLink(), 30.0f, 10.0f, rotationSpeed);
	Orbit* cam = reinterpret_cast<Orbit*>(values->MainCamera);
	cam->SetFocusPoint(D3DXVECTOR3(0, 10, 0));

	plane = new MeshPlane();
	plane->Scale(100.0f, 0, 100.0f);
	plane->SetDiffuse(0, 1, 0, 1);

	for (int i = 0; i < 5; i++)
	{
		Vanguard* vanguard = new Vanguard();
		vanguard->Position(i * 50.0f, 0, 50.0f);
		monsters.push_back(vanguard);
	}

	Mouse::Get()->CursorLoop(true);
	ShowCursor(false);
	//ShowCursor(false);
}

PaladinGame::~PaladinGame()
{
	for (Vanguard* monster : monsters)
		SAFE_DELETE(monster);
	SAFE_DELETE(paladin);
	SAFE_DELETE(plane);
}

void PaladinGame::Update()
{
	PaladinCommend();
	MonsterCommend();


	paladin->Update();
	for (Vanguard* monster : monsters)
		monster->Update();

	plane->Update();
	AttackCollision();
}

void PaladinGame::PreRender()
{
}

void PaladinGame::Render()
{
	plane->Render();
	paladin->Render();
	for (Vanguard* monster : monsters)
		monster->Render();
}

void PaladinGame::PostRender()
{
}

void PaladinGame::ResizeScreen()
{
}

void PaladinGame::PaladinCommend()
{
	//	È¸Àü
	{
		D3DXVECTOR2 rotation = paladin->Rotation();

		D3DXVECTOR3 val = Mouse::Get()->GetMoveValue();

		rotation.y += val.x * rotationSpeed * Time::Delta();

		paladin->Rotation(D3DXVECTOR3(0.0f, rotation.y, 0.0f));
	}

	if (paladin->GetState() == State::Hit ||
		paladin->GetState() == State::Dying ||
		paladin->GetState() == State::Attack)
		return;

	if (paladin->GetState() == State::Move) paladin->SetState(State::Idle);

	if (Keyboard::Get()->Press('W'))
	{
		paladin->SetState(State::Move);
		paladin->MoveToDirectionDelta(-moveSpeed, Move::Direction, 0);
	}
	else if (Keyboard::Get()->Press('S'))
	{
		paladin->SetState(State::Move);
		paladin->MoveToDirectionDelta(moveSpeed, Move::Direction, 0);
	}
	if (Keyboard::Get()->Press('D'))
	{
		paladin->SetState(State::Move);
		paladin->MoveToDirectionDelta(-moveSpeed, Move::Right, 0);
	}
	else if (Keyboard::Get()->Press('A'))
	{
		paladin->SetState(State::Move);
		paladin->MoveToDirectionDelta(moveSpeed, Move::Right, 0);
	}

	if (Mouse::Get()->Down(0))
	{
		paladin->SetState(State::Attack);
	}
}

void PaladinGame::MonsterCommend()
{
	if (paladin->GetState() != State::Dying)
	{
		for (Vanguard* monster : monsters)
		{
			if (monster->GetState() == MonsterState::Idle || monster->GetState() == MonsterState::Move)
			{
				float dist = Math::GetDistance(monster->Position(), paladin->Position());
				if (dist < monster->GetAttackRange())
				{
					monster->SetState(MonsterState::Attack);
					monster->RotationYToPointDelta(rotationSpeed, paladin->Position());
					continue;
				}
				if (dist < monster->GetMoveRange())
				{
					monster->SetState(MonsterState::Move);
					monster->RotationYToPointDelta(rotationSpeed, paladin->Position());
					monster->MoveToDirectionDelta(-moveSpeed, Move::Direction, 0);
				}
			}
		}
	}
}

void PaladinGame::AttackCollision()
{
	if (paladin->GetState() == State::Attack)
	{
		for (Vanguard* monster : monsters)
		{
			if (monster->GetState() == MonsterState::Hit) continue;
			if (Manifold::Solve(paladin->GetSwordCollider(), monster->GetCollider()))
			{
				monster->SetState(MonsterState::Hit);
				monster->Damaged(paladin->GetDamage());
			}
		}
	}
	for (Vanguard* monster : monsters)
	{
		if (monster->GetState() == MonsterState::Attack)
		{
			if (paladin->GetState() == State::Hit) continue;
			if (Manifold::Solve(monster->GetHandCollider(), paladin->GetCollider()))
			{
				paladin->SetState(State::Hit);
				paladin->Damaged(monster->GetDamage());
			}
		}
	}
}
