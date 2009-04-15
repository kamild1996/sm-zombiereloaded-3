/*
 * ============================================================================
 *
 *   Zombie:Reloaded
 *
 *   File:        classevents.inc
 *   Description: Functions for handling class related events.
 *   Author:      Richard Helgeby
 *
 * ============================================================================
 */

/* ------------------------------------
 *
 * GAME EVENTS
 *
 * ------------------------------------
 */

/**
 * To be called when a client connect to the server.
 * (OnClientPutInServer)
 */
ClassClientInit(client)
{
    if (!IsFakeClient(client))
    {
        // Set default class indexes on the player.
        ClassClientSetDefaultIndexes(client);
    }
}

ClassOnClientDisconnect(client)
{
    // Stop timers related to class attributes.
    ClassOverlayStop(client);
}

ClassOnClientSpawn(client)
{
    new bool:randomclass = GetConVarBool(gCvars[CVAR_CLASSES_RANDOM]);
    new bool:showmenu = GetConVarBool(gCvars[CVAR_CLASSES_SPAWN]);
    decl String:steamid[16];
    decl String:classname[64];
    
    if (showmenu && !randomclass)
    {
        //ClassMenu(client);
    }
    
    // Assign random classes if enabled.
    GetClientAuthString(client, steamid, sizeof(steamid));
    if (StrEqual(steamid, "BOT") || randomclass)
    {
        // Get player's team
        new teamid = GetClientTeam(client);
        
        // If the first zombie spawned, and the player is on the terrorist team, then
        // find a random zombie class, otherwise find a human class.
        if (zombieSpawned && teamid == CS_TEAM_T)
        {
            new classindex = ClassGetRandomClass(ZR_CLASS_TEAM_ZOMBIES);
            ClassSelected[client][ZR_CLASS_TEAM_ZOMBIES] = classindex;
            ClassGetName(client, classname, sizeof(classname));
        }
        else
        {
            new classindex = ClassGetRandomClass(ZR_CLASS_TEAM_HUMANS);
            ClassSelected[client][ZR_CLASS_TEAM_HUMANS] = classindex;
            ClassGetName(client, classname, sizeof(classname));
        }
        
        ZR_PrintToChat(client, "Auto-assign", classname);
    }
}

ClassOnClientDeath(client)
{
    ClassHealthRegenStop(client);
    ClassOverlayStop(client);
}

ClassOnClientInfected(client, bool:motherzombie = false)
{
    new classindex = ClassGetActiveIndex(client);
    
    // Update the players cache with zombie attributes.
    ClassReloadPlayerCache(client, classindex);
    
    // Apply the new attributes.
    ClassApplyAttributes(client, motherzombie);
}

ClassOnRoundStart()
{
    
}


/* ------------------------------------
 *
 * PLAYER COMMANDS
 *
 * ------------------------------------
 */
