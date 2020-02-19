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

	// UŒ‚—ÍÝ’è
	pEnemy->Attack() = 40;
}

CStateEnemyAttackJump::~CStateEnemyAttackJump()
{

}

void CStateEnemyAttackJump::UpdateAttackState(CStateEnemyAttack* pAttackState, CEnemy* pEnemy)
{
	Move(pEnemy);
	m_FrameCounter++;

	// —ƒ‚Ì‰¹
	if (m_FrameCounter % 45 == 0) {
		CSound::Play(SOUND_LABEL_SE_WING);
	}

	if (m_FrameCounter == 130) {		// UŒ‚—LŒø
		pEnemy->SetAnimationSpeed(3.0f);
		pEnemy->Attacked() = true;
	}
	else if(m_FrameCounter == 160){	// ’…’n‰¹
		CSound::Play(SOUND_LABEL_SE_LANDING);
	}
	else if (m_FrameCounter == 170) {		// UŒ‚–³Œø
		pEnemy->SetAnimationSpeed(1.5f);
		pEnemy->Attacked() = false;
	}

	if (m_FrameCounter <= 200) return;

	pEnemy->Attacked() = false;
	pAttackState->ChangeState(new CStateEnemyAttackNone(pEnemy));
}

void CStateEnemyAttackJump::Move(CEnemy* pEnemy)
{
	if (m_FrameCounter > 150 )return;


	if (m_FrameCounter == 60) { m_StartRotation = pEnemy->Rotation().y; }

	if (m_FrameCounter == 100) {
		CPlayer* player = CManager::GetScene()->GetGameObject<CPlayer>(CManager::LAYER_OBJECT);
		Vector3 distance = *player->GetPosition() - *pEnemy->GetPosition();

		// ƒvƒŒƒCƒ„[‚ªˆÚ“®‚µ‚Ä‚¢‚½‚çæ‰ñ‚è
		if (player->Moved()) {
			distance += player->GetFront() * 5;
		}
		m_MoveSpeed = distance / 40.0f;
	}
	if (m_FrameCounter >= 100) {

		pEnemy->AddVelocity(m_MoveSpeed);
	}
}
