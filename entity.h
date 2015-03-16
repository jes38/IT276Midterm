//   2/24/15 commit

#ifndef _ENTITY_
#define _ENTITY_

#define maxEnts		1024

//create entity structure with common variables for: inuse, positioning
typedef struct ENTITY_T
{
	int inuse;
	int x;
	int y;
	int xVel;
	int yVel;
	int dir;
	int health;
	int type;

	Sprite *sprite;

	void (*think)(struct ENTITY_T *thisEnt);
} Entity;

//misc ents
Uint32 TIME;
int waveInProg;

//list all entities in game
Entity entList[maxEnts];
int numEnts;

//list function used in entity.c
Entity *initEnt(void);
void Free_Ent(Entity *thisEnt);
void freeAllEnts();
Entity *Spawn_Ent(int spawnX, int spawnY, int xVel, int yVel, int dir, Sprite *sprite, int health, int type);
void moveAllEnts();
void Move_Ent(Entity *thisEnt, int xAmnt, int yAmnt);

//specific entity types
void spBloon(int type);
void spBullet(int towerX, int towerY, int dir, int type);
void spTower(int towerX, int towerY, int dir, int type);

//entity think functions
void towerThink(Entity *thatEnt);
void bulletThink(Entity *thatEnt);
void bloonThink(Entity *thatEnt);

//scripting
void startWave(int SpawnRate, int lvl2mix, int numBloons);
int spr;
int l2m;
int nbs;
int bloonsSpawned;
int waitTime;

void update();

#endif
