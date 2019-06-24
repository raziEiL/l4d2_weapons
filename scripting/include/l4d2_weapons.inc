/**
 * vim: set ts=4 :
 * =============================================================================
 * l4d2_weapons
 * Provides a set of functions for weapon/melee identification and manipulation.
 * GitHub: https://github.com/raziEiL/l4d2_weapons
 *
 * l4d2_weapons (C)2019 raziEiL [disawar1] <mr.raz4291@gmail.com>
 * Forked from MatthewClair/sourcemod-plugins
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

#define L4DWEP_NAME_LEN 64

/*
|==========================================================================
| WEAPON STRUCTURE
|==========================================================================
|
| WEPID enumerations are *NOT* arbitrary!
| They are used in game as the weaponid for weapon_spawn entities.
|
*/
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

/*
|--------------------------------------------------------------------------
| WEAPON PRIVATE STRUCTURE
|--------------------------------------------------------------------------
*/

// check for _spawn class
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
	true, false			// 54
};

// Weapon names for each of the weapons, used in identification.
static const char l_sWeaponNames[WEPID_SIZE][L4DWEP_NAME_LEN] =
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
	"weapon_ammo", "upgrade_item"                                                             // 54
};

// World Weapon models for each of the weapons. Used for making new weapon spawns.
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
	"models/props/terror/ammo_stack.mdl",
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

static StringMap l_mWeaponNames;

stock static void InitNamesMap()
{
	l_mWeaponNames = new StringMap();
	for (int i; i < WEPID_SIZE; i++)
		l_mWeaponNames.SetValue(l_sWeaponNames[i], i);
}

/*
|--------------------------------------------------------------------------
| WEAPON PUBLIC STRUCTURE
|--------------------------------------------------------------------------
*/

/**
 * Initializes internal structure necessary for l4d2_weapons.inc functions
 * @remark It is recommended that you run this function on plugin start, but not necessary
 *
 * @noreturn
 */
stock void L4D2Wep_Init()
{
	if (l_mWeaponNames == null)
		InitNamesMap();
}

/**
 * @param wepID		WEPID to get string for
 * @return 			Weapon name identificator string
 */
stock char[] L4D2Wep_GetNameByID(int wepID)
{
	return l_sWeaponNames[wepID];
}

/**
 * @param wepID		WEPID to check spawn class for
 * @return			True if a weapon has spawn class, false otherwise.
 */
stock bool L4D2Wep_HasSpawnClass(int wepID)
{
	return l_bHasSpawnClass[wepID];
}

/**
 * @param wepID		WEPID to get string for
 * @return 			Weapon model string
 */
stock char[] L4D2Wep_GetModelByID(int wepID)
{
	return l_sWeaponModels[wepID];
}

/**
 * @param wepID		WEPID to get slot for
 * @return 			Slot index, -1 if no slot existed
 */
stock int L4D2Wep_GetSlotByID(int wepID)
{
	return l_iWeaponSlots[wepID];
}

/**
 * Performs bounds checking to determine if a WEPID is valid
 * @remark Simple check to see if wepID has a garbage value. WEPID_NONE will return true!
 *
 * @param wepID		WEPID to check for validity
 * @return True if wepID is valid, false otherwise.
 */
stock bool L4D2Wep_IsValidID(int wepID)
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
stock bool L4D2Wep_HasValidModel(int wepID)
{
	return l_sWeaponModels[wepID][0] != '\0';
}

/**
 * Tries to look up a WEPID for a given Weapon Name.
 *
 * @param weaponName	Weapon name string to look up Id from
 * @return				The corresponding WEPID if found, else WEPID_NONE
 */
stock int L4D2Wep_NameToID(char[] weaponName)
{
	if (l_mWeaponNames == null)
		InitNamesMap();
	int iWepID;
	if (l_mWeaponNames.GetValue(weaponName, iWepID))
		return iWepID;
	return WEPID_NONE;
}

// This is the same as L4D2Wep_NameToID(), except it check and removes '_spawn' suffix
stock int L4D2Wep_NameToIDEx(char[] weaponName)
{
	int len = L4D2Wep_HasSpawnSuffix(weaponName);
	if (len){
		char[] sTemp = new char[len];
		strcopy(sTemp, len, weaponName);
		L4D2Wep_RemoveSpawnSuffix(sTemp, len);
		return L4D2Wep_NameToID(sTemp);
	}
	return L4D2Wep_NameToID(weaponName);
}

/**
 * Identifies a weapon spawn or weapon entity as a WEPID
 * @remark Should work on most weapon ents--even spawns, singles, held, etc.
 *
 * @param entity		Index of entity to identify
 * @return WEPID for the entity if it is a weapon, WEPID_NONE otherwise
 */
stock int L4D2Wep_Identify(int entity)
{
	if(entity < MAXPLAYERS+1 || !IsValidEntity(entity))
		return WEPID_NONE;

	char sClass[64];
	if (!GetEntityClassname(entity, sClass, sizeof(sClass)))
		return WEPID_NONE;

	if (HasEntProp(entity, Prop_Send, "m_weaponID"))
		return GetEntProp(entity, Prop_Send, "m_weaponID");

	return L4D2Wep_NameToIDEx(sClass);
}

// Precache a weapon world models (only for valid mdl)
stock void L4D2Wep_PrecacheModels()
{
	for (int i = WEPID_PISTOL; i < WEPID_SIZE; i++)
	{
		if (l_sWeaponModels[i][0] && !IsModelPrecached(l_sWeaponModels[i]))
			PrecacheModel(l_sWeaponModels[i], true);
	}
}

/*
|--------------------------------------------------------------------------
| WEAPON HELPERS
|--------------------------------------------------------------------------
*/

// Suffix helper. Converts "weapon_rifle" to "weapon_rifle_spawn"
stock void L4D2Wep_AddSpawnSuffix(char[] weaponName, char[] store, int len)
{
	if (!L4D2Wep_HasSpawnSuffix(weaponName))
		Format(store, len, "%s_spawn", weaponName);
}

// Suffix helper. Converts "weapon_rifle_spawn" to "weapon_rifle"
stock void L4D2Wep_RemoveSpawnSuffix(char[] weaponName, int Len = 0)
{
	if (Len)
		weaponName[Len-6]='\0';
	else {
		int len = L4D2Wep_HasSpawnSuffix(weaponName);
		if (len)
			weaponName[len-6]='\0';
	}
}

// Suffix helper. Check for a "_spawn" suffix
stock int L4D2Wep_HasSpawnSuffix(char[] weaponName)
{
	int len = strlen(weaponName);
	if (len-6 > 0 && StrEqual(weaponName[len-6], "_spawn"))
		return len;
	return 0;
}

/*
|==========================================================================
| MELEE STRUCTURE
|==========================================================================
|
| MELEEID enumerations are arbitrary.
|
*/
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

/*
|--------------------------------------------------------------------------
| MELEE PRIVATE STRUCTURE
|--------------------------------------------------------------------------
*/

// Melee names for each of the melee, used in identification/game
static const char l_sMeleeNames[MELEEID_SIZE][L4DWEP_NAME_LEN] =
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

// World Melee models for each of the melee. Used for making new melee spawns.
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

static StringMap l_mMeleeNames;
static char l_sAvailableMelee[MELEEID_SIZE][L4DWEP_NAME_LEN];
static int l_iAvailableMeleeCount;

stock static void InitMeleeNamesMap()
{
	l_mMeleeNames = new StringMap();
	for (int i; i < MELEEID_SIZE; i++)
		l_mMeleeNames.SetValue(l_sMeleeNames[i], i);
}

/*
|--------------------------------------------------------------------------
| MELEE PUBLIC STRUCTURE
|--------------------------------------------------------------------------
*/

/**
 * Initializes internal structure necessary for weapons.inc functions
 * @remark It is recommended that you run this function on plugin start, but not necessary
 *
 * @noreturn
 */
stock void L4D2Wep_InitMelees()
{
	if (l_mMeleeNames == null)
		InitMeleeNamesMap();
}

/**
 * @param meleepID	MELEEID to get string for
 * @return 			Melee name identificator string
 */
stock char L4D2Wep_GetMeleeNameByID(int meleepID)
{
	return l_sMeleeNames[meleepID];
}

/**
 * @param meleepID	WEPID to get string for
 * @return 			Melee model string
 */
stock char L4D2Wep_GetMeleeModelByID(int meleepID)
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
stock bool L4D2Wep_IsValidMeleeID(int meleeID)
{
	return meleeID >= MELEEID_FIREAXE && meleeID < MELEEID_SIZE;
}

/**
 * Tries to look up a MELEEID for a given Melee Name.
 *
 * @param meleeName 	Melee name string to look up Id from
 * @return				The corresponding MELEEID if found, MELEEID_NONE otherwise
 */
stock int L4D2Wep_MeleeNameToID(char[] meleeName)
{
	if (l_mMeleeNames == null)
		InitMeleeNamesMap();
	int iMeleeID;
	if (l_mMeleeNames.GetValue(meleeName, iMeleeID))
		return iMeleeID;
	return MELEEID_NONE;
}

/**
 * Identifies a entity as a MELEEID.
 *
 * @param entity		Index of entity to identify
 * @param spawn			Identifies weapon_melee_spawn, weapon_item_spawn classes
 * @return MELEEID for the entity if it is a weapon_melee, MELEEID_NONE otherwise
 */
stock int L4D2Wep_IdentifyMelee(int entity, bool spawn = false)
{
	if (entity < MAXPLAYERS+1 || !IsValidEntity(entity) || !HasEntProp(entity, Prop_Data, spawn ? "m_iszMeleeWeapon" : "m_strMapSetScriptName"))
	{
		return MELEEID_NONE;
	}

	char sMelee[L4DWEP_NAME_LEN];
	GetEntPropString(entity, Prop_Data, spawn ? "m_iszMeleeWeapon" : "m_strMapSetScriptName", sMelee, sizeof(sMelee));

	return L4D2Wep_MeleeNameToID(sMelee);
}

/**
 * Initializes/updates internal structure necessary for L4D2Wep_IsValidMelee() function
 * @remark Method to detect valid melees via string table (Thanks @Electr0)
 * @remark IT NECESSARY TO RUN THIS FUNCTION OnMapStart()!
 *
 * @noreturn
 */
stock void L4D2Wep_OnMapStart()
{
    l_iAvailableMeleeCount = 0;

    char sMeleeWeap[sizeof(l_sAvailableMelee[])];
    int iTableIndex = FindStringTable("MeleeWeapons"), iCountRows = GetStringTableNumStrings(iTableIndex);

    for (int i; i < iCountRows; i++)
    {
        ReadStringTable(iTableIndex, i, sMeleeWeap, sizeof(sMeleeWeap));
        Format(l_sAvailableMelee[l_iAvailableMeleeCount++], sizeof(l_sAvailableMelee[]), sMeleeWeap);
    }
}

/**
 * Performs melee checking to determine if a melee is available on current map
 * @remark Method to detect valid melees via string table (Thanks @Electr0)
 *
 * @param meleeName		Melee Name string to check for validity
 * @return True if melee is valid, false otherwise.
 */
stock bool L4D2Wep_IsValidMelee(char[] meleeName)
{
	for (int i; i < l_iAvailableMeleeCount; i++)
	{
		if (StrEqual(meleeName, l_sAvailableMelee[i], false))
			return true;
	}

	return false;
}

// Precache melee world models. Probably for debugging purpose only.
// If melee is available on current map (see L4D2Wep_IsValidMelee()) there no sense to precache models.
stock void L4D2Wep_PrecacheMeleeModels()
{
	for (int i = MELEEID_FIREAXE; i < MELEEID_SIZE; i++)
	{
		if (!IsModelPrecached(l_sMeleeModels[i]))
			PrecacheModel(l_sMeleeModels[i], true);
	}
}

/*
|==========================================================================
| ITEM STRUCTURE
|==========================================================================
|
| Since the l4d2_weapons structure was divided into two types WEAPON and MELEE,
| item structure used to identify them.
| Also this structure make the code shorter.
|
*/

enum ItemType
{
	ITEM_NONE = -1,
	ITEM_WEP,
	ITEM_MELEE
}

// Identifies a entity as ItemType
stock ItemType L4D2Wep_IdentifyItemByEnt(int entity, int &anyID = 0)
{
	int iID = L4D2Wep_IdentifyMelee(entity);
	ItemType Type;

	if (iID == MELEEID_NONE){

		iID = L4D2Wep_Identify(entity);

		if (iID == WEPID_NONE)
			Type = ITEM_NONE;
		else
			Type = ITEM_WEP;
	}
	else
		Type = ITEM_MELEE;

	anyID = iID;
	return Type;
}

// Identifies a string name as ItemType
stock ItemType L4D2Wep_IdentifyItemByName(char[] anyName, int &anyID = 0)
{
	int iID = L4D2Wep_MeleeNameToID(anyName);
	ItemType Type;

	if (iID == MELEEID_NONE){

		iID = L4D2Wep_NameToIDEx(anyName);

		if (iID == WEPID_NONE)
			Type = ITEM_NONE;
		else
			Type = ITEM_WEP;
	}
	else
		Type = ITEM_MELEE;

	anyID = iID;
	return Type;
}

stock bool L4D2Wep_IsItemNoneID(int anyID, ItemType type)
{
	return anyID == (type == ITEM_WEP ? WEPID_NONE : MELEEID_NONE);
}

stock bool L4D2Wep_IsValidItemID(int anyID, ItemType type)
{
	return type == ITEM_WEP ? L4D2Wep_IsValidID(anyID) : L4D2Wep_IsValidMeleeID(anyID);
}

// Uses cheat command to give an item to player.
stock void L4DWep_GiveItemByName(int client, char[] weaponName)
{
	CheatCommand(client, "give", StrEqual(weaponName, l_sWeaponNames[WEPID_AMMO]) ? "ammo" : weaponName);
}

stock void L4DWep_GiveItemByID(int client, int anyID, ItemType type)
{
	CheatCommand(client, "give", type == ITEM_WEP ? (anyID == WEPID_AMMO ? "ammo" : l_sWeaponNames[anyID]) : l_sMeleeNames[anyID]);
}
/*
|--------------------------------------------------------------------------
| ITEM PRIVATE STRUCTURE
|--------------------------------------------------------------------------
*/
stock static void CheatCommand(int client, char[] command, char[] args = "")
{
	int iFlags = GetCommandFlags(command);
	SetCommandFlags(command, iFlags & ~FCVAR_CHEAT);
	FakeClientCommand(client, "%s %s", command, args);
	SetCommandFlags(command, iFlags);
}

/*
|==========================================================================
| AMMO HELPER STRUCTURE
|==========================================================================
|
| Used to detect and set max ammo for WEAPON SPAWN FEATURE.
| Simply based on game convars.
|
*/

// Supports only primary weapons + chainsaw
enum AmmoID
{
	AMMOID_NONE = -1,
	AMMOID_ASHOTGUN,
	AMMOID_CHAINSAW,
	AMMOID_GL, // 2
	AMMOID_HUNTING,
	AMMOID_M60,
	AMMOID_RIFLE, // 5
	AMMOID_SHOTGUN,
	AMMOID_SMG,
	AMMOID_SNIPER // 8
}
/*
|--------------------------------------------------------------------------
| AMMO HELPER PRIVATE STRUCTURE
|--------------------------------------------------------------------------
*/
static ConVar l_hCvarAmmo[AmmoID];
static int l_iCvarAmmo[AmmoID];

static const char l_sCvarAmmoNames[AmmoID][L4DWEP_NAME_LEN] =
{
	"ammo_autoshotgun_max",
	"ammo_chainsaw_max",
	"ammo_grenadelauncher_max", // 2
	"ammo_huntingrifle_max",
	"ammo_m60_max",
	"ammo_assaultrifle_max", // 5
	"ammo_shotgun_max",
	"ammo_smg_max",
	"ammo_sniperrifle_max" // 8
};

stock static void GetAmmoCvars()
{
	for (AmmoID i; i < AmmoID; i++)
		l_iCvarAmmo[i] = l_hCvarAmmo[i].IntValue;
}

stock static void InitAmmoCvar(AmmoID ammoID)
{
	l_hCvarAmmo[ammoID] = FindConVar(l_sCvarAmmoNames[ammoID]);
	l_iCvarAmmo[ammoID] = l_hCvarAmmo[ammoID].IntValue;
	l_hCvarAmmo[ammoID].AddChangeHook(L4DWep_ch_CvarAmmoChanged);
}

/*
|--------------------------------------------------------------------------
| AMMO HELPER PUBLIC STRUCTURE
|--------------------------------------------------------------------------
*/

/**
 * Initializes internal structure necessary for AMMOID functions
 * @remark It is recommended that you run this function on plugin start, but not necessary
 *
 * @noreturn
 */
stock void L4DWep_InitAmmoCvars()
{
	if (l_hCvarAmmo[0] == null){
		for (AmmoID i; i < AmmoID; i++)
			InitAmmoCvar(i);
	}
}

// @return AmmoID if wepID is supported, AMMOID_NONE otherwise (there no ammo convar for this weapon)
stock AmmoID L4DWep_WepIDToAmmoID(int wepID)
{
	switch (wepID)
	{
		case WEPID_RIFLE, WEPID_RIFLE_DESERT, WEPID_RIFLE_AK47, WEPID_RIFLE_SG552:
			return AMMOID_RIFLE;
		case WEPID_PUMPSHOTGUN, WEPID_SHOTGUN_CHROME:
			return AMMOID_SHOTGUN;
		case WEPID_AUTOSHOTGUN, WEPID_SHOTGUN_SPAS:
			return AMMOID_ASHOTGUN;
		case WEPID_HUNTING_RIFLE:
			return AMMOID_HUNTING;
		case WEPID_SMG, WEPID_SMG_SILENCED, WEPID_SMG_MP5:
			return AMMOID_SMG;
		case WEPID_CHAINSAW:
			return AMMOID_CHAINSAW;
		case WEPID_GRENADE_LAUNCHER:
			return AMMOID_GL;
		case WEPID_RIFLE_M60:
			return AMMOID_M60;
		case WEPID_SNIPER_MILITARY, WEPID_SNIPER_AWP, WEPID_SNIPER_SCOUT:
			return AMMOID_SNIPER;
	}
	return AMMOID_NONE;
}

// @return max ammo from convar for ammoID
stock int L4DWep_GetAmmo(AmmoID ammoID)
{
	if (l_hCvarAmmo[ammoID] == null)
		InitAmmoCvar(ammoID);
	return l_iCvarAmmo[ammoID];
}

// set max ammo to weapon entity by wepID
stock bool L4DWep_SetAmmoByID(int entity, int wepID)
{
	if (HasEntProp(entity, Prop_Send, "m_iExtraPrimaryAmmo")){

		AmmoID Ammo = L4DWep_WepIDToAmmoID(wepID);

		if (Ammo != AMMOID_NONE){
			SetEntProp(entity, Prop_Send, "m_iExtraPrimaryAmmo", L4DWep_GetAmmo(Ammo), 4);
			return true;
		}
	}
	return false;
}

public void L4DWep_ch_CvarAmmoChanged(ConVar convar, const char[] oldValue, const char[] newValue)
{
	if (!StrEqual(oldValue, newValue))
		GetAmmoCvars();
}

/*
|==========================================================================
| WEAPON SPAWN FEATURE
|==========================================================================
|
| Functions to create weapon_, melee_ or _spawn class entities.
|
*/

/**
 * Spawn a weapon or melee entity.
 *
 * @param anyID			WEPID or MELEEID
 * @param ItemType		ItemType
 * @param origin		Entity origin
 * @param angles		Entity angles
 * @param applyVecFix	Fixes a weapon origin and angles to make it look better.
 * @param spawn			True to create a weapon_..._spawn class if it possible, otherwise a weapon_... class will be created.
 * @param count			Max number of weapons given before disappearing (<= 0 infinity). Use it with spawn = true only.
 * @return Entity index on success, or -1 on failure
 */
stock int L4D2Wep_SpawnItem(int anyID, ItemType type, float origin[3], float angles[3] = {0.0, ...}, bool applyVecFix = true, bool spawn = true, int count = 5)
{
	switch (type)
	{
		case ITEM_WEP:
			return L4D2Wep_Spawn(anyID, origin, angles, applyVecFix, spawn, count);
		case ITEM_MELEE:
			return L4D2Wep_SpawnMelee(anyID, origin, angles, applyVecFix, spawn, count);
	}
	return -1;
}

/**
 * Spawn a weapon entity.
 *
 * @param wepID			WEPID
 * @param origin		Entity origin
 * @param angles		Entity angles
 * @param applyVecFix	Fixes a weapon origin and angles to make it look better. True to call L4D2Wep_SpawnVectorFix()
 * @param spawn			True to create a weapon_..._spawn class if it possible, otherwise a weapon_... class will be created.
 * @param count			Max number of weapons given before disappearing (<= 0 infinity). Use it with spawn = true only.
 * @return Entity index on success, or -1 on failure
 */
stock int L4D2Wep_Spawn(int wepID, float origin[3], float angles[3] = {0.0, ...}, bool applyVecFix = true, bool spawn = true, int count = 5)
{
	if (l_sWeaponModels[wepID][0]){

		if (!IsModelPrecached(l_sWeaponModels[wepID]) && PrecacheModel(l_sWeaponModels[wepID]) <= 0)
			return -1;

		int iEnt;
		bool IsSpawn;
		char sTemp[L4DWEP_NAME_LEN];

		if (spawn && l_bHasSpawnClass[wepID]){

			sTemp = l_sWeaponNames[wepID];
			L4D2Wep_AddSpawnSuffix(sTemp, sTemp, sizeof(sTemp));
			iEnt = CreateEntityByName(sTemp);
			IsSpawn = true;
		}
		else
			iEnt = CreateEntityByName(l_sWeaponNames[wepID]);

		if (iEnt != -1){

			if (IsSpawn){

				if (count >= 1){
					IntToString(count, sTemp, sizeof(sTemp));
					DispatchKeyValue(iEnt, "count", sTemp);
				}
				else {
					DispatchKeyValue(iEnt, "count", "9999");
					DispatchKeyValue(iEnt, "spawnflags", "8");
				}
			}

			if (applyVecFix)
				L4D2Wep_FixModelVectors(wepID, origin, angles);

			DispatchKeyValue(iEnt, "solid", "6");
			DispatchKeyValue(iEnt, "rendermode", "3");
			DispatchKeyValue(iEnt, "model", l_sWeaponModels[wepID]);
			DispatchKeyValueVector(iEnt, "origin", origin);
			DispatchKeyValueVector(iEnt, "angles", angles);
			DispatchSpawn(iEnt);
			SetEntityMoveType(iEnt, MOVETYPE_NONE);

			if (!IsSpawn)
				L4DWep_SetAmmoByID(iEnt, wepID);
		}
		return iEnt;
	}
	else
		LogError("Wep ID %d has invalid model!", wepID);
	return -1;
}

/**
 * Spawn a valid melee (see L4D2Wep_IsValidMelee) entity.
 *
 * @param meleeID		MELEEID
 * @param origin		Entity origin
 * @param angles		Entity angles
 * @param applyVecFix	Fixes a melee origin and angles to make it look better. True to call L4D2Wep_FixMeleeModelVectors()
 * @param spawn			True to create a melee_..._spawn class, otherwise a melee_... class will be created.
 * @param count			Max number of melees given before disappearing (<= 0 infinity). Use it with spawn = true only.
 * @return Entity index on success, or -1 on failure
 */
stock int L4D2Wep_SpawnMelee(int meleeID, float origin[3], float angles[3] = {0.0, ...}, bool applyVecFix = true, bool spawn = true, int count = 5)
{
	if (L4D2Wep_IsValidMelee(l_sMeleeNames[meleeID])){

		int iEnt;
		char sTemp[L4DWEP_NAME_LEN];

		if (spawn){

			sTemp = l_sWeaponNames[WEPID_MELEE];
			L4D2Wep_AddSpawnSuffix(sTemp, sTemp, sizeof(sTemp));
			iEnt = CreateEntityByName(sTemp);
		}
		else
			iEnt = CreateEntityByName(l_sWeaponNames[WEPID_MELEE]);

		if (iEnt != -1){

			if (spawn){

				if (count >= 1){
					IntToString(count, sTemp, sizeof(sTemp));
					DispatchKeyValue(iEnt, "count", sTemp);
				}
				else {
					DispatchKeyValue(iEnt, "count", "9999");
					DispatchKeyValue(iEnt, "spawnflags", "8");
				}
				DispatchKeyValue(iEnt, "melee_weapon", l_sMeleeNames[meleeID]);
			}
			else
				DispatchKeyValue(iEnt, "melee_script_name", l_sMeleeNames[meleeID]);

			if (applyVecFix)
				L4D2Wep_FixMeleeModelVectors(meleeID, origin, angles);

			DispatchKeyValue(iEnt, "solid", "6");
			DispatchKeyValue(iEnt, "rendermode", "3");
			DispatchKeyValue(iEnt, "model", l_sMeleeModels[meleeID]);
			DispatchKeyValueVector(iEnt, "origin", origin);
			DispatchKeyValueVector(iEnt, "angles", angles);
			DispatchSpawn(iEnt);
			SetEntityMoveType(iEnt, MOVETYPE_NONE);
		}
		return iEnt;
	}
	else
		LogError("Melee ID %d has not available on map!", meleeID);
	return -1;
}

/**
 * Attempts to convert a weapon spawn entity to a given weapon spawn
 * @remark Truthfully, this will work on any entity with origin/rotation.
 *		Also, requires the weapon to either have a Valid weapon model or have one provided
 *
 * @param entity		Index of entity to convert to weapon spawn
 * @param wepID			ID of the weapon to have the spawner hold
 * @param count			Weapon count for the spawner (default 5)
 * @param model			World model to use for the weapon spawn
 * @return entity of the new weapon spawn, or -1 on errors.
 */
stock int L4D2Wep_ConvertWeaponSpawn(int entity, int wepID, int count = 5, const char[] model = "")
{
	if (!IsValidEntity(entity) || (!model[0] && !l_sWeaponModels[wepID][0]))
		return -1;

	float vOrigins[3], vAngles[3];
	GetEntPropVector(entity, Prop_Send, "m_vecOrigin", vOrigins);
	GetEntPropVector(entity, Prop_Send, "m_angRotation", vAngles);
	AcceptEntityInput(entity, "kill");

	entity = CreateEntityByName("weapon_spawn");
	if (entity == -1) 
		return -1;

	if (model[0]){
		if (!IsModelPrecached(model) && PrecacheModel(model) <= 0)
			return -1;
			
		DispatchKeyValue(entity, "model", model);
	}
	else {
		if (!IsModelPrecached(l_sWeaponModels[wepID]) && PrecacheModel(l_sWeaponModels[wepID]) <= 0)
			return -1;

		DispatchKeyValue(entity, "model", l_sWeaponModels[wepID]);
	}
	char sCount[6];
	IntToString(count, sCount, sizeof(sCount));
	DispatchKeyValue(entity, "count", sCount);
	DispatchKeyValueVector(entity, "origin", vOrigins);
	DispatchKeyValueVector(entity, "angles", vAngles);
	DispatchSpawn(entity);
	SetEntProp(entity, Prop_Send, "m_weaponID", wepID);
	SetEntityMoveType(entity, MOVETYPE_NONE);
	return entity;
}

stock void L4D2Wep_FixModelVectors(int wepID, float origin[3], float angles[3])
{
	switch (wepID)
	{
		case WEPID_PISTOL, WEPID_SMG, WEPID_PUMPSHOTGUN, WEPID_AUTOSHOTGUN, WEPID_RIFLE, WEPID_HUNTING_RIFLE, \
		WEPID_SMG_SILENCED, WEPID_SHOTGUN_CHROME, WEPID_RIFLE_DESERT, WEPID_SNIPER_MILITARY, WEPID_SHOTGUN_SPAS, \
		WEPID_GRENADE_LAUNCHER, WEPID_RIFLE_AK47, WEPID_PISTOL_MAGNUM, WEPID_SMG_MP5, WEPID_RIFLE_SG552, \
		WEPID_SNIPER_AWP, WEPID_SNIPER_SCOUT, WEPID_RIFLE_M60: angles[2] += 90.0, origin[2] += 1.0;
		case WEPID_FIRST_AID_KIT, WEPID_PIPE_BOMB: angles[0] += 90.0, origin[2] += 2.2;
		case WEPID_MOLOTOV, WEPID_VOMITJAR: origin[2] += 5.0;
		case WEPID_GASCAN: origin[2] += 10.5;
		case WEPID_PROPANE_TANK, WEPID_GNOME_CHOMPSKI: origin[2] += 11.0;
		case WEPID_CHAINSAW: origin[2] += 4.0;
		case WEPID_FIREWORKS_BOX: origin[2] += 3.0;
	}
}

stock void L4D2Wep_FixMeleeModelVectors(int meleeID, float origin[3], float angles[3])
{
	switch (meleeID)
	{
		case MELEEID_ALLIANCE_SHIELD: angles[0] -= 90.0;
		case MELEEID_FUBAR: angles[2] += 90.0;
		default: angles[1] += 90.0, angles[2] += 90.0;
	}
}