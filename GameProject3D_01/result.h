#ifndef RESULT_H_
#define RESULT_H_


class CResult : public CScene
{
public:
	void Init()
	{
		//// カメラの初期化
		//CCamera* camera;
		//camera = AddGameObject<CCamera>(CManager::E_Camera);

		// フィールドの初期化
		AddGameObject<CField>(CManager::E_Background);

		// スカイドームの初期化
		AddGameObject<CSkyDome>(CManager::E_Background);

		// 壁の初期化
		for (int i = 0; i < 4; i++) {
			CWall* wall;
			wall = AddGameObject<CWall>(CManager::E_Background);
			switch (i)
			{
			case 0:
				wall->SetPosition(&Vector3(15.0f, 2.5f, 0.0f));
				wall->SetRotation(&Vector3(90.0f, 90.0f, 0.0f));
				break;

			case 1:
				wall->SetPosition(&Vector3(0.0f, 2.5f, -15.0f));
				wall->SetRotation(&Vector3(90.0f, 180.0f, 0.0f));
				break;

			case 2:
				wall->SetPosition(&Vector3(-15.0f, 2.5f, 0.0f));
				wall->SetRotation(&Vector3(90.0f, 270.0f, 0.0f));
				break;

			case 3:
				wall->SetPosition(&Vector3(0.0f, 2.5f, 15.0f));
				wall->SetRotation(&Vector3(90.0f, 0.0f, 0.0f));
				break;

			}
		}

		// タイトルロゴの初期化
		CPolygon* ui = new CPolygon;
		ui->Init(XMFLOAT2(350, 70), XMFLOAT2(670, 200), "asset/score.tga");
		m_GameObject[CManager::E_UI].push_back(ui);

		CPolygon* ui2 = new CPolygon;
		ui2->Init(XMFLOAT2(530, 600), XMFLOAT2(315, 75), "asset/press.tga");
		m_GameObject[CManager::E_UI].push_back(ui2);

		// 数字の初期化
		CNumber* number = new CNumber;
		number->Init("asset/number.tga");
		number->SetPosition(XMFLOAT2(550.0f, 300.0f));
		number->SetNum(static_cast<float>(CManager::GetScore()));
		m_GameObject[CManager::E_UI].push_back(number);

		FrameCount = 0;
	}

	void Uninit()
	{
		// ゲームの終了処理

		// 継承元の終了処理
		CScene::Uninit();
	}

	void Update()
	{
		CScene::Update();

		// キー入力でシーン遷移
		if (FrameCount >= 500) {
			if (CInput::GetKeyTrigger(VK_SPACE)) {
				CManager::SetScene<CTitle>();
			}
		}
		FrameCount++;
	}

	void Draw()
	{
		CScene::Draw();

		ImGui::Begin("State");
		ImGui::Text("Result");
		ImGui::End();
	}

private:
	int FrameCount;

};

#endif // !RESULT_H