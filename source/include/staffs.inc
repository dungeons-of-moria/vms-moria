	{ Use a staff...					-RAK-	}
[psect(misc2$code)] procedure use;
      var
		i1				: unsigned;
		i2,i3,item_val,chance		: integer;
		out_val				: vtype;
		redraw,ident			: boolean;
      begin
	reset_flag := true;
	if (inven_ctr > 0) then
	  begin
	    if (find_range([55],i2,i3)) then
	      begin
		redraw := false;
		if (get_item(item_val,'Use which staff?',redraw,i2,i3)) then
		  with inventory[item_val] do
		    begin
		      if (redraw) then draw_cave;
			reset_flag := false;
			with py.misc do
			  chance := save + lev + int_adj - level - 5;
			if (py.flags.confused > 0) then
			  chance := trunc(chance/2.0);
			if (chance < 0) then chance := 0;
			if (randint(chance) < use_device) then
			  msg_print('You failed to use the staff properly.')
			else if (p1 > 0) then
			  begin
			    i1 := flags;
			    ident := false;
			    p1 := p1 - 1;
		            while (i1 > 0) do
			      begin
				i2 := bit_pos(i1);
	{ Staffs...						}
	case (i2) of
	  1  :	ident := light_area(char_row,char_col);
	  2  :	ident := detect_sdoor;
	  3  :	ident := detect_trap;
	  4  :	ident := detect_treasure;
	  5  :	ident := detect_object;
	  6  :	begin
		  teleport(100);
		  ident := true;
		end;
	  7  :	ident := earthquake;
	  8  :	begin
		  for i3 := 1 to randint(4) do
		    begin
		      y := char_row;
		      x := char_col;
		      summon_monster(y,x,false);
		    end;
		  ident := true;
		end;
	  9  :	ident := genocide;
	  10 :	ident := destroy_area(char_row,char_col);
	  11 :	ident := starlite(char_row,char_col);
	  12 :	ident := speed_monsters(+1);
	  13 :	ident := speed_monsters(-1);
	  14 :	ident := sleep_monsters2;
	  15 :	ident := hp_player(randint(8),'a staff.');
	  16 :	ident := detect_invisible;
	  17 :	begin
		  py.flags.fast := py.flags.fast + randint(30) + 15;
		  ident := true;
		end;
	  18 :	begin
		  py.flags.slow := py.flags.slow + randint(30) + 15;
		  ident := true;
		end;
	  19 :	ident := mass_poly;
	  20 :	if (remove_curse) then
		  begin
		    msg_print('The staff glows blue for a moment...');
		    ident := true;
		  end;
	  21 :	ident := detect_evil;
	  22 :	if ((cure_blindness) or (cure_poison) or (cure_confusion)) then
		  ident := true;
	  23 :	ident := dispell_creature(%X'0004',60);
	  24 :	ident := mass_genocide;
	  25 :	ident := unlight_area(char_row,char_col);
	  otherwise ;
	end;
	{ End of staff actions...				}
			      end;
			    if (ident) then
			      identify(inventory[item_val]);
			    if (flags <> 0) then
			      with py.misc do
				begin
				  exp := exp + round(level/lev);
				  prt_experience;
				end;
			    desc_charges(item_val);
			  end
		    end
		else
		  if (redraw) then draw_cave;
	      end
	    else
	      msg_print('You are not carrying any staffs.');
	  end
	else
	  msg_print('But you are not carrying anything.');
      end;
