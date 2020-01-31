#include "game_objects_all.h"
#include "state_player_idle.h"
#include "state_player_move_run.h"
#include "game_objects_all.h"
#include "modelAnimation.h"
#include "player.h"
#include "MathFunc.h"

CStatePlayerIdle::CStatePlayerIdle(CPlayer* pPlayer)
	: m_Lerp_t(0.0f)
	, m_LerpStart(CCameraManager::GetCamera()->GetLengthToAt())
	, m_LerpEnd(6.0f)
	, m_Counter(0.0f)
{
	pPlayer->SetAnimation(0, 1.0f);
	pPlayer->SetAnimationSpeed(1.0f);
}

CStatePlayerIdle::~CStatePlayerIdle()
{

}

void CStatePlayerIdle::Update(CPlayer* pPlayer)
{
	if (MoveEntry()) {
		pPlayer->ChangeState(new CStatePlayerMove(pPlayer));
	}

	// 徐々にカメラズームイン
	CCamera* camera = CCameraManager::GetCamera();
	camera->SetLengthToAt(lerp(m_LerpStart, m_LerpEnd, m_Lerp_t));
	m_Lerp_t = sinf(m_Counter * 3.14f / 180.0f);
	m_Counter += 0.5f;
	if (m_Counter >= 90.0f) { m_Counter = 90.0f; }
}
