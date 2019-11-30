#ifndef CAMERA_MANAGER_H_
#define CAMERA_MANAGER_H_

class CCamera;

class CCameraManager
{
public:

	typedef enum t_CameraRotate
	{
		RotateLeft,
		RotateRight,
		RotateUp,
		RotateDown,
	}CameraRotate;

	typedef enum t_CameraMove
	{
		MoveLeft,
		MoveRight,
		MoveForward,
		MoveBack,
		MoveUp,
		MoveDown,
	}CameraMove;

	// カメラマネージャー初期化
	static void Init();

	// カメラ生成
	static void CreateCamera();

	// カメラ更新
	static void Update();

	// カメラ投影
	static void Project();

	// カメラ破壊
	static bool DeleteCamera(const unsigned int _id);

	// 全カメラ破壊
	static void DeleteCamera();

	// 指定カメラゲット
	static CCamera* GetCamera();

	// 指定カメラ変更
	static bool SetCurrentCameraId(const unsigned int _id);

	// インスタンスを取得
	static CCameraManager& GetInstance() {

		CCameraManager instance;
		return instance;
	}

private:

	// ＜::: 変数群 :::＞ //
	static std::vector<CCamera*> m_pCamera; // カメラポインタ
	static unsigned int m_CurrentId;
	//static CTerrain* m_TerrianAccesser;

	CCameraManager() = default;
	~CCameraManager() = default;

	// 指定カメラ移動
	static void Move();

	// 指定カメラ回転
	static void Rotate();

	// 指定カメラ移動可能範囲内か？
	static void CheckIsRange();

	// ImGui
	static void DrawGUI();

};

#endif // !CAMERA_MANAGER_H_

