#ifndef STATE_PLAYER_BLOCK_H_
#define STATE_PLAYER_BLOCK_H_


#include "state_player.h"
class CStatePlayerBlock : public CStatePlayer
{
public:
	CStatePlayerBlock(CPlayer* pPlayer);
	virtual ~CStatePlayerBlock();
	virtual void Update(CPlayer* pPlayer) override;
	virtual void UpdateBlockState(CStatePlayerBlock* pStateBlock, CPlayer* pPlayer) {};

	void ChangeState(CStatePlayerBlock* pStateBlock);
	void StartGardEnable() { m_FrameCountStart = true; }
	bool& FrameCountStart() { return m_FrameCountStart; }

protected:
	CStatePlayerBlock() {} // デフォルトコンストラクタ封印

private:
	CStatePlayerBlock* m_pStateBlock;
	int m_FrameBeteweenGard;
	bool m_FrameCountStart;
};

class CStatePlayerBlockNone : public CStatePlayerBlock
{
public:
	CStatePlayerBlockNone(CPlayer* pPlayer) {}
	virtual ~CStatePlayerBlockNone() {}
	virtual void Update(CPlayer* pPlayer) override {}
	virtual void UpdateBlockState(CStatePlayerBlock* pStateBlock, CPlayer* pPlayer) {}
};

#endif // !STATE_PLAYER_BLOCK_H_
