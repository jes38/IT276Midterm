//   2/24/15 commit

#include <string.h>
#include <stdlib.h>

#include "graphics.h"
#include "sprite.h"
#include "entity.h"

extern SDL_Surface *screen;
extern Uint32 NOW;

Entity *initEnt(void)  //place entity in entList
{
	int i = 0;
	Entity *entPointer;

	while (i < maxEnts)
	{
		if(entList[i].inuse == 0){
			entPointer = &entList[i];
			entPointer -> inuse = 1;
			numEnts++;
			i = maxEnts;
		}
		else if (i == maxEnts){
			exit(1);
		}
		i++;
	}
	return entPointer;
}

void moveAllEnts()  //move all entities
{
	Entity *tempEnt;
	
	int i = 0;
	while (i < maxEnts)
	{
		if(entList[i].inuse == 1)
		{
			tempEnt = &entList[i];
			Move_Ent(tempEnt, tempEnt->xVel, tempEnt->yVel);
		}
		i++;
	}
}

void Move_Ent(Entity *thisEnt, int xAmnt, int yAmnt)  //move a specific entity by a certain ammount
{
	thisEnt -> x += xAmnt;
	thisEnt -> y += yAmnt;
	if(thisEnt->sprite != NULL) DrawSprite(thisEnt->sprite, screen, thisEnt->x, thisEnt->y, 0);
}

void Spawn_Ent(int spawnX, int spawnY, int xVel, int yVel, int dir, Sprite *sprite, int health)  //spawn new entity at a location
{
	Entity *entPointer = initEnt();
	entPointer -> x = spawnX;
	entPointer -> y = spawnY;
	entPointer -> xVel = xVel;
	entPointer -> yVel = yVel;
	entPointer -> dir = dir;
	entPointer -> sprite = sprite;
	entPointer -> health = health;

	//point in dir direction
	DrawSprite(entPointer->sprite, screen, spawnX, spawnY, 0);
}

void Free_Ent(Entity *thisEnt)  //clear entity when no longer used
{
	thisEnt -> inuse = 0;
	FreeSprite(thisEnt->sprite);
}

void freeAllEnts()  //free all entities
{
	int i = 0;
	while (i < maxEnts)
	{
		if(entList[i].inuse == 1)
		{
			Free_Ent(&entList[i]);
		}
		i++;
	}
}

//
/* Spawn specific entities */
//

void spBloon(int type)
{
	Entity *bloon;

	Sprite *bloonSprite1 = LoadSprite("images/16_16_16_2sprite.png",16,16);
	Sprite *bloonSprite2 = LoadSprite("images/32_32_16_2sprite.png",32,32);
	
	if (type == 1)
	{
		Spawn_Ent(bloonCoordX, bloonCoordY, 0, 1, 0, bloonSprite1, type);
	}
	else if (type == 2)
	{
		Spawn_Ent(bloonCoordX, bloonCoordY, 0, 1, 0, bloonSprite2, type);
	}
	else
	{
		fprintf(stdout, "invalid bloon type\n");
	}

	//bloon -> think = bloonThink;
}

void spBullet(int towerX, int towerY, int dir)
{
	Entity *bullet;
	Sprite *bSprite = LoadSprite("images/16_16_16_2sprite.png",16,16);
	
	// use dir and vector math to get x and y velocity
	int xVel = 0;
	int yVel = 0;
	Spawn_Ent(towerX, towerY, xVel, yVel, dir, bSprite, 1);

	//bullet -> think = bulletThink;
}

void spTower(int towerX, int towerY, int dir)
{
	Entity *tower;
	Sprite *tSprite = LoadSprite("images/32_32_16_2sprite.png",32,32);
	Spawn_Ent(towerX, towerY, 1, 0, dir, tSprite, -1);

	//tower -> think = towerThink;
}

//
/*Entity Think functions*/
//

void towerThink(Entity *thatEnt)
{
	int towerX = thatEnt -> x;
	int towerY = thatEnt -> y;
	int dir = thatEnt -> dir;
	
	if( (NOW/10) * 10 == NOW)
	{
		spBullet(towerX, towerY, dir);
	}
}

void bulletThink(Entity *thatEnt)
{
	int bulletX = thatEnt -> x;
	int bulletY = thatEnt -> y;
	
	if(bulletX < -35 || bulletX > 1059) Free_Ent(thatEnt);
	if(bulletY < -35 || bulletY > 803)	Free_Ent(thatEnt);
}

void bloonThink(Entity *thatEnt)
{
	int bloonX = thatEnt -> x;
	int bloonY = thatEnt -> y;

	//script path finding here
	if(bloonX == 500) {
		if(bloonY == 150 ) {
			thatEnt->xVel = 1;
			thatEnt->yVel = 0;
		}
	}

}

void initBloons(int SpawnRate)  //recommended to be at least 5
{
	int waitTime = SpawnRate /*add by a random number between -2 and 2*/;
	if( (NOW/waitTime) * waitTime == NOW)
	{
		spBloon(1);
	}
}
