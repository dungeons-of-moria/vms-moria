  value

	wizard1 	:= false;
	wizard2         := false;
	{ God and Wizard passwords: God is for debugging purposes only. }
	wdata           :=(
		(011065,87,36,139,197,12,22,228,250,54,148,136,163),
		(122565,181,170,214,182,212,8,195,173,86,76,252,124)
			  );
	used_line       := (22 of false);
	death           := false;
	total_winner    := false;
	stat_column     := 1;
	turn            := 0;
	pclass          := 0;
	print_stat      := 0;

	{ For the testing of bits				-RAK-	}
	bit_array       := (
		%X'00000001',   { Bit number 1  }
		%X'00000002',   { Bit number 2  }
		%X'00000004',   { Bit number 3  }
		%X'00000008',   { Bit number 4  }
		%X'00000010',   { Bit number 5  }
		%X'00000020',   { Bit number 6  }
		%X'00000040',   { Bit number 7  }
		%X'00000080',   { Bit number 8  }
		%X'00000100',   { Bit number 9  }
		%X'00000200',   { Bit number 10 }
		%X'00000400',   { Bit number 11 }
		%X'00000800',   { Bit number 12 }
		%X'00001000',   { Bit number 13 }
		%X'00002000',   { Bit number 14 }
		%X'00004000',   { Bit number 15 }
		%X'00008000',   { Bit number 16 }
		%X'00010000',   { Bit number 17 }
		%X'00020000',   { Bit number 18 }
		%X'00040000',   { Bit number 19 }
		%X'00080000',   { Bit number 20 }
		%X'00100000',   { Bit number 21 }
		%X'00200000',   { Bit number 22 }
		%X'00400000',   { Bit number 23 }
		%X'00800000',   { Bit number 24 }
		%X'01000000',   { Bit number 25 }
		%X'02000000',   { Bit number 26 }
		%X'04000000',   { Bit number 27 }
		%X'08000000',   { Bit number 28 }
		%X'10000000',   { Bit number 29 }
		%X'20000000',   { Bit number 30 }
		%X'40000000',   { Bit number 31 }
		%X'80000000'    { Bit number 32 }
			);

	{ Operating hours for Moria				-RAK-	}
	{       X = Open; . = Closed                                    }
	days    :=(
		'SUN:XXXXXXXXXXXXXXXXXXXXXXXX|',
		'MON:XXXXXXXX.........XXXXXXX|',
		'TUE:XXXXXXXX.........XXXXXXX|',
		'WED:XXXXXXXX.........XXXXXXX|',
		'THU:XXXXXXXX.........XXXXXXX|',
		'FRI:XXXXXXXX.........XXXXXXX|',
		'SAT:XXXXXXXXXXXXXXXXXXXXXXXX|'
		   );

	closing_flag := 0;

	{ Values for floor types					}
	blank_floor:= (0,0,0,false,false,false,false);
	dopen_floor     := (  1,true ); { Dark open floor       }
	lopen_floor     := (  2,true ); { Light open floor      }
	corr_floor1     := (  4,true ); { Corridor open floor   }
	corr_floor2     := (  5,true ); { Room junction marker  }
	corr_floor3     := (  6,true ); { Door type floor       }
	corr_floor4     := (  7,false); { Secret door type floor}
	{ Floor values 8 and 9 are used in generate             }
	rock_wall1      := ( 10,false); { Granite rock wall     }
	rock_wall2      := ( 11,false); { Magma rock wall       }
	rock_wall3      := ( 12,false); { Quartz rock wall      }
	boundry_wall    := ( 15,false); { Indestructable wall   }

	{ Player record is special					}
	py              :=      (
	(' ',' ',' ',' ',' ',
	 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	 ('','','','','')),
	(0,0,0,0,0,0,0,0,0,0,0,0),
	(0,0,0,0,0,7500,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	 false,false,false,false,false,false,false,false,false,
	 false,false,false,false,false,false,false,false,false)
				);

	bare_hands	:= '1d1';

	acc_exp 	:= 0.0;

	{ Each character class has a list of spells they can use.  In	}
	{ cases the entire array is blank                               }

	magic_spell	:=	(	(		{ Warrior	}
 (''                            ,99,99,  0,  0,false),
 (''                            ,99,99,  0,  0,false),
 (''                            ,99,99,  0,  0,false),
 (''                            ,99,99,  0,  0,false),
 (''                            ,99,99,  0,  0,false),
 (''                            ,99,99,  0,  0,false),
 (''                            ,99,99,  0,  0,false),
 (''                            ,99,99,  0,  0,false),
 (''                            ,99,99,  0,  0,false),
 (''                            ,99,99,  0,  0,false),
 (''                            ,99,99,  0,  0,false),
 (''                            ,99,99,  0,  0,false),
 (''                            ,99,99,  0,  0,false),
 (''                            ,99,99,  0,  0,false),
 (''                            ,99,99,  0,  0,false),
 (''                            ,99,99,  0,  0,false),
 (''                            ,99,99,  0,  0,false),
 (''                            ,99,99,  0,  0,false),
 (''                            ,99,99,  0,  0,false),
 (''                            ,99,99,  0,  0,false),
 (''                            ,99,99,  0,  0,false),
 (''                            ,99,99,  0,  0,false),
 (''                            ,99,99,  0,  0,false),
 (''                            ,99,99,  0,  0,false),
 (''                            ,99,99,  0,  0,false),
 (''                            ,99,99,  0,  0,false),
 (''                            ,99,99,  0,  0,false),
 (''                            ,99,99,  0,  0,false),
 (''                            ,99,99,  0,  0,false),
 (''                            ,99,99,  0,  0,false),
 (''                            ,99,99,  0,  0,false)
					),
					(               { Mage          }
 ('Magic Missile'               , 1, 1,  5, 22,false),
 ('Detect Monsters'             , 1, 1,  5, 23,false),
 ('Phase Door'                  , 1, 2,  4, 24,false),
 ('Light Area'                  , 1, 2,  4, 26,false),
 ('Cure Light Wounds'           , 3, 3,  8, 25,false),
 ('Find Hidden Traps/Doors'     , 3, 3,  6, 55,false),
 ('Stinking Cloud'              , 3, 4,  8, 27,false),
 ('Confusion'                   , 3, 4,  6, 30,false),
 ('Lightning Bolt'              , 5, 4, 25, 30,false),
 ('Trap/Door Destruction'       , 5, 5, 30, 30,false),
 ('Sleep I'                     , 5, 5, 20, 30,false),
 ('Cure Poison'                 , 5, 5, 25, 35,false),
 ('Teleport Self'               , 7, 6, 35, 35,false),
 ('Remove Curse'                , 7, 6, 40, 50,false),
 ('Frost Bolt'                  , 7, 6, 50, 40,false),
 ('Turn Stone to Mud'           , 9, 7, 75, 44,false),
 ('Create Food'                 , 9, 7, 75, 45,false),
 ('Recharge Item I'             , 9, 7, 90, 75,false),
 ('Sleep II'                    , 9, 7, 75, 45,false),
 ('Polymorph Other'             ,11, 7,100, 45,false),
 ('Identify'                    ,11, 7, 75, 99,false),
 ('Sleep III'                   ,13, 7, 90, 50,false),
 ('Fire Bolt'                   ,15, 9,100, 50,false),
 ('Slow Monster'                ,17, 9,125, 50,false),
 ('Frost Ball'                  ,19,12,150, 55,false),
 ('Recharge Item II'            ,21,12,175, 90,false),
 ('Teleport Other'              ,23,12,200, 60,false),
 ('Haste Self'                  ,25,12,250, 65,false),
 ('Fire Ball'                   ,29,18,350, 65,false),
 ('Word of Destruction'         ,33,21,500, 80,false),
 ('Genocide'                    ,37,25,800, 95,false)
					),
					(               { Priest        }
 ('Detect Evil'                 , 1, 1,  3, 10,false),
 ('Cure Light Wounds'           , 1, 2,  4, 15,false),
 ('Bless'                       , 1, 2,  3, 20,false),
 ('Remove Fear'                 , 1, 2,  3, 25,false),
 ('Call Light'                  , 3, 2,  6, 25,false),
 ('Find Traps'                  , 3, 3,  8, 27,false),
 ('Detect Doors/Stairs'         , 3, 3,  8, 27,false),
 ('Slow Poison'                 , 3, 3, 10, 28,false),
 ('Blind Creature'              , 5, 4, 16, 29,false),
 ('Portal'                      , 5, 4, 20, 30,false),
 ('Cure Medium Wounds'          , 5, 4, 20, 32,false),
 ('Chant'                       , 5, 5, 20, 34,false),
 ('Sanctuary'                   , 7, 5, 30, 36,false),
 ('Create Food'                 , 7, 5, 30, 38,false),
 ('Remove Curse'                , 7, 6, 35, 38,false),
 ('Resist Heat and Cold'        , 7, 7, 35, 38,false),
 ('Neutralize Poison'           , 9, 6, 40, 38,false),
 ('Orb of Draining'             , 9, 7, 40, 38,false),
 ('Cure Serious Wounds'         , 9, 7, 40, 40,false),
 ('Sense Invisible'             ,11, 8, 40, 42,false),
 ('Protection from Evil'        ,11, 8, 50, 42,false),
 ('Earthquake'                  ,11, 9, 60, 55,false),
 ('Sense Surroundings'          ,13,10, 60, 45,false),
 ('Cure Critical Wounds'        ,13,11, 65, 45,false),
 ('Turn Undead'                 ,15,12, 80, 50,false),
 ('Prayer'                      ,15,14, 90, 50,false),
 ('Dispell Undead'              ,17,14,125, 55,false),
 ('Heal'                        ,21,16,150, 60,false),
 ('Dispell Evil'                ,25,20,300, 70,false),
 ('Glyph of Warding'            ,33,24,500, 90,false),
 ('Holy Word'                   ,39,32,800, 99,false)
					),
					(               { Rogue         }
 ('Magic Missile'               ,99,99,  0,  0,false),
 ('Detect Monsters'             , 5, 1,  2, 50,false),
 ('Phase Door'                  , 7, 2,  4, 55,false),
 ('Light Area'                  , 9, 3,  6, 60,false),
 ('Cure Light Wounds'           ,11, 4,  8, 65,false),
 ('Find Hidden Traps/Doors'     ,13, 5, 10, 70,false),
 ('Stinking Cloud'              ,99,99,  0,  0,false),
 ('Confusion'                   ,15, 6, 12, 75,false),
 ('Lightning Bolt'              ,99,99,  0,  0,false),
 ('Trap/Door Destruction'       ,17, 7, 15, 80,false),
 ('Sleep I'                     ,19, 8, 20, 85,false),
 ('Cure Poison'                 ,21, 9, 25, 90,false),
 ('Teleport Self'               ,99,99,  0,  0,false),
 ('Remove Curse'                ,23,10, 30, 95,false),
 ('Frost Bolt'                  ,99,99,  0,  0,false),
 ('Turn Stone to Mud'           ,99,99,  0,  0,false),
 ('Create Food'                 ,25,12, 35, 95,false),
 ('Recharge Item I'             ,27,15, 45, 99,false),
 ('Sleep II'                    ,99,99,  0,  0,false),
 ('Polymorph Other'             ,99,99,  0,  0,false),
 ('Identify'                    ,29,18, 75, 99,false),
 ('Sleep III'                   ,99,99,  0,  0,false),
 ('Fire Bolt'                   ,99,99,  0,  0,false),
 ('Slow Monster'                ,99,99,  0,  0,false),
 ('Frost Ball'                  ,99,99,  0,  0,false),
 ('Recharge Item II'            ,99,99,  0,  0,false),
 ('Teleport Other'              ,99,99,  0,  0,false),
 ('Haste Self'                  ,99,99,  0,  0,false),
 ('Fire Ball'                   ,99,99,  0,  0,false),
 ('Word of Destruction'         ,99,99,  0,  0,false),
 ('Genocide'                    ,99,99,  0,  0,false)
					),
					(               { Ranger        }
 ('Magic Missile'               , 3, 1,  6, 30,false),
 ('Detect Monsters'             , 3, 2,  6, 35,false),
 ('Phase Door'                  , 3, 2,  8, 35,false),
 ('Light Area'                  , 5, 3,  8, 35,false),
 ('Cure Light Wounds'           , 5, 3,  8, 40,false),
 ('Find Hidden Traps/Doors'     , 5, 4, 10, 45,false),
 ('Stinking Cloud'              , 7, 6, 24, 40,false),
 ('Confusion'                   , 7, 6, 20, 40,false),
 ('Lightning Bolt'              , 9, 7, 30, 40,false),
 ('Trap/Door Destruction'       , 9, 8, 30, 45,false),
 ('Sleep I'                     ,11, 8, 40, 40,false),
 ('Cure Poison'                 ,11, 9, 40, 45,false),
 ('Teleport Self'               ,13,10, 50, 45,false),
 ('Remove Curse'                ,13,11, 50, 55,false),
 ('Frost Bolt'                  ,15,12, 60, 50,false),
 ('Turn Stone to Mud'           ,15,13, 60, 50,false),
 ('Create Food'                 ,17,17, 60, 55,false),
 ('Recharge Item I'             ,17,17, 70, 90,false),
 ('Sleep II'                    ,21,17, 70, 55,false),
 ('Polymorph Other'             ,21,19, 70, 60,false),
 ('Identify'                    ,23,25, 80, 95,false),
 ('Sleep III'                   ,23,20, 80, 60,false),
 ('Fire Bolt'                   ,25,20, 80, 60,false),
 ('Slow Monster'                ,25,21, 80, 65,false),
 ('Frost Ball'                  ,27,21, 90, 65,false),
 ('Recharge Item II'            ,29,23, 90, 95,false),
 ('Teleport Other'              ,31,25,100, 70,false),
 ('Haste Self'                  ,33,25,150, 75,false),
 ('Fire Ball'                   ,35,25,200, 80,false),
 ('Word of Destruction'         ,37,30,500, 95,false),
 (''                            ,99,99,  0,  0,false)
					),
					(               { Paladin       }
 ('Detect Evil'                 , 1, 1,  4, 30,false),
 ('Cure Light Wounds'           , 2, 2,  8, 35,false),
 ('Bless'                       , 3, 3, 12, 35,false),
 ('Remove Fear'                 , 5, 3, 20, 35,false),
 ('Call Light'                  , 5, 4, 20, 35,false),
 ('Find Traps'                  , 7, 5, 25, 40,false),
 ('Detect Doors/Stairs'         , 7, 5, 25, 40,false),
 ('Slow Poison'                 , 9, 7, 30, 40,false),
 ('Blind Creature'              , 9, 7, 30, 40,false),
 ('Portal'                      , 9, 8, 30, 40,false),
 ('Cure Medium Wounds'          ,11, 9, 40, 40,false),
 ('Chant'                       ,11,10, 40, 45,false),
 ('Sanctuary'                   ,11,10, 40, 45,false),
 ('Create Food'                 ,13,10, 50, 45,false),
 ('Remove Curse'                ,13,11, 50, 45,false),
 ('Resist Heat and Cold'        ,15,13, 60, 45,false),
 ('Neutralize Poison'           ,15,15, 60, 50,false),
 ('Orb of Draining'             ,17,15, 70, 50,false),
 ('Cure Serious Wounds'         ,17,15, 70, 50,false),
 ('Sense Invisible'             ,19,15, 75, 50,false),
 ('Protection from Evil'        ,19,15, 75, 50,false),
 ('Earthquake'                  ,21,17, 80, 50,false),
 ('Sense Surroundings'          ,23,17, 80, 50,false),
 ('Cure Critical Wounds'        ,25,20, 80, 50,false),
 ('Turn Undead'                 ,27,21, 90, 50,false),
 ('Prayer'                      ,29,22, 95, 50,false),
 ('Dispell Undead'              ,31,24,100, 60,false),
 ('Heal'                        ,33,28,125, 60,false),
 ('Dispell Evil'                ,35,32,150, 70,false),
 ('Glyph of Warding'            ,37,36,200, 90,false),
 ('Holy Word'                   ,39,38,500, 95,false)
					)
			);
	{ For Create Food spell use...          }
 mush :=
('& pint~ of fine grade mush'                      ,  80, ',',%X'00000000',
 1500,      0, 308,   1,   1,   0,   0,   0,   0, '1d1'  ,  1);


	char_row	:= 0;
	char_col        := 0;

	{ Initialize player generation variables			}
	sex_type  := 'FemaleMale  ';

	{Race	STR,INT,WIS,DEX,CON,CHR,
		Ages, heights, and weights,
		Racial Bases for: dis,srh,stl,fos,bth,bthb,bsav,hitdie,
		infra, choice-classes                                   }

	race	:=	(
		('Human',     0,  0,  0,  0,  0,  0,
			 14,  6, 72,  6,180, 25, 66,  4,120, 20, 1.00,
			  0,  0,  0,  0,  0,  0,  0, 10,  0,  %B'111111'
			),
		('Half-Elf', -1,  1,  0,  1, -1,  1,
			 24, 16, 66,  6,130, 15, 62,  6,100, 10, 1.10,
			  2,  6,  1, -1,  0,  5,  3,  9,  0,  %B'111111'
			),
		('Elf',      -1,  2,  1,  1, -2,  1,
			 75, 75, 60,  4,100,  6, 54,  4, 80,  6, 1.20,
			  5,  8,  1, -2, -5, 15,  6,  8,  0,  %B'011111'
			),
		('Halfling', -2,  2,  1,  3,  1,  1,
			 21, 12, 36,  3, 60,  3, 33,  3, 50,  3, 1.10,
			 15, 12,  4, -5,-10, 20, 18,  6,  4,  %B'001011'
			),
		('Gnome',    -1,  2,  0,  2,  1, -2,
			 50, 40, 42,  3, 90,  6, 39,  3, 75,  3, 1.25,
			 10,  6,  3, -3, -8, 12, 12,  7,  3,  %B'001111'
			),
		('Dwarf',     2, -3,  1, -2,  2, -3,
			 35, 15, 48,  3,150, 10, 46,  3,120, 10, 1.20,
			  2,  7,  0,  0, 15,  0,  9,  9,  5,  %B'000101'
			),
		('Half-Orc',  2, -1,  0,  0,  1, -4,
			 11,  4, 66,  1,150,  5, 62,  1,120,  5, 1.10,
			 -3,  0, -1,  3, 12, -5, -3, 10,  3,  %B'001101'
			),
		('Half-Troll',4, -4, -2, -4,  3, -6,
			 20, 10, 96, 10,300, 50, 84,  8,260, 40, 1.20,
			 -5, -1, -2,  5, 20,-10, -8, 12,  3,  %B'000101'
			)
		);

	{ Buying and selling adjustments for character race VS store	}
	{ owner race                                                    }

	rgold_adj :=	(
			{  Hum,  HfE,  Elf,  Hal,  Gno,  Dwa,  HfO,  HfT}
{Human          }       ( 0.00, 0.05, 0.05, 0.10, 0.13, 0.15, 0.20, 0.25),
{Half-Elf       }       ( 0.10, 0.00, 0.00, 0.05, 0.10, 0.20, 0.25, 0.30),
{Elf            }       ( 0.10, 0.05, 0.00, 0.05, 0.10, 0.20, 0.25, 0.30),
{Halfling       }       ( 0.15, 0.10, 0.05,-0.05, 0.05, 0.10, 0.15, 0.30),
{Gnome          }       ( 0.15, 0.15, 0.10, 0.05,-0.05, 0.10, 0.15, 0.30),
{Dwarf          }       ( 0.15, 0.20, 0.20, 0.10, 0.10,-0.05, 0.25, 0.35),
{Half-Orc       }       ( 0.15, 0.20, 0.25, 0.15, 0.15, 0.30, 0.10, 0.15),
{Half-Troll     }       ( 0.10, 0.15, 0.15, 0.10, 0.10, 0.30, 0.10, 0.10)
			);

	{ Classes...							}
	class   :=      (
('Warrior', 0.00,  9, 25, 14, 1, 38, 68, 55,30, 5,-2,-2, 2, 2,-1, false,false),
('Mage',    0.30,  0, 30, 16, 2, 36, 34, 20,20,-5, 3, 0, 0,-2, 0, false,true ),
('Priest',  0.10,  3, 25, 16, 2, 32, 48, 35,20, 0,-3, 3,-1, 1, 2, true ,false),
('Rogue',   0.00,  6, 45, 32, 4, 16, 60, 66,30, 3, 1,-2, 3, 2,-1, false,true ),
('Ranger',  0.40,  4, 30, 24, 3, 24, 56, 72,25, 2, 2, 0, 1, 1, 1, false,true ),
('Paladin', 0.35,  6, 20, 12, 1, 38, 70, 40,25, 3,-3, 1, 0, 2, 2, true ,false)
			);

	{ Class titles for different levels				}
	player_title    :=      (
	{ Warrior       }
('Novice','Veteran(1st)','Veteran(2nd)','Veteran(3rd)','Warrior(1st)',
'Warrior(2nd)','Warrior(3rd)','Swordsman-1','Swordsman-2','Swordsman-3',
'Hero','Swashbuckler','Myrmidon','Champion-1','Champion-2','Champion-3',
'Superhero','Knight','Superior Knt','Gallant Knt','Knt Errent','Keeper',
'Protector','Defender','Warder','Guardian Knt','Chevalier','Justiciar',
'Lord (1st)','Lord (2nd)','Lord (3rd)','Lord (4th)','Lord (5th)','Lord (6th)',
'Lord (7th)','Lord (8th)','Lord (9th)','Lord Gallant','Lord Keeper',
'Lord Noble'),
	{ Mage          }
('Novice','Apprentice','Trickster-1','Trickster-2','Trickster-3','Cabalist-1',
'Cabalist-2','Cabalist-3','Visionist','Phantasmist','Shadowist','Spellbinder',
'Illusionist','Evoker (1st)','Evoker (2nd)','Evoker (3rd)','Evoker (4th)',
'Conjurer','Theurgist','Thaumaturer','Magician','Enchanter','Warlock',
'Sorcerer','Necromancer','Mage (1st)','Mage (2nd)','Mage (3rd)','Mage (4th)',
'Mage (5th)','Wizard (1st)','Wizard (2nd)','Wizard (3rd)','Wizard (4th)',
'Wizard (5th)','Wizard (6th)','Wizard (7th)','Wizard (8th)','Wizard (9th)',
'Wizard Lord'),
	{ Priests       }
('Believer','Acolyte(1st)','Acolyte(2nd)','Acolyte(3rd)','Adept (1st)',
'Adept (2nd)','Adept (3rd)','Priest (1st)','Priest (2nd)','Priest (3rd)',
'Priest (4th)','Priest (5th)','Priest (6th)','Priest (7th)','Priest (8th)',
'Priest (9th)','Curate (1st)','Curate (2nd)','Curate (3rd)','Curate (4th)',
'Curate (5th)','Curate (6th)','Curate (7th)','Curate (8th)','Curate (9th)',
'Canon (1st)','Canon (2nd)','Canon (3rd)','Canon (4th)','Canon (5th)',
'Low Lama','Lama-1','Lama-2','Lama-3','High Lama','Great Lama','Patriarch',
'High Priest','Great Priest','Noble Priest'),
	{ Rogues        }
('Apprentice','Footpad','Cutpurse','Robber','Burglar','Filcher','Sharper',
'Magsman','Common Rogue','Rogue (1st)','Rogue (2nd)','Rogue (3rd)',
'Rogue (4th)','Rogue (5th)','Rogue (6th)','Rogue (7th)','Rogue (8th)',
'Rogue (9th)','Master Rogue','Expert Rogue','Senior Rogue','Chief Rogue',
'Prime Rogue','Low Thief','Thief (1st)','Thief (2nd)','Thief (3rd)',
'Thief (4th)','Thief (5th)','Thief (6th)','Thief (7th)','Thief (8th)',
'Thief (9th)','High Thief','Master Thief','Executioner','Low Assassin',
'Assassin','HighAssassin','Guildsmaster'),
	{ Rangers       }
('Runner (1st)','Runner (2nd)','Runner (3rd)','Strider (1st)','Strider (2nd)',
'Strider (3rd)','Scout (1st)','Scout (2nd)','Scout (3rd)','Scout (4th)',
'Scout (5th)','Courser (1st)','Courser (2nd)','Courser (3rd)','Courser (4th)',
'Courser (5th)','Tracker (1st)','Tracker (2nd)','Tracker (3rd)',
'Tracker (4th)','Tracker (5th)','Tracker (6th)','Tracker (7th)',
'Tracker (8th)','Tracker (9th)','Guide (1st)','Guide (2nd)','Guide (3rd)',
'Guide (4th)','Guide (5th)','Guide (6th)','Guide (7th)','Guide (8th)',
'Guide (9th)','Pathfinder-1','Pathfinder-2','Pathfinder-3','Ranger',
'High Ranger','Ranger Lord'),
	{ Paladins      }
('Gallant','Keeper (1st)','Keeper (2nd)','Keeper (3rd)','Keeper (4th)',
'Keeper (5th)','Keeper (6th)','Keeper (7th)','Keeper (8th)','Keeper (9th)',
'Protector-1','Protector-2','Protector-3','Protector-4','Protector-5',
'Protector-6','Protector-7','Protector-8','Defender-1','Defender-2',
'Defender-3','Defender-4','Defender-5','Defender-6','Defender-7','Defender-8',
'Warder (1st)','Warder (2nd)','Warder (3rd)','Warder (4th)','Warder (5th)',
'Warder (6th)','Warder (7th)','Warder (8th)','Warder (9th)','Gauardian',
'Chevalier','Justiciar','Paladin','High Lord')
				);
	{ Base experience levels, may be adjusted up for race and/or class}
	player_exp :=   (
     10,     25,     45,     70,    100,    140,    200,    280,    380,   500,
    650,    850,   1100,   1400,   1800,   2300,   2900,   3600,   4400,  5400,
   6800,   8400,  10200,  12500,  17500,  25000,  35000,  50000,  75000,100000,
 150000, 200000, 300000, 400000, 500000, 750000,1500000,2500000,5000000,9999999
			);


	{ Background information					}
	background      :=      (
('You are the illegitimate and unacknowledged child ',           10, 1, 2,-25),
('You are the illegitimate but acknowledged child ',             20, 1, 2,-15),
('You are one of several children ',                             95, 1, 2, -5),
('You are the 1st child ',                                      100, 1, 2,  0),
('of a Serf.  ',                                                 40, 2, 3, 15),
('of a Yeoman.  ',                                               65, 2, 3, 30),
('of a Townsman.  ',                                             80, 2, 3, 40),
('of a Guildsman.  ',                                            90, 2, 3, 55),
('of a Landed Knight.  ',                                        96, 2, 3, 70),
('of a Titled Noble.  ',                                         99, 2, 3, 80),
('of a Royal Blood Line.  ',                                    100, 2, 3, 90),
('You are the black sheep of the family.  ',                     20, 3,50,-30),
('You are a credit to the family.  ',                            80, 3,50,  5),
('You are a well liked child.  ',                               100, 3,50, 10),
('Your mother was a Green-Elf.  ',                               40, 4, 1,  0),
('Your father was a Green-Elf.  ',                               75, 4, 1,  5),
('Your mother was a Grey-Elf.  ',                                90, 4, 1,  5),
('Your father was a Grey-Elf.  ',                                95, 4, 1, 10),
('Your mother was a High-Elf.  ',                                98, 4, 1, 15),
('Your father was a High-Elf.  ',                               100, 4, 1, 20),
('You are one of several children ',                             60, 7, 8,  0),
('You are the only child ',                                     100, 7, 8,  5),
('of a Green-Elf ',                                              75, 8, 9,  0),
('of a Grey-Elf ',                                               95, 8, 9,  5),
('of a High-Elf ',                                              100, 8, 9, 10),
('Ranger.  ',                                                    40, 9,54, 30),
('Archer.  ',                                                    70, 9,54, 40),
('Warrior.  ',                                                   87, 9,54, 60),
('Mage.  ',                                                      95, 9,54, 75),
('Prince.  ',                                                    99, 9,54, 90),
('King.  ',                                                     100, 9,54, 95),
('You are one of several children of a Halfling ',               85,10,11, -5),
('You are the only child of a Halfling ',                       100,10,11,  5),
('Bum.  ',                                                       20,11, 3,  5),
('Tavern Owner.  ',                                              30,11, 3, 30),
('Miller.  ',                                                    40,11, 3, 40),
('Home Owner.  ',                                                50,11, 3, 50),
('Burglar.  ',                                                   80,11, 3, 60),
('Warrior.  ',                                                   95,11, 3, 65),
('Mage.  ',                                                     100,11, 3, 75),
('Clan Elder.  ',                                               100,11, 3, 90),
('You are one of several children of a Gnome ',                  85,13,14, -5),
('You are the only child of a Gnome ',                          100,13,14,  5),
('Beggar.  ',                                                    20,14, 3,  5),
('Bragart.  ',                                                   50,14, 3, 20),
('Prankster.  ',                                                 75,14, 3, 35),
('Warrior.  ',                                                   95,14, 3, 50),
('Mage.  ',                                                     100,14, 3, 75),
('You are one of two children of a Dwarven ',                    25,16,17,-10),
('You are the only child of a Dwarven ',                        100,16,17,  0),
('Thief.  ',                                                     10,17,18, 10),
('Prison Guard.  ',                                              25,17,18, 25),
('Miner.  ',                                                     75,17,18, 40),
('Warrior.  ',                                                   90,17,18, 60),
('Priest.  ',                                                    99,17,18, 80),
('King.  ',                                                     100,17,18,100),
('You are the black sheep of the family.  ',                     15,18,57,-40),
('You are a credit to the family.  ',                            85,18,57,  0),
('You are a well liked child.  ',                               100,18,57,  5),
('Your mother was an Orc, but it is unacknowledged.  ',          25,19,20,-25),
('Your father was an Orc, but it is unacknowledged.  ',         100,19,20,-25),
('You are the adopted child ',                                  100,20, 2,  0),
('Your mother was a Cave-Troll ',                                30,22,23,-30),
('Your father was a Cave-Troll ',                                60,22,23,-25),
('Your mother was a Hill-Troll ',                                75,22,23,-20),
('Your father was a Hill-Troll ',                                90,22,23,-15),
('Your mother was a Water-Troll ',                               95,22,23,-10),
('Your father was a Water-Troll ',                              100,22,23, -5),
('Cook.  ',                                                       5,23,62, 10),
('Warrior.  ',                                                   95,23,62,  5),
('Shaman.  ',                                                    99,23,62, 15),
('Clan Chief.  ',                                               100,23,62, 30),
('You have dark brown eyes, ',                                   20,50,51,  0),
('You have brown eyes, ',                                        60,50,51,  0),
('You have hazel eyes, ',                                        70,50,51,  0),
('You have green eyes, ',                                        80,50,51,  0),
('You have blue eyes, ',                                         90,50,51,  0),
('You have blue-gray eyes, ',                                   100,50,51,  0),
('straight ',                                                    70,51,52,  0),
('wavey ',                                                       90,51,52,  0),
('curly ',                                                      100,51,52,  0),
('black hair, ',                                                 30,52,53,  0),
('brown hair, ',                                                 70,52,53,  0),
('auburn hair, ',                                                80,52,53,  0),
('red hair, ',                                                   90,52,53,  0),
('blonde hair, ',                                               100,52,53,  0),
('and a very dark complexion.',                                  10,53,-1,  0),
('and a dark complexion.',                                       30,53,-1,  0),
('and an average complexion.',                                   80,53,-1,  0),
('and a fair complexion.',                                       90,53,-1,  0),
('and a very fair complexion.',                                 100,53,-1,  0),
('You have light grey eyes, ',                                   85,54,55,  0),
('You have light blue eyes, ',                                   95,54,55,  0),
('You have light green eyes, ',                                 100,54,55,  0),
('straight ',                                                    75,55,56,  0),
('wavey ',                                                      100,55,56,  0),
('black hair, and a fair complexion.',                           75,56,-1,  0),
('brown hair, and a fair complexion.',                           85,56,-1,  0),
('blonde hair, and a fair complexion.',                          95,56,-1,  0),
('silver hair, and a fair complexion.',                         100,56,-1,  0),
('You have dark brown eyes, ',                                   99,57,58,  0),
('You have glowing red eyes, ',                                 100,57,58, 10),
('straight ',                                                    90,58,59,  0),
('wavey ',                                                      100,58,59,  0),
('black hair, ',                                                 75,59,60,  0),
('brown hair, ',                                                100,59,60,  0),
('a one foot beard, ',                                           25,60,61,  0),
('a two foot beard, ',                                           60,60,61,  1),
('a three foot beard, ',                                         90,60,61,  3),
('a four foot beard, ',                                         100,60,61,  5),
('and a dark complexion.',                                      100,61,-1,  0),
('You have slime green eyes, ',                                  60,62,63,  0),
('You have puke yellow eyes, ',                                  85,62,63,  0),
('You have blue-bloodshot eyes, ',                               99,62,63,  0),
('You have glowing red eyes, ',                                 100,62,63,  5),
('dirty ',                                                       33,63,64,  0),
('mangy ',                                                       66,63,64,  0),
('oily ',                                                       100,63,64,  0),
('sea-weed green hair, ',                                        33,64,65,  0),
('bright red hair, ',                                            66,64,65,  0),
('dark purple hair, ',                                          100,64,65,  0),
('and green ',                                                   25,65,66,  0),
('and blue ',                                                    50,65,66,  0),
('and white ',                                                   75,65,66,  0),
('and black ',                                                  100,65,66,  0),
('ulcerous skin.',                                               33,66,-1,  0),
('scabby skin.',                                                 66,66,-1,  0),
('leprous skin.',                                               100,66,-1,  0)
			);


	{ Treasure related values					}
	inven_ctr       := 0;
	inven_weight    := 0;
	missle_ctr      := 0;
	equip_ctr       := 0;
	blank_treasure  := (' ',0,' ',0,0,0,0,0,0,0,0,0,0,' ',0);

	{ Gold list (All types of gold and gems are defined here)	}
	gold_list       :=      (
('copper'                                          , 100, '$',%X'00000000',
    0,      3,   1,   0,   0,   0,   0,   0,   0, ' '    ,  1),
('copper'                                          , 100, '$',%X'00000000',
    0,      4,   2,   0,   0,   0,   0,   0,   0, ' '    ,  1),
('copper'                                          , 100, '$',%X'00000000',
    0,      5,   3,   0,   0,   0,   0,   0,   0, ' '    ,  1),
('silver'                                          , 100, '$',%X'00000000',
    0,      6,   4,   0,   0,   0,   0,   0,   0, ' '    ,  1),
('silver'                                          , 100, '$',%X'00000000',
    0,      7,   5,   0,   0,   0,   0,   0,   0, ' '    ,  1),
('silver'                                          , 100, '$',%X'00000000',
    0,      8,   6,   0,   0,   0,   0,   0,   0, ' '    ,  1),
('garnets'                                         , 100, '*',%X'00000000',
    0,      9,   7,   0,   0,   0,   0,   0,   0, ' '    ,  1),
('garnets'                                         , 100, '*',%X'00000000',
    0,     10,   8,   0,   0,   0,   0,   0,   0, ' '    ,  1),
('gold'                                            , 100, '$',%X'00000000',
    0,     12,   9,   0,   0,   0,   0,   0,   0, ' '    ,  1),
('gold'                                            , 100, '$',%X'00000000',
    0,     14,   0,   0,   0,   0,   0,   0,   0, ' '    ,  1),
('gold'                                            , 100, '$',%X'00000000',
    0,     16,   1,   0,   0,   0,   0,   0,   0, ' '    ,  1),
('opals'                                           , 100, '*',%X'00000000',
    0,     18,   2,   0,   0,   0,   0,   0,   0, ' '    ,  1),
('saphires'                                        , 100, '*',%X'00000000',
    0,     20,   3,   0,   0,   0,   0,   0,   0, ' '    ,  1),
('gold'                                            , 100, '$',%X'00000000',
    0,     24,   4,   0,   0,   0,   0,   0,   0, ' '    ,  1),
('rubies'                                          , 100, '*',%X'00000000',
    0,     28,   5,   0,   0,   0,   0,   0,   0, ' '    ,  1),
('diamonds'                                        , 100, '*',%X'00000000',
    0,     32,   6,   0,   0,   0,   0,   0,   0, ' '    ,  1),
('emeralds'                                        , 100, '*',%X'00000000',
    0,     40,   7,   0,   0,   0,   0,   0,   0, ' '    ,  1),
('mithril'                                         , 100, '$',%X'00000000',
    0,     80,   8,   0,   0,   0,   0,   0,   0, ' '    ,  1)
				);

	{ Object description:  Objects are defined here.  Each object has
	the following attributes:
		Descriptor : Name of item and formats.
			%C is replaced with a random color.
			%W is replaced with a random wood.
			%M is replaced with a random metal.
			%R is replaced with a random rock.
			%P1 is replaced with the number in P1 (see below).
			%P2 is replaced with plus-to-hit.
			%P3 is replaced with plus-to-damage.
			%P4 is replaced with plus-to-AC.
			%P5 is replaced with number in P1, no sign.
			& is replaced with 'a', 'an', or a number.
			~ is replaced with null or 's'.
		Character  : Character that represents the item.
		Type value : Value representing the type of object.
			1   ~   Miscellaneous Object.
			2   &   chest.
			10  (Left Curly Brace)  sling ammo.
			11  (Left Curly Brace)  bolt.
			12  (Left Curly Brace)  arrow.
			13  ~   spike.
			15  ~   Lamp or Torch.
			20  (Right curly brace)  bow, crossbow, or sling.
			21  /   hafted weapon.
			22  \   pole arm.
			23  |   sword or dagger.
			30  ]   boots.
			31  ]   gloves and gauntlets.
			32  (   Cloak
			33  ]   helm.
			34  )   shield.
			35  [   hard armor.
			36  (   soft armor.
			40  "   amulet.
			45  =   ring.
			55  _   staff.
			60  -   rod.
			65  -   wand.
			70  ?   scroll.
			71  ?   scroll.    continued
			75  !   potion.
			76  !   potion.    continued
			77  !   flask of oil.
			80  ,   food.
			90  ?   Magic Book.
			91  ?   Prayer Book.
			100 $   valuable metal.
			100 *   valuable gems.
			101 .   Unseen trap.
			102 ^   Seen trap.
			103 :   Rubble.
			104 '   Open door.
			105 +   Closed door.
			107 <   Up staircase.
			108 >   Down staircase.
			109 #   Secret door.
			110 1,2,3,4,5,6   Entrance to store.

		Sub value  : separate value for each item of a type.
		Damage     : amount of damage item can cause.
		Weight     : relative weight of an item.
		Number     : number of items appearing in group.
			If items can be added together then
				subval > 255
			If items are considered one unit when added
			together then
				subval > 511
		To hit     : magical pluses to hit.
		To damage  : magical pluses to damage.
		AC         : objects relative armor class.
			1 is worse than 5 is worse than 10 ect...
		To AC      : Magical bonuses to AC.
		P1         : Catch all for magical abilities such as
			     pluses to strength, minuses to searching...
		Flags      : Abilities of object.  Each abilitiy is a
			     bit.  Bits 1-31 are used. (Signed integer)

		Flags used for anything that can be worn or wielded:
				Bit #   Hex #           Ability
				1       00000001        Strength
				2       00000002        Dexterity
				3       00000004        Constitution
				4       00000008        Intelligence
				5       00000010        Wisdom
				6       00000020        Charisma
				7       00000040        Searching
				8       00000080        Slow digestion
				9       00000100        Stealth
				10      00000200        Aggravation
				11      00000400        Teleportation
				12      00000800        Regeneration
				13      00001000        Speed
				14      00002000        Slay Dragon
				15      00004000        Slay Monster
				16      00008000        Slay Evil
				17      00010000        Slay Undead
				18      00020000        Cold Brand
				19      00040000        Flame Brand
				20      00080000        Resist Fire
				21      00100000        Resist Acid
				22      00200000        Resist Cold
				23      00400000        Sustain Stat (P1)
				24      00800000        Free Action
				25      01000000        See Invisible
				26      02000000        Resist Lightning
				27      04000000        Feather Fall
				28      08000000        Blindness
				29      10000000        Timidness
				30      20000000        Tunneling
				31      40000000        Infra vision
				32      80000000        Cursed
		Level      : Minimum level on which item can be found.
		Cost       : Relative cost of item.

		Special Abilities can be added to item by MAGIC_INIT,
		found in MISC.INC.

	Scrolls, Potions, and Food:
		Flags is used to define a function which reading/quaffing
		will cause.  Most scrolls and potions have only one bit
		set.  Potions will generally have some food value, found
		in P1.

	Wands and Staffs:
		Flags defines a function, P1 contains number of charges
		for item.  P1 is set in MAGIC_INIT (MISC.INC)

	Chests:
		Traps are added randomly by MAGIC.INIT in MISC.INC.
	}
	{ Object list (All objects must be defined here)                }
	object_list     :=      (
('& %M Mushroom~| of Poison'                       ,  80, ',',%X'00000001',
  500,      0, 257,   1,   1,   0,   0,   0,   0, '0d0'  ,  7),
('& %M Mushroom~| of Blindness'                    ,  80, ',',%X'00000002',
  500,      0, 258,   1,   1,   0,   0,   0,   0, '0d0'  ,  9),
('& %M Mushroom~| of Paranoia'                     ,  80, ',',%X'00000004',
  500,      0, 259,   1,   1,   0,   0,   0,   0, '0d0'  ,  9),
('& %M Mushroom~| of Confusion'                    ,  80, ',',%X'00000008',
  500,      0, 260,   1,   1,   0,   0,   0,   0, '0d0'  ,  7),
('& %M Mushroom~| of Halucination'                 ,  80, ',',%X'00000010',
  500,      0, 261,   1,   1,   0,   0,   0,   0, '0d0'  , 13),
('& %M Mushroom~| of Cure Poison'                  ,  80, ',',%X'00000020',
  500,     60, 262,   1,   1,   0,   0,   0,   0, '0d0'  ,  8),
('& %M Mushroom~| of Cure Blindness'               ,  80, ',',%X'00000040',
  500,     50, 263,   1,   1,   0,   0,   0,   0, '0d0'  , 10),
('& %M Mushroom~| of Cure Paranoia'                ,  80, ',',%X'00000080',
  500,     25, 264,   1,   1,   0,   0,   0,   0, '0d0'  , 12),
('& %M Mushroom~| of Cure Confusion'               ,  80, ',',%X'00000100',
  500,     50, 265,   1,   1,   0,   0,   0,   0, '0d0'  ,  6),
('& %M Mushroom~| of Weakness'                     ,  80, ',',%X'04000200',
  500,      0, 266,   1,   1,   0,   0,   0,   0, '0d0'  ,  7),
('& %M Mushroom~| of Unhealth'                     ,  80, ',',%X'04000400',
  500,     50, 267,   1,   1,   0,   0,   0,   0, '10d10', 15),
('& %M Mushroom~| of Restore Constitution'         ,  80, ',',%X'00010000',
  500,    350, 268,   1,   1,   0,   0,   0,   0, '0d0'  , 20),
('& %M Mushroom~| of First-Aid'                    ,  80, ',',%X'00200000',
  500,      5, 269,   1,   1,   0,   0,   0,   0, '0d0'  ,  6),
('& %M Mushroom~| of Minor Cures'                  ,  80, ',',%X'00400000',
  500,     20, 270,   1,   1,   0,   0,   0,   0, '0d0'  ,  7),
('& %M Mushroom~| of Light Cures'                  ,  80, ',',%X'00800000',
  500,     30, 271,   1,   1,   0,   0,   0,   0, '0d0'  , 10),
('& %M Mushroom~| of Restoring'                    ,  80, ',',%X'001F8040',
  500,   1000, 272,   1,   1,   0,   0,   0,   0, '0d0'  , 30),
('& Hairy %M Mold~| of Poison'                     ,  80, ',',%X'00000001',
 1200,      0, 273,   1,   1,   0,   0,   0,   0, '0d0'  , 15),
('& Hairy %M Mold~| of Halucinations'              ,  80, ',',%X'00000010',
 1200,      0, 274,   1,   1,   0,   0,   0,   0, '0d0'  , 18),
('& Hairy %M Mold~| of Cure Poison'                ,  80, ',',%X'00000020',
 1200,     75, 275,   1,   1,   0,   0,   0,   0, '0d0'  , 19),
('& Hairy %M Mold~| of Unhealth'                   ,  80, ',',%X'00000400',
 1200,     25, 276,   1,   1,   0,   0,   0,   0, '6d8'  , 28),
('& Hairy %M Mold~| of Cure Serious Wounds'        ,  80, ',',%X'02000000',
 1200,     75, 277,   2,   1,   0,   0,   0,   0, '0d0'  , 16),
('& Ration~ of Food'                               ,  80, ',',%X'00000000',
 5000,      3, 307,  10,   1,   0,   0,   0,   0, '0d0'  ,  0),
('& Ration~ of Food'                               ,  80, ',',%X'00000000',
 5000,      3, 307,  10,   1,   0,   0,   0,   0, '0d0'  ,  5),
('& Ration~ of Food'                               ,  80, ',',%X'00000000',
 5000,      3, 307,  10,   1,   0,   0,   0,   0, '0d0'  , 10),
('& Slime Mold~'                                   ,  80, ',',%X'00000000',
 3000,      2, 308,   5,   1,   0,   0,   0,   0, '0d0'  ,  1),
('& Piece~ of Elvish Waybread'                     ,  80, ',',%X'02000020',
 7500,     10, 313,   3,   1,   0,   0,   0,   0, '0d0'  ,  6),
('& Piece~ of Elvish Waybread'                     ,  80, ',',%X'02000020',
 7500,     10, 313,   3,   1,   0,   0,   0,   0, '0d0'  , 12),
('& Piece~ of Elvish Waybread'                     ,  80, ',',%X'02000020',
 7500,     10, 313,   3,   1,   0,   0,   0,   0, '0d0'  , 20),
('& Dagger (Main Gauche)^ (%P2,%P3)'               ,  23, '|',%X'00000000',
    0,     25,   1,  30,   1,   0,   0,   0,   0, '1d5'  ,  2),
('& Dagger (Misercorde)^ (%P2,%P3)'                ,  23, '|',%X'00000000',
    0,     10,   2,  15,   1,   0,   0,   0,   0, '1d4'  ,  0),
('& Dagger (Stiletto)^ (%P2,%P3)'                  ,  23, '|',%X'00000000',
    0,     10,   3,  12,   1,   0,   0,   0,   0, '1d4'  ,  0),
('& Dagger (Bodkin)^ (%P2,%P3)'                    ,  23, '|',%X'00000000',
    0,     10,   4,  20,   1,   0,   0,   0,   0, '1d4'  ,  1),
('& Broken dagger^ (%P2,%P3)'                      ,  23, '|',%X'00000000',
    0,      0,   5,  15,   1,  -2,  -2,   0,   0, '1d1'  ,  0),
('& Backsword^ (%P2,%P3)'                          ,  23, '|',%X'00000000',
    0,     60,   6,  95,   1,   0,   0,   0,   0, '1d9'  ,  7),
('& Bastard Sword^ (%P2,%P3)'                      ,  23, '|',%X'00000000',
    0,    350,   7, 140,   1,   0,   0,   0,   0, '3d4'  , 14),
('& Thrusting Sword (Bilbo)^ (%P2,%P3)'            ,  23, '|',%X'00000000',
    0,     60,   8,  80,   1,   0,   0,   0,   0, '1d6'  ,  4),
('& Thrusting Sword (Baselard)^ (%P2,%P3)'         ,  23, '|',%X'00000000',
    0,     80,   9, 100,   1,   0,   0,   0,   0, '1d7'  ,  5),
('& Broadsword^ (%P2,%P3)'                         ,  23, '|',%X'00000000',
    0,    255,  10, 150,   1,   0,   0,   0,   0, '2d5'  ,  9),
('& Two Handed Sword (Claymore)^ (%P2,%P3)'        ,  23, '|',%X'00000000',
    0,    775,  11, 200,   1,   0,   0,   0,   0, '3d6'  , 30),
('& Cutlass^ (%P2,%P3)'                            ,  23, '|',%X'00000000',
    0,     85,  12, 110,   1,   0,   0,   0,   0, '1d7'  ,  7),
('& Two Handed Sword (Espadon)^ (%P2,%P3)'         ,  23, '|',%X'00000000',
    0,    655,  13, 180,   1,   0,   0,   0,   0, '3d6'  , 35),
('& Executioner''s Sword^ (%P2,%P3)'               ,  23, '|',%X'00000000',
    0,    850,  14, 260,   1,   0,   0,   0,   0, '4d5'  , 40),
('& Two Handed Sword (Flamberge)^ (%P2,%P3)'       ,  23, '|',%X'00000000',
    0,   1000,  15, 240,   1,   0,   0,   0,   0, '4d5'  , 45),
('& Foil^ (%P2,%P3)'                               ,  23, '|',%X'00000000',
    0,     35,  16,  30,   1,   0,   0,   0,   0, '1d5'  ,  2),
('& Katana^ (%P2,%P3)'                             ,  23, '|',%X'00000000',
    0,    400,  17, 120,   1,   0,   0,   0,   0, '3d4'  , 18),
('& Longsword^ (%P2,%P3)'                          ,  23, '|',%X'00000000',
    0,    300,  18, 130,   1,   0,   0,   0,   0, '1d10' , 12),
('& Two Handed Sword (No-Dachi)^ (%P2,%P3)'        ,  23, '|',%X'00000000',
    0,    675,  19, 200,   1,   0,   0,   0,   0, '4d4'  , 45),
('& Rapier^ (%P2,%P3)'                             ,  23, '|',%X'00000000',
    0,     42,  20,  40,   1,   0,   0,   0,   0, '1d6'  ,  4),
('& Sabre^ (%P2,%P3)'                              ,  23, '|',%X'00000000',
    0,     50,  21,  50,   1,   0,   0,   0,   0, '1d7'  ,  5),
('& Small Sword^ (%P2,%P3)'                        ,  23, '|',%X'00000000',
    0,     48,  22,  75,   1,   0,   0,   0,   0, '1d6'  ,  5),
('& Two Handed Sword (Zweihander)^ (%P2,%P3)'      ,  23, '|',%X'00000000',
    0,   1000,  23, 280,   1,   0,   0,   0,   0, '4d6'  , 50),
('& Broken sword^ (%P2,%P3)'                       ,  23, '|',%X'00000000',
    0,      0,  24,  75,   1,  -2,  -2,   0,   0, '1d1'  ,  0),
('& Battle Axe (Balestarius)^ (%P2,%P3)'           ,  21, '\',%X'00000000',
    0,    500,   1, 180,   1,   0,   0,   0,   0, '2d8'  , 30),
('& Ball and Chain^ (%P2,%P3)'                     ,  21, '\',%X'00000000',
    0,    200,   2, 150,   1,   0,   0,   0,   0, '2d4'  , 20),
('& Battle Axe (European)^ (%P2,%P3)'              ,  21, '\',%X'00000000',
    0,    334,   3, 170,   1,   0,   0,   0,   0, '3d4'  , 13),
('& Broad Axe^ (%P2,%P3)'                          ,  21, '\',%X'00000000',
    0,    304,   4, 160,   1,   0,   0,   0,   0, '2d6'  , 17),
('& Cat-O-Nine Tails^ (%P2,%P3)'                   ,  21, '\',%X'00000000',
    0,     14,   5,  40,   1,   0,   0,   0,   0, '1d4'  ,  3),
('& Wooden Club^ (%P2,%P3)'                        ,  21, '\',%X'00000000',
    0,      1,   6, 100,   1,   0,   0,   0,   0, '1d3'  ,  0),
('& Flail^ (%P2,%P3)'                              ,  21, '\',%X'00000000',
    0,    353,   7, 150,   1,   0,   0,   0,   0, '2d6'  , 12),
('& Two Handed Great Flail^ (%P2,%P3)'             ,  21, '\',%X'00000000',
    0,    590,   8, 280,   1,   0,   0,   0,   0, '3d6'  , 45),
('& Morningstar^ (%P2,%P3)'                        ,  21, '\',%X'00000000',
    0,    396,   9, 150,   1,   0,   0,   0,   0, '2d6'  , 10),
('& Mace^ (%P2,%P3)'                               ,  21, '\',%X'00000000',
    0,    130,  10, 120,   1,   0,   0,   0,   0, '2d4'  ,  6),
('& War Hammer^ (%P2,%P3)'                         ,  21, '\',%X'00000000',
    0,    225,  11, 120,   1,   0,   0,   0,   0, '3d3'  ,  5),
('& Mace (Lead filled)^ (%P2,%P3)'                 ,  21, '\',%X'00000000',
    0,    502,  12, 180,   1,   0,   0,   0,   0, '3d4'  , 15),
('& Awl-Pike^ (%P2,%P3)'                           ,  22, '/',%X'00000000',
    0,    340,   1, 160,   1,   0,   0,   0,   0, '1d8'  ,  8),
('& Beaked Axe^ (%P2,%P3)'                         ,  22, '/',%X'00000000',
    0,    408,   2, 180,   1,   0,   0,   0,   0, '2d6'  , 15),
('& Fauchard^ (%P2,%P3)'                           ,  22, '/',%X'00000000',
    0,    376,   3, 170,   1,   0,   0,   0,   0, '1d10' , 17),
('& Glaive^ (%P2,%P3)'                             ,  22, '/',%X'00000000',
    0,    363,   4, 190,   1,   0,   0,   0,   0, '2d6'  , 20),
('& Halberd^ (%P2,%P3)'                            ,  22, '/',%X'00000000',
    0,    430,   5, 190,   1,   0,   0,   0,   0, '3d4'  , 22),
('& Lucerne Hammer^ (%P2,%P3)'                     ,  22, '/',%X'00000000',
    0,    376,   6, 120,   1,   0,   0,   0,   0, '2d5'  , 11),
('& Pike^ (%P2,%P3)'                               ,  22, '/',%X'00000000',
    0,    358,   7, 160,   1,   0,   0,   0,   0, '2d5'  , 15),
('& Spear^ (%P2,%P3)'                              ,  22, '/',%X'00000000',
    0,     36,   8,  50,   1,   0,   0,   0,   0, '1d6'  ,  5),
('& Lance^ (%P2,%P3)'                              ,  22, '/',%X'00000000',
    0,    230,   9, 300,   1,   0,   0,   0,   0, '2d8'  , 10),
('& Javelin^ (%P2,%P3)'                            ,  22, '/',%X'00000000',
    0,     18,  10,  30,   1,   0,   0,   0,   0, '1d4'  ,  4),
('& Short Bow^ (%P2)'                              ,  20, '}',%X'00000000',
    2,     50,   1,  30,   1,   0,   0,   0,   0, '0d0'  ,  3),
('& Long Bow^ (%P2)'                               ,  20, '}',%X'00000000',
    3,    120,   2,  40,   1,   0,   0,   0,   0, '0d0'  , 10),
('& Composite Bow^ (%P2)'                          ,  20, '}',%X'00000000',
    4,    240,   3,  40,   1,   0,   0,   0,   0, '0d0'  , 40),
('& Light Crossbow^ (%P2)'                         ,  20, '}',%X'00000000',
    5,    140,  10, 110,   1,   0,   0,   0,   0, '0d0'  , 15),
('& Heavy Crossbow^ (%P2)'                         ,  20, '}',%X'00000000',
    6,    300,  11, 200,   1,   0,   0,   0,   0, '1d1'  , 30),
('& Sling^ (%P2)'                                  ,  20, '}',%X'00000000',
    1,      5,  20,   5,   1,   0,   0,   0,   0, '0d0'  ,  1),
('& Arrow~^ (%P2,%P3)'                             ,  12, '{',%X'00000000',
    0,      1,   1,   2,   1,   0,   0,   0,   0, '1d4'  ,  2),
('& Bolt~^ (%P2,%P3)'                              ,  11, '{',%X'00000000',
    0,      2,   1,   3,   1,   0,   0,   0,   0, '1d5'  ,  2),
('& Rounded Pebble~^ (%P2,%P3)'                    ,  10, '{',%X'00000000',
    0,      1,   1,   4,   1,   0,   0,   0,   0, '1d2'  ,  0),
('& Iron Shot~^ (%P2,%P3)'                         ,  10, '{',%X'00000000',
    0,      2,   1,   5,   1,   0,   0,   0,   0, '1d3'  ,  3),
('& Iron Spike~'                                   ,  13, '~',%X'00000000',
    0,      1,   1,  10,   1,   0,   0,   0,   0, '1d1'  ,  1),
('& Brass Lantern~ with %P5 turns of light'        ,  15, '~',%X'00000000',
 7500,     35,   1,  50,   1,   0,   0,   0,   0, '1d1'  ,  1),
('& Wooden Torch~ with %P5 turns of light'         ,  15, '~',%X'00000000',
 4000,      2,  13,  30,   1,   0,   0,   0,   0, '1d1'  ,  1),
('& Orcish Pick^ (%P1) (%P2,%P3)'                  ,  25, '\',%X'20000000',
    2,    500,   2, 180,   1,   0,   0,   0,   0, '1d3'  , 20),
('& Dwarven Pick^ (%P1) (%P2,%P3)'                 ,  25, '\',%X'20000000',
    3,   1200,   3, 200,   1,   0,   0,   0,   0, '1d4'  , 50),
('& Gnomish Shovel^ (%P1) (%P2,%P3)'               ,  25, '\',%X'20000000',
    1,    100,   5,  50,   1,   0,   0,   0,   0, '1d2'  , 20),
('& Dwarven Shovel^ (%P1) (%P2,%P3)'               ,  25, '\',%X'20000000',
    2,    250,   6, 120,   1,   0,   0,   0,   0, '1d3'  , 40),
('& Pair of Soft Leather Shoes^ [%P6,%P4]'         ,  30, ']',%X'00000000',
    0,      4,   1,   5,   1,   0,   0,   1,   0, '0d0'  ,  1),
('& Pair of Soft Leather Boots^ [%P6,%P4]'         ,  30, ']',%X'00000000',
    0,      7,   2,  20,   1,   0,   0,   2,   0, '1d1'  ,  4),
('& Pair of Hard Leather Boots^ [%P6,%P4]'         ,  30, ']',%X'00000000',
    0,     12,   3,  40,   1,   0,   0,   3,   0, '1d1'  ,  6),
('& Soft Leather Cap^ [%P6,%P4]'                   ,  33, ']',%X'00000000',
    0,      4,   1,  10,   1,   0,   0,   1,   0, '0d0'  ,  2),
('& Hard Leather Cap^ [%P6,%P4]'                   ,  33, ']',%X'00000000',
    0,     12,   2,  15,   1,   0,   0,   2,   0, '0d0'  ,  4),
('& Metal Cap^ [%P6,%P4]'                          ,  33, ']',%X'00000000',
    0,     30,   3,  20,   1,   0,   0,   3,   0, '1d1'  ,  7),
('& Iron Helm^ [%P6,%P4]'                          ,  33, ']',%X'00000000',
    0,     75,   4,  75,   1,   0,   0,   5,   0, '1d3'  , 20),
('& Steel Helm^ [%P6,%P4]'                         ,  33, ']',%X'00000000',
    0,    200,   5,  60,   1,   0,   0,   6,   0, '1d3'  , 40),
('& Silver Crown^ (%P1)'                           ,  33, ']',%X'00000000',
    0,    250,   6,  20,   1,   0,   0,   0,   0, '1d1'  , 44),
('& Golden Crown^ (%P1)'                           ,  33, ']',%X'00000000',
    0,    500,   7,  30,   1,   0,   0,   0,   0, '1d1'  , 47),
('& Jewel Encrusted Crown^ (%P1)'                  ,  33, ']',%X'00000000',
    0,   1000,   8,  40,   1,   0,   0,   0,   0, '1d1'  , 50),
('& Robe^ [%P6,%P4]'                               ,  36, '(',%X'00000000',
    0,      4,   1,  20,   1,   0,   0,   2,   0, '0d0'  ,  1),
('Soft Leather Armor^ [%P6,%P4]'                   ,  36, '(',%X'00000000',
    0,     18,   2,  80,   1,   0,   0,   4,   0, '0d0'  ,  2),
('Soft Studded Leather^ [%P6,%P4]'                 ,  36, '(',%X'00000000',
    0,     35,   3,  90,   1,   0,   0,   5,   0, '1d1'  ,  3),
('Hard Leather Armor^ [%P6,%P4]'                   ,  36, '(',%X'00000000',
    0,     55,   4, 100,   1,  -1,   0,   6,   0, '1d1'  ,  5),
('Hard Studded Leather^ [%P6,%P4]'                 ,  36, '(',%X'00000000',
    0,    100,   5, 110,   1,  -1,   0,   7,   0, '1d2'  ,  7),
('Woven Cord Armor^ [%P6,%P4]'                     ,  36, '(',%X'00000000',
    0,     45,   6, 150,   1,  -1,   0,   6,   0, '0d0'  ,  7),
('Soft Leather Ring Mail^ [%P6,%P4]'               ,  36, '(',%X'00000000',
    0,    160,   7, 130,   1,  -1,   0,   6,   0, '1d2'  , 10),
('Hard Leather Ring Mail^ [%P6,%P4]'               ,  36, '(',%X'00000000',
    0,    230,   8, 150,   1,  -2,   0,   8,   0, '1d3'  , 12),
('Leather Scale Mail^ [%P6,%P4]'                   ,  36, '(',%X'00000000',
    0,    330,   9, 140,   1,  -1,   0,  11,   0, '1d1'  , 14),
('Metal Scale Mail^ [%P6,%P4]'                     ,  35, '[',%X'00000000',
    0,    430,   1, 250,   1,  -2,   0,  13,   0, '1d4'  , 24),
('Chain Mail^ [%P6,%P4]'                           ,  35, '[',%X'00000000',
    0,    530,   2, 220,   1,  -2,   0,  14,   0, '1d4'  , 26),
('Rusty Chain Mail^ [%P6,%P4]'                     ,  35, '[',%X'00000000',
    0,      0,   3, 200,   1,  -5,   0,  14,  -8, '1d4'  , 26),
('Double Chain Mail^ [%P6,%P4]'                    ,  35, '[',%X'00000000',
    0,    630,   4, 260,   1,  -2,   0,  15,   0, '1d4'  , 28),
('Augmented Chain Mail^ [%P6,%P4]'                 ,  35, '[',%X'00000000',
    0,    675,   5, 270,   1,  -2,   0,  16,   0, '1d4'  , 30),
('Bar Chain Mail^ [%P6,%P4]'                       ,  35, '[',%X'00000000',
    0,    720,   6, 280,   1,  -2,   0,  18,   0, '1d4'  , 34),
('Metal Brigandine Armor^ [%P6,%P4]'               ,  35, '[',%X'00000000',
    0,    775,   7, 290,   1,  -3,   0,  19,   0, '1d4'  , 36),
('Laminated Armor^ [%P6,%P4]'                      ,  35, '[',%X'00000000',
    0,    825,   8, 300,   1,  -3,   0,  20,   0, '1d4'  , 38),
('Partial Plate Armor^ [%P6,%P4]'                  ,  35, '[',%X'00000000',
    0,    900,   9, 260,   1,  -3,   0,  22,   0, '1d6'  , 42),
('Metal Lamellar Armor^ [%P6,%P4]'                 ,  35, '[',%X'00000000',
    0,    950,  10, 340,   1,  -3,   0,  23,   0, '1d6'  , 44),
('Full Plate Armor^ [%P6,%P4]'                     ,  35, '[',%X'00000000',
    0,   1050,  11, 380,   1,  -3,   0,  25,   0, '2d4'  , 48),
('Ribbed Plate Armor^ [%P6,%P4]'                   ,  35, '[',%X'00000000',
    0,   1200,  12, 380,   1,  -3,   0,  28,   0, '2d4'  , 50),
('& Cloak^ [%P6,%P4]'                              ,  32, '(',%X'00000000',
    0,      3,   1,  10,   1,   0,   0,   1,   0, '0d0'  ,  1),
('& Set of Leather Gloves^ [%P6,%P4]'              ,  31, ']',%X'00000000',
    0,      3,   1,   5,   1,   0,   0,   1,   0, '0d0'  ,  1),
('& Set of Gauntlets^ [%P6,%P4]'                   ,  31, ']',%X'00000000',
    0,     35,   2,  25,   1,   0,   0,   2,   0, '1d1'  , 12),
('& Small Leather Shield^ [%P6,%P4]'               ,  34, ')',%X'00000000',
    0,     30,   1,  50,   1,   0,   0,   2,   0, '1d1'  ,  3),
('& Medium Leather Shield^ [%P6,%P4]'              ,  34, ')',%X'00000000',
    0,     60,   2,  75,   1,   0,   0,   3,   0, '1d2'  ,  8),
('& Large Leather Shield^ [%P6,%P4]'               ,  34, ')',%X'00000000',
    0,    120,   3, 100,   1,   0,   0,   4,   0, '1d2'  , 15),
('& Small Metal Shield^ [%P6,%P4]'                 ,  34, ')',%X'00000000',
    0,     50,   4,  65,   1,   0,   0,   3,   0, '1d2'  , 10),
('& Medium Metal Shield^ [%P6,%P4]'                ,  34, ')',%X'00000000',
    0,    125,   5,  90,   1,   0,   0,   4,   0, '1d3'  , 20),
('& Large Metal Shield^ [%P6,%P4]'                 ,  34, ')',%X'00000000',
    0,    200,   6, 120,   1,   0,   0,   5,   0, '1d3'  , 30),
('& %R Ring| of Gain Strength^ (%P1)'              ,  45, '=',%X'00000001',
    0,    400,   1,   2,   1,   0,   0,   0,   0, '0d0'  , 30),
('& %R Ring| of Gain Dexterity^ (%P1)'             ,  45, '=',%X'00000002',
    0,    400,   2,   2,   1,   0,   0,   0,   0, '0d0'  , 30),
('& %R Ring| of Gain Constitution^ (%P1)'          ,  45, '=',%X'00000004',
    0,    400,   3,   2,   1,   0,   0,   0,   0, '0d0'  , 30),
('& %R Ring| of Gain Intelligence^ (%P1)'          ,  45, '=',%X'00000008',
    0,    350,   4,   2,   1,   0,   0,   0,   0, '0d0'  , 30),
('& %R Ring| of Speed^ (%P1)'                      ,  45, '=',%X'00001000',
    0,   3000,   7,   2,   1,   0,   0,   0,   0, '0d0'  , 50),
('& %R Ring| of Searching^ (%P1)'                  ,  45, '=',%X'00000040',
    0,    250,   8,   2,   1,   0,   0,   0,   0, '0d0'  ,  7),
('& %R Ring| of Teleportation^'                     ,  45, '=',%X'80000400',
    0,      0,   9,   2,   1,   0,   0,   0,   0, '0d0'  ,  7),
('& %R Ring| of Slow Digestion^'                   ,  45, '=',%X'00000080',
    0,    250,  10,   2,   1,   0,   0,   0,   0, '0d0'  ,  7),
('& %R Ring| of Resist Fire^'                      ,  45, '=',%X'00080000',
    0,    250,  11,   2,   1,   0,   0,   0,   0, '0d0'  , 14),
('& %R Ring| of Resist Cold^'                      ,  45, '=',%X'00200000',
    0,    250,  12,   2,   1,   0,   0,   0,   0, '0d0'  , 14),
('& %R Ring| of Feather Falling^'                  ,  45, '=',%X'04000000',
    0,    200,  13,   2,   1,   0,   0,   0,   0, '0d0'  ,  7),
('& %R Ring| of Adornment^'                        ,  45, '=',%X'00000000',
    0,     20,  14,   2,   1,   0,   0,   0,   0, '0d0'  ,  7),
('& %R Ring| of Adornment^'                        ,  45, '=',%X'00000000',
    0,     30,  15,   2,   1,   0,   0,   0,   0, '0d0'  ,  7),
('& %R Ring| of Weakness^'                         ,  45, '=',%X'00000001',
   -5,      0,  16,   2,   1,   0,   0,   0,   0, '0d0'  ,  7),
('& %R Ring| of Lordly Protection (FIRE)^'         ,  45, '=',%X'00080000',
    0,   1200,  17,   2,   1,   0,   0,   0,   5, '0d0'  , 50),
('& %R Ring| of Lordly Protection (ACID)^'         ,  45, '=',%X'00100000',
    0,   1200,  18,   2,   1,   0,   0,   0,   5, '0d0'  , 50),
('& %R Ring| of Lordly Protection (COLD)^'         ,  45, '=',%X'00200000',
    0,   1200,  19,   2,   1,   0,   0,   0,   5, '0d0'  , 50),
('& %R Ring| of WOE^'                              ,  45, '=',%X'80000640',
   -5,      0,  20,   2,   1,   0,   0,   0,  -3, '0d0'  , 50),
('& %R Ring| of Stupidity^'                        ,  45, '=',%X'80000008',
   -5,      0,  21,   2,   1,   0,   0,   0,   0, '0d0'  , 20),
('& %R Ring| of Increase Damage^ (%P3)'            ,  45, '=',%X'00000000',
    0,    100,  22,   2,   1,   0,   0,   0,   0, '0d0'  , 20),
('& %R Ring| of Increase To-Hit^ (%P2)'            ,  45, '=',%X'00000000',
    0,    100,  23,   2,   1,   0,   0,   0,   0, '0d0'  , 20),
('& %R Ring| of Protection^ [%P4]'                 ,  45, '=',%X'00000000',
    0,    100,  24,   2,   1,   0,   0,   0,   0, '0d0'  ,  7),
('& %R Ring| of Aggravate Monster^'                ,  45, '=',%X'80000200',
    0,      0,  25,   2,   1,   0,   0,   0,   0, '0d0'  ,  7),
('& %R Ring| of See Invisible^'                    ,  45, '=',%X'01000000',
    0,    340,  26,   2,   1,   0,   0,   0,   0, '0d0'  , 40),
('& %R Ring| of Sustain Strength^'                 ,  45, '=',%X'00400000',
    1,    750,  27,   2,   1,   0,   0,   0,   0, '0d0'  , 44),
('& %R Ring| of Sustain Intelligence^'             ,  45, '=',%X'00400000',
    2,    600,  28,   2,   1,   0,   0,   0,   0, '0d0'  , 44),
('& %R Ring| of Sustain Wisdom^'                   ,  45, '=',%X'00400000',
    3,    600,  29,   2,   1,   0,   0,   0,   0, '0d0'  , 44),
('& %R Ring| of Sustain Constitution^'             ,  45, '=',%X'00400000',
    4,    750,  30,   2,   1,   0,   0,   0,   0, '0d0'  , 44),
('& %R Ring| of Sustain Dexterity^'                ,  45, '=',%X'00400000',
    5,    750,  31,   2,   1,   0,   0,   0,   0, '0d0'  , 44),
('& %R Ring| of Sustain Charisma^'                 ,  45, '=',%X'00400000',
    6,    500,  32,   2,   1,   0,   0,   0,   0, '0d0'  ,  7),
('& %R Ring| of Slaying^'                          ,  45, '=',%X'00000000',
    6,   1000,  33,   2,   1,   0,   0,   0,   0, '0d0'  , 50),
('& %A Amulet| of Adornment^'                      ,  40, '"',%X'00000000',
    0,     20,  11,   3,   1,   0,   0,   0,   0, '0d0'  , 16),
('& %A Amulet| of Adornment^'                      ,  40, '"',%X'00000000',
    0,     30,  12,   3,   1,   0,   0,   0,   0, '0d0'  , 16),
('& %A Amulet| of Wisdom^ (%P1)'                   ,  40, '"',%X'00000010',
    0,    300,   5,   3,   1,   0,   0,   0,   0, '0d0'  , 20),
('& %A Amulet| of Charisma^ (%P1)'                 ,  40, '"',%X'00000020',
    0,    250,   6,   3,   1,   0,   0,   0,   0, '0d0'  , 20),
('& %A Amulet| of Searching^ (%P1)'                ,  40, '"',%X'00000040',
    0,    250,   7,   3,   1,   0,   0,   0,   0, '0d0'  , 14),
('& %A Amulet| of Teleportation^'                   ,  40, '"',%X'80000400',
    0,      0,   8,   3,   1,   0,   0,   0,   0, '0d0'  , 14),
('& %A Amulet| of Slow Digestion^'                 ,  40, '"',%X'00000080',
    0,    200,   9,   3,   1,   0,   0,   0,   0, '0d0'  , 14),
('& %A Amulet| of Resist Acid^'                    ,  40, '"',%X'00100000',
    0,    300,  10,   3,   1,   0,   0,   0,   0, '0d0'  , 24),
('& %A Amulet| of the Magi^'                       ,  40, '"',%X'01800040',
    0,   5000,  13,   3,   1,   0,   0,   0,   3, '0d0'  , 50),
('& %A Amulet| of DOOM^'                           ,  40, '"',%X'8000007F',
   -5,      0,  14,   3,   1,   0,   0,   0,   0, '0d0'  , 50),
('& Scroll~ %T| of Enchant Weapon To-Hit'          ,  70, '?',%X'00000001',
    0,    125, 257,   5,   1,   0,   0,   0,   0, '0d0'  , 12),
('& Scroll~ %T| of Enchant Weapon To-Dam'          ,  70, '?',%X'00000002',
    0,    125, 258,   5,   1,   0,   0,   0,   0, '0d0'  , 12),
('& Scroll~ %T| of Enchant Armor'                  ,  70, '?',%X'00000004',
    0,    125, 259,   5,   1,   0,   0,   0,   0, '0d0'  , 12),
('& Scroll~ %T| of Identify'                       ,  70, '?',%X'00000008',
    0,     50, 260,   5,   1,   0,   0,   0,   0, '0d0'  ,  1),
('& Scroll~ %T| of Identify'                       ,  70, '?',%X'00000008',
    0,     50, 260,   5,   1,   0,   0,   0,   0, '0d0'  ,  5),
('& Scroll~ %T| of Identify'                       ,  70, '?',%X'00000008',
    0,     50, 260,   5,   1,   0,   0,   0,   0, '0d0'  , 10),
('& Scroll~ %T| of Remove Curse'                   ,  70, '?',%X'00000010',
    0,    100, 261,   5,   1,   0,   0,   0,   0, '0d0'  ,  7),
('& Scroll~ %T| of Light'                          ,  70, '?',%X'00000020',
    0,     15, 262,   5,   1,   0,   0,   0,   0, '0d0'  ,  0),
('& Scroll~ %T| of Light'                          ,  70, '?',%X'00000020',
    0,     15, 262,   5,   1,   0,   0,   0,   0, '0d0'  ,  3),
('& Scroll~ %T| of Light'                          ,  70, '?',%X'00000020',
    0,     15, 262,   5,   1,   0,   0,   0,   0, '0d0'  ,  7),
('& Scroll~ %T| of Summon Monster'                 ,  70, '?',%X'00000040',
    0,      0, 263,   5,   1,   0,   0,   0,   0, '0d0'  ,  1),
('& Scroll~ %T| of Phase Door'                     ,  70, '?',%X'00000080',
    0,     15, 264,   5,   1,   0,   0,   0,   0, '0d0'  ,  1),
('& Scroll~ %T| of Teleport'                       ,  70, '?',%X'00000100',
    0,     40, 265,   5,   1,   0,   0,   0,   0, '0d0'  , 10),
('& Scroll~ %T| of Teleport Level'                 ,  70, '?',%X'00000200',
    0,     50, 266,   5,   1,   0,   0,   0,   0, '0d0'  , 20),
('& Scroll~ %T| of Monster Confusion'              ,  70, '?',%X'00000400',
    0,     30, 267,   5,   1,   0,   0,   0,   0, '0d0'  ,  5),
('& Scroll~ %T| of Magic Mapping'                  ,  70, '?',%X'00000800',
    0,     40, 268,   5,   1,   0,   0,   0,   0, '0d0'  ,  5),
('& Scroll~ %T| of Sleep Monster'                  ,  70, '?',%X'00001000',
    0,     35, 269,   5,   1,   0,   0,   0,   0, '0d0'  ,  5),
('& Scroll~ %T| of Rune of Protection'             ,  70, '?',%X'00002000',
    0,    500, 270,   5,   1,   0,   0,   0,   0, '0d0'  , 50),
('& Scroll~ %T| of Treasure Detection'             ,  70, '?',%X'00004000',
    0,     15, 271,   5,   1,   0,   0,   0,   0, '0d0'  ,  0),
('& Scroll~ %T| of Object Detection'               ,  70, '?',%X'00008000',
    0,     15, 272,   5,   1,   0,   0,   0,   0, '0d0'  ,  0),
('& Scroll~ %T| of Trap Detection'                 ,  70, '?',%X'00010000',
    0,     35, 273,   5,   1,   0,   0,   0,   0, '0d0'  ,  5),
('& Scroll~ %T| of Trap Detection'                 ,  70, '?',%X'00010000',
    0,     35, 273,   5,   1,   0,   0,   0,   0, '0d0'  ,  8),
('& Scroll~ %T| of Trap Detection'                 ,  70, '?',%X'00010000',
    0,     35, 273,   5,   1,   0,   0,   0,   0, '0d0'  , 12),
('& Scroll~ %T| of Door/Stair Location'            ,  70, '?',%X'00020000',
    0,     35, 274,   5,   1,   0,   0,   0,   0, '0d0'  ,  5),
('& Scroll~ %T| of Door/Stair Location'            ,  70, '?',%X'00020000',
    0,     35, 274,   5,   1,   0,   0,   0,   0, '0d0'  , 10),
('& Scroll~ %T| of Door/Stair Location'            ,  70, '?',%X'00020000',
    0,     35, 274,   5,   1,   0,   0,   0,   0, '0d0'  , 15),
('& Scroll~ %T| of Mass Genocide'                  ,  70, '?',%X'00040000',
    0,   1000, 275,   5,   1,   0,   0,   0,   0, '0d0'  , 50),
('& Scroll~ %T| of Detect Invisible'               ,  70, '?',%X'00080000',
    0,     15, 276,   5,   1,   0,   0,   0,   0, '0d0'  ,  1),
('& Scroll~ %T| of Aggravate Monster'              ,  70, '?',%X'00100000',
    0,      0, 277,   5,   1,   0,   0,   0,   0, '0d0'  ,  5),
('& Scroll~ %T| of Trap Creation'                  ,  70, '?',%X'00200000',
    0,      0, 278,   5,   1,   0,   0,   0,   0, '0d0'  , 12),
('& Scroll~ %T| of Trap/Door Destruction'          ,  70, '?',%X'00400000',
    0,     50, 279,   5,   1,   0,   0,   0,   0, '0d0'  , 12),
('& Scroll~ %T| of Door Creation'                  ,  70, '?',%X'00800000',
    0,    100, 280,   5,   1,   0,   0,   0,   0, '0d0'  , 12),
('& Scroll~ %T| of Recharging'                     ,  70, '?',%X'01000000',
    0,    200, 281,   5,   1,   0,   0,   0,   0, '0d0'  , 40),
('& Scroll~ %T| of Genocide'                       ,  70, '?',%X'02000000',
    0,    750, 282,   5,   1,   0,   0,   0,   0, '0d0'  , 35),
('& Scroll~ %T| of Darkness'                       ,  70, '?',%X'04000000',
    0,      0, 283,   5,   1,   0,   0,   0,   0, '0d0'  ,  1),
('& Scroll~ %T| of Protection from Evil'           ,  70, '?',%X'08000000',
    0,     50, 284,   5,   1,   0,   0,   0,   0, '0d0'  , 30),
('& Scroll~ %T| of Create Food'                    ,  70, '?',%X'10000000',
    0,     10, 285,   5,   1,   0,   0,   0,   0, '0d0'  ,  5),
('& Scroll~ %T| of Dispell Undead'                 ,  70, '?',%X'20000000',
    0,    200, 286,   5,   1,   0,   0,   0,   0, '0d0'  , 40),
('& Scroll~ %T| of *Enchant Weapon*'               ,  71, '?',%X'00000001',
    0,    500, 257,   5,   1,   0,   0,   0,   0, '0d0'  , 50),
('& Scroll~ %T| of Curse Weapon'                   ,  71, '?',%X'00000002',
    0,      0, 258,   5,   1,   0,   0,   0,   0, '0d0'  , 50),
('& Scroll~ %T| of *Enchant Armor*'                ,  71, '?',%X'00000004',
    0,    500, 259,   5,   1,   0,   0,   0,   0, '0d0'  , 50),
('& Scroll~ %T| of Curse Armor'                    ,  71, '?',%X'00000008',
    0,      0, 260,   5,   1,   0,   0,   0,   0, '0d0'  , 50),
('& Scroll~ %T| of Summon Undead'                  ,  71, '?',%X'00000010',
    0,      0, 261,   5,   1,   0,   0,   0,   0, '0d0'  , 15),
('& Scroll~ %T| of Blessing'                       ,  71, '?',%X'00000020',
    0,     15, 262,   5,   1,   0,   0,   0,   0, '0d0'  ,  1),
('& Scroll~ %T| of Holy Chant'                     ,  71, '?',%X'00000040',
    0,     40, 263,   5,   1,   0,   0,   0,   0, '0d0'  , 12),
('& Scroll~ %T| of Holy Prayer'                    ,  71, '?',%X'00000080',
    0,     80, 264,   5,   1,   0,   0,   0,   0, '0d0'  , 24),
('& Scroll~ %T| of Word-of-Recall'                 ,  71, '?',%X'00000100',
    0,    150, 265,   5,   1,   0,   0,   0,   0, '0d0'  ,  5),
('& Scroll~ %T| of *Destruction*'                  ,  71, '?',%X'00000200',
    0,    250, 266,   5,   1,   0,   0,   0,   0, '0d0'  , 40),
('& %C Potion~| of Gain Strength'                  ,  75, '!',%X'00000001',
    0,    200, 257,   4,   1,   0,   0,   0,   0, '1d1'  , 25),
('& %C Potion~| of Poison'                         ,  75, '!',%X'00000002',
    0,      0, 258,   4,   1,   0,   0,   0,   0, '1d1'  ,  3),
('& %C Potion~| of Restore Strength'               ,  75, '!',%X'00000004',
    0,    300, 259,   4,   1,   0,   0,   0,   0, '1d1'  , 40),
('& %C Potion~| of Gain Intelligence'              ,  75, '!',%X'00000008',
    0,    300, 260,   4,   1,   0,   0,   0,   0, '1d1'  , 25),
('& %C Potion~| of Lose Intelligence'              ,  75, '!',%X'00000010',
    0,      0, 261,   4,   1,   0,   0,   0,   0, '1d1'  , 25),
('& %C Potion~| of Restore Intelligence'           ,  75, '!',%X'00000020',
    0,    300, 262,   4,   1,   0,   0,   0,   0, '1d1'  , 40),
('& %C Potion~| of Gain Wisdom'                    ,  75, '!',%X'00000040',
    0,    300, 263,   4,   1,   0,   0,   0,   0, '1d1'  , 25),
('& %C Potion~| of Lose Wisdom'                    ,  75, '!',%X'00000080',
    0,      0, 264,   4,   1,   0,   0,   0,   0, '1d1'  , 25),
('& %C Potion~| of Restore Wisdom'                 ,  75, '!',%X'00000100',
    0,    300, 265,   4,   1,   0,   0,   0,   0, '1d1'  , 40),
('& %C Potion~| of Charisma'                       ,  75, '!',%X'00000200',
    0,    300, 266,   4,   1,   0,   0,   0,   0, '1d1'  , 25),
('& %C Potion~| of Ugliness'                       ,  75, '!',%X'00000400',
    0,      0, 267,   4,   1,   0,   0,   0,   0, '1d1'  , 25),
('& %C Potion~| of Restore Charisma'               ,  75, '!',%X'00000800',
    0,    300, 268,   4,   1,   0,   0,   0,   0, '1d1'  , 40),
('& %C Potion~| of Cure Light Wounds'              ,  75, '!',%X'10001000',
   50,     15, 269,   4,   1,   0,   0,   0,   0, '1d1'  ,  0),
('& %C Potion~| of Cure Light Wounds'              ,  75, '!',%X'10001000',
   50,     15, 269,   4,   1,   0,   0,   0,   0, '1d1'  ,  1),
('& %C Potion~| of Cure Light Wounds'              ,  75, '!',%X'10001000',
   50,     15, 269,   4,   1,   0,   0,   0,   0, '1d1'  ,  2),
('& %C Potion~| of Cure Serious Wounds'            ,  75, '!',%X'30002000',
  100,     40, 270,   4,   1,   0,   0,   0,   0, '1d1'  ,  3),
('& %C Potion~| of Cure Critical Wounds'           ,  75, '!',%X'70004000',
  100,    100, 271,   4,   1,   0,   0,   0,   0, '1d1'  ,  5),
('& %C Potion~| of Healing'                        ,  75, '!',%X'70008000',
  200,    200, 272,   4,   1,   0,   0,   0,   0, '1d1'  , 12),
('& %C Potion~| of Gain Constitution'              ,  75, '!',%X'00010000',
    0,    300, 273,   4,   1,   0,   0,   0,   0, '1d1'  , 25),
('& %C Potion~| of Gain Experience'                ,  75, '!',%X'00020000',
    0,   2500, 274,   4,   1,   0,   0,   0,   0, '1d1'  , 50),
('& %C Potion~| of Sleep'                          ,  75, '!',%X'10040000',
  100,      0, 275,   4,   1,   0,   0,   0,   0, '1d1'  ,  0),
('& %C Potion~| of Blindness'                      ,  75, '!',%X'00080000',
    0,      0, 276,   4,   1,   0,   0,   0,   0, '1d1'  ,  0),
('& %C Potion~| of Confusion'                      ,  75, '!',%X'00100000',
   50,      0, 277,   4,   1,   0,   0,   0,   0, '1d1'  ,  0),
('& %C Potion~| of Poison'                         ,  75, '!',%X'00200000',
    0,      0, 278,   4,   1,   0,   0,   0,   0, '1d1'  ,  3),
('& %C Potion~| of Haste Self'                     ,  75, '!',%X'00400000',
    0,     75, 279,   4,   1,   0,   0,   0,   0, '1d1'  ,  1),
('& %C Potion~| of Slowness'                       ,  75, '!',%X'00800000',
   50,      0, 280,   4,   1,   0,   0,   0,   0, '1d1'  ,  1),
('& Icky Green Potion~| of Slime Mold Juice'       ,  75, '!',%X'30000000',
  400,      2, 281,   4,   1,   0,   0,   0,   0, '1d1'  ,  0),
('& Light Brown Potion~| of Apple Juice'           ,  75, '!',%X'00000000',
  250,      1, 282,   4,   1,   0,   0,   0,   0, '1d1'  ,  0),
('& Clear Potion~| of Water'                       ,  75, '!',%X'00000000',
  200,      0, 283,   4,   1,   0,   0,   0,   0, '1d1'  ,  0),
('& %C Potion~| of Gain Dexterity'                 ,  75, '!',%X'02000000',
    0,    300, 284,   4,   1,   0,   0,   0,   0, '1d1'  , 25),
('& %C Potion~| of Restore Dexterity'              ,  75, '!',%X'04000000',
    0,    300, 285,   4,   1,   0,   0,   0,   0, '1d1'  , 40),
('& %C Potion~| of Restore Constitution'           ,  75, '!',%X'68000000',
    0,    300, 286,   4,   1,   0,   0,   0,   0, '1d1'  , 40),
('& %C Potion~| of Learning'                       ,  76, '!',%X'00000001',
    0,    200, 287,   4,   1,   0,   0,   0,   0, '1d1'  , 45),
('& %C Potion~| of Lose Memories'                  ,  76, '!',%X'00000002',
    0,      0, 288,   4,   1,   0,   0,   0,   0, '1d1'  , 10),
('& %C Potion~| of Salt Water'                     ,  76, '!',%X'00000004',
    0,      0, 289,   4,   1,   0,   0,   0,   0, '1d1'  ,  0),
('& %C Potion~| of Invulnerability'                ,  76, '!',%X'00000008',
    0,    250, 290,   4,   1,   0,   0,   0,   0, '1d1'  , 40),
('& %C Potion~| of Heroism'                        ,  76, '!',%X'00000010',
    0,     35, 291,   4,   1,   0,   0,   0,   0, '1d1'  ,  1),
('& %C Potion~| of Super Heroism'                  ,  76, '!',%X'00000020',
    0,    100, 292,   4,   1,   0,   0,   0,   0, '1d1'  ,  3),
('& %C Potion~| of Boldliness'                     ,  76, '!',%X'00000040',
    0,     10, 293,   4,   1,   0,   0,   0,   0, '1d1'  ,  1),
('& %C Potion~| of Restore Life Levels'            ,  76, '!',%X'00000080',
    0,    400, 294,   4,   1,   0,   0,   0,   0, '1d1'  , 40),
('& %C Potion~| of Resist Heat'                    ,  76, '!',%X'00000100',
    0,     30, 295,   4,   1,   0,   0,   0,   0, '1d1'  ,  1),
('& %C Potion~| of Resist Cold'                    ,  76, '!',%X'00000200',
    0,     30, 296,   4,   1,   0,   0,   0,   0, '1d1'  ,  1),
('& %C Potion~| of Detect Invisible'               ,  76, '!',%X'00000400',
    0,     50, 297,   4,   1,   0,   0,   0,   0, '1d1'  ,  3),
('& %C Potion~| of Slow Poison'                    ,  76, '!',%X'00000800',
    0,     25, 298,   4,   1,   0,   0,   0,   0, '1d1'  ,  1),
('& %C Potion~| of Neutralize Poison'              ,  76, '!',%X'00001000',
    0,     75, 299,   4,   1,   0,   0,   0,   0, '1d1'  ,  5),
('& %C Potion~| of Restore Mana'                   ,  76, '!',%X'00002000',
    0,     35, 300,   4,   1,   0,   0,   0,   0, '1d1'  , 25),
('& %C Potion~| Infra-Vision'                      ,  76, '!',%X'00004000',
    0,     20, 301,   4,   1,   0,   0,   0,   0, '1d1'  ,  3),
('& Flask~ of oil'                                 ,  77, '!',%X'00040000',
 7500,      3, 257,  10,   1,   0,   0,   0,   0, '2d6'  ,  1),
('& %M Wand| of Light^ (%P1 charges)'              ,  65, '-',%X'00000001',
    0,    200,   1,  10,   1,   0,   0,   0,   0, '1d1'  ,  2),
('& %M Wand| of Lightning Bolts^ (%P1 charges)'    ,  65, '-',%X'00000002',
    0,    600,   2,  10,   1,   0,   0,   0,   0, '1d1'  , 15),
('& %M Wand| of Frost Bolts^ (%P1 charges)'        ,  65, '-',%X'00000004',
    0,    800,   3,  10,   1,   0,   0,   0,   0, '1d1'  , 20),
('& %M Wand| of Fire Bolts^ (%P1 charges)'         ,  65, '-',%X'00000008',
    0,   1000,   4,  10,   1,   0,   0,   0,   0, '1d1'  , 30),
('& %M Wand| of Stone-to-Mud^ (%P1 charges)'       ,  65, '-',%X'00000010',
    0,    300,   5,  10,   1,   0,   0,   0,   0, '1d1'  , 12),
('& %M Wand| of Polymorph^ (%P1 charges)'          ,  65, '-',%X'00000020',
    0,    400,   6,  10,   1,   0,   0,   0,   0, '1d1'  , 20),
('& %M Wand| of Heal Monster^ (%P1 charges)'       ,  65, '-',%X'00000040',
    0,      0,   7,  10,   1,   0,   0,   0,   0, '1d1'  ,  2),
('& %M Wand| of Haste Monster^ (%P1 charges)'      ,  65, '-',%X'00000080',
    0,      0,   8,  10,   1,   0,   0,   0,   0, '1d1'  ,  2),
('& %M Wand| of Slow Monster^ (%P1 charges)'       ,  65, '-',%X'00000100',
    0,    500,   9,  10,   1,   0,   0,   0,   0, '1d1'  ,  2),
('& %M Wand| of Confuse Monster^ (%P1 charges)'    ,  65, '-',%X'00000200',
    0,    400,  10,  10,   1,   0,   0,   0,   0, '1d1'  ,  2),
('& %M Wand| of Sleep Monster^ (%P1 charges)'      ,  65, '-',%X'00000400',
    0,    500,  11,  10,   1,   0,   0,   0,   0, '1d1'  ,  7),
('& %M Wand| of Drain Life^ (%P1 charges)'         ,  65, '-',%X'00000800',
    0,   1200,  12,  10,   1,   0,   0,   0,   0, '1d1'  , 50),
('& %M Wand| of Trap/Door destruction^ (%P1 charges)',  65, '-',%X'00001000',
    0,    100,  13,  10,   1,   0,   0,   0,   0, '1d1'  , 12),
('& %M Wand| of Magic Missile^ (%P1 charges)'       ,  65, '-',%X'00002000',
    0,    200,  14,  10,   1,   0,   0,   0,   0, '1d1'  ,  2),
('& %M Wand| of Wall Building^ (%P1 charges)'      ,  65, '-',%X'00004000',
    0,    400,  15,  10,   1,   0,   0,   0,   0, '1d1'  , 25),
('& %M Wand| of Clone Monster^ (%P1 charges)'      ,  65, '-',%X'00008000',
    0,      0,  16,  10,   1,   0,   0,   0,   0, '1d1'  ,  2),
('& %M Wand| of Teleport Away^ (%P1 charges)'      ,  65, '-',%X'00010000',
    0,    350,  17,  10,   1,   0,   0,   0,   0, '1d1'  , 20),
('& %M Wand| of Disarming^ (%P1 charges)'          ,  65, '-',%X'00020000',
    0,    700,  18,  10,   1,   0,   0,   0,   0, '1d1'  , 20),
('& %M Wand| of Lightning Balls^ (%P1 charges)'    ,  65, '-',%X'00040000',
    0,   1200,  19,  10,   1,   0,   0,   0,   0, '1d1'  , 35),
('& %M Wand| of Cold Balls^ (%P1 charges)'         ,  65, '-',%X'00080000',
    0,   1500,  20,  10,   1,   0,   0,   0,   0, '1d1'  , 40),
('& %M Wand| of Fire Balls^ (%P1 charges)'         ,  65, '-',%X'00100000',
    0,   1800,  21,  10,   1,   0,   0,   0,   0, '1d1'  , 50),
('& %M Wand| of Stinking Cloud^ (%P1 charges)'     ,  65, '-',%X'00200000',
    0,    400,  22,  10,   1,   0,   0,   0,   0, '1d1'  ,  5),
('& %M Wand| of Acid Balls^ (%P1 charges)'         ,  65, '-',%X'00400000',
    0,   1650,  23,  10,   1,   0,   0,   0,   0, '1d1'  , 48),
('& %M Wand| of Wonder^ (%P1 charges)'             ,  65, '-',%X'00800000',
    0,    250,  24,  10,   1,   0,   0,   0,   0, '1d1'  ,  2),
('& %W Staff| of Light^ (%P1 charges)'             ,  55, '_',%X'00000001',
    0,    250,   1,  50,   1,   0,   0,   0,   0, '1d2'  ,  5),
('& %W Staff| of Door/Stair Location^ (%P1 charges)',  55, '_',%X'00000002',
    0,    350,   2,  50,   1,   0,   0,   0,   0, '1d2'  , 10),
('& %W Staff| of Trap Location^ (%P1 charges)'     ,  55, '_',%X'00000004',
    0,    350,   3,  50,   1,   0,   0,   0,   0, '1d2'  , 10),
('& %W Staff| of Treasure Location^ (%P1 charges)' ,  55, '_',%X'00000008',
    0,    200,   4,  50,   1,   0,   0,   0,   0, '1d2'  ,  5),
('& %W Staff| of Object Location^ (%P1 charges)'   ,  55, '_',%X'00000010',
    0,    200,   5,  50,   1,   0,   0,   0,   0, '1d2'  ,  5),
('& %W Staff| of Teleportation^ (%P1 charges)'     ,  55, '_',%X'00000020',
    0,    400,   6,  50,   1,   0,   0,   0,   0, '1d2'  , 20),
('& %W Staff| of Earthquakes^ (%P1 charges)'       ,  55, '_',%X'00000040',
    0,    350,   7,  50,   1,   0,   0,   0,   0, '1d2'  , 40),
('& %W Staff| of Summoning^ (%P1 charges)'         ,  55, '_',%X'00000080',
    0,      0,   8,  50,   1,   0,   0,   0,   0, '1d2'  , 10),
('& %W Staff| of Summoning^ (%P1 charges)'         ,  55, '_',%X'00000080',
    0,      0,   8,  50,   1,   0,   0,   0,   0, '1d2'  , 50),
('& %W Staff| of *Destruction*^ (%P1 charges)'     ,  55, '_',%X'00000200',
    0,   2500,  10,  50,   1,   0,   0,   0,   0, '1d2'  , 50),
('& %W Staff| of Starlite^ (%P1 charges)'          ,  55, '_',%X'00000400',
    0,    800,  11,  50,   1,   0,   0,   0,   0, '1d2'  , 20),
('& %W Staff| of Haste Monsters^ (%P1 charges)'    ,  55, '_',%X'00000800',
    0,      0,  12,  50,   1,   0,   0,   0,   0, '1d2'  , 10),
('& %W Staff| of Slow Monsters^ (%P1 charges)'     ,  55, '_',%X'00001000',
    0,    800,  13,  50,   1,   0,   0,   0,   0, '1d2'  , 10),
('& %W Staff| of Sleep Monsters^ (%P1 charges)'    ,  55, '_',%X'00002000',
    0,    700,  14,  50,   1,   0,   0,   0,   0, '1d2'  , 10),
('& %W Staff| of Cure Light Wounds^ (%P1 charges)' ,  55, '_',%X'00004000',
    0,    350,  15,  50,   1,   0,   0,   0,   0, '1d2'  ,  5),
('& %W Staff| of Detect Invisible^ (%P1 charges)'  ,  55, '_',%X'00008000',
    0,    200,  16,  50,   1,   0,   0,   0,   0, '1d2'  ,  5),
('& %W Staff| of Speed^ (%P1 charges)'             ,  55, '_',%X'00010000',
    0,    800,  17,  50,   1,   0,   0,   0,   0, '1d2'  , 40),
('& %W Staff| of Slowness^ (%P1 charges)'          ,  55, '_',%X'00020000',
    0,      0,  18,  50,   1,   0,   0,   0,   0, '1d2'  , 40),
('& %W Staff| of Mass Polymorph^ (%P1 charges)'    ,  55, '_',%X'00040000',
    0,    750,  19,  50,   1,   0,   0,   0,   0, '1d2'  , 46),
('& %W Staff| of Remove Curse^ (%P1 charges)'      ,  55, '_',%X'00080000',
    0,    500,  20,  50,   1,   0,   0,   0,   0, '1d2'  , 47),
('& %W Staff| of Detect Evil^ (%P1 charges)'       ,  55, '_',%X'00100000',
    0,    350,  21,  50,   1,   0,   0,   0,   0, '1d2'  , 20),
('& %W Staff| of Curing^ (%P1 charges)'            ,  55, '_',%X'00200000',
    0,   1000,  22,  50,   1,   0,   0,   0,   0, '1d2'  , 25),
('& %W Staff| of Dispell Evil^ (%P1 charges)'      ,  55, '_',%X'00400000',
    0,   1200,  23,  50,   1,   0,   0,   0,   0, '1d2'  , 49),
('& %W Staff| of Darkness^ (%P1 charges)'          ,  55, '_',%X'01000000',
    0,      0,  25,  50,   1,   0,   0,   0,   0, '1d2'  , 50),
('& %W Staff| of Darkness^ (%P1 charges)'          ,  55, '_',%X'01000000',
    0,      0,  25,  50,   1,   0,   0,   0,   0, '1d2'  ,  5),
('& Book of Magic Spells [Beginners-Magik]'        ,  90, '?',%X'0000007F',
    0,     25, 257,  30,   1,-100,   0,   0,   0, '1d1'  , 40),
('& Book of Magic Spells [Magik I]'                ,  90, '?',%X'0000FF80',
    0,    100, 258,  30,   1,-100,   0,   0,   0, '1d1'  , 40),
('& Book of Magic Spells [Magik II]'               ,  90, '?',%X'00FF0000',
    0,    400, 259,  30,   1,-100,   0,   0,   0, '1d1'  , 40),
('& Book of Magic Spells [The Mages Guide to Power]',  90, '?',%X'7F000000',
    0,    800, 261,  30,   1,-100,   0,   0,   0, '1d1'  , 40),
('& Holy Book of Prayers [Beginners Handbook]'     ,  91, '?',%X'000000FF',
    0,     25, 258,  30,   1,-100,   0,   0,   0, '1d1'  , 40),
('& Holy Book of Prayers [Words of Wisdom]'        ,  91, '?',%X'0000FF00',
    0,    100, 259,  30,   1,-100,   0,   0,   0, '1d1'  , 40),
('& Holy Book of Prayers [Chants and Blessings]'   ,  91, '?',%X'01FF0000',
    0,    300, 260,  30,   1,-100,   0,   0,   0, '1d1'  , 40),
('& Holy Book of Prayers [Exorcism and Dispelling]',  91, '?',%X'7E000000',
    0,    900, 261,  30,   1,-100,   0,   0,   0, '1d1'  , 40),
('& Small wooden chest'                            ,   2, '&',%X'0F000000',
    0,     20,   1, 250,   1,   0,   0,   0,   0, '2d3'  ,  7),
('& Large wooden chest'                            ,   2, '&',%X'15000000',
    0,     60,   4, 500,   1,   0,   0,   0,   0, '2d5'  , 15),
('& Small iron chest'                              ,   2, '&',%X'0F000000',
    0,    100,   7, 300,   1,   0,   0,   0,   0, '2d4'  , 25),
('& Large iron chest'                              ,   2, '&',%X'1F000000',
    0,    150,  10,1000,   1,   0,   0,   0,   0, '2d6'  , 35),
('& Small steel chest'                             ,   2, '&',%X'0F000000',
    0,    200,  13, 500,   1,   0,   0,   0,   0, '2d4'  , 45),
('& Large steel chest'                             ,   2, '&',%X'23000000',
    0,    250,  16,1000,   1,   0,   0,   0,   0, '2d6'  , 50),
('& Rat Skeleton'                                  ,   1, 's',%X'00000000',
    0,      0,   1,  10,   1,   0,   0,   0,   0, '1d1'  ,  1),
('& Giant Centipede Skeleton'                      ,   1, 's',%X'00000000',
    0,      0,   2,  25,   1,   0,   0,   0,   0, '1d1'  ,  1),
('Some filthy rags^ [%P6,%P4]'                     ,  36, '~',%X'00000000',
    0,      0,  99,  20,   1,   0,   0,   1,   0, '0d0'  ,  0),
('& empty bottle'                                  ,   1, '!',%X'00000000',
    0,      0,   4,   2,   1,   0,   0,   0,   0, '1d1'  ,  0),
('Some shards of pottery'                          ,   1, '~',%X'00000000',
    0,      0,   5,   5,   1,   0,   0,   0,   0, '1d1'  ,  0),
('& Human Skeleton'                                ,   1, 's',%X'00000000',
    0,      0,   7,  50,   1,   0,   0,   0,   0, '1d1'  ,  1),
('& Dwarf Skeleton'                                ,   1, 's',%X'00000000',
    0,      0,   8,  60,   1,   0,   0,   0,   0, '1d1'  ,  1),
('& Elf Skeleton'                                  ,   1, 's',%X'00000000',
    0,      0,   9,  40,   1,   0,   0,   0,   0, '1d1'  ,  1),
('& Gnome Skeleton'                                ,   1, 's',%X'00000000',
    0,      0,  10,  25,   1,   0,   0,   0,   0, '1d1'  ,  1),
('& broken set of teeth'                           ,   1, 's',%X'00000000',
    0,      0,  11,   3,   1,   0,   0,   0,   0, '1d1'  ,  0),
('& large broken bone'                             ,   1, 's',%X'00000000',
    0,      0,  12,   2,   1,   0,   0,   0,   0, '1d1'  ,  0),
('& broken stick'                                  ,   1, '~',%X'00000000',
    0,      0,  13,   3,   1,   0,   0,   0,   0, '1d1'  ,  0)
				);


	{ Items which are sold in the stores are different from dungeon }
	{ items so that identify works properly.  Note that the players }
	{ recieve their initial items from this list, so position is    }
	{ very important...                                             }

	inventory_init :=	(
('& Ration~ of Food'                               ,  80, ',',%X'00000000',
 5000,      3, 307,  10,   5,   0,   0,   0,   0, '0d0'  ,  0), {  1}
('& Hard Biscuit~'                                 ,  80, ',',%X'00000000',
  500,      1, 309,   2,   5,   0,   0,   0,   0, '0d0'  ,  0), {  2}
('& Strip~ of Beef Jerky'                          ,  80, ',',%X'00000000',
 1750,      2, 310,   2,   5,   0,   0,   0,   0, '0d0'  ,  0), {  3}
('& Pint of Fine Ale'                              ,  80, ',',%X'00000000',
  500,      1, 311,  10,   3,   0,   0,   0,   0, '0d0'  ,  0), {  4}
('& Pint of Fine Wine'                             ,  80, ',',%X'00000000',
  400,      2, 312,  10,   1,   0,   0,   0,   0, '0d0'  ,  0), {  5}
('& Dagger (Misercorde) (%P2,%P3)'                 ,  23, '|',%X'00000000',
    0,     10,   2,  15,   1,   0,   0,   0,   0, '1d4'  ,  0), {  6}
('& Dagger (Stiletto) (%P2,%P3)'                   ,  23, '|',%X'00000000',
    0,     10,   3,  12,   1,   0,   0,   0,   0, '1d4'  ,  0), {  7}
('& Bastard Sword (%P2,%P3)'                       ,  23, '|',%X'00000000',
    0,    350,   7, 140,   1,   0,   0,   0,   0, '3d4'  ,  0), {  8}
('& Broadsword (%P2,%P3)'                          ,  23, '|',%X'00000000',
    0,    255,  10, 150,   1,   0,   0,   0,   0, '2d5'  ,  0), {  9}
('& Longsword (%P2,%P3)'                           ,  23, '|',%X'00000000',
    0,    300,  18, 130,   1,   0,   0,   0,   0, '1d10' ,  0), { 10}
('& Small Sword (%P2,%P3)'                         ,  23, '|',%X'00000000',
    0,     48,  22,  75,   1,   0,   0,   0,   0, '1d6'  ,  0), { 11}
('& Broad Axe (%P2,%P3)'                           ,  21, '\',%X'00000000',
    0,    304,   4, 160,   1,   0,   0,   0,   0, '2d5'  ,  0), { 12}
('& Morningstar (%P2,%P3)'                         ,  21, '\',%X'00000000',
    0,    396,   9, 150,   1,   0,   0,   0,   0, '2d6'  ,  0), { 13}
('& Mace (%P2,%P3)'                                ,  21, '\',%X'00000000',
    0,    130,  10, 120,   1,   0,   0,   0,   0, '2d4'  ,  0), { 14}
('& War Hammer (%P2,%P3)'                          ,  21, '\',%X'00000000',
    0,    225,  11, 120,   1,   0,   0,   0,   0, '3d3'  ,  0), { 15}
('& Halberd (%P2,%P3)'                             ,  22, '/',%X'00000000',
    0,    430,   5, 190,   1,   0,   0,   0,   0, '3d4'  ,  0), { 16}
('& Pike (%P2,%P3)'                                ,  22, '/',%X'00000000',
    0,    358,   7, 160,   1,   0,   0,   0,   0, '2d5'  ,  0), { 17}
('& Spear (%P2,%P3)'                               ,  22, '/',%X'00000000',
    0,     36,   8,  50,   1,   0,   0,   0,   0, '1d6'  ,  0), { 18}
('& Short Bow (%P2)'                               ,  20, '}',%X'00000000',
    2,     50,   1,  30,   1,   0,   0,   0,   0, '0d0'  ,  0), { 19}
('& Long Bow (%P2)'                                ,  20, '}',%X'00000000',
    3,    120,   2,  40,   1,   0,   0,   0,   0, '0d0'  ,  0), { 20}
('& Light Crossbow (%P2)'                          ,  20, '}',%X'00000000',
    5,    160,  10, 110,   1,   0,   0,   0,   0, '0d0'  ,  0), { 21}
('& Sling (%P2)'                                   ,  20, '}',%X'00000000',
    1,      5,  20,   5,   1,   0,   0,   0,   0, '0d0'  ,  0), { 22}
('& Arrow~ (%P2,%P3)'                              ,  12, '{',%X'00000000',
    0,      1,   1,   2,   1,   0,   0,   0,   0, '1d4'  ,  0), { 23}
('& Bolt~ (%P2,%P3)'                               ,  11, '{',%X'00000000',
    0,      2,   1,   3,   1,   0,   0,   0,   0, '1d5'  ,  0), { 24}
('& Iron Shot~ (%P2,%P3)'                          ,  10, '{',%X'00000000',
    0,      2,   1,   5,   1,   0,   0,   0,   0, '1d3'  ,  0), { 25}
('& Pick (%P1) (%P2,%P3)'                          ,  25, '\',%X'20000000',
    1,     50,   1, 150,   1,   0,   0,   0,   0, '1d3'  ,  0), { 26}
('& Shovel (%P1) (%P2,%P3)'                        ,  25, '\',%X'20000000',
    0,     15,   2,  60,   1,   0,   0,   0,   0, '1d2'  ,  0), { 27}
('& Pair of Soft Leather Boots [%P6,%P4]'          ,  30, ']',%X'00000000',
    0,      7,   2,  30,   1,   0,   0,   2,   0, '1d1'  ,  0), { 28}
('& Pair of Hard Leather Boots [%P6,%P4]'          ,  30, ']',%X'00000000',
    0,     12,   3,  40,   1,   0,   0,   3,   0, '1d1'  ,  0), { 29}
('& Hard Leather Cap [%P6,%P4]'                    ,  33, ']',%X'00000000',
    0,     12,   2,  15,   1,   0,   0,   2,   0, '0d0'  ,  0), { 30}
('& Metal Cap [%P6,%P4]'                           ,  33, ']',%X'00000000',
    0,     30,   3,  20,   1,   0,   0,   3,   0, '1d1'  ,  0), { 31}
('& Iron Helm [%P6,%P4]'                           ,  33, ']',%X'00000000',
    0,     75,   4,  75,   1,   0,   0,   5,   0, '1d3'  ,  0), { 32}
('Soft Leather Armor [%P6,%P4]'                    ,  36, '(',%X'00000000',
    0,     18,   2,  80,   1,   0,   0,   4,   0, '0d0'  ,  0), { 33}
('Soft Studded Leather [%P6,%P4]'                  ,  36, '(',%X'00000000',
    0,     35,   3,  90,   1,   0,   0,   5,   0, '1d1'  ,  0), { 34}
('Hard Leather Armor [%P6,%P4]'                    ,  36, '(',%X'00000000',
    0,     55,   4, 100,   1,  -1,   0,   6,   0, '1d1'  ,  0), { 35}
('Hard Studded Leather [%P6,%P4]'                  ,  36, '(',%X'00000000',
    0,    100,   5, 110,   1,  -1,   0,   7,   0, '1d2'  ,  0), { 36}
('Leather Scale Mail [%P6,%P4]'                    ,  36, '(',%X'00000000',
    0,    330,   9, 140,   1,  -1,   0,  11,   0, '1d1'  ,  0), { 37}
('Metal Scale Mail [%P6,%P4]'                      ,  35, '[',%X'00000000',
    0,    430,   1, 250,   1,  -2,   0,  13,   0, '1d4'  ,  0), { 38}
('Chain Mail [%P6,%P4]'                            ,  35, '[',%X'00000000',
    0,    530,   2, 220,   1,  -2,   0,  14,   0, '1d4'  ,  0), { 39}
('Partial Plate Armor [%P6,%P4]'                   ,  35, '[',%X'00000000',
    0,    900,   9, 260,   1,  -3,   0,  22,   0, '1d6'  ,  0), { 40}
('Full Plate Armor [%P6,%P4]'                      ,  35, '[',%X'00000000',
    0,   1050,  11, 380,   1,  -3,   0,  25,   0, '2d4'  ,  0), { 41}
('& Cloak [%P6,%P4]'                               ,  32, '(',%X'00000000',
    0,      3,   1,  10,   1,   0,   0,   1,   0, '0d0'  ,  0), { 42}
('& Set of Leather Gloves [%P6,%P4]'               ,  31, ']',%X'00000000',
    0,      3,   1,   5,   1,   0,   0,   1,   0, '0d0'  ,  0), { 43}
('& Set of Gauntlets [%P6,%P4]'                    ,  31, ']',%X'00000000',
    0,     35,   2,  25,   1,   0,   0,   2,   0, '1d1'  ,  0), { 44}
('& Small Leather Shield [%P6,%P4]'                ,  34, ')',%X'00000000',
    0,     30,   1,  50,   1,   0,   0,   2,   0, '1d1'  ,  0), { 45}
('& Medium Leather Shield [%P6,%P4]'               ,  34, ')',%X'00000000',
    0,     60,   2,  75,   1,   0,   0,   3,   0, '1d2'  ,  0), { 46}
('& Small Metal Shield [%P6,%P4]'                  ,  34, ')',%X'00000000',
    0,     50,   4,  65,   1,   0,   0,   3,   0, '1d3'  ,  0), { 47}
('& Ring of Resist Fire'                           ,  45, '=',%X'00080000',
    0,    250,  11,   2,   1,   0,   0,   0,   0, '0d0'  ,  0), { 48}
('& Ring of Resist Cold'                           ,  45, '=',%X'00200000',
    0,    250,  12,   2,   1,   0,   0,   0,   0, '0d0'  ,  0), { 49}
('& Ring of Feather Falling'                       ,  45, '=',%X'04000000',
    0,    250,  13,   2,   1,   0,   0,   0,   0, '0d0'  ,  0), { 50}
('& Ring of Protection [%P4]'                      ,  45, '=',%X'00000000',
    0,    100,  24,   2,   1,   0,   0,   0,   0, '0d0'  ,  0), { 51}
('& Amulet of Charisma (%P1)'                      ,  40, '"',%X'00000020',
    0,    250,   6,   3,   1,   0,   0,   0,   0, '0d0'  ,  0), { 52}
('& Amulet of Slow Digestion'                      ,  40, '"',%X'00000080',
    0,    200,   9,   3,   1,   0,   0,   0,   0, '0d0'  ,  0), { 53}
('& Amulet of Resist Acid'                         ,  40, '"',%X'00100000',
    0,    300,  10,   3,   1,   0,   0,   0,   0, '0d0'  ,  0), { 54}
('& Scroll~ of Enchant Weapon To-Hit'              ,  70, '?',%X'00000001',
    0,    125, 300,   5,   1,   0,   0,   0,   0, '0d0'  ,  0), { 55}
('& Scroll~ of Enchant Weapon To-Dam'              ,  70, '?',%X'00000002',
    0,    125, 301,   5,   1,   0,   0,   0,   0, '0d0'  ,  0), { 56}
('& Scroll~ of Enchant Armor'                      ,  70, '?',%X'00000004',
    0,    125, 302,   5,   1,   0,   0,   0,   0, '0d0'  ,  0), { 57}
('& Scroll~ of Identify'                           ,  70, '?',%X'00000008',
    0,     50, 303,   5,   2,   0,   0,   0,   0, '0d0'  ,  0), { 58}
('& Scroll~ of Remove Curse'                       ,  70, '?',%X'00000010',
    0,    100, 304,   5,   1,   0,   0,   0,   0, '0d0'  ,  0), { 59}
('& Scroll~ of Light'                              ,  70, '?',%X'00000020',
    0,     15, 305,   5,   3,   0,   0,   0,   0, '0d0'  ,  0), { 60}
('& Scroll~ of Phase Door'                         ,  70, '?',%X'00000080',
    0,     15, 306,   5,   2,   0,   0,   0,   0, '0d0'  ,  0), { 61}
('& Scroll~ of Magic Mapping'                      ,  70, '?',%X'00000800',
    0,     40, 307,   5,   2,   0,   0,   0,   0, '0d0'  ,  0), { 62}
('& Scroll~ of Treasure Detection'                 ,  70, '?',%X'00004000',
    0,     15, 308,   5,   2,   0,   0,   0,   0, '0d0'  ,  0), { 63}
('& Scroll~ of Object Detection'                   ,  70, '?',%X'00008000',
    0,     15, 309,   5,   2,   0,   0,   0,   0, '0d0'  ,  0), { 64}
('& Scroll~ of Detect Invisible'                   ,  70, '?',%X'00080000',
    0,     15, 310,   5,   2,   0,   0,   0,   0, '0d0'  ,  0), { 65}
('& Scroll~ of Recharging'                         ,  70, '?',%X'01000000',
    0,    200, 311,   5,   1,   0,   0,   0,   0, '0d0'  ,  0), { 66}
('& Book of Magic Spells [Beginners-Magik]'        ,  90, '?',%X'0000007F',
    0,     25, 257,  30,   1,-100,   0,   0,   0, '1d1'  ,  0), { 67}
('& Book of Magic Spells [Magik I]'                ,  90, '?',%X'0000FF80',
    0,    100, 258,  30,   1,-100,   0,   0,   0, '1d1'  ,  0), { 68}
('& Book of Magic Spells [Magik II]'               ,  90, '?',%X'00FF0000',
    0,    400, 259,  30,   1,-100,   0,   0,   0, '1d1'  ,  0), { 69}
('& Book of Magic Spells [The Mages Guide to Power]',  90, '?',%X'7F000000',
    0,    800, 261,  30,   1,-100,   0,   0,   0, '1d1'  ,  0), { 70}
('& Holy Book of Prayers [Beginners Handbook]'     ,  91, '?',%X'000000FF',
    0,     25, 258,  30,   1,-100,   0,   0,   0, '1d1'  ,  0), { 71}
('& Holy Book of Prayers [Words of Wisdom]'        ,  91, '?',%X'0000FF00',
    0,    100, 259,  30,   1,-100,   0,   0,   0, '1d1'  ,  0), { 72}
('& Holy Book of Prayers [Chants and Blessings]'   ,  91, '?',%X'01FF0000',
    0,    300, 260,  30,   1,-100,   0,   0,   0, '1d1'  ,  0), { 73}
('& Holy Book of Prayers [Exorcism and Dispelling]',  91, '?',%X'7E000000',
    0,    900, 261,  30,   1,-100,   0,   0,   0, '1d1'  ,  0), { 74}
('& Potion~ of Restore Strength'                   ,  75, '!',%X'00000004',
    0,    300, 310,   4,   1,   0,   0,   0,   0, '1d1'  ,  0), { 75}
('& Potion~ of Restore Intelligence'               ,  75, '!',%X'00000020',
    0,    300, 311,   4,   1,   0,   0,   0,   0, '1d1'  ,  0), { 76}
('& Potion~ of Restore Wisdom'                     ,  75, '!',%X'00000100',
    0,    300, 312,   4,   1,   0,   0,   0,   0, '1d1'  ,  0), { 77}
('& Potion~ of Restore Charisma'                   ,  75, '!',%X'00000800',
    0,    300, 313,   4,   1,   0,   0,   0,   0, '1d1'  ,  0), { 78}
('& Potion~ of Cure Light Wounds'                  ,  75, '!',%X'10001000',
   50,     15, 314,   4,   2,   0,   0,   0,   0, '1d1'  ,  0), { 79}
('& Potion~ of Cure Serious Wounds'                ,  75, '!',%X'30002000',
  100,     40, 315,   4,   1,   0,   0,   0,   0, '1d1'  ,  0), { 80}
('& Potion~ of Cure Critical Wounds'               ,  75, '!',%X'70004000',
  100,    100, 316,   4,   1,   0,   0,   0,   0, '1d1'  ,  0), { 81}
('& Potion~ of Restore Dexterity'                  ,  75, '!',%X'04000000',
    0,    300, 317,   4,   1,   0,   0,   0,   0, '1d1'  ,  0), { 82}
('& Potion~ of Restore Constitution'               ,  75, '!',%X'68000000',
    0,    300, 318,   4,   1,   0,   0,   0,   0, '1d1'  ,  0), { 83}
('& Potion~ of Heroism'                            ,  76, '!',%X'00000010',
    0,     35, 319,   4,   2,   0,   0,   0,   0, '1d1'  ,  0), { 84}
('& Potion~ of Boldliness'                         ,  76, '!',%X'00000040',
    0,     10, 320,   4,   2,   0,   0,   0,   0, '1d1'  ,  0), { 85}
('& Wand of Light (%P1 charges)'                   ,  65, '-',%X'00000001',
    0,    200,   1,  10,   1,   0,   0,   0,   0, '1d1'  ,  2), { 86}
('& Wand of Lightning Bolts (%P1 charges)'         ,  65, '-',%X'00000002',
    0,    600,   2,  10,   1,   0,   0,   0,   0, '1d1'  ,  6), { 87}
('& Wand of Magic Missile (%P1 charges)'            ,  65, '-',%X'00002000',
    0,    200,  14,  10,   1,   0,   0,   0,   0, '1d1'  ,  2), { 88}
('& Wand of Disarming (%P1 charges)'               ,  65, '-',%X'00020000',
    0,    700,  18,  10,   1,   0,   0,   0,   0, '1d1'  , 12), { 89}
('& Wand of Lightning Balls (%P1 charges)'         ,  65, '-',%X'00040000',
    0,   1200,  19,  10,   1,   0,   0,   0,   0, '1d1'  , 20), { 90}
('& Wand of Wonder (%P1 charges)'                  ,  65, '-',%X'00800000',
    0,    250,  24,  10,   1,   0,   0,   0,   0, '1d1'  , 10), { 91}
('& Staff of Light (%P1 charges)'                  ,  55, '_',%X'00000001',
    0,    250,   1,  50,   1,   0,   0,   0,   0, '1d2'  ,  3), { 92}
('& Staff of Door/Stair Location (%P1 charges)'    ,  55, '_',%X'00000002',
    0,    350,   2,  50,   1,   0,   0,   0,   0, '1d2'  ,  7), { 93}
('& Staff of Trap Location (%P1 charges)'          ,  55, '_',%X'00000004',
    0,    350,   3,  50,   1,   0,   0,   0,   0, '1d2'  ,  7), { 94}
('& Staff of Detect Invisible (%P1 charges)'       ,  55, '_',%X'00008000',
    0,    200,  16,  50,   1,   0,   0,   0,   0, '1d2'  ,  3), { 95}
('& Potion~ of Restore Life Levels'                ,  76, '!',%X'00000080',
    0,    400, 321,   4,   1,   0,   0,   0,   0, '1d1'  ,  0), { 96}
('& Scroll~ of Blessing'                           ,  71, '?',%X'00000020',
    0,     15, 312,   5,   2,   0,   0,   0,   0, '0d0'  ,  0), { 97}
('& Scroll~ of Word-of-Recall'                     ,  71, '?',%X'00000100',
    0,    150, 313,   5,   3,   0,   0,   0,   0, '0d0'  ,  0), { 98}
('& Potion~ of Slow Poison'                        ,  76, '!',%X'00000800',
    0,     25, 322,   4,   2,   0,   0,   0,   0, '1d1'  ,  0), { 99}
('& Potion~ of Neutralize Poison'                  ,  76, '!',%X'00001000',
    0,     75, 323,   4,   1,   0,   0,   0,   0, '1d1'  ,  0), {100}
('& Wand of Stinking Cloud (%P1 charges)'          ,  65, '-',%X'00200000',
    0,    400,  22,  10,   1,   0,   0,   0,   0, '1d1'  ,  5), {101}
('& Iron Spike~'                                   ,  13, '~',%X'00000000',
    0,      1,   1,  10,   1,   0,   0,   0,   0, '1d1'  ,  1), {102}
('& Brass Lantern~ with %P5 turns of light'        ,  15, '~',%X'00000000',
 7500,     35,   2,  50,   1,   0,   0,   0,   0, '1d1'  ,  1), {103}
('& Wooden Torch~ with %P5 turns of light'         ,  15, '~',%X'00000000',
 4000,      2, 270,  30,   5,   0,   0,   0,   0, '1d1'  ,  1), {104}
('& Flask~ of oil'                                 ,  77, '!',%X'00040000',
 7500,      3, 257,  10,   5,   0,   0,   0,   0, '2d6'  ,  1)  {105}
					);

	store_choice	:=	(
	{ General Store }
(105,104,103,102,102,104,42,105,104,27,26,5,4,3,3,2,2,2,1,1,1,1,1,1,1,1
),
	{ Armory        }
(30,31,32,33,34,35,36,37,38,39,40,41,43,44,45,46,47,30,33,34,43,44,28,29,30,31
),
	{ Weaponsmith   }
( 6, 7, 8, 9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25, 6, 7,23,25,23,25
),
	{ Temple        }
(71,72,73,74,59,59,77,79,80,81,84,85,13,14,15,96,97,98,100,79,79,80,80,81,72,98
),
	{ Alchemy shop  }
(55,56,57,58,58,60,61,62,63,64,65,66,75,76,78,82,83,60,61,62,63,64,98,99,98,98
),
	{ Magic-User store}
(67,49,50,51,52,53,54,48,68,69,69,70,86,87,88,89,90,91,92,93,94,95,86,101,68,88
)
				);

	{ Each store will buy only certain items, based on TVAL }
	store_buy       :=      (
				[25,30,32,80,77,15,13],
				[30,31,33,34,35,36],
				[10,11,12,20,21,22,23],
				[21,70,71,75,76,91],
				[70,71,75,76],
				[40,45,55,65,70,71,75,76,90]
				);

	{ Store owners have different characteristics for pricing and haggling}
	{ Note: Store owners should be added in groups, one for each store    }
	owners          :=      (
('Erick the Honest       (Human)      General Store',
	  250, 0.75, 0.08, 0.04, 1, 12),
('Mauglin the Grumpy     (Dwarf)      Armory'       ,
	32000, 1.00, 0.12, 0.04, 6,  5),
('Arndal Beast-Slayer    (Half-Elf)   Weaponsmith'  ,
	10000, 0.85, 0.10, 0.05, 2,  8),
('Hardblow the Humble    (Human)      Temple'       ,
	 3500, 0.75, 0.09, 0.06, 1, 15),
('Ga-nat the Greedy      (Gnome)      Alchemist'    ,
	12000, 1.20, 0.15, 0.04, 5,  9),
('Valeria Starshine      (Elf)        Magic Shop'   ,
	32000, 0.75, 0.10, 0.05, 3, 11),
('Andy the Friendly      (Halfling)   General Store',
	  200, 0.70, 0.08, 0.05, 4, 15),
('Darg-Low the Grim      (Human)      Armory'       ,
	10000, 0.90, 0.11, 0.04, 1,  9),
('Oglign Dragon-Slayer   (Dwarf)      Weaponsmith'  ,
	32000, 0.95, 0.12, 0.04, 6,  8),
('Gunnar the Paladin     (Human)      Temple'       ,
	 5000, 0.85, 0.10, 0.05, 1, 23),
('Mauser the Chemist     (Half-Elf)   Alchemist'    ,
	10000, 0.90, 0.11, 0.05, 2,  8),
('Gopher the Great!      (Gnome)      Magic Shop'   ,
	20000, 1.15, 0.13, 0.06, 5, 10),
('Lyar-el the Comely     (Elf)        General Store',
	  300, 0.65, 0.07, 0.06, 3, 18),
('Mauglim the Horrible   (Half-Orc)   Armory'       ,
	 3000, 1.00, 0.13, 0.05, 7,  9),
('Ithyl-Mak the Beastly  (Half-Troll) Weaponsmith'  ,
	 3000, 1.10, 0.15, 0.06, 8,  8),
('Delihla the Pure       (Half-Elf)   Temple'       ,
	25000, 0.80, 0.07, 0.06, 2, 20),
('Wizzle the Chaotic     (Halfling)   Alchemist'    ,
	10000, 0.90, 0.10, 0.06, 4,  8),
('Inglorian the Mage     (Human?)     Magic Shop'   ,
	32000, 1.00, 0.10, 0.07, 1, 10)
				);

	{ Each type of character starts out with a few provisions...	}
	{ Note the the entries refer to array elements of INVENTORY_INIT array}
	player_init     :=      (
		(   1, 104,  42,   7,  33),     { Warrior       }
		(   1, 104,  42,   7,  67),     { Mage          }
		(   1, 104,  42,   7,  71),     { Priest        }
		(   1, 104,   7,  33,  67),     { Rogue         }
		(   1, 104,  42,   7,  67),     { Ranger        }
		(   1, 104,  42,   7,  71)      { Paladin       }
				);


	{ Identified objects flags					}
	object_ident := (max_objects of false);

	{ Traps are just Nasty treasures...				}
	trap_lista      :=      (
('an open pit'                                     , 102, ' ',%X'00000000',
    0,      0,   1,   0,   0,   0,   0,   0,   0, '2d6'  ,-50),
('an arrow trap'                                   , 101, '.',%X'00000000',
    0,      0,   2,   0,   0,   0,   0,   0,   0, '1d8'  ,  0),
('a covered pit'                                   , 101, '.',%X'00000000',
    0,      0,   3,   0,   0,   0,   0,   0,   0, '2d6'  ,  0),
('a trap door'                                     , 101, '.',%X'00000000',
    0,      0,   4,   0,   0,   0,   0,   0,   0, '2d8'  ,  0),
('a gas trap'                                      , 101, '.',%X'00000000',
    0,      0,   5,   0,   0,   0,   0,   0,   0, '1d4'  ,  0),
('a loose rock'                                    , 101, '.',%X'00000000',
    0,      0,   6,   0,   0,   0,   0,   0,   0, '0d0'  ,  0),
('a dart trap'                                     , 101, '.',%X'00000000',
    0,      0,   7,   0,   0,   0,   0,   0,   0, '1d4'  ,  0),
('a strange rune'                                  , 101, '.',%X'00000000',
    0,      0,   8,   0,   0,   0,   0,   0,   0, '0d0'  ,  0),
('some loose rock'                                 , 101, '.',%X'00000000',
    0,      0,   9,   0,   0,   0,   0,   0,   0, '2d6'  ,  0),
('a gas trap'                                      , 101, '.',%X'00000000',
    0,      0,  10,   0,   0,   0,   0,   0,   0, '1d4'  ,  0),
('a strange rune'                                  , 101, '.',%X'00000000',
    0,      0,  11,   0,   0,   0,   0,   0,   0, '0d0'  ,  0),
('a blackened spot'                                , 101, '.',%X'00000000',
    0,      0,  12,   0,   0,   0,   0,   0,   0, '4d6'  ,  0),
('some corroded rock'                              , 101, '.',%X'00000000',
    0,      0,  13,   0,   0,   0,   0,   0,   0, '4d6'  ,  0),
('a gas trap'                                      , 101, '.',%X'00000000',
    0,      0,  14,   0,   0,   0,   0,   0,   0, '2d6'  ,  0),
('a gas trap'                                      , 101, '.',%X'00000000',
    5,      0,  15,   0,   0,   0,   0,   0,   0, '1d4'  , 10),
('a gas trap'                                      , 101, '.',%X'00000000',
    5,      0,  16,   0,   0,   0,   0,   0,   0, '1d8'  ,  5),
('a dart trap'                                     , 101, '.',%X'00000000',
    5,      0,  17,   0,   0,   0,   0,   0,   0, '1d8'  , 10),
('a dart trap'                                     , 101, '.',%X'00000000',
    5,      0,  18,   0,   0,   0,   0,   0,   0, '1d8'  , 10)
				);

	{ Traps: Level represents the difficulty of disarming;	}
	{ and P1 represents the experienced gained when disarmed}
	trap_listb      :=      (
('an open pit'                                     , 102, ' ',%X'00000000',
    1,      0,   1,   0,   0,   0,   0,   0,   0, '2d6'  ,-50),
('an arrow trap'                                   , 102, '^',%X'00000000',
    3,      0,   2,   0,   0,   0,   0,   0,   0, '1d8'  ,-10),
('a covered pit'                                   , 102, '^',%X'00000000',
    2,      0,   3,   0,   0,   0,   0,   0,   0, '2d6'  ,-40),
('a trap door'                                     , 102, '^',%X'00000000',
    5,      0,   4,   0,   0,   0,   0,   0,   0, '2d8'  ,-25),
('a gas trap'                                      , 102, '^',%X'00000000',
    3,      0,   5,   0,   0,   0,   0,   0,   0, '1d4'  ,  5),
('a loose rock'                                    , 102, ';',%X'00000000',
    0,      0,   6,   0,   0,   0,   0,   0,   0, '0d0'  ,-90),
('a dart trap'                                     , 102, '^',%X'00000000',
    5,      0,   7,   0,   0,   0,   0,   0,   0, '1d4'  , 10),
('a strange rune'                                  , 102, '^',%X'00000000',
    5,      0,   8,   0,   0,   0,   0,   0,   0, '0d0'  ,-10),
('some loose rock'                                 , 102, '^',%X'00000000',
    5,      0,   9,   0,   0,   0,   0,   0,   0, '2d6'  ,-10),
('a gas trap'                                      , 102, '^',%X'00000000',
   10,      0,  10,   0,   0,   0,   0,   0,   0, '1d4'  ,  5),
('a strange rune'                                  , 102, '^',%X'00000000',
    5,      0,  11,   0,   0,   0,   0,   0,   0, '0d0'  ,-10),
('a blackened spot'                                , 102, '^',%X'00000000',
   10,      0,  12,   0,   0,   0,   0,   0,   0, '4d6'  , 10),
('some corroded rock'                              , 102, '^',%X'00000000',
   10,      0,  13,   0,   0,   0,   0,   0,   0, '4d6'  , 10),
('a gas trap'                                      , 102, '^',%X'00000000',
    5,      0,  14,   0,   0,   0,   0,   0,   0, '2d6'  ,  5),
('a gas trap'                                      , 102, '^',%X'00000000',
    5,      0,  15,   0,   0,   0,   0,   0,   0, '1d4'  , 10),
('a gas trap'                                      , 102, '^',%X'00000000',
    5,      0,  16,   0,   0,   0,   0,   0,   0, '1d8'  ,  5),
('a dart trap'                                     , 102, '^',%X'00000000',
    5,      0,  17,   0,   0,   0,   0,   0,   0, '1d8'  , 10),
('a dart trap'                                     , 102, '^',%X'00000000',
    5,      0,  18,   0,   0,   0,   0,   0,   0, '1d8'  , 10),
	{ Special case, see DOOR_LIST below (subvals must agree)        }
('a closed door'                                   , 105, '+',%X'00000000',
    0,      0,  19,   0,   0,   0,   0,   0,   0, '1d1'  ,  0)
				);

	{ Stores are just special traps 		}
	store_door      := (
('The entrance to the General Store.'              , 110, '1',%X'00000000',
    0,      0, 101,   0,   0,   0,   0,   0,   0, '0d0'  ,  0),
('The entrance to the Armory.'                     , 110, '2',%X'00000000',
    0,      0, 102,   0,   0,   0,   0,   0,   0, '0d0'  ,  0),
('The entrance to the Weapon Smiths.'              , 110, '3',%X'00000000',
    0,      0, 103,   0,   0,   0,   0,   0,   0, '0d0'  ,  0),
('The entrance to the Temple.'                     , 110, '4',%X'00000000',
    0,      0, 104,   0,   0,   0,   0,   0,   0, '0d0'  ,  0),
('The entrance to the Alchemy Shop.'               , 110, '5',%X'00000000',
    0,      0, 105,   0,   0,   0,   0,   0,   0, '0d0'  ,  0),
('The entrance to the Magic Shop.'                 , 110, '6',%X'00000000',
    0,      0, 106,   0,   0,   0,   0,   0,   0, '0d0'  ,  0)
				);


 scare_monster :=
('a strange rune'                                  , 102, '^',%X'00000000',
    0,      0,  99,   0,   0,   0,   0,   0,   0, '0d0'  ,-90);

 rubble :=
('some rubble'                                     , 103, ':',%X'00000000',
    0,      0,   1,   0,   0,   0,   0,   0,   0, '0d0'  ,  0);


	{ Secret door must have same subval as closed door in	}
	{ TRAP_LISTB.  See CHANGE_TRAP                          }
	door_list       :=      (
('an open door'                                    , 104,'''',%X'00000000',
    0,      0,   1,   0,   0,   0,   0,   0,   0, '1d1'  ,  0),
('a closed door'                                   , 105, '+',%X'00000000',
    0,      0,  19,   0,   0,   0,   0,   0,   0, '1d1'  ,  0),
('a secret door'                                   , 109, '#',%X'00000000',
    0,      0,  19,   0,   0,   0,   0,   0,   0, '1d1'  ,  0)
				);

 up_stair   :=
('an up staircase '                                , 107, '<',%X'00000000',
    0,      0,   1,   0,   0,   0,   0,   0,   0, '1d1'  ,  0);

 down_stair :=
('a down staircase'                                , 108, '>',%X'00000000',
    0,      0,   1,   0,   0,   0,   0,   0,   0, '1d1'  ,  0);


	{ Sets for easy testing 					}
	floor_set   := [1,2,4,5,6,7];
	wall_set    := [10,11,12];
	pwall_set   := [10,11,12,15];
	corr_set    := [4,5];
	trap_set    := [101,102,109,110];
	light_set   := [102,103,104,105,107,108,109,110];

	{ Descriptive phrases						}
	colors  :=   ('Amber','Azure','Blue','Blue Speckled','Blue Spotted',
		      'Black','Black Speckled','Black Spotted',
		      'Brown','Brown Speckled','Brown Spotted',
		      'Bubbling',
		      'Chartreuse','Clear','Cloudy',
		      'Copper','Copper Spotted','Crimson','Cyan',
		      'Dark Blue','Dark Green','Dark Red','Ecru',
		      'Gold','Gold Spotted',
		      'Green','Green Speckled','Green Spotted',
		      'Grey','Grey Spotted','Hazy','Indigo',
		      'Light Blue','Light Green','Magenta','Metallic Blue',
		      'Metallic Red','Metallic Green','Metallic Purple',
		      'Misty',
		      'Orange','Orange Speckled','Orange Spotted',
		      'Pink','Pink Speckled',
		      'Plaid','Puce','Purple','Purple Speckled',
		      'Purple Spotted','Red','Red Speckled','Red Spotted',
		      'Silver','Silver Speckled','Silver Spotted','Smokey',
		      'Tan','Tangerine','Topaz','Turquoise',
		      'Violet','Vermilion','White','White Speckled',
		      'White Spotted','Yellow'
		     );
	mushrooms:=   ('Blue','Black','Brown','Copper','Crimson',
		      'Dark blue','Dark green','Dark red','Gold',
		      'Green','Grey','Light Blue','Light Green',
		      'Orange','Pink','Plaid','Purple','Red','Tan',
		      'Turquoise','Violet','White','Yellow',
		      'Wrinkled','Wooden','Slimey','Speckled',
		      'Spotted','Furry'
		     );
	woods   :=   ('Applewood','Ashen','Aspen','Avocado wood',
		      'Balsa','Banyan','Birch','Cedar','Cherrywood',
		      'Cinnibar','Cottonwood','Cypress','Dogwood',
		      'Driftwood','Ebony','Elm wood','Eucalyptus',
		      'Grapevine','Hawthorn','Hemlock','Hickory',
		      'Iron wood','Juniper','Locust','Mahogany',
		      'Magnolia','Manzanita','Maple','Mulberry',
		      'Oak','Pecan','Persimmon','Pine','Redwood',
		      'Rosewood','Spruce','Sumac','Sycamore','Teak',
		      'Walnut','Zebra wood'
		     );
	metals  :=   ('Aluminium','Bone','Brass','Bronze','Cast Iron',
		      'Chromium','Copper','Gold','Iron','Lead',
		      'Magnesium','Molybdenum','Nickel',
		      'Pewter','Rusty','Silver','Steel','Tin',
		      'Titanium','Tungsten','Zirconium','Zinc',
		      'Aluminium Plated','Brass Plated','Copper Plated',
		      'Gold Plated','Nickel Plated','Silver Plated',
		      'Steel Plated','Tin Plated','Zinc Plated'
		     );
	rocks   :=   ('Amber','Agate','Alexandrite','Amethyst','Antlerite',
		      'Aquamarine','Argentite','Azurite','Beryl','Bloodstone',
		      'Calcite','Carnelian','Coral','Corundum','Cryolite',
		      'Diamond','Diorite','Emerald','Flint','Fluorite',
		      'Gabbro','Garnet','Granite','Gypsum','Hematite','Jade',
		      'Jasper','Kryptonite','Lapus lazuli','Limestone',
		      'Malachite','Manganite','Marble','Moonstone',
		      'Neptunite','Obsidian','Onyx','Opal','Pearl','Pyrite',
		      'Quartz','Quartzite','Rhodonite','Rhyolite','Ruby',
		      'Saphire','Sphalerite','Staurolite','Tiger eye','Topaz',
		      'Turquoise','Zircon'
		     );
	amulets :=   ('Birch','Cedar','Dogwood','Driftwood',
		      'Elm wood','Hemlock','Hickory','Mahogany',
		      'Maple','Oak','Pine','Redwood','Rosewood',
		      'Walnut','Aluminium','Bone','Brass','Bronze',
		      'Copper','Iron','Lead','Nickel','Agate','Amethyst',
		      'Diamond','Emerald','Flint','Garnet',
		      'Jade','Obsidian','Onyx','Opal','Pearl','Quartz',
		      'Ruby','Saphire','Tiger eye','Topaz','Turquoise'
		     );
	syllables := ('a','ab','ag','aks','ala','an','ankh','app',
		      'arg','arze','ash','aus','ban','bar','bat','bek',
		      'bie','bin','bit','bjor','blu','bot','bu',
		      'byt','comp','con','cos','cre','dalf','dan',
		      'den','doe','dok','eep','el','eng','er','ere','erk',
		      'esh','evs','fa','fid','for','fri','fu','gan',
		      'gar','glen','gop','gre','ha','he','hyd','i',
		      'ing','ion','ip','ish','it','ite','iv','jo',
		      'kho','kli','klis','la','lech','man','mar',
		      'me','mi','mic','mik','mon','mung','mur','nej',
		      'nelg','nep','ner','nes','nis','nih','nin','o',
		      'od','ood','org','orn','ox','oxy','pay','pet',
		      'ple','plu','po','pot','prok','re','rea','rhov',
		      'ri','ro','rog','rok','rol','sa','san','sat',
		      'see','sef','seh','shu','ski','sna','sne','snik',
		      'sno','so','sol','sri','sta','sun','ta','tab',
		      'tem','ther','ti','tox','trol','tue','turs','u',
		      'ulk','um','un','uni','ur','val','viv','vly',
		      'vom','wah','wed','werg','wex','whon','wun','x',
		      'yerg','yp','zun'
		     );

	{ Used in DESC.INC for replacing the & in descriptions		}
	vowel_set       := ['a','e','i','o','u','A','E','I','O','U'];

	{ Creatures must be defined here				}
	{       See TYPES.INC under creature_type for a complete list
		of all variables for creatures.  Some of the less obvious
		are explained below.

	Hit points:	'#1d#2' where #2 is the range of each roll and
			#1 is the number of added up rolls to make.
			Example: a creature with 5 eight-sided hit die
			is given '5d8'.

		Attack types:
		1       Normal attack
		2       Poison Strength
		3       Confusion attack
		4       Fear attack
		5       Fire attack
		6       Acid attack
		7       Cold attack
		8       Lightning attack
		9       Corrosion attack
		10      Blindness attack
		11      Paralysis attack
		12      Steal Money
		13      Steal Object
		14      Poison
		15      Lose dexterity
		16      Lose constitution
		17      Lose intelligence
		18      Lose wisdom
		19      Lose experience
		20      Aggravation
		21      Disenchants
		22      Eats food
		23      Eats light
		24      Eats charges
		99      Blank

		Attack descriptions:
		1       hits you.
		2       bites you.
		3       claws you.
		4       stings you.
		5       touches you.
		6       kicks you.
		7       gazes at you.
		8       breathes on you.
		9       spits on you.
		10      makes a horrible wail.
		11      embraces you.
		12      crawls on you.
		13      releases a cloud of spores.
		14      begs you for money.
		15      You've been slimed.
		16      crushes you.
		17      tramples you.
		18      drools on you.
		19      insults you.
		99      is repelled.

	Example:  For a creature which bites for 1d6, then stings for
		  2d4 and loss of dex you would use:
			'1 2 1d6|15 4 2d4'

		CMOVE flags:
Movement.       00000001        Move only to attack
	.       00000002        Move, attack normal
	.       00000008        20% random movement
	.       00000010        40% random movement
	.       00000020        75% random movement
Special +       00010000        Invisible movement
	+       00020000        Move through door
	+       00040000        Move through wall
	+       00080000        Move through creatures
	+       00100000        Picks up objects
	+       00200000        Multiply monster
Carries =       01000000        Carries objects.
	=       02000000        Carries gold.
	=       04000000        Has 60% of time.
	=       08000000        Has 90% of time.
	=       10000000        1d2 objects/gold.
	=       20000000        2d2 objects/gold.
	=       40000000        4d2 objects/gold.
Special ~       80000000        Win-the-Game creature.

		SPELL Flags:
Frequency       000001    1     These add up to x.  Then
(1 in x).       000002    2     if RANDINT(X) = 1 the
	.       000004    4     creature casts a spell.
	.       000008    8
Spells  =       000010  Teleport short (blink)
	=       000020  Teleport long
	=       000040  Teleport player to monster
	=       000080  Cause light wound
	=       000100  Cause serious wound
	=       000200  Hold person (Paralysis)
	=       000400  Cause blindness
	=       000800  Cause confusion
	=       001000  Cause fear
	=       002000  Summon monster
	=       004000  Summon undead
	=       008000  Slow Person
	=       010000  Drain Mana
	=       020000  Not Used
	=       040000  Not Used
Breaths +       080000  Breath Lightning
	+       100000  Breath Gas
	+       200000  Breath Acid
	+       400000  Breath Frost
	+       800000  Breath Fire

		CDEFENSE flags:
		0001    Hurt by Slay Dragon.
		0002    Hurt by Slay Monster.
		0004    Hurt by Slay Evil.
		0008    Hurt by Slay Undead.
		0010    Hurt by Frost.
		0020    Hurt by Fire.
		0040    Hurt by Poison.
		0080    Hurt by Acid.
		0100    Hurt by Light-Wand.
		0200    Hurt by Stone-to-Mud.
		0400    Not used.
		0800    Not used.
		1000    Cannot be charmed or slept.
		2000    Can be seen with infra-vision.
		4000    Max Hit points.
		8000    Not used.


	Sleep (sleep)	:	A measure in turns of how fast creature
				will notice player (on the average).
	Area of affect (aaf) :  Max range that creature is able to "notice"
				the player.
									}
	c_list  :=              (
('Filthy Street Urchin'     ,%X'0012000A',%X'00000000',%X'2034',  400,    0,
   4,   1, 1, 'p', '1d4'   , '1 14 0d0|12 5 0d0'              ,    0),
('Blubbering Idiot'         ,%X'0012000A',%X'00000000',%X'2030',  000,    0,
   6,   1, 1, 'p', '1d2'   , '1 18 0d0'                       ,    0),
('Pitiful Looking Beggar'   ,%X'0012000A',%X'00000000',%X'2030',  400,    0,
  10,   1, 1, 'p', '1d4'   , '1 14 0d0'                       ,    0),
('Mangy Looking Leper'      ,%X'0012000A',%X'00000000',%X'2030',  500,    0,
  10,   1, 1, 'p', '1d1'   , '1 14 0d0'                       ,    0),
('Squint Eyed Rogue'        ,%X'07120002',%X'00000000',%X'2034',  999,    0,
  10,   8, 1, 'p', '2d8'   , '1 1 1d6|13 5 0d0'               ,    0),
('Singing, Happy Drunk'     ,%X'06120038',%X'00000000',%X'2030',    0,    0,
  10,   1, 1, 'p', '2d3'   , '1 14 0d0'                       ,    0),
('Mean Looking Mercenary'   ,%X'0B12000A',%X'00000000',%X'2034',25000,    0,
  10,  20, 1, 'p', '5d8'   , '1 1 1d10'                       ,    0),
('Battle Scarred Veteran'   ,%X'0B12000A',%X'00000000',%X'2030',25000,    0,
  10,  30, 1, 'p', '7d8'   , '1 1 2d6'                        ,    0),
('Grey Mushroom patch'      ,%X'00000001',%X'00000000',%X'10A0',    0,    1,
   2,   1, 1, ',', '1d2'   , '3 13 1d4'                       ,    1),
('Giant Yellow Centipede'   ,%X'00000002',%X'00000000',%X'0002',  300,    2,
   8,  12, 1, 'c', '2d6'   , '1 2 1d3|1 4 1d3'                ,    1),
('Giant White Centipede'    ,%X'0000000A',%X'00000000',%X'0002',  400,    2,
   7,  10, 1, 'c', '3d5'   , '1 2 1d2|1 4 1d2'                ,    1),
('White Icky-Thing'         ,%X'00000012',%X'00000000',%X'0022',  100,    2,
  12,   7, 1, 'i', '3d5'   , '1 5 1d2'                        ,    1),
('Clear Icky-Thing'         ,%X'00010002',%X'00000000',%X'0022',  100,    1,
  12,   6, 1, 'i', '2d5'   , '1 5 1d2'                        ,    1),
('Giant White Mouse'        ,%X'0020000A',%X'00000000',%X'2070',  200,    1,
   8,   4, 1, 'r', '1d3'   , '1 2 1d2'                        ,    1),
('Large Brown Snake'        ,%X'0000000A',%X'00000000',%X'00B0',  999,    3,
   4,  35, 0, 'R', '4d6'   , '1 2 1d3|1 16 1d4'               ,    1),
('Large White Snake'        ,%X'00000012',%X'00000000',%X'00B0',  999,    2,
   4,  30, 1, 'R', '3d6'   , '1 2 1d1'                        ,    1),
('Kobold'                   ,%X'07020002',%X'00000000',%X'2032',  100,    5,
  20,  16, 1, 'k', '3d7'   , '1 1 1d6'                        ,    1),
('White Worm mass'          ,%X'00200022',%X'00000000',%X'01B0',    1,    2,
   6,   1, 0, 'w', '4d4'   , '14 12 1d2'                      ,    1),
('Floating Eye'             ,%X'00000001',%X'0001000D',%X'2102',  100,    1,
   2,   6, 1, 'e', '3d6'   , '11 7 0d0'                       ,    1),
('Shrieker Mushroom patch'  ,%X'00000001',%X'00000000',%X'10A0',    0,    1,
   2,   1, 1, ',', '1d1'   , '20 10 0d0'                      ,    2),
('Blubbering Icky-Thing'    ,%X'0B180012',%X'00000000',%X'0022',  100,    8,
  10,   4, 1, 'i', '5d8'   , '14 12 1d4|22 12 0d0'            ,    2),
('Metallic Green Centipedes',%X'00000012',%X'00000000',%X'0000',  100,    3,
   5,   4, 2, 'c', '4d4'   , '1 12 1d1'                       ,    2),
('Novice Warrior'           ,%X'07020002',%X'00000000',%X'2030',   50,    6,
  20,  16, 1, 'p', '9d4'   , '1 1 1d7'                        ,    2),
('Novice Rogue'             ,%X'07020002',%X'00000000',%X'2030',   50,    6,
  20,  12, 1, 'p', '8d4'   , '1 1 1d6|12 5 0d0'               ,    2),
('Novice Priest'            ,%X'07020002',%X'0000108C',%X'2030',  100,    7,
  20,  10, 1, 'p', '7d4'   , '1 1 1d5'                        ,    2),
('Novice Mage'              ,%X'07020002',%X'0000089C',%X'2030',   50,    7,
  20,   6, 1, 'p', '6d4'   , '1 1 1d4'                        ,    2),
('Yellow Mushroom patch'    ,%X'00000001',%X'00000000',%X'10A0',    0,    2,
   2,   1, 1, ',', '1d1'   , '4 13 1d6'                       ,    2),
('White Jelly'              ,%X'0B000001',%X'00000000',%X'01A0',  999,   10,
   2,   1, 2, 'J', '8d8'   , '14 5 1d2'                       ,    2),
('Giant Green Frog'         ,%X'0000000A',%X'00000000',%X'0082',  300,    6,
  14,   8, 1, 'f', '2d8'   , '1 2 1d3'                        ,    2),
('Giant Black Ant'          ,%X'0000000A',%X'00000000',%X'0002',  400,    8,
  14,  20, 1, 'a', '3d6'   , '1 2 1d4'                        ,    2),
('White Harpy'              ,%X'00000012',%X'00000000',%X'2036',  100,    5,
  16,  17, 1, 'h', '2d5'   , '1 3 1d1|1 3 1d1|1 2 1d2'        ,    2),
('Blue Yeek'                ,%X'07020002',%X'00000000',%X'2030',  100,    4,
  18,  14, 1, 'y', '2d6'   , '1 1 1d5'                        ,    2),
('Green Worm mass'          ,%X'00200022',%X'00000000',%X'01B0',  100,    3,
   7,   3, 0, 'w', '6d4'   , '9 12 1d3'                       ,    2),
('Large Black Snake'        ,%X'0000000A',%X'00000000',%X'00B0',  750,    9,
   5,  38, 0, 'R', '4d8'   , '1 2 1d4|1 16 1d6'               ,    2),
('Poltergeist'              ,%X'0F15003A',%X'0000001F',%X'100C',  750,    6,
   8,  15, 3, 'G', '2d5'   , '4 1 1d1'                        ,    3),
('Metallic Blue Centipedes' ,%X'00000012',%X'00000000',%X'0000',  150,    7,
   6,   6, 2, 'c', '4d5'   , '1 12 1d2'                       ,    3),
('Giant White Lice'         ,%X'00200022',%X'00000000',%X'01F0',  100,    1,
   6,   5, 2, 'l', '1d1'   , '1 2 1d1'                        ,    3),
('Black Naga'               ,%X'0710000A',%X'00000000',%X'20E6', 1200,   20,
  16,  40, 1, 'n', '6d8'   , '1 16 1d8'                       ,    3),
('Spotted Mushroom patch'   ,%X'00000001',%X'00000000',%X'10A0',    0,    3,
   2,   1, 1, ',', '1d1'   , '14 13 2d4'                      ,    3),
('Yellow Jelly'             ,%X'0B000001',%X'0001000F',%X'01A0',  999,   12,
   2,   1, 2, 'J', '10d8'  , '14 5 1d3'                       ,    3),
('Scruffy looking Hobbit'   ,%X'05020002',%X'00000000',%X'2030',  100,    4,
  16,   8, 1, 'p', '3d5'   , '1 1 1d4|12 5 0d0'               ,    3),
('Huge Brown Bat'           ,%X'00000022',%X'00000000',%X'2160',  400,    4,
   8,  12, 3, 'b', '2d6'   , '1 2 1d2'                        ,    3),
('Giant White Ant'          ,%X'00000002',%X'00000000',%X'0002',  800,    7,
   8,  16, 1, 'a', '3d6'   , '1 2 1d4'                        ,    3),
('Yellow Mold'              ,%X'0B000001',%X'00000000',%X'10A0',  999,    9,
   2,  10, 1, 'm', '8d8'   , '1 1 1d4'                        ,    3),
('Metallic Red Centipedes'  ,%X'0000000A',%X'00000000',%X'0000',  200,   12,
   8,   9, 2, 'c', '4d8'   , '1 12 1d2'                       ,    3),
('Yellow Worm mass'         ,%X'00200022',%X'00000000',%X'01B0',  100,    4,
   7,   4, 0, 'w', '4d8'   , '15 12 1d3'                      ,    3),
('Large Grey Snake'         ,%X'0000000A',%X'00000000',%X'00B0',  700,   10,
   5,  40, 0, 'R', '6d8'   , '1 2 1d4|1 16 1d6'               ,    3),
('Radiation Eye'            ,%X'00000001',%X'0001000B',%X'2102',  100,    6,
   2,   6, 1, 'e', '3d6'   , '2 7 1d6'                        ,    3),
('Drooling Harpy'           ,%X'00000012',%X'00000000',%X'2036',  100,    7,
  16,  22, 1, 'h', '2d8'   , '1 3 1d1|1 3 1d1|1 2 1d2|1 18 0d0',   3),
('Silver Mouse'             ,%X'0020000A',%X'00000000',%X'2072',  100,    1,
   8,   5, 1, 'r', '1d1'   , '23 1 1d1'                       ,    4),
('Black Mushroom patch'     ,%X'00000001',%X'00000000',%X'10A0',    0,    8,
   2,   1, 1, ',', '8d8'   , '1 13 1d3'                       ,    4),
('Blue Jelly'               ,%X'0B000001',%X'00000000',%X'01A0',  999,   14,
   2,   1, 1, 'J', '12d8'  , '7 5 1d6'                        ,    4),
('Creeping Copper Coins'    ,%X'00000002',%X'00000000',%X'0002',  100,    9,
   3,  24, 0, '$', '7d8'   , '1 2 1d4|14 8 2d4'               ,    4),
('Giant White Rat'          ,%X'0020000A',%X'00000000',%X'2070',  300,    1,
   8,   7, 1, 'r', '2d2'   , '14 2 1d3'                       ,    4),
('Giant Black Centipede'    ,%X'00000022',%X'00000000',%X'0002',  300,   11,
   8,  20, 1, 'c', '5d8'   , '1 2 1d2|1 4 1d2'                ,    4),
('Giant Blue Centipede'     ,%X'00000002',%X'00000000',%X'0002',  500,   10,
   8,  20, 1, 'c', '4d8'   , '1 2 1d3|1 4 1d4'                ,    4),
('Blue Worm mass'           ,%X'00200022',%X'00000000',%X'01B0',  100,    5,
   7,  12, 0, 'w', '5d8'   , '7 12 1d4'                       ,    4),
('Large Grey Snake'         ,%X'0000000A',%X'00000000',%X'00B0',  500,   14,
   6,  41, 0, 'R', '6d8'   , '1 2 1d5|1 16 1d8'               ,    4),
('Jackal'                   ,%X'00000012',%X'00000000',%X'2030',  300,    8,
  12,  16, 1, 'j', '3d8'   , '1 2 1d6'                        ,    4),
('Green Naga'               ,%X'0710000A',%X'00000000',%X'2066', 1200,   30,
  20,  40, 1, 'n', '9d8'   , '1 16 1d8|6 9 2d6'               ,    5),
('Green Glutton Ghost'      ,%X'0F150032',%X'0000003F',%X'100C',  100,   15,
  10,  20, 3, 'G', '3d6'   , '22 15 1d1'                      ,    5),
('White Mushroom patch'     ,%X'00000001',%X'00000000',%X'10A0',    0,    5,
   2,   1, 1, ',', '1d1'   , '11 13 2d4'                      ,    5),
('Green Jelly'              ,%X'0B000001',%X'00000000',%X'01A0',  999,   18,
   2,   1, 2, 'J', '22d8'  , '9 5 1d2'                        ,    5),
('Skeleton Kobold'          ,%X'00020002',%X'00000000',%X'000C',  400,   12,
  20,  26, 1, 's', '5d8'   , '1 1 1d6'                        ,    5),
('Silver Jelly'             ,%X'0B000001',%X'00000000',%X'00B0',    1,   15,
   2,  25, 1, 'J', '20d8'  , '23 5 1d3'                       ,    5),
('Giant Black Frog'         ,%X'0000000A',%X'00000000',%X'00A2',  400,   12,
  12,  18, 1, 'f', '4d8'   , '1 2 1d6'                        ,    5),
('Grey Icky-Thing'          ,%X'00000012',%X'00000000',%X'0022',  150,   10,
  14,  12, 1, 'i', '4d8'   , '1 5 1d5'                        ,    5),
('Disenchanter Eye'         ,%X'00000009',%X'00010009',%X'2102',  100,   20,
   4,  10, 0, 'e', '7d8'   , '21 7 0d0'                       ,    5),
('Black Yeek'               ,%X'07020002',%X'00000000',%X'2030',  100,    8,
  18,  16, 1, 'y', '2d8'   , '1 1 1d5'                        ,    5),
('Red Worm mass'            ,%X'00200022',%X'00000000',%X'21B0',  100,    6,
   7,  12, 0, 'w', '5d8'   , '5 12 1d6'                       ,    5),
('Giant House Fly'          ,%X'00000022',%X'00000000',%X'0062',  200,   10,
  12,  16, 3, 'F', '3d8'   , '1 2 1d2'                        ,    5),
('Copperhead Snake'         ,%X'00000012',%X'00000000',%X'00B0',    1,   15,
   6,  20, 1, 'R', '4d6'   , '14 2 2d4'                       ,    5),
('Rot Jelly'                ,%X'0B000001',%X'00000000',%X'00B0',    1,   15,
   2,  30, 1, 'J', '20d8'  , '22 5 2d3'                       ,    5),
('Purple Mushroom patch'    ,%X'00000001',%X'00000000',%X'10A0',    0,   12,
   2,   1, 2, ',', '1d1'   , '16 13 1d2'                      ,    6),
('Brown Mold'               ,%X'0B000001',%X'00000000',%X'10A0',  999,   20,
   2,  12, 1, 'm', '15d8'  , '3 1 1d4'                        ,    6),
('Giant Brown Bat'          ,%X'0000001A',%X'00000000',%X'2160',  300,   10,
  10,  15, 3, 'b', '3d8'   , '1 2 1d3'                        ,    6),
('Creeping Silver Coins'    ,%X'00000002',%X'00000000',%X'0002',  100,   18,
   4,  30, 0, '$', '12d8'  , '1 2 1d6|14 8 2d6'               ,    6),
('Orc'                      ,%X'0B020002',%X'00000000',%X'2036',  300,   16,
  20,  32, 1, 'o', '9d8'   , '1 1 1d8'                        ,    6),
('Grey Harpy'               ,%X'00000012',%X'00000000',%X'2036',  100,   14,
  16,  20, 2, 'h', '3d8'   , '1 3 1d2|1 3 1d2|1 2 1d2'        ,    6),
('Blue Icky-Thing'          ,%X'00000012',%X'00000000',%X'0022',  200,   12,
  18,  14, 1, 'i', '4d8'   , '7 5 2d3'                        ,    6),
('Rattlesnake'              ,%X'00000012',%X'00000000',%X'00B0',    1,   20,
   6,  24, 1, 'R', '6d7'   , '14 2 2d5'                       ,    6),
('Bloodshot Eye'            ,%X'00000001',%X'00010007',%X'2102',  100,   15,
   2,   6, 1, 'e', '4d8'   , '10 7 2d6'                       ,    7),
('Red Naga'                 ,%X'0710000A',%X'00000000',%X'20E6', 1200,   40,
  20,  40, 1, 'n', '11d8'  , '1 16 1d10|2 2 1d4'              ,    7),
('Red Jelly'                ,%X'0B000001',%X'00000000',%X'01A0',  999,   26,
   2,   1, 1, 'J', '26d8'  , '2 5 1d5'                        ,    7),
('Giant Red Frog'           ,%X'0000000A',%X'00000000',%X'00A2',  500,   16,
  12,  16, 1, 'f', '5d8'   , '2 2 2d4'                        ,    7),
('Green Icky-Thing'         ,%X'00000012',%X'00000000',%X'0022',  200,   18,
  14,  12, 1, 'i', '5d8'   , '9 5 2d5'                        ,    7),
('Zombie Kobold'            ,%X'00020002',%X'00000000',%X'002E',  300,   14,
  20,  14, 1, 'z', '6d8'   , '1 1 1d2|1 1 1d2'                ,    7),
('Lost Soul'                ,%X'0015001A',%X'0001002F',%X'100C',  100,   18,
   4,  10, 1, 'G', '2d8'   , '1 1 2d2|18 5 0d0'               ,    7),
('Greedy little Gnome'      ,%X'0B020002',%X'00000000',%X'2030',  100,   13,
  18,  14, 1, 'p', '3d8'   , '1 1 1d7|13 5 0d0'               ,    7),
('Giant Green Fly'          ,%X'00000022',%X'00000000',%X'0062',  500,   15,
  12,  14, 2, 'F', '3d8'   , '1 2 1d4'                        ,    7),
('Brown Yeek'               ,%X'07020002',%X'00000000',%X'2030',  100,   11,
  18,  18, 1, 'y', '3d8'   , '1 1 1d6'                        ,    8),
('Green Mold'               ,%X'0B000001',%X'00000000',%X'10A0',  750,   28,
   2,  14, 1, 'm', '21d8'  , '4 1 1d4'                        ,    8),
('Skeleton Orc'             ,%X'00020002',%X'00000000',%X'100C',  400,   26,
  20,  36, 1, 's', '10d8'  , '1 1 2d5'                        ,    8),
('Seedy looking Human'      ,%X'13020002',%X'00000000',%X'2034',  200,   22,
  20,  26, 1, 'p', '8d8'   , '1 1 3d4'                        ,    8),
('Red Icky-Thing'           ,%X'00000012',%X'00000000',%X'0022',  200,   22,
  14,  18, 2, 'i', '4d8'   , '1 5 1d3|6 8 1d5'                ,    8),
('Bandit'                   ,%X'13020002',%X'00000000',%X'2030',  999,   26,
  20,  24, 1, 'p', '8d8'   , '1 1 2d4|12 5 0d0'               ,    8),
('Yeti'                     ,%X'00020002',%X'00000000',%X'2024',  100,   30,
  20,  24, 1, 'Y', '11d8'  , '1 3 1d3|1 3 1d3|1 2 1d4'        ,    9),
('Bloodshot Icky-Thing'     ,%X'0000000A',%X'0001000B',%X'2022',  800,   24,
  14,  18, 1, 'i', '7d8'   , '1 5 1d4|9 8 2d4'                ,    9),
('Giant Grey Rat'           ,%X'0020000A',%X'00000000',%X'2070',  200,    2,
  10,  12, 1, 'r', '2d3'   , '14 2 1d4'                       ,    9),
('Black Harpy'              ,%X'0000000A',%X'00000000',%X'2036',  100,   19,
  16,  22, 2, 'h', '3d8'   , '1 3 1d2|1 3 1d2|1 2 1d3'        ,    9),
('Giant Black Bat'          ,%X'00000012',%X'00000000',%X'2060',  250,   16,
  12,  18, 3, 'b', '2d8'   , '1 2 1d6'                        ,    9),
('Clear Yeek'               ,%X'07030002',%X'00000000',%X'0030',  100,   14,
  18,  24, 1, 'y', '3d6'   , '1 1 1d5'                        ,    9),
('Orc Shaman'               ,%X'0B020002',%X'00008085',%X'2036',  200,   30,
  20,  15, 1, 'o', '7d8'   , '1 1 1d6'                        ,    9),
('Giant Red Ant'            ,%X'00000002',%X'00000000',%X'0002',  600,   22,
  12,  34, 1, 'a', '4d8'   , '1 2 1d4|2 4 1d4'                ,    9),
('King Cobra'               ,%X'00000012',%X'00000000',%X'00B0',    1,   28,
   8,  30, 1, 'R', '8d8'   , '10 9 1d2|14 2 3d4'              ,    9),
('Clear Mushroom patch'     ,%X'00210001',%X'00000000',%X'00A0',    0,    1,
   4,   1, 2, ',', '1d1'   , '1 13 1d1'                       ,   10),
('Giant White Tick'         ,%X'0000000A',%X'00000000',%X'0022',  200,   27,
  12,  40, 0, 't', '15d8'  , '14 2 2d6'                       ,   10),
('Hairy Mold'               ,%X'0B000001',%X'00000000',%X'10A0',  700,   32,
   2,  15, 1, 'm', '15d8'  , '14 1 1d3'                       ,   10),
('Disenchanter Mold'        ,%X'0B000001',%X'0001000B',%X'10A0',  100,   40,
   2,  20, 1, 'm', '16d8'  , '21 5 1d6'                       ,   10),
('Giant Red Centipede'      ,%X'00000002',%X'00000000',%X'0082',  500,   24,
  12,  26, 2, 'c', '3d8'   , '1 2 1d2|14 4 1d2'               ,   10),
('Creeping Gold Coins'      ,%X'00000002',%X'00000000',%X'0002',  100,   32,
   5,  36, 0, '$', '18d8'  , '1 2 2d5|14 8 3d5'               ,   10),
('Giant Fruit Fly'          ,%X'00200022',%X'00000000',%X'0062',  500,    4,
   8,  14, 2, 'F', '2d2'   , '1 2 1d2'                        ,   10),
('Brigand'                  ,%X'13020002',%X'00000000',%X'2030',  100,   35,
  20,  32, 1, 'p', '9d8'   , '1 1 2d4|13 5 0d0'               ,   10),
('Orc Zombie'               ,%X'00020002',%X'00000000',%X'102E',  250,   30,
  20,  24, 1, 'z', '11d8'  , '1 1 1d4|1 1 1d4'                ,   11),
('Orc Warrior'              ,%X'0F020002',%X'00000000',%X'2036',  250,   34,
  20,  36, 1, 'o', '11d8'  , '1 1 2d6'                        ,   11),
('Vorpal Bunny'             ,%X'0020000A',%X'00000000',%X'2070',  300,    2,
   8,  10, 2, 'r', '2d3'   , '1 2 1d5'                        ,   11),
('Nasty little Gnome'       ,%X'0B020002',%X'000020B5',%X'2030',   10,   32,
  18,  10, 1, 'p', '4d8'   , '1 1 1d5'                        ,   11),
('Hobgoblin'                ,%X'0F020002',%X'00000000',%X'2036',  300,   38,
  20,  38, 1, 'H', '12d8'  , '1 1 1d10'                       ,   11),
('Black Mamba'              ,%X'00000012',%X'00000000',%X'00B0',    1,   40,
  20,  32, 2, 'R', '10d8'  , '14 2 4d4'                       ,   12),
('Grape Jelly'              ,%X'0B000001',%X'0001000B',%X'01A0',  999,   60,
   2,   1, 1, 'J', '52d8'  , '19 5 5d8'                       ,   12),
('Master Yeek'              ,%X'07020002',%X'00008018',%X'2030',  100,   28,
  18,  24, 1, 'y', '5d8'   , '1 1 1d8'                        ,   12),
('Priest'                   ,%X'13020002',%X'00000285',%X'2030',  400,   36,
  20,  22, 1, 'p', '7d8'   , '1 1 2d3'                        ,   12),
('Giant Clear Ant'          ,%X'00010002',%X'00000000',%X'0102',  600,   24,
  12,  18, 1, 'a', '3d7'   , '1 2 1d4'                        ,   12),
('Air Spirit'               ,%X'00030022',%X'00000000',%X'0010',  200,   40,
  12,  20, 3, 'E', '5d8'   , '1 1 1d3'                        ,   12),
('Skeleton Human'           ,%X'00020002',%X'00000000',%X'100C',  300,   38,
  20,  30, 1, 's', '12d8'  , '1 1 1d8'                        ,   12),
('Human Zombie'             ,%X'00020002',%X'00000000',%X'102E',  200,   34,
  20,  24, 1, 'z', '11d8'  , '1 1 1d4|1 1 1d4'                ,   12),
('Moaning Spirit'           ,%X'0F15000A',%X'0001002E',%X'100C',  300,   44,
   6,  20, 1, 'G', '4d8'   , '4 10 0d0|15 5 1d8'              ,   12),
('Swordsman'                ,%X'13020002',%X'00000000',%X'2030',  999,   40,
  20,  34, 1, 'p', '11d8'  , '1 1 3d5'                        ,   12),
('Killer Brown Beetle'      ,%X'0000000A',%X'00000000',%X'0002',  300,   38,
  10,  40, 1, 'K', '13d8'  , '1 2 3d4'                        ,   13),
('Ogre'                     ,%X'07020002',%X'00000000',%X'2036',  300,   42,
  20,  32, 1, 'o', '13d8'  , '1 1 2d8'                        ,   13),
('Giant Red Speckled Frog'  ,%X'0000000A',%X'00000000',%X'00A2',  300,   32,
  12,  20, 1, 'f', '6d8'   , '1 2 3d4'                        ,   13),
('Magic User'               ,%X'13020002',%X'00002413',%X'2030',  100,   35,
  20,  10, 1, 'p', '7d8'   , '1 1 2d2'                        ,   13),
('Black Orc'                ,%X'0B020002',%X'00000000',%X'2036',  200,   40,
  20,  36, 1, 'o', '12d8'  , '1 1 3d4'                        ,   13),
('Giant Long-Eared Bat'     ,%X'0000001A',%X'00000000',%X'2160',  200,   20,
  12,  20, 3, 'b', '5d8'   , '1 2 1d4|1 3 1d2|1 3 1d2'        ,   13),
('Giant Gnat'               ,%X'00200022',%X'00000000',%X'0062',  100,    1,
   8,   4, 3, 'F', '1d2'   , '1 2 1d1'                        ,   13),
('Killer Green Beetle'      ,%X'00000012',%X'00000000',%X'0002',  300,   46,
  12,  45, 1, 'K', '16d8'  , '1 2 4d4'                        ,   14),
('Giant Flea'               ,%X'00200022',%X'00000000',%X'0062',  100,    1,
   8,  25, 2, 'F', '2d2'   , '1 2 1d2'                        ,   14),
('Giant White Dragon Fly'   ,%X'00000012',%X'0040000A',%X'0060',  500,   54,
  20,  20, 1, 'F', '5d8'   , '7 2 1d6'                        ,   14),
('Hill Giant'               ,%X'07020002',%X'00000000',%X'2034',  500,   52,
  20,  36, 1, 'P', '16d8'  , '1 1 3d6'                        ,   14),
('Skeleton Hobgoblin'       ,%X'00020002',%X'00000000',%X'100C',  300,   46,
  20,  34, 1, 's', '13d8'  , '1 1 2d5'                        ,   14),
('Flesh Golem'              ,%X'00020002',%X'00000000',%X'11B0',  100,   48,
  12,  10, 1, 'g', '12d8'  , '1 1 1d6|1 1 1d6'                ,   14),
('White Dragon Bat'         ,%X'00000012',%X'00400004',%X'2150',  500,   40,
  12,  20, 3, 'b', '2d6'   , '7 2 1d3'                        ,   14),
('Giant Black Lice'         ,%X'00200012',%X'00000000',%X'01F2',  100,    1,
   6,   7, 2, 'l', '1d1'   , '1 2 1d2'                        ,   14),
('Guardian Naga'            ,%X'1710000A',%X'00000000',%X'20E6', 1200,   60,
  20,  50, 1, 'n', '24d8'  , '1 16 2d8|1 2 1d8'               ,   15),
('Giant Grey Bat'           ,%X'00000012',%X'00000000',%X'2160',  150,   22,
  12,  22, 3, 'b', '4d8'   , '1 2 1d6|1 3 1d2|1 3 1d2'        ,   15),
('Giant Clear Centipede'    ,%X'00010002',%X'00000000',%X'0002',  300,   30,
  10,  30, 1, 'c', '5d8'   , '1 2 2d4|1 4 2d4'                ,   15),
('Giant Yellow Tick'        ,%X'0000000A',%X'00000000',%X'0022',  200,   48,
  12,  48, 0, 't', '20d8'  , '14 2 3d9'                       ,   15),
('Giant Ebony Ant'          ,%X'00200002',%X'00000000',%X'0002',  600,    3,
  12,  24, 1, 'a', '3d4'   , '1 2 2d3'                        ,   15),
('Frost Giant'              ,%X'07020002',%X'00000000',%X'0024',  500,   54,
  20,  38, 1, 'P', '17d8'  , '7 1 3d6'                        ,   15),
('Clay Golem'               ,%X'00020002',%X'00000000',%X'1200',  100,   50,
  12,  20, 1, 'g', '14d8'  , '1 1 1d8|1 1 1d8'                ,   15),
('Huge White Bat'           ,%X'00200012',%X'00000000',%X'2160',  400,    3,
   7,  12, 2, 'b', '3d8'   , '1 2 1d6'                        ,   15),
('Giant Tan Bat'            ,%X'00000012',%X'00000000',%X'2160',  400,   18,
  12,  18, 2, 'b', '3d8'   , '4 2 1d2|1 3 1d1|1 3 1d1'        ,   15),
('Violet Mold'              ,%X'0B000001',%X'00010009',%X'10A0',  700,   50,
   2,  15, 1, 'm', '17d8'  , '11 1 1d2'                       ,   15),
('Umber Hulk'               ,%X'00020002',%X'00000000',%X'2126',  100,   75,
  20,  20, 1, 'U', '20d8'  , '3 7 0d0|1 1 1d6|1 1 1d6|1 2 2d6',   16),
('Gelatinous Cube'          ,%X'2F18000A',%X'00000000',%X'00A2',    1,   36,
  12,  18, 0, 'C', '45d8'  , '6 5 1d10'                       ,   16),
('Giant Black Rat'          ,%X'0020000A',%X'00000000',%X'2070',  200,    3,
   8,  16, 1, 'r', '3d4'   , '14 2 1d5'                       ,   16),
('Giant Green Dragon Fly'   ,%X'00000012',%X'0010000A',%X'0070',  500,   58,
  20,  20, 1, 'F', '5d8'   , '14 2 1d6'                       ,   16),
('Fire Giant'               ,%X'07020002',%X'00000000',%X'2014',  500,   62,
  20,  40, 1, 'P', '20d8'  , '5 1 3d7'                        ,   16),
('Green Dragon Bat'         ,%X'00000012',%X'00100004',%X'2150',  500,   44,
  12,  22, 3, 'b', '2d7'   , '14 2 1d3'                       ,   16),
('Quasit'                   ,%X'1103000A',%X'000010FA',%X'0004',  200,   48,
  20,  30, 1, 'q', '5d8'   , '15 2 1d6|1 3 1d3|1 3 1d3'       ,   16),
('Troll'                    ,%X'07020002',%X'00000000',%X'2026',  400,   64,
  20,  40, 1, 'T', '17d8'  , '1 1 1d4|1 1 1d4|1 2 1d6'        ,   17),
('Water Spirit'             ,%X'0000000A',%X'00000000',%X'0020',  400,   58,
  12,  28, 2, 'E', '8d8'   , '1 1 2d4'                        ,   17),
('Giant Brown Scorpion'     ,%X'0000000A',%X'00000000',%X'0002',  200,   62,
  14,  44, 1, 'S', '11d8'  , '1 2 2d4|2 4 1d7'                ,   17),
('Earth Spirit'             ,%X'0000000A',%X'00000000',%X'0200',  500,   64,
  10,  40, 1, 'E', '13d8'  , '1 1 1d8|1 1 1d8'                ,   17),
('Fire Spirit'              ,%X'00000002',%X'00000000',%X'2010',  200,   66,
  20,  30, 2, 'E', '10d8'  , '5 1 2d6'                        ,   18),
('Urik-Hai Orc'             ,%X'0B020002',%X'00000000',%X'2036',  200,   68,
  20,  42, 1, 'o', '14d8'  , '1 1 3d5'                        ,   18),
('Stone Giant'              ,%X'07020002',%X'00000000',%X'2204',  500,   80,
  20,  40, 1, 'P', '22d8'  , '1 1 3d8'                        ,   18),
('Stone Golem'              ,%X'00020002',%X'00000000',%X'1200',  100,  100,
  12,  75, 0, 'g', '28d8'  , '1 1 1d10|1 1 1d10'              ,   19),
('Grey Ooze'                ,%X'071A0022',%X'00000000',%X'00A2',    1,   40,
  15,  10, 1, 'O', '6d8'   , '7 5 2d6'                        ,   19),
('Disenchanter Ooze'        ,%X'071A0022',%X'00000000',%X'00A2',    1,   50,
  15,  15, 1, 'O', '6d8'   , '21 5 0d0'                       ,   19),
('Giant Spotted Rat'        ,%X'0020000A',%X'00000000',%X'2070',  200,    3,
   8,  20, 1, 'r', '4d3'   , '14 2 1d5'                       ,   19),
('Mummified Kobold'         ,%X'0B020002',%X'00000000',%X'102C',  750,   46,
  20,  24, 1, 'M', '13d8'  , '1 1 1d6|1 1 1d6'                ,   19),
('Killer Black Beetle'      ,%X'0000000A',%X'00000000',%X'0002',  300,   75,
  14,  46, 1, 'K', '18d8'  , '1 2 4d5'                        ,   19),
('Red Mold'                 ,%X'0B000001',%X'00000000',%X'30A0',  700,   64,
   2,  16, 1, 'm', '17d8'  , '5 5 4d4'                        ,   19),
('Quythulg'                 ,%X'00010000',%X'00002010',%X'5000',    0,  200,
  10,   1, 1, 'Q', '4d8'   , '0 0 0d0'                        ,   20),
('Giant Red Bat'            ,%X'0000000A',%X'00000000',%X'2060',  200,   40,
  12,  24, 2, 'b', '5d8'   , '1 2 1d7|1 3 1d3|1 3 1d3'        ,   20),
('Giant Black Dragon Fly'   ,%X'00000012',%X'00200009',%X'0072',  500,   58,
  20,  22, 1, 'F', '4d8'   , '10 2 1d6'                       ,   20),
('Cloud Giant'              ,%X'07020002',%X'00000000',%X'2134',  500,  125,
  20,  44, 1, 'P', '24d8'  , '8 1 3d8'                        ,   20),
('Black Dragon Bat'         ,%X'00000012',%X'00200004',%X'2150',  500,   50,
  12,  24, 3, 'b', '2d8'   , '6 2 1d3'                        ,   21),
('Blue Dragon Bat'          ,%X'00000012',%X'00080004',%X'2150',  500,   54,
  12,  26, 3, 'b', '3d6'   , '8 2 1d3'                        ,   21),
('Mummified Orc'            ,%X'0B020002',%X'00000000',%X'102C',  750,   56,
  20,  28, 1, 'M', '14d8'  , '1 1 2d4|1 1 2d4'                ,   21),
('Killer Boring Beetle'     ,%X'00000002',%X'00000000',%X'0002',  300,   70,
  12,  48, 1, 'K', '18d8'  , '1 2 4d5'                        ,   21),
('Killer Stag Beetle'       ,%X'0000000A',%X'00000000',%X'0002',  300,   80,
  12,  50, 1, 'K', '20d8'  , '1 2 3d4|1 1 1d12'               ,   22),
('Black Mold'               ,%X'0B000081',%X'00000000',%X'10A0',  500,   68,
   2,  18, 1, 'm', '15d8'  , '1 1 4d3'                        ,   22),
('Iron Golem'               ,%X'00020002',%X'00000000',%X'1080',  100,  160,
  12,  99,-1, 'g', '80d8'  , '1 1 1d12|1 1 1d12'              ,   22),
('Giant Yellow Scorpion'    ,%X'0000000A',%X'00000000',%X'0002',  200,   60,
  12,  38, 1, 'S', '12d8'  , '1 2 1d8|14 4 2d5'               ,   22),
('Green Ooze'               ,%X'00200001',%X'00000000',%X'00B2',    1,    6,
  15,   5, 0, 'O', '4d8'   , '6 5 2d3'                        ,   22),
('Black Ooze'               ,%X'003A0012',%X'0001000B',%X'0192',    1,    7,
  10,   6,-1, 'O', '6d8'   , '9 5 2d6'                        ,   23),
('Warrior'                  ,%X'13120002',%X'00000000',%X'2030',  400,   60,
  20,  40, 1, 'p', '15d8'  , '1 1 3d5'                        ,   23),
('Red Dragon Bat'           ,%X'00000012',%X'00800004',%X'2150',  500,   60,
  12,  28, 3, 'b', '3d8'   , '5 2 1d3'                        ,   23),
('Killer Blue Beetle'       ,%X'0000000A',%X'00000000',%X'0002',  300,   85,
  15,  50, 1, 'K', '20d8'  , '1 2 4d5'                        ,   23),
('Giant Silver Ant'         ,%X'0000000A',%X'00000000',%X'0002',  600,   45,
  15,  38, 1, 'a', '6d8'   , '6 2 4d4'                        ,   23),
('Crimson Mold'             ,%X'0B000001',%X'00000000',%X'10A0',  500,   65,
   2,  18, 1, 'm', '16d8'  , '1 1 1d3|4 5 0d0'                ,   23),
('Forest Wight'             ,%X'0F00000A',%X'0000100F',%X'112E',  300,  140,
  20,  30, 1, 'W', '12d8'  , '1 1 1d6|1 1 1d6|19 5 12d8'      ,   24),
('Berzerker'                ,%X'07020002',%X'00000000',%X'2030',  100,   65,
  20,  20, 1, 'p', '15d8'  , '1 1 1d8|1 1 1d8'                ,   24),
('Mummified Human'          ,%X'0B020002',%X'00000000',%X'102C',  600,   70,
  20,  34, 1, 'M', '17d8'  , '1 1 2d4|1 1 2d4'                ,   24),
('Banshee'                  ,%X'0F15001A',%X'0001002F',%X'100C',  100,   60,
  20,  24, 2, 'G', '6d8'   , '4 10 0d0|19 5 14d8'             ,   24),
('Giant Troll'              ,%X'0F020002',%X'00000000',%X'2026',  500,   85,
  20,  40, 1, 'T', '19d8'  , '1 1 1d6|1 1 1d6|1 2 3d4'        ,   25),
('Giant Brown Tick'         ,%X'00000012',%X'00000000',%X'0022',  200,   70,
  12,  50, 0, 't', '18d8'  , '14 2 1d10|10 4 1d1'             ,   25),
('Killer Red Beetle'        ,%X'00000012',%X'00000000',%X'0002',  300,   85,
  15,  50, 1, 'K', '20d8'  , '2 2 4d4'                        ,   25),
('Wooden Mold'              ,%X'00000001',%X'00000000',%X'10A0',  999,  100,
   2,  50, 1, 'm', '25d8'  , '14 8 2d6'                       ,   25),
('Giant Blue Dragon Fly'    ,%X'00000012',%X'00080009',%X'0030',  500,   75,
  20,  24, 1, 'F', '6d8'   , '1 2 1d6'                        ,   25),
('Giant Grey Ant Lion'      ,%X'0008000A',%X'00000000',%X'0032',  400,   90,
  10,  40, 1, 'A', '19d8'  , '1 2 2d12'                       ,   26),
('Disenchanter Bat'         ,%X'00000012',%X'00000000',%X'2060',    1,   75,
  14,  24, 3, 'b', '4d8'   , '21 1 0d0'                       ,   26),
('Giant Fire Tick'          ,%X'0000000A',%X'00000000',%X'2012',  200,   90,
  14,  54, 1, 't', '16d8'  , '5 9 3d7'                        ,   26),
('White Wraith'             ,%X'0F02000A',%X'00000000',%X'112E',  100,  165,
  20,  40, 1, 'W', '15d8'  , '1 1 1d6|1 1 1d6|19 5 15d8'      ,   26),
('Giant Black Scorpion'     ,%X'00000002',%X'00000000',%X'0002',  200,   85,
  12,  50, 1, 'S', '13d8'  , '1 2 1d10|14 4 2d5'              ,   26),
('Clear Ooze'               ,%X'000B000A',%X'00000000',%X'00B2',    1,   12,
  10,  14, 1, 'O', '4d8'   , '3 5 1d8'                        ,   26),
('Killer Fire Beetle'       ,%X'00000012',%X'00000000',%X'2012',  300,   95,
  14,  45, 1, 'K', '13d8'  , '1 2 3d4|5 9 4d5'                ,   27),
('Vampire'                  ,%X'17000002',%X'00001209',%X'112E',  100,  175,
  20,  45, 1, 'V', '20d8'  , '1 1 1d6|1 1 1d6|19 5 18d8'      ,   27),
('Giant Red Dragon Fly'     ,%X'00000012',%X'00800008',%X'2050',  500,   75,
  20,  24, 1, 'F', '7d8'   , '4 2 1d6'                        ,   27),
('Shimmering Mold'          ,%X'0B000081',%X'00000000',%X'10A0',  100,  180,
   2,  24, 1, 'm', '32d8'  , '8 5 5d4'                        ,   27),
('Black Knight'             ,%X'17020002',%X'0000010F',%X'2030',  100,  140,
  20,  60, 1, 'p', '25d8'  , '1 1 5d5'                        ,   28),
('Mage'                     ,%X'13020002',%X'00002C73',%X'2030',  100,  150,
  20,  30, 1, 'p', '10d8'  , '1 1 2d5'                        ,   28),
('Ice Troll'                ,%X'0F020002',%X'00000000',%X'0026',  500,  160,
  20,  46, 1, 'T', '22d8'  , '1 1 1d5|1 1 1d5|7 2 3d6'        ,   28),
('Giant Purple Worm'        ,%X'0000000A',%X'00000000',%X'2030',  300,  400,
  14,  65, 1, 'w', '65d8'  , '1 1 1d8|6 2 2d8|14 4 1d8'       ,   29),
('Young Blue Dragon'        ,%X'1F00000A',%X'0008100B',%X'2005',  700,  300,
  20,  50, 1, 'd', '33d8'  , '1 3 1d4|1 3 1d4|1 2 1d6'        ,   29),
('Young White Dragon'       ,%X'1F00000A',%X'0040100B',%X'2005',  700,  275,
  20,  50, 1, 'd', '32d8'  , '1 3 1d4|1 3 1d4|1 2 1d6'        ,   29),
('Young Green Dragon'       ,%X'1F00000A',%X'0010100B',%X'2005',  700,  290,
  20,  50, 1, 'd', '32d8'  , '1 3 1d4|1 3 1d4|1 2 1d6'        ,   29),
('Giant Fire Bat'           ,%X'00000012',%X'00000000',%X'2050',  100,   85,
  14,  30, 2, 'b', '5d8'   , '5 2 3d6|1 3 1d4|1 3 1d4'        ,   29),
('Giant Glowing Rat'        ,%X'0020000A',%X'00000000',%X'2070',  200,    4,
   8,  24, 1, 'r', '3d3'   , '8 2 2d6'                        ,   29),
	{ Now things are going to get tough...                  }
	{ Some of the creatures have Max hit points, denoted in }
	{ their CDEFENSE flags as the '4000' bit set            }
('Skeleton Troll'           ,%X'00020002',%X'00000000',%X'500C',  400,  225,
  20,  55, 1, 's', '14d8'  , '1 1 1d6|1 1 1d6|1 2 3d4'        ,   30),
('Giant Lightning Bat'      ,%X'0000000A',%X'00000000',%X'2042',  100,   80,
  15,  34, 2, 'b', '8d8'   , '8 2 3d8|1 3 1d5|1 3 1d5'        ,   30),
('Giant Static Ant'         ,%X'0000000A',%X'00000000',%X'0002',  600,   80,
  10,  40, 1, 'a', '8d8'   , '8 2 5d5'                        ,   30),
('Grave Wight'              ,%X'0F02000A',%X'0000190A',%X'512E',  300,  325,
  20,  35, 1, 'W', '12d8'  , '1 1 1d7|1 1 1d7|19 5 20d8'      ,   30),
('Killer Slicer Beetle'     ,%X'0000000A',%X'00000000',%X'0012',  300,  200,
  14,  55, 1, 'K', '22d8'  , '1 2 5d8'                        ,   30),
('Giant White Ant Lion'     ,%X'0008000A',%X'00000000',%X'0022',  400,  175,
  12,  45, 1, 'A', '20d8'  , '7 2 3d10'                       ,   30),
('Ghost'                    ,%X'1715000A',%X'0001002F',%X'500C',  100,  350,
  20,  30, 2, 'G', '13d8'  , '4 7 0d0|19 5 22d8|17 3 1d10'    ,   31),
('Giant Black Ant Lion'     ,%X'0008000A',%X'00000000',%X'0032',  400,  170,
  14,  45, 1, 'A', '23d8'  , '1 2 2d12|6 9 3d6'               ,   31),
('Death Watch Beetle'       ,%X'0000000A',%X'00000000',%X'0012',  300,  190,
  16,  60, 1, 'K', '25d8'  , '1 2 5d4|1 10 5d6'               ,   31),
('Ogre Magi'                ,%X'07020002',%X'0000A356',%X'403C',  300,  250,
  20,  42, 1, 'o', '14d8'  , '1 1 3d6'                        ,   31),
('Crystal Ooze'             ,%X'07390009',%X'00000000',%X'0092',  600,    8,
   5,  30,-1, 'O', '12d8'  , '7 5 4d4'                        ,   31),
('Two-Headed Troll'         ,%X'0F020002',%X'00000000',%X'6026',  500,  275,
  20,  48, 1, 'T', '14d8'  , '1 1 1d8|1 1 1d8|1 2 1d6|1 2 1d6',   32),
('Invisible Stalker'        ,%X'00030022',%X'00000000',%X'0000',  200,  200,
  20,  46, 3, 'E', '19d8'  , '1 1 1d6'                        ,   32),
('Giant Hunter Ant'         ,%X'00000002',%X'00000000',%X'0002',   10,  150,
  16,  40, 1, 'a', '12d8'   , '1 2 4d8'                        ,   32),
('Ninja'                    ,%X'0F020002',%X'00000000',%X'6030',  100,  300,
  20,  65, 1, 'p', '15d8'  , '14 1 3d4|2 1 3d4'               ,   32),
('Barrow Wight'             ,%X'0F02000A',%X'00001308',%X'512E',  100,  375,
  20,  40, 1, 'W', '13d8'  , '1 1 1d8|1 1 1d8|19 5 26d8'      ,   33),
('Skeleton 2-Headed Troll'  ,%X'00020002',%X'00000000',%X'500C',  200,  325,
  20,  48, 1, 's', '20d8'  , '1 1 1d9|1 1 1d9|1 2 1d5|1 2 1d5',   33),
('Water Elemental'          ,%X'0000000A',%X'00000000',%X'0020',  500,  325,
  12,  36, 1, 'E', '25d8'  , '1 1 1d10|1 1 1d10'              ,   33),
('Fire Elemental'           ,%X'000A0012',%X'00000000',%X'2010',  700,  350,
  16,  40, 0, 'E', '25d8'  , '5 1 4d6'                        ,   33),
('Lich'                     ,%X'1F020002',%X'00019F75',%X'500C',  600,  750,
  20,  50, 1, 'L', '25d8'  , '15 5 2d8|19 5 30d8|24 5 0d0'    ,   34),
('Master Vampire'           ,%X'17000002',%X'00001307',%X'512E',  100,  700,
  20,  55, 1, 'V', '23d8'  , '1 1 1d6|1 1 1d6|19 5 32d8'      ,   34),
('Spirit Troll'             ,%X'00040002',%X'00000000',%X'402E',  100,  425,
  20,  56, 1, 'T', '15d8'  , '1 3 1d5|1 3 1d5|1 2 1d6'        ,   34),
('Giant Red Scorpion'       ,%X'0000000A',%X'00000000',%X'0002',  400,  275,
  12,  50, 2, 'S', '18d8'  , '1 2 1d6|14 4 1d4'               ,   34),
('Earth Elemental'          ,%X'001A0002',%X'00000000',%X'0200',  900,  375,
  10,  60, 0, 'E', '30d8'  , '1 1 4d6|1 1 4d6'                ,   34),
('Young Black Dragon'       ,%X'1F00000A',%X'0020100B',%X'6005',  500,  600,
  20,  55, 1, 'd', '32d8'  , '1 3 1d5|1 3 1d5|1 2 1d6'        ,   35),
('Young Red Dragon'         ,%X'1F00000A',%X'0080100A',%X'6015',  500,  650,
  20,  60, 1, 'd', '36d8'  , '1 3 1d8|1 3 1d8|1 2 2d8'        ,   35),
('Necromancer'              ,%X'13020002',%X'00005762',%X'6030',  100,  600,
  20,  40, 1, 'p', '17d8'  , '1 1 2d6'                        ,   35),
('Mummified Troll'          ,%X'0F020002',%X'00000000',%X'502C',  500,  400,
  20,  38, 1, 'M', '18d8'  , '1 1 2d6|1 1 2d6'                ,   35),
('Giant Red Ant Lion'       ,%X'00080009',%X'00000000',%X'0032',  400,  350,
  14,  48, 1, 'A', '23d8'  , '5 2 3d12'                       ,   35),
('Mature White Dragon'      ,%X'2F00000A',%X'0040100A',%X'4005',  700, 1000,
  20,  65, 1, 'd', '48d8'  , '1 3 1d8|1 3 1d8|1 2 2d8'        ,   35),
('Xorn'                     ,%X'00160002',%X'00000000',%X'4200',  100,  650,
  20,  80, 1, 'X', '20d8'  , '1 1 1d6|1 1 1d6|1 1 1d6'        ,   36),
('Giant Mottled Ant Lion'   ,%X'0008000A',%X'00000000',%X'0032',  400,  350,
  14,  50, 2, 'A', '24d8 ' , '1 2 2d10'                       ,   36),
('Grey Wraith'              ,%X'0F02000A',%X'00011308',%X'512E',  100,  700,
  20,  50, 1, 'W', '23d8'  , '1 1 1d10|1 1 1d10|19 5 34d8'    ,   36),
('Young Multi-Hued Dragon'  ,%X'7F00000A',%X'00F81005',%X'6005',  500, 1250,
  20,  55, 1, 'd', '40d8'  , '1 3 1d9|1 3 1d9|1 2 2d10'       ,   36),
('Mature Blue Dragon'       ,%X'2F00000A',%X'00081009',%X'6005',  400, 1200,
  20,  75, 1, 'd', '48d8'  , '1 3 1d8|1 3 1d8|1 2 2d10'       ,   36),
('Mature Green Dragon'      ,%X'1F00000A',%X'0010100A',%X'6005',  700, 1100,
  20,  70, 1, 'd', '48d8'  , '1 3 1d4|1 3 1d4|1 2 1d6'        ,   36),
('Iridescent Beetle'        ,%X'0000000A',%X'00000000',%X'0012',  300,  850,
  16,  60, 1, 'K', '32d8'  , '1 2 4d6|1 1 1d12|11 7 0d0'      ,   37),
('King Vampire'             ,%X'17000002',%X'00001307',%X'512E',  100, 1000,
  20,  65, 1, 'V', '38d8'  , '1 1 1d6|1 1 1d6|19 5 38d8'      ,   37),
('King Lich'                ,%X'1F020002',%X'00019F73',%X'500C',  500, 1400,
  20,  65, 1, 'L', '52d8'  , '15 5 2d10|19 5 36d8|24 5 0d0'   ,   37),
('Mature Red Dragon'        ,%X'2F00000A',%X'00801808',%X'6015',  300, 1400,
  20,  80, 1, 'd', '60d8'  , '1 3 1d10|1 3 1d10|1 2 2d12'     ,   37),
('Mature Black Dragon'      ,%X'2F00000A',%X'00201009',%X'6005',  700, 1350,
  20,  55, 1, 'd', '58d8'  , '1 3 1d8|1 3 1d8|1 2 2d10'       ,   37),
('Mature Multi-Hued Dragon' ,%X'7F00000A',%X'00F81A05',%X'6005',  500, 1650,
  20,  65, 1, 'd', '80d8'  , '1 3 1d10|1 3 1d10|1 2 2d12'     ,   38),
('Ancient White Dragon'     ,%X'4F000002',%X'00401A08',%X'4005',  800, 1500,
  20,  80, 2, 'D', '88d8'  , '1 3 1d8|1 3 1d8|1 2 2d8'        ,   38),
('Emperor Wight'            ,%X'1B02000A',%X'00001306',%X'512E',  100, 1600,
  20,  40, 2, 'W', '48d8'  , '1 1 1d12|1 1 1d12|19 5 42d8'    ,   38),
('Black Wraith'             ,%X'1F02000A',%X'00001307',%X'512E',  100, 1700,
  20,  55, 1, 'W', '50d8'  , '1 1 1d12|1 1 1d12|19 5 44d8'    ,   38),
('Nether Wraith'            ,%X'1F07000A',%X'00005316',%X'512E',  100, 2100,
  20,  55, 1, 'W', '58d8'  , '1 1 1d12|1 1 1d12|19 5 52d8'    ,   39),
('Sorcerer'                 ,%X'13020002',%X'0000FF73',%X'6030',  100, 2150,
  20,  50, 2, 'p', '30d8'  , '1 1 2d8'                        ,   39),
('Ancient Blue Dragon'      ,%X'4F000002',%X'00081A08',%X'6005',  800, 2500,
  20,  90, 2, 'D', '87d8'  , '1 3 1d9|1 3 1d9|1 2 2d12'       ,   39),
('Ancient Green Dragon'     ,%X'4F000002',%X'00101A09',%X'6005',  700, 2400,
  20,  85, 2, 'D', '90d8'  , '1 3 1d8|1 3 1d8|1 2 2d10'       ,   39),
('Ancient Black Dragon'     ,%X'4F000002',%X'00201A07',%X'6005',  700, 2500,
  20,  90, 2, 'D', '90d8'  , '1 3 1d9|1 3 1d9|1 2 2d10'       ,   39),
('Disenchanter Worms'       ,%X'00200022',%X'00000000',%X'01B0',  100,   30,
   5,   5, 0, 'w', '10d8'  , '21 12 1d4'                      ,   40),
('Rotting Quylthulg'        ,%X'00010000',%X'00004010',%X'5000',    0, 1000,
  20,   1, 2, 'Q', '12d8'  , '0 0 0d0'                        ,   40),
('Ancient Red Dragon'       ,%X'7F000002',%X'00801E06',%X'6015',  500, 2750,
  20, 100, 2, 'D', '105d8' , '1 3 1d10|1 3 1d10|1 2 2d14'     ,   40),
('Death Quasit'             ,%X'1103000A',%X'000010FA',%X'0004',    0, 1000,
  20,  80, 3, 'q', '55d8'  , '15 2 3d6|1 3 3d3|1 3 3d3'       ,   40),
('Emperor Lich'             ,%X'2F020002',%X'00019F72',%X'500C',  500,10000,
  20,  75, 2, 'L', '190d8' , '15 5 2d12|19 5 46d8|24 5 0d0'   ,   40),
('Ancient Multi-Hued Dragon',%X'7F000002',%X'00F89E05',%X'6005',  500,12000,
  20, 100, 2, 'D', '260d8' , '1 3 1d12|1 3 1d12|1 2 3d12'     ,   40),
	{ Winning creatures should follow here.                         }
	{ Winning creatures are denoted by the 32 bit in CMOVE          }
	{ Iggy is not a win creature, just a royal pain in the ass...   }
('Evil Iggy'                ,%X'7F130002',%X'0001D713',%X'5004',    0,18000,
  20,  80, 2, 'p', '300d8' , '2 1 4d6|13 19 0d0'               ,   50),
	{ Here is the only actual win creature...                       }
('Balrog'                   ,%X'FF1F0002',%X'0081C743',%X'5004',    0,55000,
  20, 125, 3, 'B', '375d8' , '5 1 10d12|1 17 8d12|24 5 0d0'    ,  100)
				);

	blank_monster	:= (0,0,0,0,0,0,0,0,0,false,false);
