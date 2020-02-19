#include "game_objects_all.h"
#include "seqence_manager.h"
#include "user_interface_manager.h"
#include "fade_manager.h"
#include "state_player_died.h"
#include "state_player_idle.h"
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
	// 全てのUIのデリートフラグON
	for (CImage* ui : m_UI) {
		ui->Delete() = true;
	}
}

void CStatePlayerDied::Update(CPlayer* pPlayer)
{
	// アニメーションはループさせない
	if (m_FrameCounter == pPlayer->GetCurrentAnimFrameNum() - 1) {
		pPlayer->SetMotionStop(true);
	}

	if (m_FrameCounter == 60 * 1) {
		{	// 「力尽きました」
			CImage* image = new CImage("asset/image/user_interface/died.png");
			image->SetSize(XMFLOAT2(818 * 0.6f, 241 * 0.6f));
			image->SetPosition(XMFLOAT2(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f));
			image->SetColor(XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
			CUserInterfaceManager::AddUI(image, CUserInterfaceManager::LAYER_1);// UIに追加する
			m_UI.push_back(image);
		}
		CSound::Play(SOUND_LABEL_SE_KATYA);
	}

	if ( m_FrameCounter == 60 * 4.5) {
		m_UI[m_UI.size()-1]->Delete() = true;	// ひとつ前のUIを消す

		if (pPlayer->DeadTimes() < 3) {
			{	// 「スタート位置に戻ります」
				CImage* image = new CImage("asset/image/user_interface/startpoint_return.png");
				image->SetSize(XMFLOAT2(1000 * 0.6f, 241 * 0.6f));
				image->SetPosition(XMFLOAT2(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f));
				image->SetColor(XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
				CUserInterfaceManager::AddUI(image, CUserInterfaceManager::LAYER_1);// UIに追加する
				m_UI.push_back(image);
			}
			CSound::Play(SOUND_LABEL_SE_KATYA);
		}
		else {
			{	// 「スタート位置に戻ります」
				CImage* image = new CImage("asset/image/user_interface/game_over.png");
				image->SetSize(XMFLOAT2(1000 * 0.6f, 241 * 0.6f));
				image->SetPosition(XMFLOAT2(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f));
				image->SetColor(XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
				CUserInterfaceManager::AddUI(image, CUserInterfaceManager::LAYER_1);// UIに追加する
				m_UI.push_back(image);
			}
			CSound::Play(SOUND_LABEL_SE_KATYA);
		}
	}
	if (pPlayer->DeadTimes() == 3 && m_FrameCounter == 60 * 8) {
		m_UI[m_UI.size() - 1]->Delete() = true;	// ひとつ前のUIを消す
		{	// 「Quest Failed」
			CImage* image = new CImage("asset/image/user_interface/quest_failed.png");
			image->SetSize(XMFLOAT2(450 * 1.2f, 276 * 1.2f));
			image->SetPosition(XMFLOAT2(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f));
			image->SetColor(XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
			CUserInterfaceManager::AddUI(image, CUserInterfaceManager::LAYER_1);// UIに追加する
		}
		CSound::Play(SOUND_LABEL_SE_DON);
	}

	// ===== フェードアウトスタート ===========================
	if (m_FrameCounter == 60 * 10) {	// 10秒
		// 死んだ回数が2回以下ならリスポーン
		if (pPlayer->DeadTimes() < 3) {
			if (!CFadeManager::FadeOut()) {
				CFadeManager::StartFadeOut(2.0f);	// フェードアウト開始
			}
		}
		else {	// 3回死んだら失敗
			CManager::Failed() = true;
			return;
		}
	}

	// フェードアウト終了でシーン遷移 ===============================
	if (CFadeManager::FadeOutComplete()) {
		pPlayer->SetMotionStop(false);
		pPlayer->ChangeState(new CStatePlayerIdle(pPlayer));
		pPlayer->ReSpwan();		// リスポーン
		CFadeManager::StartFadeIn(3.0f);	// フェードアウト開始
		return;
	}


	// カウンター更新
	m_FrameCounter++;
}

