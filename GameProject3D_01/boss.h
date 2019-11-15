#ifndef BOSS_H_
#define BOSS_H_
#include "collision.h"
#include <d3dx9.h>

#define BOSS_MAX_HP					(5000.0f)
#define BOSS_MAX_SHOT_BULLET		(300)	// àÍâÒÇÃÉ^Å[ÉìÇ≈åÇÇƒÇÈç≈ëÂÇÃêî

enum
{
	BOSS_SHOT_PATERN_1 = 0,
	BOSS_SHOT_PATERN_2,
	BOSS_SHOT_PATERN_3,
	BOSS_SHOT_PATERN_4,
	BOSS_SHOT_PATERN_5,
	BOSS_SHOT_PATERN_6,
	BOSS_SHOT_PATERN_7,
	BOSS_SHOT_PATERN_8,
	BOSS_SHOT_PATERN_MAX,
};

typedef struct {
	D3DXVECTOR2 Position;
	Circle Collision;
	int Width;
	int Height;
	int HP;
	bool bUse;
	bool IsExplosion;
	bool bArrivalPosition;
}Boss;

void Boss_Initialize(void);
void Boss_Finalize(void);
void Boss_Update(void);
void Boss_Draw(void);
void Boss_Create(float x, float y);
void Boss_Destroy(void);
bool Boss_IsEnable(void);
void Boss_SwitchCreate(bool b_switch);
bool Boss_GetSwitch(void);
float Boss_GetDegree(void);
const Circle* Boss_GetCollision(void);
const Circle* Boss_GetOutSideCollision(void);
Boss* Boss_GetBoss(void);
void Boss_DrawHP(void);
void Boss_SetEyeDirection(void);

#endif // !BOSS_H_
