
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
	m_Position = Vector3(0.0f, 5.0f, -10.0f);
	m_Rotation = Vector3(XMConvertToRadians(15.0f), 0.0f, 0.0f);

	// ビューポート初期化
	m_Viewport.left   = 0;
	m_Viewport.top    = 0;
	m_Viewport.right  = SCREEN_WIDTH;
	m_Viewport.bottom = SCREEN_HEIGHT;

	// 前ベクトル初期化
	m_DirVec.front = Vector3(0.0f, 0.0f, 1.0f);
	m_DirVec.front.Normalize();

	// 右ベクトル初期化
	m_DirVec.up    = Vector3(0.0f, 1.0f, 0.0f);
	m_DirVec.right = m_DirVec.up.VCross(m_DirVec.front);
	m_DirVec.right.Normalize();

	// 上ベクトル初期化
	m_DirVec.up = m_DirVec.front.VCross(m_DirVec.right);
	m_DirVec.up.Normalize();

	// 少しだけ下を向く
	XMMATRIX rotationMtx;
	rotationMtx    = XMMatrixRotationAxis(m_DirVec.right, XMConvertToRadians(15.0f));
	m_DirVec.up    = XMVector3TransformNormal(m_DirVec.up, rotationMtx);
	m_DirVec.front = XMVector3TransformNormal(m_DirVec.front, rotationMtx);
	m_DirVec.front = XMVector3Normalize(m_DirVec.front);
	m_DirVec.up    = XMVector3Normalize(m_DirVec.up);

	/* カメラ情報初期化 */
	m_LengthToAt     = 5.0f;
	m_At             = Vector3(0.0f, 10.0f, 0.0f);
	m_SpinVerticall  = 0.0f;
	m_SpinHorizontal = 0.0f;
	m_RotateSpeed    = 0.004f;
	m_MoveSpeedScale = 1.0f;
	m_BindAtObject = true;
}


void CCamera::Uninit()
{


}


void CCamera::Update()
{
	// 視点移動
	Move();

	// カメラの移動可能範囲か？
	if (IsRange()) {

		Pan();// 視点遷移左右（注視オブジェクトがある場合注視点回転）

		Tilt();// 視点遷移上下（注視オブジェクトがある場合注視点回転）
	}

	// 前フレームとの回転差分
	Vector3 rotationValue =
		Vector3(m_Position.x - m_At.x, m_Position.y - m_At.y, m_Position.z - m_At.z);

	// 回転差分から回転角度計算
	m_Rotation = Vector3(
		XMConvertToRadians(atan2f(rotationValue.y, rotationValue.x)),
		XMConvertToRadians(atan2f(rotationValue.z, rotationValue.x)),
		XMConvertToRadians(atan2f(rotationValue.y, rotationValue.x))
	);

	// カメラの位置更新
	Vector3 vFront = m_DirVec.front * m_LengthToAt;
	m_Position = Vector3(m_At.x - vFront.x, m_At.y + 1.0f - vFront.y, m_At.z - vFront.z);

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

	 Vector3 cameraFront = m_DirVec.front;

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

	Vector3 position  = m_Position;

	// ビューマトリクス設定
	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixLookAtLH(position, m_At, m_DirVec.up));
	CRenderer::SetViewMatrix(&XMLoadFloat4x4(&m_ViewMatrix));

	// プロジェクションマトリクス設定
	XMStoreFloat4x4(&m_ProjectionMatrix ,XMMatrixPerspectiveFovLH(1.0f, dxViewport.Width / dxViewport.Height, 0.1f, 1000.0f));

	CRenderer::SetProjectionMatrix(&XMLoadFloat4x4(&m_ProjectionMatrix));

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
			ImGui::Checkbox("BindAt", &m_BindAtObject);
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

void CCamera::Pan()
{
	if (CInput::GetKeyPress(VK_LEFT)) {
		m_SpinHorizontal -= m_RotateSpeed;
	}
	if (CInput::GetKeyPress(VK_RIGHT)) {
		m_SpinHorizontal += m_RotateSpeed;
	}

	XMMATRIX rotationMtx;
	rotationMtx = XMMatrixRotationY(m_SpinHorizontal);
	m_DirVec.up = XMVector3TransformNormal(m_DirVec.up, rotationMtx);
	m_DirVec.front = XMVector3TransformNormal(m_DirVec.front, rotationMtx);
	m_DirVec.right = XMVector3TransformNormal(m_DirVec.right, rotationMtx);
	m_DirVec.up = XMVector3Normalize(m_DirVec.up);
	m_DirVec.front = XMVector3Normalize(m_DirVec.front);
	m_DirVec.right = XMVector3Normalize(m_DirVec.right);
}

void CCamera::Tilt()
{
	if (CInput::GetKeyPress(VK_UP)) {
		m_SpinVerticall -= m_RotateSpeed;
	}
	if (CInput::GetKeyPress(VK_DOWN)) {
		m_SpinVerticall += m_RotateSpeed;
	}

	XMMATRIX rotationMtx;
	rotationMtx = XMMatrixRotationAxis(m_DirVec.right, m_SpinVerticall);
	m_DirVec.up = XMVector3TransformNormal(m_DirVec.up, rotationMtx);
	m_DirVec.front = XMVector3TransformNormal(m_DirVec.front, rotationMtx);
	m_DirVec.front = XMVector3Normalize(m_DirVec.front);
	m_DirVec.up = XMVector3Normalize(m_DirVec.up);
}

void CCamera::Move()
{
	if (m_pAtObject != NULL)
	{
		if (m_BindAtObject) {
			m_At = Vector3(m_pAtObject->GetPosition()->x, m_pAtObject->GetPosition()->y + 2.0f, m_pAtObject->GetPosition()->z);
			return;
		}
	}

	float speed = CAMERA_MOVE_SPEED * m_MoveSpeedScale;

	if (CInput::GetKeyPress('W')) {
		m_At.x += m_DirVec.front.x * speed;
		m_At.y += m_DirVec.front.y * speed;
		m_At.z += m_DirVec.front.z * speed;
	}
	if (CInput::GetKeyPress('A')) {
		m_At.x -= m_DirVec.right.x * speed;
		m_At.y -= m_DirVec.right.y * speed;
		m_At.z -= m_DirVec.right.z * speed;
	}
	if (CInput::GetKeyPress('S')) {
		m_At.x -= m_DirVec.front.x * speed;
		m_At.y -= m_DirVec.front.y * speed;
		m_At.z -= m_DirVec.front.z * speed;
	}
	if (CInput::GetKeyPress('D')) {
		m_At.x += m_DirVec.right.x * speed;
		m_At.y += m_DirVec.right.y * speed;
		m_At.z += m_DirVec.right.z * speed;
	}
	if (CInput::GetKeyPress('Q')) {
		m_At.y -= speed;
	}
	if (CInput::GetKeyPress('E')) {
		m_At.y += speed;
	}
}

void CCamera::SetAt(CGameObject* pPlayer)
{
	if (pPlayer) {
		m_pAtObject = pPlayer;
		XMFLOAT3 front;
		XMStoreFloat3(&front, m_DirVec.front);
		front.x *= m_LengthToAt;
		front.y *= m_LengthToAt;
		front.z *= m_LengthToAt;

		// カメラの初期位置
		m_Position.x = m_pAtObject->GetPosition()->x - front.x;
		m_Position.y = m_pAtObject->GetPosition()->y - front.y;
		m_Position.z = m_pAtObject->GetPosition()->z - front.z;
	}
	else {
		m_pAtObject = NULL;
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

