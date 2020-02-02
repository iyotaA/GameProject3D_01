#include "game_objects_all.h"
#include "state_player.h"
#include "state_player_move_run.h"
#include "state_player_idle.h"
#include "state_player_dodge.h"
#include "state_player_move_run.h"
#include "state_player_move_dash.h"
#include "modelAnimation.h"
#include "player.h"
#include "MathFunc.h"


CStatePlayerMoveDash::CStatePlayerMoveDash(CPlayer* pPlayer, float StartSpeed)
	: m_MoveSpeed(1.0f)
	, m_StartSpeed(StartSpeed)
	, m_TargetSpeed(1.25f)
	, m_StartLength(CCameraManager::GetCamera()->GetLengthToAt())
	, m_TargetLength(6.5f)
	, m_FrameCounter(0)
	, m_Volocity(Vector3(0.0f, 0.0f, 0.0f))
{
	pPlayer->SetAnimation(PLAYER_STATE_DASH, 1.0f);
	pPlayer->SetAnimationSpeed(1.0f);
}

CStatePlayerMoveDash::~CStatePlayerMoveDash()
{

}

void CStatePlayerMoveDash::UpdateMoveState(CStatePlayerMove* pMoveState, CPlayer* pPlayer)
{
	//******************************
	// �L�[����
	//******************************
	if (CInput::GetKeyTrigger(VK_SPACE) || CInput::GetGamepadTrigger(VK_PAD_A)) {

		// 20F�o���Ă������\��
		if (m_FrameCounter >= 20) {
			pPlayer->ChangeState(new CStatePlayerDodge(pPlayer));
			return;
		}
	}
	if (CInput::GetGamepadRelease(RIGHT_TRRIGER))
	{
		pMoveState->ChangeState(new CStatePlayerMoveRun(pPlayer, m_MoveSpeed));
		return;
	}

	// �ړ�����
	Move(pPlayer);

	// ���X�ɃJ�����Y�[���A�E�g
	CCamera* camera = CCameraManager::GetCamera();
	float lerp_deg = m_FrameCounter;
	if (lerp_deg >= 90.0f) { lerp_deg = 90.0f; }
	float lerp_t = sinf(lerp_deg * DEGREE_TO_RADIAN);
	camera->SetLengthToAt(lerp(m_StartLength, m_TargetLength, lerp_t));

	// �J�E���^�[�X�V
	m_FrameCounter++;
}

void CStatePlayerMoveDash::Move(CPlayer* pPlayer)
{
	CCamera* camera = CCameraManager::GetCamera();

	Vector3 cameraFront = camera->GetDir3Vector()->front;
	Vector3 cameraRight = camera->GetDir3Vector()->right;

	// �W���C�p�b�h
	XMFLOAT2 inputNum = CInput::GetGamepadLeftStick();
	Vector3 moveDistance = Vector3(0.0f, 0.0f, 0.0f);
	moveDistance += Vector3(cameraRight.x, 0.0f, cameraRight.z) * inputNum.x;
	moveDistance += Vector3(cameraFront.x, 0.0f, cameraFront.z) * inputNum.y;

	// �L�[�{�[�h
	if (CInput::GetKeyPress('W')) {
		moveDistance.x += cameraFront.x;
		moveDistance.z += cameraFront.z;
		inputNum.y += 1.0f;
	}
	else if (CInput::GetKeyPress('S')) {
		moveDistance.x -= cameraFront.x;
		moveDistance.z -= cameraFront.z;
		inputNum.y -= 1.0f;
	}
	if (CInput::GetKeyPress('A')) {
		moveDistance.x -= cameraRight.x;
		moveDistance.z -= cameraRight.z;
		inputNum.x -= 1.0f;
	}
	else if (CInput::GetKeyPress('D')) {
		moveDistance.x += cameraRight.x;
		moveDistance.z += cameraRight.z;
		inputNum.x += 1.0f;
	}

	// �ړ���
	moveDistance.Normalize();
	m_Volocity += moveDistance * m_MoveSpeed * pPlayer->DefaultSpeed();

	// �ړ��ʌ���...
	m_Volocity *= 0.87f;

	// �ړ�
	pPlayer->AddVelocity(m_Volocity);

	// ��]�p�x
	float radian = atan2f(inputNum.x, inputNum.y) + atan2f(cameraFront.x, cameraFront.z);
	pPlayer->SetRotation(&Vector3(0.0f, radian, 0.0f));

	// ���x�X�V
	float lerp_deg = m_FrameCounter;
	if (lerp_deg >= 90.0f) { lerp_deg = 90.0f; }
	float lerp_t = sinf(lerp_deg * DEGREE_TO_RADIAN);
	m_MoveSpeed = lerp(m_StartSpeed, m_TargetSpeed, lerp_t);
}
