#ifndef __GAMENGINE_H__
#define __GAMENGINE_H__

#include <stdint.h>
#include "../../brandonware/BrandonPhysics.h"
#include "../../brandonware/BrandonTypes.h"
#include "../../brandonware/BrandonMath.h"

enum WeaponTypes {MELEE,HANDGUN,SHOTGUN,RIFLE};
enum WeaponFire {SEMI,AUTO};
enum WeaponState {READY,ATTACK,RELOAD};
typedef struct GE_WeaponData
{
  enum WeaponTypes type;  //Type of weapon
  enum WeaponFire fire;
  int16_t damage;         //How much damage dealt
  fixed32_3 dispersion;   //Degrees of dispersion. For melee, it's the arc it hits
  fixed32_3 range;        //Max distance of weapon (for melee weapons)
  fixed32_3 velocity;     //Projectile velocity
  uint8_t magCap;         //Max ammo in magazine. Ignored for melee
  fixed32_3 fireRate;     //How many milliseconds between shots
  fixed32_3 reloadSpeed;  //How many milliseconds to reload
  uint16_t readySprite;    //Index of ready sprite
  uint16_t fireSprite;     //Index of attack sprite
  uint8_t soundIndex;     //Index of sound effect
  uint8_t name[15];
}weaponData;
typedef struct GE_Weapon
{
  enum WeaponState state; //State of weapon
  uint8_t magCur;         //Current ammo in magazine
  ufixed32_3 fireTimer;   //Timer of attack
  ufixed32_3 reloadTimer; //Timer for reload
  const weaponData* data;        //Data for weapon
}weapon;

typedef struct GE_Player
{
  uint8_t active;
  entity entityData;
  uint8_t activeWeapon;
  weapon weapons[4];
  int16_t health;
  int16_t hunger;
  fixed32_3 speed;
  uint16_t pistolAmmo;
  uint16_t shotgunAmmo;
  uint16_t rifleAmmo;
  uint32_t paces;
  uint32_t kills;
}player;

typedef struct GE_Projectile
{
  uint8_t active;     //1: active; 0: inactive
  entity entityData;
  int16_t damage;
  player* owner;
}projectile;

typedef struct GE_EnemyData
{
  const weaponData* weapondata;
  dimension dim;
  int16_t maxHealth;
  ufixed32_3 speed;
  uint16_t frame1;         //index of frame one
  uint16_t frame2;         //index of frame two
  uint16_t frameAttackl;   //Attack frame
  uint16_t frameDeath;     //Death sprite
}enemyData;

enum CombatAIType {CLOSE,RANGED};
typedef struct GE_EnemyAI
{
  player* target;
  enum CombatAIType AIType;
}enemyAI;

typedef struct GE_Enemy
{
  uint8_t active;
  entity entityData;
  const enemyData* data;
  int16_t health;
  weapon weapon;
  enemyAI ai;
}enemy;

enum ItemTypes {HEALTH,FOOD,AMMO,WEAPON};
typedef struct GE_Item
{
  enum ItemTypes itemType;
  uint8_t active;
  entity entityData;
  uint32_t data1;           //Amount of health/food restored; identifies type of ammo; index of weapon
  uint32_t data2;           //ammount of ammo provided,ammo in magazine
  uint16_t sprite;          //sprite index
}item;

#endif
