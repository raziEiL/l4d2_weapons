#include <adt_trie>
#include <sourcemod>
#include <sdktools>

#if defined __l4d2_weapons_inc__
#endinput
#endif
#define __l4d2_weapons_inc__


// Weapon ID enumerations.
// These values are *NOT* arbitrary!
// They are used in game as the weaponid for weapon_spawn entities
enum WeaponId
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
	WEPID_UPGRADE_ITEM      // 55
};

new const bool:hasSpawnClass[WeaponId] =
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
new const String:WeaponNames[WeaponId][] =
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
new const String:WeaponModels[WeaponId][] =
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
	"models/w_models/weapons/w_shotgun_spas.mdl",
	"models/w_models/weapons/w_eq_medkit.mdl",
	"models/w_models/weapons/w_eq_molotov.mdl",
	"models/w_models/weapons/w_eq_pipebomb.mdl",
	"models/w_models/weapons/w_eq_painpills.mdl",
	"models/props_junk/gascan001a.mdl",
	"models/props_junk/propanecanister001a.mdl",
	"models/props_equipment/oxygentank01.mdl",
	"",
	"models/weapons/melee/w_chainsaw.mdl",
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

new const WeaponSlots[_:WeaponId] =
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

/*
new const String:WeaponMeleeModels[][] =
{
	"models/weapons/w_knife_t.mdl",
	"models/weapons/melee/w_bat.mdl",
	"models/weapons/melee/w_cricket_bat.mdl",
	"models/weapons/melee/w_crowbar.mdl",
	"models/weapons/melee/w_didgeridoo.mdl",
	"models/weapons/melee/w_electric_guitar.mdl",
	"models/weapons/melee/w_fireaxe.mdl",
	"models/weapons/melee/w_frying_pan.mdl",
	"models/weapons/melee/w_golfclub.mdl",
	"models/weapons/melee/w_katana.mdl",
	"models/weapons/melee/w_machete.mdl",
	"models/weapons/melee/w_riotshield.mdl",
	"models/weapons/melee/w_tonfa.mdl"
};
*/

static Handle:hWeaponNamesTrie;


/**
 * Initializes internal structure necessary for weapons.inc functions
 * @remark It is recommended that you run this function on plugin start, but not necessary
 *
 * @noreturn
 */
stock L4D2Weapons_Init()
{
	if(hWeaponNamesTrie == INVALID_HANDLE)
	{
		InitWeaponNamesTrie();
	}
}

stock GetSlotFromWeaponId(WeaponId:wepid)
{
	return WeaponSlots[wepid];
}

/**
 * Performs bounds checking to determine if a WeaponId is valid
 * @remark Simple check to see if wepid has a garbage value
 *
 * @param wepid		WeaponId to check for validity
 * @return True if wepid is valid, false otherwise.
 */
stock bool:IsValidWeaponId(WeaponId:wepid)
{
	return wepid >= WEPID_NONE && wepid < WeaponId;
}

/**
 * Checks to see if a given weaponid has a known WeaponModel in this file's model array
 * @remark Some weapons (like weapon_melee) have multiple valid models, and this will report false for them.
 *
 * @param wepid		WeaponId to check for a known weapon model for.
 * @return True if a valid weapon model exists for wepid, false otherwise.
 */
stock bool:HasValidWeaponModel(WeaponId:wepid)
{
	// return IsValidWeaponId(wepid) && WeaponModels[wepid][0] == '\0';
	return WeaponModels[wepid][0] != '\0';
}

/**
 * Tries to look up a WeaponId for a given Weapon Name.
 *
 * @param weaponName	Weapon name string to look up Id from
 * @return				The corresponding WeaponId if found, else WEPID_NONE
 */
stock WeaponId:WeaponNameToId(String:weaponName[])
{
	if(hWeaponNamesTrie == INVALID_HANDLE)
	{
		InitWeaponNamesTrie();
	}
	new WeaponID:id;
	if(GetTrieValue(hWeaponNamesTrie, weaponName, id))
	{
		return WeaponId:id;
	}
	return WEPID_NONE;
}

// This is the same as WeaponNameToId(), except it check and removes '_spawn' suffix
stock WeaponId:WeaponNameToIdEx(String:weaponName[])
{
	new len = HasSpawnSuffix(weaponName);
	if (len){
		decl String:sTemp[len];
		strcopy(sTemp, len, weaponName);
		RemoveSpawnSuffix(sTemp, len);
		return WeaponNameToId(sTemp);
	}
	return WeaponNameToId(weaponName);
}

stock static InitWeaponNamesTrie()
{
	hWeaponNamesTrie = CreateTrie();
	for(new i = 0; i < _:WeaponId; i++)
	{
		SetTrieValue(hWeaponNamesTrie, WeaponNames[WeaponId:i], i);
	}
}

/**
 * Identifies a weapon spawn or weapon entity as a WeaponID
 * @remark Should work on most weapon ents--even spawns, singles, held, etc.
 *
 * @param entity		Index of entity to identify
 * @return WeaponID for the entity if it is a weapon, WEPID_NONE otherwise
 */
stock WeaponId:IdentifyWeapon(entity)
{
	if(!entity || !IsValidEntity(entity) || !IsValidEdict(entity))
	{
		return WEPID_NONE;
	}
	decl String:class[64];
	if(!GetEdictClassname(entity, class, sizeof(class)))
	{
		return WEPID_NONE;
	}

	if(StrEqual(class, "weapon_spawn"))
	{
		return WeaponId:GetEntProp(entity,Prop_Send,"m_weaponID");
	}

	return WeaponNameToIdEx(class);
}

// returns an entity of the new weapon spawn on success
// -1 on failure or removal
/**
 * Attempts to convert a weapon spawn entity to a given weapon spawn
 * @remark Truthfully, this will work on any entity with origin/rotation.
 *		Also, requires the weapon to either have a Valid weapon model or have one provided
 *
 * @param entity		Index of entity to convert to weapon spawn
 * @param wepid			WeaponId of the weapon to have the spawner hold
 * @param count			Weapon count for the spawner (default 5)
 * @param model			World model to use for the weapon spawn
 * @return entity of the new weapon spawn, or -1 on errors.
 */
stock ConvertWeaponSpawn(entity, WeaponId:wepid, count=5, const String:model[] = "")
{
	if(!IsValidEntity(entity)) return -1;
	if(!IsValidWeaponId(wepid)) return -1;
	if(model[0] == '\0' && !HasValidWeaponModel(wepid)) return -1;


	new Float:origins[3], Float:angles[3];
	GetEntPropVector(entity, Prop_Send, "m_vecOrigin", origins);
	GetEntPropVector(entity, Prop_Send, "m_angRotation", angles);

	AcceptEntityInput(entity, "kill");

	entity = CreateEntityByName("weapon_spawn");
	if(!IsValidEntity(entity)) return -1;

	SetEntProp(entity, Prop_Send, "m_weaponID", wepid);

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
		if(!IsModelPrecached(WeaponModels[wepid]))
		{
			if(PrecacheModel(WeaponModels[wepid]) <= 0)
			{
				return -1;
			}
		}

		SetEntityModel(entity, WeaponModels[wepid]);
	}
	decl String:sCount[64];
	IntToString(count, sCount, sizeof(sCount));
	DispatchKeyValue(entity, "count", sCount);

	TeleportEntity(entity, origins, angles, NULL_VECTOR);
	DispatchSpawn(entity);
	SetEntityMoveType(entity,MOVETYPE_NONE);
	return entity;
}

// Precache valid weapon models
stock PrecacheWeapons()
{
	for(new i = 1; i < _:WEPID_TANK_CLAW; i++)
	{
		if (HasValidWeaponModel(WeaponId:i) && !IsModelPrecached(WeaponModels[WeaponId:i]))
			PrecacheModel(WeaponModels[WeaponId:i]);
	}
}

// Suffix helpers
stock AddSpawnSuffix(String:weaponName[], String:store[], len)
{
	if (!HasSpawnSuffix(weaponName))
		FormatEx(store, len, "%s_spawn", weaponName);
}

stock RemoveSpawnSuffix(String:weaponName[], Len = 0)
{
	if (Len)
		weaponName[Len-6]='\0';
	else {
		new len = HasSpawnSuffix(weaponName);
		if (len)
			weaponName[len-6]='\0';
	}
}

stock HasSpawnSuffix(String:weaponName[])
{
	new len = strlen(weaponName);
	if (len-6 > 0 && StrEqual(weaponName[len-6], "_spawn"))
		return len;
	return 0;
}