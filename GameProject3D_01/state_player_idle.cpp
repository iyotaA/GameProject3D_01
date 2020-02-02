#include "game_objects_all.h"
#include "state_player_idle.h"
#include "state_player_move_run.h"
#include "state_player_attack.h"
#include "game_objects_all.h"
#include "modelAnimation.h"
#include "player.h"
#include "MathFunc.h"

CStatePlayerIdle::CStatePlayerIdle(CPlayer* pPlayer)
	: m_LerpStart(CCameraManager::GetCamera()->GetLengthToAt())
	, m_LerpEnd(4.0f)
	, m_FrameCounter(0.0f)
	, m_MoveSpeed(1.0f)
	, m_Volocity(Vector3())
{
	pPlayer->SetAnimation(PLAYER_STATE_IDLE, 1.0f);
	pPlayer->SetAnimationSpeed(1.0f);
}

CStatePlayerIdle::~CStatePlayerIdle()
{

}

void CStatePlayerIdle::Update(CPlayer* pPlayer)
{
	if (MoveEntry()) {
		pPlayer->ChangeState(new CStatePlayerMove(pPlayer));
		return;
	}

	Move(pPlayer);

	// ���X�ɃJ�����Y�[���C��
	CCamera* camera = CCameraManager::GetCamera();
	float lerp_deg = m_FrameCounter;
	if (lerp_deg >= 90.0f) { lerp_deg = 90.0f; }
	float lerp_t = sinf(lerp_deg * DEGREE_TO_RADIAN);
	camera->SetLengthToAt(lerp(m_LerpStart, m_LerpEnd, lerp_t));

	m_FrameCounter++;
}

void CStatePlayerIdle::Move(CPlayer* pPlayer)
{
	// �ړ���
	m_Volocity += Vector3() * m_MoveSpeed * pPlayer->DefaultSpeed();

	// �ړ��ʌ���...
	m_Volocity *= 0.87f;

	// �ړ�
	pPlayer->AddVelocity(m_Volocity);

	// ����
	m_MoveSpeed *= 0.95f;
	if (m_MoveSpeed <= 1.0f) { m_MoveSpeed = 1.0f; }
}
