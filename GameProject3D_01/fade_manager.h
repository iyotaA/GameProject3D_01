#ifndef FADE_MANAGER_H_
#define FADE_MANAGER_H_

class CImage;

static class CFadeManager
{
public:

	//初期化
	static void Init();

	//終了処理
	static void Uninit();

	//更新
	static void Update();

	//色更新
	static void SetColor(XMFLOAT4 _color);

	// フェードアウトスタート
	static void StartFadeOut( float _sec);

	// フェードインスタート
	static void StartFadeIn( float _sec);

	// ゲッタ
	static bool FadeOutComplete() {
		if (m_FadeOutComplete) {
			m_FadeOutComplete = false;
			return true;
		}
		return false;
	}
	static bool FadeInComplete() {
		if (m_FadeInComplete) {
			m_FadeInComplete = false;
			return true;
		}
		return false;
	}
	static bool  FadeIn() { return m_FadeIn; }
	static bool  FadeOut() { return m_FadeOut; }

private:
	static CImage* m_Polygon;
	static XMFLOAT4 m_Color;
	static bool		 m_FadeIn;
	static bool		 m_FadeOut;
	static bool		 m_FadeOutComplete;
	static bool		 m_FadeInComplete;
	static int		 m_FrameCounter;
	static float		 m_CompleteSec;

};

#endif // !FADE_MANAGER_H_
