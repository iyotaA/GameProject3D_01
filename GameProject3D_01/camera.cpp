
// �C���N���[�h ////////////////////////////////////
#include "main.h"
#include "game_objects_all.h"
#include "scene.h"
#include "camera_manager.h"
#include "camera.h"

//=======================================
//	�֐���	: Init()
//-----------------------------------------------------------------------
// �@�\	: ������
// ����	:
//     _id = �J�������ʔԍ�
// �߂�l	:�@void
//=======================================
void CCamera::Init(unsigned int _id)
{
	m_CameraId = _id;

	// �g�����X�t�H�[��������
	m_Position = Vector3(0.0f, 5.0f, -10.0f);
	m_Rotation = Vector3(0.0f, 0.0f, 0.0f);
	m_PrevRotation = Vector3(0.0f, 0.0f, 0.0f);

	// �r���[�|�[�g������
	m_Viewport.left   = 0;
	m_Viewport.top    = 0;
	m_Viewport.right  = SCREEN_WIDTH;
	m_Viewport.bottom = SCREEN_HEIGHT;

	// Front_Up_Right�x�N�g��������
	m_DirVec.SetFrontUpRight(Vector3(0.0f, 0.0f, 1.0f));

	/* �J������񏉊��� */
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
//	�֐���	: Uninit()
//-----------------------------------------------------------------------
// �@�\	: �I������
// ����	: ����
// �߂�l	:�@void
//=======================================
void CCamera::Uninit()
{


}


//=======================================
//	�֐���	: Update()
//-----------------------------------------------------------------------
// �@�\	: �X�V
// ����	: ����
// �߂�l	:�@void
//=======================================
void CCamera::Update()
{
	// �Ǐ]�ړ�
	if (m_pAtPoint != NULL)
	{
		if (m_BindAtObject) {
			m_At = m_pAtPoint->GetAt() + m_Offset;
		}
	}

	// ���_�̕�����؂�ւ���
	UpdateDirection();

	// �J�����V�F�C�N�i�L�����̂݁j
	Shake();

	// �ʒu�X�V
	Vector3 vFront = m_DirVec.front * m_LengthToAt;
	m_Position = m_At - vFront;

	// �n�ʂƂ̏Փ˂�������΃J�����̈ʒu�X�V
	CollisionTerrian();
}


//=======================================
//	�֐���	: Project()
//-----------------------------------------------------------------------
// �@�\	: ���W�E������E�����_�����Ƀr���[�}�g���N�X����
//           �v���W�F�N�V�����}�g���N�X����
// ����	: ����
// �߂�l	:�@void
//=======================================
void CCamera::Project()
{
	// �r���[�|�[�g�ݒ�
	D3D11_VIEWPORT dxViewport;
	dxViewport.Width    = (float)(m_Viewport.right - m_Viewport.left);
	dxViewport.Height   = (float)(m_Viewport.bottom - m_Viewport.top);
	dxViewport.MinDepth = 0.0f;
	dxViewport.MaxDepth = 1.0f;
	dxViewport.TopLeftX = (float)m_Viewport.left;
	dxViewport.TopLeftY = (float)m_Viewport.top;

	CRenderer::GetDeviceContext()->RSSetViewports(1, &dxViewport);

	// �r���[�}�g���N�X�ݒ�
	XMMATRIX viewMtx;
	viewMtx = XMMatrixLookAtLH(m_Position, m_At, m_DirVec.up);
	XMStoreFloat4x4(&m_ViewMatrix, viewMtx);

	// �r���[�t�s��
	XMStoreFloat4x4(&m_InvViewMatrix, XMMatrixInverse(NULL, viewMtx));

	// �v���W�F�N�V�����}�g���N�X�ݒ�
	XMStoreFloat4x4(&m_ProjectionMatrix ,XMMatrixPerspectiveFovLH(1.0f, dxViewport.Width / dxViewport.Height, 0.1f, 1000.0f));
}


//=======================================
//	�֐���	: GetVisivility()
//-----------------------------------------------------------------------
// �@�\	: �Ώۂ̃��[���h���W��������ɂ��邩�̔���
// ����	:
//     position :: ����Ώۂ̃��[���h���W�|�C���^
// �߂�l	: ������ɓ����Ă�����true
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
//	�֐���	: DrawGUI()
//-----------------------------------------------------------------------
// �@�\	: ImGui�ɂ��f�o�b�O�E�B���h�E�ւ̕`��
// ����	: ����
// �߂�l	: void
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
//	�֐���	: UpdateDirection()
//-----------------------------------------------------------------------
// �@�\	: ��]�p�x�����Ɏ��_��������]������
// ����	: ����
// �߂�l	: void
//=======================================
void CCamera::UpdateDirection()
{
	Vector3X3 prevDir = m_DirVec;

	{// ����]
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

	{// �c��]
		XMMATRIX rotationMtx;
		rotationMtx = XMMatrixRotationAxis(m_DirVec.right, m_SpinVerticall);

		m_DirVec.up = XMVector3TransformNormal(m_DirVec.up, rotationMtx);
		m_DirVec.up = XMVector3Normalize(m_DirVec.up);

		m_DirVec.front = XMVector3TransformNormal(m_DirVec.front, rotationMtx);
		m_DirVec.front = XMVector3Normalize(m_DirVec.front);
	}

	// �I�u�W�F�N�g�ɒǏ]����ꍇ�͊p�x�𐧌�
	if (m_BindAtObject) {
		Vector3 vecY = Vector3(0.0f, 1.0f, 0.0f); // Y���x�N�g��
		float dot = vecY.VDot(m_DirVec.front);
		if (!(dot >= -0.6f && dot <= 0.6f)) {
			m_DirVec = prevDir;
			m_PrevRotation = m_Rotation;
			m_SpinVerticall = 0.0f;
			m_SpinHorizontal = 0.0f;
			return;
		}
	}

	// �J�����̉�]�����X�Ɏ�߂�
	m_SpinVerticall *= 0.92f;
	m_SpinHorizontal *= 0.92f;
}


//=======================================
//	�֐���	: CollisionTerrian()
//-----------------------------------------------------------------------
// �@�\	: �n�`�Ƃ̓����蔻��Ɖ����o��
// ����	: ����
// �߂�l	: �n�`�ƏՓ˂�����true
//=======================================
bool CCamera::CollisionTerrian()
{
	// �n�ʂւ̃A�N�Z�X�|�C���^�Q�b�g
	CTerrain* pTerrain = CManager::GetScene()->GetGameObject<CTerrain>(CManager::LAYER_BACKGROUND);
	if (!pTerrain)return false;

	// �n�ʂƂ̃R���W����
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
//	�֐���	: SetRotation()
//-----------------------------------------------------------------------
// �@�\	: �J�����̌������Z�b�g
// ����	: �p�x�i���W�A���j
// �߂�l	: void
//=======================================
void CCamera::SetRotation(Vector3* rotation)
{
	m_PrevRotation = m_Rotation;
	m_Rotation = *rotation;

	// ��]�ʌv�Z
	m_SpinHorizontal = m_Rotation.y - m_PrevRotation.y;
	m_SpinVerticall    = m_Rotation.x - m_PrevRotation.x;
	m_PrevRotation = m_Rotation;
}


//=======================================
//	�֐���	: Pan()
//-----------------------------------------------------------------------
// �@�\	: �������̉�]
// ����	:
//     _rotate_dir :: ���E����ENUM
// �߂�l	: void
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
//	�֐���	: Tilt()
//-----------------------------------------------------------------------
// �@�\	: �c�����̉�]
// ����	:
//     _rotate_dir :: �㉺����ENUM
// �߂�l	: void
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
//	�֐���	: Move()
//-----------------------------------------------------------------------
// �@�\	: �����I�u�W�F�N�g���������̒����_�ړ�
// ����	:
//     _move_dir :: �ړ�����ENUM
// �߂�l	: void
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
//	�֐���	: SetAt()
//-----------------------------------------------------------------------
// �@�\	: �����_�I�u�W�F�N�g���Z�b�g�iCGameObject*�j
// ����	:
//     pAt :: �o�C���h����I�u�W�F�N�g�̃|�C���^
//     offset :: �I�u�W�F�N�g����̒����_�̃I�t�Z�b�g
// �߂�l	: void
//=======================================
void CCamera::SetAt(CGameObject* pAt, Vector3 offset)
{
	if (pAt) {
		m_pAtPoint = pAt;
		m_Offset = offset;

		// �J�����̏����ʒu
		m_Position = m_pAtPoint->GetAt() + m_Offset - m_DirVec.front;
		m_BindAtObject = true;
	}
	else {
		m_pAtPoint = NULL;

		// �J�����̏����ʒu
		m_Position = Vector3(0.0f, 10.0f, -10.0f);
	}
}


//=======================================
//	�֐���	: Shake()
//-----------------------------------------------------------------------
// �@�\	: �J�����V�F�C�N
// ����	: ����
// �߂�l	: void
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

	// 2F���ɍ��W�ړ�
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
