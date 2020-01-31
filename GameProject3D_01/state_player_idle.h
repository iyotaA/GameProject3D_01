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

private:
	float m_Lerp_t;
	float m_LerpStart;
	float m_LerpEnd;
	float m_Counter;

};

#endif // !STATE_PLAYER_IDLE_H_
