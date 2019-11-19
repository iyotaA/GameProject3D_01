#ifndef BULLET_H_
#define BULLET_H_

class CModel;
class CCollisionSphere;

class CBullet : public CGameObject
{
private:
	CModel* m_pModel;
	Vector3X3 m_Direction;
	CCollisionSphere* m_CollisionSphere;

public:

	void Init();
	void Uninit();
	void Update();
	void Draw();

	void SetDirection(Vector3X3* direction) { m_Direction = *direction; }
};

#endif // !BULLET_H_
