#ifndef STATE_PLAYER_ATTACK_LOW_H_
#define STATE_PLAYER_ATTACK_LOW_H_

#include "state_player_attack.h"
class CStatePlayerAttackLow : public CStatePlayerAttack
{
public:
	CStatePlayerAttackLow(CPlayer* pPlayer);
	virtual ~CStatePlayerAttackLow();
	virtual void Update(CPlayer* pPlayer) override{}
	virtual void UpdateAttackState(CStatePlayerAttack* pAttackState, CPlayer* pPlayer) override;

private:
	CStatePlayerAttackLow() {} // デフォルトコンストラクタ封印
	void Move(CPlayer* pPlayer);

private:
	int m_FrameCounter;
	Vector3 m_Velocity;
	Vector3 m_DirFront;

};

#endif // !STATE_PLAYER_ATTACK_LOW_H_
