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

	// テンプレートクラス（追加）
	template <typename T>
	T* AddGameObject(int layer)
	{
		T* object = new T();
		object->Init();
		m_GameObject[layer].push_back(object);
		return object;
	}

	// テンプレートクラス（指定した型を探索）
	template <typename T>
	T* GetGameObject(int layer)
	{
		for (CGameObject* object : m_GameObject[layer]) {

			// typeid()は型を調べる
			if (typeid(*object) == typeid(T)) {
				return (T*)object;
			}
		}

		return nullptr;
	}

	// テンプレートクラス（指定した型を複数探索）
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

	// 全てのゲームオブジェクトの初期化
	virtual void Init() = 0;

	// 全てのゲームオブジェクトの終了処理
	virtual void Uninit()
	{
		// UI破壊
		CUserInterfaceManager::DeleteExpectUI(CUserInterfaceManager::LAYER_FOREGROUND);

		// ゲームオブジェクト終了処理
		for (int i = 0; i < CManager::LAYER_MAX; i++) {
			for (CGameObject* object : m_GameObject[i]) {

				object->Uninit();
				delete object;
			}

			// リストのクリア
			m_GameObject[i].clear();
		}

		// カメラ破壊
		CCameraManager::DeleteCamera();

		// 入力を有効にするする
		CInput::InputEnable(true);
	}

	// 全てのゲームオブジェクトの更新
	virtual void Update()
	{
		for (int i = 0; i < CManager::LAYER_MAX; i++) {
			for (CGameObject* object : m_GameObject[i]) {

				object->Update();
			}

			// リスト削除
			m_GameObject[i].remove_if([](CGameObject* object) {
				return object->Destroy();
			});
		}
	}

	// 全てのゲームオブジェクトの描画
	virtual void Draw()
	{
		// コリジョングリッド準備
		CDebugPrimitive::DebugPrimitive_BatchBegin();

		for (int i = 0; i < CManager::LAYER_MAX; i++) {

			if (i == CManager::LAYER_EFFECT) {
				// コリジョングリッド描画
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

	// ゲームオブジェクトの動的削除
	void DestroyGameObject(CGameObject* GameObject)
	{
		GameObject->SetDestroy();
	}

protected:
	std::list<CGameObject*> m_GameObject[CManager::LAYER_MAX];

};

#endif // !SCENE_H_

