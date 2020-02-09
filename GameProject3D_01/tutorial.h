#ifndef TUTORIAL_H_
#define TUTORIAL_H_

class CGame;

class CTutorial : public CScene
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
			CManager::SetScene<CGame>();
		}
	}

	void Draw()
	{
		CScene::Draw();

		ImGui::Begin("State");
		ImGui::Text("Tutorial");
		ImGui::End();
	}

};

#endif // !TUTORIAL_H_


