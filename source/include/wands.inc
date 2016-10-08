	{ Wands for the aiming...				}
[psect(misc2$code)] procedure aim;
      var
		i1				: unsigned;
		i2,i3,i4,chance			: integer;
		dir,item_val			: integer;
		dumy,y_dumy,x_dumy		: integer;
		out_val				: vtype;
		redraw,ident			: boolean;
      begin
	redraw := false;
	reset_flag := true;
	if (inven_ctr > 0) then
	  begin
	    if (find_range([65],i2,i3)) then
	      begin
		if (get_item(item_val,'Aim which wand?',redraw,i2,i3)) then
		  with inventory[item_val] do
		    begin
		      if (redraw) then draw_cave;
		      reset_flag := false;
		      redraw := false;
		      y_dumy := char_row;
		      x_dumy := char_col;
		if (get_dir('Which direction?',dir,dumy,y_dumy,x_dumy)) then
		        begin
			  if (py.flags.confused > 0) then
			    begin
			      msg_print('You are confused...');
			      repeat
				dir := randint(9);
			      until(dir <> 5);
			    end;
		          i1 := flags;
		          ident := false;
			  with py.misc do
			    chance := save + lev + int_adj - level;
			  if (py.flags.confused > 0) then 
			    chance := trunc(chance/2.0);
			  if (chance < 0) then chance := 0;
			  if (randint(chance) < use_device) then
			    msg_print('You failed to use the wand properly.')
			  else if (p1 > 0) then
			    begin
			      p1 := p1 - 1;
		              while (i1 > 0) do
			        begin
			          i2 := bit_pos(i1);
			          i3 := char_row;
			          i4 := char_col;
	{ Wands							}
	case (i2) of
	  1  :	begin
		  msg_print('A line of blue shimmering light appears.');
		  light_line(dir,char_row,char_col);
		  ident := true;
		end;
	  2  :	begin
		  fire_bolt(1,dir,i3,i4,damroll('3d8'),'Lightning Bolt');
		  ident := true;
		end;
	  3  :	begin
		  fire_bolt(4,dir,i3,i4,damroll('4d8'),'Frost Bolt');
		  ident := true;
		end;
	  4  :	begin
		  fire_bolt(5,dir,i3,i4,damroll('6d8'),'Fire Bolt');
		  ident := true;
		end;
	  5  :	ident := wall_to_mud(dir,i3,i4);
	  6  :	ident := poly_monster(dir,i3,i4);
	  7  :	ident := hp_monster(dir,i3,i4,-damroll('4d6'));
	  8  :	ident := speed_monster(dir,i3,i4,1);
	  9  :	ident := speed_monster(dir,i3,i4,-1);
	  10 :	ident := confuse_monster(dir,i3,i4);
	  11 :	ident := sleep_monster(dir,i3,i4);
	  12 :	ident := drain_life(dir,i3,i4);
	  13 :	ident := td_destroy2(dir,i3,i4);
	  14 :	begin
		  fire_bolt(0,dir,i3,i4,damroll('2d6'),'Magic Missle');
		  ident := true;
		end;
	  15 :	ident := build_wall(dir,i3,i4);
	  16 :	ident := clone_monster(dir,i3,i4);
	  17 :	ident := teleport_monster(dir,i3,i4);
	  18 :	ident := disarm_all(dir,i3,i4);
	  19 :	begin
		  fire_ball(1,dir,i3,i4,24,'Lightning Ball');
		  ident := true;
		end;
	  20 :	begin
		  fire_ball(4,dir,i3,i4,32,'Cold Ball');
		  ident := true;
		end;
	  21 :	begin
		  fire_ball(5,dir,i3,i4,48,'Fire Ball');
		  ident := true;
		end;
	  22 :	begin
		  fire_ball(2,dir,i3,i4,8,'Stinking Cloud');
		  ident := true;
		end;
	  23 :	begin
		  fire_ball(3,dir,i3,i4,40,'Acid Ball');
		  ident := true;
		end;
	  24 :	i1 := 2**(randint(24) - 1);
	  otherwise ;
	end;
	{ End of Wands...					}
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
		    end
	      end
	    else
	      msg_print('You are not carrying any wands.');
	  end
	else
	  msg_print('But you are not carrying anything.');
	if (redraw) then draw_cave;
      end;
