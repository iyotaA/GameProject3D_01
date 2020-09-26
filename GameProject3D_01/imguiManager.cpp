
#include "main.h"
#include "renderer.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "imguiManager.h"

// グローバル変数
bool	CImgui::m_ShowWindow			= false;
bool	CImgui::m_ShowAnotherWindow		= false;
ImVec4	CImgui::m_ClearColor			= ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
ImGuiWindowFlags CImgui::m_Flags			= 0;


void CImgui::Init()
{
	// imgui初期化
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(GetWindow());
	ImGui_ImplDX11_Init(CRenderer::GetDevice(), CRenderer::GetDeviceContext());

	m_ShowWindow = true;
	m_ShowAnotherWindow = false;
	m_ClearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);


	m_Flags |= ImGuiWindowFlags_NoTitleBar; // タイトルバーを非表示にします。
	m_Flags |= ImGuiWindowFlags_NoResize; // ウィンドウをリサイズ不可にします。
	m_Flags |= ImGuiWindowFlags_NoMove; // ウィンドウを移動不可にします。
	m_Flags |= ImGuiWindowFlags_NoScrollbar; // スクロールバーを無効にします。
	m_Flags |= ImGuiWindowFlags_NoScrollWithMouse; // マウスホイールでのスクロール操作を無効にします。
	m_Flags |= ImGuiWindowFlags_NoCollapse; // タイトルバーをダブルクリックして閉じる挙動を無効にします。
	m_Flags |= ImGuiWindowFlags_NoBackground; // ウィンドウ内の背景を非表示にします。
	m_Flags |= ImGuiWindowFlags_NoBringToFrontOnFocus; // ウィンドウをクリックしてフォーカスした際に他のウィンドウよりも前面に表示する挙動を無効にします。
	m_Flags |= ImGuiWindowFlags_NoNav; // ゲームパッドやキーボードでのUIの操作を無効にします。
	m_Flags |= ImGuiWindowFlags_NoSavedSettings; // imgui.iniでウィンドウの位置などを自動保存/ロードさせないようにします。
	m_Flags |= ImGuiWindowFlags_AlwaysAutoResize; // 自動でウィンドウ内のコンテンツに合わせてリサイズします。
	m_Flags |= ImGuiWindowFlags_NoFocusOnAppearing; // 表示/非表示の際のトランジションアニメーションを無効にします。
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
