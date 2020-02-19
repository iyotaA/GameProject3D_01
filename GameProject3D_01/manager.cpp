
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
bool CManager::m_GameClear;
bool CManager::m_GameFailed;

void CManager::Init()
{
	// �����_���[�̏�����
	CRenderer::Init();

	// �C���v�b�g�̏�����
	CInput::Init();

	// �T�E���h�̏�����
	CSound::InitSound(GetWindow());

	// �V�F�[�_�[�̏�����
	ShaderManager::Init();

	// �R���W�����O���b�h������
	CDebugPrimitive::DebugPrimitive_Init();

	// �t�F�[�h�}�l�[�W���[�̏�����
	CFadeManager::Init();

	// �V�[���̏�����
	SetScene<CGame>();

	//�J�����}�l�[�W���[�̐ݒ�
	CCameraManager::Init();

	// imgui������
	CImgui::Init();

	m_GameClear = false;
	m_GameFailed = false;
}

void CManager::Uninit()
{
	// imgui�I������
	CImgui::Uninit();

	// �V�[���̏I������
	m_Scene->Uninit();
	delete m_Scene;

	// �t�F�[�h�}�l�[�W���[�̏I������
	CFadeManager::Uninit();

	// �R���W�����O���b�h�I������
	CDebugPrimitive::DebugPrimitive_Uninit();

	// �V�F�[�_�[�̏I������
	ShaderManager::Uninit();

	// UI�j��
	CUserInterfaceManager::DeleteAllUI();

	// �J�����j��
	CCameraManager::DeleteCamera();

	// �T�E���h�̏I������
	CSound::UninitSound();

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

	// �t�F�[�h�}�l�[�W���[�X�V
	CFadeManager::Update();

	// UI�X�V
	CUserInterfaceManager::Update();

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

	// �J�������e
	CCameraManager::Project();

	// �V�[���̕`��
	m_Scene->Draw();
	//m_Scene->DrawGUI();

	// UI�`��
	CUserInterfaceManager::Draw();

	// imgui�`��
	CImgui::Draw();

	CRenderer::End();
	// �`��̏I��

}

CScene* CManager::GetScene()
{
	return m_Scene;
}
