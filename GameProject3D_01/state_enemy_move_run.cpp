#include "game_objects_all.h"
#include "scene.h"
#include "state_enemy_move_run.h"
#include "state_enemy_move_rotate.h"
#include "state_enemy_idle.h"
#include "enemy.h"
#include "MathFunc.h"


CStateEnemyMoveRun::CStateEnemyMoveRun(CEnemy* pEnemy)
	: m_FrameCounter(0)
	, m_PlayerAccesser(CManager::GetScene()->GetGameObject<CPlayer>(CManager::LAYER_OBJECT))
{
	pEnemy->SetAnimation(ENEMY_STATE_RUN, 1.0f);
	pEnemy->SetAnimationSpeed(2.5f);

	// �v���C���[�ւ̌������Z�o
	Vector3 direction = *m_PlayerAccesser->GetPosition() - *pEnemy->GetPosition();
	direction.Normalize();

	// �����Ƒ������瑬�x���Z�o
	m_MoveSpeed = direction * 0.45f;

	// �U����L����Ԃ�
	pEnemy->Attacked() = true;

	// �U���͐ݒ�
	pEnemy->Attack() = 35;
}

CStateEnemyMoveRun::~CStateEnemyMoveRun()
{

}

void CStateEnemyMoveRun::UpdateMoveState(CStateEnemyMove* pMoveState, CEnemy* pEnemy)
{
	// �ړ�����
	Move(pEnemy);

	// �v���C���[�Ƃ̋����Z�o
	Vector3 distance = *m_PlayerAccesser->GetPosition() - *pEnemy->GetPosition();

	//=====================================================
	//		�J�ڏ���
	//=====================================================
	if ((m_FrameCounter >= 180) ||		// 180F��ɑJ��
		((pEnemy->CollisionTerrian()) && m_FrameCounter >= 50) ||		// 50F��ȍ~�ɒn�`�ɏՓ˂�����J��
		((distance.Length() >= 20.0f) && m_FrameCounter >= 100)) {	// 100F��ȍ~�Ƀv���C���[�Ƃ̋�����30���ꂽ��J��

		pMoveState->ChangeState(new CStateEnemyMoveRotate(pEnemy));
		pEnemy->Attacked() = false;// �U���𖳌���Ԃ�
		return;
	}

	// �J�E���^�[�X�V
	m_FrameCounter++;
}

bool CStateEnemyMoveRun::Move(CEnemy* pEnemy)
{
	// �X�V�O�̈ʒu�擾
	Vector3 prev_pos = *pEnemy->GetPosition();

	// �ړ��X�s�[�h�𑫂��Ă���
	pEnemy->AddVelocity(m_MoveSpeed);

	// �X�V�O�Ƃ̈ړ��ʂ���i�s�������擾�E�Z�b�g
	Vector3 move_distance= *pEnemy->GetPosition() - prev_pos;
	pEnemy->Rotation().y = atan2f(move_distance.x, move_distance.z);

	// �O�b�o������I��
	return (m_FrameCounter >= 180) ? true : false;
}
