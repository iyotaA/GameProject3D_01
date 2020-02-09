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
	void Move(CPlayer* pPlayer);
	void Action(CPlayer* pPlayer);

	bool MoveEntry() {
		return CInput::GetIsInputStick(LEFT_STICK) ||
			CInput::GetKeyPress('W') || CInput::GetKeyPress('A') ||
			CInput::GetKeyPress('S') || CInput::GetKeyPress('D');
	}

private:
	float m_LerpStart;
	float m_LerpEnd;
	float m_FrameCounter;
	float m_MoveSpeed;
	Vector3 m_Volocity;

};

#endif // !STATE_PLAYER_IDLE_H_
