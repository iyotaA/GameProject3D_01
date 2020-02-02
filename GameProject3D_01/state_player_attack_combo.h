#ifndef STATE_PLAYER_ATTACK_COMBO_H_
#define STATE_PLAYER_ATTACK_COMBO_H_

#include "state_player_attack.h"
class CStatePlayerAttackCombo :	public CStatePlayerAttack
{
public:
	CStatePlayerAttackCombo(CPlayer* pPlayer);
	virtual ~CStatePlayerAttackCombo();
	virtual void Update(CPlayer* pPlayer) override{}
	virtual void UpdateAttackState(CStatePlayerAttack* pAttackState, CPlayer* pPlayer) override;

private:
	CStatePlayerAttackCombo() {} // デフォルトコンストラクタ封印
	void Move(CPlayer* pPlayer);

private:
	int m_FrameCounter;
	Vector3 m_Velocity;
	Vector3 m_DirFront;

};

#endif // !STATE_PLAYER_ATTACK_COMBO_H_
