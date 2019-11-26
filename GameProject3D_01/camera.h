#ifndef CAMERA_H_
#define CAMERA_H_

#define CAMERA_MOVE_SPEED (0.05f)

class CCamera
{
public:
	void Init();
	void Uninit();
	void Update();
	void Project();
	void DrawGUI();

	XMFLOAT4X4 GetViewMatrix() { return m_ViewMatrix; }
	XMFLOAT4X4 GetProjectionMatrix() { return m_ProjectionMatrix; }

	Vector3X3* GetDir3Vector() { return &m_DirVec; }
	bool GetVisivility(XMFLOAT3* position);
	bool GetIsBindAtObject() { return m_BindAtObject; }

	void SetAt(CGameObject* pPlayer);
	void SetPos(Vector3* pPos);
	void AddPos(Vector3* pAddPos);

private:

	Vector3 m_Position, m_Rotation;
	CGameObject* m_pAtPoint;
	RECT		 m_Viewport;
	Vector3X3    m_DirVec;
	Vector3      m_At;

	XMFLOAT4X4 m_ViewMatrix;
	XMFLOAT4X4 m_ProjectionMatrix;
	XMFLOAT4X4 m_InvViewMatrix;

	float		m_LengthToAt;
	float		m_SpinVerticall;
	float		m_SpinHorizontal;
	float		m_RotateSpeed;
	float		m_MoveSpeedScale;
	bool		m_BindAtObject;

	bool IsRange();
	void Pan();
	void Tilt();
	void Move();

};

#endif
