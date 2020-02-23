#ifndef SCENE_H_
#define SCENE_H_

#include "user_interface_manager.h"
#include "fade_manager.h"
#include "seqence_manager.h"
#include "collision3D.h"
#include "shader_all.h"
#include "debug_primitive.h"
#include "gameObject.h"
#include "player.h"
#include "polygon3d.h"
#include "polygon.h"
#include "skyDome.h"
#include "enemy.h"
#include "texture.h"
#include "number.h"
#include "image.h"
#include "shader_all.h"
#include "terrain.h"

class CScene
{
public:
	CScene() {}
	virtual ~CScene(){}

	// �e���v���[�g�N���X�i�ǉ��j
	template <typename T>
	T* AddGameObject(int layer)
	{
		T* object = new T();
		object->Init();
		m_GameObject[layer].push_back(object);
		return object;
	}

	// �e���v���[�g�N���X�i�w�肵���^��T���j
	template <typename T>
	T* GetGameObject(int layer)
	{
		for (CGameObject* object : m_GameObject[layer]) {

			// typeid()�͌^�𒲂ׂ�
			if (typeid(*object) == typeid(T)) {
				return (T*)object;
			}
		}

		return nullptr;
	}

	// �e���v���[�g�N���X�i�w�肵���^�𕡐��T���j
	template <typename T>
	std::vector<T*> GetGameObjects(int layer)
	{
		std::vector<T*> objects;
		for (CGameObject* object : m_GameObject[layer]) {

			if (typeid(*object) == typeid(T)) {

				objects.push_back((T*)object);
			}
		}

		return objects;
	}

	// �S�ẴQ�[���I�u�W�F�N�g�̏�����
	virtual void Init() = 0;

	// �S�ẴQ�[���I�u�W�F�N�g�̏I������
	virtual void Uninit()
	{
		// UI�j��
		CUserInterfaceManager::DeleteExpectUI(CUserInterfaceManager::LAYER_FOREGROUND);

		// �Q�[���I�u�W�F�N�g�I������
		for (int i = 0; i < CManager::LAYER_MAX; i++) {
			for (CGameObject* object : m_GameObject[i]) {

				object->Uninit();
				delete object;
			}

			// ���X�g�̃N���A
			m_GameObject[i].clear();
		}

		// �J�����j��
		CCameraManager::DeleteCamera();

		// ���͂�L���ɂ��邷��
		CInput::InputEnable(true);
	}

	// �S�ẴQ�[���I�u�W�F�N�g�̍X�V
	virtual void Update()
	{
		for (int i = 0; i < CManager::LAYER_MAX; i++) {
			for (CGameObject* object : m_GameObject[i]) {

				object->Update();
			}

			// ���X�g�폜
			m_GameObject[i].remove_if([](CGameObject* object) {
				return object->Destroy();
			});
		}
	}

	// �S�ẴQ�[���I�u�W�F�N�g�̕`��
	virtual void Draw()
	{
		// �R���W�����O���b�h����
		CDebugPrimitive::DebugPrimitive_BatchBegin();

		for (int i = 0; i < CManager::LAYER_MAX; i++) {

			if (i == CManager::LAYER_EFFECT) {
				// �R���W�����O���b�h�`��
				CDebugPrimitive::DebugPrimitive_BatchRun();
			}

			for (CGameObject* object : m_GameObject[i]) {

				object->Draw();
			}
		}
	}

	virtual void DrawGUI()
	{
		for (int i = 0; i < CManager::LAYER_MAX; i++) {

			for (CGameObject* object : m_GameObject[i]) {

				object->DrawGUI();
			}
		}
	}

	// �Q�[���I�u�W�F�N�g�̓��I�폜
	void DestroyGameObject(CGameObject* GameObject)
	{
		GameObject->SetDestroy();
	}

protected:
	std::list<CGameObject*> m_GameObject[CManager::LAYER_MAX];

};

#endif // !SCENE_H_

