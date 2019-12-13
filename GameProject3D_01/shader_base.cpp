#include "main.h"
#include "shader_base.h"


void CShaderBase::SetMaterial(MATERIAL Material)
{
	CRenderer::GetDeviceContext()->UpdateSubresource(m_MaterialBuffer, 0, NULL, &Material, 0, 0);
}

void CShaderBase::SetLight(LIGHT Light)
{
	LIGHT light;
	Vector3 dir = Vector3(0.0f, -1.0f, 1.0f);
	dir.Normalize();
	light.Direction = XMFLOAT4(dir.x, dir.y, dir.z, 0.0f);
	light.Diffuse = COLOR(1.0f, 1.0f, 1.0f, 1.0f);
	light.Ambient = COLOR(1.0f, 1.0f, 1.0f, 1.0f);

	CRenderer::GetDeviceContext()->UpdateSubresource(m_LightBuffer, 0, NULL, &light, 0, 0);
}

XMFLOAT4X4 CShaderBase::Transpose(XMFLOAT4X4* Matrix)
{
	XMFLOAT4X4 outMatrix;

	outMatrix._11 = Matrix->_11;
	outMatrix._12 = Matrix->_21;
	outMatrix._13 = Matrix->_31;
	outMatrix._14 = Matrix->_41;

	outMatrix._21 = Matrix->_12;
	outMatrix._22 = Matrix->_22;
	outMatrix._23 = Matrix->_32;
	outMatrix._24 = Matrix->_42;

	outMatrix._31 = Matrix->_13;
	outMatrix._32 = Matrix->_23;
	outMatrix._33 = Matrix->_33;
	outMatrix._34 = Matrix->_43;

	outMatrix._41 = Matrix->_14;
	outMatrix._42 = Matrix->_24;
	outMatrix._43 = Matrix->_34;
	outMatrix._44 = Matrix->_44;

	return outMatrix;
}


