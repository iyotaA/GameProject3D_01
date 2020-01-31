
// インクルード ////////////////////////////////////
#include "game_objects_all.h"
#include "renderer.h"
#include "texture.h"
#include "gameObject.h"
#include "2dAnimation.h"
#include "number.h"


void CNumber::Init()
{
	m_Num = 0;
	m_Position = Vector3(0.0f, 0.0f, 0.0f);

	m_Polygon = new CPolygon2D();
	m_Polygon->Init("asset/image/number.png");
	m_Polygon->SetColor(XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f));
}

void CNumber::Uninit()
{
	m_Polygon->Uninit();
	delete m_Polygon;
}

void CNumber::Update()
{
}

void CNumber::Draw()
{
	int num = static_cast<int>(m_Num);

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


	CRenderer::SetDepthEnable(false);
	XMFLOAT2 size = m_Polygon->Size();
	for (int i = digit; i > 0; i--) {

		int n = num % 10;
		if (!(n < 0 || n > 9)) {

			m_Polygon->SetPosition(XMFLOAT2(m_Position.x + i * size.x * SPACE_MAGNIFICATION + (Zero_Count - 1) * size.x * SPACE_MAGNIFICATION, m_Position.y));
			m_Polygon->SetUV(UV(
				static_cast<float>(n % 5 * 0.2f), static_cast<float>(n / 5 * 0.2f),
				static_cast<float>(n % 5 * 0.2f) + 0.2f, static_cast<float>(n / 5 * 0.2f) + 0.2f)
			);
			m_Polygon->Draw();
		}

		num /= 10;
	}
	CRenderer::SetDepthEnable(true);
}
