#ifndef SCENE_H_
#define SCENE_H_

#include "collision3D.h"
#include "stl.h"
#include "debug_primitive.h"
#include "gameObject.h"
#include "model.h"
#include "modelAnimation.h"
#include "player.h"
#include "camera.h"
#include "polygon3d.h"
#include "polygon.h"
#include "field.h"
#include "skyDome.h"
#include "wall.h"
#include "bullet.h"
#include "enemy.h"
#include "texture.h"
#include "2dAnimation.h"
#include "number.h"
#include "score.h"
#include "terrain.h"

class CScene
{
public:

protected:
	std::list<CGameObject*> m_GameObject[CManager::E_Max];

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
		for (int i = 0; i < CManager::E_Max; i++) {
			//		C++11から追加されたfor文の書き方
			//								↓ここがリストであることが条件
			for (CGameObject* object : m_GameObject[i]) {

				object->Uninit();
				delete object;
			}

			// リストのクリア
			m_GameObject[i].clear();
		}
	}

	// 全てのゲームオブジェクトの更新
	virtual void Update()
	{
		for (int i = 0; i < CManager::E_Max; i++) {
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

		for (int i = 0; i < CManager::E_Max; i++) {

			if (i == CManager::E_UI) {
				// コリジョングリッド描画
				CDebugPrimitive::DebugPrimitive_BatchRun();
			}

			for (CGameObject* object : m_GameObject[i]) {

				object->Draw();
			}
		}
	}

	// ゲームオブジェクトの動的削除
	void DestroyGameObject(CGameObject* GameObject)
	{
		GameObject->SetDestroy();
	}
};

#endif // !SCENE_H_

