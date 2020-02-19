#ifndef TITLE_H_
#define TITLE_H_

class CGame;

class CTitle : public CScene
{

public:
	void Init()
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		//		UI�I�u�W�F�N�g�̏�����
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		{// �w�i����
			CImage* image = new CImage("asset/image/user_interface/shine001.png");
			image->SetSize(XMFLOAT2(SCREEN_WIDTH, SCREEN_HEIGHT));
			image->SetPosition(XMFLOAT2(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f));
			image->SetColor(XMFLOAT4(0.2f, 0.2f, 0.2f, 0.5f));
			CUserInterfaceManager::AddUI(image, CUserInterfaceManager::LAYER_0);// UI�ɒǉ�����
		}
		{// �^�C�g�����S
			CImage* image = new CImage("asset/image/user_interface/title_rogo.png");
			image->SetSize(XMFLOAT2(1011, 476));
			image->SetPosition(XMFLOAT2(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT * 0.4f));
			image->SetColor(XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
			CUserInterfaceManager::AddUI(image, CUserInterfaceManager::LAYER_0);// UI�ɒǉ�����
		}
		{// Press Button A
			CImage* image = new CImage("asset/image/user_interface/press_button_A.png");
			image->SetSize(XMFLOAT2(267.0f, 70.0f));
			image->SetPosition(XMFLOAT2(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT * 0.8f));
			image->SetColor(XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
			CUserInterfaceManager::AddUI(image, CUserInterfaceManager::LAYER_0);// UI�ɒǉ�����
			m_UI.push_back(image);
		}

		//////////////////////////////////////////////////////////////////////////////////////////////////////
		//		3D�I�u�W�F�N�g�̏�����
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		//�J�����̐����E�ʒu�ݒ�
		CCameraManager::CreateCamera();
		CCameraManager::GetCamera()->SetAtPos(&Vector3(30, 15, -25));

		// �X�J�C�h�[���̏�����
		AddGameObject<CSkyDome>(CManager::LAYER_BACKGROUND);

		// �t�B�[���h�̏�����
		AddGameObject<CTerrain>(CManager::LAYER_BACKGROUND);


		//===== BGM�Đ� =================
		CSound::Play(SOUND_LABEL_BGM_TITLE);

		//===== ���͂��֎~���� ==============
		CInput::InputEnable(false);
		// ����̃L�[����͉\�ɂ���
		CInput::EnableKeyCode(VK_RETURN);
		CInput::EnableGamepadButton(XINPUT_GAMEPAD_A);

		//===== �t�F�[�h�C���J�n ==============
		CFadeManager::StartFadeIn(3.0f);
		m_FrameCounter = 0;
	}

	void Uninit()
	{
		// �Q�[���̏I������
		m_UI[0]->Delete() = true;

		//===== �p�����̏I������ =============
		CScene::Uninit();

		// �Q�[���̏I������
		if (!m_UI.empty()) {
			m_UI.clear();
		}
	}

	void Update()
	{
		CScene::Update();

		// �L�[���͂ŃV�[���J��
		if (CInput::GetKeyTrigger(VK_RETURN) || CInput::GetGamepadTrigger(XINPUT_GAMEPAD_A)) {
			if (CFadeManager::FadeInComplete()) {
				if (!CFadeManager::FadeOut()) {
					CSound::Play(SOUND_LABEL_SE_TITLE_RETURN);	// ���͉��Đ�
					CFadeManager::StartFadeOut(3.0f);						// �t�F�[�h�A�E�g�J�n
					m_FrameCounter = 0;
					m_UI[0]->SetColor(XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
				}
			}
		}

		// �t�F�[�h�A�E�g���ɏ��X�ɉ��ʂ������� ============================
		if (CFadeManager::FadeOut()) {
			float t = m_FrameCounter * DELTA_TIME / 2.0f;
			if (t >= 1.0f) { t = 1.0f; }
			float volume = Lerp(0.5f, 0.0f, t);
			CSound::SetVolume(SOUND_LABEL_BGM_TITLE, volume);
		}

		// �_�ł����� =========================================
		if (!CFadeManager::FadeOut()) {
			float a = sinf(m_FrameCounter * 4 * DEGREE_TO_RADIAN) * 0.4f + 0.6f;
			m_UI[0]->SetColor(XMFLOAT4(1.0f, 1.0f, 1.0f, a));
		}

		// �t�F�[�h�A�E�g�I���ŃV�[���J�� ===============================
		if (CFadeManager::FadeOutComplete()) {
			//===== ���͂�L���ɂ��� ==============
			CInput::InputEnable(true);

			// �Đ����Ă��邷�ׂẴT�E���h���~
			CSound::StopSound();
			CSound::SetVolume(SOUND_LABEL_BGM_TITLE, 0.5f);
			CManager::SetScene<CGame>();
			return;
		}

		// �J�E���^�[�X�V
		m_FrameCounter++;
	}

	void Draw()
	{
		CScene::Draw();
	}

	void DrawGUI()
	{
		ImGui::Begin("System");
		ImGui::SetWindowFontScale(1.3f);
		ImGui::Text("[SCENE : TITLE]");

		// FPS�\��
		DrawFPS();
		// �R���W�����p�̃f�o�b�O�\��
		CDebugPrimitive::DrawGUI();
		ImGui::End();

		// �J�����̃f�o�b�O�\��
		CCameraManager::DrawGUI();
	}

private:
	int m_FrameCounter;
	std::vector<CImage*> m_UI;

};

#endif // !TITLE_H_

