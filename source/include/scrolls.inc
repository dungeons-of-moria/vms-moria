	{ Scrolls for the reading				-RAK-	}
[psect(misc2$code)] procedure read;
      var
		i1                              : unsigned;
		i2,i3,i4,item_val               : integer;
		thit,tdam,y,x                   : integer;
		tmp                             : array [1..5] of integer;
		out_val                         : vtype;
		redraw,ident,first,flag         : boolean;
      begin
	first := true;
	reset_flag := true;
	if (inven_ctr > 0) then
	  begin
	    if (find_range([70,71],i2,i3)) then
	      if (py.flags.blind > 0) then
		msg_print('You can''t see to read the scroll.')
	      else if (no_light) then
		msg_print('You have no light to read by.')
	      else if (py.flags.confused > 0) then
		begin
		  msg_print('The text seems to swim about the page!');
		  msg_print('You are too confused to read...');
		end
	      else
		begin
		  redraw := false;
		  if (get_item(item_val,'Read which scroll?',redraw,
								i2,i3)) then
		    with inventory[item_val] do
		      begin
			if (redraw) then draw_cave;
			reset_flag := false;
			i1 := flags;
			ident := false;
			while (i1 > 0) do
			  begin
			    i2 := bit_pos(i1);
			    if (tval = 71) then i2 := i2 + 31;
			    if (first) then
			      if (not(i2 in [4,25])) then
				begin
			msg_print('As you read the scroll it vanishes.');
				  first := false;
				end;

	{ Scrolls...						}
	case (i2) of
	  1  :  with inventory[23] do
		  begin
		    if (tval > 0) then
		    begin
		      objdes(out_val,23,false);
		      msg_print('Your ' + out_val + ' glows faintly!');
		      if (enchant(tohit)) then
			begin
			  flags := uand(%X'7FFFFFFF',flags);
			  py_bonuses(blank_treasure,0);
			end
		      else
			msg_print('The enchantment fails...');
		    end;
		    ident := true;
		  end;
	  2  :  with inventory[23] do
		  begin
		    if (tval > 0) then
		    begin
		      objdes(out_val,23,false);
		      msg_print('Your ' + out_val + ' glows faintly!');
		      if (enchant(todam)) then
			begin
			  flags := uand(%X'7FFFFFFF',flags);
			  py_bonuses(blank_treasure,0);
			end
		      else
			msg_print('The enchantment fails...');
		    end;
		    ident := true;
		  end;
	  3  :  begin
		  i3 := 0;
		  i4 := 0;
		  if (inventory[26].tval > 0) then
		    begin
		      i3 := i3 + 1;
		      tmp[i3] := 26;
		    end;
		  if (inventory[27].tval > 0) then
		    begin
		      i3 := i3 + 1;
		      tmp[i3] := 27;
		    end;
		  if (inventory[32].tval > 0) then
		    begin
		      i3 := i3 + 1;
		      tmp[i3] := 32;
		    end;
		  if (inventory[28].tval > 0) then
		    begin
		      i3 := i3 + 1;
		      tmp[i3] := 28;
		    end;
		  if (inventory[24].tval > 0) then
		    begin
		      i3 := i3 + 1;
		      tmp[i3] := 24;
		    end;
		  if (i3 > 0) then i4 := tmp[randint(i3)];
		  if (uand(%X'80000000',inventory[26].flags) <> 0) then
		    i4 := 26
		  else if (uand(%X'80000000',inventory[27].flags) <> 0) then
		    i4 := 27
		  else if (uand(%X'80000000',inventory[32].flags) <> 0) then
		    i4 := 32
		  else if (uand(%X'80000000',inventory[24].flags) <> 0) then
		    i4 := 24
		  else if (uand(%X'80000000',inventory[28].flags) <> 0) then
		    i4 := 28;
		  if (i4 > 0) then
		    with inventory[i4] do
		      begin
			objdes(out_val,i4,false);
			msg_print('Your ' + out_val + ' glows faintly!');
			if (enchant(toac)) then
			  begin
			    flags := uand(%X'7FFFFFFF',flags);
			  py_bonuses(blank_treasure,0);
			  end
			else
			  msg_print('The enchantment fails...');
		      end;
		    ident := true;
		end;
	  4  :  begin
		  identify(inventory[item_val]);
		  msg_print('This is an identify scroll');
		  msg_print(' ');
		  if (ident_spell) then first := false;
		end;
	  5  :  if (remove_curse) then
		  begin
		    msg_print('You feel as if someone is watching over you.');
		    ident := true;;
		  end;
	  6  :  ident := light_area(char_row,char_col);
	  7  :  begin
		  for i3 := 1 to randint(3) do
		    begin
		      y := char_row;
		      x := char_col;
		      summon_monster(y,x,false);
		    end;
		  ident := true;
		end;
	  8  :  begin
		  teleport(10);
		  ident := true;
		end;
	  9  :  begin
		  teleport(100);
		  ident := true;
		end;
	  10 :  begin
		  dun_level := dun_level - 3 + 2*randint(2);
		  if (dun_level < 1) then
		    dun_level := 1;
		  moria_flag := true;
		  ident := true;
		end;
	  11 :  begin
		  msg_print('Your hands begin to glow.');
		  py.flags.confuse_monster := true;
		  ident := true;
		end;
	  12 :  ident := map_area;
	  13 :  ident := sleep_monsters1(char_row,char_col);
	  14 :  ident := warding_glyph;
	  15 :  ident := detect_treasure;
	  16 :  ident := detect_object;
	  17 :  ident := detect_trap;
	  18 :  ident := detect_sdoor;
	  19 :  begin
		  msg_print('This is a mass genocide scroll.');
		  msg_print(' ');
		  ident := mass_genocide;
		end;
	  20 :  ident := detect_invisible;
	  21 :  begin
		  ident := aggravate_monster(20);
		  msg_print('There is a high pitched humming noise');
		end;
	  22 :  ident := trap_creation;
	  23 :  ident := td_destroy;
	  24 :  ident := door_creation;
	  25 :  begin
		  identify(inventory[item_val]);
		  msg_print('This is a Recharge-Item scroll.');
		  msg_print(' ');
		  if (recharge(60)) then first := false;
		end;
	  26 :  begin
		  msg_print('This is a genocide scroll.');
		  msg_print(' ');
		  ident := genocide;
		end;
	  27 :  ident := unlight_area(char_row,char_col);
	  28 :  ident := protect_evil;
	  29 :  ident := create_food;
	  30 :  ident := dispell_creature(%X'0008',60);
	  31 :  begin
		  msg_print('That scroll appeared to be blank.');
		  ident := true;
		end;
	  32 :  with inventory[23] do
		  begin
		    if (tval > 0) then
		    begin
		      objdes(out_val,23,false);
		      msg_print('Your ' + out_val + ' glows brightly!');
		      flag := false;
		      for i3 := 1 to randint(2) do
			if (enchant(tohit)) then
			  flag := true;
		      for i3 := 1 to randint(2) do
			if (enchant(todam)) then
			  flag := true;
		      if (flag) then
			begin
			  flags := uand(%X'7FFFFFFF',flags);
			  py_bonuses(blank_treasure,0);
			end
		      else
			msg_print('The enchantment fails...');
		    end;
		    ident := true;
		  end;
	  33 :  with inventory[23] do
		  begin
		    if (tval > 0) then
		      begin
			inventory[inven_max] := inventory[23];
			objdes(out_val,23,false);
			msg_print('Your ' + out_val +
				' glows black, then fades.');
			tohit := -randint(5) - randint(5);
			todam := -randint(5) - randint(5);
			flags := %X'80000000';
			py_bonuses(inventory[inven_max],-1);
			ident := true;
		      end;
		  end;
	  34 :  begin
		  if (uand(%X'80000000',inventory[26].flags) <> 0) then
		    i3 := 26
		  else if (uand(%X'80000000',inventory[27].flags) <> 0) then
		    i3 := 27
		  else if (uand(%X'80000000',inventory[32].flags) <> 0) then
		    i3 := 32
		  else if (uand(%X'80000000',inventory[24].flags) <> 0) then
		    i3 := 24
		  else if (uand(%X'80000000',inventory[28].flags) <> 0) then
		    i3 := 28
		  else if (inventory[26].tval > 0) then
		    i3 := 26
		  else if (inventory[27].tval > 0) then
		    i3 := 27
		  else if (inventory[24].tval > 0) then
		    i3 := 24
		  else if (inventory[28].tval > 0) then
		    i3 := 28
		  else
		    i3 := 0;
		  if (i3 > 0) then
		    with inventory[i3] do
		      begin
			objdes(out_val,i3,false);
			msg_print('Your ' + out_val + ' glows brightly!');
			flag := false;
			for i3 := 1 to randint(2) + 1 do
			  if (enchant(toac)) then
			    flag := true;
			if (flag) then
			  begin
			    flags := uand(%X'7FFFFFFF',flags);
			  py_bonuses(blank_treasure,0);
			  end
			else
			  msg_print('The enchantment fails...');
		      end;
		    ident := true;
		end;
	  35 :  begin
		  if ((inventory[26].tval > 0) and (randint(4) = 1)) then
		    i3 := 26
		  else if ((inventory[27].tval > 0) and (randint(3) = 1)) then
		    i3 := 27
		  else if ((inventory[32].tval > 0) and (randint(3) = 1)) then
		    i3 := 32
		  else if ((inventory[24].tval > 0) and (randint(3) = 1)) then
		    i3 := 24
		  else if ((inventory[28].tval > 0) and (randint(3) = 1)) then
		    i3 := 28
		  else if (inventory[26].tval > 0) then
		    i3 := 26
		  else if (inventory[27].tval > 0) then
		    i3 := 27
		  else if (inventory[32].tval > 0) then
		    i3 := 32
		  else if (inventory[24].tval > 0) then
		    i3 := 24
		  else if (inventory[28].tval > 0) then
		    i3 := 28
		  else
		    i3 := 0;
		  if (i3 > 0) then
		    with inventory[i3] do
		      begin
			inventory[inven_max] := inventory[i3];
			objdes(out_val,i3,false);
		      msg_print('Your ' + out_val +
				' glows black, then fades.');
			flags := %X'80000000';
			toac := -randint(5) - randint(5);
			py_bonuses(inventory[inven_max],-1);
			ident := true;
		      end;
		end;
	  36 :  begin
		  for i3 := 1 to randint(3) do
		    begin
		      y := char_row;
		      x := char_col;
		      summon_undead(y,x);
		    end;
		  ident := true;
		end;
	  37 :  ident := bless(randint(12)+6);
	  38 :  ident := bless(randint(24)+12);
	  39 :  ident := bless(randint(48)+24);
	  40 :  begin
		  ident := true;
		  py.flags.word_recall := 25 + randint(30);
		  msg_print('The air about you becomes charged...');
		end;
	  41 :  ident := destroy_area(char_row,char_col);
	  42 :  ;
	  43 :  ;
	  44 :  ;
	  45 :  ;
	  46 :  ;
	  47 :  ;
	  48 :  ;
	  49 :  ;
	  50 :  ;
	  51 :  ;
	  52 :  ;
	  53 :  ;
	  54 :  ;
	  55 :  ;
	  56 :  ;
	  57 :  ;
	  58 :  ;
	  59 :  ;
	  60 :  ;
	  61 :  ;
	  62 :  ;
	  otherwise ;
	end;
	{ End of Scrolls...                                     }
			  end;
			if (not(reset_flag)) then
			  begin
			    if (ident) then
			      identify(inventory[item_val]);
			    if (not(first)) then
			      begin
				desc_remain(item_val);
				inven_destroy(item_val);
				if (flags <> 0) then
				  with py.misc do
				    begin
				      exp := exp + round(level/lev);
				      prt_experience;
				    end;
			      end;
			  end;
		      end
		  else
		    if (redraw) then draw_cave;
		end
	    else
	      msg_print('You are not carrying any scrolls.');
	  end
	else
	  msg_print('But you are not carrying anything.');
      end;
