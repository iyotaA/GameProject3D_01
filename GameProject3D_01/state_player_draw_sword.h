#ifndef STATE_PLAYER_DRAW_SWORD_H_
#define STATE_PLAYER_DRAW_SWORD_H_

#include "state_player.h"
class CStatePlayerDrawSword : public CStatePlayer
{
public:
	CStatePlayerDrawSword(CPlayer* pPlayer, bool isMove);
	~CStatePlayerDrawSword();
	void Update(CPlayer* pPlayer) override;

private:
	CStatePlayerDrawSword() {}	// デフォルトコンストラクタ封印

private:
	float m_FrameCounter;
	bool  m_IsMove;

};

#endif // !STATE_PLAYER_DRAW_SWORD_H_
