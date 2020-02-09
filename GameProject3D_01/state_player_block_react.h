#ifndef STATE_PLAYER_BLOCK_REACT_H_
#define STATE_PLAYER_BLOCK_REACT_H_

#include "state_player_block.h"
class CStatePlayerBlockReact : public CStatePlayerBlock
{
public:
	CStatePlayerBlockReact(CStatePlayerBlock* pStateBlock, CPlayer* pPlayer);
	virtual ~CStatePlayerBlockReact();
	virtual void Update(CPlayer* pPlayer) override {}
	virtual void UpdateBlockState(CStatePlayerBlock* pStateBlock, CPlayer* pPlayer);

private:
	CStatePlayerBlockReact() {} // デフォルトコンストラクタ封印
	void Move(CPlayer* pPlayer);

private:
	int m_FrameCounter;
	Vector3 m_Velocity;
	Vector3 m_ForceDirection;

};

#endif // !STATE_PLAYER_BLOCK_REACT_H_
