#include "game_objects_all.h"
#include "scene.h"
#include "state_enemy_move_run.h"
#include "state_enemy_idle.h"
#include "modelAnimation.h"
#include "enemy.h"
#include "MathFunc.h"


CStateEnemyMoveRun::CStateEnemyMoveRun(CEnemy* pEnemy)
	: m_FrameCounter(0)
	, m_StartPosition(*pEnemy->GetPosition())
	, m_TargetPosition(*CManager::GetScene()->GetGameObject<CPlayer>(CManager::LAYER_OBJECT)->GetPosition())
{
	pEnemy->SetAnimation(ENEMY_STATE_RUN, 1.0f);
	pEnemy->SetAnimationSpeed(1.5f);
}

CStateEnemyMoveRun::~CStateEnemyMoveRun()
{

}

void CStateEnemyMoveRun::UpdateMoveState(CStateEnemyMove* pMoveState, CEnemy* pEnemy)
{
	// �ړ�����
	if (Move(pEnemy)) {
		pEnemy->ChangeState(new CStateEnemyIdle(pEnemy));
		return;
	}

	// �J�E���^�[�X�V
	m_FrameCounter++;
}

bool CStateEnemyMoveRun::Move(CEnemy* pEnemy)
{
	// �X�V�O�̈ʒu�擾
	Vector3 prev_pos = *pEnemy->GetPosition();

	// ���X�Ƀv���C���[�Ɉړ���]
	float lerp_deg = m_FrameCounter * DELTA_TIME / 2.0f * 90.0f;
	if (lerp_deg >= 90.0f) { lerp_deg = 90.0f; }
	float lerp_t = sinf(lerp_deg * DEGREE_TO_RADIAN);
	pEnemy->SetPosition(&Lerp(m_StartPosition, m_TargetPosition, lerp_t));

	// �X�V�O�Ƃ̈ړ��ʂ���i�s�������擾�E�Z�b�g
	Vector3 move_distance= *pEnemy->GetPosition() - prev_pos;
	pEnemy->Rotation().y = atan2f(move_distance.x, move_distance.z);

	return (lerp_t >= 1.0f) ? true : false;
}
