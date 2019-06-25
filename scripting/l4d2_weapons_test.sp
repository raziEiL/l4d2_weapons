#define PLUGIN_VERSION "1.2"

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
	RegServerCmd("sm_l4d2wep", CommandWep, "Weapon structure test");
	RegServerCmd("sm_l4d2wep_melee", CommandWepMelee, "Melee structure test");
	RegServerCmd("sm_l4d2wep_identify", CommandIdentify, "Loop through entity to identify weapons");
	RegServerCmd("sm_l4d2wep_identify_melee", CommandIdentifyMelees, "Loop through entity to identify melees");
	// Usages: <weapName> || <ID> <TYPE>
	RegAdminCmd("sm_l4d2wep_give", CommandGiveItem, ADMFLAG_ROOT, "Gives a weapon/melee to payer");
	// Usages: <weapName> || <ID> <TYPE>
	RegAdminCmd("sm_l4d2wep_spawn", CommandSpawnItem, ADMFLAG_ROOT, "Creates a weapon/melee spawn class. If it not possible creates a single weapon/melee");
	// Usages: <IsSpawnClass> [SpawnMelees]
	RegAdminCmd("sm_l4d2wep_spawn_all", CommandSpawnItems, ADMFLAG_ROOT, "Spawns items with 0.5s interval");
	RegAdminCmd("sm_l4d2wep_eq", CommandEquip, ADMFLAG_ROOT, "Creates and equip a single weapon/melee to player slot");
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
	int i, iEnts = GetEntityCount(), iCount, iWepID;
	char sClassName[L4DWEP_NAME_LEN];
	LogMessage("IDENTIFY_SPAWN");
	iCount = 0;
	for (i = MaxClients+1; i <= iEnts; i++){
		if (IsValidEntity(i) && (iWepID = L4D2Wep_Identify(i, IDENTIFY_SPAWN)) != WEPID_NONE){
			GetEntityClassname(i, sClassName, sizeof(sClassName));
			LogMessage("%d ent %d, ID %d, Class '%s', Name '%s'", ++iCount, i, iWepID, sClassName, L4D2Wep_GetNameByID(iWepID));
		}
	}
	LogMessage("IDENTIFY_SINGLE");
	iCount = 0;
	for (i = MaxClients+1; i <= iEnts; i++){
		if (IsValidEntity(i) && (iWepID = L4D2Wep_Identify(i, IDENTIFY_SINGLE)) != WEPID_NONE){
			GetEntityClassname(i, sClassName, sizeof(sClassName));
			LogMessage("%d ent %d, ID %d, Class '%s', Name '%s'", ++iCount, i, iWepID, sClassName, L4D2Wep_GetNameByID(iWepID));
		}
	}
	LogMessage("IDENTIFY_HOLD");
	iCount = 0;
	for (i = MaxClients+1; i <= iEnts; i++){
		if (IsValidEntity(i) && (iWepID = L4D2Wep_Identify(i, IDENTIFY_HOLD)) != WEPID_NONE){
			GetEntityClassname(i, sClassName, sizeof(sClassName));
			LogMessage("%d ent %d, ID %d, Class '%s', Name '%s'", ++iCount, i, iWepID, sClassName, L4D2Wep_GetNameByID(iWepID));
		}
	}
	LogMessage("IDENTIFY ALL");
	iCount = 0;
	for (i = MaxClients+1; i <= iEnts; i++){
		if (IsValidEntity(i) && (iWepID = L4D2Wep_Identify(i, IDENTIFY_ALL)) != WEPID_NONE){
			GetEntityClassname(i, sClassName, sizeof(sClassName));
			LogMessage("%d ent %d, ID %d, Class '%s', Name '%s'", ++iCount, i, iWepID, sClassName, L4D2Wep_GetNameByID(iWepID));
		}
	}
}
// weapon_melee_spawn, weapon_item_spawn
public Action CommandIdentifyMelees(int args)
{
	int i, iEnts = GetEntityCount(), iCount, iMeleeID;
	char sClassName[L4DWEP_NAME_LEN];
	LogMessage("IDENTIFY_SPAWN");
	for (i = MaxClients+1; i <= iEnts; i++){
		if (IsValidEntity(i) && (iMeleeID = L4D2Wep_IdentifyMelee(i, IDENTIFY_SPAWN)) != MELEEID_NONE){
			GetEntityClassname(i, sClassName, sizeof(sClassName));
			LogMessage("%d ent %d, ID %d, Class '%s', Name '%s'", ++iCount, i, iMeleeID, sClassName, L4D2Wep_GetMeleeNameByID(iMeleeID));
		}
	}
	LogMessage("IDENTIFY_SINGLE");
	iCount = 0;
	for (i = MaxClients+1; i <= iEnts; i++){
		if (IsValidEntity(i) && (iMeleeID = L4D2Wep_IdentifyMelee(i, IDENTIFY_SINGLE)) != MELEEID_NONE){
			GetEntityClassname(i, sClassName, sizeof(sClassName));
			LogMessage("%d ent %d, ID %d, Class '%s', Name '%s'", ++iCount, i, iMeleeID, sClassName, L4D2Wep_GetMeleeNameByID(iMeleeID));
		}
	}
	LogMessage("IDENTIFY_HOLD");
	iCount = 0;
	for (i = MaxClients+1; i <= iEnts; i++){
		if (IsValidEntity(i) && (iMeleeID = L4D2Wep_IdentifyMelee(i, IDENTIFY_HOLD)) != MELEEID_NONE){
			GetEntityClassname(i, sClassName, sizeof(sClassName));
			LogMessage("%d ent %d, ID %d, Class '%s', Name '%s'", ++iCount, i, iMeleeID, sClassName, L4D2Wep_GetMeleeNameByID(iMeleeID));
		}
	}
	LogMessage("IDENTIFY_ALL");
	iCount = 0;
	for (i = MaxClients+1; i <= iEnts; i++){
		if (IsValidEntity(i) && (iMeleeID = L4D2Wep_IdentifyMelee(i, IDENTIFY_ALL)) != MELEEID_NONE){
			GetEntityClassname(i, sClassName, sizeof(sClassName));
			LogMessage("%d ent %d, ID %d, Class '%s', Name '%s'", ++iCount, i, iMeleeID, sClassName, L4D2Wep_GetMeleeNameByID(iMeleeID));
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

			if (!L4D2Wep_IsValidItemAndID(iID, Type)){
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

			if (!L4D2Wep_IsValidItemAndID(iID, Type)){
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

			if (!L4D2Wep_IsValidItemAndID(iID, Type)){
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

public Action CommandEquip(int client, int args)
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

			if (!L4D2Wep_IsValidItemAndID(iID, Type)){
				ReplyToCommand(client, "Unknown weapon name or bad ID!");
				return Plugin_Handled;
			}

			float vOrigin[3];
			GetClientAbsOrigin(client, vOrigin);
			int iEnt = L4D2Wep_SpawnItem(iID, Type, vOrigin, _, _, false);

			if (iEnt == -1){
				ReplyToCommand(client, "Failed to spawn ID %d, ItemType %d, ent %d", iID, Type, iEnt);
				return Plugin_Handled;
			}

			int iItemSlot = L4D2Wep_IdentifyEquipSlot(iEnt);
			if (iItemSlot >= 0){
			
				int iCurrentWep = GetPlayerWeaponSlot(client, iItemSlot);
				if (L4D2Wep_IsValidAndEntity(iCurrentWep)){
					RemovePlayerItem(client, iCurrentWep);
					AcceptEntityInput(iCurrentWep, "Kill");
				}
				EquipPlayerWeapon(client, iEnt);
				// TODO: set player ammo
			}
			else
				ReplyToCommand(client, "Failed to equip! Item has invalid slot");
		}
		else
			ReplyToCommand(client, "Usages: <weapName> || <ID> <TYPE>");
	}
	else
		ReplyToCommand(client, "Command is not available from server console!");

	return Plugin_Handled;
}

