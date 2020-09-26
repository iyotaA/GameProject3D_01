#ifndef STATE_PLAYER_ATTACK_JUMP_H_
#define STATE_PLAYER_ATTACK_JUMP_H_

#include "state_player_attack.h"
class CStatePlayerAttackJump : public CStatePlayerAttack
{
public:
	CStatePlayerAttackJump(CPlayer* pPlayer, bool isMove);
	virtual ~CStatePlayerAttackJump();
	virtual void Update(CPlayer* pPlayer) override{}
	virtual void UpdateAttackState(CStatePlayerAttack* pAttackState, CPlayer* pPlayer) override;

private:
	CStatePlayerAttackJump() {} // �f�t�H���g�R���X�g���N�^����
	void Move(CPlayer* pPlayer);

private:
	int m_FrameCounter;
	Vector3 m_Velocity;
	Vector3 m_DirFront;
	bool	m_Move;

};

#endif // !STATE_PLAYER_ATTACK_JUMP_H_
