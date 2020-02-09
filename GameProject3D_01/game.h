#ifndef GAME_H_
#define GAME_H_

class CResult;
#include "MathFunc.h"

class CGame : public CScene
{
public:
	void Init()
	{
		// スカイドームの初期化
		AddGameObject<CSkyDome>(CManager::E_Background);

		// フィールドの初期化
		AddGameObject<CTerrain>(CManager::E_Background);
		//AddGameObject<CField>(CManager::E_Background);

		// エネミーの初期化
		AddGameObject<CEnemy>(CManager::E_3D);

		// プレイヤーの初期化
		CPlayer* player;
		player = AddGameObject<CPlayer>(CManager::E_3D);

		// カメラセット
		CCamera* camera = CCameraManager::GetCamera();		// 注視点
		camera->SetAt(player, Vector3(0.0f, 2.5f, 0.0f));

		// BGM再生
		CSound::Play(SOUND_LABEL_BGM_GAME);

		m_time = 0.0f;
		FrameCount = 0;
	}

	void Uninit()
	{
		// ゲームの終了処理
		CSound::StopSound(SOUND_LABEL_BGM_GAME);

		// 継承元の終了処理
		CScene::Uninit();
	}

	void Update()
	{
		CScene::Update();

		//if (FrameCount % 100 == 0) {

		//	int index;
		//	index = rand() % 20;
		//	AddGameObject<CEnemy>(CManager::E_3D)->SetPosition(XMFLOAT3(EnemySpawnPlace[index].x, 6.0f, EnemySpawnPlace[index].y));
		//}

		//if (IsClear()) {
		//	CManager::SetScene<CResult>();
		//}

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

	bool IsClear(void) { return (m_time <= 0.0f) ? true : false; }

private:
	float m_time;
	XMFLOAT2 EnemySpawnPlace[20];
	int FrameCount;

};


#endif // !GAME_H_
