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
		// �Q�[���̏I������

		// �p�����̏I������
		CScene::Uninit();
	}

	void Update()
	{
		CScene::Update();

		// �L�[���͂ŃV�[���J��
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

