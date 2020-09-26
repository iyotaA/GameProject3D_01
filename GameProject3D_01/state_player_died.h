#ifndef STATE_PLAYER_DIED_H_
#define STATE_PLAYER_DIED_H_


#include "image.h"
#include "state_player.h"

class CStatePlayerDied :	public CStatePlayer
{
public:
	CStatePlayerDied(CPlayer* pPlayer);
	~CStatePlayerDied();
	void Update(CPlayer* pPlayer) override;

private:
	CStatePlayerDied() {}	// �f�t�H���g�R���X�g���N�^����

private:
	int m_FrameCounter;
	std::vector< CImage*> m_UI;

};

#endif // !STATE_PLAYER_DIED_H_
