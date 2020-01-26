#pragma once
#include "state_player.h"

class CStatePlayerDodge : public CStatePlayer
{
public:
	CStatePlayerDodge(CPlayer* pPlayer);
	virtual ~CStatePlayerDodge();
	virtual void Update(CPlayer* pPlayer) override;

private:
	int m_FrameCounter;

};

