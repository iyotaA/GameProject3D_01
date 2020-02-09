#include "game_objects_all.h"
#include "scene.h"
#include "modelAnimation.h"
#include "state_enemy_move_rotate.h"
#include "state_enemy_idle.h"
#include "enemy.h"
#include "player.h"
#include "MathFunc.h"


CStateEnemyMoveRotate::CStateEnemyMoveRotate(CEnemy* pEnemy, float StartSpeed)
	: m_FrameCounter(0)
{
	pEnemy->SetAnimation(ENEMY_STATE_WALK, 1.0f);
	pEnemy->SetAnimationSpeed(1.0f);

	m_StartDegree = atan2f(pEnemy->GetFront().x, pEnemy->GetFront().z);

	CPlayer* player = CManager::GetScene()->GetGameObject<CPlayer>(CManager::E_3D);
	Vector3 dir_to_player = *player->GetPosition() - *pEnemy->GetPosition();
	dir_to_player.Normalize();
	m_TargetDegree = atan2f(dir_to_player.x, dir_to_player.z);
}

CStateEnemyMoveRotate::~CStateEnemyMoveRotate()
{

}

void CStateEnemyMoveRotate::UpdateMoveState(CStateEnemyMove* pMoveState, CEnemy* pEnemy)
{

	// 回転処理
	if (Rotate(pEnemy)) {
		pEnemy->ChangeState(new CStateEnemyIdle(pEnemy));
		return;
	}

	// カウンター更新
	m_FrameCounter++;
}

bool CStateEnemyMoveRotate::Rotate(CEnemy* pEnemy)
{
	// 徐々にプレイヤーに向けて回転
	float lerp_deg = m_FrameCounter * DELTA_TIME / 2.0f * 90.0f;
	if (lerp_deg >= 90.0f) { lerp_deg = 90.0f; }
	float lerp_t = sinf(lerp_deg * DEGREE_TO_RADIAN);
	pEnemy->Rotation().y =  lerp(m_StartDegree, m_TargetDegree, lerp_t);

	return (lerp_t >= 1.0f) ? true: false;
}
