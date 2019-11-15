
#include "main.h"
#include "renderer.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "imguiManager.h"

// ÉOÉçÅ[ÉoÉãïœêî
bool	CImgui::m_ShowWindow			= false;
bool	CImgui::m_ShowAnotherWindow		= false;
ImVec4	CImgui::m_ClearColor			= ImVec4(0.45f, 0.55f, 0.60f, 1.00f);


void CImgui::Init()
{
	// imguièâä˙âª
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(GetWindow());
	ImGui_ImplDX11_Init(CRenderer::GetDevice(), CRenderer::GetDeviceContext());

	m_ShowWindow = true;
	m_ShowAnotherWindow = false;
	m_ClearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
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
