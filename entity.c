//   2/17/15 submittion commit

#include <string.h>
#include <stdlib.h>

#include "graphics.h"
#include "entity.h"

//fill entList
Entity *initEnt(void)
{
	int i = 0;
	Entity *entPointer;

	while (i < maxEnts)
	{
		if(entList[i].inuse == 0){
			entPointer = &entList[i];
			entPointer -> inuse = 1;
		}
		else if (numEnts < maxEnts){
			entPointer = &entList[numEnts++];
			entPointer -> inuse = 1;
		}
		else if (i == maxEnts){
			exit(1);
		}
		i++;
	}
	return entPointer;
}

//spawn new entity at a location
void Spawn_Ent(int spawnx, int spawny)
{
	Entity *entPointer = initEnt();
	entPointer -> x = spawnx;
	entPointer -> y = spawny;
}

//clear entity when no longer used
void Free_Ent(Entity *thisEnt)
{
	thisEnt -> inuse = 0;
}

//
/* Spawn specific entities */
//

//spawn a bloon
void spBloon(int type)
{
	Spawn_Ent(bloonCoordX, bloonCoordY);
	
	bloonHealth = type;
	if (type == 1)
	{
		//use sprite for easy bloon
	}
	else if (type == 2)
	{
		//use sprite for medium bloon
	}
	else
	{
		fprintf(stdout, "invalid bloon type\n");
	}
}

//spawn a bullet
void spBullet(int towerX, int towerY, int dir)
{
	Spawn_Ent(towerX, towerY);
	//point bullet to direction
	//use sprite for Bullet
}
