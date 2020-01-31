
// �C���N���[�h ////////////////////////////////////
#include "game_objects_all.h"
#include "scene.h"
#include "title.h"
#include "game.h"
#include "result.h"
#include "tutorial.h"
#include "shader_all.h"

// �O���[�o���ϐ� ////////////////////////////////////
CScene*	CManager::m_Scene;
int CManager::m_Score;


void CManager::Init()
{
	// �����_���[�̏�����
	CRenderer::Init();

	// �C���v�b�g�̏�����
	CInput::Init();

	//�J�����̐���
	CCameraManager::CreateCamera();
	CCameraManager::CreateCamera();

	// �V�F�[�_�[�̏�����
	ShaderManager::Init();

	// �R���W�����O���b�h������
	CDebugPrimitive::DebugPrimitive_Init();

	// �V�[���̏�����
	SetScene<CGame>();

	//�J�����}�l�[�W���[�̐ݒ�
	CCameraManager::SetCurrentCameraId(0);
	CCameraManager::Init();

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

	// �V�F�[�_�[�̏I������
	ShaderManager::Uninit();

	// �J�����j��
	CCameraManager::DeleteCamera();

	// �C���v�b�g�̏I������
	CInput::Uninit();

	// �����_���[�̏I������
	CRenderer::Uninit();
}

void CManager::Update()
{
	// �C���v�b�g�̍X�V
	CInput::Update();

	// �J�����X�V
	CCameraManager::Update();

	// �V�[���̍X�V
	m_Scene->Update();
}

void CManager::Draw()
{
	// imgui�`�揀��
	CImgui::NewFrame();

	//// Z�o�b�t�@��������
	//CRenderer::BeginDepth();
	//m_Scene->Draw();

	// �`��̊J�n
	CRenderer::Begin();

	// GUI�̕`��
	DrawGUI();

	// �J�������e
	CCameraManager::Project();

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
