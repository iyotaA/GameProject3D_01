#ifndef STATE_PLAYER_RUN_H_
#define STATE_PLAYER_RUN_H_

#include "state_player_move.h"

class CStatePlayerRun : public CStatePlayerMove
{
public:
	CStatePlayerRun(CPlayer* pPlayer);
	virtual ~CStatePlayerRun();
	virtual void Update(CPlayer* pPlayer) override;

private:
	CStatePlayerRun(){} // デフォルトコンストラクタ封印
	float m_MoveSpeed;
	float m_AnimationSpeed;

};


#endif // !STATE_PLAYER_RUN_H_

