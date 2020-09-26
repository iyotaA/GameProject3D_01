
// インクルード ////////////////////////////////////
#include "main.h"
#include "game_objects_all.h"
#include "scene.h"
#include "camera_manager.h"
#include "camera.h"

//=======================================
//	関数名	: Init()
//-----------------------------------------------------------------------
// 機能	: 初期化
// 引数	:
//     _id = カメラ識別番号
// 戻り値	:　void
//=======================================
void CCamera::Init(unsigned int _id)
{
	m_CameraId = _id;

	// トランスフォーム初期化
	m_Position = Vector3(0.0f, 5.0f, -10.0f);
	m_Rotation = Vector3(0.0f, 0.0f, 0.0f);
	m_PrevRotation = Vector3(0.0f, 0.0f, 0.0f);

	// ビューポート初期化
	m_Viewport.left   = 0;
	m_Viewport.top    = 0;
	m_Viewport.right  = SCREEN_WIDTH;
	m_Viewport.bottom = SCREEN_HEIGHT;

	// Front_Up_Rightベクトル初期化
	m_DirVec.SetFrontUpRight(Vector3(0.0f, 0.0f, 1.0f));

	/* カメラ情報初期化 */
	m_LengthToAt     = 6.5f;
	m_FrameCounter = 0;
	m_At             = Vector3(0.0f, 10.0f, 0.0f);
	m_Offset		 = Vector3();
	m_SpinVerticall  = 0.0f;
	m_SpinHorizontal = 0.0f;
	m_RotateSpeed    = 0.004f;
	m_MoveSpeedScale = 1.0f;
	m_BindAtObject = false;
	m_Shake = false;
}


//=======================================
//	関数名	: Uninit()
//-----------------------------------------------------------------------
// 機能	: 終了処理
// 引数	: 無し
// 戻り値	:　void
//=======================================
void CCamera::Uninit()
{


}


//=======================================
//	関数名	: Update()
//-----------------------------------------------------------------------
// 機能	: 更新
// 引数	: 無し
// 戻り値	:　void
//=======================================
void CCamera::Update()
{
	// 追従移動
	if (m_pAtPoint != NULL)
	{
		if (m_BindAtObject) {
			m_At = m_pAtPoint->GetAt() + m_Offset;
		}
	}

	// 視点の方向を切り替える
	UpdateDirection();

	// カメラシェイク（有効時のみ）
	Shake();

	// 位置更新
	Vector3 vFront = m_DirVec.front * m_LengthToAt;
	m_Position = m_At - vFront;

	// 地面との衝突が無ければカメラの位置更新
	CollisionTerrian();
}


//=======================================
//	関数名	: Project()
//-----------------------------------------------------------------------
// 機能	: 座標・上方向・注視点を元にビューマトリクス生成
//           プロジェクションマトリクス生成
// 引数	: 無し
// 戻り値	:　void
//=======================================
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

	// ビュー逆行列
	XMStoreFloat4x4(&m_InvViewMatrix, XMMatrixInverse(NULL, viewMtx));

	// プロジェクションマトリクス設定
	XMStoreFloat4x4(&m_ProjectionMatrix ,XMMatrixPerspectiveFovLH(1.0f, dxViewport.Width / dxViewport.Height, 0.1f, 1000.0f));
}


//=======================================
//	関数名	: GetVisivility()
//-----------------------------------------------------------------------
// 機能	: 対象のワールド座標が視野内にあるかの判定
// 引数	:
//     position :: 判定対象のワールド座標ポインタ
// 戻り値	: 視野内に入っていたらtrue
//=======================================
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


//=======================================
//	関数名	: DrawGUI()
//-----------------------------------------------------------------------
// 機能	: ImGuiによるデバッグウィンドウへの描画
// 引数	: 無し
// 戻り値	: void
//=======================================
void CCamera::DrawGUI()
{
	{
		ImGui::SliderFloat("LengthToAt", &m_LengthToAt, 1.0f, 200.0f);
		ImGui::SliderFloat("RotateSpeed", &m_RotateSpeed, 0.001f, 0.05f);
		ImGui::SliderFloat("MoveSpeed", &m_MoveSpeedScale, 1.0f, 20.0f);
	}

	ImGui::Spacing();
	ImGui::InputFloat3("AtPosition", (float*)&m_At);
	ImGui::InputFloat3("Rotate", (float*)&m_Rotation);
	ImGui::Checkbox("BindAt", &m_BindAtObject);
}


//=======================================
//	関数名	: UpdateDirection()
//-----------------------------------------------------------------------
// 機能	: 回転角度を元に視点方向を回転させる
// 引数	: 無し
// 戻り値	: void
//=======================================
void CCamera::UpdateDirection()
{
	Vector3X3 prevDir = m_DirVec;

	{// 横回転
		XMMATRIX rotationMtx;
		//rotationMtx = XMMatrixRotationY(m_SpinHorizontal);
		rotationMtx = XMMatrixRotationRollPitchYaw(0.0f, m_SpinHorizontal, 0.0f);

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

	// オブジェクトに追従する場合は角度を制限
	if (m_BindAtObject) {
		Vector3 vecY = Vector3(0.0f, 1.0f, 0.0f); // Y軸ベクトル
		float dot = vecY.VDot(m_DirVec.front);
		if (!(dot >= -0.6f && dot <= 0.6f)) {
			m_DirVec = prevDir;
			m_PrevRotation = m_Rotation;
			m_SpinVerticall = 0.0f;
			m_SpinHorizontal = 0.0f;
			return;
		}
	}

	// カメラの回転を徐々に弱める
	m_SpinVerticall *= 0.92f;
	m_SpinHorizontal *= 0.92f;
}


//=======================================
//	関数名	: CollisionTerrian()
//-----------------------------------------------------------------------
// 機能	: 地形との当たり判定と押し出し
// 引数	: 無し
// 戻り値	: 地形と衝突したらtrue
//=======================================
bool CCamera::CollisionTerrian()
{
	// 地面へのアクセスポインタゲット
	CTerrain* pTerrain = CManager::GetScene()->GetGameObject<CTerrain>(CManager::LAYER_BACKGROUND);
	if (!pTerrain)return false;

	// 地面とのコリジョン
	float height = pTerrain->GetHeight(&(m_Position + Vector3(0.0f, 0.5f, 0.0f)));
	if (FAILD_NUM != (int)height) {

		if (m_Position.y <= height) {
			m_Position.y = height + 0.5f;
			return true;
		}
		else {
			return false;
		}
	}
}


//=======================================
//	関数名	: SetRotation()
//-----------------------------------------------------------------------
// 機能	: カメラの向きをセット
// 引数	: 角度（ラジアン）
// 戻り値	: void
//=======================================
void CCamera::SetRotation(Vector3* rotation)
{
	m_PrevRotation = m_Rotation;
	m_Rotation = *rotation;

	// 回転量計算
	m_SpinHorizontal = m_Rotation.y - m_PrevRotation.y;
	m_SpinVerticall    = m_Rotation.x - m_PrevRotation.x;
	m_PrevRotation = m_Rotation;
}


//=======================================
//	関数名	: Pan()
//-----------------------------------------------------------------------
// 機能	: 横方向の回転
// 引数	:
//     _rotate_dir :: 左右判定ENUM
// 戻り値	: void
//=======================================
void CCamera::Pan(CCameraManager::CameraRotate _rotate_dir)
{
	if (_rotate_dir == CCameraManager::RotateLeft) {
		m_Rotation.y -= m_RotateSpeed;
	}
	if (_rotate_dir == CCameraManager::RotateRight) {
		m_Rotation.y += m_RotateSpeed;
	}

	m_SpinHorizontal += m_Rotation.y - m_PrevRotation.y;
	m_PrevRotation = m_Rotation;
}


//=======================================
//	関数名	: Tilt()
//-----------------------------------------------------------------------
// 機能	: 縦方向の回転
// 引数	:
//     _rotate_dir :: 上下判定ENUM
// 戻り値	: void
//=======================================
void CCamera::Tilt(CCameraManager::CameraRotate _rotate_dir)
{
	if (_rotate_dir == CCameraManager::RotateUp) {
		m_Rotation.x -= m_RotateSpeed;
	}
	if (_rotate_dir == CCameraManager::RotateDown) {
		m_Rotation.x += m_RotateSpeed;
	}

	m_SpinVerticall += m_Rotation.x - m_PrevRotation.x;
	m_PrevRotation = m_Rotation;
}


//=======================================
//	関数名	: Move()
//-----------------------------------------------------------------------
// 機能	: 注視オブジェクトが無い時の注視点移動
// 引数	:
//     _move_dir :: 移動方向ENUM
// 戻り値	: void
//=======================================
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


//=======================================
//	関数名	: SetAt()
//-----------------------------------------------------------------------
// 機能	: 注視点オブジェクトをセット（CGameObject*）
// 引数	:
//     pAt :: バインドするオブジェクトのポインタ
//     offset :: オブジェクトからの注視点のオフセット
// 戻り値	: void
//=======================================
void CCamera::SetAt(CGameObject* pAt, Vector3 offset)
{
	if (pAt) {
		m_pAtPoint = pAt;
		m_Offset = offset;

		// カメラの初期位置
		m_Position = m_pAtPoint->GetAt() + m_Offset - m_DirVec.front;
		m_BindAtObject = true;
	}
	else {
		m_pAtPoint = NULL;

		// カメラの初期位置
		m_Position = Vector3(0.0f, 10.0f, -10.0f);
	}
}


//=======================================
//	関数名	: Shake()
//-----------------------------------------------------------------------
// 機能	: カメラシェイク
// 引数	: 無し
// 戻り値	: void
//=======================================
void CCamera::Shake()
{
	if (!m_Shake)return;
	if (m_FrameCounter * DELTA_TIME >= 0.5f) {
		m_Shake = false;
		m_Offset = Vector3(0.0f, 2.3f, 0.0f);
		m_FrameCounter = 0;
		return;
	}

	// 2F毎に座標移動
	if (m_FrameCounter++ % 2 == 0) {

		XMFLOAT2 shake = XMFLOAT2(rand() % 10 / 40.0f - 0.125f, rand() % 10 / 40.0f - 0.125f);
		m_Offset = Vector3(0.0f, 2.3, 0.0f); + m_DirVec.right * shake.x + m_DirVec.up * shake.y;
	}
}


void CCamera::SetPos(Vector3* pPos)
{
	m_Position = *pPos;
}


void CCamera::SetAtPos(Vector3* pPos)
{
	m_At = *pPos;
}


void CCamera::AddPos(Vector3* pAddPos)
{
	m_Position += *pAddPos;
}
