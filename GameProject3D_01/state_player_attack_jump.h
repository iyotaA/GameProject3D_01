#ifndef STATE_PLAYER_ATTACK_JUMP_H_
#define STATE_PLAYER_ATTACK_JUMP_H_

#include "state_player_attack.h"
class CStatePlayerAttackJump : public CStatePlayerAttack
{
public:
	CStatePlayerAttackJump(CPlayer* pPlayer);
	virtual ~CStatePlayerAttackJump();
	virtual void Update(CPlayer* pPlayer) override{}
	virtual void UpdateAttackState(CStatePlayerAttack* pAttackState, CPlayer* pPlayer) override;

private:
	CStatePlayerAttackJump() {} // デフォルトコンストラクタ封印
	void Move(CPlayer* pPlayer);

private:
	int m_FrameCounter;
	Vector3 m_Velocity;
	Vector3 m_DirFront;

};

#endif // !STATE_PLAYER_ATTACK_JUMP_H_
