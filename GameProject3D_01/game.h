#ifndef GAME_H_
#define GAME_H_

class CResult;
#include "MathFunc.h"

class CGame : public CScene
{
public:
	void Init()
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		//		UI�I�u�W�F�N�g�̏�����
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		{// ���C�t�E�X�^�~�i�Q�[�W�̃t���[��
			CImage* image = new CImage("asset/image/user_interface/ui_header.png");
			image->SetSize(XMFLOAT2(720.0f, 160.0f));
			image->SetPosition(XMFLOAT2(360.0f, 80.0f));
			image->SetColor(XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
			CUserInterfaceManager::AddUI(image, CUserInterfaceManager::LAYER_1);// UI�ɒǉ�����
		}
		{// ���C�t�E�X�^�~�i�Q�[�W�̃t���[��
			CImage* image = new CImage("asset/image/user_interface/clock_hand.png");
			image->SetSize(XMFLOAT2(120, 120));
			image->SetPosition(XMFLOAT2(82, 82));
			image->SetColor(XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
			CUserInterfaceManager::AddUI(image, CUserInterfaceManager::LAYER_1);// UI�ɒǉ�����
			m_UI.push_back(image);
		}
		{// �N�G�X�g�{�[�h
			CImage* image = new CImage("asset/image/user_interface/game_quest.png");
			image->SetSize(XMFLOAT2(600.0f, 600.0f));
			image->SetPosition(XMFLOAT2(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f));
			image->SetColor(XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
			CUserInterfaceManager::AddUI(image, CUserInterfaceManager::LAYER_1);// UI�ɒǉ�����
			m_UI.push_back(image);
		}
		{// �A�C�e���̘g
			CImage* image = new CImage("asset/image/user_interface/item_frame.png");
			image->SetSize(XMFLOAT2(490 * 0.3, 390 * 0.4));
			image->SetPosition(XMFLOAT2(1250, 650));
			image->SetColor(XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
			CUserInterfaceManager::AddUI(image, CUserInterfaceManager::LAYER_0);// UI�ɒǉ�����
			m_UI.push_back(image);
		}

		//////////////////////////////////////////////////////////////////////////////////////////////////////
		//		3D�I�u�W�F�N�g�̏�����
		//////////////////////////////////////////////////////////////////////////////////////////////////////
			//�J�����̐���
		CCameraManager::CreateCamera();

		// �X�J�C�h�[���̏�����
		AddGameObject<CSkyDome>(CManager::LAYER_BACKGROUND);

		// �t�B�[���h�̏�����
		AddGameObject<CTerrain>(CManager::LAYER_BACKGROUND);

		// �G�l�~�[�̏�����
		AddGameObject<CEnemy>(CManager::LAYER_OBJECT);

		// �v���C���[�̏�����
		CPlayer* player;
		player = AddGameObject<CPlayer>(CManager::LAYER_OBJECT);

		// �J�����Z�b�g
		CCamera* camera = CCameraManager::GetCamera();		// �����_
		camera->SetAt(player, Vector3(0.0f, 2.3f, 0.0f));

		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// ���͂��֎~����
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		CInput::InputEnable(false);
		// ����̃L�[����͉\�ɂ���
		CInput::EnableKeyCode(VK_RETURN);
		CInput::EnableGamepadButton(XINPUT_GAMEPAD_A);

		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// �V�[�P���X�̏�����
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		CSeqenceManager::Init();

		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// BGM�Đ�
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		CSound::Play(SOUND_LABEL_BGM_ENVIRONMENT);

		m_time = 0.0f;
		m_FrameCounter = 0;
		m_Start = false;

		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// �t�F�[�h�C���X�^�[�g
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		CFadeManager::StartFadeIn(3.0f);
	}

	void Uninit()
	{
		// �Q�[���̏I������
		CSound::StopSound(SOUND_LABEL_BGM_GAME);

		// �V�[�P���X�̏I������
		CSeqenceManager::Uninit();

		// �p�����̏I������
		CScene::Uninit();
	}

	void Update()
	{
		if (CInput::GetGamepadTrigger(XINPUT_GAMEPAD_A) || CInput::GetKeyTrigger(VK_RETURN)) {
			if (!m_Start) {
				m_UI[1]->Delete() = true;
				m_Start = true;
				CSound::Play(SOUND_LABEL_SE_BUTTON);

				//===== ���͂��\�ɖ߂� ==============
				CInput::InputEnable(true);
			}
		}

		// �N���A���́A���s�ŃV�[���J�� ===============================================
		if (CManager::Clear() || CManager::Failed()) {
			if (CFadeManager::FadeInComplete()) {
				if (!CFadeManager::FadeOut()) {
					CFadeManager::StartFadeOut(3.0f);						// �t�F�[�h�A�E�g�J�n
					m_FrameCounter = 0;
				}
			}
		}

		// �t�F�[�h�A�E�g�I���ŃV�[���J�� ===========================================
		if (CManager::Clear() || CManager::Failed()) {
			if (CFadeManager::FadeOutComplete()) {
				CManager::SetScene<CResult>();
				return;
			}
		}

		// ���Ԍv��
		if (!CManager::Clear()) {
			float time = m_FrameCounter * DELTA_TIME / 600.0f * 360.0f;
			if (time >= 360.0f) {
				time = 360.0f;
				CManager::Clear() = true;
			}
			m_UI[0]->SetRotate(time * DEGREE_TO_RADIAN);
		}

		// �V�[�P���X�̏I������
		CSeqenceManager::Update();

		CScene::Update();
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
		ImGui::Text("[SCENE : GAME]");

		// FPS�\��
		DrawFPS();
		ImGui::InputFloat2("position", (float*)m_UI[0]->GetPosition());

		// �R���W�����p�̃f�o�b�O�\��
		CDebugPrimitive::DrawGUI();
		ImGui::End();

		// �J�����̃f�o�b�O�\��
		CCameraManager::DrawGUI();

		// �e�Q�[���I�u�W�F�N�g�̃f�o�b�O�\��
		CScene::DrawGUI();
	}

private:
	std::vector<CImage*> m_UI;
	float m_time;
	int m_FrameCounter;
	bool m_Start;

};


#endif // !GAME_H_
