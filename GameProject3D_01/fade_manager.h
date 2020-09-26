#ifndef FADE_MANAGER_H_
#define FADE_MANAGER_H_

class CImage;

static class CFadeManager
{
public:

	//������
	static void Init();

	//�I������
	static void Uninit();

	//�X�V
	static void Update();

	//�F�X�V
	static void SetColor(XMFLOAT4 _color);

	// �t�F�[�h�A�E�g�X�^�[�g
	static void StartFadeOut( float _sec);

	// �t�F�[�h�C���X�^�[�g
	static void StartFadeIn( float _sec);

	// �Q�b�^
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
