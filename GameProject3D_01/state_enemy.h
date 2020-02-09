#ifndef STATE_ENEMY_H_
#define STATE_ENEMY_H_

class CEnemy;

typedef enum
{
	ENEMY_STATE_ATTACK_HOOK,
	ENEMY_STATE_ATTACK_JUMP,
	ENEMY_STATE_DAMAGE,
	ENEMY_STATE_DIED,
	ENEMY_STATE_IDLE_STOP,
	ENEMY_STATE_IDLE_WATCH_AROUND,
	ENEMY_STATE_RUN,
	ENEMY_STATE_WALK,
	ENEMY_STATE_ATTACK_ATHER,
	ENEMY_STATE_ROTATE,
}ENEMY_STATE;

class CStateEnemy
{
public:
	virtual ~CStateEnemy() {}
	virtual void Update(CEnemy* pEnemy) = 0;
};

class CStateEnemyNone: public CStateEnemy
{
public:
	~CStateEnemyNone() {}
	virtual void Update(CEnemy* pEnemy) override {}
};

#endif // !STATE_ENEMY_H_

