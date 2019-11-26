#ifndef CAMERA_MANAGER_H_
#define CAMERA_MANAGER_H_

class CCameraManager
{
public:

	// �J��������
	static void CreateCamera();

	// �J�����X�V
	static void Update();

	// �J�������e
	static void Project();

	// �J�����j��
	static void DeleteCamera(const unsigned int _id);

	// �S�J�����j��
	static void DeleteCamera();

	// �J�����Q�b�g
	static CCamera* GetCamera(const unsigned int _id);

private:

	static std::vector<CCamera*> m_pCamera;

	// �w��J�����ړ�
	static void Move(const unsigned int _id, Vector3* const _additionalPos);

};

#endif // !CAMERA_MANAGER_H_

