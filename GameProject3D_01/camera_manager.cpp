#include "stl.h"
#include "game_objects_all.h"
#include "gameObject.h"
#include "camera.h"
#include "camera_manager.h"

std::vector<CCamera*> CCameraManager::m_pCamera;



void CCameraManager::CreateCamera() {

	CCamera* _camera = new CCamera();
	_camera->Init();

	m_pCamera.push_back(_camera);

}

// カメラ更新
void CCameraManager::Update()
{

	for (CCamera* camera : m_pCamera) {

		camera->Update();
	}

}

// カメラ投影
void CCameraManager::Project()
{

	for (CCamera* camera : m_pCamera) {

		camera->Project();
	}

}

// 全カメラ破壊
void CCameraManager::DeleteCamera()
{

	for (CCamera* camera : m_pCamera) {

		camera->Uninit();
	}

	m_pCamera.clear();
}


// カメラ破壊
void CCameraManager::DeleteCamera(const unsigned int _id)
{
	m_pCamera[_id]->Uninit();
	m_pCamera.erase(m_pCamera.begin() + _id);
}



CCamera* CCameraManager::GetCamera(const unsigned int _id) {

	if (_id >= m_pCamera.size())
		return nullptr;

	return m_pCamera[_id];
}


// 指定カメラ移動
void CCameraManager::Move(const unsigned int _id, Vector3* const _additionalPos)
{
	if (_id >= m_pCamera.size())
		return;

	m_pCamera[_id]->AddPos(_additionalPos);
}
