#ifndef STATE_PLAYER_MOVE_DASH_H_
#define STATE_PLAYER_MOVE_DASH_H_


#include "state_player_move.h"
class CStatePlayerMoveDash : public CStatePlayerMove
{
public:
	CStatePlayerMoveDash(CPlayer* pPlayer, float StartSpeed);
	virtual ~CStatePlayerMoveDash();
	virtual void Update(CPlayer* pPlayer) override{}
	virtual void UpdateMoveState(CStatePlayerMove* pMoveState, CPlayer* pPlayer) override;

private:
	CStatePlayerMoveDash() {} // デフォルトコンストラクタ封印
	void Move(CPlayer* pPlayer);

private:
	float m_MoveSpeed;
	float m_StartSpeed;
	float m_TargetSpeed;
	float m_StartLength;
	float m_TargetLength;
	int   m_FrameCounter;
	Vector3 m_Volocity;

};

#endif // !STATE_PLAYER_MOVE_DASH_H_
