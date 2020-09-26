#ifndef STATE_PLAYER_DAMAGE_LARGE_H_
#define STATE_PLAYER_DAMAGE_LARGE_H_

#include "state_player_damage.h"
class CStatePlayerDamageSmall : public CStatePlayerDamage
{
public:
	CStatePlayerDamageSmall(CPlayer* pPlayer, int damage);
	virtual ~CStatePlayerDamageSmall();
	virtual void Update(CPlayer* pPlayer) override {}
	virtual void UpdateDamageState(CStatePlayerDamage* pStateDamage, CPlayer* pPlayer);

private:
	CStatePlayerDamageSmall() {} // デフォルトコンストラクタ封印

private:
	int m_FrameCounter;

};

#endif // !STATE_PLAYER_DAMAGE_LARGE_H_
