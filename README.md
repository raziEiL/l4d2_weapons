**Description:**
https://github.com/raziEiL/l4d2_weapons

Fork by Dragokas to support multilingual translation.

**Dependency:**
 - Localizer.inc: https://github.com/dragokas/SM-Localizer/

**Added new stocks:**

```
stock bool L4D2Wep_GetTitle(int wepID, char[] buffer, int maxlength, int client)
stock char[] L4D2Wep_GetTitleEx(int client, int WepID)
stock bool L4D2Wep_GetMeleeTitle(int meleeID, char[] buffer, int maxlength, int client)
stock char[] L4D2Wep_GetMeleeTitleEx(int client, int MeleeID)

```

New test command to verify translation:

```
sm_l4d2weploc
```
