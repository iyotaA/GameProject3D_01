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
	CStatePlayerMove(){} // デフォルトコンストラクタ封印
	bool MoveEntry() {
		return CInput::GetIsInputStick(LEFT_STICK) ||
			CInput::GetKeyPress('W') || CInput::GetKeyPress('A') ||
			CInput::GetKeyPress('S') || CInput::GetKeyPress('D');
	}

private:
	CStatePlayerMove* m_pStateMove;
	float m_Lerp_t;
	float m_LerpStart;
	float m_LerpEnd;
	float m_Counter;
};


#endif // !STATE_PLAYER_MOVE_H_
