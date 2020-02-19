#include "game_objects_all.h"
#include "state_player_dodge.h"
#include "state_player_move.h"
#include "state_player_idle.h"
#include "state_player_died.h"
#include "state_player_damage.h"
#include "modelAnimation.h"
#include "player.h"
#include "scene.h"
#include "enemy.h"

CStatePlayerDodge::CStatePlayerDodge(CPlayer* pPlayer)
	: m_FrameCounter(0)
	, m_Velocity(Vector3())
	, m_DirFront(pPlayer->GetFront())
{
	pPlayer->SetAnimation(PLAYER_STATE_ROOL, 1.0f);
	pPlayer->SetAnimationSpeed(1.2f);
	pPlayer->SetMoveSpeed(1.0f);
	pPlayer->Stamina() -= 20;
	pPlayer->Dodged() = true;
}

CStatePlayerDodge::~CStatePlayerDodge()
{

}

void CStatePlayerDodge::Update(CPlayer* pPlayer)
{
	// ���S�X�e�[�g�ɑJ��
	if (pPlayer->Life() <= 0.0f) {
		pPlayer->ChangeState(new CStatePlayerDied(pPlayer));
		return;
	}

	// �_���[�W�X�e�[�g�ɑJ��
	if (pPlayer->Damaged()) {
		CEnemy* enemy = CManager::GetScene()->GetGameObject<CEnemy>(CManager::LAYER_OBJECT);
		pPlayer->ChangeState(new CStatePlayerDamage(pPlayer, enemy->Attack()));
		return;
	}

	// �ړ�����
	Move(pPlayer);

	if (pPlayer->AnimationBlending()) return;

	if(m_FrameCounter == 30){ pPlayer->Dodged() = false; }

	if (m_FrameCounter <= pPlayer->GetCurrentAnimFrameNum() - 40) return;

	// �ړ����͂��ꂽ���H
	MoveEntry() ?
		pPlayer->ChangeState(new CStatePlayerMove(pPlayer)) // true
		: pPlayer->ChangeState(new CStatePlayerIdle(pPlayer)); // else
}

void CStatePlayerDodge::Move(CPlayer* pPlayer)
{
	if (m_FrameCounter++ <= 25) {
		m_Velocity += m_DirFront * pPlayer->DefaultSpeed();
		m_Velocity *= 0.9f;
		pPlayer->AddVelocity(m_Velocity);
		return;
	}

	m_Velocity *= 0.98f;
	pPlayer->AddVelocity(m_Velocity);
}
