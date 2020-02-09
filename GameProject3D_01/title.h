#ifndef TITLE_H_
#define TITLE_H_

class CTutorial;

class CTitle : public CScene
{

public:
	void Init()
	{

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

