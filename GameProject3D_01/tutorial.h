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
		// �Q�[���̏I������

		// �p�����̏I������
		CScene::Uninit();
	}

	void Update()
	{
		CScene::Update();

		// �L�[���͂ŃV�[���J��
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


