#ifndef DAMAGE_H_
#define DAMAGE_H_

class CCollisionSphere;

class CDamage
{
private:
	short m_HitPoint;
	short m_AttackValue;
	CCollisionSphere* m_CollisionSphere;

public:
	CDamage(short hitPoint, short attackValue) {
		m_HitPoint    = hitPoint;
		m_AttackValue = attackValue;
		m_CollisionSphere = new CCollisionSphere;
	}

	void ReceiveDamage(float damageValue) {	m_HitPoint -= damageValue; }
	void DoDamage(CDamage* object) { object->ReceiveDamage(m_AttackValue); }
	CCollisionSphere* GetCollisionSphere(void) { return m_CollisionSphere; }
	bool IsAlive(void) {
		if (m_HitPoint > 0) return true;
		else return false;
	}
	void DebugDraw(void) {
		ImGui::Text("HP = %d", m_HitPoint);
	}
};


#endif // !DAMAGE_H_


