#include "main.h"
#include "game_objects_all.h"
#include "scene.h"
#include "camera_manager.h"
#include "camera.h"

std::vector<CCamera*> CCameraManager::m_pCamera;
unsigned int		  CCameraManager::m_CurrentId;
//CTerrain*			  CCameraManager::m_TerrianAccesser;


// �J�����}�l�[�W���[������
void CCameraManager::Init()
{
	// �n�`�ɃA�N�Z�X���邽�߂̃|�C���^
	//m_TerrianAccesser = CManager::GetScene()->GetGameObject<CTerrain>(CManager::E_Background);
}

// �J��������
void CCameraManager::CreateCamera()
{

	CCamera* _camera = new CCamera();
	_camera->Init(m_pCamera.size());

	m_pCamera.push_back(_camera);

}

// �J�����X�V
void CCameraManager::Update()
{
	Move();
	Rotate();

	for (CCamera* camera : m_pCamera) {

		camera->Update();
	}

}

// �J�������e
void CCameraManager::Project()
{

	m_pCamera[m_CurrentId]->Project();

	DrawGUI();

}

// �S�J�����j��
void CCameraManager::DeleteCamera()
{

	for (CCamera* camera : m_pCamera) {

		camera->Uninit();
	}

	m_pCamera.clear();
}


// �J�����j��
bool CCameraManager::DeleteCamera(const unsigned int _id)
{
	if (_id >= m_pCamera.size())
		return false;

	m_pCamera[_id]->Uninit();
	m_pCamera.erase(m_pCamera.begin() + _id);
	return true;
}


// �w��J�����Q�b�g
CCamera* CCameraManager::GetCamera() {

	return m_pCamera[m_CurrentId];
}


// �w��J�����ύX
bool CCameraManager::SetCurrentCameraId(const unsigned int _id)
{
	if (_id >= m_pCamera.size())
		return false;

	m_CurrentId = _id;
	return true;
}


// �w��J�����ړ�
void CCameraManager::Move()
{
	// �J�������Ǐ]���Ă�����L�[���͂ɂ��ړ��͂��Ȃ�
	if (m_pCamera[m_CurrentId]->GetIsBindAtObject()) return;

	if (CInput::GetKeyPress('W')) {
		m_pCamera[m_CurrentId]->Move(MoveForward);
	}
	if (CInput::GetKeyPress('A')) {
		m_pCamera[m_CurrentId]->Move(MoveLeft);
	}
	if (CInput::GetKeyPress('S')) {
		m_pCamera[m_CurrentId]->Move(MoveBack);
	}
	if (CInput::GetKeyPress('D')) {
		m_pCamera[m_CurrentId]->Move(MoveRight);
	}
	if (CInput::GetKeyPress('Q')) {
		m_pCamera[m_CurrentId]->Move(MoveDown);
	}
	if (CInput::GetKeyPress('E')) {
		m_pCamera[m_CurrentId]->Move(MoveUp);
	}
}


// �w��J������]
void CCameraManager::Rotate()
{

	if (CInput::GetKeyPress(VK_LEFT)) {
		m_pCamera[m_CurrentId]->Pan(RotateLeft);
	}
	if (CInput::GetKeyPress(VK_RIGHT)) {
		m_pCamera[m_CurrentId]->Pan(RotateRight);
	}
	if (CInput::GetKeyPress(VK_UP)) {
		m_pCamera[m_CurrentId]->Tilt(RotateUp);
	}
	if (CInput::GetKeyPress(VK_DOWN)) {
		m_pCamera[m_CurrentId]->Tilt(RotateDown);
	}

}


// �w��J�����ړ��\�͈͓����H
void CCameraManager::CheckIsRange()
{

}


void CCameraManager::DrawGUI()
{
	ImGui::Begin("System");

	ImGuiID Window_Camera_Id = ImGui::GetID("FocusCamera");

	ImGui::BeginChildFrame(Window_Camera_Id, ImVec2(120, 30));

	ImGui::InputInt("Id", (int*)&m_CurrentId);

	ImGui::EndChildFrame();

	ImGui::End();

	if (m_CurrentId >= m_pCamera.size()) {
		m_CurrentId = 0;
	}
	else if (m_CurrentId < 0) {
		m_CurrentId = m_pCamera.size() - 1;
	}
}
