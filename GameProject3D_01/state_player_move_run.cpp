#include "game_objects_all.h"
#include "state_player.h"
#include "state_player_move_run.h"
#include "state_player_move_dash.h"
#include "state_player_idle.h"
#include "state_player_dodge.h"
#include "state_player_damage_large.h"
#include "state_player_sheathe_sword.h"
#include "player.h"
#include "MathFunc.h"


CStatePlayerMoveRun::CStatePlayerMoveRun(CPlayer* pPlayer, float StartSpeed)
	: m_MoveSpeed(1.0f)
	, m_StartSpeed(StartSpeed)
	, m_TargetSpeed(1.0f)
	, m_StartLength(CCameraManager::GetCamera()->GetLengthToAt())
	, m_TargetLength(5.0f)
	, m_FrameCounter(0)
	, m_Volocity(Vector3())
{
	pPlayer->SetAnimation(PLAYER_STATE_RUN, 1.0f);
	pPlayer->SetAnimationSpeed(1.0f);
}

CStatePlayerMoveRun::~CStatePlayerMoveRun()
{

}

void CStatePlayerMoveRun::UpdateMoveState(CStatePlayerMove* pMoveState, CPlayer* pPlayer)
{
	//******************************
	// キー入力
	//******************************
	if (CInput::GetGamepadPress(RIGHT_TRRIGER) || CInput::GetKeyPress(VK_RSHIFT)) {
		if (!pPlayer->EmptyStamina()) {
			if (pPlayer->WeaponState() == SWORD_STATE_DRAW) {
				pPlayer->ChangeState(new CStatePlayerSheatheSword(pPlayer, true));
			}
			else {
				pMoveState->ChangeState(new CStatePlayerMoveDash(pPlayer, m_MoveSpeed));
			}
			return;
		}
	}

	// 移動処理
	Move(pPlayer);

	// 足音
	if (m_FrameCounter % 23 == 0) { CSound::Play(SOUND_LABEL_SE_FOOTSTEP); }

	// 徐々にカメラズームアウト
	CCamera* camera = CCameraManager::GetCamera();
	float lerp_deg = m_FrameCounter / 60.0f * 90.0f;
	if (lerp_deg >= 90.0f) { lerp_deg = 90.0f; }
	float lerp_t = sinf(lerp_deg * DEGREE_TO_RADIAN);
	camera->SetLengthToAt(Lerp(m_StartLength, m_TargetLength, lerp_t));

	// カウンター更新
	m_FrameCounter++;
}

void CStatePlayerMoveRun::Move(CPlayer* pPlayer)
{
	CCamera* camera = CCameraManager::GetCamera();

	Vector3 cameraFront = camera->GetDir3Vector()->front;
	Vector3 cameraRight = camera->GetDir3Vector()->right;

	// ジョイパッド
	XMFLOAT2 inputNum = CInput::GetGamepadLeftStick();
	Vector3 moveDistance = Vector3(0.0f, 0.0f, 0.0f);
	moveDistance += Vector3(cameraRight.x, 0.0f, cameraRight.z) * inputNum.x;
	moveDistance += Vector3(cameraFront.x, 0.0f, cameraFront.z) * inputNum.y;

	// キーボード
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

	// 移動量
	moveDistance.Normalize();
	m_Volocity += moveDistance * m_MoveSpeed * pPlayer->DefaultSpeed();

	// 移動量減衰...
	m_Volocity *= 0.87f;

	// 移動
	pPlayer->AddVelocity(m_Volocity);

	// 回転角度
	float radian = atan2f(inputNum.x, inputNum.y) + atan2f(cameraFront.x, cameraFront.z);
	pPlayer->SetRotation(&Vector3(0.0f, radian, 0.0f));

	// 速度更新
	float lerp_deg = m_FrameCounter;
	if (lerp_deg >= 90.0f) { lerp_deg = 90.0f; }
	float lerp_t = sinf(lerp_deg * DEGREE_TO_RADIAN);
	m_MoveSpeed = Lerp(m_StartSpeed, m_TargetSpeed, lerp_t);
}
