
#include "main.h"
#include "renderer.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "imguiManager.h"

// �O���[�o���ϐ�
bool	CImgui::m_ShowWindow			= false;
bool	CImgui::m_ShowAnotherWindow		= false;
ImVec4	CImgui::m_ClearColor			= ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
ImGuiWindowFlags CImgui::m_Flags			= 0;


void CImgui::Init()
{
	// imgui������
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(GetWindow());
	ImGui_ImplDX11_Init(CRenderer::GetDevice(), CRenderer::GetDeviceContext());

	m_ShowWindow = true;
	m_ShowAnotherWindow = false;
	m_ClearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);


	m_Flags |= ImGuiWindowFlags_NoTitleBar; // �^�C�g���o�[���\���ɂ��܂��B
	m_Flags |= ImGuiWindowFlags_NoResize; // �E�B���h�E�����T�C�Y�s�ɂ��܂��B
	m_Flags |= ImGuiWindowFlags_NoMove; // �E�B���h�E���ړ��s�ɂ��܂��B
	m_Flags |= ImGuiWindowFlags_NoScrollbar; // �X�N���[���o�[�𖳌��ɂ��܂��B
	m_Flags |= ImGuiWindowFlags_NoScrollWithMouse; // �}�E�X�z�C�[���ł̃X�N���[������𖳌��ɂ��܂��B
	m_Flags |= ImGuiWindowFlags_NoCollapse; // �^�C�g���o�[���_�u���N���b�N���ĕ��鋓���𖳌��ɂ��܂��B
	m_Flags |= ImGuiWindowFlags_NoBackground; // �E�B���h�E���̔w�i���\���ɂ��܂��B
	m_Flags |= ImGuiWindowFlags_NoBringToFrontOnFocus; // �E�B���h�E���N���b�N���ăt�H�[�J�X�����ۂɑ��̃E�B���h�E�����O�ʂɕ\�����鋓���𖳌��ɂ��܂��B
	m_Flags |= ImGuiWindowFlags_NoNav; // �Q�[���p�b�h��L�[�{�[�h�ł�UI�̑���𖳌��ɂ��܂��B
	m_Flags |= ImGuiWindowFlags_NoSavedSettings; // imgui.ini�ŃE�B���h�E�̈ʒu�Ȃǂ������ۑ�/���[�h�����Ȃ��悤�ɂ��܂��B
	m_Flags |= ImGuiWindowFlags_AlwaysAutoResize; // �����ŃE�B���h�E���̃R���e���c�ɍ��킹�ă��T�C�Y���܂��B
	m_Flags |= ImGuiWindowFlags_NoFocusOnAppearing; // �\��/��\���̍ۂ̃g�����W�V�����A�j���[�V�����𖳌��ɂ��܂��B
}

void CImgui::Uninit()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void CImgui::NewFrame()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void CImgui::Draw()
{
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}
