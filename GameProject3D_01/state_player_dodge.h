#ifndef STATE_PLAYER_DODGE_H_
#define STATE_PLAYER_DODGE_H_


#include "state_player.h"

class CStatePlayerDodge : public CStatePlayer
{
public:
	CStatePlayerDodge(CPlayer* pPlayer);
	virtual ~CStatePlayerDodge();
	virtual void Update(CPlayer* pPlayer) override;

private:
	CStatePlayerDodge() {}
	void Move(CPlayer* pPlayer);

	bool MoveEntry() {
		return CInput::GetIsInputStick(LEFT_STICK) ||
			CInput::GetKeyPress('W') || CInput::GetKeyPress('A') ||
			CInput::GetKeyPress('S') || CInput::GetKeyPress('D');
	}

private:
	int m_FrameCounter;
	Vector3 m_Velocity;
	Vector3 m_DirFront;

};

#endif // !STATE_PLAYER_DODGE_H_
