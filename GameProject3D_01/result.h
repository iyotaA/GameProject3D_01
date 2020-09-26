#ifndef RESULT_H_
#define RESULT_H_

class CTitle;

class CResult : public CScene
{
public:
	void Init()
	{
		//カメラの生成
		CCameraManager::CreateCamera();

		//////////////////////////////////////////////////////////////////////////////////////////////////////
		//		UIオブジェクトの初期化
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		{//リザルト画面の背景
			CImage* image = new CImage("asset/image/user_interface/result.png");
			image->SetSize(XMFLOAT2(SCREEN_WIDTH, SCREEN_HEIGHT));
			image->SetPosition(XMFLOAT2(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f));
			image->SetColor(XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
			CUserInterfaceManager::AddUI(image, CUserInterfaceManager::LAYER_0);// UIに追加する
		}
		{// Press Button A
			CImage* image = new CImage("asset/image/user_interface/press_button_A.png");
			image->SetSize(XMFLOAT2(267.0f, 70.0f));
			image->SetPosition(XMFLOAT2(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT * 0.9f));
			image->SetColor(XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
			CUserInterfaceManager::AddUI(image, CUserInterfaceManager::LAYER_0);// UIに追加する
			m_UI.push_back(image);
		}

		//===== フェードイン開始 ==============
		CFadeManager::StartFadeIn(3.0f);
		m_FrameCounter = 0;

		CManager::Clear() = false;
		CManager::Failed() = false;
	}

	void Uninit()
	{
		// 継承元の終了処理
		CScene::Uninit();
	}

	void Update()
	{
		CScene::Update();

		// キー入力でシーン遷移
		if (CInput::GetKeyTrigger(VK_RETURN) || CInput::GetGamepadTrigger(XINPUT_GAMEPAD_A)) {
			if (CFadeManager::FadeInComplete()) {
				if (!CFadeManager::FadeOut()) {
					CSound::Play(SOUND_LABEL_SE_BUTTON);
					CFadeManager::StartFadeOut(3.0f);						// フェードアウト開始
					m_UI[0]->SetColor(XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
					m_FrameCounter = 0;
				}
			}
		}


		// フェードアウト中に徐々に音量を下げる ============================
		if (CFadeManager::FadeOut()) {
			float t = m_FrameCounter * DELTA_TIME / 2.0f;
			if (t >= 1.0f) { t = 1.0f; }
			float volume = Lerp(0.5f, 0.0f, t);
			CSound::SetVolume(SOUND_LABEL_BGM_TITLE, volume);
		}


		// フェードアウト終了でシーン遷移 ===============================
		if (CFadeManager::FadeOutComplete()) {
			// 再生しているすべてのサウンドを停止
			CSound::StopSound();
			CSound::SetVolume(SOUND_LABEL_BGM_TITLE, 0.5f);
			CManager::SetScene<CTitle>();
			return;
		}

		// 点滅させる =========================================
		if (!CFadeManager::FadeOut()) {
			float a = sinf(m_FrameCounter * 4 * DEGREE_TO_RADIAN) * 0.4f + 0.6f;
			m_UI[0]->SetColor(XMFLOAT4(1.0f, 1.0f, 1.0f, a));
		}

		m_FrameCounter++;
	}

	void Draw()
	{
		CScene::Draw();
	}

	void DrawGUI()
	{
		ImGui::Begin("System");
		ImGui::SetWindowFontScale(1.3f);
		ImGui::Text("[SCENE : RESULT]");

		// FPS表示
		DrawFPS();

		// コリジョン用のデバッグ表示
		CDebugPrimitive::DrawGUI();
		ImGui::End();

		// カメラのデバッグ表示
		CCameraManager::DrawGUI();
	}

private:
	int m_FrameCounter;
	std::vector<CImage*> m_UI;

};

#endif // !RESULT_H