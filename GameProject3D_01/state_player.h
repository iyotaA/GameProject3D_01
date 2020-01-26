#ifndef STATE_PLAYER_H_
#define STATE_PLAYER_H_

class CPlayer;

class CStatePlayer
{
public:
	virtual ~CStatePlayer() {}
	virtual void Update(CPlayer* pPlayer) = 0;
};

class CStatePlayerNone: public CStatePlayer
{
public:
	virtual ~CStatePlayerNone() {}
	virtual void Update(CPlayer* pPlayer) override {}
};

#endif // !STATE_PLAYER_H_

