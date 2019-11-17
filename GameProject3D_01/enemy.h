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

	CCollisionSphere* GetCollisionSphere(void) { return m_CollisionSphere; }
	CDamage* GetDamageManager(void) { return m_DamageManager; }

};


#endif // !ENEMY_H_

