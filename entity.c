//   2/24/15 commit

#include <string.h>
#include <stdlib.h>

#include "graphics.h"
#include "sprite.h"
#include "entity.h"

extern SDL_Surface *screen;
extern int ECON;
extern int LIVES;

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

Entity *Spawn_Ent(int spawnX, int spawnY, int xVel, int yVel, int dir, Sprite *sprite, int health, int type)  //spawn new entity at a location
{
	Entity *entPointer = initEnt();
	entPointer -> x = spawnX;
	entPointer -> y = spawnY;
	entPointer -> xVel = xVel;
	entPointer -> yVel = yVel;
	entPointer -> dir = dir;
	entPointer -> sprite = sprite;
	entPointer -> health = health;
	entPointer -> type = type;

	//point in dir direction
	DrawSprite(entPointer->sprite, screen, spawnX, spawnY, 0);
	return entPointer;
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
		Spawn_Ent(700, 10, 0, 1, 0, bloonSprite1, type, type);
	}
	else if (type == 2)
	{
		Spawn_Ent(700, 10, 0, 1, 0, bloonSprite2, type, type);
	}
	else
	{
		fprintf(stdout, "invalid bloon type\n");
	}

	//bloon->think = bloonThink;
}

void spBullet(int towerX, int towerY, int dir, int type)
{
	Entity *bullet;
	Sprite *bSprite = LoadSprite("images/16_16_16_2sprite.png",16,16);
	
	// use dir and vector math to get x and y velocity
	int xVel;
	int yVel;
	if (type == 1){
		xVel = 0;
		yVel = -2;
	}
	if (type == 2){
		xVel = 0;
		yVel = -3;
	}
	Spawn_Ent(towerX, towerY, xVel, yVel, dir, bSprite, 1, 0);

	//bullet->think = bulletThink;
}

void spTower(int towerX, int towerY, int dir, int type)
{
	int cost;
	if(type == 1){cost = 20;}
	if(type == 2){cost = 30;}
	if(type == 3){cost = 30;}
	if(type == 4){cost = 40;}
	
	if(ECON >= cost){	
		Entity *tower;
		Sprite *tSprite = LoadSprite("images/32_32_16_2sprite.png",32,32);
		tower = Spawn_Ent(towerX, towerY, 0, 0, dir, tSprite, -1, type);

		tower->think = towerThink;

		ECON = ECON - cost;
	}
}

//
/*Entity Think functions*/
//

void towerThink(Entity *thatEnt)
{
	int towerX = thatEnt -> x;
	int towerY = thatEnt -> y;
	int dir = thatEnt -> dir;
	int type = thatEnt -> type;
	
	if (type == 1){
		if( (TIME/30) * 30 == TIME){spBullet(towerX, towerY, dir, 1);}
	}
	else if (type == 2){
		if( (TIME/20) * 20 == TIME){spBullet(towerX, towerY, dir, 1);}
	}
	else if (type == 3){
		if( (TIME/30) * 30 == TIME){spBullet(towerX, towerY, dir, 2);}
	}
	else if (type == 4){
		if( (TIME/20) * 20 == TIME){spBullet(towerX, towerY, dir, 2);}
	}
}

void bulletThink(Entity *thatEnt)
{
	int bulletX = thatEnt -> x;
	int bulletY = thatEnt -> y;
	
	if(bulletX < -35 || bulletX > 1059) Free_Ent(thatEnt);
	if(bulletY < -35 || bulletY > 803)	Free_Ent(thatEnt);

	/*
	if (bullet collides with bloon)
	{
		ECON++;
		Free_Ent(thatEnt);
	}
	*/
}

void bloonThink(Entity *thatEnt)
{
	int bloonX = thatEnt -> x;
	int bloonY = thatEnt -> y;

	//script path finding here
	if(bloonX == 700 && bloonY == 350 ) {
		thatEnt->xVel = -1;
		thatEnt->yVel = 0;
	}
	if(bloonX == 300 && bloonY == 350 ) {
		thatEnt->xVel = 0;
		thatEnt->yVel = 1;
	}
	if(bloonX == 300 && bloonY == 750 ) {
		LIVES--;
		Free_Ent(thatEnt);
	}

	/*
	if (bullet collides with bloon)
	{
		Free_Ent(thatEnt);
	}
	*/
}

void startWave(int SpawnRate/*a bigger number is a slower rate, recoomended 15*/, int lvl2mix/*chance out of 10 that a lvl 2 bloon spawns*/, int numBloons)  
{
	spr = SpawnRate;
	l2m = lvl2mix;
	nbs = numBloons;
	bloonsSpawned = 0;
	waitTime = SpawnRate + (rand()%4) - 2;
	
	waveInProg = 1;
}

void update()
{
	int i = 0;
	while (i < maxEnts)
	{
		if(entList[i].inuse == 1 && entList[i].think != NULL)
		{
			(*entList[i].think)(&entList[i]);
		}
		i++;
	}

	TIME++;

	//controls waves
	
	if (waveInProg == 1)
	{
		if (bloonsSpawned < nbs)
		{
			if( (TIME/waitTime) * waitTime == TIME)
			{
				int randNum = rand()%10;
			
				if (randNum <= l2m)
				{
					spBloon(2);
					bloonsSpawned++;
					waitTime = spr + (rand()%4) - 2;
				}
				else
				{
					spBloon(1);
					bloonsSpawned++;
					waitTime = spr + (rand()%4) - 2;
				}
			}
		}
		else{waveInProg = 0;}
	}
	
}
