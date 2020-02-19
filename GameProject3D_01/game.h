#ifndef GAME_H_
#define GAME_H_

class CResult;
#include "MathFunc.h"

class CGame : public CScene
{
public:
	void Init()
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		//		UIオブジェクトの初期化
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		{// ライフ・スタミナゲージのフレーム
			CImage* image = new CImage("asset/image/user_interface/ui_header.png");
			image->SetSize(XMFLOAT2(720.0f, 160.0f));
			image->SetPosition(XMFLOAT2(360.0f, 80.0f));
			image->SetColor(XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
			CUserInterfaceManager::AddUI(image, CUserInterfaceManager::LAYER_1);// UIに追加する
		}
		{// ライフ・スタミナゲージのフレーム
			CImage* image = new CImage("asset/image/user_interface/clock_hand.png");
			image->SetSize(XMFLOAT2(120, 120));
			image->SetPosition(XMFLOAT2(82, 82));
			image->SetColor(XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
			CUserInterfaceManager::AddUI(image, CUserInterfaceManager::LAYER_1);// UIに追加する
			m_UI.push_back(image);
		}
		{// クエストボード
			CImage* image = new CImage("asset/image/user_interface/game_quest.png");
			image->SetSize(XMFLOAT2(600.0f, 600.0f));
			image->SetPosition(XMFLOAT2(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f));
			image->SetColor(XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
			CUserInterfaceManager::AddUI(image, CUserInterfaceManager::LAYER_1);// UIに追加する
			m_UI.push_back(image);
		}
		{// アイテムの枠
			CImage* image = new CImage("asset/image/user_interface/item_frame.png");
			image->SetSize(XMFLOAT2(490 * 0.3, 390 * 0.4));
			image->SetPosition(XMFLOAT2(1250, 650));
			image->SetColor(XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
			CUserInterfaceManager::AddUI(image, CUserInterfaceManager::LAYER_0);// UIに追加する
			m_UI.push_back(image);
		}

		//////////////////////////////////////////////////////////////////////////////////////////////////////
		//		3Dオブジェクトの初期化
		//////////////////////////////////////////////////////////////////////////////////////////////////////
			//カメラの生成
		CCameraManager::CreateCamera();

		// スカイドームの初期化
		AddGameObject<CSkyDome>(CManager::LAYER_BACKGROUND);

		// フィールドの初期化
		AddGameObject<CTerrain>(CManager::LAYER_BACKGROUND);

		// エネミーの初期化
		AddGameObject<CEnemy>(CManager::LAYER_OBJECT);

		// プレイヤーの初期化
		CPlayer* player;
		player = AddGameObject<CPlayer>(CManager::LAYER_OBJECT);

		// カメラセット
		CCamera* camera = CCameraManager::GetCamera();		// 注視点
		camera->SetAt(player, Vector3(0.0f, 2.3f, 0.0f));

		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// 入力を禁止する
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		CInput::InputEnable(false);
		// 特定のキーを入力可能にする
		CInput::EnableKeyCode(VK_RETURN);
		CInput::EnableGamepadButton(XINPUT_GAMEPAD_A);

		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// シーケンスの初期化
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		CSeqenceManager::Init();

		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// BGM再生
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		CSound::Play(SOUND_LABEL_BGM_ENVIRONMENT);

		m_time = 0.0f;
		m_FrameCounter = 0;
		m_Start = false;

		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// フェードインスタート
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		CFadeManager::StartFadeIn(3.0f);
	}

	void Uninit()
	{
		// ゲームの終了処理
		CSound::StopSound(SOUND_LABEL_BGM_GAME);

		// シーケンスの終了処理
		CSeqenceManager::Uninit();

		// 継承元の終了処理
		CScene::Uninit();
	}

	void Update()
	{
		if (CInput::GetGamepadTrigger(XINPUT_GAMEPAD_A) || CInput::GetKeyTrigger(VK_RETURN)) {
			if (!m_Start) {
				m_UI[1]->Delete() = true;
				m_Start = true;
				CSound::Play(SOUND_LABEL_SE_BUTTON);

				//===== 入力を可能に戻す ==============
				CInput::InputEnable(true);
			}
		}

		// クリア又は、失敗でシーン遷移 ===============================================
		if (CManager::Clear() || CManager::Failed()) {
			if (CFadeManager::FadeInComplete()) {
				if (!CFadeManager::FadeOut()) {
					CFadeManager::StartFadeOut(3.0f);						// フェードアウト開始
					m_FrameCounter = 0;
				}
			}
		}

		// フェードアウト終了でシーン遷移 ===========================================
		if (CManager::Clear() || CManager::Failed()) {
			if (CFadeManager::FadeOutComplete()) {
				CManager::SetScene<CResult>();
				return;
			}
		}

		// 時間計測
		if (!CManager::Clear()) {
			float time = m_FrameCounter * DELTA_TIME / 600.0f * 360.0f;
			if (time >= 360.0f) {
				time = 360.0f;
				CManager::Clear() = true;
			}
			m_UI[0]->SetRotate(time * DEGREE_TO_RADIAN);
		}

		// シーケンスの終了処理
		CSeqenceManager::Update();

		CScene::Update();
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
		ImGui::Text("[SCENE : GAME]");

		// FPS表示
		DrawFPS();
		ImGui::InputFloat2("position", (float*)m_UI[0]->GetPosition());

		// コリジョン用のデバッグ表示
		CDebugPrimitive::DrawGUI();
		ImGui::End();

		// カメラのデバッグ表示
		CCameraManager::DrawGUI();

		// 各ゲームオブジェクトのデバッグ表示
		CScene::DrawGUI();
	}

private:
	std::vector<CImage*> m_UI;
	float m_time;
	int m_FrameCounter;
	bool m_Start;

};


#endif // !GAME_H_
