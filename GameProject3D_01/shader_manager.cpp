#include "main.h"
#include "shader_all.h"

std::vector<ShaderBase*>	ShaderManager::m_Shaders;


// シェーダーマネージャー初期化
void ShaderManager::Init()
{
	AddShader<CShaderDefault>("asset/shader/shader3DDefaultVS.cso", "asset/shader/shader3DDefaultPS.cso");
	AddShader<CShaderPerPixelLight>("asset/shader/shader3DPerPixelLightingVS.cso", "asset/shader/shader3DPerPixelLightingPS.cso");
	AddShader<CShaderMultiTexture>("asset/shader/shader3DMultiTextureVS.cso", "asset/shader/shader3DMultiTexturePS.cso");
}

// 全てのシェーダーの終了処理
void ShaderManager::Uninit()
{
	for (ShaderBase* shader : m_Shaders) {

		shader->Uninit();
		delete shader;
	}

	// リストのクリア
	m_Shaders.clear();
}


