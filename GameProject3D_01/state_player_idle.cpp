#include "game_objects_all.h"
#include "state_player_idle.h"
#include "state_player_move_run.h"
#include "state_player_attack.h"
#include "state_player_damage.h"
#include "state_player_draw_sword.h"
#include "state_player_sheathe_sword.h"
#include "state_player_block.h"
#include "state_player_died.h"
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
	// 死亡ステートに遷移
	if (pPlayer->Life() <= 0.0f) {
		pPlayer->ChangeState(new CStatePlayerDied(pPlayer));
		return;
	}

	// 入力・状態遷移
	Action(pPlayer);

	// 移動処理
	Move(pPlayer);

	// 徐々にカメラズームイン
	CCamera* camera = CCameraManager::GetCamera();
	float lerp_deg = m_FrameCounter;
	if (lerp_deg >= 90.0f) { lerp_deg = 90.0f; }
	float lerp_t = sinf(lerp_deg * DEGREE_TO_RADIAN);
	camera->SetLengthToAt(Lerp(m_LerpStart, m_LerpEnd, lerp_t));

	// カウンター更新
	m_FrameCounter++;
}

void CStatePlayerIdle::Move(CPlayer* pPlayer)
{
	// 移動量
	m_Volocity += Vector3() * m_MoveSpeed * pPlayer->DefaultSpeed();

	// 移動量減衰...
	m_Volocity *= 0.87f;

	// 移動
	pPlayer->AddVelocity(m_Volocity);

	// 減速
	m_MoveSpeed *= 0.95f;
	if (m_MoveSpeed <= 1.0f) { m_MoveSpeed = 1.0f; }
}

void CStatePlayerIdle::Action(CPlayer* pPlayer)
{
	// 移動ステートに遷移
	if (MoveEntry()) {
		pPlayer->ChangeState(new CStatePlayerMove(pPlayer));
		return;
	}

	// ダメージステートに遷移
	if (pPlayer->Damaged()){

		pPlayer->ChangeState(new CStatePlayerDamage(pPlayer));
		return;
	}

	// 抜刀・攻撃ステートに遷移
	if ((m_FrameCounter >= 10) &&
		(CInput::GetGamepadTrigger(XINPUT_GAMEPAD_Y) || CInput::GetKeyTrigger('I'))){

		// 抜刀時は攻撃ステートに遷移
		if ((pPlayer->WeaponState() == SWORD_STATE_DRAW)) {

			pPlayer->ChangeState(new CStatePlayerAttack(pPlayer, PLAYER_STATE_ATTACK_VIRTICAL));
		}
		// 納刀時は抜刀ステートに遷移
		else if((pPlayer->WeaponState() == SWORD_STATE_SHEATHE)) {

			pPlayer->ChangeState(new CStatePlayerDrawSword(pPlayer, false));
		}
		return;
	}

	// 防御ステートに遷移
	if (CInput::GetGamepadPress(LEFT_TRRIGER) || CInput::GetKeyPress(VK_LSHIFT)){

		if ((pPlayer->WeaponState() == SWORD_STATE_DRAW)) {
			pPlayer->ChangeState(new CStatePlayerBlock(pPlayer));
			return;
		}
	}

	// 納刀ステートに遷移
	if ((pPlayer->WeaponState() == SWORD_STATE_DRAW) &&
		(CInput::GetGamepadTrigger(XINPUT_GAMEPAD_X) || CInput::GetKeyTrigger('K'))){

		pPlayer->ChangeState(new CStatePlayerSheatheSword(pPlayer, false));
		return;
	}
}

