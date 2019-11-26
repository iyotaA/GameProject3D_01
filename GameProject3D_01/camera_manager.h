#ifndef CAMERA_MANAGER_H_
#define CAMERA_MANAGER_H_

class CCameraManager
{
public:

	// カメラ生成
	static void CreateCamera();

	// カメラ更新
	static void Update();

	// カメラ投影
	static void Project();

	// カメラ破壊
	static void DeleteCamera(const unsigned int _id);

	// 全カメラ破壊
	static void DeleteCamera();

	// カメラゲット
	static CCamera* GetCamera(const unsigned int _id);

private:

	static std::vector<CCamera*> m_pCamera;

	// 指定カメラ移動
	static void Move(const unsigned int _id, Vector3* const _additionalPos);

};

#endif // !CAMERA_MANAGER_H_

