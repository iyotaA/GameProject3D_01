#ifndef CAMERA_H_
#define CAMERA_H_

#define CAMERA_MOVE_SPEED (0.05f)

class CCamera
{
public:
	void Init(unsigned int _id);
	void Uninit();
	void Update();
	void Project();
	void DrawGUI();
	const Vector3 GetAt() { return m_Position; }

	XMFLOAT4X4 GetViewMatrix() { return m_ViewMatrix; }
	XMFLOAT4X4 GetProjectionMatrix() { return m_ProjectionMatrix; }

	unsigned int GetId() { return m_CameraId; }
	Vector3X3* const GetDir3Vector() { return &m_DirVec; }
	Vector3 GetPosition() const { return m_Position; }
	float GetRotateSpeed() const { return m_RotateSpeed; }
	float GetLengthToAt() const { return m_LengthToAt; }
	bool GetIsBindAtObject() const { return m_BindAtObject; }
	bool GetVisivility(XMFLOAT3* position) const;

	void SetAt(CGameObject* pPlayer, Vector3 offset);
	void SetPos(Vector3* pPos);
	void SetAtPos(Vector3* pPos);
	void SetLengthToAt(float length) { m_LengthToAt = length; }
	void SetShake() { m_Shake = true; }

	void SetRotation(Vector3* rotation);
	void AddPos(Vector3* pAddPos);
	void Pan(CCameraManager::CameraRotate _rotate_dir);
	void Tilt(CCameraManager::CameraRotate _rotate_dir);
	void Move(CCameraManager::CameraMove _move_dir);

private:

	CGameObject* m_pAtPoint;
	RECT				m_Viewport;
	Vector3			m_Position;
	Vector3			m_Rotation;
	Vector3			m_PrevRotation;
	Vector3X3		m_DirVec;
	Vector3			m_At;
	Vector3			m_Offset;

	XMFLOAT4X4 m_ViewMatrix;
	XMFLOAT4X4 m_ProjectionMatrix;
	XMFLOAT4X4 m_InvViewMatrix;

	unsigned int m_CameraId;
	int			m_FrameCounter;
	float		m_LengthToAt;
	float		m_SpinVerticall;
	float		m_SpinHorizontal;
	float		m_RotateSpeed;
	float		m_MoveSpeedScale;
	bool		m_BindAtObject;
	bool		m_Shake;

	void IsRange();
	bool CollisionTerrian();
	void Shake();

};

#endif
