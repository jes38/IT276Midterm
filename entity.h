//   2/24/15 commit

#ifndef _ENTITY_
#define _ENTITY_

#define maxEnts		1024
#define bloonCoordX	500
#define bloonCoordY 10

//create entity structure with common variables for: inuse, positioning
typedef struct ENTITY_T Entity;

struct ENTITY_T
{
	int inuse;
	int x;
	int y;
	int xVel;
	int yVel;
	int dir;
	int health;

	Sprite *sprite;

	void (*think)(Entity *thisEnt);
};

//list all entities in game
Entity entList[maxEnts];
int numEnts;

//list function used in entity.c
Entity *initEnt(void);
void Free_Ent(Entity *thisEnt);
void freeAllEnts();
void Spawn_Ent(int spawnX, int spawnY, int xVel, int yVel, int dir, Sprite *sprite, int health);
void moveAllEnts();
void Move_Ent(Entity *thisEnt, int xAmnt, int yAmnt);

//specific entity types
void spBloon(int type);
void spBullet(int towerX, int towerY, int dir);
void spTower(int towerX, int towerY, int dir);

//entity think functions
void towerThink(Entity *thatEnt);
void bulletThink(Entity *thatEnt);
void bloonThink(Entity *thatEnt);

//scripting
void initBloons(int SpawnRate);

#endif
