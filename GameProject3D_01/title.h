#ifndef TITLE_H_
#define TITLE_H_

class CGame;

class CTitle : public CScene
{

public:
	void Init()
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		//		UIオブジェクトの初期化
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		{// 背景効果
			CImage* image = new CImage("asset/image/user_interface/shine001.png");
			image->SetSize(XMFLOAT2(SCREEN_WIDTH, SCREEN_HEIGHT));
			image->SetPosition(XMFLOAT2(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f));
			image->SetColor(XMFLOAT4(0.2f, 0.2f, 0.2f, 0.5f));
			CUserInterfaceManager::AddUI(image, CUserInterfaceManager::LAYER_0);// UIに追加する
		}
		{// タイトルロゴ
			CImage* image = new CImage("asset/image/user_interface/title_rogo.png");
			image->SetSize(XMFLOAT2(1011, 476));
			image->SetPosition(XMFLOAT2(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT * 0.4f));
			image->SetColor(XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
			CUserInterfaceManager::AddUI(image, CUserInterfaceManager::LAYER_0);// UIに追加する
		}
		{// Press Button A
			CImage* image = new CImage("asset/image/user_interface/press_button_A.png");
			image->SetSize(XMFLOAT2(267.0f, 70.0f));
			image->SetPosition(XMFLOAT2(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT * 0.8f));
			image->SetColor(XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
			CUserInterfaceManager::AddUI(image, CUserInterfaceManager::LAYER_0);// UIに追加する
			m_UI.push_back(image);
		}

		//////////////////////////////////////////////////////////////////////////////////////////////////////
		//		3Dオブジェクトの初期化
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		//カメラの生成・位置設定
		CCameraManager::CreateCamera();
		CCameraManager::GetCamera()->SetAtPos(&Vector3(30, 15, -25));

		// スカイドームの初期化
		AddGameObject<CSkyDome>(CManager::LAYER_BACKGROUND);

		// フィールドの初期化
		AddGameObject<CTerrain>(CManager::LAYER_BACKGROUND);


		//===== BGM再生 =================
		CSound::Play(SOUND_LABEL_BGM_TITLE);

		//===== 入力を禁止する ==============
		CInput::InputEnable(false);
		// 特定のキーを入力可能にする
		CInput::EnableKeyCode(VK_RETURN);
		CInput::EnableGamepadButton(XINPUT_GAMEPAD_A);

		//===== フェードイン開始 ==============
		CFadeManager::StartFadeIn(3.0f);
		m_FrameCounter = 0;
	}

	void Uninit()
	{
		// ゲームの終了処理
		m_UI[0]->Delete() = true;

		//===== 継承元の終了処理 =============
		CScene::Uninit();

		// ゲームの終了処理
		if (!m_UI.empty()) {
			m_UI.clear();
		}
	}

	void Update()
	{
		CScene::Update();

		// キー入力でシーン遷移
		if (CInput::GetKeyTrigger(VK_RETURN) || CInput::GetGamepadTrigger(XINPUT_GAMEPAD_A)) {
			if (CFadeManager::FadeInComplete()) {
				if (!CFadeManager::FadeOut()) {
					CSound::Play(SOUND_LABEL_SE_TITLE_RETURN);	// 入力音再生
					CFadeManager::StartFadeOut(3.0f);						// フェードアウト開始
					m_FrameCounter = 0;
					m_UI[0]->SetColor(XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
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

		// 点滅させる =========================================
		if (!CFadeManager::FadeOut()) {
			float a = sinf(m_FrameCounter * 4 * DEGREE_TO_RADIAN) * 0.4f + 0.6f;
			m_UI[0]->SetColor(XMFLOAT4(1.0f, 1.0f, 1.0f, a));
		}

		// フェードアウト終了でシーン遷移 ===============================
		if (CFadeManager::FadeOutComplete()) {
			//===== 入力を有効にする ==============
			CInput::InputEnable(true);

			// 再生しているすべてのサウンドを停止
			CSound::StopSound();
			CSound::SetVolume(SOUND_LABEL_BGM_TITLE, 0.5f);
			CManager::SetScene<CGame>();
			return;
		}

		// カウンター更新
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
		ImGui::Text("[SCENE : TITLE]");

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

#endif // !TITLE_H_

