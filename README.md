# l4d2_weapons 
The API for SourceMod plugin developers that provides stock of useful weapons function. Basically here the reworked version of **weapons.inc**. The include file was written specifically for [Left 4 Dead 2](https://www.l4d.com/blog/) under [SourceMod](https://www.sourcemod.net/ "SourceMod") platform (SourcePawn).

## Features:
 - Support SourceMod 1.7, 1.11 Transitional Syntax.
 - A lot of weapons/melees functions.
 - Provides weapons/melees data: ID, names, models etc.

## API structure
Since the `weapon_melee` class contains its own melees, the weapons code has been logically divided into two parts: **WEAPON** and **MELEE**. Both parts have functions to check for garbage value and identify their type of weapon. The **ITEM** struct is needed to determine to which type (**WEAPON** / **MELEE**) an entity or weapon name belongs.


## Usages:
Use `L4D2Wep_Identify`, `L4D2Wep_NameToIDEx` for **WEAPON** and `L4D2Wep_IdentifyMelee`, `L4D2Wep_MeleeNameToID` functions for **MELEE** to identify their **ID** by entity or class name. 
If you want the code work for both types (**ITEMS**) use `L4D2Wep_IdentifyItemByName`, `L4D2Wep_IdentifyItemByEnt` functions to identify ([See example](https://github.com/raziEiL/l4d2_weapons/blob/master/scripting/l4d2_weapons_test.sp#L157)). 

When you got an **WEAPON** / **MELEE** / **ITEM** ID you can do following things:
 - [Filter](https://github.com/raziEiL/l4d2_weapons/blob/master/scripting/include/l4d2_weapons.inc#L108) weapon/melee entities.  
 - Grab info about: model, weapon name, weapon slot index, check for spawn class
 - Create any weapons as weapon_ or _spawn class (Fixed pos/angles)
 - Create any melees as weapon_melee or weapon_melee_spawn class (Fixed pos/angles)
 - Give a weapon/melee to player (based on give cmd)
 - Sets a weapon max ammo (based on convars)
 - Sets a weapon max ammo to player (based on offsets)  

## API:
### Weapons:
```
void L4D2Wep_Init()
char[] L4D2Wep_GetNameByID(int wepID)
bool L4D2Wep_HasSpawnClass(int wepID)
char[] L4D2Wep_GetModelByID(int wepID)
int L4D2Wep_GetSlotByID(int wepID)
bool L4D2Wep_IsValidID(int wepID)
bool L4D2Wep_HasValidModel(int wepID)
int L4D2Wep_NameToID(char[] weaponName)
int L4D2Wep_NameToIDEx(char[] weaponName)
int L4D2Wep_Identify(int entity, int flags = IDENTIFY_SAFE)
void L4D2Wep_PrecacheModels()
```
#### Weapons Helpers:
```
void L4D2Wep_AddSpawnSuffix(char[] weaponName, char[] store, int len)
void L4D2Wep_RemoveSpawnSuffix(char[] weaponName, int Len = 0)
int L4D2Wep_HasSpawnSuffix(char[] weaponName)
bool L4D2Wep_IsValidAndEntity(int entity)
bool L4D2Wep_IsEntity(int entity)
```
### Meeles:
```
void L4D2Wep_InitMelees()
char L4D2Wep_GetMeleeNameByID(int meleepID)
char L4D2Wep_GetMeleeModelByID(int meleepID)
bool L4D2Wep_IsValidMeleeID(int meleeID)
int L4D2Wep_MeleeNameToID(char[] meleeName)
int L4D2Wep_IdentifyMelee(int entity, int flags = IDENTIFY_SAFE)
void L4D2Wep_OnMapStart()
bool L4D2Wep_IsValidMelee(char[] meleeName)
bool L4D2Wep_IsValidMeleeIDEx(int meleeID)
void L4D2Wep_PrecacheMeleeModels()
```
### Items:
```
int L4D2Wep_IdentifyItemByEnt(int entity, int &anyID = 0, int flags = IDENTIFY_SAFE)
int L4D2Wep_IdentifyItemByName(char[] anyName, int &anyID = 0)
int L4D2Wep_IdentifyEquipSlot(int entity)
bool L4D2Wep_IsValidItemAndID(int anyID, int itemType)
bool L4D2Wep_IsItemNoneID(int anyID, int itemType)
bool L4D2Wep_IsValidItemID(int anyID, int itemType)
void L4D2Wep_GiveItemByName(int client, char[] weaponName)
void L4D2Wep_GiveItemByID(int client, int anyID, int itemType)
```
### Ammo:
```
void L4D2Wep_InitAmmoCvars()
int L4D2Wep_WepIDToAmmoID(int wepID)
int L4D2Wep_GetAmmo(int ammoID)
bool L4D2Wep_SetAmmoByID(int entity, int wepID)
int L4D2Wep_WepIDToOffset(int wepID)
int L4D2Wep_GetPlayerAmmo(int client)
bool L4D2Wep_SetPlayerAmmo(int client, int maxAmmo)
```
### Spawn:
```
int L4D2Wep_SpawnItem(int anyID, int itemType, float origin[3], float angles[3] = {0.0, ...}, bool applyVecFix = true, bool spawn = true, int count = 5)
int L4D2Wep_Spawn(int wepID, float origin[3], float angles[3] = {0.0, ...}, bool applyVecFix = true, bool spawn = true, int count = 5)
int L4D2Wep_SpawnMelee(int meleeID, float origin[3], float angles[3] = {0.0, ...}, bool applyVecFix = true, bool spawn = true, int count = 5)
int L4D2Wep_ConvertWeaponSpawn(int entity, int wepID, int count = 5, const char[] model = "")
```
### Fixes:
```
void L4D2Wep_FixModelVectors(int wepID, float origin[3], float angles[3])
void L4D2Wep_FixMeleeModelVectors(int meleeID, float origin[3], float angles[3])
```

## Note:
**l4d2_weapons** does not provide a way to unlock weapons, for this goal you may use any known plugins or extensions. However, if you unlock any vanilla weapons like a [knife](https://forums.alliedmods.net/showthread.php?p=1709049) the **l4d2_weapons** will detect it and support.

If you want to add custom weapons support to **l4d2_weapons** look at line [custom melee](https://github.com/raziEiL/l4d2_weapons/blob/master/scripting/include/l4d2_weapons.inc#L555) for example.

## Examples:
**l4d2_weapons_test.sp** plugin provides examples of how this API can be used.

## Credits:
 - Forked from [MatthewClair/sourcemod-plugins](https://github.com/MatthewClair/sourcemod-plugins)
 - Thanks [@Electr0](https://forums.alliedmods.net/member.php?u=152668) for method to detect valid melees via string table.
 
## Donation
If you want to thank us for the hard work feel free to [send any amount](https://www.paypal.me/razicat "send any amount").
