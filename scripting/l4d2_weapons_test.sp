#define PLUGIN_VERSION "1.0"

#pragma semicolon 1
#pragma newdecls required
#include <sourcemod>
#include <l4d2_weapons>

public Plugin myinfo =
{
	name = "L4D2 Weapons Test",
	author = "raziEiL [disawar1]",
	description = "Examples of l4d2_weapons.inc usage (For debugging purpose only)",
	version = PLUGIN_VERSION,
	url = "http://steamcommunity.com/id/raziEiL"
}

public void OnPluginStart()
{
	// weapon test
	RegServerCmd("sm_l4d2wep", CommandWep);
	// melee test
	RegServerCmd("sm_l4d2wep_melee", CommandWepMelee);
	// loop through entity to identify
	RegServerCmd("sm_l4d2wep_identify", CommandIdentify);
	RegServerCmd("sm_l4d2wep_identify_melee", CommandIdentifyMelees);
	// give a weapon/melee to payer
	RegAdminCmd("sm_l4d2wep_give", CommandGiveAny, ADMFLAG_ROOT);
}

public void OnMapStart()
{
	L4D2Wep_OnMapStart();
}

public Action CommandWep(int args)
{
	LogMessage("WEPID CONSTANT TEST");
	for (int i; i < WEPID_SIZE; i++){

		LogMessage("ID %d, Slot %d, hasSpawn %d, Name '%s', hasValidMdl %d, Mdl '%s'", i, \
		L4D2Wep_GetSlotByID(i), L4D2Wep_HasSpawnClass(i), L4D2Wep_GetNameByID(i), \
		L4D2Wep_HasValidModel(i), L4D2Wep_GetModelByID(i));
	}

	LogMessage("STRINGMAP TEST");
	for (int i; i < WEPID_SIZE; i++)
		LogMessage("ID %d %s", i, L4D2Wep_NameToID(L4D2Wep_GetNameByID(i)) == i ? "OK" : "NOT EQUAL!");

	LogMessage("SUFFIX TEST");
	char sWepClass[L4DWEP_NAME_LEN];
	sWepClass = L4D2Wep_GetNameByID(WEPID_AUTOSHOTGUN);
	LogMessage("'%s' - Initial string", sWepClass);
	L4D2Wep_AddSpawnSuffix(sWepClass, sWepClass, sizeof(sWepClass));
	LogMessage("'%s', ID %d - Add suffix", sWepClass, L4D2Wep_NameToIDEx(sWepClass));
	L4D2Wep_RemoveSpawnSuffix(sWepClass);
	LogMessage("'%s' - Remove suffix", sWepClass);

	return Plugin_Handled;
}

public Action CommandWepMelee(int args)
{
	LogMessage("MELEEID CONSTANT AND VALID TEST");
	for (int i; i < MELEEID_SIZE; i++)
		LogMessage("ID %d, Name '%s', IsValid %d, Mdl '%s'", i, L4D2Wep_GetMeleeNameByID(i), L4D2Wep_IsValidMelee(L4D2Wep_GetMeleeNameByID(i)), L4D2Wep_GetMeleeModelByID(i));

	LogMessage("STRINGMAP TEST");
	for (int i; i < MELEEID_SIZE; i++)
		LogMessage("ID %d %s", i, L4D2Wep_MeleeNameToID(L4D2Wep_GetMeleeNameByID(i)) == i ? "OK" : "NOT EQUAL!");

	return Plugin_Handled;
}

public Action CommandIdentify(int args)
{
	LogMessage("IDENTIFY ENT TEST");
	int iEnts = GetEntityCount(), iEnt, iWepID;
	char sClassName[L4DWEP_NAME_LEN];
	for (int i = MAXPLAYERS+1; i <= iEnts; i++){
		if (IsValidEntity(i) && (iEnt = EntRefToEntIndex(i)) != INVALID_ENT_REFERENCE && (iWepID = L4D2Wep_Identify(iEnt)) != WEPID_NONE){
			GetEntityClassname(iEnt, sClassName, sizeof(sClassName));
			LogMessage("ent %d, ID %d, Class '%s', Name '%s'", i, iWepID, sClassName, L4D2Wep_GetNameByID(iWepID));
		}
	}
}

public Action CommandIdentifyMelees(int args)
{
	LogMessage("IDENTIFY MELEE ENT TEST");
	int iEnts = GetEntityCount(), iEnt, iMeleeID;
	char sClassName[L4DWEP_NAME_LEN];
	for (int i = MAXPLAYERS+1; i <= iEnts; i++){
		if (IsValidEntity(i) && (iEnt = EntRefToEntIndex(i)) != INVALID_ENT_REFERENCE){

			// weapon_melee_spawn, weapon_item_spawn
			if ((iMeleeID = L4D2Wep_IdentifyMelee(iEnt, true)) != MELEEID_NONE){

				GetEntityClassname(iEnt, sClassName, sizeof(sClassName));
				LogMessage("ent %d, ID %d, Class '%s', Name '%s'", i, iMeleeID, sClassName, L4D2Wep_GetMeleeNameByID(iMeleeID));
			}
			// weapon_melee
			else if ((iMeleeID = L4D2Wep_IdentifyMelee(iEnt)) != MELEEID_NONE){

				GetEntityClassname(iEnt, sClassName, sizeof(sClassName));
				LogMessage("ent %d, ID %d, Class '%s', Name '%s'", i, iMeleeID, sClassName, L4D2Wep_GetMeleeNameByID(iMeleeID));
			}
		}
	}
}

public Action CommandGiveAny(int client, int args)
{
	if (client){

		char sTemp[L4DWEP_NAME_LEN];
		GetCmdArg(1, sTemp, sizeof(sTemp));

		// give wep by name
		if (args == 1){

			int iID;
			int iType = L4D2Wep_IdentifyAnyByName(sTemp, iID);

			if (iType == WEPTYPE_NONE || L4D2Wep_IsNoneAnyID(iID, iType) || !L4D2Wep_IsValidAnyID(iID, iType)){
				ReplyToCommand(client, "Unknown weapon name or bad ID!");
				return Plugin_Handled;
			}
			L4DWep_GiveAnyByName(client, sTemp);
		}
		// give wep by id and type
		else if (args == 2){

			int iID = StringToInt(sTemp);
			GetCmdArg(2, sTemp, sizeof(sTemp));
			int iType = StringToInt(sTemp);

			if (iType == WEPTYPE_NONE || L4D2Wep_IsNoneAnyID(iID, iType) || !L4D2Wep_IsValidAnyID(iID, iType)){
				ReplyToCommand(client, "Unknown weapon name or bad ID!");
				return Plugin_Handled;
			}
			L4DWep_GiveAnyByID(client, iID, iType);
 		}
		else
			ReplyToCommand(client, "!give <weapName> | <ID> <TYPE>");
	}
	else
		ReplyToCommand(client, "Command is not available from server console!");

	return Plugin_Handled;
}
