#ifndef DEBUG_PRIMITIVE_H_
#define DEBUG_PRIMITIVE_H_


class CDebugPrimitive
{
public:
	static void DebugPrimitive_Init();
	static void DebugPrimitive_Uninit();

	static void DebugPrimitive_BatchBegin();
	static void DebugPrimitive_BatchRun();

	static void DrawGUI();

	static void DebugPrimitive_BatchCirecleDraw(CCollisionSphere* _sphere, const XMFLOAT4* _color);
	static void DebugPrimitive_BatchCubeDraw(CCollisionOBB* _cube, const XMFLOAT4* _color);

private:
	static std::vector<VERTEX_3D>	m_Vertices;
	static std::vector<WORD>		m_Indices;
	static ID3D11Buffer*			m_VertexBufer;
	static ID3D11Buffer*			m_IndexBufer;
	static CShaderDefault*			m_Shader;

	static unsigned int				m_CircleCount;	// サークル描画数
	static unsigned int				m_CubeCount;	// 立方体描画数
	static bool						m_IsDisplayed;  // 表示するか？

};

#endif // DEBUG_PRIMITIVE_H_

