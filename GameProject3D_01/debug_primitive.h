#ifndef DEBUG_PRIMITIVE_H_
#define DEBUG_PRIMITIVE_H_


class CDebugPrimitive
{
private:
	static ID3D11Buffer*	m_VertexBufer;
	static ID3D11Buffer*	m_IndexBufer;
	static VERTEX_3D*		m_pVertex;
	static WORD*            m_pIndex;
	static int              m_CircleCount;	// サークル描画数
	static bool				m_IsDisplayed;  // 表示するか？

public:
	static void DebugPrimitive_Init();
	static void DebugPrimitive_Uninit();
	static void DebugPrimitive_BatchBegin();
	static void DebugPrimitive_BatchCirecleDraw(const XMFLOAT3* pos, float radius, const XMFLOAT4* color);
	static void DebugPrimitive_BatchRun();
	static void DrawGUI();

	// set m_IsDisplayed
	static void IsDisplayed(bool flag) { m_IsDisplayed = flag; }
};

#endif // DEBUG_PRIMITIVE_H_

