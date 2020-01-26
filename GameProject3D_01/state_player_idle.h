#ifndef STATE_PLAYER_IDLE_H_
#define STATE_PLAYER_IDLE_H_

#include "state_player.h"

class CStatePlayerIdle : public CStatePlayer
{
public:
	CStatePlayerIdle(CPlayer* pPlayer);
	virtual ~CStatePlayerIdle();
	virtual void Update(CPlayer* pPlayer) override;

private:
	CStatePlayerIdle() {}	// �f�t�H���g�R���X�g���N�^����
};

#endif // !STATE_PLAYER_IDLE_H_
