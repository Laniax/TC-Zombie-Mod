UPDATE `creature_template` SET `faction_a`=233, `faction_h`=233, `exp`=2, `scale`=0.87, `AIName`= '', `ScriptName`= 'npc_zombie_turret', `unit_flags`=33554946, `flags_extra`=2 WHERE `entry` IN (792134,792135);
DELETE FROM `smart_scripts` WHERE `entryorguid`=792134;
