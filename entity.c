//   2/24/15 commit

#include <string.h>
#include <stdlib.h>

#include "graphics.h"
#include "sprite.h"
#include "entity.h"

extern SDL_Surface *screen;
extern int ECON;
extern int LIVES;
extern int LEVEL;
extern int ROTATION;

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

void Move_Ent(Entity *thisEnt, int xAmnt, int yAmnt)  //move a specific entity by a certain ammount
{
	int offX;
	int offY;

	thisEnt -> x += xAmnt;
	thisEnt -> y += yAmnt;

	offX = thisEnt->x - thisEnt->size;
	offY = thisEnt->y - thisEnt->size;
	if(thisEnt->sprite != NULL) DrawSprite(thisEnt->sprite, screen, offX, offY, 0);
}

Entity *Spawn_Ent(int spawnX, int spawnY, int xVel, int yVel, int dir, Sprite *sprite, int health, int type)  //spawn new entity at a location
{
	Entity *entPointer = initEnt();
	int offX;
	int offY;

	entPointer -> x = spawnX;
	entPointer -> y = spawnY;
	entPointer -> xVel = xVel;
	entPointer -> yVel = yVel;
	entPointer -> dir = dir;
	entPointer -> sprite = sprite;
	entPointer -> health = health;
	entPointer -> type = type;
	entPointer -> colliding = 0;
	if (type==1 || type==2 || type ==3 || type==4){entPointer -> size = 16;}
	else {entPointer -> size = 8;}
	offX = spawnX - entPointer->size;
	offY = spawnY - entPointer->size;

	//point in dir direction
	DrawSprite(entPointer->sprite, screen, offX, offY, 0);
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
		bloon = Spawn_Ent(700, 10, 0, 1, 0, bloonSprite1, type, 50);
		bloon->think = bloonThink;
	}
	else if (type == 2)
	{
		bloon = Spawn_Ent(700, 10, 0, 1, 0, bloonSprite2, type, 50);
		bloon->think = bloonThink;
	}
	else
	{
		fprintf(stdout, "invalid bloon type\n");
	}

	
}

void spBullet(int towerX, int towerY, int dir, int type)
{
	Entity *bullet;
	Sprite *bSprite = LoadSprite("images/16_16_16_2sprite.png",16,16);
	
	// use dir and vector math to get x and y velocity
	int xVel;
	int yVel;
	if (type == 1){
		if (dir == 0){xVel = 0; yVel = -2;}
		else if (dir == 45){xVel = 1; yVel = -1;}
		else if (dir == 90){xVel = 2; yVel = 0;}
		else if (dir == 135){xVel = 1; yVel = 1;}
		else if (dir == 180){xVel = 0; yVel = 2;}
		else if (dir == 225){xVel = -1; yVel = 1;}
		else if (dir == 270){xVel = -2; yVel = 0;}
		else if (dir == 315){xVel = -1; yVel = -1;}
	}
	if (type == 2){
		xVel = 0;
		yVel = -3;
	}
	Spawn_Ent(towerX, towerY, xVel, yVel, dir, bSprite, 1, 40);
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
		dir = ROTATION;

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

//main update function
void update()
{
	int i = 0;
	while (i < maxEnts)
	{
		//do think function
		if(entList[i].inuse == 1 && entList[i].think != NULL)
		{
			(*entList[i].think)(&entList[i]);
		}

		//scrolls through entities in use
		if(entList[i].inuse == 1)
		{
			Entity *tempEnt;
			int X;
			int Y;

			tempEnt = &entList[i];
			X = tempEnt -> x;
			Y = tempEnt -> y;

			//detect if bullet collides with bloon
			if (tempEnt->type == 40) //if the entity is a bullet
			{
				int q = 0;
				while (q < maxEnts)
				{
					if(entList[q].inuse==1 && entList[q].type==50) //if the enemy is a bloon
					{
						Entity *enemy;
						int Xdist;
						int Ydist;
						int tempDist;
						enemy = &entList[q];

						Xdist = (tempEnt->x) - (enemy->x);
						Ydist = (tempEnt->y) - (enemy->y);
						if (Xdist < 0){Xdist = Xdist * -1;}
						if (Ydist < 0){Ydist = Ydist * -1;}

						tempDist = (Xdist * Xdist) + (Ydist * Ydist);
						if (tempDist <= 256){tempEnt->colliding = 1; enemy->colliding = 1;}
					}
					q++;
				}
			}

			Move_Ent(tempEnt, tempEnt->xVel, tempEnt->yVel); //moves all entities

			//upon colliding, free entity and update economy
			if (tempEnt->colliding == 1)
			{
				if (tempEnt->type == 50) {ECON++;}
				Free_Ent(tempEnt);
			}

			//out of bounds detection
			if(X < -35 || X > 1059) Free_Ent(tempEnt);
			if(Y < -35 || Y > 803)	Free_Ent(tempEnt);
		}
		i++;
	}

	TIME++; //level time

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
		else{waveInProg = 0; LEVEL++;}
	}
	
}
