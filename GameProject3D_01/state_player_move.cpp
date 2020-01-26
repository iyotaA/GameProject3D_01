#include "main.h"
#include "state_player_move.h"
#include "state_player_move_run.h"
#include "state_player_idle.h"
#include "game_objects_all.h"
#include "modelAnimation.h"
#include "player.h"

CStatePlayerMove::CStatePlayerMove(CPlayer* pPlayer)
	:m_pStateMove(new CStatePlayerRun(pPlayer))
{

}

CStatePlayerMove::~CStatePlayerMove()
{
	delete m_pStateMove;
}

void CStatePlayerMove::Update(CPlayer* pPlayer)
{
	// 入力がされてなければ待機モーションに
	if (!pPlayer->PressMovingEntry()) {
		pPlayer->ChangeState(new CStatePlayerIdle(pPlayer));
		return;
	}

	// 更新
	m_pStateMove->Update(pPlayer);
}
