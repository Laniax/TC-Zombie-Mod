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