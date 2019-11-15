#ifndef GAMEOBJECT_H_
#define GAMEOBJECT_H_

class CGameObject
{
private:
	bool m_Destroy = false;

protected:
	XMFLOAT3 m_Position;
	XMFLOAT3 m_Rotation;
	XMFLOAT3 m_Scale;

public:
	CGameObject() {}
	virtual ~CGameObject() {}
	virtual void Init() {}
	virtual void Uninit(){}
	virtual void Update(){}
	virtual void Draw(){}

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
};

#endif
