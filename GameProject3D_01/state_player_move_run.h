#ifndef STATE_PLAYER_MOVE_RUN_H_
#define STATE_PLAYER_MOVE_RUN_H_

#include "state_player_move.h"

class CStatePlayerMoveRun : public CStatePlayerMove
{
public:
	CStatePlayerMoveRun(CPlayer* pPlayer, float StartSpeed);
	virtual ~CStatePlayerMoveRun();
	virtual void Update(CPlayer* pPlayer) override{}
	virtual void UpdateMoveState(CStatePlayerMove* pMoveState, CPlayer* pPlayer) override;

private:
	CStatePlayerMoveRun(){} // デフォルトコンストラクタ封印
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


#endif // !STATE_PLAYER_MOVE_RUN_H_

