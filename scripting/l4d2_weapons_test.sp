#define PLUGIN_VERSION "1.1"

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

Handle g_hCmdSpawnTimer;
int g_iCmdID;
bool g_bIsSpawn;

public void OnPluginStart()
{
	RegServerCmd("sm_l4d2wep", CommandWep, "weapon structure test");
	RegServerCmd("sm_l4d2wep_melee", CommandWepMelee, "melee structure test");
	RegServerCmd("sm_l4d2wep_identify", CommandIdentify, "loop through entity to identify weapons");
	RegServerCmd("sm_l4d2wep_identify_melee", CommandIdentifyMelees, "loop through entity to identify melees");
	// Usages: <weapName> || <ID> <TYPE>
	RegAdminCmd("sm_l4d2wep_give", CommandGiveItem, ADMFLAG_ROOT, "give a weapon/melee to payer");
	// Usages: <weapName> || <ID> <TYPE>
	RegAdminCmd("sm_l4d2wep_spawn", CommandSpawnItem, ADMFLAG_ROOT, "spawn a weapon/melee by player position");
	// Usages: <IsSpawnClass> [SpawnMelees]
	RegAdminCmd("sm_l4d2wep_spawn_all", CommandSpawnItems, ADMFLAG_ROOT, "spawn items with 0.5s interval");
}

public void OnMapStart()
{
	L4D2Wep_OnMapStart();
	g_hCmdSpawnTimer = null;
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

public Action CommandGiveItem(int client, int args)
{
	if (client){

		char sTemp[L4DWEP_NAME_LEN];
		GetCmdArg(1, sTemp, sizeof(sTemp));

		// give wep by name
		if (args == 1){

			int iID;
			ItemType Type = L4D2Wep_IdentifyItemByName(sTemp, iID);

			if (Type == ITEM_NONE || L4D2Wep_IsItemNoneID(iID, Type) || !L4D2Wep_IsValidItemID(iID, Type)){
				ReplyToCommand(client, "Unknown weapon name or bad ID!");
				return Plugin_Handled;
			}
			L4DWep_GiveItemByName(client, sTemp);
		}
		// give wep by id and type
		else if (args == 2){

			int iID = StringToInt(sTemp);
			GetCmdArg(2, sTemp, sizeof(sTemp));
			ItemType Type = view_as<ItemType>(StringToInt(sTemp));

			if (Type == ITEM_NONE || L4D2Wep_IsItemNoneID(iID, Type) || !L4D2Wep_IsValidItemID(iID, Type)){
				ReplyToCommand(client, "Unknown weapon name or bad ID!");
				return Plugin_Handled;
			}
			L4DWep_GiveItemByID(client, iID, Type);
 		}
		else
			ReplyToCommand(client, "Usages: <weapName> || <ID> <TYPE>");
	}
	else
		ReplyToCommand(client, "Command is not available from server console!");

	return Plugin_Handled;
}

public Action CommandSpawnItem(int client, int args)
{
	if (client){

		char sTemp[L4DWEP_NAME_LEN];
		GetCmdArg(1, sTemp, sizeof(sTemp));

		if (args == 1 || args == 2){

			int iID;
			ItemType Type;
			// spawn wep by name
			if (args == 1){
				Type = L4D2Wep_IdentifyItemByName(sTemp, iID);
			}
			// give wep by id and type
			else if (args == 2){

				iID = StringToInt(sTemp);
				GetCmdArg(2, sTemp, sizeof(sTemp));
				Type = view_as<ItemType>(StringToInt(sTemp));
			}

			if (Type == ITEM_NONE || L4D2Wep_IsItemNoneID(iID, Type) || !L4D2Wep_IsValidItemID(iID, Type)){
				ReplyToCommand(client, "Unknown weapon name or bad ID!");
				return Plugin_Handled;
			}

			float vOrigin[3];
			GetClientAbsOrigin(client, vOrigin);
			int iEnt = L4D2Wep_SpawnItem(iID, Type, vOrigin);

			if (iEnt == -1)
				ReplyToCommand(client, "Failed to spawn ID %d, ItemType %d, ent %d", iID, Type, iEnt);
		}
		else
			ReplyToCommand(client, "Usages: <weapName> || <ID> <TYPE>");
	}
	else
		ReplyToCommand(client, "Command is not available from server console!");

	return Plugin_Handled;
}

public Action CommandSpawnItems(int client, int args)
{
	if (client){

		if (g_hCmdSpawnTimer){
			CloseHandle(g_hCmdSpawnTimer);
			g_hCmdSpawnTimer = null;
			ReplyToCommand(client, "Canceled!");
			return Plugin_Handled;
		}
		if (args){

			char sTemp[2];
			GetCmdArg(1, sTemp, sizeof(sTemp));
			g_bIsSpawn = view_as<bool>(StringToInt(sTemp));

			if (args == 1){
				g_iCmdID = WEPID_PISTOL;
				g_hCmdSpawnTimer = CreateTimer(0.5, WP_t_SpawnWep, GetClientUserId(client), TIMER_FLAG_NO_MAPCHANGE|TIMER_REPEAT);
			}
			else {
				g_iCmdID = MELEEID_FIREAXE;
				g_hCmdSpawnTimer = CreateTimer(0.5, WC_t_SpawnMelees, GetClientUserId(client), TIMER_FLAG_NO_MAPCHANGE|TIMER_REPEAT);
			}
			ReplyToCommand(client, "Spawn weapons with 0.5s. interval");
		}
		else
			ReplyToCommand(client, "Usages: <IsSpawnClass> [SpawnMelees]");
	}
	return Plugin_Handled;
}

public Action WP_t_SpawnWep(Handle timer, any client)
{
	client = GetClientOfUserId(client);
	if (!client || g_iCmdID >= WEPID_TANK_CLAW){
		g_hCmdSpawnTimer = null;
		ReplyToCommand(client, "Weapons has been spawned!");
		return Plugin_Stop;
	}

	float vOrigin[3];
	GetClientAbsOrigin(client, vOrigin);
	L4D2Wep_Spawn(g_iCmdID++, vOrigin, _, _, g_bIsSpawn);
	return Plugin_Continue;
}

public Action WC_t_SpawnMelees(Handle timer, any client)
{
	client = GetClientOfUserId(client);
	if (!client || g_iCmdID >= MELEEID_SIZE){
		g_hCmdSpawnTimer = null;
		ReplyToCommand(client, "Melees has been spawned!");
		return Plugin_Stop;
	}

	float vOrigin[3];
	GetClientAbsOrigin(client, vOrigin);
	L4D2Wep_SpawnMelee(g_iCmdID++, vOrigin, _, _, g_bIsSpawn);
	return Plugin_Continue;
}