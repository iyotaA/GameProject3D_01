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
	CStatePlayerDrawSword() {}	// �f�t�H���g�R���X�g���N�^����

private:
	float m_FrameCounter;
	bool  m_IsMove;

};

#endif // !STATE_PLAYER_DRAW_SWORD_H_
