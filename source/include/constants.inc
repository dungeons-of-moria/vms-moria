const
	{Note to the Wizard:                                            }
	{       Tweaking these constants can *GREATLY* change the game. }
	{       Two years of constant tuning have generated these       }
	{       values.  Minor adjustments are encouraged, but you must }
	{       be very careful not to unbalance the game.  Moria was   }
	{       meant to be challenging, not a give away.  Many         }
	{       adjustments can cause the game to act strangely, or even}
	{       cause errors.                                           }

	{ Current version number of Moria				}
	cur_version   = 4.8;

	{ QIOW constants, see $IODEF in STARLET.MLB			}
	IO$_WRITEVBLK = %B'0000000000110000';   {  48D, 0030H   }
	IO$_TTYREADALL= %B'0000000000111010';   {  58D, 003AH   }
	IO$M_NOECHO   = %B'0000000001000000';   {  64D, 0040H   }
	IO$M_NOWAIT   = %B'0000000010000000';   { 128D, 0080H   }
	IO$M_PURGE    = %B'0000100000000000';   {2048D, 0800H   }
	IO$MOR_OUTPUT = IO$_WRITEVBLK;
	IO$MOR_INPUT  = IO$_TTYREADALL + IO$M_NOECHO;
	IO$MOR_DELAY  = IO$MOR_INPUT   + IO$M_NOWAIT;
	IO$MOR_IPURGE = IO$MOR_DELAY   + IO$M_PURGE;

	{ IO bug exists at high baud rates (baud >= 9600).  This bug	}
	{ is believed to be in the device driver.  It can be avoided by }
	{ hibernating the process a small amount of time before perform-}
	{ ing a QIOW read operation.                            -RAK-   }
	{ This constant no longer used after VMS 4.0                    }
	IO$MOR_IOPAUSE= 5;      { x/100 seconds of sleep before inkey   }

	{ Encryption constants						}
	{ Note: These numbers were pulled from the air, and can be      }
	{       changed.  If changed, characters produced from other    }
	{       versions will fail to restore.                          }
	encrypt_seed1 = 1175191;
	encrypt_seed2 = 997551771;

	{ Dungeon size parameters					}
	max_height    = 66;     { Multiple of 11; >= 22 }
	max_width     = 198;    { Multiple of 33; >= 66 }
	screen_height = 22;
	screen_width  = 66;

	{ Output dungeon section sizes					}
	outpage_height= 44;     { 44 lines of dungeon per section       }
	outpage_width = 99;     { 100 columns of dungeon per section    }

	{ Dungeon generation values					}
	{ Note: The entire design of dungeon can be changed by only     }
	{       slight adjustments here.                                }
	dun_tun_rnd     = 36;   { Random direction (4 is min)           }
	dun_tun_chg     = 70;   { Chance of changing direction (99 max) }
	dun_tun_fnd     = 12;   { Distance for auto find to kick in     }
	dun_tun_con     = 15;   { Chance of extra tunneling             }
	dun_roo_mea     = 32;   { Mean of # of rooms, standard dev=2    }
	dun_tun_pen     = 25;   { % chance of room doors                }
	dun_tun_jct     = 15;   { % chance of doors at tunnel junctons  }
	dun_str_den     = 5;    { Density of streamers                  }
	dun_str_rng     = 2;    { Width of streamers                    }
	dun_str_mag     = 3;    { Number of magma streamers             }
	dun_str_mc      = 95;   { 1/x chance of treasure per magma      }
	dun_str_qua     = 2;    { Number of quartz streamers            }
	dun_str_qc      = 55;   { 1/x chance of treasure per quartz     }
	dun_unusual     = 300;  { Level/x chance of unusual room        }

	{ Store constants						}
	max_owners      = 18;   { Number of owners to choose from       }
	max_stores      = 6;    { Number of different stores            }
	store_inven_max = 24;   { Max number of discrete objs in inven  }
	store$choices   = 26;   { Number of items to choice stock from  }
	store$max_inven = 20;   { Max diff objs in stock before auto sell}
	store$min_inven = 14;   { Min diff objs in stock before auto buy}
	store$turn_around= 3;   { Amount of buying and selling normally }
	inven_init_max  = 105;  { Size of store init array              }
	cost_adj        = 1.00; { Adjust prices for buying and selling  }

	{ Treasure constants						}
	inven_max     = 35;     { Size of inventory array(Do not change)}
	max_obj_level = 50;     { Maximum level of magic in dungeon     }
	obj_great     = 20;     { 1/n Chance of item being a Great Item }
	max_objects   = 344;    { Number of objects for universe        }
	max_gold      = 18;     { Number of different types of gold     }
	max_talloc    = 225;    { Max objects per level                 }
	treas_room_alloc = 7;   { Amount of objects for rooms           }
	treas_any_alloc  = 2;   { Amount of objects for corridors       }
	treas_gold_alloc = 2;   { Amount of gold (and gems)             }

	{ Magic Treasure Generation constants				}
	{ Note: Number of special objects, and degree of enchantments   }
	{       can be adjusted here.                                   }
	obj_std_adj     = 1.25; { Adjust STD per level                  }
	obj_std_min     = 7;    { Minimum STD                           }
	obj_town_level  = 7;    { Town object generation level          }
	obj_base_magic  = 15;   { Base amount of magic                  }
	obj_base_max    = 70;   { Max amount of magic                   }
	obj_div_special = 6;    { magic_chance/# = special magic        }
	obj_div_cursed  = 1.3;  { magic_chance/# = cursed items         }

	{ Constants describing limits of certain objects		}
	obj$lamp_max    =15000; { Maximum amount that lamp can be filled}
	obj$bolt_range  = 18;   { Maximum range of bolts and balls      }
	obj$rune_prot   = 3000; { Rune of protection resistance         }

	{ Creature contants						}
	max_creatures    = 279; { Number of creatures defined for univ  }
	max_malloc       = 100 + 1;{ Max that can be allocated          }
	max_malloc_chance= 160; { 1/x chance of new monster each round  }
	max_mons_level   =  40; { Maximum level of creatures            }
	max_sight        =  20; { Maximum dis a creature can be seen    }
	max_spell_dis    =  20; { Maximum dis creat. spell can be cast  }
	max_mon_mult     =  75; { Maximum reproductions on a level      }
	mon_mult_adj     =   7; { High value slows multiplication       }
	mon_nasty        =  50; { Dun_level/x chance of high level creat}
	min_malloc_level =  14; { Minimum number of monsters/level      }
	min_malloc_td    =   4; { Number of people on town level (day)  }
	min_malloc_tn    =   8; { Number of people on town level (night)}
	win_mon_tot      =   2; { Total number of "win" creatures       }
	win_mon_appear   =  50; { Level where winning creatures begin   }
	mon$summon_adj   =   2; { Adjust level of summoned creatures    }
	mon$drain_life   =   2; { Percent of player exp drained per hit }

	{ Trap constants						}
	max_trapa        = 18;  { Number of defined traps               }
	max_trapb        = 19;  { Includes secret doors                 }

	{ Descriptive constants 					}
	max_colors    = 67;     { Used with potions     }
	max_mush      = 29;     { Used with mushrooms   }
	max_woods     = 41;     { Used with staffs      }
	max_metals    = 31;     { Used with wands       }
	max_rocks     = 52;     { Used with rings       }
	max_amulets   = 39;     { Used with amulets     }
	max_syllables = 153;    { Used with scrolls     }

	{ Player constants						}
	max_player_level = 40;  { Maximum possible character level      }
	max_races        = 8;   { Number of defined races               }
	max_class        = 6;   { Number of defined classes             }
	use_device       = 3;   { x> Harder devices x< Easier devices   }
	max_background   = 128; { Number of types of histories for univ }
	player_food_full =10000;{ Getting full                          }
	player_food_max  =15000;{ Maximum food value, beyond is wasted  }
	player_food_faint=  300;{ Character begins fainting             }
	player_food_weak = 1000;{ Warn player that he is getting very low}
	player_food_alert= 2000;{ Warn player that he is getting low    }
	player$regen_faint  = 0.0005;   { Regen factor when fainting    }
	player$regen_weak   = 0.0015;   { Regen factor when weak        }
	player$regen_normal = 0.0030;   { Regen factor when full        }
	player$regen_hpbase = 0.0220;   { Min amount hp regen           }
	player$regen_mnbase = 0.0080;   { Min amount mana regen         }
	player_weight_cap= 130; { "#"*(1/10 pounds) per strength point  }
	player_exit_pause= 6;   { Pause time before player can re-roll  }

	{ Base to hit constants 					}
	bth_lev_adj     = 3;    { Adjust BTH per level                  }
	bth_plus_adj    = 3;    { Adjust BTH per plus-to-hit            }
	bth_hit         = 12;   { Automatic hit; 1/bth_hit              }

	{ Misc								}
	null            = chr(0);
