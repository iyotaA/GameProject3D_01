#ifndef STATE_PLAYER_ATTACK_H_
#define STATE_PLAYER_ATTACK_H_


#include "state_player.h"

class CStatePlayerAttack : public CStatePlayer
{
public:
	CStatePlayerAttack(CPlayer* pPlayer);
	virtual ~CStatePlayerAttack();
	virtual void Update(CPlayer* pPlayer) override;
	virtual void UpdateAttackState(CStatePlayerAttack* pAttackState, CPlayer* pPlayer) {};

	void ChangeState(CStatePlayerAttack* pAttackState);

protected:
	CStatePlayerAttack() {} // デフォルトコンストラクタ封印
	bool MoveEntry() {
		return CInput::GetIsInputStick(LEFT_STICK) ||
			CInput::GetKeyPress('W') || CInput::GetKeyPress('A') ||
			CInput::GetKeyPress('S') || CInput::GetKeyPress('D');
	}

private:
	CStatePlayerAttack* m_pStateAttack;
	float m_Lerp_t;
	float m_LerpStart;
	float m_LerpEnd;
	float m_Counter;

};

class CStatePlayerAttackNone : public CStatePlayerAttack
{
public:
	CStatePlayerAttackNone(CPlayer* pPlayer){}
	virtual ~CStatePlayerAttackNone(){}
	virtual void Update(CPlayer* pPlayer) override{}
	virtual void UpdateAttackState(CStatePlayerAttack* pAttackState, CPlayer* pPlayer) {};
};

#endif // !STATE_PLAYER_ATTACK_H_
