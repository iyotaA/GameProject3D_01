#ifndef STATE_PLAYER_DAMAGE_SMALL_H_
#define STATE_PLAYER_DAMAGE_SMALL_H_

#include "state_player_damage.h"
class CStatePlayerDamageLarge : public CStatePlayerDamage
{
public:
	CStatePlayerDamageLarge(CPlayer* pPlayer);
	virtual ~CStatePlayerDamageLarge();
	virtual void Update(CPlayer* pPlayer) override{}
	virtual void UpdateDamageState(CStatePlayerDamage* pStateDamage, CPlayer* pPlayer);

private:
	CStatePlayerDamageLarge() {} // �f�t�H���g�R���X�g���N�^����
	void Move(CPlayer* pPlayer);

private:
	int m_FrameCounter;
	Vector3 m_Velocity;
	Vector3 m_DirFront;

};

#endif // !STATE_PLAYER_DAMAGE_SMALL_H_
