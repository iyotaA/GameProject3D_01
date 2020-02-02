#ifndef STATE_PLAYER_ATTACK_HORIZONTAL_H_
#define STATE_PLAYER_ATTACK_HORIZONTAL_H_

#include "state_player_attack.h"
class CStatePlayerAttackHorizontal : public CStatePlayerAttack
{
public:
	CStatePlayerAttackHorizontal(CPlayer* pPlayer);
	virtual ~CStatePlayerAttackHorizontal();
	virtual void Update(CPlayer* pPlayer) override{}
	virtual void UpdateAttackState(CStatePlayerAttack* pAttackState, CPlayer* pPlayer) override;

private:
	CStatePlayerAttackHorizontal() {} // デフォルトコンストラクタ封印
	void Move(CPlayer* pPlayer);

private:
	int m_FrameCounter;
	Vector3 m_Velocity;
	Vector3 m_DirFront;

};

#endif // !STATE_PLAYER_ATTACK_HORIZONTAL_H_
