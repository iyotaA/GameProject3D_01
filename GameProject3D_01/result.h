#ifndef RESULT_H_
#define RESULT_H_

class CTitle;

class CResult : public CScene
{
public:
	void Init()
	{
		//�J�����̐���
		CCameraManager::CreateCamera();

		//////////////////////////////////////////////////////////////////////////////////////////////////////
		//		UI�I�u�W�F�N�g�̏�����
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		{//���U���g��ʂ̔w�i
			CImage* image = new CImage("asset/image/user_interface/result.png");
			image->SetSize(XMFLOAT2(SCREEN_WIDTH, SCREEN_HEIGHT));
			image->SetPosition(XMFLOAT2(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f));
			image->SetColor(XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
			CUserInterfaceManager::AddUI(image, CUserInterfaceManager::LAYER_0);// UI�ɒǉ�����
		}
		{// Press Button A
			CImage* image = new CImage("asset/image/user_interface/press_button_A.png");
			image->SetSize(XMFLOAT2(267.0f, 70.0f));
			image->SetPosition(XMFLOAT2(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT * 0.9f));
			image->SetColor(XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
			CUserInterfaceManager::AddUI(image, CUserInterfaceManager::LAYER_0);// UI�ɒǉ�����
			m_UI.push_back(image);
		}

		//===== �t�F�[�h�C���J�n ==============
		CFadeManager::StartFadeIn(3.0f);
		m_FrameCounter = 0;

		CManager::Clear() = false;
		CManager::Failed() = false;
	}

	void Uninit()
	{
		// �p�����̏I������
		CScene::Uninit();
	}

	void Update()
	{
		CScene::Update();

		// �L�[���͂ŃV�[���J��
		if (CInput::GetKeyTrigger(VK_RETURN) || CInput::GetGamepadTrigger(XINPUT_GAMEPAD_A)) {
			if (CFadeManager::FadeInComplete()) {
				if (!CFadeManager::FadeOut()) {
					CSound::Play(SOUND_LABEL_SE_BUTTON);
					CFadeManager::StartFadeOut(3.0f);						// �t�F�[�h�A�E�g�J�n
					m_UI[0]->SetColor(XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
					m_FrameCounter = 0;
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


		// �t�F�[�h�A�E�g�I���ŃV�[���J�� ===============================
		if (CFadeManager::FadeOutComplete()) {
			// �Đ����Ă��邷�ׂẴT�E���h���~
			CSound::StopSound();
			CSound::SetVolume(SOUND_LABEL_BGM_TITLE, 0.5f);
			CManager::SetScene<CTitle>();
			return;
		}

		// �_�ł����� =========================================
		if (!CFadeManager::FadeOut()) {
			float a = sinf(m_FrameCounter * 4 * DEGREE_TO_RADIAN) * 0.4f + 0.6f;
			m_UI[0]->SetColor(XMFLOAT4(1.0f, 1.0f, 1.0f, a));
		}

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
		ImGui::Text("[SCENE : RESULT]");

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

#endif // !RESULT_H