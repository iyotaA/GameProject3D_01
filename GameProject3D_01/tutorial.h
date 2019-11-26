#ifndef TUTORIAL_H_
#define TUTORIAL_H_

class CGame;

class CTutorial : public CScene
{
public:
	void Init()
	{
		//// �J�����̏�����
		//CCamera* camera;
		//camera = AddGameObject<CCamera>(CManager::E_Camera);

		// �t�B�[���h�̏�����
		AddGameObject<CField>(CManager::E_Background);

		CPolygon* ui = new CPolygon;
		ui->Init(XMFLOAT2(170, 100), XMFLOAT2(996, 480), "asset/tutorial.tga");
		m_GameObject[CManager::E_UI].push_back(ui);

		CPolygon* ui2 = new CPolygon;
		ui2->Init(XMFLOAT2(500, 600), XMFLOAT2(315, 75), "asset/press.tga");
		m_GameObject[CManager::E_UI].push_back(ui2);
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


