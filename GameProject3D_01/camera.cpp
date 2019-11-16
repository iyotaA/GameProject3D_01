
// インクルード ////////////////////////////////////
#include "game_objects_all.h"
#include "scene.h"
#include "title.h"
#include "game.h"
#include "result.h"
#include "tutorial.h"


void CCamera::Init()
{
	// トランスフォーム初期化
	m_Position = XMFLOAT3(0.0f, 5.0f, -10.0f);
	m_Rotation = XMFLOAT3(XMConvertToRadians(15.0f), 0.0f, 0.0f);

	// ビューポート初期化
	m_Viewport.left   = 0;
	m_Viewport.top    = 0;
	m_Viewport.right  = SCREEN_WIDTH;
	m_Viewport.bottom = SCREEN_HEIGHT;

	// 前ベクトル初期化
	m_DirVec.front = { 0.0f, 0.0f, 1.0f};
	m_DirVec.front = XMVector3Normalize(m_DirVec.front);

	// 右ベクトル初期化
	m_DirVec.up = { 0.0f, 1.0f, 0.0f };
	m_DirVec.right = XMVector3Cross(m_DirVec.up, m_DirVec.front);
	m_DirVec.right = XMVector3Normalize(m_DirVec.right);

	// 上ベクトル初期化
	m_DirVec.up = XMVector3Cross(m_DirVec.front, m_DirVec.right);
	m_DirVec.up = XMVector3Normalize(m_DirVec.up);


	//XMMATRIX rotationMtx;
	//rotationMtx = XMMatrixRotationAxis(m_DirVec.right, XMConvertToRadians(15.0f));
	//m_DirVec.up = XMVector3TransformNormal(m_DirVec.up, rotationMtx);
	//m_DirVec.front = XMVector3TransformNormal(m_DirVec.front, rotationMtx);
	//m_DirVec.front = XMVector3Normalize(m_DirVec.front);
	//m_DirVec.up = XMVector3Normalize(m_DirVec.up);

	/* カメラ情報初期化 */
	m_LengthToAt = 5.0f;									// 注視点までの距離を設定
	m_At = { 0.0f, 0.0f, 0.0f };
	m_SpinVerticall  = 0.0f;
	m_SpinHorizontal = 0.0f;
	m_RotateSpeed = 0.004f;
	m_MoveSpeedScale = 1.0f;
}


void CCamera::Uninit()
{


}


void CCamera::Update()
{
	XMFLOAT3 at;
	XMStoreFloat3(&at, m_At);

	XMFLOAT3 cameraFront;
	XMFLOAT3 cameraRight;
	XMStoreFloat3(&cameraFront, m_DirVec.front);
	XMStoreFloat3(&cameraRight, m_DirVec.right);

	float speed = CAMERA_MOVE_SPEED * m_MoveSpeedScale;

	// 視点の縦横回転
	if (CInput::GetKeyPress(VK_UP)) {
		m_SpinVerticall -= m_RotateSpeed;
	}
	if (CInput::GetKeyPress(VK_LEFT)) {
		m_SpinHorizontal -= m_RotateSpeed;
	}
	if (CInput::GetKeyPress(VK_DOWN)) {
		m_SpinVerticall += m_RotateSpeed;
	}
	if (CInput::GetKeyPress(VK_RIGHT)) {
		m_SpinHorizontal += m_RotateSpeed;
	}

	if (!CInput::GetKeyPress(VK_RSHIFT))
	{

		if (CInput::GetKeyPress('W')) {
			m_Position.x += cameraFront.x * speed;
			m_Position.y += cameraFront.y * speed;
			m_Position.z += cameraFront.z * speed;
		}
		if (CInput::GetKeyPress('A')) {
			m_Position.x -= cameraRight.x * speed;
			m_Position.y -= cameraRight.y * speed;
			m_Position.z -= cameraRight.z * speed;
		}
		if (CInput::GetKeyPress('S')) {
			m_Position.x -= cameraFront.x * speed;
			m_Position.y -= cameraFront.y * speed;
			m_Position.z -= cameraFront.z * speed;
		}
		if (CInput::GetKeyPress('D')) {
			m_Position.x += cameraRight.x * speed;
			m_Position.y += cameraRight.y * speed;
			m_Position.z += cameraRight.z * speed;
		}
		if (CInput::GetKeyPress('Q')) {
			m_Position.y -= speed;
		}
		if (CInput::GetKeyPress('E')) {
			m_Position.y += speed;
		}
	}

	// カメラの移動可能範囲か？
	if (IsRange()) {

		XMMATRIX rotationMtx;
		rotationMtx = XMMatrixRotationAxis(m_DirVec.right, m_SpinVerticall);
		m_DirVec.up = XMVector3TransformNormal(m_DirVec.up, rotationMtx);
		m_DirVec.front = XMVector3TransformNormal(m_DirVec.front, rotationMtx);
		m_DirVec.front = XMVector3Normalize(m_DirVec.front);
		m_DirVec.up = XMVector3Normalize(m_DirVec.up);

		rotationMtx = XMMatrixRotationY(m_SpinHorizontal);
		m_DirVec.up = XMVector3TransformNormal(m_DirVec.up, rotationMtx);
		m_DirVec.front = XMVector3TransformNormal(m_DirVec.front, rotationMtx);
		m_DirVec.right = XMVector3TransformNormal(m_DirVec.right, rotationMtx);
		m_DirVec.up = XMVector3Normalize(m_DirVec.up);
		m_DirVec.front = XMVector3Normalize(m_DirVec.front);
		m_DirVec.right = XMVector3Normalize(m_DirVec.right);
	}

	//XMFLOAT3 rotationValue;
	//rotationValue.x = m_Position.x - at.x;
	//rotationValue.y = m_Position.y - at.y;
	//rotationValue.z = m_Position.z - at.z;

	//m_Rotation.x = XMConvertToRadians(atan2f(rotationValue.y, rotationValue.x));
	//m_Rotation.y = XMConvertToRadians(atan2f(rotationValue.z, rotationValue.x));
	//m_Rotation.z = XMConvertToRadians(atan2f(rotationValue.y, rotationValue.x));

	// XMVECTOR -> XMFLOAT3
	XMFLOAT3 vFront;
	XMStoreFloat3(&vFront, m_DirVec.front);
	vFront.x *= m_LengthToAt;
	vFront.y *= m_LengthToAt;
	vFront.z *= m_LengthToAt;

	// カメラの位置更新
	//m_Position.x = at.x - vFront.x;
	//m_Position.y = at.y + 1.0f - vFront.y;
	//m_Position.z = at.z - vFront.z;

	if (m_pPlayer != nullptr) {
		m_At = XMLoadFloat3(
			&XMFLOAT3(
				m_pPlayer->GetPosition().x,
				m_pPlayer->GetPosition().y + 1.0f,
				m_pPlayer->GetPosition().z
			)
		);
	}
	else {
		m_At = XMLoadFloat3(
			&XMFLOAT3(
				m_Position.x + cameraFront.x * m_LengthToAt,
				m_Position.y + cameraFront.y * m_LengthToAt,
				m_Position.z + cameraFront.z * m_LengthToAt
			)
		);
	}


	// 慣性（徐々にスピードダウン）
	m_SpinVerticall *= 0.92f;
	m_SpinHorizontal *= 0.92f;
}



void CCamera::Draw()
{
	// ビューポート設定
	D3D11_VIEWPORT dxViewport;
	dxViewport.Width    = (float)(m_Viewport.right - m_Viewport.left);
	dxViewport.Height   = (float)(m_Viewport.bottom - m_Viewport.top);
	dxViewport.MinDepth = 0.0f;
	dxViewport.MaxDepth = 1.0f;
	dxViewport.TopLeftX = (float)m_Viewport.left;
	dxViewport.TopLeftY = (float)m_Viewport.top;

	CRenderer::GetDeviceContext()->RSSetViewports(1, &dxViewport);

	// インバースビューマトリクス設定
	XMMATRIX invMtx = XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);
	invMtx *= XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
	XMStoreFloat4x4(&m_InvViewMatrix, invMtx);

	XMFLOAT3 cameraFront;
	XMStoreFloat3(&cameraFront, m_DirVec.front);

	//if (m_pPlayer != nullptr) {
	//	m_At = XMLoadFloat3(
	//		&XMFLOAT3(
	//			m_pPlayer->GetPosition().x,
	//			m_pPlayer->GetPosition().y + 1.0f,
	//			m_pPlayer->GetPosition().z
	//		)
	//	);
	//}
	//else {
	//	m_At = XMLoadFloat3(
	//		&XMFLOAT3(
	//			m_Position.x + cameraFront.x * m_LengthToAt,
	//			m_Position.y + cameraFront.y * m_LengthToAt,
	//			m_Position.z + cameraFront.z * m_LengthToAt
	//		)
	//	);
	//}

	// XMFLOAT3 -> XMVECTOR
	XMVECTOR position  = XMLoadFloat3(&m_Position);

	// ビューマトリクス設定
	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixLookAtLH(position, m_At, m_DirVec.up));
	CRenderer::SetViewMatrix(&XMLoadFloat4x4(&m_ViewMatrix));

	// プロジェクションマトリクス設定
	XMStoreFloat4x4(&m_ProjectionMatrix ,XMMatrixPerspectiveFovLH(1.0f, dxViewport.Width / dxViewport.Height, 0.1f, 1000.0f));

	CRenderer::SetProjectionMatrix(&XMLoadFloat4x4(&m_ProjectionMatrix));

	XMFLOAT3 At;
	XMStoreFloat3(&At, m_At);

	// ImGui
	DrawGUI();
}

bool CCamera::GetVisivility(XMFLOAT3* position)
{
	XMFLOAT3 pos = *position;

	XMVECTOR worldPos, viewPos, projPos;
	XMFLOAT3 projPosF;

	worldPos = XMLoadFloat3(position);
	viewPos = XMVector3TransformCoord(worldPos, XMLoadFloat4x4(&m_ViewMatrix));
	projPos = XMVector3TransformCoord(viewPos, XMLoadFloat4x4(&m_ProjectionMatrix));
	XMStoreFloat3(&projPosF, projPos);


	return
		(-1.0f < projPosF.x && projPosF.x < 1.0f &&
			-1.0f < projPosF.y && projPosF.y < 1.0f &&
			0.0f < projPosF.z && projPosF.z < 1.0f)
		? true : false;
}


void CCamera::DrawGUI()
{
	ImGui::Begin("System");
	ImGuiID Window_System_Id = ImGui::GetID("System");

	if (ImGui::CollapsingHeader("Camera"))
	{
		ImGui::Columns(2);
		{
			ImGui::SliderFloat("LengthToAt", &m_LengthToAt, 1.0f, 200.0f);
			ImGui::SliderFloat("RotateSpeed", &m_RotateSpeed, 0.001f, 0.01f);
			ImGui::SliderFloat("MoveSpeed", &m_MoveSpeedScale, 1.0f, 20.0f);
		}

		ImGui::Spacing();

		ImGuiID Window_Camera_Id = ImGui::GetID("Camera");


		{
			ImGui::NextColumn();
			ImGui::BeginChildFrame(Window_Camera_Id, ImVec2(ImGui::GetColumnWidth(0), 100));

			ImGui::Text("PosX = %.1f", m_Position.x);
			ImGui::Text("PosY = %.1f", m_Position.y);
			ImGui::Text("PosZ = %.1f", m_Position.z);

			ImGui::EndChildFrame();
		}
	}

	ImGui::End();
}

bool CCamera::IsRange()
{
	return true;
	XMMATRIX rotationMtx;
	XMVECTOR vector = m_DirVec.up; // カメラ上方向ベクトルを取得

	rotationMtx = XMMatrixRotationAxis(m_DirVec.right, m_SpinVerticall);
	vector = XMVector3TransformNormal(vector, rotationMtx);
	vector = XMVector3Normalize(vector);

	XMFLOAT3 up;
	XMStoreFloat3(&up, vector);

	if (up.y >= 0.7f)	return true;
	else return false;
}

void CCamera::SetAt(CPlayer* pPlayer)
{
	if (pPlayer) {
		m_pPlayer = pPlayer;
		XMFLOAT3 front;
		XMStoreFloat3(&front, m_DirVec.front);
		front.x *= m_LengthToAt;
		front.y *= m_LengthToAt;
		front.z *= m_LengthToAt;

		// カメラの初期位置
		m_Position.x = m_pPlayer->GetPosition().x - front.x;
		m_Position.y = m_pPlayer->GetPosition().y - front.y;
		m_Position.z = m_pPlayer->GetPosition().z - front.z;
	}
	else {
		m_pPlayer = NULL;
		XMFLOAT3 front;
		XMStoreFloat3(&front, m_DirVec.front);
		front.x *= m_LengthToAt;
		front.y *= m_LengthToAt;
		front.z *= m_LengthToAt;

		XMFLOAT3 at;
		XMStoreFloat3(&at, m_At);

		// カメラの初期位置
		m_Position.x = 0.0f;
		m_Position.y = 10.0f;
		m_Position.z = -10.0f;
	}
}

