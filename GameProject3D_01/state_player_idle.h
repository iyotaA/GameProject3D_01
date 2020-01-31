#ifndef STATE_PLAYER_IDLE_H_
#define STATE_PLAYER_IDLE_H_

#include "state_player.h"

class CStatePlayerIdle : public CStatePlayer
{
public:
	CStatePlayerIdle(CPlayer* pPlayer);
	~CStatePlayerIdle();
	void Update(CPlayer* pPlayer) override;

private:
	CStatePlayerIdle() {}	// デフォルトコンストラクタ封印
	bool MoveEntry() {
		return CInput::GetIsInputStick(LEFT_STICK) ||
			CInput::GetKeyPress('W') || CInput::GetKeyPress('A') ||
			CInput::GetKeyPress('S') || CInput::GetKeyPress('D');
	}

};

#endif // !STATE_PLAYER_IDLE_H_
