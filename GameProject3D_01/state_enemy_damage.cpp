#include "game_objects_all.h"
#include "state_enemy_idle_stop.h"
#include "state_enemy_damage.h"
#include "enemy.h"

CStateEnemyDamage::CStateEnemyDamage(CEnemy* pEnemy)
{
	// Œø‰Ê‰¹Ä¶
	//CSound::Play(SOUND_LABEL_SE_HIT);
}

CStateEnemyDamage::~CStateEnemyDamage()
{
}

void CStateEnemyDamage::Update(CEnemy* pEnemy)
{

}
