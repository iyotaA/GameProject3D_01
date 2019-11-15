#ifndef TITLE_H_
#define TITLE_H_

class CTutorial;

class CTitle : public CScene
{

public:
	void Init()
	{
		// カメラの初期化
		CCamera* camera;
		camera = AddGameObject<CCamera>(CManager::E_Camera);

		// フィールドの初期化
		AddGameObject<CField>(CManager::E_Background);
		
		// タイトルロゴの初期化	
		CPolygon* ui = new CPolygon;
		ui->Init(XMFLOAT2(150, 170), XMFLOAT2(1125, 262), "asset/title.tga");
		m_GameObject[CManager::E_UI].push_back(ui);

		CPolygon* ui2 = new CPolygon;
		ui2->Init(XMFLOAT2(500, 600), XMFLOAT2(315, 75), "asset/press.tga");
		m_GameObject[CManager::E_UI].push_back(ui2);
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
		if (CInput::GetKeyTrigger(VK_SPACE)) {
			CManager::SetScene<CTutorial>();
		}
	}

	void Draw()
	{
		CScene::Draw();

		ImGui::Begin("State");
		ImGui::Text("Title");
		ImGui::End();
	}

};

#endif // !TITLE_H_

