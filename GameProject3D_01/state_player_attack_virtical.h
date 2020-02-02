#ifndef STATE_PLAYER_ATTACK_VIRTICAL_H_
#define STATE_PLAYER_ATTACK_VIRTICAL_H_

#include "state_player_attack.h"
class CStatePlayerAttackVirtical : public CStatePlayerAttack
{
public:
	CStatePlayerAttackVirtical(CPlayer* pPlayer);
	virtual ~CStatePlayerAttackVirtical();
	virtual void Update(CPlayer* pPlayer) override {}
	virtual void UpdateAttackState(CStatePlayerAttack* pAttackState, CPlayer* pPlayer) override;

private:
	CStatePlayerAttackVirtical() {} // �f�t�H���g�R���X�g���N�^����
	void Move(CPlayer* pPlayer);

private:
	int m_FrameCounter;
	Vector3 m_Velocity;
	Vector3 m_DirFront;

};

#endif // !STATE_PLAYER_ATTACK_VIRTICAL_H_
