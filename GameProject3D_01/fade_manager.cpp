#include "main.h"
#include "image.h"
#include "user_interface_manager.h"
#include "fade_manager.h"

CImage* CFadeManager::m_Polygon  = nullptr;
XMFLOAT4 CFadeManager::m_Color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
float		  CFadeManager::m_CompleteSec = 0.0f;
int			  CFadeManager::m_FrameCounter = 0;
bool		  CFadeManager::m_FadeIn  = false;
bool		  CFadeManager::m_FadeOut = false;
bool		  CFadeManager::m_FadeOutComplete = false;
bool		  CFadeManager::m_FadeInComplete = false;
//初期化
void CFadeManager::Init()
{
	// メンバ初期化  ==========================
	m_Color = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	m_CompleteSec = 0.0f;
	m_FrameCounter = 0;
	m_FadeIn = false;
	m_FadeOut = false;
	m_FadeOutComplete = false;
	m_FadeInComplete = false;

	// ポリゴン作成  ===========================
	m_Polygon = new CImage("asset/image/white.png");
	m_Polygon->SetPosition(XMFLOAT2(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f));
	m_Polygon->SetSize(XMFLOAT2(SCREEN_WIDTH, SCREEN_HEIGHT));

	// UIの最前面に追加  =======================
	CUserInterfaceManager::AddUI(m_Polygon, CUserInterfaceManager::LAYER_FOREGROUND);
}


//終了処理
void CFadeManager::Uninit()
{

}


//更新
void CFadeManager::Update()
{
	if (m_FadeIn) {
		// 1 → 0
		float t = 1.0f - m_FrameCounter * DELTA_TIME / m_CompleteSec;

		m_Color.w = t;
		m_Polygon->SetColor(m_Color);

		if (t <= 0.0f) {
			m_FadeIn = false;
			m_FadeInComplete = true;
		}

		m_FrameCounter++;
	}
	else if (m_FadeOut) {
		// 0 → 1
		float t = m_FrameCounter * DELTA_TIME / m_CompleteSec;

		m_Color.w = t;
		m_Polygon->SetColor(m_Color);

		if (t >= 1.0f) {
			m_FadeOut = false;
			m_FadeOutComplete = true;
		}

		m_FrameCounter++;
	}
}


//色更新
void CFadeManager::SetColor(XMFLOAT4 _color)
{
	m_Color = _color;
}


// フェードインスタート
void CFadeManager::StartFadeIn(float _sec)
{
	if (!m_FadeIn && !m_FadeOut) {
		m_FadeIn = true;
		m_FadeInComplete = false;
		m_FrameCounter = 0;
		m_CompleteSec = _sec;
	}
}


// フェードアウトスタート
void CFadeManager::StartFadeOut(float _sec)
{
	if (!m_FadeOut && !m_FadeIn) {
		m_FadeOut = true;
		m_FadeOutComplete = false;
		m_FrameCounter = 0;
		m_CompleteSec = _sec;
	}
}
