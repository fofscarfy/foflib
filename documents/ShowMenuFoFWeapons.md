# ShowMenuFoF Weapons

When creating custom menus you can customize the text in each field as well as set a background. This background corresponds to a weapon index, which is passed back to the server with `menuselect_fof <index>` as a client command. This means that if you create a menu with weapon backgrounds then you will need to use unique backgrounds to extract the information.

If you do not use a valid weapon index then the background will simply be a properly formatted 'X' graphic. This is useful if you want to create a menu with player selections without worrying about overlapping backgrounds.

The menu icons in chests use corresponding localization tokens. These are unnecessary and you can swap them out or use raw text instead. The ones listed below are the default associated ones.

The weapon indices here correspond to those passed in events where there is overlap.

## Table of Values
***Note***: *vmt/vtf files are located under `materials/vgui`*

| Index | Weapon | Token | vmt | vtf |
| --- | --- | --- | --- | --- |
| 18 | Sawed-Off Shotgun | `#Item13b` | `sawed_shotgun.vmt` | `sawed_shotgun.vtf` |
| 19 | Right-Handed | `#Item41` | `accuracy_right.vmt` | `accuracy_right.vtf` |
| 20 | Left-Handed | `#Item42` | `accuracy_left.vmt` | `accuracy_left.vtf` |
| 21 | Fanning | `#Item44` | `accuracy_fan.vmt` | `accuracy_fanning.vtf` |
| 22 | Ambidextrous | `#Item43` | `accuracy_ambi.vmt` | `accuracy_ambi.vtf` |
| 23 | Gun Throw | `#Item14` | `gun_throw.vmt` | `gun_throw.vtf` |
| 24 | Jumpmaster | `#Item23e` | `walljump.vmt` | `walljump.vtf` |
| 25 | Slide | `#Item23d` | `slide.vmt` | `slide.vtf` |
| 26 | Heavy Loads | `#Item25` | `heavyload.vmt` | `heavyload.vtf` |
| 27 | Brass Knuckles | `#Item34b` | `brass_knuckles.vmt` | `brass_knuckles.vtf` |
| 28 | Knives | `#Item1b` | `knife.vmt` | `knife.vtf` |
| 29 | Boots | `#Item34` | `boots.vmt` | `boots.vtf` |
| 30 | Deringer | `#Item3b` | `deringer.vmt` | `deringer.vtf` |
| 31 | Dynamite | `#Item0` | `dynamite.vmt` | `dynamite.vtf` |
| 32 | Heavy Loads | `#Item25` | `heavyload.vmt` | `heavyload.vtf` |
| 33 | Volcanic | `#ItemVolcanic` | `volcanic.vmt` | `volcanic.vtf` |
| 34 | Colt Navy | `#Item2` | `coltnavy.vmt` | `coltnavy.vtf` |
| 35 | Hatchet | `#Item19` | `axe.vmt` | `axe.vtf` |
| 36 | Bow | `#Item11` | `bow.vmt` | `bow.vtf` |
| 37 | Sawed-Off Shotgun | `#Item13b` | `sawed_shotgun.vmt` | `sawed_shotgun.vtf` |
| 38 | Hammerless | `#Item3` | `hammerless.vmt` | `hammerless.vtf` |
| 39 | Remington Army | `#Rem_Army` | `nma.vmt` | `nma.vtf` |
| 40 | Mare's Leg | `#MaresLeg` | `maresleg.vmt` | `maresleg.vtf` |
| 41 | Schofield | `#Item31a` | `schofield.vmt` | `schofield.vtf` |
| 42 | Smith Carbine | `#Item5` | `carbine.vmt` | `carbine.vtf` |
| 43 | Peacemaker | `#Item21` | `peacemaker.vmt` | `peacemaker.vtf` |
| 44 | Black Bow | `#Item11b` | `bow_black.vmt` | `bow_black.vtf` |
| 45 | Yellowboy | `#Item4a` (henry=`#Item4`) | `henry.vmt` | `henry.vtf` |
| 46 | Coachgun | `#Item6` | `coachgun.vmt` | `coachgun.vtf` |
| 47 | Spencer Carbine | `#Item18` | `spencer.vmt` | `spencer.vtf` |
| 48 | Machete | `#Item26` | `machete.vmt` | `machete.vtf` |
| 49 | Pump Shotgun | `#Item30` | `shotgun.vmt` | `shotgun.vtf` |
| 50 | Black Dynamite | `#Item0b` | `dynamite_black.vmt` | `dynamite_black.vtf` |
| 51 | Sharps Rifle | `#Item12` | `sharps.vmt` | `sharps.vtf` |
| 52 | Colt Walker | `#Item32` | `walker.vmt` | `walker.vtf` |
| 53 | Dynamite Belt | `#Item17` | `dynamite_belt.vmt` | `dynamite_belt.vtf` |
| 54 | Portable Whiskey | `#Item15` | `whiskey.vmt` | `whiskey.vtf` |
| 55 | Bow | `#Item11` | `bow.vmt` | `bow.vtf` |
