#ifndef RESULT_H_
#define RESULT_H_


class CResult : public CScene
{
public:
	void Init()
	{

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