#include "game_objects_all.h"
#include "seqence_manager.h"
#include "user_interface_manager.h"
#include "fade_manager.h"
#include "state_player_died.h"
#include "state_player_idle.h"
#include "modelAnimation.h"
#include "player.h"
#include "MathFunc.h"

CStatePlayerDied::CStatePlayerDied(CPlayer* pPlayer)
	: m_FrameCounter(0)
{
	pPlayer->SetAnimation(PLAYER_STATE_DIED, 1.0f);
	pPlayer->SetAnimationSpeed(1.0f);

	pPlayer->Died() = true;
	pPlayer->DeadTimes() += 1;

	// �Đ����Ă��邷�ׂẴT�E���h���~
	CSound::StopSound();
	CSound::Play(SOUND_LABEL_BGM_ENVIRONMENT);

	// ���S���̃V�[�P���X�Đ�
	CSeqenceManager::PlaySeqence(CSeqenceManager::SEQUENCE_DIED, *pPlayer->GetPosition());
}

CStatePlayerDied::~CStatePlayerDied()
{
	// �S�Ă�UI�̃f���[�g�t���OON
	for (CImage* ui : m_UI) {
		ui->Delete() = true;
	}
}

void CStatePlayerDied::Update(CPlayer* pPlayer)
{
	// �A�j���[�V�����̓��[�v�����Ȃ�
	if (m_FrameCounter == pPlayer->GetCurrentAnimFrameNum() - 1) {
		pPlayer->SetMotionStop(true);
	}

	if (m_FrameCounter == 60 * 1) {
		{	// �u�͐s���܂����v
			CImage* image = new CImage("asset/image/user_interface/died.png");
			image->SetSize(XMFLOAT2(818 * 0.6f, 241 * 0.6f));
			image->SetPosition(XMFLOAT2(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f));
			image->SetColor(XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
			CUserInterfaceManager::AddUI(image, CUserInterfaceManager::LAYER_1);// UI�ɒǉ�����
			m_UI.push_back(image);
		}
		CSound::Play(SOUND_LABEL_SE_KATYA);
	}

	if ( m_FrameCounter == 60 * 4.5) {
		m_UI[m_UI.size()-1]->Delete() = true;	// �ЂƂO��UI������

		if (pPlayer->DeadTimes() < 3) {
			{	// �u�X�^�[�g�ʒu�ɖ߂�܂��v
				CImage* image = new CImage("asset/image/user_interface/startpoint_return.png");
				image->SetSize(XMFLOAT2(1000 * 0.6f, 241 * 0.6f));
				image->SetPosition(XMFLOAT2(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f));
				image->SetColor(XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
				CUserInterfaceManager::AddUI(image, CUserInterfaceManager::LAYER_1);// UI�ɒǉ�����
				m_UI.push_back(image);
			}
			CSound::Play(SOUND_LABEL_SE_KATYA);
		}
		else {
			{	// �u�X�^�[�g�ʒu�ɖ߂�܂��v
				CImage* image = new CImage("asset/image/user_interface/game_over.png");
				image->SetSize(XMFLOAT2(1000 * 0.6f, 241 * 0.6f));
				image->SetPosition(XMFLOAT2(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f));
				image->SetColor(XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
				CUserInterfaceManager::AddUI(image, CUserInterfaceManager::LAYER_1);// UI�ɒǉ�����
				m_UI.push_back(image);
			}
			CSound::Play(SOUND_LABEL_SE_KATYA);
		}
	}
	if (pPlayer->DeadTimes() == 3 && m_FrameCounter == 60 * 8) {
		m_UI[m_UI.size() - 1]->Delete() = true;	// �ЂƂO��UI������
		{	// �uQuest Failed�v
			CImage* image = new CImage("asset/image/user_interface/quest_failed.png");
			image->SetSize(XMFLOAT2(450 * 1.2f, 276 * 1.2f));
			image->SetPosition(XMFLOAT2(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f));
			image->SetColor(XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
			CUserInterfaceManager::AddUI(image, CUserInterfaceManager::LAYER_1);// UI�ɒǉ�����
		}
		CSound::Play(SOUND_LABEL_SE_DON);
	}

	// ===== �t�F�[�h�A�E�g�X�^�[�g ===========================
	if (m_FrameCounter == 60 * 10) {	// 10�b
		// ���񂾉񐔂�2��ȉ��Ȃ烊�X�|�[��
		if (pPlayer->DeadTimes() < 3) {
			if (!CFadeManager::FadeOut()) {
				CFadeManager::StartFadeOut(2.0f);	// �t�F�[�h�A�E�g�J�n
			}
		}
		else {	// 3�񎀂񂾂玸�s
			CManager::Failed() = true;
			return;
		}
	}

	// �t�F�[�h�A�E�g�I���ŃV�[���J�� ===============================
	if (CFadeManager::FadeOutComplete()) {
		pPlayer->SetMotionStop(false);
		pPlayer->ChangeState(new CStatePlayerIdle(pPlayer));
		pPlayer->ReSpwan();		// ���X�|�[��
		CFadeManager::StartFadeIn(3.0f);	// �t�F�[�h�A�E�g�J�n
		return;
	}


	// �J�E���^�[�X�V
	m_FrameCounter++;
}

