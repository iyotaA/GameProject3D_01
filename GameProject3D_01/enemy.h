#ifndef ENEMY_H_
#define ENEMY_H_

class CEnemy : public CGameObject
{
private:
	CModel* m_pModel;
	CCollisionSphere* m_CollisionSphere;
	CDamage* m_DamageManager;

public:

	void Init();
	void Uninit();
	void Update();
	void Draw();

	const XMFLOAT3 GetPosition() { return m_Position; }
	void SetPosition(XMFLOAT3 position) { m_Position = position; }
	CCollisionSphere* GetCollisionSphere(void) { return m_CollisionSphere; }
	CDamage* GetDamageManager(void) { return m_DamageManager; }

};


#endif // !ENEMY_H_

