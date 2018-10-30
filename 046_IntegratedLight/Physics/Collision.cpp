#include "stdafx.h"
#include "Collision.h"
#include "Collider.h"
#include "RayCollider.h"
#include "SphereCollider.h"
#include "CubeCollider.h"
#include "CapsuleCollider.h"

CollisionCallback Dispatch[Shape::eCount][Shape::eCount] =
{
	{
		RaytoRay, RaytoSphere, RaytoCube, RaytoCapsule
	},
	{
		SpheretoRay, SpheretoSphere, SpheretoCube, SpheretoCapsule
	},
	{
		CubetoRay, CubetoSphere, CubetoCube, CubetoCapsule
	},
	{
		CapsuletoRay, CapsuletoSphere, CapsuletoCube, CapsuletoCapsule
	},
};

bool RaytoRay(Collider * a, Collider * b)
{
	return false;
}

bool RaytoSphere(Collider * a, Collider * b)
{
	return false;
}

bool RaytoCube(Collider * a, Collider * b)
{
	return false;
}

bool RaytoCapsule(Collider * a, Collider * b)
{
	return false;
}

bool SpheretoRay(Collider * a, Collider * b)
{
	return false;
}

bool SpheretoSphere(Collider * a, Collider * b)
{
	D3DXVECTOR3 Apos, Bpos, AToB;
	float Aradius, Bradius;
	float distance;

	{
		D3DXMATRIX Amat, Bmat;

		Amat = a->Transform();
		Apos = D3DXVECTOR3(Amat._41, Amat._42, Amat._43);
		Aradius = a->GetRadius();

		Bmat = b->Transform();
		Bpos = D3DXVECTOR3(Bmat._41, Bmat._42, Bmat._43);
		Bradius = a->GetRadius();
	}

	AToB = Bpos - Apos;
	distance = D3DXVec3Length(&AToB);
	if ((Aradius + Bradius) < distance)
		return false;
	return true;
}

bool SpheretoCube(Collider * a, Collider * b)
{
	return CubetoSphere(b, a);
}

bool SpheretoCapsule(Collider * a, Collider * b)
{
	return false;
}

bool CubetoRay(Collider * a, Collider * b)
{
	return false;
}

bool CubetoSphere(Collider * a, Collider * b)
{
	D3DXVECTOR3 Apos, Bpos, AToB;
	D3DXVECTOR3 Aaxis[3];
	float AaxisLen[3], Bradius;
	float r[3];

	{
		D3DXMATRIX Amat, Bmat;
		Amat = a->Transform();

		Math::GetOrientedData(Aaxis, Apos, Amat);

		D3DXVECTOR3 Ascale;
		Ascale = a->Scale();

		AaxisLen[0] = Ascale.x / 2.0f;	AaxisLen[1] = Ascale.y / 2.0f;	AaxisLen[2] = Ascale.z / 2.0f;

		Bmat = b->Transform();
		Bpos = D3DXVECTOR3(Bmat._41, Bmat._42, Bmat._43);
		Bradius = b->GetRadius();
	}

	AToB = Bpos - Apos;

	for (size_t i = 0; i < 3; i++)
	{
		r[i] = D3DXVec3Dot(&AToB, &Aaxis[i]);

		if (-(AaxisLen[i] - Bradius) > r[i])
			return false;
		if ((AaxisLen[i] - Bradius) < r[i])
			return false;
	}

	return true;
}

bool CubetoCube(Collider * a, Collider * b)
{
	double c[3][3];
	double absC[3][3];
	double d[3];
	double r0, r1, r;
	int i;
	const double cutoff = 0.999999;
	bool existsParallelPair = false;

	D3DXVECTOR3 Aaxis[3], Baxis[3];
	double AaxisLen[3], BaxisLen[3];
	D3DXVECTOR3 APos, BPos;

	{
		D3DXMATRIX Amat, Bmat;
		Amat = a->Transform();
		Bmat = b->Transform();

		Math::GetOrientedData(Aaxis, APos, Amat);
		Math::GetOrientedData(Baxis, BPos, Bmat);

		D3DXVECTOR3 Ascale, Bscale;
		Ascale = a->Scale();
		Bscale = b->Scale();

		AaxisLen[0] = Ascale.x / 2.0f;	AaxisLen[1] = Ascale.y / 2.0f;	AaxisLen[2] = Ascale.z / 2.0f;
		BaxisLen[0] = Bscale.x / 2.0f;	BaxisLen[1] = Bscale.y / 2.0f;	BaxisLen[2] = Bscale.z / 2.0f;
	}

	D3DXVECTOR3 diff = APos - BPos;


	for (i = 0; i < 3; ++i)
	{
		c[0][i] = D3DXVec3Dot(&Aaxis[0], &Baxis[i]);
		absC[0][i] = abs(c[0][i]);
		if (absC[0][i] > cutoff)
			existsParallelPair = true;
	}


	d[0] = D3DXVec3Dot(&diff, &Aaxis[0]);
	r = abs(d[0]);
	r0 = AaxisLen[0];
	r1 = BaxisLen[0] * absC[0][0] + BaxisLen[1] * absC[0][1] + BaxisLen[2] * absC[0][2];
	if (r > r0 + r1)
	{
		return false;
	}
	for (i = 0; i < 3; ++i)
	{
		c[1][i] = D3DXVec3Dot(&Aaxis[1], &Baxis[i]);
		absC[1][i] = abs(c[1][i]);
		if (absC[1][i] > cutoff)
			existsParallelPair = true;
	}
	d[1] = D3DXVec3Dot(&diff, &Aaxis[1]);
	r = abs(d[1]);
	r0 = AaxisLen[1];
	r1 = BaxisLen[0] * absC[1][0] + BaxisLen[1] * absC[1][1] + BaxisLen[2] * absC[1][2];
	if (r > r0 + r1)
	{
		return false;
	}
	for (i = 0; i < 3; ++i)
	{
		c[2][i] = D3DXVec3Dot(&Aaxis[2], &Baxis[i]);
		absC[2][i] = abs(c[2][i]);
		if (absC[2][i] > cutoff)
			existsParallelPair = true;
	}
	d[2] = D3DXVec3Dot(&diff, &Aaxis[2]);
	r = abs(d[2]);
	r0 = AaxisLen[2];
	r1 = BaxisLen[0] * absC[2][0] + BaxisLen[1] * absC[2][1] + BaxisLen[2] * absC[2][2];
	if (r > r0 + r1)
	{
		return false;
	}
	r = abs(D3DXVec3Dot(&diff, &Baxis[0]));
	r0 = AaxisLen[0] * absC[0][0] + AaxisLen[1] * absC[1][0] + AaxisLen[2] * absC[2][0];
	r1 = BaxisLen[0];
	if (r > r0 + r1)
	{
		return false;
	}


	r = abs(D3DXVec3Dot(&diff, &Baxis[1]));
	r0 = AaxisLen[0] * absC[0][1] + AaxisLen[1] * absC[1][1] + AaxisLen[2] * absC[2][1];
	r1 = BaxisLen[1];
	if (r > r0 + r1)
	{
		return false;
	}


	r = abs(D3DXVec3Dot(&diff, &Baxis[2]));
	r0 = AaxisLen[0] * absC[0][2] + AaxisLen[1] * absC[1][2] + AaxisLen[2] * absC[2][2];
	r1 = BaxisLen[2];
	if (r > r0 + r1)
	{
		return false;
	}


	if (existsParallelPair == true)
	{
		return true;
	}


	r = abs(d[2] * c[1][0] - d[1] * c[2][0]);
	r0 = AaxisLen[1] * absC[2][0] + AaxisLen[2] * absC[1][0];
	r1 = BaxisLen[1] * absC[0][2] + BaxisLen[2] * absC[0][1];
	if (r > r0 + r1)
	{
		return false;
	}


	r = abs(d[2] * c[1][1] - d[1] * c[2][1]);
	r0 = AaxisLen[1] * absC[2][1] + AaxisLen[2] * absC[1][1];
	r1 = BaxisLen[0] * absC[0][2] + BaxisLen[2] * absC[0][0];
	if (r > r0 + r1)
	{
		return false;
	}


	r = abs(d[2] * c[1][2] - d[1] * c[2][2]);
	r0 = AaxisLen[1] * absC[2][2] + AaxisLen[2] * absC[1][2];
	r1 = BaxisLen[0] * absC[0][1] + BaxisLen[1] * absC[0][0];
	if (r > r0 + r1)
	{
		return false;
	}


	r = abs(d[0] * c[2][0] - d[2] * c[0][0]);
	r0 = AaxisLen[0] * absC[2][0] + AaxisLen[2] * absC[0][0];
	r1 = BaxisLen[1] * absC[1][2] + BaxisLen[2] * absC[1][1];
	if (r > r0 + r1)
	{
		return false;
	}


	r = abs(d[0] * c[2][1] - d[2] * c[0][1]);
	r0 = AaxisLen[0] * absC[2][1] + AaxisLen[2] * absC[0][1];
	r1 = BaxisLen[0] * absC[1][2] + BaxisLen[2] * absC[1][0];
	if (r > r0 + r1)
	{
		return false;
	}


	r = abs(d[0] * c[2][2] - d[2] * c[0][2]);
	r0 = AaxisLen[0] * absC[2][2] + AaxisLen[2] * absC[0][2];
	r1 = BaxisLen[0] * absC[1][1] + BaxisLen[1] * absC[1][0];
	if (r > r0 + r1)
	{
		return false;
	}


	r = abs(d[1] * c[0][0] - d[0] * c[1][0]);
	r0 = AaxisLen[0] * absC[1][0] + AaxisLen[1] * absC[0][0];
	r1 = BaxisLen[1] * absC[2][2] + BaxisLen[2] * absC[2][1];
	if (r > r0 + r1)
	{
		return false;
	}


	r = abs(d[1] * c[0][1] - d[0] * c[1][1]);
	r0 = AaxisLen[0] * absC[1][1] + AaxisLen[1] * absC[0][1];
	r1 = BaxisLen[0] * absC[2][2] + BaxisLen[2] * absC[2][0];
	if (r > r0 + r1)
	{
		return false;
	}


	r = abs(d[1] * c[0][2] - d[0] * c[1][2]);
	r0 = AaxisLen[0] * absC[1][2] + AaxisLen[1] * absC[0][2];
	r1 = BaxisLen[0] * absC[2][1] + BaxisLen[1] * absC[2][0];
	if (r > r0 + r1)
	{
		return false;
	}

	return true;
}

bool CubetoCapsule(Collider * a, Collider * b)
{
	return false;
}

bool CapsuletoRay(Collider * a, Collider * b)
{
	return false;
}

bool CapsuletoSphere(Collider * a, Collider * b)
{
	return false;
}

bool CapsuletoCube(Collider * a, Collider * b)
{
	return false;
}

bool CapsuletoCapsule(Collider * a, Collider * b)
{
	CapsuleCollider* A = reinterpret_cast<CapsuleCollider*>(a);
	CapsuleCollider* B = reinterpret_cast<CapsuleCollider*>(b);

	float r1, r2, s, t;
	D3DXVECTOR3 p1, q1, p2, q2, c1, c2;
	A->GetCapsule(&p1, &q1, &r1);
	B->GetCapsule(&p2, &q2, &r2);
	float radius = r1 + r2;
	
	D3DXVECTOR3 d1 = q1 - p1; // Ä¸½¶1 µÎ Æ÷ÀÎÆ®°£ÀÇ ¹æÇâº¤ÅÍ
	D3DXVECTOR3 d2 = q2 - p2; // Ä¸½¶2 µÎ Æ÷ÀÎÆ®°£ÀÇ ¹æÇâº¤ÅÍ
	D3DXVECTOR3 r = p1 - p2;
	float e1 = D3DXVec3Dot(&d1, &d1); // Squared length of segment S1, always nonnegative
	float e2 = D3DXVec3Dot(&d2, &d2); // Squared length of segment S2, always nonnegative
	float f = D3DXVec3Dot(&d2, &r);
	// Check if either or both segments degenerate into points
	if (e1 <= D3DX_16F_EPSILON && e2 <= D3DX_16F_EPSILON) {
		// Both segments degenerate into points
		s = t = 0.0f;
		c1 = p1;
		c2 = p2;
		return D3DXVec3Dot(&D3DXVECTOR3(c1 - c2), &D3DXVECTOR3(c1 - c2)) <= radius * radius;
	}
	if (e1 <= D3DX_16F_EPSILON) {
		// First segment degenerates into a point
		s = 0.0f;
		t = f / e2; // s = 0 => t = (b*s + f) / e = f / e
		t = Math::Clamp(t, 0.0f, 1.0f);
	}
	else {
		float c = D3DXVec3Dot(&d1, &r);
		if (e2 <= D3DX_16F_EPSILON) {
			// Second segment degenerates into a point
			t = 0.0f;
			s = Math::Clamp(-c / e1, 0.0f, 1.0f); // t = 0 => s = (b*t - c) / a = -c / a
		}
		else {
			// The general nondegenerate case starts here
			float b = D3DXVec3Dot(&d1, &d2);
			float denom = e1*e2 - b*b; // Always nonnegative
									 // If segments not parallel, compute closest point on L1 to L2 and
									 // clamp to segment S1. Else pick arbitrary s (here 0)
			if (denom != 0.0f) {
				s = Math::Clamp((b*f - c*e2) / denom, 0.0f, 1.0f);
			}
			else s = 0.0f;
			// Compute point on L2 closest to S1(s) using
			// t = Dot((P1 + D1*s) - P2,D2) / Dot(D2,D2) = (b*s + f) / e
			t = (b*s + f) / e2;
			// If t in [0,1] done. Else clamp t, recompute s for the new value
			// of t using s = Dot((P2 + D2*t) - P1,D1) / Dot(D1,D1)= (t*b - c) / a
			// and clamp s to [0, 1]
			if (t < 0.0f) {
				t = 0.0f;
				s = Math::Clamp(-c / e1, 0.0f, 1.0f);
			}
			else if (t > 1.0f) {
				t = 1.0f;
				s = Math::Clamp((b - c) / e1, 0.0f, 1.0f);
			}
		}
	}
	c1 = p1 + d1 * s;
	c2 = p2 + d2 * t;
	return D3DXVec3Dot(&D3DXVECTOR3(c1 - c2), &D3DXVECTOR3(c1 - c2)) <= radius * radius;
}