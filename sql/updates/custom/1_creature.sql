/*
43953 - Retch! - Shoots a green splash a-like missile towards your target. Maybe we can use this to make zombies cast this at the fort in some late phase?
63027 - Proximity Mine - Triggers a missile that throws a bunch of mines around the player and spawns one on the ground. Might be cool to script this one as well. (Explosion: 66351)
43050 - Gnomsih Flame Turret! - Might be nice for a phase vehicle that shoots fire
x                               - And how about one that freezes all zombies?
57609 - Fire Cannon - Blows up a spawnpoint (Explosion: 60855)
How about placing a bowel of brain to attract them which explodes? (Bowels and Brains Bowl)
60861 - Airship Explodes With Fireball - Might be fun for another turret. It shoots a fireball and then makes an explosion. Maybe with invisible dummies it shoots on and all mobs in few yards around will die?
*/

-- Creatures
UPDATE `creature_template` SET `ScriptName`='npc_proximity_mine' WHERE `entry`=34362; -- Proximity Mine
DELETE FROM `creature_template` WHERE `entry` IN (792130,792131,792132,792133,792134,792135,792136,792137,792138,792139);
INSERT INTO `creature_template` (`entry`,`difficulty_entry_1`,`difficulty_entry_2`,`difficulty_entry_3`,`KillCredit1`,`KillCredit2`,`modelid1`,`modelid2`,`modelid3`,`modelid4`,`name`,`subname`,`IconName`,`gossip_menu_id`,`minlevel`,`maxlevel`,`exp`,`faction_A`,`faction_H`,`npcflag`,`speed_walk`,`speed_run`,`scale`,`rank`,`mindmg`,`maxdmg`,`dmgschool`,`attackpower`,`dmg_multiplier`,`baseattacktime`,`rangeattacktime`,`unit_class`,`unit_flags`,`dynamicflags`,`family`,`trainer_type`,`trainer_spell`,`trainer_class`,`trainer_race`,`minrangedmg`,`maxrangedmg`,`rangedattackpower`,`type`,`type_flags`,`lootid`,`pickpocketloot`,`skinloot`,`resistance1`,`resistance2`,`resistance3`,`resistance4`,`resistance5`,`resistance6`,`spell1`,`spell2`,`spell3`,`spell4`,`spell5`,`spell6`,`spell7`,`spell8`,`PetSpellDataId`,`VehicleId`,`mingold`,`maxgold`,`AIName`,`MovementType`,`InhabitType`,`Health_mod`,`Mana_mod`,`Armor_mod`,`RacialLeader`,`questItem1`,`questItem2`,`questItem3`,`questItem4`,`questItem5`,`questItem6`,`movementId`,`RegenHealth`,`equipment_id`,`mechanic_immune_mask`,`flags_extra`,`ScriptName`,`WDBVerified`) VALUES
(792130,0,0,0,0,0,26942,0,0,0,'Zombie','','',0,80,80,2,233,233,0,1,1.14286,1,0,252,357,0,304,1,2000,0,1,0,8,0,0,0,0,0,215,320,44,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,'',0,3,2,1,1,0,0,0,0,0,0,0,88,1,0,0,0,'npc_zombie',1),
(792131,0,0,0,0,0,17200,0,0,0,'Zombie Kill Counter','Invisible for players','',0,80,80,1,35,35,0,1,1.14286,1,0,200,281,0,278,1,2000,0,1,33554432,8,0,0,0,0,0,166,246,35,10,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,'',0,3,1,1,1,0,0,0,0,0,0,0,0,1,0,0,130,'npc_zombie_kill_counter',1),
(792132,0,0,0,0,0,17200,0,0,0,'Zombie Spawner','Invisible for players','',0,80,80,1,35,35,0,1,1.14286,1,0,200,281,0,278,1,2000,0,1,33554432,8,0,0,0,0,0,166,246,35,10,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,'',0,3,1,1,1,0,0,0,0,0,0,0,0,1,0,0,130,'npc_zombie_spawner',1),
(792133,0,0,0,0,0,17200,0,0,0,'Zombie Spawnpoint','Invisible for players','',0,80,80,1,35,35,0,1,1.14286,1,0,200,281,0,278,1,2000,0,1,33554432,8,0,0,0,0,0,166,246,35,10,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,'',0,3,1,1,1,0,0,0,0,0,0,0,0,1,0,0,130,'npc_zombie_spawnpoint',1),
(792134,0,0,0,0,0,17200,0,0,0,'Zombie Barricade Trigger','Invisible for players','',0,80,80,1,35,35,0,1,1.14286,1,0,200,281,0,278,1,2000,0,1,33554432,8,0,0,0,0,0,166,246,35,10,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,'',0,3,1,1,1,0,0,0,0,0,0,0,0,1,0,0,130,'npc_zombie_barricade_trigger',1),
(792135,0,0,0,0,0,27754,0,0,0,'Zombie Turret','','',0,80,80,2,233,233,0,1,1.14286,0.87,0,2,2,0,24,1,2000,0,1,33554946,8,0,0,0,0,0,1,1,0,9,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,'SmartAI',0,3,26.7451,0.001361,1,0,0,0,0,0,0,0,0,1,0,0,2,'npc_zombie_turret',1),
(792136,0,0,0,0,0,28235,0,0,0,'Zombie Turret','','',0,80,80,0,35,35,0,1,1.14286,1,0,2,2,0,24,1,2000,0,1,0,8,0,0,0,0,0,1,1,0,9,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,'',0,3,26.7451,0.001361,1,0,0,0,0,0,0,0,0,1,0,0,0,'npc_zombie_turret',1),
(792137,0,0,0,0,0,6977,0,0,0,'Bomb Bot','','',0,80,80,2,35,35,0,1,1.14286,1,0,422,586,0,642,7.5,2000,0,1,0,8,0,0,0,0,0,345,509,103,9,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,'',0,3,1.5873,1,1,0,0,0,0,0,0,0,144,1,0,0,0,'npc_bomb_bot',1),
(792138,0,0,0,0,0,3233,0,0,0,'Cockroach','','',0,1,1,0,2049,2049,0,1,1.14286,1,0,2,2,0,24,1,2000,0,1,0,8,0,0,0,0,0,1,1,0,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,'',0,3,0.1,1,1,0,0,0,0,0,0,0,100,1,0,0,0,'',1),
(792139,0,0,0,0,0,3233,0,0,0,'Taruk','Zombie Event','',65533,80,80,1,35,35,1,1,1.14286,2,0,200,281,0,278,1,2000,0,1,514,8,0,0,0,0,0,166,246,35,10,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,'',0,3,1,1,1,0,0,0,0,0,0,0,0,1,0,0,2,'custom_mr_asshat',1);

-- Spawn the creatures
DELETE FROM `creature` WHERE `id` IN (792133);
INSERT INTO `creature` (`id`,`map`,`spawnMask`,`phaseMask`,`equipment_id`,`position_x`,`position_y`,`position_z`,`orientation`,`spawntimesecs`,`spawndist`,`currentwaypoint`,`curhealth`,`curmana`,`MovementType`,`npcflag`,`unit_flags`,`dynamicflags`) VALUES
(792133,289,1,1,0,136.054184,201.700638,95.039246,4.680336,600,0,0,300,0,0,0,0,0),
(792133,289,1,1,0,156.154419,201.463287,98.452942,4.246449,600,0,0,300,0,0,0,0,0),
(792133,289,1,1,0,121.992653,200.666763,98.161942,4.664613,600,0,0,300,0,0,0,0,0),
(792133,289,1,1,0,107.410492,199.340973,96.184494,4.821693,600,0,0,300,0,0,0,0,0),
(792133,289,1,1,0,107.863098,133.579697,97.949394,1.482956,600,0,0,300,0,0,0,0,0),
(792133,289,1,1,0,123.680313,132.347824,97.240753,1.570144,600,0,0,300,0,0,0,0,0),
(792133,289,1,1,0,138.114655,132.511612,97.275185,1.542667,600,0,0,300,0,0,0,0,0),
(792133,289,1,1,0,158.301880,134.723984,99.867416,1.750798,600,0,0,300,0,0,0,0,0);

-- Gameobjects
DELETE FROM `gameobject_template` WHERE `entry` IN (293610,293611);
INSERT INTO `gameobject_template` (`entry`,`type`,`displayId`,`name`,`IconName`,`castBarCaption`,`unk1`,`faction`,`flags`,`size`,`questItem1`,`questItem2`,`questItem3`,`questItem4`,`questItem5`,`questItem6`,`data0`,`data1`,`data2`,`data3`,`data4`,`data5`,`data6`,`data7`,`data8`,`data9`,`data10`,`data11`,`data12`,`data13`,`data14`,`data15`,`data16`,`data17`,`data18`,`data19`,`data20`,`data21`,`data22`,`data23`,`AIName`,`ScriptName`,`WDBVerified`) VALUES
(293610,5,8477,'Zombie Barricade','','','',0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,'','',1),
(293611,22,166,'Repair Barricade','','','',0,0,1,0,0,0,0,0,0,68077,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,'','',1);

-- Conditions for spells to hit Barricades
/*DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=13 AND `SourceEntry` IN (7670,42880,42904);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`,`SourceGroup`,`SourceEntry`,`ElseGroup`,`ConditionTypeOrReference`,`ConditionValue1`,`ConditionValue2`,`ConditionValue3`,`ErrorTextId`,`ScriptName`,`Comment`) VALUES
(13,0,7670,0,29,792130,3,0,0,'',"Explode targets all Zombies within 3 yards");
(13,0,42880,0,18,1,792130,0,0,'',NULL),
(13,0,42904,0,18,1,792130,0,0,'',NULL);*/

-- Scriptname for repairing broken barricade spell
DELETE FROM `spell_script_names` WHERE `ScriptName` IN ('spell_repair_channel','spell_zombie_rapid_fire');
INSERT INTO `spell_script_names` (`spell_id`,`ScriptName`) VALUES
(68077,'spell_repair_channel'),(71250,'spell_zombie_rapid_fire');

-- Explosion Visual triggers Knockback
DELETE FROM `spell_linked_spell` WHERE `spell_trigger`=71495;
INSERT INTO `spell_linked_spell` (`spell_trigger`,`spell_effect`,`type`,`comment`) VALUES
(71495,40191,0,"Explosion Visual - Legion Ring Knockback");

-- Add arrow tag to the Triggers - Infected visual to Cockroaches
/*DELETE FROM `creature_template_addon` WHERE `entry` IN (792133,792138);
INSERT INTO `creature_template_addon` (`entry`,`mount`,`bytes1`,`bytes2`,`emote`,`auras`) VALUES
(792133,0,0,0,0,'20374'),(792138,0,0,0,0,'49702');*/