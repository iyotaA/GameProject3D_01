#include "game_objects_all.h"
#include "state_enemy_died.h"
#include "seqence_manager.h"
#include "user_interface_manager.h"
#include "modelAnimation.h"
#include "enemy.h"
#include "sound.h"
#include "image.h"
#include "input.h"

CStateEnemyDied::CStateEnemyDied(CEnemy* pEnemy)
	: m_FrameCounter(0)
{
	// 効果音再生
	CSound::StopSound();
	CSound::Play(SOUND_LABEL_SE_VICTORY);
	pEnemy->SetAnimation(ENEMY_STATE_DIED, 1.0f);
	pEnemy->SetAnimationSpeed(0.75f);

	// クリアシーケンス再生
	CSeqenceManager::PlaySeqence(CSeqenceManager::SEQUENCE_CLEAR, *pEnemy->GetPosition());
}

CStateEnemyDied::~CStateEnemyDied()
{
}

void CStateEnemyDied::Update(CEnemy* pEnemy)
{
	if (m_FrameCounter == 190) {
		CSound::Play(SOUND_LABEL_BGM_TITLE);
	}

	if (m_FrameCounter == pEnemy->GetCurrentAnimFrameNum() / 0.75f - 1) {
		pEnemy->SetMotionStop(true);
	}

	if (m_FrameCounter == 60 * 28) {
		CSound::Play(SOUND_LABEL_SE_DON);
		CImage* image = new CImage("asset/image/user_interface/quest_clear.png");
		image->SetSize(XMFLOAT2(450 * 1.2f, 276 * 1.2f));
		image->SetPosition(XMFLOAT2(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f));
		image->SetColor(XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
		CUserInterfaceManager::AddUI(image, CUserInterfaceManager::LAYER_0);// UIに追加する

		CInput::InputEnable(false);
	}

	if (m_FrameCounter == 60 * 30) {
		CManager::Clear() = true;
	}

	// カウンター更新
	m_FrameCounter++;

}

