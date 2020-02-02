#ifndef STATE_PLAYER_ATTACK_TOP_H_
#define STATE_PLAYER_ATTACK_TOP_H_

#include "state_player_attack.h"
class CStatePlayerAttackTop : public CStatePlayerAttack
{
public:
	CStatePlayerAttackTop(CPlayer* pPlayer);
	virtual ~CStatePlayerAttackTop();
	virtual void Update(CPlayer* pPlayer) override{}
	virtual void UpdateAttackState(CStatePlayerAttack* pAttackState, CPlayer* pPlayer) override;

private:
	CStatePlayerAttackTop() {} // デフォルトコンストラクタ封印
	void Move(CPlayer* pPlayer);

private:
	int m_FrameCounter;
	Vector3 m_Velocity;
	Vector3 m_DirFront;

};

#endif // !STATE_PLAYER_ATTACK_TOP_H_
