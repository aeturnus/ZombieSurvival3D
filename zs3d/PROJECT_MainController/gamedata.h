#ifndef __GAMEDATA_H__
#define __GAMEDATA_H__

#include "GamEngine.h"

#define SND_NULL 0
#define SND_GUNEMPTY 1
#define SND_GUNMAG 2
#define SND_GUNCHAMBER 3
#define SND_KNIFE 4
#define SND_GLOCK 5
#define SND_SHOTGUN 6
#define SND_AR15 7
#define SND_ZOMBIEDIE 8
#define SND_ZOMBIEATTACK 9
#define SND_SKELTAL 10
#define SND_HELO 11

#define CROSS_PISTOL 16
#define CROSS_SHOTGUN 17
#define CROSS_RIFLE 18

//Weapons: type,firemode,damage,dispersion,range,velocity,magCap,fireRate,reloadSpeed,readysprite,firesprite
#define KNIFE_RDY 8
#define KNIFE_ATK 9
const weaponData wpn_knife = {MELEE,SEMI,10,2000,8000,000,0,300,000,KNIFE_RDY,KNIFE_ATK,SND_KNIFE,"Knife"};

#define GLOCK_RDY 10
#define GLOCK_ATK 11
const weaponData wpn_glock17 = {HANDGUN,SEMI,25,5000,000,15000,17,400,1500,GLOCK_RDY,GLOCK_ATK,SND_GLOCK,"Glock 17"};

#define R870_RDY 12
#define R870_ATK 13
const weaponData wpn_r870 = {SHOTGUN,SEMI,15,30000,000,10000,5,500,2000,R870_RDY,R870_ATK,SND_SHOTGUN,"Rem. 870"};

#define AR15_RDY 14
#define AR15_ATK 15
const weaponData wpn_ar15 = {RIFLE,SEMI,50,1000,000,20000,30,300,2500,AR15_RDY,AR15_ATK,SND_AR15,"AR15"};

const weaponData wpn_glock18 = {HANDGUN,AUTO,25,10000,000,10000,33,100,2000,GLOCK_RDY,GLOCK_ATK,SND_GLOCK,"Glock 18"};
const weaponData wpn_r870m = {SHOTGUN,SEMI,15,30000,000,10000,8,500,2000,R870_RDY,R870_ATK,SND_SHOTGUN,"Rem. 870 Mod"};
const weaponData wpn_ar15auto = {RIFLE,AUTO,50,1000,000,20000,30,200,2500,AR15_RDY,AR15_ATK,SND_AR15,"AR15 Auto"};


#define ZOMBIEWPN_RDY 0
#define ZOMBIEWPN_ATK 0
const weaponData wpn_zombie1 = {MELEE,SEMI,10,0000,4000,0,0,1000,0,ZOMBIEWPN_RDY,ZOMBIEWPN_ATK,SND_ZOMBIEATTACK,"Zombie Hand"};
const weaponData wpn_zombie2 = {MELEE,SEMI,20,0000,6000,0,0,1000,0,ZOMBIEWPN_RDY,ZOMBIEWPN_ATK,SND_ZOMBIEATTACK,"Zombie Hand"};
const weaponData wpn_skeltal = {MELEE,SEMI,50,0000,8000,0,0,2000,0,ZOMBIEWPN_RDY,ZOMBIEWPN_ATK,SND_SKELTAL,"Skelton"};


#define WPN_KNIFE 0
#define WPN_GLOCK17 1
#define WPN_R870 2
#define WPN_AR15 3
#define WPN_GLOCK18 4
#define WPN_R870M 5
#define WPN_AR15A 6

const weaponData* weaponPresets[] = {&wpn_knife,&wpn_glock17,&wpn_r870,&wpn_ar15,&wpn_glock18,&wpn_r870m,&wpn_ar15auto};


#define ZOMBIE_W1   0
#define ZOMBIE_W2   1
#define ZOMBIE_ATK  2
#define ZOMBIE_DEATH 3
#define ZOMBIE2 4
#define ZOMBIE2_ATK 5
#define SKELETON 6
#define SKELETON_ATK 7
//weapon, dimension, maxhealth, speed,frame-shit
const enemyData emy_zombie1 = {&wpn_zombie1,{1000,1000,2000},100,500,ZOMBIE_W1,ZOMBIE_W2,ZOMBIE_ATK,ZOMBIE_DEATH};
const enemyData emy_zombie2 = {&wpn_zombie2,{1000,1000,2000},150,750,ZOMBIE2,ZOMBIE2,ZOMBIE2_ATK,ZOMBIE_DEATH};
const enemyData emy_mrskeltal = {&wpn_skeltal,{2000,2000,3000},1000,500,SKELETON,SKELETON,SKELETON_ATK,ZOMBIE_DEATH};

const enemyData* enemyPresets[] = {&emy_zombie1,&emy_zombie2,&emy_mrskeltal};

#endif
