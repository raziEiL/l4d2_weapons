# l4d2_weapons
**Status: in development**  
The API for SourceMod plugin developers that provides stock of useful weapons function. Basically here the reworked version of **weapons.inc**. The include file was written specifically for **Left 4 Dead 2** under **SourceMod** platform (SourcePawn).

## Features:
 - SourceMod 1.7 syntax.
 - Weapons/Melees identification.
 - Provides weapons/melees ID, names, models etc.

## API structure
Since the `weapon_melee` class contains its own melees, the weapons code has been logically divided into two parts: **WEAPON** and **MELEE**. To make the code more flexible and avoid to use `view_as` often, **WEAPON** and **MELEE** enums don't have a tag! Therefore, you must understand what value you are passing to the function. Both parts have functions for checking for garbage value and identify their type of weapon. The **ITEM** struct is needed to determine to which type (**WEAPON** / **MELEE**) an entity or weapon name belongs.

## Usages:
Identify entity or weapon name as `ItemType` and detect weapon type (**WEAPON** / **MELEE**).  
When you got an `ItemType` you can do following things:
 - Grab info about: model, weapon name, weapon slot index, check for spawn class
 - Create any weapons as weapon_ or _spawn class (Fixed pos/angles)
 - Create any melees as weapon_melee or weapon_melee_spawn class (Fixed pos/angles)
 - Give a weapon/melee to player (based on give cmd)
 - Gets a weapon max ammo (based on convars)

## Note:
**l4d2_weapons** does not provide a way to unlock weapons, for this goal you may use any known plugins or extensions. However, if you unlock any vanilla weapons (like a `knife`) the **l4d2_weapons** will detect it and support.

If you have custom weapons on your server and want to **l4d2_weapons** support it you have to edit the code by yourself (look for line `custom melee` for examples)

## Examples:
**l4d2_weapons_test.sp** plugin provides examples of how this API can be used.

## Credits:
 - Forked from [MatthewClair/sourcemod-plugins](https://github.com/MatthewClair/sourcemod-plugins)
 - Thanks [@Electr0](https://forums.alliedmods.net/member.php?u=152668) for method to detect valid melees via string table.
 
## Donation
My cat wants a new toy! I try to make quality and beautiful toys for my beloved cat. I create toys in my spare time but sometimes I can step into a tangle and get confused! Oops! It takes time to get out! When the toy is ready I give it to the cat, the GitHub cat and the community can also play with it. So if you enjoy my toys and want to thank me for my work, you can send any amount. All money will be spent on milk! [Donate :feet:](https://www.paypal.me/razicat)
