#include "game_objects_all.h"
#include "state_player_move.h"
#include "state_player_move_run.h"
#include "state_player_idle.h"
#include "modelAnimation.h"
#include "player.h"
#include "MathFunc.h"

CStatePlayerMove::CStatePlayerMove(CPlayer* pPlayer)
	: m_pStateMove(new CStatePlayerRun(pPlayer))
	, m_Lerp_t(0.0f)
	, m_LerpStart(CCameraManager::GetCamera()->GetLengthToAt())
	, m_LerpEnd(8.0f)
	, m_Counter(0.0f)
{
}

CStatePlayerMove::~CStatePlayerMove()
{
	delete m_pStateMove;
}

void CStatePlayerMove::Update(CPlayer* pPlayer)
{
	// ���͂�����ĂȂ���Αҋ@���[�V������
	if (!MoveEntry()) {
		pPlayer->ChangeState(new CStatePlayerIdle(pPlayer));
		return;
	}

	// �X�V
	m_pStateMove->Update(pPlayer);

	// ���X�ɃJ�����Y�[���A�E�g
	CCamera* camera = CCameraManager::GetCamera();
	camera->SetLengthToAt(lerp(m_LerpStart, m_LerpEnd, m_Lerp_t));
	m_Lerp_t = sinf(m_Counter * 3.14f / 180.0f);
	m_Counter += 1.0f;
	if (m_Counter >= 90.0f) { m_Counter = 90.0f; }
}

