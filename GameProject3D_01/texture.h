#ifndef TEXTURE_H_
#define TEXTURE_H_

class CTexture
{
private:

	ID3D11Texture2D*			m_Texture;
	ID3D11ShaderResourceView*	m_ShaderResourceView;
	XMINT2						m_Size;

public:

	void Load(const char *FileName);
	void LoadSTB(const char *FileName);
	void loadTextureFromMemory(const unsigned char* image, int len);
	void Unload();
	const XMINT2* GetSize() { return &m_Size; }

	ID3D11ShaderResourceView* GetShaderResourceView(){ return m_ShaderResourceView; }


};

#endif
