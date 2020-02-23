#include "game_objects_all.h"
#include "state_player_idle.h"
#include "player.h"
#include "state_player_draw_sword.h"
#include "state_player_attack_jump.h"
#include "state_player_died.h"
#include "state_player_damage.h"
#include "scene.h"
#include "enemy.h"

CStatePlayerDrawSword::CStatePlayerDrawSword(CPlayer* pPlayer, bool isMove)
	: m_FrameCounter(0.0f)
	, m_IsMove(isMove)
{
	pPlayer->SetAnimation(PLAYER_STATE_DRAW_A_SWORD, 1.0f);
	pPlayer->SetAnimationSpeed(1.0f);

	CSound::Play(SOUND_LABEL_SE_DRAW_A_SWORD);
}

CStatePlayerDrawSword::~CStatePlayerDrawSword()
{

}

void CStatePlayerDrawSword::Update(CPlayer* pPlayer)
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


	// �J�E���^�[�X�V
	m_FrameCounter++;

	if (m_FrameCounter == 40) {
		pPlayer->WeaponState() = SWORD_STATE_DRAW;
	}

	if (m_FrameCounter < pPlayer->GetCurrentAnimFrameNum() - 10) return;

	pPlayer->WeaponState() = SWORD_STATE_DRAW;
	if (m_IsMove) {

		// �U���X�e�[�g�ɑJ��
		pPlayer->ChangeState(new CStatePlayerAttackJump(pPlayer, true));
	}

	// �ҋ@�X�e�[�g�ɑJ��
	pPlayer->ChangeState(new CStatePlayerIdle(pPlayer));
}

