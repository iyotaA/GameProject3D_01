#ifndef STATE_PLAYER_SHEATHE_SWORD_H_
#define STATE_PLAYER_SHEATHE_SWORD_H_


#include "state_player.h"
class CStatePlayerSheatheSword : public CStatePlayer
{
public:
	CStatePlayerSheatheSword(CPlayer* pPlayer, bool isMove);
	~CStatePlayerSheatheSword();
	void Update(CPlayer* pPlayer) override;

private:
	CStatePlayerSheatheSword() {}	// �f�t�H���g�R���X�g���N�^����

private:
	int m_FrameCounter;
	bool  m_IsMove;

};

#endif // !STATE_PLAYER_SHEATHE_SWORD_H_
