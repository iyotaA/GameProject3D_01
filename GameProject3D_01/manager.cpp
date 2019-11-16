
// �C���N���[�h ////////////////////////////////////
#include "game_objects_all.h"
#include "scene.h"
#include "title.h"
#include "game.h"
#include "result.h"
#include "tutorial.h"

// �O���[�o���ϐ� ////////////////////////////////////
CScene*	CManager::m_Scene;
int CManager::m_Score;


void CManager::Init()
{
	// �����_���[�̏�����
	CRenderer::Init();

	// �C���v�b�g�̏�����
	CInput::Init();

	// �R���W�����O���b�h������
	CDebugPrimitive::DebugPrimitive_Init();

	// �V�[���̏�����
	SetScene<CGame>();

	// imgui������
	CImgui::Init();

	m_Score = 0;
}

void CManager::Uninit()
{
	// imgui�I������
	CImgui::Uninit();

	// �V�[���̏I������
	m_Scene->Uninit();
	delete m_Scene;

	// �R���W�����O���b�h�I������
	CDebugPrimitive::DebugPrimitive_Uninit();

	// �C���v�b�g�̏I������
	CInput::Uninit();

	// �����_���[�̏I������
	CRenderer::Uninit();
}

void CManager::Update()
{
	// �C���v�b�g�̍X�V
	CInput::Update();

	// �V�[���̍X�V
	m_Scene->Update();
}

void CManager::Draw()
{

	// �`��̊J�n
	CRenderer::Begin();

	// imgui�`�揀��
	CImgui::NewFrame();

	// GUI�̕`��
	DrawGUI();

	// �V�[���̕`��
	m_Scene->Draw();

	// imgui�`��
	CImgui::Draw();

	CRenderer::End();
	// �`��̏I��

}

void CManager::DrawGUI()
{
	DrawFPS();
}

CScene* CManager::GetScene()
{
	return m_Scene;
}
