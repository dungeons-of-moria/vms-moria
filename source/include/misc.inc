	{ Use date and time to produce random seed		-RAK-	}
[psect(setup$code)] function get_seed : unsigned;
    type
	$quad = [quad,unsafe] record
		l0      : unsigned;
		l1      : unsigned;
	end;
    var
	time            : $quad;
	seed_val        : unsigned;

    [asynchronous,external (SYS$GETTIM)] function get_time(
		var time : $quad) : integer;
		external;

    begin
      get_time(time);                           { Current time  }
      seed_val := uor(time.l0,time.l1);         { Random number }
      get_seed := uor(seed_val,%X'00000001');   { Odd number    }
    end;


	{ Returns the day number; 1=Sunday...7=Saturday 	-RAK-	}
[psect(setup$code)] function day_num : integer;
    var
	i1              : integer;
    [external(LIB$DAY)] function day(
	var daynum              : integer;
	dum1                    : integer := %immed 0;
	dum2                    : integer := %immed 0) : integer;
	external;
    begin
      day(i1);
      day_num := ((i1+3) mod 7) + 1;
    end;


	{ Returns the hour number; 0=midnight...23=11 PM	-RAK-	}
[psect(setup$code)] function hour_num : integer;
    var
	hour            : integer;
	time_str        : packed array [1..11] of char;
    begin
      time(time_str);
      readv(substr(time_str,1,2),hour);
      hour_num := hour;
    end;


	{ Check the day-time strings to see if open		-RAK-	}
[psect(setup$code)] function check_time : boolean;
    begin
      case days[day_num,(hour_num+5)] of
	'.'  :  check_time := false;    { Closed                }
	'X'  :  check_time := true;     { Normal hours          }
	otherwise check_time := false;  { Other, assumed closed }
      end;
    end;


	{ Generates a random integer x where 1<=X<=MAXVAL	-RAK-	}
  function randint                      (
		%immed maxval           :       integer
					) : integer;
	external;

  function rand_rep			(
		%immed num              : integer;
		%immed die              : integer
					) : integer;
	external;


	{ Generates a random integer number of NORMAL distribution -RAK-}
[psect(misc1$code)] function randnor(mean,stand : integer) : integer;
    begin
      randnor :=  trunc(sqrt(-2.0*ln(randint(9999999)/10000000.0))*
		  cos(6.283*(randint(9999999)/10000000.0))*stand) + mean;
    end;


	{ Returns position of first set bit			-RAK-	}
  function bit_pos              (
		%ref  test      : unsigned
				) : integer;
	external;


	{ Checks a co-ordinate for in bounds status		-RAK-	}
[psect(misc1$code)] function in_bounds(y,x : integer) : boolean;
    begin
      if ((y > 1) and (y < cur_height) and
	  (x > 1) and (x < cur_width)) then
	in_bounds := true
      else
	in_bounds := false;
    end;


	{ Distance between two points				-RAK-	}
  function distance                     (
			%immed y1       : integer;
			%immed x1       : integer;
			%immed y2       : integer;
			%immed x2       : integer
					)       : integer;
	external;


	{ Checks points north, south, east, and west for a type -RAK-	}
[psect(misc1$code)] function next_to4   (
			y,x             :       integer;
			group_set       : obj_set
					) : integer;
    var
	i1      : integer;
    begin
      i1 := 0;
      if (y > 1) then
	if (cave[y-1,x].fval in group_set) then
	  i1 := i1 + 1;
      if (y < cur_height) then
	if (cave[y+1,x].fval in group_set) then
	  i1 := i1 + 1;
      if (x > 1) then
	if (cave[y,x-1].fval in group_set) then
	  i1 := i1 + 1;
      if (x < cur_width) then
	if (cave[y,x+1].fval in group_set) then
	  i1 := i1 + 1;
      next_to4 := i1
    end;


	{ Checks all adjacent spots for elements		-RAK-	}
[psect(misc1$code)] function next_to8   (
			y,x             :       integer;
			group_set       :       obj_set
					) : integer;
    var
	i1,i2,i3        : integer;
    begin
      i1 := 0;
      for i2 := (y - 1) to (y + 1) do
	for i3 := (x - 1) to (x + 1) do
	  if (in_bounds(i2,i3)) then
	    if (cave[i2,i3].fval in group_set) then
	      i1 := i1 + 1;
      next_to8 := i1
    end;


	{ Link all free space in treasure list together 		}
[psect(generate$code)] procedure tlink;
      var
	i1              : integer;
      begin
	for i1 := 1 to max_talloc do
	  begin
	    t_list[i1] := blank_treasure;
	    t_list[i1].p1 := i1 - 1;
	  end;
	tcptr := max_talloc;
      end;


	{ Link all free space in monster list together			}
[psect(generate$code)] procedure mlink;
      var
	i1              : integer;
      begin
	for i1 := 1 to max_malloc do
	  begin
	    m_list[i1] := blank_monster;
	    m_list[i1].nptr := i1 - 1;
	  end;
	m_list[2].nptr := 0;
	muptr := 0;
	mfptr := max_malloc;
      end;


	{ Initializes M_LEVEL array for use with PLACE_MONSTER	-RAK-	}
[psect(setup$code)] procedure init_m_level;
    var
	i1,i2,i3                : integer;
    begin
      i1 := 1;
      i2 := 0;
      i3 := max_creatures - win_mon_tot;
      repeat
	m_level[i2] := 0;
	while ((i1 <= i3) and (c_list[i1].level = i2)) do
	  begin
	    m_level[i2] := m_level[i2] + 1;
	    i1 := i1 + 1;
	  end;
	i2 := i2 + 1;
      until (i2 > max_mons_level);
      for i1 := 2 to max_mons_level do
	m_level[i1] := m_level[i1] + m_level[i1-1];
    end;


	{ Initializes T_LEVEL array for use with PLACE_OBJECT	-RAK-	}
[psect(setup$code)] procedure init_t_level;
    var
	i1,i2                   : integer;
    begin
      i1 := 1;
      i2 := 0;
      repeat
	while ((i1 <= max_objects) and (object_list[i1].level = i2)) do
	  begin
	    t_level[i2] := t_level[i2] + 1;
	    i1 := i1 + 1;
	  end;
	i2 := i2 + 1;
      until ((i2 > max_obj_level) or (i1 > max_objects));
      for i1 := 1 to max_obj_level do
	t_level[i1] := t_level[i1] + t_level[i1-1];
    end;


	{ Adjust prices of objects				-RAK-	}
  procedure price_adjust;
    var
	i1                      : integer;
    begin
      for i1 := 1 to max_objects do
	with object_list[i1] do
	  cost := trunc(cost*cost_adj + 0.99);
      for i1 := 1 to inven_init_max do
	with inventory_init[i1] do
	  cost := trunc(cost*cost_adj + 0.99);
    end;


	{ Converts input string into a dice roll		-RAK-	}
	{       Normal input string will look like '2d6', '3d8'... ect. }
[psect(misc1$code)] function damroll(dice : dtype) : integer;
    var
	i1,num,sides                    : integer;
    begin
      for i1 := 1 to length(dice) do
	if (dice[i1] = 'd') then
	  dice[i1] := ' ';
      num := 0;
      sides := 0;
      readv(dice,num,sides,error:=continue);
      damroll := rand_rep(num,sides);
    end;


	{ Returns true if no obstructions between two given points -RAK-}
[psect(misc1$code)] function los(y1,x1,y2,x2 : integer) : boolean;
    var
	ty,tx,stepy,stepx,p1,p2         : integer;
	slp,tmp                         : real;
	flag                            : boolean;
    begin
      ty := (y1 - y2);
      tx := (x1 - x2);
      flag := true;
      if ((ty <> 0) or (tx <> 0)) then
	begin
	  if (ty < 0) then
	    stepy := -1
	  else
	    stepy := 1;
	  if (tx < 0) then
	    stepx := -1
	  else
	    stepx := 1;
	  if (ty = 0) then
	    repeat
	      x2 := x2 + stepx;
	      flag := cave[y2,x2].fopen;
	    until((x1 = x2) or (not (flag)))
	  else if (tx = 0) then
	    repeat
	      y2 := y2 + stepy;
	      flag := cave[y2,x2].fopen;
	    until((y1 = y2) or (not (flag)))
	  else if (abs(ty) > abs(tx)) then
	    begin
	      slp := abs(tx/ty)*stepx;
	      tmp := x2;
	      repeat
		y2 := y2 + stepy;
		tmp := tmp + slp;
		p1 := round(tmp - 0.1);
		p2 := round(tmp + 0.1);
		if (not ((cave[y2,p1].fopen) or (cave[y2,p2].fopen))) then
		  flag := false;
	      until((y1 = y2) or (not (flag)))
	    end
	  else
	    begin
	      slp := abs(ty/tx)*stepy;
	      tmp := y2;
	      repeat
		x2 := x2 + stepx;
		tmp := tmp + slp;
		p1 := round(tmp - 0.1);
		p2 := round(tmp + 0.1);
		if (not ((cave[p1,x2].fopen) or (cave[p2,x2].fopen))) then
		  flag := false;
	      until((x1 = x2) or (not (flag)))
	    end;
	end;
      los := flag;
    end;


	{ Returns symbol for given row, column			-RAK-	}
[psect(misc5$code)] procedure loc_symbol(y,x : integer; var sym : char);
    begin
      with cave[y,x] do
	if ((cptr = 1) and (not(find_flag))) then
	  sym := '@'
	else if (py.flags.blind > 0) then
	  sym := ' '
	else
	  begin
	    if (cptr > 1) then
	      begin
		with m_list[cptr] do
		  if ((ml) and
		      ((uand(c_list[mptr].cmove,%X'00010000') = 0) or
		       (py.flags.see_inv))) then
		    sym := c_list[mptr].cchar
		  else if (tptr > 0) then
		    sym := t_list[tptr].tchar
		  else if (fval < 10) then
		    sym := '.'
		  else
		    sym := '#';
	      end
	    else if (tptr > 0) then
	      sym := t_list[tptr].tchar
	    else if (fval < 10) then
	      sym := '.'
	    else
	      sym := '#';
	  end;
    end;


	{ Tests a spot for light or field mark status		-RAK-	}
[psect(misc1$code)] function test_light(y,x : integer) : boolean;
    begin
      with cave[y,x] do
	if ((pl) or (fm) or (tl)) then
	  test_light := true
	else
	  test_light := false;
    end;


	{ Prints the map of the dungeon 			-RAK-	}
[psect(misc2$code)] procedure prt_map;
    var
	i1,i2,i3,i4,i5  : integer;
	ypos,xpos,isp   : integer;
	floor_str       : vtype;
	tmp_char        : char;
	flag            : boolean;
    begin
      redraw := false;                  { Screen has been redrawn       }
      i3 := 1;                          { Used for erasing dirty lines  }
      i4 := 14;                         { Erasure starts in this column }
      for i1 := panel_row_min to panel_row_max do       { Top to bottom }
	begin
	  i3 := i3 + 1;                 { Increment dirty line ctr      }
	  if (used_line[i3]) then       { If line is dirty...           }
	    begin
	      erase_line(i3,i4);        { erase it.                     }
	      used_line[i3] := false;   { Now it's a clean line         }
	    end;
	  floor_str := '';              { Floor_str is string to be printed}
	  ypos := i1;                   { Save row                      }
	  flag := false;                { False until floor_str <> ''   }
	  isp := 0;                     { Number of blanks encountered  }
	  for i2 := panel_col_min to panel_col_max do   { Left to right }
	    with cave[i1,i2] do
	      begin                     { Get character for location    }
		if (test_light(i1,i2)) then
		  loc_symbol(i1,i2,tmp_char)
		else if ((cptr = 1) and (not(find_flag))) then
		  tmp_char := '@'
		else if (cptr > 1) then
		  if (m_list[cptr].ml) then
		    loc_symbol(i1,i2,tmp_char)
		  else
		    tmp_char := ' '
		else
		  tmp_char := ' ';
		if (tmp_char = ' ') then{ If blank...                   }
		  begin
		    if (flag) then      { If floor_str <> '' then       }
		      begin
			isp := isp + 1; { Increment blank ctr           }
			if (isp > 3) then       { Too many blanks, print}
			  begin                 { floor_str and reset   }
			    print(floor_str,ypos,xpos);
			    flag := false;
			    isp := 0;
			  end;
		      end
		  end
		else
		  begin
		    if (flag) then      { Floor_str <> ''               }
		      begin
			if (isp > 0) then       { Add on the blanks     }
			  begin
			    for i5 := 1 to isp do
			      floor_str := floor_str + ' ';
			    isp := 0;
			  end;                  { Add on the character  }
			floor_str := floor_str + tmp_char;
		      end
		    else
		      begin             { Floor_str = ''                }
			xpos := i2;     { Save column for printing      }
			flag := true;   { Set flag to true              }
			floor_str := tmp_char;  { Floor_str <> ''       }
		      end;
		  end;
	      end;
	  if (flag) then                { Print remainder, if any       }
	    print(floor_str,ypos,xpos);
	end;
    end;


	{ Compact monsters					-RAK-	}
[psect(misc2$code)] procedure compact_monsters;
    var
	i1,i2,i3,ctr,cur_dis            : integer;
	delete_1,delete_any             : boolean;
    begin
      cur_dis := 66;
      delete_any := false;
      repeat
	i1 := muptr;
	i2 := 0;
	repeat
	  delete_1 := false;
	  i3 := m_list[i1].nptr;
	  with m_list[i1] do
	    if (cur_dis > cdis) then
	      if (randint(3) = 1) then
		begin
		  if (i2 = 0) then
		    muptr := i3
		  else
		    m_list[i2].nptr := i3;
		  cave[fy,fx].cptr := 0;
		  m_list[i1] := blank_monster;
		  m_list[i1].nptr := mfptr;
		  mfptr := i1;
		  ctr := ctr + 1;
		  delete_1 := true;
		  delete_any := true;
		end;
	  if (not(delete_1)) then i2 := i1;
	  i1 := i3;
	until (i1 = 0);
	if (not(delete_any)) then cur_dis := cur_dis - 6;
      until (delete_any);
      if (cur_dis < 66) then prt_map;
    end;


	{ Returns a pointer to next free space			-RAK-	}
[psect(misc3$code)] procedure popm(var x : integer);
    begin
      if (mfptr < 1) then compact_monsters;
      x := mfptr;
      mfptr := m_list[x].nptr;
    end;


	{ Pushs a record back onto free space list		-RAK-	}
[psect(misc3$code)] procedure pushm(x : integer);
    begin
      m_list[x] := blank_monster;
      m_list[x].nptr := mfptr;
      mfptr := x;
    end;


	{ Gives Max hit points					-RAK-	}
[psect(misc3$code)] function max_hp(hp_str : dtype) : integer;
    var
	i1,num,die              : integer;
    begin
      for i1 := 1 to length(hp_str) do
	if (hp_str[i1] = 'd') then
	  hp_str[i1] := ' ';
      readv(hp_str,num,die);
      max_hp := num*die;
    end;


	{ Places a monster at given location			-RAK-	}
[psect(misc3$code)] procedure place_monster(y,x,z : integer; slp : boolean);
    var
	i1,cur_pos              : integer;
    begin
      popm(cur_pos);
      with m_list[cur_pos] do
	begin
	  fy := y;
	  fx := x;
	  mptr := z;
	  nptr := muptr;
	  muptr := cur_pos;
	  if (uand(c_list[z].cdefense,%X'4000') <> 0) then
	    hp := max_hp(c_list[z].hd)
	  else
	    hp := damroll(c_list[z].hd);
	  cspeed := c_list[z].speed + py.flags.speed;
	  stuned := 0;
	  cdis := distance(char_row,char_col,y,x);
	  cave[y,x].cptr := cur_pos;
	  if (slp) then
	    begin
	      csleep := trunc(c_list[z].sleep/5.0) + randint(c_list[z].sleep);
	    end
	  else
	    csleep := 0;
	end;
    end;


	{ Places a monster at given location			-RAK-	}
[psect(misc3$code)] procedure place_win_monster;
    var
	cur_pos                 : integer;
	y,x                     : integer;
    begin
      if (not(total_winner)) then
	begin
	  popm(cur_pos);
	  with m_list[cur_pos] do
	    begin
	      repeat
		y := randint(cur_height-2)+1;
		x := randint(cur_width-2)+1;
	      until ((cave[y,x].fval in [1,2,4])        and
		     (cave[y,x].cptr = 0)               and
		     (cave[y,x].tptr = 0)               and
		     (distance(y,x,char_row,char_col) > max_sight));
	      fy := y;
	      fx := x;
	      mptr := randint(win_mon_tot) +
				m_level[max_mons_level] + m_level[0];
	      nptr := muptr;
	      muptr := cur_pos;
	      if (uand(c_list[mptr].cdefense,%X'4000') <> 0) then
		hp := max_hp(c_list[mptr].hd)
	      else
		hp := damroll(c_list[mptr].hd);
	      cspeed := c_list[mptr].speed + py.flags.speed;
	      stuned := 0;
	      cdis := distance(char_row,char_col,y,x);
	      cave[y,x].cptr := cur_pos;
	      csleep := 0;
	    end;
	end;
    end;


	{ Allocates a random monster				-RAK-	}
[psect(misc3$code)] procedure alloc_monster(alloc_set : obj_set;
			  num,dis : integer;
			  slp : boolean         );
    var
	y,x,a,b,i1,i2,i3        : integer;
    begin
      for i1 := 1 to num do
	begin
	  repeat
	    y := randint(cur_height-2)+1;
	    x := randint(cur_width-2)+1;
	  until ((cave[y,x].fval in alloc_set)  and
		 (cave[y,x].cptr = 0)           and
		 (cave[y,x].fopen)              and
		 (distance(y,x,char_row,char_col) > dis));
	  if (dun_level = 0) then
	    i2 := randint(m_level[0])
	  else if (dun_level > max_mons_level) then
	    i2 := randint(m_level[max_mons_level]) + m_level[0]
	  else if (randint(mon_nasty) = 1) then
	    begin
	      i2 := dun_level + abs(randnor(0,4)) + 1;
	      if (i2 > max_mons_level) then i2 := max_mons_level;
	      i3 := m_level[i2] - m_level[i2-1];
	      i2 := randint(i3) + m_level[i2-1];
	    end
	  else
	    i2 := randint(m_level[dun_level]) + m_level[0];
	  place_monster(y,x,i2,slp);
	end
    end;


	{ Places creature adjacent to given location		-RAK-	}
[psect(misc3$code)] function summon_monster(
				var y,x :       integer;
				slp     :       boolean
					) : boolean;
    var
	i1,i2,i3,i4,i5          : integer;
    begin
      i1 := 0;
      i5 := dun_level + mon$summon_adj;
      summon_monster := false;
      if (i5 > max_mons_level) then
	i4 := max_mons_level
      else
	i4 := i5;
      if (dun_level = 0) then
	i4 := randint(m_level[0])
      else
	i4 := randint(m_level[i4]) + m_level[0];
      repeat
	i2 := y - 2 + randint(3);
	i3 := x - 2 + randint(3);
	if (in_bounds(i2,i3)) then
	  with cave[i2,i3] do
	    if (fval in [1,2,4,5]) then
	      if (cptr = 0) then
		if (fopen) then
		  begin
		    place_monster(i2,i3,i4,slp);
		    summon_monster := true;
		    i1 := 9;
		    y := i2;
		    x := i3;
		  end;
	i1 := i1 + 1;
      until (i1 > 9);
    end;


	{ Places undead adjacent to given location		-RAK-	}
[psect(misc3$code)] function summon_undead(var y,x : integer) : boolean;
    var
	i1,i2,i3,i4,i5,ctr      : integer;
    begin
      i1 := 0;
      summon_undead := false;
      i4 := m_level[max_mons_level] + m_level[0];
      repeat
	i5 := randint(i4);
	ctr := 0;
	repeat
	  if (uand(c_list[i5].cdefense,%X'0008') <> 0) then
	    begin
	      ctr := 20;
	      i4  := 0;
	    end
	  else
	    begin
	      i5 := i5 + 1;
	      if (i5 > i4) then
		ctr := 20
	      else
		ctr := ctr + 1;
	    end;
	until(ctr > 19)
      until(i4 = 0);
      repeat
	i2 := y - 2 + randint(3);
	i3 := x - 2 + randint(3);
	if (in_bounds(i2,i3)) then
	  with cave[i2,i3] do
	    if (fval in [1,2,4,5]) then
	      if ((cptr = 0) and (fopen)) then
		begin
		  place_monster(i2,i3,i5,false);
		  summon_undead := true;
		  i1 := 9;
		  y := i2;
		  x := i3;
		end;
	i1 := i1 + 1;
      until (i1 > 9);
    end;


	{ If too many objects on floor level, delete some of them-RAK-	}
[psect(misc2$code)] procedure compact_objects;
    var
	i1,i2,ctr,cur_dis               : integer;
	flag                            : boolean;
    begin
      ctr := 0;
      cur_dis := 66;
      repeat
	for i1 := 1 to cur_height do
	  for i2 := 1 to cur_width do
	    with cave[i1,i2] do
	      if (tptr > 0) then
		if (distance(i1,i2,char_row,char_col) > cur_dis) then
		  begin
		    flag := false;
		    with t_list[tptr] do
		      case tval of
			102     : if (subval in [1,6,9]) then
				    flag := true
				  else if (randint(4) = 1) then
				    flag := true;
			103     : flag := true;
			104,105 : if (randint(4) = 1) then flag := true;
			107,108 : ;
			otherwise if (randint(8) = 1) then flag := true;
		      end;
		    if (flag) then
		      begin
			fopen := true;
			t_list[tptr] := blank_treasure;
			t_list[tptr].p1 := tcptr;
			tcptr := tptr;
			tptr := 0;
			ctr := ctr + 1;
		      end;
		  end;
	  if (ctr = 0) then cur_dis := cur_dis - 6;
	until (ctr > 0);
	if (cur_dis < 66) then prt_map;
    end;


	{ Gives pointer to next free space			-RAK-	}
[psect(misc4$code)] procedure popt(var x : integer);
    var
	i1                      : integer;
    begin
      if (tcptr < 1) then compact_objects;
      x := tcptr;
      tcptr := t_list[x].p1;
    end;


	{ Pushs a record back onto free space list		-RAK-	}
[psect(misc4$code)] procedure pusht(x : integer);
    begin
      t_list[x] := blank_treasure;
      t_list[x].p1 := tcptr;
      tcptr := x;
    end;


	{ Order the treasure list by level			-RAK-	}
[psect(setup$code)] procedure sort_objects;
    var
	i1,i2,i3,gap            : integer;
	tmp                     : treasure_type;
    begin
      gap := max_objects div 2;
      while (gap > 0) do
	begin
	  for i1 := gap+1 to max_objects do
	    begin
	      i2 := i1 - gap;
	      while (i2 > 0) do
		begin
		  i3 := i2 + gap;
		  if (object_list[i2].level > object_list[i3].level) then
		    begin
		      tmp := object_list[i2];
		      object_list[i2] := object_list[i3];
		      object_list[i3] := tmp;
		    end
		  else
		    i2 := 0;
		  i2 := i2 - gap;
		end;
	    end;
	  gap := gap div 2;
	end;
    end;



	{ Chance of treasure having magic abilities		-RAK-	}
	{ Chance increases with each dungeon level                      }
[psect(misc4$code)] procedure magic_treasure(x,level : integer);
    var
	chance,special,cursed,i1        : integer;

	{ Boolean : is object enchanted 	  -RAK- }
    function magik(chance : integer) : boolean;
      begin
	if (randint(100) <= chance) then
	  magik := true
	else
	  magik := false;
      end;

	{ Enchant a bonus based on degree desired -RAK- }
    function m_bonus(base,max_std,level : integer) : integer;
      var
	x,stand_dev             : integer;
      begin
	stand_dev := trunc(obj_std_adj*level) + obj_std_min;
	if (stand_dev > max_std) then stand_dev := max_std;
	x := trunc(abs(randnor(0,stand_dev))/10.0) + base;
	if (x < base) then
	  m_bonus := base
	else
	  m_bonus := x;
      end;

    begin
      chance := obj_base_magic + level;
      if (chance > obj_base_max) then chance := obj_base_max;
      special := trunc(chance/obj_div_special);
      cursed  := trunc(chance/obj_div_cursed);
      with t_list[x] do
	{ Depending on treasure type, it can have certain magical properties}
	case tval of
	34,35,36: begin { Armor and shields}
		    if (magik(chance)) then
		      begin
			toac := m_bonus(1,30,level);
			if (magik(special)) then
			  case randint(9) of
			1     : begin
				  flags := uor(flags,%X'02380000');
				  name := name + ' (R)';
				  toac := toac + 5;
				  cost := cost + 2500;
				end;
			2     : begin   { Resist Acid   }
				  flags := uor(flags,%X'00100000');
				  name := name + ' (RA)';
				  cost := cost + 1000;
				end;
			3,4   : begin   { Resist Fire   }
				  flags := uor(flags,%X'00080000');
				  name := name + ' (RF)';
				  cost := cost + 600;
				end;
			5,6   : begin   { Resist Cold   }
				  flags := uor(flags,%X'00200000');
				  name := name + ' (RC)';
				  cost := cost + 600;
				end;
			7,8,9 : begin   { Resist Lightning}
				  flags := uor(flags,%X'02000000');
				  name := name + ' (RL)';
				  cost := cost + 500;
				end;
			  end
		      end
		    else if (magik(cursed)) then
		      begin
			toac := -m_bonus(1,40,level);
			cost := 0;
			flags := uor(%X'80000000',flags);
		      end
		  end;
	21,22,23: begin { Weapons       }
		    if (magik(chance)) then
		      begin
			tohit := m_bonus(0,40,level);
			todam := m_bonus(0,40,level);
			if (magik(special)) then
			  case randint(16) of
		1             : begin   { Holy Avenger  }
				  flags := uor(flags,%X'01418001');
				  tohit := tohit + 5;
				  todam := todam + 5;
				  toac  := randint(4);
				  p1    := randint(4) - 1;
				  name  := name + ' (HA)';
				  cost  := cost + p1*500;
				  cost  := cost + 10000;
				end;
		2             : begin   { Defender      }
				  flags := uor(flags,%X'07B80900');
				  tohit := tohit + 3;
				  todam := todam + 3;
				  toac  := 5 + randint(5);
				  name  := name + ' [%P4] (DF)';
				  p1    := randint(3);
				  cost  := cost + p1*500;
				  cost  := cost + 7500;
				end;
		3,4           : begin   { Slay Monster  }
				  flags := uor(flags,%X'01004000');
				  tohit := tohit + 3;
				  todam := todam + 3;
				  name := name + ' (SM)';
				  cost := cost + 5000;
				end;
		5,6           : begin   { Slay Dragon   }
				  flags := uor(flags,%X'00002000');
				  tohit := tohit + 3;
				  todam := todam + 3;
				  name := name + ' (SD)';
				  cost := cost + 4000;
				end;
		7,8           : begin   { Slay Evil     }
				  flags := uor(flags,%X'00008000');
				  tohit := tohit + 3;
				  todam := todam + 3;
				  name := name + ' (SE)';
				  cost := cost + 4000;
				end;
		9,10          : begin   { Slay Undead   }
				  flags := uor(flags,%X'00010000');
				  tohit := tohit + 2;
				  todam := todam + 2;
				  name := name + ' (SU)';
				  cost := cost + 3000;
				end;
		11,12,13      : begin   { Flame Tongue  }
				  flags := uor(flags,%X'00040000');
				  tohit := tohit + 1;
				  todam := todam + 3;
				  name := name + ' (FT)';
				  cost := cost + 2000;
				end;
		14,15,16      : begin   { Frost Brand   }
				  flags := uor(flags,%X'00020000');
				  tohit := tohit + 1;
				  todam := todam + 1;
				  name := name + ' (FB)';
				  cost := cost + 1200;
				end;
			  end;
		      end
		    else if (magik(cursed)) then
		      begin
			tohit := -m_bonus(1,55,level);
			todam := -m_bonus(1,55,level);
			flags := uor(%X'80000000',flags);
			cost := 0;
		      end
		  end;
	     20 : begin { Bows, crossbows, and slings   }
		    if (magik(chance)) then
		      tohit := m_bonus(1,30,level)
		    else if (magik(cursed)) then
		      begin
			tohit := -m_bonus(1,50,level);
			flags := uor(%X'80000000',flags);
			cost := 0;
		      end;
		  end;
	     25 : begin { Digging tools         }
		    if (magik(chance)) then
		      case randint(3) of
			1,2:begin
			      p1 := m_bonus(2,25,level);
			      cost := cost + p1*100;
			    end;
			3 : begin
			      p1 := -m_bonus(1,30,level);
			      cost := 0;
			      flags := uor(%X'80000000',flags);
			    end;
		      end;
		  end;
	     31 : begin { Gloves and Gauntlets  }
		    if (magik(chance)) then
		      begin
			toac := m_bonus(1,20,level);
			if (magik(special)) then
			  case randint(2) of
			    1  :begin
				  flags := uor(%X'00800000',flags);
				  name := name + ' of Free Action';
				  cost := cost + 1000;
				end;
			    2  :begin
				  tohit := 1 + randint(3);
				  todam := 1 + randint(3);
				  name := name + ' of Slaying';
				  cost := cost + (tohit+todam)*250;
				end;
			  end;
		      end
		    else if (magik(cursed)) then
		      begin
			if (magik(special)) then
			  case randint(2) of
			    1 : begin
				  flags := uor(%X'80000002',flags);
				  name := name + ' of Clumsiness';
				  p1 := 1;
				end;
			    2 : begin
				  flags := uor(%X'80000001',flags);
				  name := name + ' of Weakness';
				  p1 := 1;
				end;
			  end;
			toac := -m_bonus(1,40,level);
			p1   := -m_bonus(1,10,level);
			flags := uor(%X'80000000',flags);
			cost := 0;
		      end
		  end;
	  30    : begin { Boots }
		    if (magik(chance)) then
		      begin
			toac := m_bonus(1,20,level);
			if (magik(special)) then
			  case randint(12) of
			    1 : begin
				  flags := uor(%X'00001000',flags);
				  name := name + ' of Speed';
				  p1 := 1;
				  cost := cost + 5000;
				end;
		      2,3,4,5 : begin
				  flags := uor(%X'00000100',flags);
				  name := name + ' of Stealth';
				  cost := cost + 500;
				end;
		      otherwise begin
				  flags := uor(%X'04000000',flags);
				  name := name + ' of Slow descent';
				  cost := cost + 250;
				end
			    end
		      end
		    else if (magik(cursed)) then
		      begin
			case randint(3) of
			  1 : begin
				flags := uor(%X'80001000',flags);
				name := name + ' of Slowness';
				p1 := -1;
			      end;
			  2 : begin
				flags := uor(%X'80000200',flags);
				name := name + ' of Noise';
			      end;
			  3 : begin
				flags := uor(%X'80000000',flags);
				name := name + ' of Great Mass';
				weight := weight*5;
			      end;
			end;
			cost := 0;
			ac := -m_bonus(2,45,level);
		      end;
		  end;
	  33    : begin { Helms }
		    if (magik(chance)) then
		      begin
			toac := m_bonus(1,20,level);
			if (magik(special)) then
			  case subval of
		1,2,3,4,5  :  case randint(3) of
				1 : begin
				      p1 := randint(2);
				      flags := uor(%X'00000008',flags);
				      name := name + ' of Intelligence';
				      cost := cost + p1*500;
				    end;
				2 : begin
				      p1 := randint(2);
				      flags := uor(%X'00000010',flags);
				      name := name + ' of Wisdom';
				      cost := cost + p1*500;
				    end;
				3 : begin
				      p1 := 1 + randint(4);
				      flags := uor(%X'40000000',flags);
				      name := name + ' of Infra-Vision';
				      cost := cost + p1*250;
				    end;
			      end;
		6,7,8      :  case randint(6) of
				1 : begin
				      p1 := randint(3);
				      flags := uor(%X'00800007',flags);
				      name := name + ' of Might';
				      cost := cost + 1000 + p1*500;
				    end;
				2 : begin
				      p1 := randint(3);
				      flags := uor(%X'00000030',flags);
				      name := name + ' of Lordliness';
				      cost := cost + 1000 + p1*500;
				    end;
				3 : begin
				      p1 := randint(3);
				      flags := uor(%X'01380008',flags);
				      name := name + ' of the Magi';
				      cost := cost + 3000 + p1*500;
				    end;
				4 : begin
				      p1 := randint(3);
				      flags := uor(%X'00000020',flags);
				      name := name + ' of Beauty';
				      cost := cost + 750;
				    end;
				5 : begin
				      p1 := 1 + randint(4);
				      flags := uor(%X'01000040',flags);
				      name := name + ' of Seeing';
				      cost := cost + 1000 + p1*100;
				    end;
				6 : begin
				      flags := uor(%X'00000800',flags);
				      name := name + ' of Regeneration';
				      cost := cost + 1500;
				    end;
			      end;
			  end;
		      end
		    else if (magik(cursed)) then
		      begin
			toac := -m_bonus(1,45,level);
			flags := uor(%X'80000000',flags);
			cost := 0;
			if (magik(special)) then
			  case randint(7) of
			    1 : begin
				  p1 := -1;
				  flags := uor(%X'00000008',flags);
				  name := name + ' of Stupidity';
				end;
			    2 : begin
				  p1 := -1;
				  flags := uor(%X'00000010',flags);
				  name := name + ' of Dullness';
				end;
			    3 : begin
				  flags := uor(%X'08000000',flags);
				  name := name + ' of Blindness';
				end;
			    4 : begin
				  flags := uor(%X'10000000',flags);
				  name := name + ' of Timidness';
				end;
			    5 : begin
				  p1 := -1;
				  flags := uor(%X'00000001',flags);
				  name := name + ' of Weakness';
				end;
			    6 : begin
				  flags := uor(%X'00000400',flags);
				  name := name + ' of Teleportation';
				end;
			    7 : begin
				  p1 := -1;
				  flags := uor(%X'00000020',flags);
				  name := name + ' of Ugliness';
				end;
			  end;
			p1 := p1*randint(5);
		      end;
		  end;
	  45    : begin { Rings         }
		    case subval of
		1,2,3,4,5,6 :   if (magik(cursed)) then
				  begin
				    p1 := -m_bonus(1,20,level);
				    flags := uor(%X'80000000',flags);
				    cost := -cost;
				  end
				else
				  begin
				    p1 := m_bonus(1,10,level);
				    cost := cost + p1*100;
				  end;
			  7 :   if (magik(cursed)) then
				  begin
				    p1 := -randint(3);
				    flags := uor(%X'80000000',flags);
				    cost := -cost;
				  end
				else
				  p1 := 1;
			  8  :  begin
				  p1 := 5*m_bonus(1,20,level);
				  cost := cost + p1*100;
				end;
			  22 :  begin   { Increase damage       }
				  todam := m_bonus(1,20,level);
				  cost := cost + todam*100;
				  if (magik(cursed)) then
				    begin
				      todam := -todam;
				      flags := uor(%X'80000000',flags);
				      cost := -cost;
				    end
				end;
			  23 :  begin   { Increase To-Hit       }
				  tohit := m_bonus(1,20,level);
				  cost := cost + todam*100;
				  if (magik(cursed)) then
				    begin
				      tohit := -tohit;
				      flags := uor(%X'80000000',flags);
				      cost := -cost;
				    end
				end;
			  24 :  begin   { Protection            }
				  toac := m_bonus(1,20,level);
				  cost := cost + todam*100;
				  if (magik(cursed)) then
				    begin
				      toac := -toac;
				      flags := uor(%X'80000000',flags);
				      cost := -cost;
				    end
				end;
			  33 :  begin   { Slaying       }
				  todam := m_bonus(1,25,level);
				  tohit := m_bonus(1,25,level);
				  cost := cost + (tohit+todam)*100;
				  if (magik(cursed)) then
				    begin
				      tohit := -tohit;
				      todam := -todam;
				      flags := uor(%X'80000000',flags);
				      cost := -cost;
				    end
				end;
			    otherwise ;
			  end;
		  end;
	  40    : begin { Amulets       }
		    case subval of
		1,2,3,4,5,6 :   if (magik(cursed)) then
				  begin
				    p1 := -m_bonus(1,20,level);
				    flags := uor(%X'80000000',flags);
				    cost := -cost;
				  end
				else
				  begin
				    p1 := m_bonus(1,10,level);
				    cost := cost + p1*100;
				  end;
			  7  :  begin
				  p1 := 5*m_bonus(1,25,level);
				  if (magik(cursed)) then
				    begin
				      p1 := -p1;
				      cost := -cost;
				      flags := uor(%X'80000000',flags);
				    end
				  else
				    cost := cost + 100*p1;
				end;
			    otherwise ;
			  end;
		  end;
			{ Subval should be even for store, odd for dungeon}
			{ Dungeon found ones will be partially charged    }
	  15    : begin { Lamps and torches}
		    if ((subval mod 2) = 1) then
		      p1 := randint(p1);
		  end;
	  65    : begin { Wands         }
		    case subval of
			1   :   p1 := randint(10) + 6;
			2   :   p1 := randint(8)  + 6;
			3   :   p1 := randint(5)  + 6;
			4   :   p1 := randint(8)  + 6;
			5   :   p1 := randint(4)  + 3;
			6   :   p1 := randint(8)  + 6;
			7   :   p1 := randint(20) + 12;
			8   :   p1 := randint(20) + 12;
			9   :   p1 := randint(10) + 6;
			10  :   p1 := randint(12) + 6;
			11  :   p1 := randint(10) + 12;
			12  :   p1 := randint(3)  + 3;
			13  :   p1 := randint(8)  + 6;
			14  :   p1 := randint(10) + 6;
			15  :   p1 := randint(5)  + 3;
			16  :   p1 := randint(5)  + 3;
			17  :   p1 := randint(5)  + 6;
			18  :   p1 := randint(5)  + 4;
			19  :   p1 := randint(8)  + 4;
			20  :   p1 := randint(6)  + 2;
			21  :   p1 := randint(4)  + 2;
			22  :   p1 := randint(8)  + 6;
			23  :   p1 := randint(5)  + 2;
			24  :   p1 := randint(12) + 12;
			otherwise ;
		    end
		  end;
	  55    : begin { Staffs        }
		    case subval of
			1   :   p1 := randint(20) + 12;
			2   :   p1 := randint(8)  + 6;
			3   :   p1 := randint(5)  + 6;
			4   :   p1 := randint(20) + 12;
			5   :   p1 := randint(15) + 6;
			6   :   p1 := randint(4)  + 5;
			7   :   p1 := randint(5)  + 3;
			8   :   p1 := randint(3)  + 1;
			9   :   p1 := randint(3)  + 1;
			10  :   p1 := randint(3)  + 1;
			11  :   p1 := randint(5)  + 6;
			12  :   p1 := randint(10) + 12;
			13  :   p1 := randint(5)  + 6;
			14  :   p1 := randint(5)  + 6;
			15  :   p1 := randint(5)  + 6;
			16  :   p1 := randint(10) + 12;
			17  :   p1 := randint(3)  + 4;
			18  :   p1 := randint(5)  + 6;
			19  :   p1 := randint(5)  + 6;
			20  :   p1 := randint(3)  + 4;
			21  :   p1 := randint(10) + 12;
			22  :   p1 := randint(3)  + 4;
			23  :   p1 := randint(3)  + 4;
			24  :   p1 := randint(3)  + 1;
			25  :   p1 := randint(10) + 6;
			otherwise ;
		    end
		  end;
	  32    : begin { Cloaks        }
		    if (magik(chance)) then
		      begin
			if (magik(special)) then
			  case randint(2) of
		1             : begin
				  name := name + ' of Protection';
				  toac := m_bonus(2,40,level);
				  cost := cost + 250 + toac*100;
				end;
		2             : begin
				  toac := m_bonus(1,20,level);
				  p1 := randint(3);
				  flags := uor(%X'00000100',flags);
				  name := name + ' of Stealth (%P1)';
				  cost := cost + p1*500 + toac*100;
				end;
			  end
			else
			  begin
			    toac := m_bonus(1,20,level);
			    cost := cost + toac+100;
			  end;
		      end
		    else if (magik(cursed)) then
		      case randint(3) of
			1 : begin
			      flags := uor(%X'80000200',flags);
			      name := name + ' of Irritation';
			      ac   :=  0;
			      toac  := -m_bonus(1,10,level);
			      tohit := -m_bonus(1,10,level);
			      todam := -m_bonus(1,10,level);
			      cost :=  0;
			    end;
			2 : begin
			      flags := uor(%X'80000000',flags);
			      name := name + ' of Vulnerability';
			      ac   := 0;
			      toac := -m_bonus(10,100,level+50);
			      cost := 0;
			    end;
			3 : begin
			      flags := uor(%X'80000000',flags);
			      name := name + ' of Enveloping';
			      toac  := -m_bonus(1,10,level);
			      tohit := -m_bonus(2,40,level+10);
			      todam := -m_bonus(2,40,level+10);
			      cost := 0;
			    end;
		      end;
		  end;
	  2     : begin { Chests        }
		    case (randint(level)+4) of
		      1         : begin
				    name := name + '^ (Empty)';
				  end;
		      2         : begin
				    flags := uor(%X'00000001',flags);
				    name := name + '^ (Locked)';
				  end;
		      3,4       : begin
				    flags := uor(%X'00000011',flags);
				    name := name + '^ (Poison Needle)';
				  end;
		      5,6       : begin
				    flags := uor(%X'00000021',flags);
				    name := name + '^ (Poison Needle)';
				  end;
		      7,8,9     : begin
				    flags := uor(%X'00000041',flags);
				    name := name + '^ (Gas Trap)';
				  end;
		      10,11     : begin
				    flags := uor(%X'00000081',flags);
				    name := name + '^ (Explosion Device)';
				  end;
		      12,13,14  : begin
				    flags := uor(%X'00000101',flags);
				    name := name + '^ (Summoning Runes)';
				  end;
		      15,16,17  : begin
				    flags := uor(%X'00000071',flags);
				    name := name + '^ (Multiple Traps)';
				  end;
		      otherwise   begin
				    flags := uor(%X'00000181',flags);
				    name := name + '^ (Multiple Traps)';
				  end;
		    end;
		  end;
	10,11,12,13: begin      { Arrows, bolts, ammo, and spikes       }
		  if (tval in [11,12]) then
		    if (magik(chance)) then
		      begin
			tohit := m_bonus(1,35,level);
			todam := m_bonus(1,35,level);
			if (magik(special)) then
			  case tval of  {CASE 1}
			 11,12 :  case randint(10) of   {CASE 2}
			   1,2,3 :begin
				    name := name + ' of Slaying';
				    tohit := tohit + 5;
				    todam := todam + 5;
				    cost := cost + 20;
				  end;
			   4,5   :begin
				    flags := uor(flags,%X'00040000');
				    tohit := tohit + 2;
				    todam := todam + 4;
				    name := name + ' of Fire';
				    cost := cost + 25;
				  end;
			   6,7   :begin
				    flags := uor(flags,%X'00008000');
				    tohit := tohit + 3;
				    todam := todam + 3;
				    name := name + ' of Slay Evil';
				    cost := cost + 25;
				  end;
			   8,9   :begin
				    flags := uor(flags,%X'01004000');
				    tohit := tohit + 2;
				    todam := todam + 2;
				    name := name + ' of Slay Monster';
				    cost := cost + 30;
				  end;
			   10    :begin
				    flags := uor(flags,%X'00002000');
				    tohit := tohit + 10;
				    todam := todam + 10;
				    name := name + ' of Dragon Slaying';
				    cost := cost + 35;
				  end;
				  end; {CASE 2}
		      otherwise ;
			  end;  {CASE 1}
		      end
		    else if (magik(cursed)) then
		      begin
			tohit := -m_bonus(5,55,level);
			todam := -m_bonus(5,55,level);
			flags := uor(%X'80000000',flags);
			cost := 0;
		      end;
		    number := 0;
		    for i1 := 1 to 7 do number := number + randint(6);
		    missle_ctr := missle_ctr + 1;
		    if (missle_ctr > 65534) then
		      missle_ctr := 1;
		    subval := missle_ctr + 512;
		  end;
	    otherwise ;
	  end
      end;


	{ Places a particular trap at location y,x		-RAK-	}
[psect(misc4$code)] procedure place_trap(y,x,typ,subval : integer);
    var
	cur_pos                 : integer;
	cur_trap                : treasure_type;
    begin
      if (typ = 1) then
	cur_trap := trap_lista[subval]
      else
	cur_trap := trap_listb[subval];
      popt(cur_pos);
      cave[y,x].tptr  := cur_pos;
      t_list[cur_pos] := cur_trap;
    end;


	{ Places rubble at location y,x 			-RAK-	}
[psect(misc4$code)] procedure place_rubble(y,x : integer);
    var
	cur_pos                 : integer;
    begin
      popt(cur_pos);
      with cave[y,x] do
	begin
	  tptr := cur_pos;
	  fopen := false;
	end;
      t_list[cur_pos] := rubble;
    end;


[psect(misc4$code)] procedure place_open_door(y,x : integer);
    var
	cur_pos                 : integer;
  begin
    popt(cur_pos);
    with cave[y,x] do
      begin
	tptr := cur_pos;
	t_list[cur_pos] := door_list[1];
	fval  := corr_floor3.ftval;
	fopen := true;
      end;
  end;


[psect(misc4$code)] procedure place_broken_door(y,x : integer);
    var
	cur_pos                 : integer;
  begin
    popt(cur_pos);
    with cave[y,x] do
      begin
	tptr := cur_pos;
	t_list[cur_pos] := door_list[1];
	fval  := corr_floor3.ftval;
	fopen := true;
	t_list[cur_pos].p1 := 1;
      end;
  end;


[psect(misc4$code)] procedure place_closed_door(y,x : integer);
    var
	cur_pos                 : integer;
  begin
    popt(cur_pos);
    with cave[y,x] do
      begin
	tptr := cur_pos;
	t_list[cur_pos] := door_list[2];
	fval  := corr_floor3.ftval;
	fopen := false;
      end;
  end;


[psect(misc4$code)] procedure place_locked_door(y,x : integer);
    var
	cur_pos                 : integer;
  begin
    popt(cur_pos);
    with cave[y,x] do
      begin
	tptr := cur_pos;
	t_list[cur_pos] := door_list[2];
	fval  := corr_floor3.ftval;
	fopen := false;
	t_list[cur_pos].p1 := randint(10) + 10;
      end;
  end;


[psect(misc4$code)] procedure place_stuck_door(y,x : integer);
    var
	cur_pos                 : integer;
  begin
    popt(cur_pos);
    with cave[y,x] do
      begin
	tptr := cur_pos;
	t_list[cur_pos] := door_list[2];
	fval  := corr_floor3.ftval;
	fopen := false;
	t_list[cur_pos].p1 := -randint(10) - 10;
      end;
  end;


[psect(misc4$code)] procedure place_secret_door(y,x : integer);
    var
	cur_pos                 : integer;
  begin
    popt(cur_pos);
    with cave[y,x] do
      begin
	tptr := cur_pos;
	t_list[cur_pos] := door_list[3];
	fval  := corr_floor4.ftval;
	fopen := false;
      end;
  end;


[psect(misc4$code)] procedure place_door(y,x : integer);
    var
	cur_pos                 : integer;
  begin
    case randint(3) of
      1 : case randint(4) of
	    1           : place_broken_door(y,x);
	    otherwise     place_open_door(y,x);
	  end;
      2 : case randint(12) of
	    1,2         : place_locked_door(y,x);
	    3           : place_stuck_door(y,x);
	    otherwise     place_closed_door(y,x);
	  end;
      3 : place_secret_door(y,x);
    end;
  end;


	{ Place an up staircase at given y,x			-RAK-	}
[psect(misc4$code)] procedure place_up_stairs(y,x : integer);
    var
	cur_pos                         : integer;
    begin
      with cave[y,x] do
	if (tptr <> 0) then
	  begin
	    pusht(tptr);
	    tptr := 0;
	    fopen := true;
	  end;
      popt(cur_pos);
      cave[y,x].tptr := cur_pos;
      t_list[cur_pos] := up_stair;
    end;

	{ Place a down staircase at given y,x			-RAK-	}
[psect(misc4$code)] procedure place_down_stairs(y,x : integer);
    var
	cur_pos                         : integer;
    begin
      with cave[y,x] do
	if (tptr <> 0) then
	  begin
	    pusht(tptr);
	    tptr := 0;
	    fopen := true;
	  end;
      popt(cur_pos);
      cave[y,x].tptr := cur_pos;
      t_list[cur_pos] := down_stair;
    end;

	{ Places a staircase 1=up, 2=down			-RAK-	}
[psect(misc4$code)] procedure place_stairs(typ,num,walls : integer);
    var
	i1,i2,y1,x1,y2,x2               : integer;
	flag                            : boolean;
    begin
      for i1 := 1 to num do
	  begin
	    flag := false;
	    repeat
	      i2 := 0;
	      repeat
		y1 := randint(cur_height - 12);
		x1 := randint(cur_width  - 12);
		y2 := y1 + 12;
		x2 := x1 + 12;
		repeat
		  repeat
		    with cave[y1,x1] do
		      if (fval in [1,2,4]) then
			if (tptr = 0) then
			  if (next_to4(y1,x1,wall_set) >= walls) then
			    begin
			      flag := true;
			      case typ of
				1 : place_up_stairs(y1,x1);
				2 : place_down_stairs(y1,x1);
			      end;
			    end;
		    x1 := x1 + 1;
		  until ((x1 = x2) or (flag));
		  x1 := x2 - 12;
		  y1 := y1 + 1;
		until ((y1 = y2) or (flag));
		i2 := i2 + 1;
	      until ((flag) or (i2 > 30));
	      walls := walls - 1;
	    until(flag);
	  end;
    end;


	{ Places a treasure (Gold or Gems) at given row, column -RAK-	}
[psect(misc4$code)] procedure place_gold(y,x : integer);
    var
	cur_pos,i1              : integer;
    begin
      popt(cur_pos);
      i1 := trunc((randint(dun_level+2)+2)/2.0);
      if (randint(obj_great) = 1) then
	i1 := i1 + randint(dun_level);
      if (i1 > max_gold) then
	i1 := max_gold;
      cave[y,x].tptr := cur_pos;
      t_list[cur_pos] := gold_list[i1];
      with t_list[cur_pos] do
	cost := randint(8*cost) + cost;
    end;


	{ Returns the array number of a random object		-RAK-	}
[psect(misc4$code)] function get_obj_num(level : integer) : integer;
    var
	i1      : integer;
    begin
      if (level > max_obj_level)  then level := max_obj_level;
      if (randint(obj_great) = 1) then level := max_obj_level;
      if (level = 0) then
	i1 := randint(t_level[0])
      else
	i1 := randint(t_level[level]);
      get_obj_num := i1;
    end;



	{ Places an object at given row, column co-ordinate	-RAK-	}
[psect(misc4$code)] procedure place_object(y,x : integer);
    var
	cur_pos                         : integer;
	mag1,mag2                       : integer;
    begin
      popt(cur_pos);
      cave[y,x].tptr := cur_pos;
      t_list[cur_pos] := object_list[get_obj_num(dun_level)];
      magic_treasure(cur_pos,dun_level);
    end;


	{ Allocates an object for tunnels and rooms		-RAK-	}
[psect(misc4$code)] procedure alloc_object      (
			alloc_set       :       obj_set;
			typ,num         :       integer
						);
    var
	i1,i2,i3                : integer;
    begin
      for i3 := 1 to num do
	begin
	  repeat
	    i1 := randint(cur_height);
	    i2 := randint(cur_width);
	  until ((cave[i1,i2].fval in alloc_set) and
		 (cave[i1,i2].tptr = 0));
	  case typ of
	    1 : place_trap(i1,i2,1,randint(max_trapa));
	    2 : place_trap(i1,i2,2,randint(max_trapb));
	    3 : place_rubble(i1,i2);
	    4 : place_gold(i1,i2);
	    5 : place_object(i1,i2);
	  end
	end
    end;


	{ Creates objects nearby the coordinates given		-RAK-	}
[psect(misc4$code)] procedure random_object(y,x,num : integer);
    var
	i1,i2,i3                : integer;
    begin
      repeat
	i1 := 0;
	repeat
	  i2 := y - 3 + randint(5);
	  i3 := x - 4 + randint(7);
	  with cave[i2,i3] do
	    if (fval in floor_set) then
	      if (tptr = 0) then
		begin
		  if (randint(100) < 75) then
		    place_object(i2,i3)
		  else
		    place_gold(i2,i3);
		  i1 := 9;
		end;
	  i1 := i1 + 1;
	until (i1 > 10);
	num := num - 1;
      until (num = 0);
    end;


	{ Converts stat num into string 			-RAK-	}
[psect(misc5$code)] procedure cnv_stat  (
			stat            :       byteint;
			var out_val     :       stat_type
					);
    var
	tmp_str                         : vtype;
	part1,part2                     : integer;
    begin
      if (stat > 18) then
	begin
	  part1 := 18;
	  part2 := stat - 18;
	  writev(tmp_str,part1:2,'/',part2:1);
	end
      else
	writev(tmp_str,stat:2);
      if (length(tmp_str) < 6) then tmp_str := pad(tmp_str,' ',6);
      out_val := tmp_str;
    end;


	{ Print character stat in given row, column		-RAK-	}
[psect(misc5$code)] procedure prt_stat  (
			stat_name       : vtype;
			stat            : byteint;
			row,column      : integer
					);
      var
		out_val1                : stat_type;
		out_val2                : vtype;
      begin
	cnv_stat(stat,out_val1);
	out_val2 := stat_name + out_val1;
	put_buffer(out_val2,row,column);
      end;


	{ Print character info in given row, column		-RAK-	}
[psect(misc5$code)] procedure prt_field(info : vtype; row,column : integer);
      var
		out_val1,out_val2       : vtype;
      begin
	put_buffer(pad(info,' ',14),row,column);
      end;


	{ Print number with header at given row, column 	-RAK-	}
[psect(misc5$code)] procedure prt_num   (
			header          :       vtype;
			num,row,column  :       integer
					);
      var
		out_val                 : vtype;
      begin
	writev(out_val,header,num:1,' ');
	put_buffer(out_val,row,column);
      end;


	{ Adjustment for wisdom 				-JWT-	}
[psect(misc2$code)] function wis_adj : integer;
    begin
      if (py.stat.cwis > 117) then
	wis_adj := 7
      else if (py.stat.cwis > 107) then
	wis_adj := 6
      else if (py.stat.cwis > 87) then
	wis_adj := 5
      else if (py.stat.cwis > 67) then
	wis_adj := 4
      else if (py.stat.cwis > 17) then
	wis_adj := 3
      else if (py.stat.cwis > 14) then
	wis_adj := 2
      else if (py.stat.cwis > 7) then
	wis_adj := 1
      else
	wis_adj := 0;
    end;


	{ adjustment for intellegence				-JWT-	}
[psect(misc2$code)] function int_adj : integer;
    begin
      if (py.stat.cint > 117) then
	int_adj := 7
      else if (py.stat.cint > 107) then
	int_adj := 6
      else if (py.stat.cint > 87) then
	int_adj := 5
      else if (py.stat.cint > 67) then
	int_adj := 4
      else if (py.stat.cint > 17) then
	int_adj := 3
      else if (py.stat.cint > 14) then
	int_adj := 2
      else if (py.stat.cint > 7) then
	int_adj := 1
      else
	int_adj := 0;
    end;


	{ Adjustment for charisma				-RAK-	}
	{ Percent decrease or increase in price of goods                }
[psect(misc2$code)] function chr_adj : real;
    begin
      if (py.stat.cchr > 117) then
	chr_adj := -0.10
      else if (py.stat.cchr > 107) then
	chr_adj := -0.08
      else if (py.stat.cchr > 87) then
	chr_adj := -0.06
      else if (py.stat.cchr > 67) then
	chr_adj := -0.04
      else if (py.stat.cchr > 18) then
	chr_adj := -0.02
      else
	case py.stat.cchr of
	  18 :  chr_adj :=  0.00;
	  17 :  chr_adj :=  0.01;
	  16 :  chr_adj :=  0.02;
	  15 :  chr_adj :=  0.03;
	  14 :  chr_adj :=  0.04;
	  13 :  chr_adj :=  0.06;
	  12 :  chr_adj :=  0.08;
	  11 :  chr_adj :=  0.10;
	  10 :  chr_adj :=  0.12;
	   9 :  chr_adj :=  0.14;
	   8 :  chr_adj :=  0.16;
	   7 :  chr_adj :=  0.18;
	   6 :  chr_adj :=  0.20;
	   5 :  chr_adj :=  0.22;
	   4 :  chr_adj :=  0.24;
	   3 :  chr_adj :=  0.25;
	   otherwise chr_adj := 0.00;   { Error trap    }
	end;
    end;


	{ Returns a character's adjustment to hit points        -JWT-   }
[psect(misc2$code)] function con_adj : integer;
  begin
    with py.stat do
      if      (ccon =   3) then con_adj := -4
      else if (ccon =   4) then con_adj := -3
      else if (ccon =   5) then con_adj := -2
      else if (ccon =   6) then con_adj := -1
      else if (ccon <  17) then con_adj :=  0
      else if (ccon =  17) then con_adj :=  1
      else if (ccon <  94) then con_adj :=  2
      else if (ccon < 117) then con_adj :=  3
      else                      con_adj :=  4
  end;


	{ Calculates hit points for each level that is gained.	-RAK-	}
[psect(misc2$code)] function get_hitdie : integer;
    var
	i1      : integer;
    begin
      get_hitdie := randint(py.misc.hitdie) + con_adj;
    end;


	{ Prints title of character				-RAK-	}
[psect(misc5$code)] procedure prt_title;
    begin
      prt_field(py.misc.title,5,stat_column);
    end;


	{ Prints strength					-RAK-	}
[psect(misc5$code)] procedure prt_strength;
    begin
      prt_stat('',py.stat.cstr,7,stat_column+6);
    end;


	{ Prints intelligence					-RAK-	}
[psect(misc5$code)] procedure prt_intelligence;
    begin
      prt_stat('',py.stat.cint,8,stat_column+6);
    end;


	{ Prints wisdom 					-RAK-	}
[psect(misc5$code)] procedure prt_wisdom;
    begin
      prt_stat('',py.stat.cwis,9,stat_column+6);
    end;


	{ Prints dexterity					-RAK-	}
[psect(misc5$code)] procedure prt_dexterity;
    begin
      prt_stat('',py.stat.cdex,10,stat_column+6);
    end;


	{ Prints constitution					-RAK-	}
[psect(misc5$code)] procedure prt_constitution;
    begin
      prt_stat('',py.stat.ccon,11,stat_column+6);
    end;


	{ Prints charisma					-RAK-	}
[psect(misc5$code)] procedure prt_charisma;
    begin
      prt_stat('',py.stat.cchr,12,stat_column+6);
    end;



	{ Prints level						-RAK-	}
[psect(misc5$code)] procedure prt_level;
    begin
      prt_num( '',py.misc.lev,14,stat_column+6);
    end;


	{ Prints players current mana points (a real number...) -RAK-	}
[psect(misc1$code)] procedure prt_cmana;
    begin
      prt_num( '',trunc(py.misc.cmana),16,stat_column+6);
    end;


	{ Prints Max hit points 				-RAK-	}
[psect(misc5$code)] procedure prt_mhp;
    begin
      prt_num( '',py.misc.mhp,17,stat_column+6);
    end;


	{ Prints players current hit points (a real number...)	-RAK-	}
[psect(misc1$code)] procedure prt_chp;
    begin
      prt_num( '',trunc(py.misc.chp),18,stat_column+6);
    end;


	{ prints current AC					-RAK-	}
[psect(misc5$code)] procedure prt_pac;
    begin
      prt_num( '',py.misc.dis_ac,20,stat_column+6);
    end;


	{ Prints current gold					-RAK-	}
[psect(misc5$code)] procedure prt_gold;
    begin
      prt_num( '',py.misc.au,21,stat_column+6);
    end;


	{ Prints depth in stat area				-RAK-	}
[psect(misc5$code)] procedure prt_depth;
      var
		depths  : vtype;
		depth   : integer;
      begin
	depth := dun_level*50;
	if (depth = 0) then
	  depths := 'Town level'
	else
	  writev(depths,'Depth: ',depth:1,' (feet)');
	prt(depths,24,61);
      end;


	{ Prints status of hunger				-RAK-	}
[psect(misc5$code)] procedure prt_hunger;
    begin
      if (uand(%X'000002',py.flags.status) <> 0) then
	put_buffer('Weak    ',24,1)
      else if (uand(%X'000001',py.flags.status) <> 0) then
	put_buffer('Hungry  ',24,1)
      else
	put_buffer('        ',24,1);
    end;


	{ Prints Blind status					-RAK-	}
[psect(misc5$code)] procedure prt_blind;
    begin
      if (uand(%X'000004',py.flags.status) <> 0) then
	put_buffer('Blind  ',24,9)
      else
	put_buffer('       ',24,9);
    end;


	{ Prints Confusion status				-RAK-	}
[psect(misc5$code)] procedure prt_confused;
    begin
      if (uand(%X'000008',py.flags.status) <> 0) then
	put_buffer('Confused  ',24,16)
      else
	put_buffer('          ',24,16);
    end;


	{ Prints Fear status					-RAK-	}
[psect(misc5$code)] procedure prt_afraid;
    begin
      if (uand(%X'000010',py.flags.status) <> 0) then
	put_buffer('Afraid  ',24,26)
      else
	put_buffer('        ',24,26);
    end;


	{ Prints Poisoned status				-RAK-	}
[psect(misc5$code)] procedure prt_poisoned;
    begin
      if (uand(%X'000020',py.flags.status) <> 0) then
	put_buffer('Poisoned  ',24,34)
      else
	put_buffer('          ',24,34);
    end;


	{ Prints Searching status				-RAK-	}
[psect(misc5$code)] procedure prt_search;
    begin
      if (uand(%X'000100',py.flags.status) <> 0) then
	put_buffer('Searching  ',24,44)
      else
	put_buffer('           ',24,44);
    end;


	{ Prints Resting status 				-RAK-	}
[psect(misc5$code)] procedure prt_rest;
    begin
      if (uand(%X'000200',py.flags.status) <> 0) then
	put_buffer('Resting    ',24,44)
      else
	put_buffer('           ',24,44);
    end;


	{ Prints winner status on display			-RAK-	}
[psect(misc5$code)] procedure prt_winner;
    begin
      put_buffer('*Winner*',23,1);
    end;





	{ Increases a stat by one randomized level		-RAK-	}
[psect(misc2$code)] function in_statp(stat : byteint) : byteint;
    begin
      if (stat < 18) then
	stat := stat + 1
      else if (stat < 88) then
	stat := stat + randint(25)
      else if (stat < 108) then
	stat := stat + randint(10)
      else
	stat := stat + 1;
      if (stat > 118) then
	stat := 118;
      in_statp := stat;
    end;


	{ Decreases a stat by one randomized level		-RAK-	}
[psect(misc2$code)] function de_statp(stat : byteint) : byteint;
    begin
      if (stat < 19) then
	stat := stat - 1
      else if (stat < 109) then
	begin
	  stat := stat - randint(10) - 5;
	  if (stat < 18) then stat := 18;
	end
      else
	stat := stat - randint(3);
      if (stat < 3) then stat := 3;
      de_statp := stat;
    end;


	{ Increases a stat by one true level			-RAK-	}
[psect(misc2$code)] function in_statt(stat : byteint) : byteint;
    begin
      if (stat < 18) then
	stat := stat + 1
      else
	begin
	  stat := stat + 10;
	  if (stat > 118) then
	    stat := 118;
	end;
      in_statt := stat;
    end;


	{ Decreases a stat by true level			-RAK-	}
[psect(misc2$code)] function de_statt(stat : byteint) : byteint;
    begin
      if (stat > 27) then
	stat := stat - 10
      else if (stat > 18) then
	stat := 18
      else
	begin
	  stat := stat - 1;
	  if (stat < 3) then stat := 3;
	end;
      de_statt := stat;
    end;


	{ Returns a character's adjustment to hit.              -JWT-   }
[psect(misc2$code)] function tohit_adj : integer;
  var
	total                   : integer;
  begin
    with py.stat do
      begin
	if      (cdex <   4) then total := -3
	else if (cdex <   6) then total := -2
	else if (cdex <   8) then total := -1
	else if (cdex <  16) then total :=  0
	else if (cdex <  17) then total :=  1
	else if (cdex <  18) then total :=  2
	else if (cdex <  69) then total :=  3
	else if (cdex < 118) then total :=  4
	else                      total :=  5;
	if      (cstr <   4) then total := total - 3
	else if (cstr <   5) then total := total - 2
	else if (cstr <   7) then total := total - 1
	else if (cstr <  18) then total := total + 0
	else if (cstr <  94) then total := total + 1
	else if (cstr < 109) then total := total + 2
	else if (cstr < 117) then total := total + 3
	else                      total := total + 4;
    end;
    tohit_adj := total;
  end;


	{ Returns a character's adjustment to armor class       -JWT-   }
[psect(misc2$code)] function toac_adj : integer;
  begin
    with py.stat do
      if      (cdex <   4) then toac_adj :=  -4
      else if (cdex =   4) then toac_adj :=  -3
      else if (cdex =   5) then toac_adj :=  -2
      else if (cdex =   6) then toac_adj :=  -1
      else if (cdex <  15) then toac_adj :=   0
      else if (cdex <  18) then toac_adj :=   1
      else if (cdex <  59) then toac_adj :=   2
      else if (cdex <  94) then toac_adj :=   3
      else if (cdex < 117) then toac_adj :=   4
      else                      toac_adj :=   5
  end;


	{ Returns a character's adjustment to disarm            -RAK-   }
[psect(misc2$code)] function todis_adj : integer;
  begin
    with py.stat do
      if      (cdex =   3) then todis_adj :=  -8
      else if (cdex =   4) then todis_adj :=  -6
      else if (cdex =   5) then todis_adj :=  -4
      else if (cdex =   6) then todis_adj :=  -2
      else if (cdex =   7) then todis_adj :=  -1
      else if (cdex <  13) then todis_adj :=   0
      else if (cdex <  16) then todis_adj :=   1
      else if (cdex <  18) then todis_adj :=   2
      else if (cdex <  59) then todis_adj :=   4
      else if (cdex <  94) then todis_adj :=   5
      else if (cdex < 117) then todis_adj :=   6
      else                      todis_adj :=   8
  end;


	{ Returns a character's adjustment to damage            -JWT-   }
[psect(misc2$code)] function todam_adj : integer;
  begin
    with py.stat do
      if      (cstr <   4) then todam_adj := -2
      else if (cstr <   5) then todam_adj := -1
      else if (cstr <  16) then todam_adj :=  0
      else if (cstr <  17) then todam_adj :=  1
      else if (cstr <  18) then todam_adj :=  2
      else if (cstr <  94) then todam_adj :=  3
      else if (cstr < 109) then todam_adj :=  4
      else if (cstr < 117) then todam_adj :=  5
      else                      todam_adj :=  6;
  end;


	{ Pre-declaration of PRT_EXPERIENCE			-RAK-	}
[psect(misc5$code)] procedure prt_experience;
	forward;


	{ Prints character-screen info				-RAK-	}
[psect(misc5$code)] procedure prt_stat_block;
    begin
      prt_field(py.misc.race,                 3,stat_column);
      prt_field(py.misc.tclass,               4,stat_column);
      prt_field(py.misc.title,                5,stat_column);
      prt_stat('STR : ',py.stat.cstr,         7,stat_column);
      prt_stat('INT : ',py.stat.cint,         8,stat_column);
      prt_stat('WIS : ',py.stat.cwis,         9,stat_column);
      prt_stat('DEX : ',py.stat.cdex,        10,stat_column);
      prt_stat('CON : ',py.stat.ccon,        11,stat_column);
      prt_stat('CHR : ',py.stat.cchr,        12,stat_column);
      prt_num( 'LEV : ',py.misc.lev,         14,stat_column);
      prt_num( 'EXP : ',py.misc.exp,         15,stat_column);
      prt_num( 'MANA: ',trunc(py.misc.cmana),16,stat_column);
      prt_num( 'MHP : ',py.misc.mhp,         17,stat_column);
      prt_num( 'CHP : ',trunc(py.misc.chp),  18,stat_column);
      prt_num( 'AC  : ',py.misc.dis_ac,      20,stat_column);
      prt_num( 'GOLD: ',py.misc.au,          21,stat_column);
      if (total_winner) then prt_winner;
      if (uand(%X'000003',py.flags.status) <> 0) then
	prt_hunger;
      if (uand(%X'000004',py.flags.status) <> 0) then
	prt_blind;
      if (uand(%X'000008',py.flags.status) <> 0) then
	prt_confused;
      if (uand(%X'000010',py.flags.status) <> 0) then
	prt_afraid;
      if (uand(%X'000020',py.flags.status) <> 0) then
	prt_poisoned;
      if (uand(%X'000100',py.flags.status) <> 0) then
	prt_search;
      if (uand(%X'000200',py.flags.status) <> 0) then
	prt_rest;
    end;


	{ Draws entire screen					-RAK-	}
[psect(misc5$code)] procedure draw_cave;
    begin
      clear(1,1);
      prt_stat_block;
      prt_map;
      prt_depth;
    end;


	{ Prints the following information on the screen.	-JWT-	}
[psect(create$code)] procedure put_character;
    begin
      clear(1,1);
      with py.misc do
	begin
	  prt('Name      : ' + name,3,3);
	  prt('Race      : ' + race,4,3);
	  prt('Sex       : ' + sex,5,3);
	  prt('Class     : ' + tclass,6,3)
	end
    end;


	{ Prints the following information on the screen.	-JWT-	}
[psect(create$code)] procedure put_stats;
    begin
      with py do
	begin
	  prt_stat('STR : ',stat.cstr,3,65);
	  prt_stat('INT : ',stat.cint,4,65);
	  prt_stat('WIS : ',stat.cwis,5,65);
	  prt_stat('DEX : ',stat.cdex,6,65);
	  prt_stat('CON : ',stat.ccon,7,65);
	  prt_stat('CHR : ',stat.cchr,8,65);
	  prt_num('+ To Hit   : ',misc.dis_th,10,4);
	  prt_num('+ To Damage: ',misc.dis_td,11,4);
	  prt_num('+ To AC    : ',misc.dis_tac,12,4);
	  prt_num('  Total AC : ',misc.dis_ac,13,4);
	end
    end;


	{ Returns a rating of x depending on y			-JWT-	}
[psect(create$code)] function likert(x,y : integer) : btype;
      begin
	case trunc(x/y) of
	  -3,-2,-1      : likert := 'Very Bad';
	  0,1           : likert := 'Bad';
	  2             : likert := 'Poor';
	  3,4           : likert := 'Fair';
	  5             : likert := 'Good';
	  6             : likert := 'Very Good';
	  7,8           : likert := 'Superb';
	  otherwise       likert := 'Excellent';
	end
      end;


	{ Prints age, height, weight, and SC			-JWT-	}
[psect(create$code)] procedure put_misc1;
    begin
      with py do
	begin
	  prt_num('Age          : ',misc.age,3,40);
	  prt_num('Height       : ',misc.ht ,4,40);
	  prt_num('Weight       : ',misc.wt ,5,40);
	  prt_num('Social Class : ',misc.sc ,6,40);
	end;
    end;


	{ Prints the following information on the screen.	-JWT-	}
[psect(create$code)] procedure put_misc2;
    begin
      with py.misc do
	begin
	  prt_num('Level      : ',lev ,10,31);
	  prt_num('Experience : ',exp ,11,31);
	  prt_num('Gold       : ',au  ,12,31);
	  prt_num('Max Hit Points : ',mhp  ,10,54);
	  prt_num('Cur Hit Points : ',trunc(chp),11,54);
	  prt_num('Max Mana       : ',mana ,12,54);
	  prt_num('Cur Mana       : ',trunc(cmana),13,54);
	end
    end;


	{ Prints ratings on certain abilities			-RAK-	}
[psect(create$code)] procedure put_misc3;
      var
	xbth,xbthb,xfos,xsrh,xstl,xdis,xsave,xdev       : integer;
	xinfra                                          : vtype;
      begin
	clear(14,1);
	with py.misc do
	  begin
	    xbth  := bth + lev*bth_lev_adj + ptohit*bth_plus_adj;
	    xbthb := bthb + lev*bth_lev_adj + ptohit*bth_plus_adj;
	    xfos  := 27 - fos;
	    if (xfos < 0) then xfos := 0;
	    xsrh  := srh + int_adj;
	    xstl  := stl;
	    xdis  := disarm + lev + 2*todis_adj + int_adj;
	    xsave := save + lev + wis_adj;
	    xdev  := save + lev + int_adj;
	    writev(xinfra,py.flags.see_infra*10:1,' feet');
	  end;
	prt('(Miscellaneous Abilities)',16,24);
	put_buffer('Fighting    : '+likert(xbth ,12) ,17, 2);
	put_buffer('Bows/Throw  : '+likert(xbthb,12) ,18, 2);
	put_buffer('Saving Throw: '+likert(xsave, 6) ,19, 2);
	put_buffer('Stealth     : '+likert(xstl , 1) ,17,27);
	put_buffer('Disarming   : '+likert(xdis , 8) ,18,27);
	put_buffer('Magic Device: '+likert(xdev , 7) ,19,27);
	put_buffer('Perception  : '+likert(xfos , 3) ,17,52);
	put_buffer('Searching   : '+likert(xsrh , 6) ,18,52);
	put_buffer('Infra-Vision: '+xinfra,           19,52);
      end;


	{ Used to display the character on the screen.		-RAK-	}
[psect(create$code)] procedure display_char;
      var
	dummy   : char;
      begin
	put_character;
	put_misc1;
	put_stats;
	put_misc2;
	put_misc3;
      end;


	{ Gets a name for the character 			-JWT-	}
[psect(create$code)] procedure get_name;
    begin
      prt('Enter your player''s name  [press <RETURN> when finished]',22,3);
      get_string(py.misc.name,3,15,24);
      clear(21,1);
    end;


	{ Chances the name of the character			-JWT-	}
[psect(create$code)] procedure change_name;
    var
	c       : char;
	flag    : boolean;
    begin
      flag := false;
      display_char;
      repeat
	prt('<c>hange character name.     <ESCAPE> to continue.',22,3);
	inkey(c);
	case ord(c) of
	  99            : get_name;
	  0,3,25,26,27  : flag := true;
	  otherwise;
	end;
      until (flag);
    end;


	{ Builds passwords					-RAK-	}
[psect(setup$code)] procedure bpswd;
      var
		i1              : integer;
      begin
	seed := wdata[1,0];
	for i1 := 1 to 12 do
	  password1[i1] := chr( uxor(wdata[1,i1],randint(255)) );
	seed := wdata[2,0];
	for i1 := 1 to 12 do
	  password2[i1] := chr( uxor(wdata[2,i1],randint(255)) );
	seed := get_seed;
      end;


	{ Destroy an item in the inventory			-RAK-	}
[psect(misc4$code)] procedure inven_destroy(item_val : integer);
      var
		i2      : integer;
      begin
	inventory[inven_max] := inventory[item_val];
	with inventory[item_val] do
	  begin
	    if ((number > 1) and (subval < 512))  then
	      begin
		number := number - 1;
		inven_weight := inven_weight - weight;
		inventory[inven_max].number := 1;
	      end
	    else
	      begin
		inven_weight := inven_weight - weight*number;
		for i2 := item_val to inven_ctr-1 do
		  inventory[i2] := inventory[i2+1];
		inventory[inven_ctr] := blank_treasure;
		inven_ctr := inven_ctr - 1;
	      end;
	  end
      end;


	{ Drops an item from inventory to given location	-RAK-	}
[psect(misc4$code)] procedure inven_drop(item_val,y,x : integer);
      var
	i1                              : integer;
      begin
	with cave[y,x] do
	  begin
	    if (tptr > 0) then pusht(tptr);
	    inven_destroy(item_val);
	    popt(i1);
	    t_list[i1] := inventory[inven_max];
	    tptr := i1;
	  end;
      end;


	{ Destroys a type of item on a given percent chance	-RAK-	}
[psect(misc4$code)] function inven_damage(
			typ             :       obj_set;
			perc            :       integer
					) : integer;
      var
		i1,i2           : integer;
      begin
	i2 := 0;
	for i1 := 1 to inven_ctr do
	  with inventory[i1] do
	    if (tval in typ) then
	      if (randint(100) < perc) then
		begin
		  inven_destroy(i1);
		  i2 := i2 + 1;
		end;
	inven_damage := i2;
      end;


	{ Computes current weight limit 			-RAK-	}
[psect(misc4$code)] function weight_limit : integer;
      var
	weight_cap      : integer;
      begin
	weight_cap := py.stat.cstr*player_weight_cap + py.misc.wt;
	if (weight_cap > 3000) then weight_cap := 3000;
	weight_limit := weight_cap;
      end;


	{ Check inventory for too much weight			-RAK-	}
[psect(misc4$code)] function inven_check_weight : boolean;
      var
	item_wgt,max_weight             : integer;
      begin
	inven_check_weight := false;
	max_weight := weight_limit;
	with inventory[inven_max] do
	  item_wgt := number*weight;
		{ Now, check to see if player can carry object  }
	if ((inven_weight + item_wgt) <= max_weight) then
	  inven_check_weight := true;   { Can carry weight      }
      end;


	{ Check to see if he will be carrying too many objects	-RAK-	}
[psect(misc4$code)] function inven_check_num : boolean;
      var
	item_num,i1                     : integer;
	flag                            : boolean;
      begin
	inven_check_num := false;
	if (inven_ctr < 22) then
	  inven_check_num := true
	else if (inventory[inven_max].subval > 255) then
	  for i1 := 1 to inven_ctr do
	    with inventory[i1] do
	      if (tval = inventory[inven_max].tval) then
		if (subval = inventory[inven_max].subval) then
		  inven_check_num := true;
      end;


	{ Add the item in INVEN_MAX to players inventory.  Return the	}
	{ item position for a description if needed...          -RAK-   }
[psect(misc4$code)] procedure inven_carry(var item_val : integer);
      var
	item_num,wgt                    : integer;
	typ,subt                        : integer;
	flag                            : boolean;

	{ Insert INVEN_MAX at given location	}
      procedure insert(pos,wgt : integer);
	var
		i1      : integer;
	begin
	  for i1 :=  inven_ctr downto pos do
	    inventory[i1+1] := inventory[i1];
	  inventory[pos] := inventory[inven_max];
	  inven_ctr      := inven_ctr   + 1;
	  inven_weight   := inven_weight + wgt;
	end;

	{ INVEN_CARRY routine			}
      begin
		{ Now, check to see if player can carry object  }
	item_val := 0;
	flag := false;
	with inventory[inven_max] do
	  begin
	    item_num := number;
	    typ  := tval;
	    subt := subval;
	    wgt  := number*weight;
	  end;
	repeat
	  item_val := item_val + 1;
	  with inventory[item_val] do
	    if (typ = tval) then
	      begin
		if (subt = subval) then { Adds to other item    }
		  if (subt > 255) then
		    begin
		      number := number + item_num;
		      inven_weight := inven_weight + wgt;
		      flag := true;
		    end;
	      end
	    else if (typ > tval) then
	      begin             { Insert into list              }
		insert(item_val,wgt);
		flag := true;
	      end;
	until ((item_val >= inven_ctr) or (flag));
	if (not(flag)) then     { Becomes last item in list     }
	  begin
	    insert(inven_ctr+1,wgt);
	    item_val := inven_ctr;
	  end;
      end;


	{ Returns spell chance of failure for spell		-RAK-	}
[psect(misc2$code)] procedure spell_chance(var spell : spl_rec);
      begin
	with magic_spell[py.misc.pclass,spell.splnum] do
	  with spell do
	    begin
	      splchn := sfail - 3*(py.misc.lev-slevel);
	      if (class[py.misc.pclass].mspell) then
		splchn := splchn - 3*(int_adj-1)
	      else
		splchn := splchn - 3*(wis_adj-1);
	      if (smana > py.misc.cmana) then
		splchn := splchn + 5*trunc(smana-py.misc.cmana);
	      if (splchn > 95) then
		splchn := 95
	      else if (splchn < 5) then
		splchn := 5;
	    end
      end;


	{ Print list of spells					-RAK-	}
[psect(misc2$code)] procedure print_new_spells(
			spell           :       spl_type;
			num             :       integer;
			var redraw      :       boolean
					);
    var
	i1                              : integer;
	out_val                         : vtype;
    begin
      redraw := true;
      clear(1,1);
      prt('   Name                          Level  Mana  %Failure',2,1);
      for i1 := 1 to num do
	with magic_spell[py.misc.pclass,spell[i1].splnum] do
	  begin
	    spell_chance(spell[i1]);
	    writev(out_val,chr(96+i1),') ',pad(sname,' ',30),
		slevel:3,'    ',smana:3,'      ',spell[i1].splchn:2);
	    prt(out_val,2+i1,1);
	  end;
    end;



	{ Returns spell pointer 				-RAK-	}
[psect(misc2$code)] function get_spell(spell : spl_type; num : integer;
		     var sn,sc : integer; prompt : vtype;
		     var redraw : boolean) : boolean;
    var
	i1                              : integer;
	flag                            : boolean;
	choice                          : char;
	out_val1,out_val2               : vtype;
    begin
      sn := 0;
      flag := true;
      writev(out_val1,'(Spells a-',chr(num+96),
				', *=List, <ESCAPE>=exit) ',prompt);
      while (((sn < 1) or (sn > num)) and (flag)) do
	begin
	  prt(out_val1,1,1);
	  inkey(choice);
	  sn := ord(choice);
	  case sn of
	    0,3,25,26,27:       begin
			  flag := false;
			  reset_flag := true;
			end;
	    42        : print_new_spells(spell,num,redraw);
	    otherwise   sn := sn - 96;
	  end;
	end;
      msg_flag := false;
      if (flag) then
	begin
	  spell_chance(spell[sn]);
	  sc := spell[sn].splchn;
	  sn := spell[sn].splnum;
	end;
      get_spell := flag;
    end;


	{ Learn some magic spells (Mage)			-RAK-	}
[psect(misc2$code)] function learn_spell(var redraw : boolean) : boolean;
      var
	i2                              : unsigned;
	i1,i3,sn,sc                     : integer;
	new_spells                      : integer;
	spell_flag                      : unsigned;
	spell                           : spl_type;
      begin
	learn_spell := false;
	case int_adj of
	  0 : new_spells := 0;
	  1 : new_spells := 1;
	  2 : new_spells := 1;
	  3 : new_spells := 1;
	  4 : new_spells := randint(2);
	  5 : new_spells := randint(2);
	  6 : new_spells := randint(3);
	  7 : new_spells := randint(2)+1;
	  otherwise new_spells := 0;
	end;
	i1 := 0;
	spell_flag := 0;
	repeat
	  i1 := i1 + 1;
	  if (inventory[i1].tval = 90) then
	    spell_flag := uor(spell_flag,inventory[i1].flags);
	until (i1 >= inven_ctr);
	while ((new_spells > 0) and (spell_flag > 0)) do
	  begin
	    i1 := 0;
	    i2 := spell_flag;
	    repeat
	      i3 := bit_pos(i2);
	      with magic_spell[py.misc.pclass,i3] do
		if (slevel <= py.misc.lev) then
		  if (not(learned)) then
		    begin
		      i1 := i1 + 1;
		      spell[i1].splnum := i3;
		    end;
	    until(i2 = 0);
	    if (i1 > 0) then
	      begin
		print_new_spells(spell,i1,redraw);
		if (get_spell(spell,i1,sn,sc,'Learn which spell?',redraw)) then
		  begin
		    magic_spell[py.misc.pclass,sn].learned := true;
		    learn_spell := true;
		    if (py.misc.mana = 0) then
		      begin
			py.misc.mana   := 1;
			py.misc.cmana := 1;
		      end;
		  end
		else
		  new_spells := 0;
	      end
	    else
	      new_spells := 0;
	    new_spells := new_spells - 1;
	  end;
      end;


	{ Learn some prayers (Priest)				-RAK-	}
[psect(misc2$code)] function learn_prayer : boolean;
      var
	i1,i2,i3,i4,new_spell           : integer;
	test_array                      : array [1..32] of integer;
	spell_flag                      : unsigned;
      begin
	i1 := 0;
	spell_flag := 0;
	repeat
	  i1 := i1 + 1;
	  if (inventory[i1].tval = 91) then
	    spell_flag := uor(spell_flag,inventory[i1].flags);
	until(i1 >= inven_ctr);
	i1 := 0;
	while (spell_flag > 0) do
	  begin
	    i2 := bit_pos(spell_flag);
	    with magic_spell[py.misc.pclass,i2] do
	      if (slevel <= py.misc.lev) then
		if (not(learned)) then
		  begin
		    i1 := i1 + 1;
		    test_array[i1] := i2;
		  end;
	  end;
	case wis_adj of
	  0 : i2 := 0;
	  1 : i2 := 1;
	  2 : i2 := 1;
	  3 : i2 := 1;
	  4 : i2 := randint(2);
	  5 : i2 := randint(2);
	  6 : i2 := randint(3);
	  7 : i2 := randint(2)+1;
	end;
	new_spell := 0;
	while ((i1 > 0) and (i2 > 0)) do
	  begin
	    i3 := randint(i1);
	    magic_spell[py.misc.pclass,test_array[i3]].learned := true;
	    new_spell := new_spell + 1;
	    for i4 := i3 to i1-1 do
	      test_array[i4] := test_array[i4+1];
	    i1 := i1 - 1;       { One less spell to learn       }
	    i2 := i2 - 1;       { Learned one                   }
	  end;
	  if (new_spell > 0) then
	    begin
	      if (new_spell > 1) then
		msg_print('You learned new prayers!')
	      else
		msg_print('You learned a new prayer!');
	      if (py.misc.exp = 0) then msg_print(' ');
	      if (py.misc.mana = 0) then
		begin
		  py.misc.mana  := 1;
		  py.misc.cmana := 1;
		end;
	      learn_prayer := true;
	    end
	  else
	    learn_prayer := false;
      end;


	{ Gain some mana if you knows at least one spell	-RAK-	}
[psect(misc2$code)] procedure gain_mana(amount : integer);
      var
	i1,new_mana                     : integer;
	knows_spell                     : boolean;
      begin
	knows_spell := false;
	for i1 := 1 to 31 do
	  if (magic_spell[py.misc.pclass,i1].learned) then
	    knows_spell := true;
	if (knows_spell) then
	  begin
	    if (odd(py.misc.lev)) then
	      case amount of
		0 : new_mana := 0;
		1 : new_mana := 1;
		2 : new_mana := 1;
		3 : new_mana := 1;
		4 : new_mana := 2;
		5 : new_mana := 2;
		6 : new_mana := 3;
		7 : new_mana := 4;
		otherwise new_mana := 0;
	      end
	    else
	      case amount of
		0 : new_mana := 0;
		1 : new_mana := 1;
		2 : new_mana := 1;
		3 : new_mana := 2;
		4 : new_mana := 2;
		5 : new_mana := 3;
		6 : new_mana := 3;
		7 : new_mana := 4;
		otherwise new_mana := 0;
	      end;
	    py.misc.mana  := py.misc.mana  + new_mana;
	    py.misc.cmana := py.misc.cmana + new_mana;
	  end;
      end;


	{ Increases hit points and level			-RAK-	}
[psect(misc2$code)] procedure gain_level;
    var
	nhp,dif_exp,need_exp            : integer;
	redraw                          : boolean;
	out_val                         : vtype;
    begin
      with py.misc do
	  begin
	    nhp := get_hitdie;
	    mhp := mhp + nhp;
	    chp := chp + nhp;
	    lev := lev + 1;
	    need_exp := trunc(player_exp[lev]*expfact);
	    if (py.misc.exp > need_exp) then
	      begin
		dif_exp := py.misc.exp - need_exp;
		py.misc.exp := need_exp + (dif_exp div 2);
	      end;
	    title := player_title[pclass,lev];
	    writev(out_val,'Welcome to level ',lev:1,'.');
	    msg_print(out_val);
	    msg_print(' ');
	    msg_flag := false;
	    prt_mhp;
	    prt_chp;
	    prt_level;
	    prt_title;
	    with class[pclass] do
	      begin
		if (mspell) then
		  begin
		    redraw := false;
		    learn_spell(redraw);
		    if (redraw) then draw_cave;
		    gain_mana(int_adj);
		    prt_cmana;
		  end
		else if (pspell) then
		  begin
		    learn_prayer;
		    gain_mana(wis_adj);
		    prt_cmana;
		  end;
	      end;
	  end;
    end;


	{ Prints experience					-RAK-	}
  procedure prt_experience;
    var
	tmp_exp         : integer;
    begin
      with py.misc do
	begin
	  if (exp > player_max_exp) then exp := player_max_exp;
	  if (lev < max_player_level) then
	    begin
	      while (trunc(player_exp[lev]*expfact) <= exp) do gain_level;
	      if (exp > max_exp) then max_exp := exp;
	    end;
	end;
      prt_num('',py.misc.exp,15,stat_column+6);
    end;


	{ Inserts a string into a string				}
  procedure insert_str                  (
			%ref object_str : varying[a] of char;
			%ref mtc_str    : varying[b] of char;
			%ref insert_str : varying[c] of char
					);
	external;


	{ Inserts a number into a string				}
[psect(misc1$code)] procedure insert_num(
			var object_str  : varying[a] of char;
			mtc_str         : varying[b] of char;
			number          : integer;
			show_sign       : boolean
					);
    var
	pos,olen,mlen   : integer;
	str1,str2       : vtype;
    begin
      pos := index(object_str,mtc_str);
      if (pos > 0) then
	begin
	  olen := length(object_str);
	  mlen := length(mtc_str);
	  object_str := object_str + ' ';
	  str1 := substr(object_str,1,pos-1);
	  str2 := substr(object_str,pos+mlen,olen-(pos+mlen-1));
	  if ((number >= 0) and (show_sign)) then
	    writev(object_str,str1,'+',number:1,str2)
	  else
	    writev(object_str,str1,number:1,str2);
	end
    end;


	{ Checks to see if user is a wizard			-RAK-	}
[psect(wizard$code)] function check_pswd : boolean;
      var
		i1              : integer;
		x               : char;
		tpw             : packed array [1..12] of char;
      begin
	check_pswd := false;
	i1 := 0;
	tpw := '            ';
	prt('Password : ',1,1);
	repeat
	  inkey(x);
	  case ord(x) of
	    13  :       ;
	    otherwise   begin
			  i1 := i1 + 1;
			  tpw[i1] := x;
			end
	  end;
	until ((i1 = 12) or (ord(x) = 13));
	if (tpw = password1) then
	  begin
	    wizard1 := true;
	    check_pswd := true;
	  end
	else if (tpw = password2) then
	  begin
	    wizard1 := true;
	    wizard2 := true;
	    check_pswd := true;
	  end;
	msg_flag := false;
	erase_line(msg_line,msg_line);
      end;


	{ Weapon weight VS strength and dexterity		-RAK-	}
[psect(moria$code)] function attack_blows(
			weight          :       integer;
			var wtohit      :       integer
					) : integer;
      var
	adj_weight,blows                : integer;
      begin
	blows  := 1;
	wtohit := 0;
	with py.stat do
	  begin
	    if ((cstr*15) < weight) then
	      wtohit := -weight
	    else
	      begin
		if      (cdex <  10) then blows := 1
		else if (cdex <  19) then blows := 2
		else if (cdex <  68) then blows := 3
		else if (cdex < 108) then blows := 4
		else if (cdex < 118) then blows := 4
		else                      blows := 5;
		adj_weight := trunc((cstr*10)/weight);
		if      (adj_weight < 2) then blows := 1
		else if (adj_weight < 3) then blows := trunc(blows/3.0)
		else if (adj_weight < 4) then blows := trunc(blows/2.5)
		else if (adj_weight < 5) then blows := trunc(blows/2.25)
		else if (adj_weight < 7) then blows := trunc(blows/2.00)
		else if (adj_weight < 9) then blows := trunc(blows/1.75)
		else                          blows := trunc(blows/1.50);
	      end;
	  end;
	attack_blows := blows;
      end;


	{ Critical hits, Nasty way to die...			-RAK-	}
[psect(moria$code)] function critical_blow(
			weight,plus,dam :       integer
					  ) : integer;
      begin
	critical_blow := dam;
	{ Weight of weapon, pluses to hit, and character level all      }
	{ contirbute to the chance of a critical                        }
	if (randint(5000) <= (weight+5*plus+3*py.misc.lev)) then
	  begin
	    weight := weight + randint(650);
	    if      (weight < 400) then
	      begin
		critical_blow := 2*dam + 5;
		msg_print('It was a good hit! (x2 damage)');
	      end
	    else if (weight < 700) then
	      begin
		critical_blow := 3*dam + 10;
		msg_print('It was an excellent hit! (x3 damage)');
	      end
	    else if (weight < 900) then
	      begin
		critical_blow := 4*dam + 15;
		msg_print('It was a superb hit! (x4 damage)');
	      end
	    else
	      begin
		critical_blow := 5*dam + 20;
		msg_print('It was a *GREAT* hit! (x5 damage)');
	      end
	  end;
      end;



	{ Given direction 'dir', returns new row, column location -RAK- }
[psect(misc1$code)] function move(dir : integer; var y,x : integer) : boolean;
      var
		new_row,new_col         : integer;
      begin
	case dir of
	  1 : begin
		new_row := y + 1;
		new_col := x - 1;
	      end;
	  2 : begin
		new_row := y + 1;
		new_col := x;
	      end;
	  3 : begin
		new_row := y + 1;
		new_col := x + 1;
	      end;
	  4 : begin
		new_row := y;
		new_col := x - 1;
	      end;
	  5 : begin
		new_row := y;
		new_col := x;
	      end;
	  6 : begin
		new_row := y;
		new_col := x + 1;
	      end;
	  7 : begin
		new_row := y - 1;
		new_col := x - 1;
	      end;
	  8 : begin
		new_row := y - 1;
		new_col := x;
	      end;
	  9 : begin
		new_row := y - 1;
		new_col := x + 1;
	      end;
	end;
	move := false;
	if ((new_row >= 1) and (new_row <= cur_height)) then
	  if ((new_col >= 1) and (new_col <= cur_width)) then
	    begin
	      y := new_row;
	      x := new_col;
	      move := true;
	    end
      end;


	{ Saving throws for player character... 		-RAK-	}
[psect(moria$code)] function player_saves(adjust : integer) : boolean;
	begin
	  if (randint(100) <= (py.misc.save + adjust)) then
	    player_saves := true
	  else
	    player_saves := false;
	end;


	{ Init players with some belongings			-RAK-	}
[psect(setup$code)] procedure char_inven_init;
      var
	i1,i2,dummy             : integer;
      begin
	for i1 := 1 to 5 do
	  begin
	    i2 := player_init[py.misc.pclass,i1];
	    inventory[inven_max] := inventory_init[i2];
	    inven_carry(dummy);
	  end;
      end;
