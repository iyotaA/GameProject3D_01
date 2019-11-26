#ifndef ANIMATION_H_
#define ANIMATION_H_

class C2DAnimation
{
public:
	static void Draw(XMFLOAT2 position, XMFLOAT2 uv0, XMFLOAT2 cutSize);
	static void SetTexture(CTexture* texture) { m_Texture = texture; }

	static XMINT2 GetTextureSize() {
		if (m_Texture != nullptr) {
			return *m_Texture->GetSize();
		}
	}

private:
	static ID3D11Buffer*	m_VertexBufer;
	static CTexture*		m_Texture;

};

#endif