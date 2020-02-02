#ifndef STATE_PLAYER_DAMAGE_H_
#define STATE_PLAYER_DAMAGE_H_

#include "state_player.h"
class CStatePlayerDamage : public CStatePlayer
{
public:
	CStatePlayerDamage(CPlayer* pPlayer);
	virtual ~CStatePlayerDamage();
	virtual void Update(CPlayer* pPlayer) override;
	virtual void UpdateDamageState(CStatePlayerDamage* pStateDamage, CPlayer* pPlayer) {};

	void ChangeState(CStatePlayerDamage* pStateDamage);

protected:
	CStatePlayerDamage() {} // デフォルトコンストラクタ封印

private:
	CStatePlayerDamage* m_pStateDamage;

};

class CStatePlayerDamageNone : public CStatePlayerDamage
{
public:
	CStatePlayerDamageNone(CPlayer* pPlayer) {}
	virtual ~CStatePlayerDamageNone() {}
	virtual void Update(CPlayer* pPlayer) override {}
	virtual void UpdateMoveState(CStatePlayerDamage* pStateDamage, CPlayer* pPlayer) {}
};

#endif // !STATE_PLAYER_DAMAGE_H_
