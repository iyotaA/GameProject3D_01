#ifndef IMGUI_MANAGER_H_
#define IMGUI_MANAGER_H_

class CImgui
{
public:
	static void Init();
	static void Uninit();
	static void NewFrame();
	static void Draw();

	static void SetShowWindowState(bool state) { m_ShowWindow = state; }
	static void SetShowAnotherWindowState(bool state) { m_ShowAnotherWindow = state; }

	static bool GetShowWindowState() { return m_ShowWindow; }
	static bool GetShowAnotherWindowState() { return m_ShowAnotherWindow; }
	static ImGuiWindowFlags GetFlags() { return m_Flags; }

private:

	static bool m_ShowWindow;
	static bool m_ShowAnotherWindow;
	static ImVec4 m_ClearColor;
	static ImGuiWindowFlags m_Flags;
};

#endif // !IMGUI_MANAGER_H_
