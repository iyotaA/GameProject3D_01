
// インクルード ////////////////////////////////////
#include "main.h"
#include "gameObject.h"
#include "camera_manager.h"
#include "camera.h"

void CCamera::Init(unsigned int _id)
{
	m_CameraId = _id;

	// トランスフォーム初期化
	m_Position = Vector3(0.0f, 5.0f, -10.0f);
	m_Rotation = Vector3(XMConvertToRadians(15.0f), 0.0f, 0.0f);

	// ビューポート初期化
	m_Viewport.left   = 0;
	m_Viewport.top    = 0;
	m_Viewport.right  = SCREEN_WIDTH;
	m_Viewport.bottom = SCREEN_HEIGHT;

	// Front_Up_Rightベクトル初期化
	m_DirVec.SetFrontUpRight(Vector3(0.0f, 0.0f, 1.0f));

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
	// 追従移動
	if (m_pAtPoint != NULL)
	{
		if (m_BindAtObject) {
			m_At = Vector3(m_pAtPoint->GetPosition()->x, m_pAtPoint->GetPosition()->y + 2.0f, m_pAtPoint->GetPosition()->z);
		}
	}


	{// 横回転
		XMMATRIX rotationMtx;
		rotationMtx = XMMatrixRotationY(m_SpinHorizontal);

		m_DirVec.up = XMVector3TransformNormal(m_DirVec.up, rotationMtx);
		m_DirVec.up = XMVector3Normalize(m_DirVec.up);

		m_DirVec.front = XMVector3TransformNormal(m_DirVec.front, rotationMtx);
		m_DirVec.front = XMVector3Normalize(m_DirVec.front);

		m_DirVec.right = XMVector3TransformNormal(m_DirVec.right, rotationMtx);
		m_DirVec.right = XMVector3Normalize(m_DirVec.right);
	}

	{// 縦回転
		XMMATRIX rotationMtx;
		rotationMtx = XMMatrixRotationAxis(m_DirVec.right, m_SpinVerticall);

		m_DirVec.up = XMVector3TransformNormal(m_DirVec.up, rotationMtx);
		m_DirVec.up = XMVector3Normalize(m_DirVec.up);

		m_DirVec.front = XMVector3TransformNormal(m_DirVec.front, rotationMtx);
		m_DirVec.front = XMVector3Normalize(m_DirVec.front);
	}


	// カメラの位置更新
	Vector3 vFront = m_DirVec.front * m_LengthToAt;
	m_Position = Vector3(m_At.x - vFront.x, m_At.y + 1.0f - vFront.y, m_At.z - vFront.z);

	// 回転の慣性（徐々にスピードダウン）
	m_SpinVerticall *= 0.92f;
	m_SpinHorizontal *= 0.92f;
}


void CCamera::Project()
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

	// ビューマトリクス設定
	XMMATRIX viewMtx;
	viewMtx = XMMatrixLookAtLH(m_Position, m_At, m_DirVec.up);
	XMStoreFloat4x4(&m_ViewMatrix, viewMtx);
	//CRenderer::SetViewMatrix(&XMLoadFloat4x4(&m_ViewMatrix));

	// ビュー逆行列
	XMStoreFloat4x4(&m_InvViewMatrix, XMMatrixInverse(NULL, viewMtx));

	// プロジェクションマトリクス設定
	XMStoreFloat4x4(&m_ProjectionMatrix ,XMMatrixPerspectiveFovLH(1.0f, dxViewport.Width / dxViewport.Height, 0.1f, 1000.0f));

	//CRenderer::SetProjectionMatrix(&XMLoadFloat4x4(&m_ProjectionMatrix));
}

bool CCamera::GetVisivility(XMFLOAT3* position)const
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
	ImGui::Text("Camera [ %d ]", m_CameraId);

	ImGui::Columns(2);
	{
		ImGui::SliderFloat("LengthToAt", &m_LengthToAt, 1.0f, 200.0f);
		ImGui::SliderFloat("RotateSpeed", &m_RotateSpeed, 0.001f, 0.01f);
		ImGui::SliderFloat("MoveSpeed", &m_MoveSpeedScale, 1.0f, 20.0f);
		ImGui::Checkbox("BindAt", &m_BindAtObject);
	}

	ImGui::Spacing();

	ImGuiID Window_Camera_Id = ImGui::GetID("FocusCamera");

	{
		ImGui::NextColumn();
		ImGui::BeginChildFrame(Window_Camera_Id, ImVec2(ImGui::GetColumnWidth(0), 100));

		ImGui::InputFloat3("AtPosition", (float*)&m_At);

		ImGui::EndChildFrame();
	}
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

void CCamera::Pan(CCameraManager::CameraRotate _rotate_dir)
{
	if (_rotate_dir == CCameraManager::RotateLeft) {
		m_SpinHorizontal -= m_RotateSpeed;
	}
	if (_rotate_dir == CCameraManager::RotateRight) {
		m_SpinHorizontal += m_RotateSpeed;
	}
}

void CCamera::Tilt(CCameraManager::CameraRotate _rotate_dir)
{
	if (_rotate_dir == CCameraManager::RotateUp) {
		m_SpinVerticall -= m_RotateSpeed;
	}
	if (_rotate_dir == CCameraManager::RotateDown) {
		m_SpinVerticall += m_RotateSpeed;
	}
}

void CCamera::Move(CCameraManager::CameraMove _move_dir)
{

	float speed = CAMERA_MOVE_SPEED * m_MoveSpeedScale;

	if (_move_dir == CCameraManager::MoveForward) {
		m_At.x += m_DirVec.front.x * speed;
		m_At.y += m_DirVec.front.y * speed;
		m_At.z += m_DirVec.front.z * speed;
	}
	if (_move_dir == CCameraManager::MoveLeft) {
		m_At.x -= m_DirVec.right.x * speed;
		m_At.y -= m_DirVec.right.y * speed;
		m_At.z -= m_DirVec.right.z * speed;
	}
	if (_move_dir == CCameraManager::MoveBack) {
		m_At.x -= m_DirVec.front.x * speed;
		m_At.y -= m_DirVec.front.y * speed;
		m_At.z -= m_DirVec.front.z * speed;
	}
	if (_move_dir == CCameraManager::MoveRight) {
		m_At.x += m_DirVec.right.x * speed;
		m_At.y += m_DirVec.right.y * speed;
		m_At.z += m_DirVec.right.z * speed;
	}
	if (_move_dir == CCameraManager::MoveDown) {
		m_At.y -= speed;
	}
	if (_move_dir == CCameraManager::MoveUp) {
		m_At.y += speed;
	}
}

void CCamera::SetAt(CGameObject* pAt)
{
	if (pAt) {
		m_pAtPoint = pAt;
		XMFLOAT3 front;
		XMStoreFloat3(&front, m_DirVec.front);
		front.x *= m_LengthToAt;
		front.y *= m_LengthToAt;
		front.z *= m_LengthToAt;

		// カメラの初期位置
		m_Position.x = m_pAtPoint->GetPosition()->x - front.x;
		m_Position.y = m_pAtPoint->GetPosition()->y - front.y;
		m_Position.z = m_pAtPoint->GetPosition()->z - front.z;
	}
	else {
		m_pAtPoint = NULL;
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


void CCamera::SetPos(Vector3* pPos)
{
	m_Position = *pPos;
}


void CCamera::AddPos(Vector3* pAddPos)
{
	m_Position += *pAddPos;
}

