//   2/17/15 submittion commit

#ifndef _ENTITY_
#define _ENTITY_

#define maxEnts		1024
#define bloonCoordX	1000
#define bloonCoordY 1000

//create entity structure with common variables for: inuse, positioning
typedef struct ENTITY_T
{
	int inuse;
	int x;
	int y;
}Entity;

//list all entities in game
Entity entList[maxEnts];
int numEnts;

//list function used in entity.c
Entity *initEnt(void);
void Free_Ent(Entity *thisEnt);
void Spawn_Ent(int spawnx, int spawny);

//specific entity types
int bloonHealth;
void spBloon(int type);
void spBullet(int towerX, int towerY, int dir);

#endif
