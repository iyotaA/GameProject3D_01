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

	// �J�����}�l�[�W���[������
	static void Init();

	// �J��������
	static void CreateCamera();

	// �J�����X�V
	static void Update();

	// �J�������e
	static void Project();

	// �J�����j��
	static bool DeleteCamera(const unsigned int _id);

	// �S�J�����j��
	static void DeleteCamera();

	// �w��J�����Q�b�g
	static CCamera* GetCamera();

	// �w��J�����ύX
	static bool SetCurrentCameraId(const unsigned int _id);

	// �C���X�^���X���擾
	static CCameraManager& GetInstance() {

		CCameraManager instance;
		return instance;
	}

private:

	// ��::: �ϐ��Q :::�� //
	static std::vector<CCamera*> m_pCamera; // �J�����|�C���^
	static unsigned int m_CurrentId;
	//static CTerrain* m_TerrianAccesser;

	CCameraManager() = default;
	~CCameraManager() = default;

	// �w��J�����ړ�
	static void Move();

	// �w��J������]
	static void Rotate();

	// �w��J�����ړ��\�͈͓����H
	static void CheckIsRange();

	// ImGui
	static void DrawGUI();

};

#endif // !CAMERA_MANAGER_H_

