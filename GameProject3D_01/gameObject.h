#ifndef GAMEOBJECT_H_
#define GAMEOBJECT_H_

class CGameObject
{
public:
	// getter
	const Vector3* GetPosition() { return &m_Position; }
	const Vector3* GetRotation() { return &m_Rotation; }
	const Vector3* GetScale() { return &m_Scale; }

	// setter
	void SetPosition(const Vector3* _pos) { m_Position = *_pos; }
	void SetRotation(const Vector3* _rot) { m_Rotation = *_rot; }
	void SetScale(const Vector3* _scl) { m_Scale = *_scl; }

	CGameObject() {}
	virtual ~CGameObject() {}
	virtual void Init() {}
	virtual void Uninit() {}
	virtual void Update() {}
	virtual void Draw() {}

	void SetDestroy() { m_Destroy = true; }
	bool Destroy()
	{
		if (m_Destroy) {
			Uninit();
			delete this;
			return true;
		}
		else {
			return false;
		}
	}

protected:
	Vector3 m_Position;
	Vector3 m_Rotation;
	Vector3 m_Scale;

private:
	bool m_Destroy = false;

};

#endif
