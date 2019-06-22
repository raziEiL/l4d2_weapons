/**
 * vim: set ts=4 :
 * =============================================================================
 * l4d2_weapons
 * Provides a set of functions for weapon/melee identification and manipulation.
 * GitHub: https://github.com/raziEiL/l4d2_weapons
 *
 * l4d2_weapons (C)2019 raziEiL [disawar1] <mr.raz4291@gmail.com>
 * SourceMod (C)2004-2008 AlliedModders LLC.  All rights reserved.
 * =============================================================================
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License, version 3.0, as published by the
 * Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * As a special exception, AlliedModders LLC gives you permission to link the
 * code of this program (as well as its derivative works) to "Half-Life 2," the
 * "Source Engine," the "SourcePawn JIT," and any Game MODs that run on software
 * by the Valve Corporation.  You must obey the GNU General Public License in
 * all respects for all other code used.  Additionally, AlliedModders LLC grants
 * this exception to all derivative works.  AlliedModders LLC defines further
 * exceptions, found in LICENSE.txt (as of this writing, version JULY-31-2007),
 * or <http://www.sourcemod.net/license.php>.
 */

#include <sourcemod>
#include <sdktools>

#if defined __l4d2_weapons_inc__
#endinput
#endif
#define __l4d2_weapons_inc__

#define _NAME_LENGTH 64

// ------------------------------WEAPON FUNCTION------------------------------------

// Weapon ID enumerations.
// These values are *NOT* arbitrary!
// They are used in game as the weaponid for weapon_spawn entities
enum ()
{
	WEPID_NONE,             // 0
	WEPID_PISTOL,           // 1
	WEPID_SMG,              // 2
	WEPID_PUMPSHOTGUN,      // 3
	WEPID_AUTOSHOTGUN,      // 4
	WEPID_RIFLE,            // 5
	WEPID_HUNTING_RIFLE,    // 6
	WEPID_SMG_SILENCED,     // 7
	WEPID_SHOTGUN_CHROME,   // 8
	WEPID_RIFLE_DESERT,     // 9
	WEPID_SNIPER_MILITARY,  // 10
	WEPID_SHOTGUN_SPAS,     // 11
	WEPID_FIRST_AID_KIT,    // 12
	WEPID_MOLOTOV,          // 13
	WEPID_PIPE_BOMB,        // 14
	WEPID_PAIN_PILLS,       // 15
	WEPID_GASCAN,           // 16
	WEPID_PROPANE_TANK,     // 17
	WEPID_OXYGEN_TANK,      // 18
	WEPID_MELEE,            // 19
	WEPID_CHAINSAW,         // 20
	WEPID_GRENADE_LAUNCHER, // 21
	WEPID_AMMO_PACK,        // 22
	WEPID_ADRENALINE,       // 23
	WEPID_DEFIBRILLATOR,    // 24
	WEPID_VOMITJAR,         // 25
	WEPID_RIFLE_AK47,       // 26
	WEPID_GNOME_CHOMPSKI,   // 27
	WEPID_COLA_BOTTLES,     // 28
	WEPID_FIREWORKS_BOX,    // 29
	WEPID_INCENDIARY_AMMO,  // 30
	WEPID_FRAG_AMMO,        // 31
	WEPID_PISTOL_MAGNUM,    // 32
	WEPID_SMG_MP5,          // 33
	WEPID_RIFLE_SG552,      // 34
	WEPID_SNIPER_AWP,       // 35
	WEPID_SNIPER_SCOUT,     // 36
	WEPID_RIFLE_M60,        // 37
	WEPID_TANK_CLAW,        // 38
	WEPID_HUNTER_CLAW,      // 39
	WEPID_CHARGER_CLAW,     // 40
	WEPID_BOOMER_CLAW,      // 41
	WEPID_SMOKER_CLAW,      // 42
	WEPID_SPITTER_CLAW,     // 43
	WEPID_JOCKEY_CLAW,      // 44
	WEPID_MACHINEGUN,       // 45
	WEPID_FATAL_VOMIT,      // 46
	WEPID_EXPLODING_SPLAT,  // 47
	WEPID_LUNGE_POUNCE,     // 48
	WEPID_LOUNGE,           // 49
	WEPID_FULLPULL,         // 50
	WEPID_CHOKE,            // 51
	WEPID_THROWING_ROCK,    // 52
	WEPID_TURBO_PHYSICS,    // 53 what is this
	WEPID_AMMO,             // 54
	WEPID_UPGRADE_ITEM,      // 55
	WEPID_SIZE
};

static const bool l_bHasSpawnClass[WEPID_SIZE] =
{
	false, true, true,		// 0
	true, true, true,		// 3
	true, true, true,		// 6
	true, true, true,		// 9
	true, true, true,		// 12
	true, true, false,		// 15
	false, true, true,		// 18
	true, false, true,		// 21
	true, true, true,		// 24
	false, false, false, 	// 27
	true, true, true,		// 30
	false, false, false,	// 33
	false, true, false,		// 36
	false, false, false,	// 39
	false, false, false,	// 42
	false, false, false,	// 45
	false, false, false,	// 48
	false, false, false,	// 51
	false, false			// 54
};

// Weapon names for each of the weapons, used in identification.
static const char l_sWeaponNames[WEPID_SIZE][_NAME_LENGTH] =
{
	"weapon_none", "weapon_pistol", "weapon_smg",                                            // 0
	"weapon_pumpshotgun", "weapon_autoshotgun", "weapon_rifle",                              // 3
	"weapon_hunting_rifle", "weapon_smg_silenced", "weapon_shotgun_chrome",                  // 6
	"weapon_rifle_desert", "weapon_sniper_military", "weapon_shotgun_spas",                  // 9
	"weapon_first_aid_kit", "weapon_molotov", "weapon_pipe_bomb",                            // 12
	"weapon_pain_pills", "weapon_gascan", "weapon_propanetank",                              // 15
	"weapon_oxygentank", "weapon_melee", "weapon_chainsaw",                                  // 18
	"weapon_grenade_launcher", "weapon_ammo_pack", "weapon_adrenaline",                      // 21
	"weapon_defibrillator", "weapon_vomitjar", "weapon_rifle_ak47",                          // 24
	"weapon_gnome", "weapon_cola_bottles", "weapon_fireworkcrate",                           // 27
	"weapon_upgradepack_incendiary", "weapon_upgradepack_explosive", "weapon_pistol_magnum", // 30
	"weapon_smg_mp5", "weapon_rifle_sg552", "weapon_sniper_awp",                             // 33
	"weapon_sniper_scout", "weapon_rifle_m60", "weapon_tank_claw",                           // 36
	"weapon_hunter_claw", "weapon_charger_claw", "weapon_boomer_claw",                       // 39
	"weapon_smoker_claw", "weapon_spitter_claw", "weapon_jockey_claw",                       // 42
	"weapon_machinegun", "vomit", "splat",                                                   // 45
	"pounce", "lounge", "pull",                                                              // 48
	"choke", "rock", "physics",                                                              // 51
	"ammo", "upgrade_item"                                                                   // 54
};

// World Weapon models for each of the weapons. Useful for making new weapon spawns
// Some models are left blank because no single model can be given, the model is known or none exist.
static const char l_sWeaponModels[WEPID_SIZE][PLATFORM_MAX_PATH] =
{
	"",
	"models/w_models/weapons/w_pistol_B.mdl",
	"models/w_models/weapons/w_smg_uzi.mdl",
	"models/w_models/weapons/w_shotgun.mdl",
	"models/w_models/weapons/w_autoshot_m4super.mdl",
	"models/w_models/weapons/w_rifle_m16a2.mdl",
	"models/w_models/weapons/w_sniper_mini14.mdl",
	"models/w_models/weapons/w_smg_a.mdl",
	"models/w_models/weapons/w_pumpshotgun_a.mdl",
	"models/w_models/weapons/w_desert_rifle.mdl",           // "models/w_models/weapons/w_rifle_b.mdl"
	"models/w_models/weapons/w_sniper_military.mdl",
	"models/w_models/weapons/w_shotgun_spas.mdl",			// 9
	"models/w_models/weapons/w_eq_medkit.mdl",
	"models/w_models/weapons/w_eq_molotov.mdl",
	"models/w_models/weapons/w_eq_pipebomb.mdl",
	"models/w_models/weapons/w_eq_painpills.mdl",
	"models/props_junk/gascan001a.mdl",
	"models/props_junk/propanecanister001a.mdl",
	"models/props_equipment/oxygentank01.mdl",
	"",
	"models/weapons/melee/w_chainsaw.mdl",					// 18
	"models/w_models/weapons/w_grenade_launcher.mdl",
	"",
	"models/w_models/weapons/w_eq_adrenaline.mdl",
	"models/w_models/weapons/w_eq_defibrillator.mdl",
	"models/w_models/weapons/w_eq_bile_flask.mdl",
	"models/w_models/weapons/w_rifle_ak47.mdl",
	"models/props_junk/gnome.mdl",
	"models/w_models/weapons/w_cola.mdl",
	"models/props_junk/explosive_box001.mdl",
	"models/w_models/weapons/w_eq_incendiary_ammopack.mdl",
	"models/w_models/weapons/w_eq_explosive_ammopack.mdl",
	"models/w_models/weapons/w_desert_eagle.mdl",
	"models/w_models/weapons/w_smg_mp5.mdl",
	"models/w_models/weapons/w_rifle_sg552.mdl",
	"models/w_models/weapons/w_sniper_awp.mdl",
	"models/w_models/weapons/w_sniper_scout.mdl",
	"models/w_models/weapons/w_m60.mdl",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	""
};

static const int l_iWeaponSlots[WEPID_SIZE] =
{
	-1, // WEPID_NONE
	1,  // WEPID_PISTOL
	0,  // WEPID_SMG
	0,  // WEPID_PUMPSHOTGUN
	0,  // WEPID_AUTOSHOTGUN
	0,  // WEPID_RIFLE
	0,  // WEPID_HUNTING_RIFLE
	0,  // WEPID_SMG_SILENCED
	0,  // WEPID_SHOTGUN_CHROME
	0,  // WEPID_RIFLE_DESERT
	0,  // WEPID_SNIPER_MILITARY
	0,  // WEPID_SHOTGUN_SPAS
	3,  // WEPID_FIRST_AID_KIT
	2,  // WEPID_MOLOTOV
	2,  // WEPID_PIPE_BOMB
	4,  // WEPID_PAIN_PILLS
	-1, // WEPID_GASCAN
	-1, // WEPID_PROPANE_TANK
	-1, // WEPID_OXYGEN_TANK
	1,  // WEPID_MELEE
	1,  // WEPID_CHAINSAW
	0,  // WEPID_GRENADE_LAUNCHER
	3,  // WEPID_AMMO_PACK
	4,  // WEPID_ADRENALINE
	3,  // WEPID_DEFIBRILLATOR
	2,  // WEPID_VOMITJAR
	0,  // WEPID_RIFLE_AK47
	-1, // WEPID_GNOME_CHOMPSKI
	-1, // WEPID_COLA_BOTTLES
	-1, // WEPID_FIREWORKS_BOX
	3,  // WEPID_INCENDIARY_AMMO
	3,  // WEPID_FRAG_AMMO
	1,  // WEPID_PISTOL_MAGNUM
	0,  // WEPID_SMG_MP5
	0,  // WEPID_RIFLE_SG552
	0,  // WEPID_SNIPER_AWP
	0,  // WEPID_SNIPER_SCOUT
	0,  // WEPID_RIFLE_M60
	-1, // WEPID_TANK_CLAW
	-1, // WEPID_HUNTER_CLAW
	-1, // WEPID_CHARGER_CLAW
	-1, // WEPID_BOOMER_CLAW
	-1, // WEPID_SMOKER_CLAW
	-1, // WEPID_SPITTER_CLAW
	-1, // WEPID_JOCKEY_CLAW
	-1, // WEPID_MACHINEGUN
	-1, // WEPID_FATAL_VOMIT
	-1, // WEPID_EXPLODING_SPLAT
	-1, // WEPID_LUNGE_POUNCE
	-1, // WEPID_LOUNGE
	-1, // WEPID_FULLPULL
	-1, // WEPID_CHOKE
	-1, // WEPID_THROWING_ROCK
	-1, // WEPID_TURBO_PHYSICS
	-1, // WEPID_AMMO
	-1  // WEPID_UPGRADE_ITEM
};

stock static StringMap mWeaponNames;

/**
 * Initializes internal structure necessary for weapons.inc functions
 * @remark It is recommended that you run this function on plugin start, but not necessary
 *
 * @noreturn
 */
stock void L4D2Weapons_Init()
{
	if(mWeaponNames == null)
	{
		InitWeaponNamesMap();
	}
}

stock static void InitWeaponNamesMap()
{
	mWeaponNames = new StringMap();
	for(int i; i < WEPID_SIZE; i++)
	{
		mWeaponNames.SetValue(l_sWeaponNames[i], i);
	}
}

stock char[] GetWepNameByID(int wepID)
{
	return l_sWeaponNames[wepID];
}

stock bool GetWepHasSpawnClassByID(int wepID)
{
	return l_bHasSpawnClass[wepID];
}

stock char[] GetWepModelByID(int wepID)
{
	return l_sWeaponModels[wepID];
}

stock int GetWepSlotByID(int wepID)
{
	return l_iWeaponSlots[wepID];
}

/**
 * Performs bounds checking to determine if a WEPID is valid
 * @remark Simple check to see if wepID has a garbage value
 *
 * @param wepID		WEPID to check for validity
 * @return True if wepID is valid, false otherwise.
 */
stock bool IsValidWeaponID(int wepID)
{
	return wepID >= WEPID_NONE && wepID < WEPID_SIZE;
}

/**
 * Checks to see if a given wepID has a known WeaponModel in this file's model array
 * @remark Some weapons (like weapon_melee) have multiple valid models, and this will report false for them.
 *
 * @param wepID		WEPID to check for a known weapon model for.
 * @return True if a valid weapon model exists for WEPID, false otherwise.
 */
stock bool HasValidWeaponModel(int wepID)
{
	return l_sWeaponModels[wepID][0];
}

/**
 * Tries to look up a WEPID for a given Weapon Name.
 *
 * @param weaponName	Weapon name string to look up Id from
 * @return				The corresponding WeaponId if found, else WEPID_NONE
 */
stock int WeaponNameToID(char[] weaponName)
{
	if(mWeaponNames == null)
	{
		InitWeaponNamesMap();
	}
	int iID;
	if(mWeaponNames.GetValue(weaponName, iID))
	{
		return iID;
	}
	return WEPID_NONE;
}

// This is the same as WeaponNameToID(), except it check and removes '_spawn' suffix
stock int WeaponNameToIDEx(char[] weaponName)
{
	int len = HasSpawnSuffix(weaponName);
	if (len){
		char[] sTemp = new char[len];
		strcopy(sTemp, len, weaponName);
		RemoveSpawnSuffix(sTemp, len);
		return WeaponNameToID(sTemp);
	}
	return WeaponNameToID(weaponName);
}

/**
 * Identifies a weapon spawn or weapon entity as a WEPID
 * @remark Should work on most weapon ents--even spawns, singles, held, etc.
 *
 * @param entity		Index of entity to identify
 * @return WEPID for the entity if it is a weapon, WEPID_NONE otherwise
 */
stock int IdentifyWeapon(int entity)
{
	if(entity < MAXPLAYERS+1 || !IsValidEntity(entity))
		return WEPID_NONE;

	char sClass[64];
	if (!GetEntityClassname(entity, sClass, sizeof(sClass)))
		return WEPID_NONE;

	if (HasEntProp(entity, Prop_Send, "m_weaponID"))
		return GetEntProp(entity, Prop_Send, "m_weaponID");

	return WeaponNameToIDEx(sClass);
}

// returns an entity of the new weapon spawn on success
// -1 on failure or removal
/**
 * Attempts to convert a weapon spawn entity to a given weapon spawn
 * @remark Truthfully, this will work on any entity with origin/rotation.
 *		Also, requires the weapon to either have a Valid weapon model or have one provided
 *
 * @param entity		Index of entity to convert to weapon spawn
 * @param wepID			WeaponId of the weapon to have the spawner hold
 * @param count			Weapon count for the spawner (default 5)
 * @param model			World model to use for the weapon spawn
 * @return entity of the new weapon spawn, or -1 on errors.
 */
stock int ConvertWeaponSpawn(int entity, int wepID, int count = 5, const char[] model = "")
{
	if(!IsValidEntity(entity)) return -1;
	if(!IsValidWeaponId(wepID)) return -1;
	if(model[0] == '\0' && !HasValidWeaponModel(wepID)) return -1;


	float origins[3], angles[3];
	GetEntPropVector(entity, Prop_Send, "m_vecOrigin", origins);
	GetEntPropVector(entity, Prop_Send, "m_angRotation", angles);

	AcceptEntityInput(entity, "kill");

	entity = CreateEntityByName("weapon_spawn");
	if(!IsValidEntity(entity)) return -1;

	SetEntProp(entity, Prop_Send, "m_weaponID", wepID);

	if(model[0] == '\0')
	{
		if(!IsModelPrecached(model))
		{
			if(PrecacheModel(model) <= 0)
			{
				return -1;
			}
		}

		SetEntityModel(entity, model);
	}
	else
	{
		if(!IsModelPrecached(l_sWeaponModels[wepID]))
		{
			if(PrecacheModel(l_sWeaponModels[wepID]) <= 0)
			{
				return -1;
			}
		}

		SetEntityModel(entity, l_sWeaponModels[wepID]);
	}
	char sCount[64];
	IntToString(count, sCount, sizeof(sCount));
	DispatchKeyValue(entity, "count", sCount);

	TeleportEntity(entity, origins, angles, NULL_VECTOR);
	DispatchSpawn(entity);
	SetEntityMoveType(entity,MOVETYPE_NONE);
	return entity;
}

// Precache weapon world models
stock void PrecacheWepModels()
{
	for(int i = WEPID_PISTOL; i < WEPID_TANK_CLAW; i++)
	{
		if (HasValidWeaponModel(i) && !IsModelPrecached(l_sWeaponModels[i]))
			PrecacheModel(l_sWeaponModels[i], true);
	}
}

// Suffix helpers. Do this: "weapon_rifle" -> "weapon_rifle_spawn"
stock void AddSpawnSuffix(char[] weaponName, char[] store, int len)
{
	if (!HasSpawnSuffix(weaponName))
		FormatEx(store, len, "%s_spawn", weaponName);
}

// Do this: "weapon_rifle_spawn" -> "weapon_rifle"
stock void RemoveSpawnSuffix(char[] weaponName, int Len = 0)
{
	if (Len)
		weaponName[Len-6]='\0';
	else {
		int len = HasSpawnSuffix(weaponName);
		if (len)
			weaponName[len-6]='\0';
	}
}

// Check for "_spawn" suffix
stock int HasSpawnSuffix(char[] weaponName)
{
	int len = strlen(weaponName);
	if (len-6 > 0 && StrEqual(weaponName[len-6], "_spawn"))
		return len;
	return 0;
}

// ------------------------------MELEE FUNCTION------------------------------------

enum ()
{
	MELEEID_NONE = -1,
	MELEEID_FIREAXE,
	MELEEID_BASEBALL_BAT,
	MELEEID_CRICKET_BAT,
	MELEEID_CROWBAR,
	MELEEID_FRYING_PAN,
	MELEEID_GOLFCLUB,
	MELEEID_ELECTRIC_GUITAR,
	MELEEID_KATANA,
	MELEEID_MACHETE,
	MELEEID_TONFA,
	MELEEID_KNIFE,
	// custom melee
	MELEEID_ALLIANCE_SHIELD,
	MELEEID_FUBAR,
	MELEEID_NAIL_BOARD,
	MELEEID_SLEDGEHAMMER,
	// size
	MELEEID_SIZE
}

static const char l_sMeleeNames[MELEEID_SIZE][_NAME_LENGTH] =
{
	"fireaxe",
	"baseball_bat",
	"cricket_bat",
	"crowbar",
	"frying_pan",
	"golfclub",
	"electric_guitar",
	"katana",
	"machete",
	"tonfa",
	"knife",
	// custom melee
	"alliance_shield",
	"fubar",
	"nail_board",
	"sledgehammer"
};

static const char l_sMeleeModels[MELEEID_SIZE][PLATFORM_MAX_PATH] =
{
	"models/weapons/melee/w_fireaxe.mdl",
	"models/weapons/melee/w_bat.mdl",
	"models/weapons/melee/w_cricket_bat.mdl",
	"models/weapons/melee/w_crowbar.mdl",
	"models/weapons/melee/w_frying_pan.mdl",
	"models/weapons/melee/w_golfclub.mdl",
	"models/weapons/melee/w_electric_guitar.mdl",
	"models/weapons/melee/w_katana.mdl",
	"models/weapons/melee/w_machete.mdl",
	"models/weapons/melee/w_tonfa.mdl",
	"models/w_models/weapons/w_knife_t.mdl",
	// custom melee
	"models/weapons/melee/w_riotshield.mdl",
	"models/weapons/melee/w_fubar.mdl",
	"models/weapons/melee/w_paintrain.mdl",
	"models/weapons/melee/w_sledgehammer.mdl"
};

static StringMap mMeleeNames;

/**
 * Initializes internal structure necessary for weapons.inc functions
 * @remark It is recommended that you run this function on plugin start, but not necessary
 *
 * @noreturn
 */
stock void L4D2Melees_Init()
{
	if(mMeleeNames == null)
	{
		InitMeleeNamesMap();
	}
}

stock static void InitMeleeNamesMap()
{
	mMeleeNames = new StringMap();
	for(int i; i < MELEEID_SIZE; i++)
	{
		mMeleeNames.SetValue(l_sMeleeNames[i], i);
	}
}

stock char GetMeleeNameByID(int meleepID)
{
	return l_sMeleeNames[meleepID];
}

stock char GetMeleeModelByID(int meleepID)
{
	return l_sMeleeModels[meleepID];
}

/**
 * Performs bounds checking to determine if a MELEEID is valid
 * @remark Simple check to see if meleeID has a garbage value
 *
 * @param meleeID		MELEEID to check for validity
 * @return True if meleeID is valid, false otherwise.
 */
stock bool IsValidMeleeID(int meleeID)
{
	return meleeID >= MELEEID_FIREAXE && meleeID < MELEEID_SIZE;
}

/**
 * Tries to look up a MELEEID for a given Melee Name.
 *
 * @param meleeName 	Melee name string to look up Id from
 * @return				The corresponding MELEEID if found, else MELEEID_NONE
 */
stock int MeleeNameToID(char[] meleeName)
{
	if(mMeleeNames == null)
	{
		InitMeleeNamesMap();
	}
	int iID;
	if(mMeleeNames.GetValue(meleeName, iID))
	{
		return iID;
	}
	return MELEEID_NONE;
}

/**
 * Identifies a entity as a MELEEID. Use IdentifyWeapon() for identifies melee_spawn class
 *
 * @param entity		Index of entity to identify
 * @return MELEEID for the entity if it is a melee, MELEEID_NONE otherwise
 */
stock int IdentifyMelee(int entity)
{
	if (entity < MAXPLAYERS+1 || !IsValidEntity(entity) || !HasEntProp(entity, Prop_Data, "m_strMapSetScriptName"))
	{
		return MELEEID_NONE;
	}

	char sMelee[_NAME_LENGTH];
	GetEntPropString(entity, Prop_Data, "m_strMapSetScriptName", sMelee, sizeof(sMelee));

	return MeleeNameToID(sMelee);
}

static char l_sAvailableMelee[MELEEID_SIZE][_NAME_LENGTH];
static int l_iAvailableMeleeCount;

/**
 * Initializes/updates internal structure necessary for IsValidMelee function
 * @remark It NECESSARY to RUN this function OnMapStart()!
 *
 * @noreturn
 */
stock void UpdateValidMeleeClasses()
{
    l_iAvailableMeleeCount = 0;

    char sMeleeWeap[sizeof(l_sAvailableMelee[])];
    int iTableIndex = FindStringTable("MeleeWeapons"), iCountRows = GetStringTableNumStrings(iTableIndex);

    for (int i; i < iCountRows; i++ )
    {
        ReadStringTable(iTableIndex, i, sMeleeWeap, sizeof(sMeleeWeap));
        Format(l_sAvailableMelee[l_iAvailableMeleeCount++], sizeof(l_sAvailableMelee[]), sMeleeWeap);
    }
}

// Check if melee is available on current map
stock bool IsValidMelee(char[] meleeName)
{
	for(int i; i < l_iAvailableMeleeCount; i++)
	{
		if(StrEqual(meleeName, l_sAvailableMelee[i], false))
		{
			return true;
		}
	}

	return false;
}

// Precache melee world models. Probably for debugging purpose only.
stock void PrecacheMeleeModels()
{
	for(int i = MELEEID_FIREAXE; i < MELEEID_SIZE; i++)
	{
		if (!IsModelPrecached(l_sMeleeModels[i]))
			PrecacheModel(l_sMeleeModels[i], true);
	}
}

// ------------------------------WEAPON AND MELEE FUNCTION------------------------------------

enum ()
{
	WEPTYPE_NONE = -1,
	WEPTYPE_WEP,
	WEPTYPE_MELEE
}

// Identifies a entity as WEPTYPE
stock int IdentifyTypeByEnt(int entity, int &id = 0)
{
	int iID = IdentifyMelee(entity);
	int iWepType;

	if (iID == MELEEID_NONE){

		iID = IdentifyWeapon(entity);

		if (iID == WEPID_NONE)
			iWepType = WEPTYPE_NONE;
		else
			iWepType = WEPTYPE_WEP;
	}
	else
		iWepType = WEPTYPE_MELEE;

	id = iID;
	return iWepType;
}

// Identifies a string name as WEPTYPE
stock int IdentifyTypeByName(char[] name, int &id = 0)
{
	int iID = MeleeNameToID(name);
	int iWepType;

	if (iID == MELEEID_NONE){

		iID = WeaponNameToIDEx(name);

		if (iID == WEPID_NONE)
			iWepType = WEPTYPE_NONE;
		else
			iWepType = WEPTYPE_WEP;
	}
	else
		iWepType = WEPTYPE_MELEE;

	id = iID;
	return iWepType;
}

// Is melee or weapon has NONE ID
stock bool IsNoneID(int id)
{
	return id == MELEEID_NONE || id == WEPID_NONE;
}