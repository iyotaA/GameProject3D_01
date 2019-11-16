#ifndef MANAGER_H_
#define MANAGER_H_

class CScene;

class CManager
{
private:
	static CScene* m_Scene;
	static int m_Score;

public:
	static 	enum
	{
		E_Camera = 0,
		E_Background,
		E_3D,
		E_Effect,
		E_UI,
		E_Max
	};

	static void Init();
	static void Uninit();
	static void Update();
	static void Draw();
	static void DrawGUI();

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

	static void AddScore() { m_Score++; }
	static int GetScore() { return m_Score; }
};

#endif
