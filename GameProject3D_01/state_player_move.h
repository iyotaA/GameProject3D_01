#ifndef STATE_PLAYER_MOVE_H_
#define STATE_PLAYER_MOVE_H_

#include "state_player.h"

class CStatePlayerMove : public CStatePlayer
{
public:
	CStatePlayerMove(CPlayer* pPlayer);
	virtual ~CStatePlayerMove();
	virtual void Update(CPlayer* pPlayer) override;

protected:
	CStatePlayerMove(){} // �f�t�H���g�R���X�g���N�^����

private:
	CStatePlayerMove* m_pStateMove;
};


#endif // !STATE_PLAYER_MOVE_H_
