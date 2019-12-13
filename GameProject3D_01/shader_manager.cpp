#include "main.h"
#include "shader_all.h"

std::vector<CShaderBase*>	ShaderManager::m_Shaders;


// �V�F�[�_�[�}�l�[�W���[������
void ShaderManager::Init()
{
	AddShader<CShaderDefault>("asset/shader/shader3DDefaultVS.cso", "asset/shader/shader3DDefaultPS.cso");
	AddShader<CShaderPerPixelLight>("asset/shader/shader3DPerPixelLightingVS.cso", "asset/shader/shader3DPerPixelLightingPS.cso");
	AddShader<CShaderMultiTexture>("asset/shader/shader3DMultiTextureVS.cso", "asset/shader/shader3DMultiTexturePS.cso");
	AddShader<CShaderNormalMap>("asset/shader/shader3DNormalMappingVS.cso", "asset/shader/shader3DNormalMappingPS.cso");
	AddShader<CShaderWater>("asset/shader/shader3DWaterVS.cso", "asset/shader/shader3DWaterPS.cso");
}

// �S�ẴV�F�[�_�[�̏I������
void ShaderManager::Uninit()
{
	for (CShaderBase* shader : m_Shaders) {

		shader->Uninit();
		delete shader;
	}

	// ���X�g�̃N���A
	m_Shaders.clear();
}


