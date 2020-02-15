#include "game_objects_all.h"
#include "seqence_manager.h"
#include "user_interface_manager.h"
#include "fade_manager.h"
#include "state_player_died.h"
#include "modelAnimation.h"
#include "player.h"
#include "MathFunc.h"

CStatePlayerDied::CStatePlayerDied(CPlayer* pPlayer)
	: m_FrameCounter(0)
{
	pPlayer->SetAnimation(PLAYER_STATE_DIED, 1.0f);
	pPlayer->SetAnimationSpeed(1.0f);

	pPlayer->Died() = true;
	pPlayer->DeadTimes() += 1;

	// 再生しているすべてのサウンドを停止
	CSound::StopSound();
	CSound::Play(SOUND_LABEL_BGM_ENVIRONMENT);

	// 死亡時のシーケンス再生
	CSeqenceManager::PlaySeqence(CSeqenceManager::SEQUENCE_DIED, *pPlayer->GetPosition());
}

CStatePlayerDied::~CStatePlayerDied()
{

}

void CStatePlayerDied::Update(CPlayer* pPlayer)
{
	// アニメーションはループさせない
	if (m_FrameCounter == pPlayer->GetCurrentAnimFrameNum() - 1) {
		pPlayer->SetMotionStop(true);
	}

	if (pPlayer->DeadTimes() == 3 && m_FrameCounter == 60 * 7) {
		CImage* image= new CImage("asset/image/user_interface/quest_failed.png");
		image->SetSize(XMFLOAT2(450 * 1.2f, 276 * 1.2f));
		image->SetPosition(XMFLOAT2(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f));
		image->SetColor(XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
		CUserInterfaceManager::AddUI(image, CUserInterfaceManager::LAYER_0);// UIに追加する
	}

	// ===== フェードアウトスタート ===========================
	if (m_FrameCounter == 60 * 10) {	// 10秒
		// 死んだ回数が2回以下ならリスポーン
		if (pPlayer->DeadTimes() < 3) {
			if (!CFadeManager::FadeOut()) {
				CFadeManager::StartFadeOut(2.0f);	// フェードアウト開始
				m_FrameCounter = 0;
			}
		}
		else {	// 3回死んだら失敗
			CManager::Failed() = true;
			return;
		}
	}

	// フェードアウト終了でシーン遷移 ===============================
	if (CFadeManager::FadeOutComplete()) {
		CFadeManager::StartFadeIn(2.0f);	// フェードアウト開始
		pPlayer->SetMotionStop(false);
		pPlayer->ReSpwan();		// リスポーン
		return;
	}


	// カウンター更新
	m_FrameCounter++;
}

