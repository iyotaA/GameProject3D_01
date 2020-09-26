
// ƒCƒ“ƒNƒ‹[ƒh ////////////////////////////////////
// ‰æ‘œ“Ç‚İ‚İƒ‰ƒCƒuƒ‰ƒŠ‚ğ—˜—p
//   ˆÈ‰º‚Ìƒ}ƒNƒ‚ÅÀ‘•‚à’è‹`‚³‚ê‚é
#define _CRT_SECURE_NO_WARNINGS
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "main.h"
#include "renderer.h"
#include "texture.h"


/*_bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb^
 DirectX Texƒ‰ƒCƒuƒ‰ƒŠ‚ğg‚¤‚Æpng,jpg‚Ì“Ç‚İ‚İ‚ªŠy‚É
u.pngv‚Íulibpngvuzlibvustblibv‚ğg‚¨‚¤
u.ddsv‚Íƒƒ‚ƒŠ‚É‰æ‘œƒf[ƒ^‚ğˆ³k‚µ‚½ó‘Ô‚Å‚Ä‚é‚Ì‚ÅƒAƒNƒZƒX‚ª‘‚¢II
^bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb_*/


void CTexture::Load(const char *FileName)
{
	unsigned char	header[18];
	unsigned char	*image;
	unsigned char	depth;
	unsigned int	bpp;
	unsigned int	size;


	// ƒwƒbƒ_“Ç‚İ‚İ
	FILE* file = nullptr;
	file = fopen(FileName, "rb");
	//assert(file);
	if (file == nullptr) return;

	fread(header, sizeof(header), 1, file);


	// ‰æ‘œƒTƒCƒYæ“¾
	m_Size.x = header[13] * 256 + header[12];
	m_Size.y = header[15] * 256 + header[14];
	depth = header[16];

	if (depth == 32)
		bpp = 4;
	else if (depth == 24)
		bpp = 3;
	else
		bpp = 0;

	if (bpp != 4) {
		/* ƒfƒoƒbƒO‚Ì‚İ“Ç‚İ‚ß‚È‚©‚Á‚½ƒtƒ@ƒCƒ‹‚ğ•\¦ */
#if defined(_DEBUG) || defined(DEBUG)
		char Buf[256];
		sprintf(&Buf[0], "%s‚ğ“Ç‚İ‚ß‚Ü‚¹‚ñ‚Å‚µ‚½", FileName);
		MessageBox(NULL, Buf, "ƒGƒ‰[", MB_OK | MB_ICONASTERISK | MB_DEFBUTTON2);
#endif
		assert(false);
	}

	size = m_Size.x * m_Size.y * bpp;

	// ƒƒ‚ƒŠŠm•Û
	image = (unsigned char*)new unsigned char[size];

	// ‰æ‘œ“Ç‚İ‚İ
	fread(image, size, 1, file);

	// R<->B
	for (unsigned int y = 0; y < m_Size.y; y++)
	{
		for (unsigned int x = 0; x < m_Size.x; x++)
		{
			unsigned char c;
			c = image[(y * m_Size.x + x) * bpp + 0];
			image[(y * m_Size.x + x) * bpp + 0] = image[(y * m_Size.x + x) * bpp + 2];
			image[(y * m_Size.x + x) * bpp + 2] = c;
		}
	}

	fclose(file);


	D3D11_TEXTURE2D_DESC desc;
	desc.Width = m_Size.x;
	desc.Height = m_Size.y;
	desc.MipLevels = 0;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = image;
	initData.SysMemPitch = m_Size.x * bpp;
	initData.SysMemSlicePitch = size;

	auto hr = CRenderer::GetDevice()->CreateTexture2D(&desc, &initData, &m_Texture);
	if (FAILED(hr)) {
		assert(false);
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc = {};
	SRVDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	SRVDesc.Texture2D.MostDetailedMip = 0;
	SRVDesc.Texture2D.MipLevels = 0;

	hr = CRenderer::GetDevice()->CreateShaderResourceView(m_Texture, &SRVDesc, &m_ShaderResourceView);
	if (FAILED(hr))
	{
		assert(false);
	}


	delete image;
}

void CTexture::LoadSTB(const char *FileName)
{
	unsigned char* image;
	int bpp;
	int size;

	// ƒtƒ@ƒCƒ‹‚ğ“Ç‚İ‚İA‰æ‘œƒf[ƒ^‚ğæ‚èo‚·
	//   ÅŒã‚Ìˆø”‚ÅƒsƒNƒZƒ‹‚ ‚½‚è‚ÌƒoƒCƒg”‚ğ‹­§‚Å‚«‚é
	image = stbi_load(FileName, &m_Size.x, &m_Size.y, &bpp, 0);

	size = m_Size.x * m_Size.y * bpp;

	if (bpp == 3) {
		for (unsigned int y = 0; y < m_Size.y; y++)
		{
			for (unsigned int x = 0; x < m_Size.x; x++)
			{
				unsigned char c;
				c = image[(y * m_Size.x + x) * bpp + 0];
				image[(y * m_Size.x + x) * bpp + 0] = image[(y * m_Size.x + x) * bpp + 2];
				image[(y * m_Size.x + x) * bpp + 2] = c;
			}
		}
	}


	D3D11_TEXTURE2D_DESC desc;
	desc.Width = m_Size.x;
	desc.Height = m_Size.y;
	desc.MipLevels = 0;
	desc.ArraySize = 1;
	if (bpp == 3) {
		desc.Format = DXGI_FORMAT_B8G8R8X8_UNORM;
	}
	else if(bpp == 4){
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	}
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = image;
	initData.SysMemPitch = m_Size.x * bpp;
	initData.SysMemSlicePitch = size;

	auto hr = CRenderer::GetDevice()->CreateTexture2D(&desc, NULL, &m_Texture);
	if (FAILED(hr)) {
		assert(false);
	}
	CRenderer::GetDeviceContext()->UpdateSubresource(m_Texture, 0, NULL, image, sizeof(unsigned char) * m_Size.x * bpp, 0);


	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc = {};
	if (bpp == 3) {
		SRVDesc.Format = DXGI_FORMAT_B8G8R8X8_UNORM;
	}
	else if (bpp == 4) {
		SRVDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	}
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	SRVDesc.Texture2D.MostDetailedMip = 0;
	SRVDesc.Texture2D.MipLevels = -1;

	hr = CRenderer::GetDevice()->CreateShaderResourceView(m_Texture, &SRVDesc, &m_ShaderResourceView);
	if (FAILED(hr))
	{
		assert(false);
	}

	CRenderer::GetDeviceContext()->GenerateMips(m_ShaderResourceView);

	// ƒƒ‚ƒŠã‚Ì‰æ‘œƒf[ƒ^‚ğ”jŠü
	stbi_image_free(image);
}

void CTexture::loadTextureFromMemory(const unsigned char* image, int len)
{
	unsigned char* pixels;
	int bpp;

	// ƒtƒ@ƒCƒ‹‚ğ“Ç‚İ‚İA‰æ‘œƒf[ƒ^‚ğæ‚èo‚·
	//   ÅŒã‚Ìˆø”‚ÅƒsƒNƒZƒ‹‚ ‚½‚è‚ÌƒoƒCƒg”‚ğ‹­§‚Å‚«‚é
	pixels = (unsigned char*)stbi_load_from_memory(image, len, &m_Size.x, &m_Size.y, &bpp, 0);
	if (pixels == nullptr) {
		assert(false);
	}

	D3D11_TEXTURE2D_DESC desc;
	D3D11_SUBRESOURCE_DATA initialData;
	HRESULT hr;

	ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));
	desc.Width = m_Size.x;
	desc.Height = m_Size.y;
	desc.MipLevels = 1;
	desc.ArraySize = 1;

	if (bpp == 3) {
		desc.Format = DXGI_FORMAT_B8G8R8X8_UNORM;
	}
	else if (bpp == 4) {
		desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	}

	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	ZeroMemory(&initialData, sizeof(D3D11_SUBRESOURCE_DATA));
	initialData.pSysMem = pixels;
	initialData.SysMemPitch = m_Size.x * bpp;
	initialData.SysMemSlicePitch = m_Size.x * m_Size.y * bpp;

	hr = CRenderer::GetDevice()->CreateTexture2D(&desc, &initialData, &m_Texture);
	if (FAILED(hr))
	{
		assert(false);
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
	ZeroMemory(&SRVDesc, sizeof(SRVDesc));
	SRVDesc.Format = desc.Format;
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	SRVDesc.Texture2D.MostDetailedMip = 0;
	SRVDesc.Texture2D.MipLevels = 1;

	hr = CRenderer::GetDevice()->CreateShaderResourceView(m_Texture, &SRVDesc, &m_ShaderResourceView);

	if (FAILED(hr))
	{
		assert(false);
	}

	// ƒƒ‚ƒŠã‚Ì‰æ‘œƒf[ƒ^‚ğ”jŠü
	stbi_image_free(pixels);
}

void CTexture::Unload()
{
	SAFE_RELEASE(m_Texture);
	SAFE_RELEASE(m_ShaderResourceView);
}



