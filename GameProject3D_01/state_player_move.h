#ifndef STATE_PLAYER_MOVE_H_
#define STATE_PLAYER_MOVE_H_

#include "state_player.h"

class CStatePlayerMove : public CStatePlayer
{
public:
	CStatePlayerMove(CPlayer* pPlayer);
	virtual ~CStatePlayerMove();
	virtual void Update(CPlayer* pPlayer) override;
	virtual void UpdateMoveState(CStatePlayerMove* pAttackState, CPlayer* pPlayer) {};

	void ChangeState(CStatePlayerMove* pMoveState);

protected:
	CStatePlayerMove(){} // デフォルトコンストラクタ封印
	bool MoveEntry() {
		return CInput::GetIsInputStick(LEFT_STICK) ||
			CInput::GetKeyPress('W') || CInput::GetKeyPress('A') ||
			CInput::GetKeyPress('S') || CInput::GetKeyPress('D');
	}

private:
	bool Action(CPlayer* pPlayer);

private:
	CStatePlayerMove* m_pStateMove;
	int m_FrameCounter;

};


#endif // !STATE_PLAYER_MOVE_H_
