#include "game_objects_all.h"
#include "state_player_idle.h"
#include "state_player_move.h"
#include "player.h"
#include "state_player_sheathe_sword.h"
#include "state_player_died.h"
#include "state_player_damage.h"
#include "scene.h"
#include "enemy.h"

CStatePlayerSheatheSword::CStatePlayerSheatheSword(CPlayer* pPlayer, bool isMove)
	: m_FrameCounter(0)
	, m_IsMove(isMove)
{
	pPlayer->SetAnimation(PLAYER_STATE_SHEATHE_A_SWORD, 1.0f);
	pPlayer->SetAnimationSpeed(1.0f);

	CSound::Play(SOUND_LABEL_SE_DRAW_A_SWORD);
}

CStatePlayerSheatheSword::~CStatePlayerSheatheSword()
{

}

void CStatePlayerSheatheSword::Update(CPlayer* pPlayer)
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

	if (m_FrameCounter <= 30)return;

	pPlayer->WeaponState() = SWORD_STATE_SHEATHE;

	if (m_IsMove) {
		// �ړ��X�e�[�g�ɑJ��
		pPlayer->ChangeState(new CStatePlayerMove(pPlayer));
	}
	else {
		// �ҋ@�X�e�[�g�ɑJ��
		pPlayer->ChangeState(new CStatePlayerIdle(pPlayer));
	}
}

