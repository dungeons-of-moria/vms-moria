  type
	byteint		= [byte] 0..255;
	bytlint		= [byte] -128..127;
	wordint		= [word] 0..65535;
	worlint		= [word] -32768..32767;
	quad_type	= record
			    l0	: unsigned;
			    l1  : unsigned;
			  end;
	atype		= varying  [16] of char;
	btype		= varying  [14] of char;
	ctype		= varying  [26] of char;
	dtype		= varying   [5] of char;
	etype		= varying  [34] of char;
	mtype		= varying [190] of char;
	ntype		= varying[1024] of char;
	ttype		= varying  [68] of char;
	vtype		= varying  [80] of char;
	stat_type	= packed array [1..6] of char;
	obj_set		= set of 0..255;
	char_set	= set of 'A'..'z';
	key_type	= record		{ For char saver	}
		file_id : [key(0)] packed array [1..70] of char;
		seed	: integer;
	end;
	creature_type = record
		name	: ctype;	{ Descrip of creature	}
		cmove	: unsigned;	{ Bit field		}
		spells	: unsigned;	{ Creature spells	}
		cdefense: wordint;	{ Bit field		}
		sleep	: worlint;	{ Inactive counter	}
		mexp	: wordint; 	{ Exp value for kill	}
		aaf	: byteint;	{ Area affect radius	}
		ac	: byteint;	{ AC			}
		speed	: bytlint;	{ Movement speed	}
		cchar	: char;		{ Character rep.	}
		hd	: dtype;	{ Creatures hit die	}
		damage	: etype;	{ Type attack and damage}
		level	: byteint;	{ Level of creature	}
	end;
	monster_type = record
		hp	: worlint;	{ Hit points		}
		csleep	: worlint;	{ Inactive counter	}
		cdis	: worlint;	{ Cur dis from player	}
		mptr	: wordint;	{ Pointer into creature	}
		nptr	: wordint;	{ Pointer to next block	}
		cspeed	: bytlint;	{ Movement speed	}

			{ Note: FY and FX constrain dungeon size to 255	}
		fy	: byteint;	{ Y Pointer into map	}
		fx	: byteint;	{ X Pointer into map	}

		stuned	: [bit(6),pos(104)] -32..31; { Rounds stunned}
		ml	: [bit(1),pos(110)] boolean; { On if shown   }
		confused: [bit(1),pos(111)] boolean; { On if confused}
	end;
	treasure_type = record
		name	: ttype;	{ Object name		}
		tval	: byteint;	{ Catagory number	}
		tchar	: char;		{ Character representation}
		flags	: unsigned;	{ Special flags		}
		p1	: integer;	{ Misc. use variable	}
		cost	: integer;	{ Cost of item		}
		subval	: integer;	{ Sub-catagory number	}
		weight	: wordint;	{ Weight		}
		number	: wordint;	{ Number of items	}
		tohit	: worlint;	{ Pluses to hit		}
		todam	: worlint;	{ Pluses to damage	}
		ac	: worlint;	{ Normal AC		}
		toac	: worlint;	{ Pluses to AC		}
		damage	: dtype;	{ Damage when hits	}
		level	: bytlint;	{ Level item found	}
	end;
	player_type = record
		misc : record
			name	: vtype;	{ Name of character	}
			race	: vtype;	{ Race of character	}
			sex	: vtype;	{ Sex of character	}
			title	: vtype;	{ Character's title	}
			tclass	: vtype;	{ Character's class	}
			max_exp : integer;	{ Max experience}
			exp	: integer;	{ Cur experienc	}
			au	: integer;	{ Gold		}
			age	: wordint;	{ Characters age}
			ht	: wordint;	{ Height	}
			wt	: wordint;	{ Weight	}
			lev	: wordint;	{ Level		}
			max_lev	: wordint;	{ Max level explored}
			srh	: worlint;	{ Chance in search}
			fos	: worlint;	{ Frenq of search}
			bth	: worlint;	{ Base to hit	}
			bthb	: worlint;	{ BTH with bows	}
			mana	: worlint;	{ Mana points	}
			mhp	: worlint;	{ Max hit pts	}
			ptohit	: worlint;	{ Pluses to hit	}
			ptodam	: worlint;	{ Pluses to dam	}
			pac	: worlint;	{ Total AC	}
			ptoac	: worlint;	{ Magical AC	}
			dis_th	: worlint;	{ Display +ToHit}
			dis_td	: worlint;	{ Display +ToDam}
			dis_ac	: worlint;	{ Display +ToAC }
			dis_tac	: worlint;	{ Display +ToTAC}
			disarm  : worlint;	{ % to Disarm	}
			save	: worlint;	{ Saving throw	}
			sc	: worlint;	{ Social Class	}
			pclass	: byteint;	{ # of class		}
			prace	: byteint;	{ # of race		}
			hitdie	: byteint;	{ Char hit die	}
			stl	: bytlint;	{ Stealth factor}
			expfact	: real;		{ Experience factor}
			cmana	: real;		{ Cur mana pts  }
			chp	: real;		{ Cur hit pts	}
			history : array [1..5] of vtype;{ History record}
		end;
		stat : record
			str	: byteint;	{ Max strength	      }
			cstr	: byteint;	{ Current strength    }
			dex	: byteint;	{ Max dexterity	      }
			cdex	: byteint;	{ Current dexterity   }
			con	: byteint;	{ Max constitution    }
			ccon	: byteint;	{ Current constitution}
			int	: byteint;	{ Max intelligence    }
			cint	: byteint;	{ Current intelligence}
			wis	: byteint;	{ Max wisdom	      }
			cwis	: byteint;	{ Current wisdom      }
			chr	: byteint;	{ Max charisma	      }
			cchr	: byteint;	{ Current charisma    }
		end;
		flags : record
			status		: unsigned;	{ Status of player    }
			rest		: integer;	{ Rest counter	      }
			blind		: integer;	{ Blindness counter   }
			paralysis	: integer;	{ Paralysis counter   }
			confused	: integer;	{ Confusion counter   }
			food		: integer;	{ Food counter        }
			food_digested	: integer;	{ Food per round      }
			protection	: integer;	{ Protection fr. evil }
			speed		: integer;	{ Cur speed adjust    }
			fast		: integer;	{ Temp speed change   }
			slow		: integer;	{ Temp speed change   }
			afraid		: integer;	{ Fear                }
			poisoned	: integer;	{ Poisoned            }
			image		: integer;	{ Halucinate          }
			protevil	: integer;	{ Protect VS evil     }
			invuln		: integer;	{ Increases AC        }
			hero		: integer;	{ Heroism	      }
			shero		: integer;	{ Super Heroism	      }
			blessed		: integer;	{ Blessed	      }
			resist_heat	: integer;	{ Timed heat resist   }
			resist_cold	: integer;	{ Timed cold resist   }
			detect_inv	: integer;	{ Timed see invisible }
			word_recall	: integer;	{ Timed teleport level}
			see_infra	: integer;	{ See warm creatures  }
			tim_infra	: integer;	{ Timed infra vision  }
			see_inv		: boolean;	{ Can see invisible   }
			teleport	: boolean;	{ Random teleportation}
			free_act	: boolean;	{ Never paralyzed     }
			slow_digest	: boolean;	{ Lower food needs    }
			aggravate	: boolean;	{ Agravate monsters   }
			fire_resist	: boolean;	{ Resistance to fire  }
			cold_resist	: boolean;	{ Resistance to cold  }
			acid_resist	: boolean;	{ Resistance to acid  }
			regenerate	: boolean;	{ Regenerate hit pts  }
			lght_resist	: boolean;	{ Resistance to light }
			ffall		: boolean;	{ No damage falling   }
			sustain_str	: boolean;	{ Keep strength       }
			sustain_int	: boolean;	{ Keep intelligence   }
			sustain_wis	: boolean;	{ Keep wisdom         }
			sustain_con	: boolean;	{ Keep constitution   }
			sustain_dex	: boolean;	{ Keep dexterity      }
			sustain_chr	: boolean;	{ Keep charisma       }
			confuse_monster : boolean;	{ Glowing hands...    }
		end;
	end;
	spell_type = record
		sname	: ctype;
		slevel	: byteint;
		smana	: byteint;
		sexp	: wordint;
		sfail	: byteint;
		learned : boolean;
	end;
	spl_rec = record
		splnum	: integer;
		splchn	: integer;
	end;
	spl_type = array [1..22] of spl_rec;
	race_type = packed record
		trace	: vtype;		{ Type of race		}
		str_adj : bytlint;	{ adjustments		}
		int_adj : bytlint;
		wis_adj : bytlint;
		dex_adj : bytlint;
		con_adj : bytlint;
		chr_adj : bytlint;
		b_age	: wordint; { Base age of character	}
		m_age	: wordint; { Maximum age of character	}
		m_b_ht	: wordint; { base height for males	}
		m_m_ht	: wordint; { mod height for males	}
		m_b_wt	: wordint; { base weight for males	}
		m_m_wt	: wordint; { mod weight for males	}
		f_b_ht	: wordint; { base height females	}
		f_m_ht	: wordint; { mod height for females	}
		f_b_wt	: wordint; { base weight for female	}
		f_m_wt	: wordint; { mod weight for females	}
		b_exp	: real;	   { Base experience factor	}
		b_dis	: bytlint; { base chance to disarm	}
		srh	: bytlint; { base chance for search	}
		stl	: bytlint; { Stealth of character	}
		fos	: bytlint; { frequency of auto search	}
		bth	: bytlint; { adj base chance to hit	}
		bthb	: bytlint; { adj base to hit with bows	}
		bsav	: bytlint; { Race base for saving throw	}
		bhitdie	: bytlint; { Base hit points for race	}
		infra	: bytlint; { See infra-red		}
		tclass	: unsigned; { Bit field for class types	}
	end;
	class_type = record
		title	: vtype;	   { type of class		}
		m_exp	: real;		   { Class experience factor	}
		adj_hd	: bytlint;{ Adjust hit points		}
		mdis	: bytlint;{ mod disarming traps	}
		msrh	: bytlint;{ modifier to searching	}
		mstl	: bytlint;{ modifier to stealth	}
		mfos	: bytlint;{ modifier to freq-of-search	}
		mbth	: bytlint;{ modifier to base to hit	}
		mbthb	: bytlint;{ modifier to base to hit - bows}
		msav	: bytlint;{ Class modifier to save	}
		madj_str: bytlint;{ Class modifier for strength}
		madj_int: bytlint;{ Class modifier for intelligence}
		madj_wis: bytlint;{ Class modifier for wisdom	}
		madj_dex: bytlint;{ Class modifier for dexterity}
		madj_con: bytlint;{ Class modifier for constitution}
		madj_chr: bytlint;{ Class modifier for charisma}
		pspell	: boolean;	   { class use priest spells	}
		mspell	: boolean;	   { class use mage spells	}
	end;
	background_type	= record
		info	: vtype;	   { History information	}
		roll	: byteint;   { Die roll needed for history}
		chart	: byteint;   { Table number		}
		next	: bytlint;{ Pointer to next table	}
		bonus	: bytlint;{ Bonus to the Social Class	}
	end;
	floor_type = record
		ftval	: [bit(7),pos(0)] 0..15;
		ftopen	: [bit(1),pos(7)] boolean;
	end;
	cave_type = record
		cptr	: byteint;
		tptr	: byteint;
		fval	: [bit(4),pos(16)] 0..15;
		fopen	: [bit(1),pos(20)] boolean;
		fm	: [bit(1),pos(21)] boolean;
		pl	: [bit(1),pos(22)] boolean;
		tl	: [bit(1),pos(23)] boolean;
	end;
	row_floor  = array [1..max_width] of cave_type;
	owner_type = record
		owner_name	: vtype;
		max_cost	: worlint;
		max_inflate	: real;
		min_inflate	: real;
		haggle_per	: real;
		owner_race	: byteint;
		insult_max	: byteint;
	end;
	inven_record = record
		scost		: integer;
		sitem		: treasure_type;
	end;
	store_type = record
		store_open	: worlint;
		owner		: byteint;
		insult_cur	: bytlint;
		store_ctr	: byteint;
		store_inven	: array [1..store_inven_max] of inven_record;
	end;
	SigArgs		= ARRAY[0..100] OF INTEGER;
	MechArgs	= ARRAY[0..4]   OF INTEGER;
