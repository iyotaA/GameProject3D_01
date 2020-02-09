#ifndef GAME_H_
#define GAME_H_

class CResult;
#include "MathFunc.h"

class CGame : public CScene
{
public:
	void Init()
	{
		// �X�J�C�h�[���̏�����
		AddGameObject<CSkyDome>(CManager::E_Background);

		// �t�B�[���h�̏�����
		AddGameObject<CTerrain>(CManager::E_Background);
		//AddGameObject<CField>(CManager::E_Background);

		// �G�l�~�[�̏�����
		AddGameObject<CEnemy>(CManager::E_3D);

		// �v���C���[�̏�����
		CPlayer* player;
		player = AddGameObject<CPlayer>(CManager::E_3D);

		// �J�����Z�b�g
		CCamera* camera = CCameraManager::GetCamera();		// �����_
		camera->SetAt(player, Vector3(0.0f, 2.5f, 0.0f));

		// BGM�Đ�
		CSound::Play(SOUND_LABEL_BGM_GAME);

		m_time = 0.0f;
		FrameCount = 0;
	}

	void Uninit()
	{
		// �Q�[���̏I������
		CSound::StopSound(SOUND_LABEL_BGM_GAME);

		// �p�����̏I������
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
