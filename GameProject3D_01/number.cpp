
// インクルード ////////////////////////////////////
#include "main.h"
#include "renderer.h"
#include "texture.h"
#include "gameObject.h"
#include "2dAnimation.h"
#include "number.h"

#define COUNTER_STOP	(999)
#define MAX_DIGIT		(3)

void CNumber::Init(char* pFileName)
{
	m_Num = 0;

	// テクスチャ読み込み //////
	m_Texture = new CTexture();
	m_Texture->Load(pFileName);
	m_Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
}

void CNumber::Uninit()
{
	delete m_Texture;
}

void CNumber::Update()
{
}

void CNumber::Draw()
{
	int num = m_Num;

	// カンスト処理
	if (num > COUNTER_STOP)	num = COUNTER_STOP;

	int digit = 0;
	int numBuf = num;

	// スコアの桁数を取得
	do
	{
		numBuf /= 10;
		digit++;
	} while (numBuf != 0);

	// ゼロ埋めする数を取得
	int Zero_Count = MAX_DIGIT - digit;

	// スコア表示

	CRenderer::SetDepthEnable(false);
	C2DAnimation::SetTexture(m_Texture);
	for (int i = MAX_DIGIT - 1; i >= 0; i--) {

		int n = num % 10;

		if (!(n < 0 || n > 9)) {

			XMINT2 size = C2DAnimation::GetTextureSize();
			C2DAnimation::Draw(XMFLOAT2(m_Position.x + i * (size.x / 5), m_Position.y), XMFLOAT2((float)(n % 5 * 0.2f), (float)(n / 5 * 0.2f)), XMFLOAT2(size.x / 5, size.y / 5));
		}

		num /= 10;
	}
	CRenderer::SetDepthEnable(true);
}
