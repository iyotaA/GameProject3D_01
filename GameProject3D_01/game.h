#ifndef GAME_H_
#define GAME_H_

class CResult;


class CGame : public CScene
{
public:
	void Init()
	{
		//// カメラの初期化
		//CCamera* camera;
		//camera = AddGameObject<CCamera>(CManager::E_Camera);

		// スカイドームの初期化
		AddGameObject<CSkyDome>(CManager::E_Background);

		// フィールドの初期化
		AddGameObject<CTerrain>(CManager::E_Background);

		//// プレイヤーの初期化
		CPlayer* player;
		player = AddGameObject<CPlayer>(CManager::E_3D);
		//player->SetCamera(camera);	// カメラセット

		CCamera* camera = CCameraManager::GetCamera(0);		// 注視点
		camera->SetAt(player);

		// 数字の初期化
		number = new CNumber;
		number->Init("asset/number.tga");
		number->SetPosition(XMFLOAT2(550.0f, 0.0f));
		m_GameObject[CManager::E_UI].push_back(number);

		m_time = 0.0f;

		EnemySpawnPlace[0] = XMFLOAT2(0.0f, 5.0f);
		EnemySpawnPlace[1] = XMFLOAT2(3.0f, -2.5f);
		EnemySpawnPlace[2] = XMFLOAT2(5.5f, 5.0f);
		EnemySpawnPlace[3] = XMFLOAT2(4.0f, 2.0f);
		EnemySpawnPlace[4] = XMFLOAT2(-4.0f, -6.0f);
		EnemySpawnPlace[5] = XMFLOAT2(1.0f, 4.5f);
		EnemySpawnPlace[6] = XMFLOAT2(3.5f, -6.5f);
		EnemySpawnPlace[7] = XMFLOAT2(7.5f, 5.0f);
		EnemySpawnPlace[8] = XMFLOAT2(-6.5f, 7.0f);
		EnemySpawnPlace[9] = XMFLOAT2(-3.5f, 1.0f);
		EnemySpawnPlace[10] = XMFLOAT2(-9.5f, 4.0f);
		EnemySpawnPlace[11] = XMFLOAT2(1.5f, 6.5f);
		EnemySpawnPlace[12] = XMFLOAT2(-6.5f, -7.5f);
		EnemySpawnPlace[13] = XMFLOAT2(0.0f, -9.0f);
		EnemySpawnPlace[14] = XMFLOAT2(5.5f, 10.0f);
		EnemySpawnPlace[15] = XMFLOAT2(6.5f, 0.0f);
		EnemySpawnPlace[16] = XMFLOAT2(0.0f, -4.5f);
		EnemySpawnPlace[17] = XMFLOAT2(4.5f, -2.0f);
		EnemySpawnPlace[18] = XMFLOAT2(8.5f, -3.5f);
		EnemySpawnPlace[19] = XMFLOAT2(-2.5f, 5.0f);

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

		// タイム計測
		m_time += 0.016f;
		//number->SetNum(m_time);

		//if (FrameCount % 100 == 0) {

		//	int index;
		//	index = rand() % 20;
		//	AddGameObject<CEnemy>(CManager::E_3D)->SetPosition(XMFLOAT3(EnemySpawnPlace[index].x, 6.0f, EnemySpawnPlace[index].y));
		//}

		if (IsClear()) {
			CManager::SetScene<CResult>();
		}

		FrameCount++;
	}

	void Draw()
	{

		ImGui::Begin("System");
		ImGui::SetWindowFontScale(1.2f);
		ImGui::Text("[Scene]:Game");
		//ImGui::Columns(3, "System", true);
		ImGui::End();

		CScene::Draw();
	}

	bool IsClear(void)
	{
		if (m_time <= 0.0f) { return true; }
		else return false;
	}

private:
	float m_time;
	CNumber* number;
	XMFLOAT2 EnemySpawnPlace[20];
	int FrameCount;

};


#endif // !GAME_H_
