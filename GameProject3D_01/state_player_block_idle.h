#ifndef STATE_PLAYER_BLOCK_IDLE_H_
#define STATE_PLAYER_BLOCK_IDLE_H_

#include "state_player_block.h"
class CStatePlayerBlockIdle : public CStatePlayerBlock
{
public:
	CStatePlayerBlockIdle(CPlayer* pPlayer);
	virtual ~CStatePlayerBlockIdle();
	virtual void Update(CPlayer* pPlayer) override {}
	virtual void UpdateBlockState(CStatePlayerBlock* pStateBlock, CPlayer* pPlayer);

private:
	CStatePlayerBlockIdle() {} // �f�t�H���g�R���X�g���N�^����

private:

};

#endif // !STATE_PLAYER_BLOCK_IDLE_H_
