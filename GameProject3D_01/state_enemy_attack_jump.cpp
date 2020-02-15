#include "game_objects_all.h"
#include "scene.h"
#include "state_enemy_attack_jump.h"
#include "modelAnimation.h"
#include "enemy.h"
#include "player.h"
#include "MathFunc.h"
#include "sound.h"

CStateEnemyAttackJump::CStateEnemyAttackJump(CEnemy* pEnemy, bool isMove)
	: m_FrameCounter(0)
	, m_StartPosition(*pEnemy->GetPosition())
	, m_TargetPosition(*pEnemy->GetPosition())
	, m_StartRotation(pEnemy->Rotation().y)
	, m_TargetRotation(pEnemy->Rotation().y)
	, m_Move(isMove)
{
	pEnemy->SetAnimation(ENEMY_STATE_ATTACK_JUMP, 1.0f);
	pEnemy->SetAnimationSpeed(2.3f);
	m_pPlayer = CManager::GetScene()->GetGameObject<CPlayer>(CManager::LAYER_OBJECT);
}

CStateEnemyAttackJump::~CStateEnemyAttackJump()
{

}

void CStateEnemyAttackJump::UpdateAttackState(CStateEnemyAttack* pAttackState, CEnemy* pEnemy)
{
	// 翼の音
	if (m_FrameCounter % 45 == 0) {
		CSound::Play(SOUND_LABEL_SE_WING);
	}

	Move(pEnemy);
	m_FrameCounter++;

	if (m_FrameCounter == 130) {
		pEnemy->SetAnimationSpeed(3.0f);
		pEnemy->Attacked() = true;				// 攻撃有効
	}

	// 着地音
	if(m_FrameCounter == 160){
		CSound::Play(SOUND_LABEL_SE_LANDING);
	}

	if (m_FrameCounter == 170) {
		pEnemy->SetAnimationSpeed(1.5f);
		pEnemy->Attacked() = false;				// 攻撃無効
	}

	if (m_FrameCounter <= 200) return;

	pEnemy->Attacked() = false;
	pAttackState->ChangeState(new CStateEnemyAttackNone(pEnemy));
}

void CStateEnemyAttackJump::Move(CEnemy* pEnemy)
{
	if (m_FrameCounter > 150 )return;


	if (m_FrameCounter == 60) { m_StartRotation = pEnemy->Rotation().y; }
	if (m_FrameCounter >= 60) {
		// プレイヤーの方向	に回転
		float lerp_deg = (m_FrameCounter - 59) / 90.0f * 90.0f;
		if (lerp_deg >= 90.0f) { lerp_deg = 90.0f; }
		float lerp_t = sinf(lerp_deg * DEGREE_TO_RADIAN);

		Vector3 dir_to_player = *m_pPlayer->GetPosition() - pEnemy->GetCenterPosition();
		pEnemy->Rotation().y = Lerp(m_StartRotation, atan2f(dir_to_player.x, dir_to_player.z), lerp_t);
	}

	if (m_FrameCounter == 100) {m_StartPosition = *pEnemy->GetPosition();}
	if (m_FrameCounter >= 100) {
		// プレイヤーの現在位置を終了位置に
		m_TargetPosition = *m_pPlayer->GetPosition();

		// 徐々にプレイヤーに移動
		float lerp_deg = (m_FrameCounter - 99) / 50.0f * 90.0f;
		if (lerp_deg >= 90.0f) { lerp_deg = 90.0f; }
		float lerp_t = sinf(lerp_deg * DEGREE_TO_RADIAN);
		pEnemy->SetPosition(&Lerp(m_StartPosition, m_TargetPosition, lerp_t));
	}
}
