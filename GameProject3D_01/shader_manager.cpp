#include "main.h"
#include "shader_all.h"

std::vector<ShaderBase*>	ShaderManager::m_Shaders;



// �V�F�[�_�[�}�l�[�W���[������
void ShaderManager::Init()
{
	AddShader<CShaderDefault>("asset/shader/shader3DDefaultVS.cso", "asset/shader/shader3DDefaultPS.cso");
	AddShader<CShaderDefault>("asset/shader/shader3DDefaultVS.cso", "asset/shader/shader3DDefaultPS.cso");
}

