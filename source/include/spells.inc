{ Following are spell procedure/functions			-RAK-	}
{ These routines are commonly used in the scroll, potion, wands, and    }
{ staves routines, and are occassionly called from other areas.         }
{ Now included are creature spells also...                      -RAK    }

	{ Sleep creatures adjacent to player			-RAK-	}
[psect(misc6$code)] function sleep_monsters1(y,x : integer) : boolean;
      var
		i1,i2           : integer;
      begin
	sleep_monsters1 := false;
	for i1 := y-1 to y+1 do
	  for i2 := x-1 to x+1 do
	    with cave[i1,i2] do
	      if (cptr > 1) then
		with m_list[cptr] do
		  with c_list[mptr] do
		    begin
		      sleep_monsters1 := true;
		      if ((randint(max_mons_level) < level) or
			 (uand(%X'1000',cdefense) <> 0)) then
			msg_print('The ' + name + ' is unaffected.')
		      else
			begin
			  msg_print('The ' + name + ' falls asleep.');
			  csleep := 500;
			end;
		    end;
      end;

	{ Detect any monsters on the current panel		-RAK-	}
[psect(misc6$code)] function detect_treasure : boolean;
      var
		i1,i2           : integer;
      begin
	detect_treasure := false;
	for i1 := panel_row_min to panel_row_max do
	  for i2 := panel_col_min to panel_col_max do
	    with cave[i1,i2] do
	      if (tptr > 0) then
		if (t_list[tptr].tval = 100) then
		  if (not (test_light(i1,i2))) then
		    begin
		      lite_spot(i1,i2);
		      tl := true;
		      detect_treasure := true;
		    end;
      end;

	{ Detect all objects on the current panel		-RAK-	}
[psect(misc6$code)] function detect_object : boolean;
      var
		i1,i2           : integer;
      begin
	detect_object := false;
	for i1 := panel_row_min to panel_row_max do
	  for i2 := panel_col_min to panel_col_max do
	    with cave[i1,i2] do
	      if (tptr > 0) then
		if (t_list[tptr].tval < 100) then
		  if (not (test_light(i1,i2))) then
		    begin
		      lite_spot(i1,i2);
		      tl := true;
		      detect_object := true;
		    end;
      end;

	{ Locates and displays traps on current panel		-RAK-	}
[psect(misc6$code)] function detect_trap : boolean;
      var
		i1,i2           : integer;
      begin
	detect_trap := false;
	for i1 := panel_row_min to panel_row_max do
	  for i2 := panel_col_min to panel_col_max do
	    with cave[i1,i2] do
	      if (tptr > 0) then
		if (t_list[tptr].tval = 101) then
		  begin
		    change_trap(i1,i2);
		    fm := true;
		    detect_trap := true;
		  end
		else if (t_list[tptr].tval = 2) then
		  with t_list[tptr] do
		    known2(name);
      end;

	{ Locates and displays all secret doors on current panel -RAK-	}
[psect(misc6$code)] function detect_sdoor : boolean;
      var
		i1,i2           : integer;
      begin
	detect_sdoor := false;
	for i1 := panel_row_min to panel_row_max do
	  for i2 := panel_col_min to panel_col_max do
	    with cave[i1,i2] do
	      if (tptr > 0) then
		{ Secret doors  }
		if (t_list[tptr].tval = 109) then
		  begin
		    fval := corr_floor3.ftval;
		    change_trap(i1,i2);
		    fm := true;
		    detect_sdoor := true;
		  end
		{ Staircases    }
		else if (t_list[tptr].tval in [107,108]) then
		  if (not(fm)) then
		    begin
		      fm := true;
		      lite_spot(i1,i2);
		      detect_sdoor := true;
		    end;
      end;

	{ Locates and displays all invisible creatures on current panel -RAK-}
[psect(misc6$code)] function detect_invisible : boolean;
      var
		i1              : integer;
		dummy           : char;
		flag            : boolean;
      begin
	flag := false;
	i1 := muptr;
	repeat
	  with m_list[i1] do
	    if (panel_contains(fy,fx)) then
	      if (uand(%X'10000',c_list[mptr].cmove) <> 0) then
		begin
		  ml := true;
		  print(c_list[mptr].cchar,fy,fx);
		  flag := true;
		end;
	  i1 := m_list[i1].nptr;
	until(i1 = 0);
	if (flag) then
	  begin
	    msg_print('You sense the presence of invisible creatures!');
	    msg_print(' ');
	    msg_flag := false;
	  end;
	detect_invisible := flag;
      end;

	{ Light an area: 1.  If corridor then light immediate area -RAK-}
	{                2.  If room then light entire room.            }
[psect(misc6$code)] function light_area(y,x : integer) : boolean;
      var
		i1,i2           : integer;
      begin
	msg_print('You are surrounded by a white light.');
	light_area := true;
	if ((cave[y,x].fval in [1,2]) and (dun_level > 0)) then
	  light_room(y,x)
	else
	  for i1 := y-1 to y+1 do
	    for i2 := x-1 to x+1 do
	      if (in_bounds(i1,i2)) then
		begin
		  if (not (test_light(i1,i2))) then
		    lite_spot(i1,i2);
		  cave[i1,i2].pl := true;
		end;
      end;

	{ Darken an area, opposite of light area		-RAK-	}
[psect(misc6$code)] function unlight_area(y,x : integer) : boolean;
      var
	i1,i2,i3,tmp1,tmp2              : integer;
	start_row,start_col             : integer;
	end_row,end_col                 : integer;
	flag                            : boolean;
      begin
	flag := false;
	if ((cave[y,x].fval in [1,2]) and (dun_level > 0)) then
	  begin
	    tmp1 := trunc(screen_height/2);
	    tmp2 := trunc(screen_width /2);
	    start_row := trunc(y/tmp1)*tmp1 + 1;
	    start_col := trunc(x/tmp2)*tmp2 + 1;
	    end_row := start_row + tmp1 - 1;
	    end_col := start_col + tmp2 - 1;
	    for i1 := start_row to end_row do
	      begin
		out_val := '';
		i3 := 0;
		for i2 := start_col to end_col do
		  with cave[i1,i2] do
		    if (fval in [1,2]) then
		      begin
			pl := false;
			fval := 1;
			if (not(test_light(i1,i2))) then
			  begin
			    if (i3 = 0) then
			      i3 := i2;
			    out_val := out_val + ' ';
			  end
			else if (i3 > 0) then
			  begin
			    flag := true;
			    print(out_val,i1,i3);
			    out_val := '';
			    i3 := 0;
			  end;
		      end
		    else if (i3 > 0) then
		      begin
			flag := true;
			print(out_val,i1,i3);
			out_val := '';
			i3 := 0;
		      end;
		if (i3 > 0) then
		  begin
		    flag := true;
		    print(out_val,i1,i3);
		  end;
	      end;
	  end
	else
	  for i1 := y-1 to y+1 do
	    for i2 := x-1 to x+1 do
	      if (in_bounds(i1,i2)) then
		with cave[i1,i2] do
		  if (fval in [4,5,6]) then
		    if (pl) then
		      begin
			pl := false;
			flag := true;
		      end;
	if (flag) then
	  begin
	    msg_print('Darkness surrounds you...');
	    unlight_area := true;
	  end
	else
	  unlight_area := false;
      end;

	{ Map the current area plus some			-RAK-	}
[psect(misc6$code)] function map_area : boolean;
      var
		i1,i2,i3,i4,i5,i6,i7,i8         : integer;
      begin
	map_area := true;
	i1 := panel_row_min - randint(10);
	i2 := panel_row_max + randint(10);
	i3 := panel_col_min - randint(20);
	i4 := panel_col_max + randint(20);
	for i5 := i1 to i2 do
	  for i6 := i3 to i4 do
	    if (in_bounds(i5,i6)) then
	      if (cave[i5,i6].fval in floor_set) then
		for i7 := i5-1 to i5+1 do
		  for i8 := i6-1 to i6+1 do
		    with cave[i7,i8] do
		      if (fval in pwall_set) then
			pl := true
		      else if (tptr > 0) then
			if (t_list[tptr].tval in light_set) then
			  fm := true;
	prt_map;
      end;

	{ Identify an object					-RAK-	}
[psect(misc6$code)] function ident_spell : boolean;
      var
		item_val                : integer;
		out_val                 : vtype;
		redraw                  : boolean;
      begin
	ident_spell := false;
	redraw := false;
	if (get_item(item_val,'Item you wish identified?',
						redraw,1,inven_ctr)) then
	  with inventory[item_val] do
	    begin
	      ident_spell := true;
	      identify(inventory[item_val]);
	      known2(name);
	      objdes(out_val,item_val,true);
	      msg_print(chr(item_val+96) + cur_char2(item_val)+' ' + out_val);
	    end;
	if (redraw) then
	  begin
	    msg_print(' ');
	    draw_cave;
	  end;
      end;

	{ Get all the monsters on the level pissed off...	-RAK-	}
[psect(misc6$code)] function aggravate_monster  (
			dis_affect              :       integer
						) : boolean;
      var
		i1              : integer;
      begin
	aggravate_monster := true;
	i1 := muptr;
	repeat
	  with m_list[i1] do
	    begin
	      csleep := 0;
	      if (cdis <= dis_affect) then
		if (cspeed < 2) then
		  cspeed := cspeed + 1;
	    end;
	  i1 := m_list[i1].nptr;
	until(i1 = 0);
      end;

	{ Surround the fool with traps (chuckle)		-RAK-	}
[psect(misc6$code)] function trap_creation : boolean;
      var
		i1,i2           : integer;
      begin
	trap_creation := true;
	for i1 := char_row-1 to char_row+1 do
	  for i2 := char_col-1 to char_col+1 do
	    with cave[i1,i2] do
	      if (fval in floor_set) then
		begin
		  if (tptr > 0) then
		    delete_object(i1,i2);
		  place_trap(i1,i2,1,randint(max_trapa));
		end;
      end;

	{ Surround the player with doors...			-RAK-	}
[psect(misc6$code)] function door_creation : boolean;
      var
		i1,i2,i3        : integer;
      begin
	door_creation := true;
	for i1 := char_row-1 to char_row+1 do
	  for i2 := char_col-1 to char_col+1 do
	    if ((i1 <> char_row) or (i2 <> char_col)) then
	      with cave[i1,i2] do
		if (fval in floor_set) then
		  begin
		    popt(i3);
		    if (tptr > 0) then
		      delete_object(i1,i2);
		    fopen := false;
		    tptr := i3;
		    t_list[i3] := door_list[2];
		    if (test_light(i1,i2)) then
		      lite_spot(i1,i2);
		  end;
      end;

	{ Destroys any adjacent door(s)/trap(s) 		-RAK-	}
[psect(misc6$code)] function td_destroy : boolean;
      var
		i1,i2           : integer;
      begin
	td_destroy := false;
	for i1 := char_row-1 to char_row+1 do
	  for i2 := char_col-1 to char_col+1 do
	    with cave[i1,i2] do
	      if (tptr > 0) then
		if (t_list[tptr].tval in [101,102,104,105,109]) then
		  begin
		    if (delete_object(i1,i2)) then
		      td_destroy := true;
		  end;
      end;

	{ Display all creatures on the current panel		-RAK-	}
[psect(misc6$code)] function detect_monsters : boolean;
      var
		i1,i2           : integer;
		flag            : boolean;
      begin
	flag := false;
	i1 := muptr;
	repeat
	  with m_list[i1] do
	    if (panel_contains(fy,fx)) then
	      if (uand(%X'10000',c_list[mptr].cmove) = 0) then
		begin
		  ml := true;
		  print(c_list[mptr].cchar,fy,fx);
		  flag := true;
		end;
	  i1 := m_list[i1].nptr;
	until(i1 = 0);
	if (flag) then
	  begin
	    msg_print('You sense the presense of monsters!');
	    msg_print(' ');
	    msg_flag := false;
	    detect_monsters := true;
	  end;
	detect_monsters := flag;
      end;


	{ Leave a line of light in given dir, blue light can sometimes	}
	{ hurt creatures...                                     -RAK-   }
[psect(misc6$code)] function light_line(dir,y,x : integer) : boolean;
      begin
	light_line := true;
	while (cave[y,x].fopen) do
	  begin
	    with cave[y,x] do
	      begin
		if (panel_contains(y,x)) then
		  begin
		    if (not((tl) or (pl))) then
		      if (fval = 2) then
			light_room(y,x)
		      else
			lite_spot(y,x);
		    if (cptr > 1) then
		      with m_list[cptr] do
			with c_list[mptr] do
			  if (uand(%X'0100',cdefense) <> 0) then
			    begin
			      msg_print('The ' + name + ' wails out in pain!');
			      i1 := mon_take_hit(cptr,damroll('2d8'));
			      if (i1 > 0) then
				msg_print('The ' + name +
						' dies in a fit of agony.');
			    end;
		  end;
		pl := true;
	      end;
	    move(dir,y,x);
	  end;
      end;


	{ Light line in all directions				-RAK-	}
[psect(misc6$code)] function starlite(y,x : integer) : boolean;
      var
	i1                      : integer;
      begin
	msg_print('The end of the staff bursts into a blue shimmering light.');
	starlite := true;
	for i1 := 1 to 9 do
	  if (i1 <> 5) then
	    light_line(i1,y,x);
      end;


	{ Disarms all traps/chests in a given direction 	-RAK-	}
[psect(misc6$code)] function disarm_all(dir,y,x : integer) : boolean;
      var
	i1,oldy,oldx            : integer;
      begin
	disarm_all := false;
	repeat
	  with cave[y,x] do
	    begin
	      if (tptr > 0) then
		with t_list[tptr] do
		  if (tval in [101,102]) then
		    begin
		      if (delete_object(y,x)) then
			disarm_all := true;
		    end
		  else if (tval = 105) then
		    begin
		      p1 := 0;
		    end
		  else if (tval = 109) then
		    begin
		      fval := corr_floor3.ftval;
		      change_trap(y,x);
		      fm := true;
		      disarm_all := true;
		    end
		  else if (tval = 2) then
		    if (flags > 0) then
		      begin
			msg_print('Click!');
			flags := 0;
			disarm_all := true;
			i1 := index(name,' (');
			if (i1 > 0) then
			  name := substr(name,1,i1-1);
			name := name + ' (Unlocked)';
			known2(name);
		      end;
	    end;
	  oldy := y;
	  oldx := x;
	  move(dir,y,x);
	until(not(cave[oldy,oldx].fopen))
      end;

	{ Return flags for given type area affect		-RAK-	}
[psect(misc6$code)] procedure get_flags (
			typ                             : integer;
			var weapon_type,harm_type       : integer;
			var destroy                     : obj_set
					);
      begin
	    case typ of
	1  :  begin     { Lightning     }
		weapon_type := %X'00080000';
		harm_type   := %X'0100';
		destroy     := [45,60,65];
	      end;
	2  :  begin     { Poison Gas    }
		weapon_type := %X'00100000';
		harm_type   := %X'0040';
		destroy     := [];
	      end;
	3  :  begin     { Acid          }
		weapon_type := %X'00200000';
		harm_type   := %X'0080';
		destroy     :=
			[12,20,21,22,30,31,32,33,34,35,36,55,70,71,80,104,105];
	      end;
	4  :  begin     { Frost         }
		weapon_type := %X'00400000';
		harm_type   := %X'0010';
		destroy     := [75,76];
	      end;
	5  :  begin     { Fire          }
		weapon_type := %X'00800000';
		harm_type   := %X'0020';
		destroy     :=
			[12,20,21,22,30,31,32,36,55,70,71,75,76,80,104,105];
	      end;
	6  :  begin     { Holy Orb      }
		weapon_type := %X'00000000';
		harm_type   := %X'0004';
		destroy     := [];
	      end;
	otherwise begin
		    weapon_type := 0;
		    harm_type   := 0;
		    destroy     := [];
		  end;
	end;
      end;


	{ Shoot a bolt in a given direction			-RAK-	}
[psect(misc6$code)] function fire_bolt  (
			typ,dir,y,x,dam : integer;
			bolt_typ        : ctype
					) : boolean;
      var
	i1,oldy,oldx,dist               : integer;
	weapon_type,harm_type           : integer;
	flag                            : boolean;
	dummy                           : obj_set;
      begin
	fire_bolt := true;
	flag := false;
	get_flags(typ,weapon_type,harm_type,dummy);
	oldy := y;
	oldx := x;
	dist := 0;
	repeat
	  move(dir,y,x);
	  if (test_light(oldy,oldx)) then
	    lite_spot(oldy,oldx)
	  else
	    unlite_spot(oldy,oldx);
	  dist := dist + 1;
	  if (dist > obj$bolt_range) then
	    flag := true
	  else
	    begin
	      with cave[y,x] do
		begin
		  if (fopen) then
		    begin
		      if (cptr > 1) then
			begin
			  flag := true;
			  with m_list[cptr] do
			    with c_list[mptr] do
			      begin
				msg_print('The ' + bolt_typ + ' strikes the '
						+ c_list[mptr].name + '.');
				if (uand(harm_type,cdefense) <> 0) then
				  dam := dam*2
				else if (uand(weapon_type,spells) <> 0) then
				  dam := trunc(dam/4.0);
				i1 := mon_take_hit(cptr,dam);
				if (i1 > 0) then
				  msg_print('The ' + c_list[i1].name +
						' dies in a fit of agony.')
				else
				  begin
				    if (panel_contains(y,x)) then
				      begin
					print(c_list[mptr].cchar,y,x);
					m_list[cptr].ml := true;
				      end;
				  end
			      end
			end
		      else if (panel_contains(y,x)) then
			print('*',y,x);
		    end
		  else
		    flag := true;
		end;
	      oldy := y;
	      oldx := x;
	    end;
	until (flag);
      end;


	{ Shoot a ball in a given direction.  Note that balls have an	}
	{ area affect....                                       -RAK-   }
[psect(misc6$code)] function fire_ball          (
			typ,dir,y,x,dam_hp      :       integer;
			descrip                 :       ctype
						) : boolean;
      var
	i1,i2,i3                        : integer;
	dam,max_dis,thit,tkill          : integer;
	oldy,oldx,dist                  : integer;
	weapon_type,harm_type           : integer;
	flag                            : boolean;
	destroy                         : obj_set;
      begin
	fire_ball := true;
	thit   := 0;
	tkill  := 0;
	max_dis := 2;
	get_flags(typ,weapon_type,harm_type,destroy);
	flag := false;
	oldy := y;
	oldx := x;
	dist := 0;
	repeat
	  move(dir,y,x);
	  dist := dist + 1;
	  if (test_light(oldy,oldx)) then
	    lite_spot(oldy,oldx)
	  else
	    unlite_spot(oldy,oldx);
	  if (dist > obj$bolt_range) then
	    flag := true
	  else
	    begin
	      with cave[y,x] do
		begin
		  if ((not(fopen)) or (cptr > 1)) then
		    begin
		      flag := true;
		      if (not(fopen)) then
			begin
			  y := oldy;
			  x := oldx;
			end;
	{ The ball hits and explodes...                 }
	{ The explosion...                      }
	for i1 := y-max_dis to y+max_dis do
	  for i2 := x-max_dis to x+max_dis do
	    if (in_bounds(i1,i2)) then
	      if (distance(y,x,i1,i2) <= max_dis) then
	      if (los(y,x,i1,i2)) then          { FIXED BUG V4.5        }
		with cave[i1,i2] do
		  begin
		    if (tptr > 0) then
		      if (t_list[tptr].tval in destroy) then
			delete_object(i1,i2);
		    if (fopen) then
		      begin
			if (panel_contains(i1,i2)) then print('*',i1,i2);
			if (cptr > 1) then
			  with m_list[cptr] do
			    with c_list[mptr] do
			      begin
				thit := thit + 1;
				dam := dam_hp;
				if (uand(harm_type,cdefense) <> 0) then
				  dam := dam*2
				else if (uand(weapon_type,spells) <> 0) then
				  dam := dam div 4;
				dam := trunc(dam/(distance(i1,i2,y,x)+1));
				i3 := mon_take_hit(cptr,dam);
				if (i3 > 0) then
				  tkill := tkill + 1
				else
				  begin
				    if (panel_contains(i1,i2)) then
				      begin
					print(cchar,i1,i2);
					ml := true;
				      end
				  end;
			      end;
		      end;
		  end;
	for i1 := (y - 2) to (y + 2) do
	  for i2 := (x - 2) to (x + 2) do
	    if (in_bounds(i1,i2)) then
	      if (panel_contains(i1,i2)) then
		if (distance(y,x,i1,i2) <= max_dis) then
		  begin
		    with cave[i1,i2] do
		      if (test_light(i1,i2)) then
			lite_spot(i1,i2)
		      else if (cptr = 1) then
			lite_spot(i1,i2)
		      else if (cptr > 1) then
			if (m_list[cptr].ml) then
			  lite_spot(i1,i2)
			else
			  unlite_spot(i1,i2)
		      else
			unlite_spot(i1,i2);
		  end;
	{ End  explosion...                     }
	  if (thit = 1) then
	    msg_print('The ' + descrip + ' envelopes a creature!')
	  else if (thit > 1) then
	    msg_print('The ' + descrip +
				' envelopes several creatures!');
	  if (tkill = 1) then
	    msg_print('There is a scream of agony!')
	  else if (tkill > 1) then
	    msg_print('There are several screams of agony!');
	{ End ball hitting...                   }
		    end
		  else if (panel_contains(y,x)) then
		    print('*',y,x);
		end;
	      oldy := y;
	      oldx := x;
	    end;
	until (flag);
      end;


	{ Breath weapon works like a fire_ball, but affects the player. }
	{ Note the area affect....                              -RAK-   }
[psect(misc6$code)] function breath             (
			typ,y,x,dam_hp          :       integer;
			ddesc                   :       vtype
						) : boolean;
      var
	i1,i2,i3                        : integer;
	dam,max_dis                     : integer;
	weapon_type,harm_type           : integer;
	flag                            : boolean;
	destroy                         : obj_set;
      begin
	breath := true;
	max_dis := 2;
	get_flags(typ,weapon_type,harm_type,destroy);
	for i1 := y-2 to y+2 do
	  for i2 := x-2 to x+2 do
	    if (in_bounds(i1,i2)) then
	      if (distance(y,x,i1,i2) <= max_dis) then
		with cave[i1,i2] do
		  begin
		    if (tptr > 0) then
		      if (t_list[tptr].tval in destroy) then
			delete_object(i1,i2);
		    if (fopen) then
		      begin
			if (panel_contains(i1,i2)) then
			  print('*',i1,i2);
			if (cptr > 1) then
			  with m_list[cptr] do
			    with c_list[mptr] do
			      begin
				dam := dam_hp;
				if (uand(harm_type,cdefense) <> 0) then
				  dam := dam*2
				else if (uand(weapon_type,spells) <> 0) then
				  dam := trunc(dam/4.0);
				dam := trunc(dam/(distance(i1,i2,y,x)+1));
				hp := hp - dam;
				csleep := 0;
				if (hp < 0) then
				  begin
				    monster_death(fy,fx,cmove);
				    delete_monster(cptr);
				  end;
			      end
			else if (cptr = 1) then
			  begin
			    dam := trunc(dam_hp/(distance(i1,i2,y,x)+1));
			    case typ of
				1 :     light_dam(dam,ddesc);
				2 :     poison_gas(dam,ddesc);
				3 :     acid_dam(dam,ddesc);
				4 :     cold_dam(dam,ddesc);
				5 :     fire_dam(dam,ddesc);
			    end;
			  end;
		      end;
		  end;
	for i1 := (y - 2) to (y + 2) do
	  for i2 := (x - 2) to (x + 2) do
	    if (in_bounds(i1,i2)) then
	      if (panel_contains(i1,i2)) then
		if (distance(y,x,i1,i2) <= max_dis) then
		  begin
		    with cave[i1,i2] do
		      if (test_light(i1,i2)) then
			lite_spot(i1,i2)
		      else if (cptr = 1) then
			lite_spot(i1,i2)
		      else if (cptr > 1) then
			if (m_list[cptr].ml) then
			  lite_spot(i1,i2)
			else
			  unlite_spot(i1,i2)
		      else
			unlite_spot(i1,i2);
		  end;
      end;


	{ Recharge a wand, staff, or rod.  Sometimes the item breaks. -RAK-}
[psect(misc6$code)] function recharge(num : integer) : boolean;
      var
		item_val        : integer;
		redraw          : boolean;
      begin
	recharge := false;
	redraw := false;
	if (get_item(item_val,'Recharge which item?',redraw,1,inven_ctr)) then
	  with inventory[item_val] do
	    if (tval in [55,60,65]) then
	      if (randint(8) = 1) then
		begin
		  recharge := true;
		  msg_print('There is a bright flash of light...');
		  inven_destroy(item_val);
		end
	      else
		begin
		  recharge := true;
		  num := trunc(num/(level+2));
		  p1 := p1 + 2 + randint(num);
		  if (index(name,'^') = 0) then
		    insert_str(name,' (%P1','^ (%P1');
		end;
	if (redraw) then
	  begin
	    msg_print(' ');
	    draw_cave;
	  end;
      end;

	{ Increase or decrease a creatures hit points		-RAK-	}
[psect(misc6$code)] function hp_monster(dir,y,x,dam : integer) : boolean;
      var
		i1                      : integer;
		flag                    : boolean;
      begin
	hp_monster := false;
	flag := false;
	repeat
	  move(dir,y,x);
	  with cave[y,x] do
	    begin
	      if (fopen) then
		begin
		  if (cptr > 1) then
		    begin
		      flag := true;
		      with m_list[cptr] do
			with c_list[mptr] do
			  begin
			    hp_monster := true;
			    i1 := mon_take_hit(cptr,dam);
			    if (i1 > 0) then
	msg_print('The ' + c_list[i1].name + ' dies in a fit of agony.')
			    else
			      msg_print('The ' + name + ' screams in agony.');
			  end
		    end
		end
	      else
		flag := true;
	    end;
	until (flag);
      end;

	{ Drains life; note it must be living...		-RAK-	}
[psect(misc6$code)] function drain_life(dir,y,x : integer) : boolean;
      var
		i1                      : integer;
		flag                    : boolean;
      begin
	drain_life := false;
	flag := false;
	repeat
	  move(dir,y,x);
	  with cave[y,x] do
	    begin
	      if (fopen) then
		begin
		  if (cptr > 1) then
		    begin
		      flag := true;
		      with m_list[cptr] do
			with c_list[mptr] do
			  if (uand(cdefense,%X'0008') = 0) then
			    begin
			      drain_life := true;
			      i1 := mon_take_hit(cptr,50);
			      if (i1 > 0) then
	msg_print('The ' + c_list[i1].name + ' dies in a fit of agony.')
			      else
	msg_print('The ' + name + ' screams in agony.');
			    end
		    end
		end
	      else
		flag := true;
	    end;
	until (flag);
      end;

	{ Increase or decrease a creatures speed		-RAK-	}
	{ NOTE: cannot slow a winning creature (BALROG)                 }
[psect(misc6$code)] function speed_monster(dir,y,x,spd : integer) : boolean;
      var
		flag                    : boolean;
      begin
	speed_monster := false;
	flag := false;
	repeat
	  move(dir,y,x);
	  with cave[y,x] do
	    begin
	      if (fopen) then
		begin
		  if (cptr > 1) then
		    begin
		      flag := true;
		      with m_list[cptr] do
			with c_list[mptr] do
			  if (spd > 0) then
			    begin
			      cspeed := cspeed + spd;
			      csleep := 0;
			    end
			  else if (randint(max_mons_level) > level) then
			    begin
			      cspeed := cspeed + spd;
			      csleep := 0;
			    end
			  else
			    msg_print('The ' + name + ' is unaffected.');
		      speed_monster := true;
		    end
		end
	      else
		flag := true;
	    end;
	until (flag);
      end;

	{ Confuse a creature					-RAK-	}
[psect(misc6$code)] function confuse_monster(dir,y,x : integer) : boolean;
      var
		flag                    : boolean;
      begin
	confuse_monster := false;
	flag := false;
	repeat
	  move(dir,y,x);
	  with cave[y,x] do
	    if (fopen) then
	      begin
		if (cptr > 1) then
		  with m_list[cptr] do
		    with c_list[mptr] do
		      begin
			confuse_monster := true;
			flag := true;
			if ((randint(max_mons_level) < level) or
			 (uand(%X'1000',cdefense) <> 0)) then
			  msg_print('The ' + name + ' is unaffected.')
			else
			  begin
			    confused := true;
			    csleep := 0;
			    msg_print('The ' + name + ' appears confused.');
			  end;
		      end;
	      end
	    else
	      flag := true;
	until (flag);
      end;

	{ Sleep a creature...					-RAK-	}
[psect(misc6$code)] function sleep_monster(dir,y,x : integer) : boolean;
      var
		flag                    : boolean;
      begin
	sleep_monster := false;
	flag := false;
	repeat
	  move(dir,y,x);
	  with cave[y,x] do
	    if (fopen) then
	      begin
		if (cptr > 1) then
		  with m_list[cptr] do
		    with c_list[mptr] do
		      begin
			sleep_monster := true;
			flag := true;
			if ((randint(max_mons_level) < level) or
			 (uand(%X'1000',cdefense) <> 0)) then
			  msg_print('The ' + name + ' is unaffected.')
			else
			  begin
			    csleep := 500;
			    msg_print('The ' + name + ' falls asleep.');
			  end;
		      end;
	      end
	    else
	      flag := true;
	until (flag);
      end;

	{ Turn stone to mud, delete wall....			-RAK-	}
[psect(misc6$code)] function wall_to_mud(dir,y,x : integer) : boolean;
      var
		i1                      : integer;
		out_val                 : vtype;
		flag                    : boolean;
      begin
	wall_to_mud := false;
	flag := false;
	repeat
	  move(dir,y,x);
	  with cave[y,x] do
	    if (in_bounds(y,x)) then
	      begin
		if (fval in wall_set) then
		  begin
		    flag := true;
		    twall(y,x,1,0);
		    if (test_light(y,x)) then
		      begin
			msg_print('The wall turns into mud.');
			wall_to_mud := true;
		      end;
		  end
		else if ((tptr > 0) and (not(fopen))) then
		  begin
		    flag := true;
		    if (panel_contains(y,x)) then
		      if (test_light(y,x)) then
			begin
			  inventory[inven_max] := t_list[tptr];
			  objdes(out_val,inven_max,false);
			  msg_print('The ' + out_val + ' turns into mud.');
			  wall_to_mud := true;
			end;
		    delete_object(y,x);
		  end;
		if (cptr > 1) then
		  with m_list[cptr] do
		    with c_list[mptr] do
		      if (uand(%X'0200',cdefense) <> 0) then
			begin
			  i1 := mon_take_hit(cptr,100);
			  flag := true;
			  if (ml) then
			    if (i1 > 0) then
			msg_print('The ' + name + ' dies in a fit of agony.')
			    else
			      msg_print('The ' + name + ' wails out in pain!');
			end;
	      end
	    else
	      flag := true;
	until (flag);
      end;


	{ Destroy all traps and doors in a given direction	-RAK-	}
[psect(misc6$code)] function td_destroy2(dir,y,x : integer) : boolean;
      begin
	td_destroy2 := false;
	repeat
	  move(dir,y,x);
	  with cave[y,x] do
	    if (tptr > 0) then
	      with t_list[tptr] do
		if (tval in [2,101,102,105,109]) then
		  begin
		    if (delete_object(y,x)) then
		      begin
			msg_print('There is a bright flash of light!');
			fopen := true;
			td_destroy2 := true;
		      end;
		  end;
	until (not(cave[y,x].fopen));
      end;

	{ Polymorph a monster					-RAK-	}
	{ NOTE: cannot polymorph a winning creature (BALROG)            }
[psect(misc6$code)] function poly_monster(dir,y,x : integer) : boolean;
      var
		dist                    : integer;
		flag                    : boolean;
      begin
	poly_monster := false;
	flag := false;
	dist := 0;
	repeat
	  move(dir,y,x);
	  dist := dist + 1;
	  if (dist <+ obj$bolt_range) then
	    begin
	      with cave[y,x] do
		begin
		  if (fopen) then
		    begin
		      if (cptr > 1) then
			with c_list[m_list[cptr].mptr] do
			  if (randint(max_mons_level) > level) then
			    begin
			      flag := true;
			      delete_monster(cptr);
			      place_monster(y,x,
			randint(m_level[max_mons_level])+ m_level[0],false);
			      if (panel_contains(y,x)) then
				if (test_light(y,x)) then
				  poly_monster := true;
			    end
			  else
			    msg_print('The ' + name + ' is unaffected.');
		    end
		  else
		    flag := true;
		end;
	    end
	  else
	    flag := true;
	until (flag);
      end;

	{ Create a wall...					-RAK-	}
[psect(misc6$code)] function build_wall(dir,y,x : integer) : boolean;
      var
		i1              : integer;
      begin
	build_wall := false;
	i1 := 0;
	move(dir,y,x);
	while ((cave[y,x].fopen) and (i1 < 10)) do
	  with cave[y,x] do
	    begin
	      if (tptr > 0) then
		delete_object(y,x);
	      if (cptr > 1) then
		mon_take_hit(cptr,damroll('2d8'));
	      fval  := rock_wall2.ftval;
	      fopen := rock_wall2.ftopen;
	      fm := false;
	      if (test_light(y,x)) then
		lite_spot(y,x);
	      i1 := i1 + 1;
	      build_wall := true;
	      move(dir,y,x);
	    end;
      end;

	{ Replicate a creature					-RAK-	}
[psect(misc6$code)] function clone_monster(dir,y,x : integer) : boolean;
      var
		flag                    : boolean;
      begin
	flag := false;
	clone_monster := false;
	repeat
	  move(dir,y,x);
	  with cave[y,x] do
	    if (cptr > 1) then
	      begin
		multiply_monster(y,x,m_list[cptr].mptr,false);
		if (panel_contains(y,x)) then
		  if (m_list[cptr].ml) then
		    clone_monster := true;
		flag := true;
	      end
	until ((not(cave[y,x].fopen)) or (flag));
      end;

	{ Move the creature record to a new location		-RAK-	}
[psect(misc6$code)] function teleport_away(monptr,dis : integer) : boolean;
      var
		yn,xn,ctr               : integer;
      begin
	with m_list[monptr] do
	  begin
	    ctr := 0;
	    repeat
	      repeat
		yn := fy + (randint(2*dis+1) - (dis + 1));
		xn := fx + (randint(2*dis+1) - (dis + 1));
	      until(in_bounds(yn,xn));
	      ctr := ctr + 1;
	      if (ctr > 9) then
		begin
		  ctr := 0;
		  dis := dis + 5;
		end;
	    until ((cave[yn,xn].fopen) and (cave[yn,xn].cptr = 0));
	    move_rec(fy,fx,yn,xn);
	    if (test_light(fy,fx)) then
	      lite_spot(fy,fx);
	    fy := yn;
	    fx := xn;
	    ml := false;
	    teleport_away := true;
	  end;
      end;

	{ Teleport player to spell casting creature		-RAK-	}
[psect(misc6$code)] function teleport_to(ny,nx : integer) : boolean;
      var
	dis,ctr,y,x,i1,i2               : integer;
      begin
	teleport_to := true;
	dis := 1;
	ctr := 0;
	repeat
	  y := ny + (randint(2*dis+1) - (dis + 1));
	  x := nx + (randint(2*dis+1) - (dis + 1));
	  ctr := ctr + 1;
	  if (ctr > 9) then
	    begin
	      ctr := 0;
	      dis := dis + 1;
	    end;
	until ((cave[y,x].fopen) and (cave[y,x].cptr < 2));
	move_rec(char_row,char_col,y,x);
	for i1 := char_row-1 to char_row+1 do
	  for i2 := char_col-1 to char_col+1 do
	    with cave[i1,i2] do
	      begin
		tl := false;
		if (not(test_light(i1,i2))) then
		  unlite_spot(i1,i2);
	      end;
	if (test_light(char_row,char_col)) then
	  lite_spot(char_row,char_col);
	char_row := y;
	char_col := x;
	move_char(5);
	creatures(false);
      end;

	{ Teleport all creatures in a given direction away	-RAK-	}
[psect(misc6$code)] function teleport_monster(dir,y,x : integer) : boolean;
      var
		yn,xn                   : integer;
		flag                    : boolean;
      begin
	flag := false;
	teleport_monster := false;
	repeat
	  move(dir,y,x);
	  with cave[y,x] do
	    if (cptr > 1) then
	      begin
		teleport_away(cptr,max_sight);
		teleport_monster := true;
	      end;
	until ((not(cave[y,x].fopen)) or (flag));
      end;

	{ Delete all creatures within MAX_SIGHT distance	-RAK-	}
	{ NOTE : Winning creatures cannot be genocided                  }
[psect(misc6$code)] function mass_genocide : boolean;
      var
		i1,i2           : integer;
      begin
	mass_genocide := false;
	i1 := muptr;
	repeat
	  with m_list[i1] do
	    with c_list[mptr] do
	      begin
		i2 := nptr;
		if (cdis <= max_sight) then
		  if (uand(cdefense,%X'80000000') = 0) then
		    begin
		      delete_monster(i1);
		      mass_genocide := true;
		    end;
	      end;
	  i1 := i2;
	until(i1 = 0);
      end;

	{ Delete all creatures of a given type from level.	-RAK-	}
	{ This does not keep creatures of type from appearing later.    }
	{ NOTE : Winning creatures that are genocided will be considered}
	{        as teleporting to another level.  Genocide will NOT win}
	{        the game...                                            }
[psect(misc6$code)] function genocide : boolean;
      var
		i1,i2           : integer;
		typ             : char;
      begin
	genocide := true;
	i1 := muptr;
	if (get_com('Which type of creature do wish exterminated?',typ)) then
	  repeat
	    with m_list[i1] do
	      with c_list[mptr] do
		begin
		  i2 := nptr;
		  if (typ = c_list[mptr].cchar) then
		    if (uand(cdefense,%X'80000000') = 0) then
		      delete_monster(i1)
		    else
		      msg_print('The ' + name + ' is unaffected.');
		end;
	    i1 := i2;
	  until(i1 = 0);
      end;

	{ Change speed of any creature player can see....	-RAK-	}
	{ NOTE: cannot slow a winning creature (BALROG)                 }
[psect(misc6$code)] function speed_monsters(spd : integer) : boolean;
      var
		i1,i2           : integer;
      begin
	i1 := muptr;
	speed_monsters := false;
	repeat
	  with m_list[i1] do
	    begin
	      i2 := nptr;
	      if (ml) then
		with c_list[mptr] do
		  if (spd > 0) then
		    begin
		      cspeed := cspeed + spd;
		      csleep := 0;
		      speed_monsters := true;
		    end
		  else if (randint(max_mons_level) > level) then
		    begin
		      cspeed := cspeed + spd;
		      csleep := 0;
		      speed_monsters := true;
		    end
		  else
		    msg_print('The ' + name + ' is unaffected.');
	    end;
	  i1 := i2;
	until(i1 = 0);
      end;

	{ Sleep any creature that player can see		-RAK-	}
[psect(misc6$code)] function sleep_monsters2 : boolean;
      var
		i1,i2           : integer;
      begin
	i1 := muptr;
	sleep_monsters2 := false;
	repeat
	  with m_list[i1] do
	    with c_list[mptr] do
	      begin
		i2 := nptr;
		sleep_monsters2 := true;
		if (ml) then
		  begin
		    if ((randint(max_mons_level) < level) or (uand(%X'1000',cdefense) <> 0)) then
		      msg_print('The ' + name + ' is unaffected.')
		    else
		      csleep := 500;
		  end;
	      end;
	  i1 := i2;
	until(i1 = 0);
      end;

	{ Polymorph any creature that player can see... 	-RAK-	}
	{ NOTE: cannot polymorph a winning creature (BALROG)            }
[psect(misc6$code)] function mass_poly : boolean;
      var
		i1,i2,y,x       : integer;
      begin
	i1 := muptr;
	mass_poly := false;
	repeat
	  with m_list[i1] do
	    begin
	      i2 := nptr;
	      if (cdis < max_sight) then
		with c_list[mptr] do
		  if (uand(cdefense,%X'80000000') = 0) then
		    begin
		      y := fy;
		      x := fx;
		      delete_monster(i1);
		      place_monster(y,x,randint(m_level[max_mons_level])
							+ m_level[0],false);
		      mass_poly := true;
		    end;
	    end;
	  i1 := i2;
	until(i1 = 0);
      end;

	{ Display evil creatures on current panel		-RAK-	}
[psect(misc6$code)] function detect_evil : boolean;
      var
		i1              : integer;
		dummy           : char;
		flag            : boolean;
      begin
	flag := false;
	i1 := muptr;
	repeat
	  with m_list[i1] do
	    if (panel_contains(fy,fx)) then
	      if (uand(%X'0004',c_list[mptr].cdefense) <> 0) then
		begin
		  ml := true;
		  print(c_list[mptr].cchar,fy,fx);
		  flag := true;
		end;
	  i1 := m_list[i1].nptr;
	until(i1 = 0);
	if (flag) then
	  begin
	    msg_print('You sense the presence of evil!');
	    msg_print(' ');
	    msg_flag := false;
	  end;
	detect_evil := flag;
      end;

	{ Change players hit points in some manner		-RAK-	}
[psect(misc6$code)] function hp_player(num : integer; kind : vtype) : boolean;
      begin
	hp_player := false;
	with py.misc do
	  begin
	    if (num < 0) then
	      begin
		take_hit(num,kind);
		if (chp < 0) then
		  msg_print('You feel your life slipping away!');
		hp_player := true;
	      end
	    else if (chp < mhp) then
	      begin
		chp := chp + num;
		if (chp > mhp) then
		  chp := mhp;
		prt_chp;
		case trunc(num/5) of
		  0          :  msg_print('You feel a little better.');
		  1,2        :  msg_print('You feel better.');
		  3,4,5,6    :  msg_print('You feel much better.');
		  otherwise     msg_print('You feel very good.');
		end;
		hp_player := true;
	      end;
	  end;
      end;

	{ Cure players confusion				-RAK-	}
[psect(misc6$code)] function cure_confusion : boolean;
      begin
	cure_confusion := false;
	with py.flags do
	  if (confused > 1) then
	    begin
	      confused := 1;
	      cure_confusion := true;
	    end;
      end;

	{ Cure players blindness				-RAK-	}
[psect(misc6$code)] function cure_blindness : boolean;
      begin
	cure_blindness := false;
	with py.flags do
	  if (blind > 1) then
	    begin
	      blind := 1;
	      cure_blindness := true;
	    end;
      end;

	{ Cure poisoning					-RAK-	}
[psect(misc6$code)] function cure_poison : boolean;
      begin
	cure_poison := false;
	with py.flags do
	  if (poisoned > 1) then
	    begin
	      poisoned := 1;
	      cure_poison := true;
	    end;
      end;

	{ Cure the players fear 				-RAK-	}
[psect(misc6$code)] function remove_fear : boolean;
      begin
	remove_fear := false;
	with py.flags do
	  if (afraid > 1) then
	    begin
	      afraid := 1;
	      remove_fear := true;
	    end;
      end;

	{ This is a fun one.  In a given block, pick some walls and	}
	{ turn them into open spots.  Pick some open spots and turn     }
	{ them into walls.  An "Earthquake" effect...           -RAK-   }
[psect(misc6$code)] function earthquake : boolean;
      var
		i1,i2           : integer;
      begin
	earthquake := true;
	for i1 := char_row-8 to char_row+8 do
	  for i2 := char_col-8 to char_col+8 do
	    if ((i1 <> char_row) or (i2 <> char_col)) then
	      if (in_bounds(i1,i2)) then
		if (randint(8) = 1) then
		  begin
		    with cave[i1,i2] do
		      begin
			if (tptr > 0) then
			  delete_object(i1,i2);
			if (cptr > 1) then
			  mon_take_hit(cptr,damroll('2d8'));
			if (fval in wall_set) then
			  begin
			    if (next_to4(i1,i2,[1,2]) > 0) then
			      begin
				fval  := corr_floor2.ftval;
				fopen := corr_floor2.ftopen;
			      end
			    else
			      begin
				fval  := corr_floor1.ftval;
				fopen := corr_floor1.ftopen;
			      end;
			    if (test_light(i1,i2)) then unlite_spot(i1,i2);
			    pl := false;
			    fm := false;
			    if (tl) then lite_spot(i1,i2);
			  end
			else if (fval in floor_set) then
			  begin
			    case randint(10) of
			      1,2,3,4,5 : begin
					    fval  := rock_wall3.ftval;
					    fopen := rock_wall3.ftopen;
					  end;
			      6,7,8     : begin
					    fval  := rock_wall2.ftval;
					    fopen := rock_wall2.ftopen;
					  end;
			      9,10      : begin
					    fval  := rock_wall1.ftval;
					    fopen := rock_wall1.ftopen;
					  end;
			    end;
			    fm := false;
			  end;
		      end;
		    if (test_light(i1,i2)) then
		      lite_spot(i1,i2);
		  end;
      end;

	{ Evil creatures don't like this...                     -RAK-   }
[psect(misc6$code)] function protect_evil : boolean;
      begin
	protect_evil := true;
	with py.flags do
	  protevil := protevil + randint(25) + 3*py.misc.lev;
      end;

	{ Create some high quality mush for the player. 	-RAK-	}
[psect(misc6$code)] function create_food : boolean;
      begin
	create_food := true;
	with cave[char_row,char_col] do
	  begin
	    if (tptr > 0) then
	      delete_object(char_row,char_col);
	    place_object(char_row,char_col);
	    t_list[tptr] := mush;
	  end;
      end;


	{ Attempts to destroy a type of creature.  Success depends on	}
	{ the creatures level VS. the player's level            -RAK-   }
[psect(misc6$code)] function dispell_creature(
				cflag   : integer;
				damage  : integer) : boolean;
      var
	i1,m_next               : integer;
      begin
	i1 := muptr;
	dispell_creature := false;
	repeat
	  m_next := m_list[i1].nptr;
	  with m_list[i1] do
	    if (ml) then
	      if (uand(cflag,c_list[mptr].cdefense) <> 0) then
		  begin
		    hp := hp - randint(damage);
		    csleep := 0;
		    if (hp < 0) then
		      begin
			msg_print('The ' + c_list[mptr].name + ' dissolves!');
			monster_death(fy,fx,c_list[mptr].cmove);
			with c_list[mptr] do
			  with py.misc do
			    exp := exp + round(mexp*(level/lev));
			delete_monster(i1);
		      end
		    else
		      msg_print('The ' + c_list[mptr].name + ' shudders.');
		    dispell_creature := true;
		  end;
	  i1 := m_next;
	until(i1 = 0);
      end;

	{ Attempt to turn (confuse) undead creatures... 	-RAK-	}
[psect(misc6$code)] function turn_undead : boolean;
      var
		i1              : integer;
      begin
	i1 := muptr;
	turn_undead := false;
	repeat
	  with m_list[i1] do
	    if (panel_contains(fy,fx)) then
	      if (ml) then
		if (uand(%X'0008',c_list[mptr].cdefense) <> 0) then
		  begin
		    if (((py.misc.lev+1) > c_list[mptr].level) or
			(randint(5) = 1)) then
		      begin
			msg_print('The ' + c_list[mptr].name +
						' runs frantically!');
			confused := true;
		      end
		    else
		      msg_print('The ' + c_list[mptr].name +
						' is unaffected.');
		    turn_undead := true;
		  end;
	  i1 := m_list[i1].nptr;
	until(i1 = 0);
      end;

	{ Leave a glyph of warding... Creatures will not pass over! -RAK-}
[psect(misc6$code)] function warding_glyph : boolean;
      var
		i1                      : integer;
      begin
	warding_glyph := true;
	with cave[char_row,char_col] do
	  if (tptr = 0) then
	    begin
	      popt(i1);
	      tptr := i1;
	      t_list[i1] := scare_monster;
	    end;
      end;

	{ Lose a strength point.				-RAK-	}
[psect(misc6$code)] function lose_str : boolean;
      begin
	lose_str := true;
	if (not(py.flags.sustain_str)) then
	  begin
	    with py.stat do
	      cstr := de_statp(cstr);
	    msg_print('You feel very sick.');
	    prt_strength;
	  end
	else
	  msg_print('You feel sick for a moment, then it passes.');
      end;

	{ Lose an intelligence point.				-RAK-	}
[psect(misc6$code)] function lose_int : boolean;
      begin
	lose_int := true;
	if (not(py.flags.sustain_int)) then
	  begin
	    with py.stat do
	      cint := de_statp(cint);
	    msg_print('You become very dizzy.');
	    prt_intelligence;
	  end
	else
	  msg_print('You become dizzy for a moment, then it passes.');
      end;

	{ Lose a wisdom point.					-RAK-	}
[psect(misc6$code)] function lose_wis : boolean;
      begin
	lose_wis := true;
	if (not(py.flags.sustain_wis)) then
	  begin
	    with py.stat do
	      cwis := de_statp(cwis);
	    msg_print('You feel very naive.');
	    prt_wisdom;
	  end
	else
	  msg_print('You feel naive for a moment, then it passes.');
      end;

	{ Lose a dexterity point.				-RAK-	}
[psect(misc6$code)] function lose_dex : boolean;
      begin
	lose_dex := true;
	if (not(py.flags.sustain_dex)) then
	  begin
	    with py.stat do
	      cdex := de_statp(cdex);
	    msg_print('You feel very sore.');
	    prt_dexterity;
	  end
	else
	  msg_print('You feel sore for a moment, then it passes.');
      end;

	{ Lose a constitution point.				-RAK-	}
[psect(misc6$code)] function lose_con : boolean;
      begin
	lose_con := true;
	if (not(py.flags.sustain_con)) then
	  begin
	    with py.stat do
	      ccon := de_statp(ccon);
	    msg_print('You feel very sick.');
	    prt_constitution;
	  end
	else
	  msg_print('You feel sick for a moment, then it passes.');
      end;

	{ Lose a charisma point.				-RAK-	}
[psect(misc6$code)] function lose_chr : boolean;
      begin
	lose_chr := true;
	if (not(py.flags.sustain_chr)) then
	  begin
	    with py.stat do
	      cchr := de_statp(cchr);
	    msg_print('Your skin starts to itch.');
	    prt_charisma;
	  end
	else
	  msg_print('Your skin starts to itch, but feels better now.');
      end;

	{ Lose experience					-RAK-	}
[psect(misc6$code)] procedure lose_exp(amount : integer);
    var
	i1,i2                           : integer;
	av_hp,lose_hp                   : integer;
	av_mn,lose_mn                   : integer;
	flag                            : boolean;
    begin
      with py.misc do
	begin
	  if (amount > exp) then
	    exp := 0
	  else
	    exp := exp - amount;
	  i1 := 1;
	  while (trunc(player_exp[i1]*expfact) <= exp) do
	    i1 := i1 + 1;
	  i2 := lev - i1;
	  while (i2 > 0) do
	    begin
	      av_hp := trunc(mhp/lev);
	      av_mn := trunc(mana/lev);
	      lev   := lev - 1;
	      i2    := i2 - 1;
	      lose_hp := randint(av_hp*2-1);
	      lose_mn := randint(av_mn*2-1);
	      mhp  := mhp  - lose_hp;
	      mana := mana - lose_mn;
	      if (mhp  < 1) then mhp  := 1;
	      if (mana < 0) then mana := 0;
	      with class[pclass] do
		if ((mspell) or (pspell)) then
		  begin
		    i1 := 32;
		    flag := false;
		    repeat
		      i1 := i1 - 1;
		      if (magic_spell[pclass,i1].learned) then
			flag := true;
		    until((flag) or (i1 < 2));
		    if (flag) then
		      begin
			magic_spell[pclass,i1].learned := false;
			if (mspell) then
			  msg_print('You have forgotten a magic spell!')
			else
			  msg_print('You have forgotten a prayer!');
		      end;
		  end;
	    end;
	  if (chp   > mhp)  then chp   := mhp;
	  if (cmana > mana) then cmana := mana;
	  title := player_title[pclass,lev];
	  prt_experience;
	  prt_mhp;
	  prt_chp;
	  prt_cmana;
	  prt_level;
	  prt_title;
	end;
    end;

	{ Slow Poison						-RAK-	}
[psect(misc6$code)] function slow_poison : boolean;
    begin
      slow_poison := false;
      with py.flags do
	if (poisoned > 0) then
	  begin
	    poisoned := trunc(poisoned/2.0);
	    if (poisoned < 1) then poisoned := 1;
	    slow_poison := true;
	    msg_print('The effects of the poison has been reduced.');
	  end;
    end;

	{ Bless 						-RAK-	}
[psect(misc6$code)] function bless(amount : integer) : boolean;
    begin
      bless := true;
      with py.flags do
	blessed := blessed + amount;
    end;

	{ Detect Invisible for period of time			-RAK-	}
[psect(misc6$code)] function detect_inv2(amount : integer) : boolean;
    begin
      detect_inv2 := true;
      with py.flags do
	detect_inv := detect_inv + amount;
    end;

	{ The spell of destruction...				-RAK-	}
	{ NOTE : Winning creatures that are deleted will be considered  }
	{        as teleporting to another level.  This will NOT win the}
	{        game...                                                }
[psect(misc6$code)] function destroy_area(y,x : integer) : boolean;
    var
	i1,i2,i3                        : integer;

    procedure replace_spot(y,x,typ : integer);
      begin
	with cave[y,x] do
	  begin
	    case typ of
	      1,2,3  :  begin
			  fval  := corr_floor1.ftval;
			  fopen := corr_floor1.ftopen;
			end;
	      4,7,10 :  begin
			  fval  := rock_wall1.ftval;
			  fopen := rock_wall1.ftopen;
			end;
	      5,8,11 :  begin
			  fval  := rock_wall2.ftval;
			  fopen := rock_wall2.ftopen;
			end;
	      6,9,12 :  begin
			  fval  := rock_wall3.ftval;
			  fopen := rock_wall3.ftopen;
			end;
	    end;
	    pl := false;
	    fm := false;
	    if (tptr > 0) then
	      delete_object(y,x);
	    if (cptr > 1) then
	      delete_monster(cptr);
	  end;
      end;

    begin
      destroy_area := true;
      if (dun_level > 0) then
	begin
	  for i1 := (y-15) to (y+15) do
	    for i2 := (x-15) to (x+15) do
	      if (in_bounds(i1,i2)) then
		if (cave[i1,i2].fval <> 15) then
		  begin
		    i3 := distance(i1,i2,y,x);
		    if (i3 < 13) then
		      replace_spot(i1,i2,randint(6))
		    else if (i3 < 16) then
		      replace_spot(i1,i2,randint(9));
		  end;
	end;
      msg_print('There is a searing blast of light!');
      py.flags.blind := py.flags.blind + 10 + randint(10);
    end;


	{ Enchants a plus onto an item...			-RAK-	}
[psect(misc6$code)] function enchant(var pluses : worlint) : boolean;
    var
	chance                  : integer;
    begin
      chance := 0;
      enchant := false;
      if (pluses > 0) then
	case pluses of
	  1  :  chance := 040;
	  2  :  chance := 100;
	  3  :  chance := 200;
	  4  :  chance := 400;
	  5  :  chance := 600;
	  6  :  chance := 700;
	  7  :  chance := 800;
	  8  :  chance := 900;
	  9  :  chance := 950;
	  otherwise chance := 995;
	end;
      if (randint(1000) > chance) then
	begin
	  pluses := pluses + 1;
	  enchant := true;
	end;
    end;


	{ Removes curses from items in inventory		-RAK-	}
[psect(misc6$code)] function remove_curse : boolean;
    var
	i1                      : integer;
    begin
      remove_curse := false;
      for i1 := 23 to 32 do
	with inventory[i1] do
	  if (uand(%X'80000000',flags) <> 0) then
	    begin
	      flags := uand(%X'7FFFFFFF',flags);
	      py_bonuses(blank_treasure,0);
	      remove_curse := true;
	    end;
    end;


	{ Restores any drained experience			-RAK-	}
[psect(misc6$code)] function restore_level : boolean;
    begin
      restore_level := false;
      with py.misc do
	if (max_exp > exp) then
	  begin
	    restore_level := true;
	    msg_print('You feel your life energies returning...');
	    while (exp < max_exp) do
	      begin
		exp := max_exp;
		prt_experience;
	      end;
	  end;
    end;

