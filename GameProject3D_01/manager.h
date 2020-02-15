#ifndef MANAGER_H_
#define MANAGER_H_

class CScene;

class CManager
{
public:
	static 	enum
	{
		LAYER_CAMERA = 0,
		LAYER_BACKGROUND,
		LAYER_OBJECT,
		LAYER_EFFECT,
		LAYER_MAX
	};

	static void Init();
	static void Uninit();
	static void Update();
	static void Draw();

	static CScene* GetScene();

	template <typename T>
	static void SetScene() {
		if(m_Scene != nullptr){
			m_Scene->Uninit();
			delete m_Scene;
		}

		//T* scene = new T();
		//scene->Init();
		m_Scene = new T();
		m_Scene->Init();
	}

	static bool& Clear() { return m_GameClear; }
	static bool& Failed() { return m_GameFailed; }

private:
	static CScene* m_Scene;
	static bool m_GameClear;
	static bool m_GameFailed;

};

#endif
