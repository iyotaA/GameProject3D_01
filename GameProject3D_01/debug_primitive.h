#ifndef DEBUG_PRIMITIVE_H_
#define DEBUG_PRIMITIVE_H_


class CDebugPrimitive
{
private:
	static ID3D11Buffer*	m_VertexBufer;
	static ID3D11Buffer*	m_IndexBufer;
	static VERTEX_3D*		m_pVertex;
	static WORD*            m_pIndex;
	static int              m_CircleCount;	// ÉTÅ[ÉNÉãï`âÊêî

public:
	static void DebugPrimitive_Init(void);
	static void DebugPrimitive_Uninit(void);
	static void DebugPrimitive_BatchBegin(void);
	static void DebugPrimitive_BatchCirecleDraw(const XMFLOAT3* pos, float radius, const XMFLOAT4* color);
	static void DebugPrimitive_BatchRun(void);
};

#endif // DEBUG_PRIMITIVE_H_

