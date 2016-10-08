var
	seed				: [psect(player$data),global]
					  unsigned;	{ Contains seed #     }
	randes_seed			: [psect(setup$data)]
					  unsigned;	{ For encoding colors }
	town_seed			: [psect(generate$data)]
					  unsigned;	{ Seed for town genera}
	channel				: [psect(io$data),global]
					  integer;	{ I/O channel #       }
	io$bin_pause			: [psect(io$data)]
					  quad_type;	{ I/O pause time      }
	cur_height,cur_width		: [psect(player$data)]
					  integer;	{ Cur dungeon size    }
	dun_level			: [psect(player$data)]
					  integer;	{ Cur dungeon level   }
	missle_ctr			: [psect(player$data)]
					  integer;	{ Counter for missles }
	msg_line			: [psect(player$data)]
					  integer;	{ Contains message txt}
	msg_flag			: [psect(player$data)]
					  boolean;	{ Set with first msg  }
	old_msg				: [psect(player$data)]
					  vtype;	{ Last message	      }
	generate			: [psect(setup$data)]
					  boolean;	{ Generate next level }
	death				: [psect(player$data)]
					  boolean;	{ True if died	      }
	died_from			: [psect(player$data)]
					  vtype;	{ What killed him     }
	find_flag			: [psect(player$data)]
					  boolean;	{ Used in MORIA	      }
	reset_flag			: [psect(player$data)]
					  boolean;	{ Used in MORIA	      }
	cave_flag			: [psect(player$data)]
					  boolean;	{ Used in GET_PANEL   }
	light_flag			: [psect(player$data)]
					  boolean;	{ Used in MOVE_LIGHT  }
	redraw				: [psect(player$data)]
					  boolean;	{ For redraw screen   }
	stat_column			: [psect(player$data)]
					  integer;	{ Column for stats    }
	print_stat			: [psect(player$data)]
					  unsigned;	{ Flag for stats      }
	turn				: [psect(player$data)]
					  integer;	{ Cur trun of game    }
	wizard1				: [psect(player$data)]
					  boolean;	{ Wizard flag	      }
	wizard2				: [psect(player$data)]
					  boolean;	{ Wizard flag	      }
	used_line			: [psect(player$data)]
					  array [2..23] of boolean;
	password1			: [psect(setup$data)]
					  packed array [1..12] of char;
	password2			: [psect(setup$data)]
					  packed array [1..12] of char;
	wdata				: [psect(setup$data)]
					  array [1..2,0..12] of unsigned;
	days				: [psect(setup$data)]
					  array [1..7] of vtype;
	closing_flag			: [psect(player$data)]
					  integer;	{ Used for closing   }

	{ Bit testing array						}
	bit_array			: [psect(player$data)]
					  array [1..32] of unsigned;

	{ External file names; are all located in directory with image	}
	MORIA_HOU			: [psect(setup$data)] vtype;
	MORIA_MOR			: [psect(setup$data)] vtype;
	MORIA_MAS			: [psect(setup$data)] vtype;
	MORIA_TOP			: [psect(setup$data)] vtype;
	MORIA_HLP			: [psect(setup$data)] vtype;

	{  Following are calculated from max dungeon sizes		}
	max_panel_rows,max_panel_cols	: [psect(player$data)] integer;
	quart_height,quart_width	: [psect(player$data)] integer;
	panel_row,panel_col		: [psect(player$data)] integer;
	panel_row_min,panel_row_max	: [psect(player$data)] integer;
	panel_col_min,panel_col_max	: [psect(player$data)] integer;
	panel_col_prt,panel_row_prt	: [psect(player$data)] integer;

	{  Following are all floor definitions				}
	cave				: [psect(moria$data)]
					  array [1..max_height] of row_floor;
	blank_floor			: [psect(generate$data)] cave_type;
	dopen_floor			: [psect(generate$data)] floor_type;
	lopen_floor			: [psect(generate$data)] floor_type;
	corr_floor1			: [psect(generate$data)] floor_type;
	corr_floor2			: [psect(generate$data)] floor_type;
	corr_floor3			: [psect(generate$data)] floor_type;
	corr_floor4			: [psect(generate$data)] floor_type;
	rock_wall1			: [psect(generate$data)] floor_type;
	rock_wall2			: [psect(generate$data)] floor_type;
	rock_wall3			: [psect(generate$data)] floor_type;
	boundry_wall			: [psect(generate$data)] floor_type;

	{  Following are set definitions				}
	floor_set			: [psect(moria$data)] obj_set;
	wall_set			: [psect(moria$data)] obj_set;
	pwall_set			: [psect(moria$data)] obj_set;
	corr_set			: [psect(moria$data)] obj_set;
	trap_set			: [psect(moria$data)] obj_set;
	light_set			: [psect(moria$data)] obj_set;

	{ Following are player variables				}
	py			: [psect(player$data)] player_type;
	player_title		: [psect(player$data)]
				  array [1..max_class] of
				  array [1..max_player_level] of btype;
	player_exp		: [psect(player$data)]
				  array [1..max_player_level] of integer;
	acc_exp			: [psect(player$data)]
				  real;	{ Accumulator for fractional exp}
	bare_hands		: [psect(player$data)]
				  dtype;
	char_row		: [psect(player$data)] integer;
	char_col		: [psect(player$data)] integer;
	com_val			: [psect(player$data)] integer;
	pclass			: [psect(player$data)] integer;
	sex_type		: [psect(player$data)] vtype;
	race			: [psect(create$data)]
				  array [1..max_races] of race_type;
	background		: [psect(create$data)]
				  array [1..max_background] of background_type;
	rgold_adj		: [psect(store$data)]
				  array [1..max_races] of
				  array [1..max_races] of real;
	class			: [psect(create$data)]
				  array [1..max_class] of class_type;
	magic_spell		: [psect(player$data)]
				  array [1..max_class] of 
				  array [1..31] of spell_type;
	mush			: [psect(setup$data)] treasure_type;
	player_init		: [psect(create$data)]
				  array [1..max_class] of
				  array [1..5] of byteint;
	total_winner		: [psect(setup$data)] boolean;

	{ Following are store definitions				}
	owners			: [psect(store$data)]
				  array [1..max_owners] of owner_type;
	store			: [psect(store$data)]
				  array [1..max_stores] of store_type;
	store_door		: [psect(generate$data)]
				  array [1..max_stores] of treasure_type;
	store_choice		: [psect(store$data)]
				  array [1..max_stores] of 
				  array [1..store$choices] of integer;
	store_buy		: [psect(store$data)]
				  array [1..max_stores] of obj_set;

	{ Following are treasure arrays	and variables			}
	object_list		: [psect(moria$data)] 
				  array [1..max_objects] of treasure_type;
	object_ident		: [psect(moria$data)] 
				  array [1..max_objects] of boolean;
	t_level			: [psect(moria$data)] 
				  array [0..max_obj_level] of integer;
	gold_list		: [psect(moria$data)] 
				  array [1..max_gold]    of treasure_type;
	t_list			: [psect(moria$data)] 
				  array [1..max_talloc]  of treasure_type;
	inventory		: [psect(player$data)] 
				  array [1..inven_max]	 of treasure_type;
	inventory_init		: [psect(setup$data)] 
				  array [1..inven_init_max] of treasure_type;
	blank_treasure		: [psect(moria$data)] treasure_type;
	inven_ctr		: [psect(player$data)]
				  integer;	{ Total different obj's	}
	inven_weight		: [psect(player$data)]
				  integer;	{ Cur carried weight	}
	equip_ctr		: [psect(player$data)]
				  integer;	{ Cur equipment ctr	}
	tcptr			: [psect(moria$data)]
				  integer;	{ Cur treasure heap ptr	}

	{ Following are feature objects defined for dungeon		}
	trap_lista		: [psect(generate$data)]
				  array [1..max_trapa]   of treasure_type;
	trap_listb		: [psect(generate$data)]
				  array [1..max_trapb]   of treasure_type;
	scare_monster		: [psect(generate$data)]
				  treasure_type;	{ Special trap	}
	rubble			: [psect(generate$data)]
				  treasure_type;
	door_list		: [psect(generate$data)]
				  array [1..3]		 of treasure_type;
	up_stair		: [psect(generate$data)]
				  treasure_type;
	down_stair		: [psect(generate$data)]
				  treasure_type;

	{ Following are creature arrays and variables			}
	c_list		: [psect(moria$data)]
			  array [1..max_creatures] of creature_type;
	m_list		: [psect(moria$data)]
			  array [1..max_malloc]    of monster_type;
	m_level		: [psect(generate$data)]
			  array [0..max_mons_level] of integer;
	blank_monster	: [psect(generate$data)]
			  monster_type;	{ Blank monster values	}
	muptr		: [psect(generate$data)]
			  integer;	{ Cur used monster ptr	}
	mfptr		: [psect(generate$data)]
			  integer;	{ Cur free monster ptr	}
	mon_tot_mult	: [psect(moria$data)]
			  integer;	{ # of repro's of creature	}

	{ Following are arrays for descriptive pieces			}
	colors			: [psect(setup$data)]
				  array [1..max_colors]	 of atype;
	mushrooms		: [psect(setup$data)]
				  array [1..max_mush]    of atype;
	woods			: [psect(setup$data)]
				  array [1..max_woods]   of atype;
	metals			: [psect(setup$data)]
				  array [1..max_metals]  of atype;
	rocks			: [psect(setup$data)]
				  array [1..max_rocks]   of atype;
	amulets			: [psect(setup$data)]
				  array [1..max_amulets] of atype;
	syllables		: [psect(setup$data)]
				  array [1..max_syllables] of dtype;
	vowel_set		: [psect(moria$data)]
				  char_set;

	{ Following are variables for the Save Character Routines	}
	finam			: [psect(save$data)]
				  vtype;
	key_rec			: [psect(save$data)]
				  key_type;

	{ Cursor variables, used for cursor positioning			}
	cursor_r	: [psect(io$data),global]
			  array [1..24] of varying[10] of char;
	curlen_r	: [psect(io$data),global] integer;
	cursor_c	: [psect(io$data),global]
			  array [1..80] of varying[10] of char;
	curlen_c	: [psect(io$data),global] integer;
	cursor_l	: [psect(io$data),global] integer;
	row_first	: [psect(io$data),global] boolean;
	cursor_erl	: [psect(io$data),global] varying[10] of char;
	cursor_erp	: [psect(io$data),global] varying[10] of char;
