	{ Creatures movement and attacking are done from here	-RAK-	}
  procedure creatures;
      var
		i1,i2,i3,moldy,moldx	: integer;


	{ Updates screen when monsters move about		-RAK-	}
      procedure update_mon(monptr : integer);
	var
		flag		: boolean;
	begin
	  with m_list[monptr] do
	    with cave[fy,fx] do
	      begin
		flag := false;
		if (cdis <= max_sight) then
		  if (py.flags.blind < 1) then
		    if (panel_contains(fy,fx)) then
		      if (los(char_row,char_col,fy,fx)) then
			with c_list[mptr] do
			  begin
	{ Normal sight...	}
			    if ((pl) or (tl)) then
			      begin
			        if (py.flags.see_inv) then
			          flag := true
			        else if (uand(%X'10000',cmove) = 0) then
			          flag := true;
			      end
	{ Infra vision...	}
			      else if (py.flags.see_infra > 0) then
				if (cdis <= py.flags.see_infra) then
				  if (uand(%X'2000',cdefense) <> 0) then
			            flag := true;
			  end;
	{ Light it up...	}
		if (flag) then
		  begin
		    if (not(ml)) then
		      begin
			print(c_list[mptr].cchar,fy,fx);
			ml := true;
			if (search_flag) then
			  search_off;
			if (py.flags.rest > 0) then
			  rest_off;
			flush;
			if (find_flag) then
			  begin
			    find_flag := false;
			    move_char(5);
			  end;
		      end;
		  end
	{ Turn it off...	}
		else if (ml) then
		  begin
		    ml := false;
		    if ((tl) or (pl)) then
		      lite_spot(fy,fx)
		    else
		      unlite_spot(fy,fx);
		  end;
	      end;
	end;


	{ Move the criiters about the dungeon			-RAK-	}
      function mon_move(monptr : integer) : boolean;
        type
		mm_type = array [1..5] of integer;
        var
		i1,i2,i3		: integer;
		mm			: mm_type;
		out_val			: vtype;
		move_test		: boolean;

	{ Choose correct directions for monster movement	-RAK-	}
        procedure get_moves(monptr : integer; var mm : mm_type);
	  var
		y,ay,x,ax,move_val	: integer;
	  begin
	    y := m_list[monptr].fy - char_row;
	    x := m_list[monptr].fx - char_col;
	    ay := abs(y);
	    ax := abs(x);
	    if (y < 0) then
	      move_val := 8
	    else
	      move_val := 0;
	    if (x > 0) then
	      move_val := move_val + 4;
	    if (ay > (ax*1.7321)) then
	      move_val := move_val + 2
	    else if (ax > (ay*1.7321)) then
	      move_val := move_val + 1;
	    case move_val of
	      0     :	begin
			  mm[1] := 9;
			  if (ay > ax) then
			    begin
			      mm[2] := 8;
			      mm[3] := 6;
			      mm[4] := 7;
			      mm[5] := 3;
			    end
			  else
			    begin
			      mm[2] := 6;
			      mm[3] := 8;
			      mm[4] := 3;
			      mm[5] := 7;
			    end
			end;
	      1,9   :	begin
			  mm[1] := 6;
			  if (y < 0) then
			    begin
			      mm[2] := 3;
			      mm[3] := 9;
			      mm[4] := 2;
			      mm[5] := 8;
			    end
			  else
			    begin
			      mm[2] := 9;
			      mm[3] := 3;
			      mm[4] := 8;
			      mm[5] := 2;
			    end
			end;
	      2,6   :	begin
			  mm[1] := 8;
			  if (x < 0) then
			    begin
			      mm[2] := 9;
			      mm[3] := 7;
			      mm[4] := 6;
			      mm[5] := 4;
			    end
			  else
			    begin
			      mm[2] := 7;
			      mm[3] := 9;
			      mm[4] := 4;
			      mm[5] := 6;
			    end
			end;
	      4     :	begin
			  mm[1] := 7;
			  if (ay > ax) then
			    begin
			      mm[2] := 8;
			      mm[3] := 4;
			      mm[4] := 9;
			      mm[5] := 1;
			    end
			  else
			    begin
			      mm[2] := 4;
			      mm[3] := 8;
			      mm[4] := 1;
			      mm[5] := 9;
			    end
			end;
	      5,13  :	begin
			  mm[1] := 4;
			  if (y < 0) then
			    begin
			      mm[2] := 1;
			      mm[3] := 7;
			      mm[4] := 8;
			      mm[5] := 2;
			    end
			  else
			    begin
			      mm[2] := 7;
			      mm[3] := 1;
			      mm[4] := 2;
			      mm[5] := 8;
			    end
			end;
	      8     :	begin
			  mm[1] := 3;
			  if (ay > ax) then
			    begin
			      mm[2] := 2;
			      mm[3] := 6;
			      mm[4] := 1;
			      mm[5] := 9;
			    end
			  else
			    begin
			      mm[2] := 6;
			      mm[3] := 2;
			      mm[4] := 9;
			      mm[5] := 1;
			    end
			end;
	      10,14 :	begin
			  mm[1] := 2;
			  if (x < 0) then
			    begin
			      mm[2] := 1;
			      mm[3] := 3;
			      mm[4] := 4;
			      mm[5] := 6;
			    end
			  else
			    begin
			      mm[2] := 3;
			      mm[3] := 1;
			      mm[4] := 6;
			      mm[5] := 4;
			    end
			end;
	      12    :	begin
			  mm[1] := 1;
			  if (ay > ax) then
			    begin
			      mm[2] := 2;
			      mm[3] := 4;
			      mm[4] := 3;
			      mm[5] := 7;
			    end
			  else
			    begin
			      mm[2] := 4;
			      mm[3] := 2;
			      mm[4] := 7;
			      mm[5] := 3;
			    end
			end
	    end
          end;


	{ Make an attack on the player (chuckle...)		-RAK-	}
        procedure make_attack(monptr : integer);
          var
		xpos,atype,adesc,dam	: integer;
		i1,i2,i3,i4		: integer;
		attstr,attx		: vtype;
		cdesc,mdesc,ddesc	: vtype;
		damstr			: dtype;
		flag			: boolean;
	  begin
	{ Cancel 'run' mode if attacking so the poor player		}
	{ Won't be forced to continue running in corridors	-RHM-	}
	    find_flag := false;
	{ Cancel 'SEARCH' mode too...				-RHM-	}
	    if search_flag then search_off;
	    with m_list[monptr] do
	    with c_list[mptr] do
	      begin
	        attstr := damage;
		if ((uand(%X'10000',cmove) <> 0) and 
		    (not(py.flags.see_inv)))then
		  cdesc := 'It '
		else if (py.flags.blind > 0) then
		  cdesc := 'It '
		else if (not(ml)) then
		  cdesc := 'It '
		else
		  cdesc := 'The ' + name + ' ';
		{ For 'DIED_FROM' string	}
		if (uand(%X'80000000',cmove) <> 0) then
		  ddesc := 'The ' + name
		else
		  ddesc := '& ' + name;
		inventory[inven_max].name   := ddesc;
		inventory[inven_max].number := 1;
		objdes(ddesc,inven_max,true);
		{ End DIED_FROM			}
	        while (length(attstr) > 0) do
	          begin
	            xpos := index(attstr,'|');
	            if (xpos > 0) then
		      begin
		        attx := substr(attstr,1,xpos-1);
		        attstr := substr(attstr,xpos+1,length(attstr)-xpos);
		      end
	            else
		      begin
		        attx := attstr;
		        attstr := '';
		      end;
	            readv(attx,atype,adesc,damstr);
		    flag := false;
		    if (py.flags.protevil > 0) then
		      if (uand(cdefense,%X'0004') <> 0) then
		        if ((py.misc.lev+1) > level) then
			  begin
			    atype := 99;
			    adesc := 99;
			  end;
		    with py.misc do
	            case atype of
{Normal attack	}     1  :	if (test_hit(60,level,0,pac+ptoac)) then
				  flag := true;
{Poison Strength}     2  :	if (test_hit(0,level,0,pac+ptoac)) then
				  flag := true;
{Confusion attack}    3  :	if (test_hit(10,level,0,pac+ptoac)) then
				  flag := true;
{Fear attack	}     4  :	if (test_hit(10,level,0,pac+ptoac)) then
				  flag := true;
{Fire attack	}     5  :	if (test_hit(10,level,0,pac+ptoac)) then
				  flag := true;
{Acid attack	}     6  :	if (test_hit(0,level,0,pac+ptoac)) then
				  flag := true;
{Cold attack	}     7  :	if (test_hit(10,level,0,pac+ptoac)) then
				  flag := true;
{Lightning attack}    8  :	if (test_hit(10,level,0,pac+ptoac)) then
				  flag := true;
{Corrosion attack}    9  :	if (test_hit(0,level,0,pac+ptoac)) then
				  flag := true;
{Blindness attack}    10 :	if (test_hit(0,level,0,pac+ptoac)) then
				  flag := true;
{Paralysis attack}    11 :	if (test_hit(4,level,0,pac+ptoac)) then
				  flag := true;
{Steal Money	}     12 :	if (test_hit(8,level,0,py.misc.lev)) then
				  if (py.misc.au > 0) then
				    flag := true;
{Steal Object	}     13 :	if (test_hit(5,level,0,py.misc.lev)) then
				  if (inven_ctr > 0) then
				    flag := true;
{Poison		}     14 :	if (test_hit(10,level,0,pac+ptoac)) then
				  flag := true;
{Lose dexterity}      15 :	if (test_hit(0,level,0,pac+ptoac)) then
				  flag := true;
{Lose constitution}   16 :	if (test_hit(0,level,0,pac+ptoac)) then
				  flag := true;
{Lose intelligence}   17 :	if (test_hit(0,level,0,pac+ptoac)) then
				  flag := true;
{Lose wisdom}         18 :	if (test_hit(0,level,0,pac+ptoac)) then
				  flag := true;
{Lose experience}     19 :	if (test_hit(3,level,0,pac+ptoac)) then
				  flag := true;
{Aggravate monsters}  20 :	flag := true;
{Disenchant	   }  21 :	if (test_hit(5,level,0,pac+ptoac)) then
				  flag := true;
{Eat food	   }  22 :	if (test_hit(0,level,0,pac+ptoac)) then
				  flag := true;
{Eat light	   }  23 :	if (test_hit(0,level,0,pac+ptoac)) then
				  flag := true;
		      99 :	flag := true;
	              otherwise ;
	            end;
		    if (flag) then
		      begin
		        case adesc of
		          1 : msg_print(cdesc + 'hits you.');
		          2 : msg_print(cdesc + 'bites you.');
		          3 : msg_print(cdesc + 'claws you.');
		          4 : msg_print(cdesc + 'stings you.');
		          5 : msg_print(cdesc + 'touches you.');
		          6 : msg_print(cdesc + 'kicks you.');
		          7 : msg_print(cdesc + 'gazes at you.');
		          8 : msg_print(cdesc + 'breathes on you.');
			  9 : msg_print(cdesc + 'spits on you.');
			 10 : msg_print(cdesc + 'makes a horrible wail.');
			 11 : msg_print(cdesc + 'embraces you.');
			 12 : msg_print(cdesc + 'crawls on you.');
			 13 : msg_print(cdesc + 'releases a cloud of spores.');
			 14 : msg_print(cdesc + 'begs you for money.');
			 15 : msg_print('You''ve been slimed!');
			 16 : msg_print(cdesc + 'crushes you.');
			 17 : msg_print(cdesc + 'tramples you.');
			 18 : msg_print(cdesc + 'drools on you.');
			 99 : msg_print(cdesc + 'is repelled.');
		          otherwise ;
		        end;
			case atype of
{Normal attack	}         1  :	begin
				  dam := damroll(damstr);
				  with py.misc do
				    dam :=dam - round(((pac+ptoac)/100.0)*dam);
				  take_hit(dam,ddesc);
				  prt_chp;
				end;
{Poison Strength}         2  :	begin
				  take_hit(damroll(damstr),ddesc);
				  if (py.flags.sustain_str) then
		msg_print('You feel weaker for a moment, then it passes.')
				    else if (randint(2) = 1) then
				      begin
					msg_print('You feel weaker.');
				        py.stat.cstr := de_statp(py.stat.cstr);
				        prt_strength;
				      end;
				  prt_chp;
				end;
{Confusion attack}        3  :	with py.flags do
				  begin
				    take_hit(damroll(damstr),ddesc);
				    if (randint(2) = 1) then
				      begin
				        if (confused < 1) then
				          begin
					    msg_print('You feel confused.');
				            confused:=confused+randint(level);
				          end;
				        confused := confused + 3;
				      end;
				    prt_chp;
				  end;
{Fear attack	}         4  :	with py.flags do
				  begin
				    take_hit(damroll(damstr),ddesc);
				    if (player_saves(wis_adj)) then
				      msg_print('You resist the effects!')
				    else if (afraid < 1) then
				      begin
					msg_print('You are suddenly afraid!');
				        afraid := afraid + 3 + randint(level);
				      end
				    else
				      afraid := afraid + 3;
				    prt_chp;
				  end;
{Fire attack	}         5  :	begin
				  msg_print('You are enveloped in flames!');
				  fire_dam(damroll(damstr),ddesc);
				end;
{Acid attack	}         6  :	begin
				  msg_print('You are covered in acid!');
				  acid_dam(damroll(damstr),ddesc);
				end;
{Cold attack	}         7  :	begin
				  msg_print('You are covered with frost!');
				  cold_dam(damroll(damstr),ddesc);
				end;
{Lightning attack}        8  :	begin
				  msg_print('Lightning strikes you!');
				  light_dam(damroll(damstr),ddesc);
				end;
{Corrosion attack}        9  :	begin
			msg_print('A stinging red gas swirls about you.');
				  corrode_gas(ddesc);
				  take_hit(damroll(damstr),ddesc);
				  prt_chp;
				end;
{Blindness attack}        10 :	with py.flags do
				  begin
				    take_hit(damroll(damstr),ddesc);
				    if (blind < 1) then
				      begin
				        blind := blind + 10 + randint(level);
					msg_print('Your eyes begin to sting.');
					msg_print(' ');
				      end;
				    blind := blind + 5;
				    prt_chp;
				  end;
{Paralysis attack}        11 :	with py.flags do
				  begin
				    take_hit(damroll(damstr),ddesc);
				    if (player_saves(con_adj)) then
				      msg_print('You resist the effects!')
				    else if (paralysis < 1) then
				      begin
				        if (free_act) then
					  msg_print('You are unaffected.')
				        else
					  begin
					    paralysis:=randint(level) + 3;
				            msg_print('You are paralyzed.');
				          end;
				      end;
				    prt_chp;
				  end;
{Steal Money	 }        12 :	with py.misc do
				  begin
				    if ((randint(124) < py.stat.cdex) and
				        (py.flags.paralysis < 1)) then
		msg_print('You quickly protect your money pouch!')
				    else
				      begin
				        i1 := trunc(au/10) + randint(25);
				        if (i1 > au) then
				          au := 0
				        else
				          au := au - i1;
				        msg_print('Your purse feels lighter.');
				        prt_gold;
				      end;
				    if (randint(2) = 1) then
				      begin
				        msg_print('There is a puff of smoke!');
				        teleport_away(monptr,max_sight);
				      end;
				  end;
{Steal Object	}         13 :	with py.stat do
				  begin
				    if ((randint(124) < py.stat.cdex) and
				        (py.flags.paralysis < 1)) then
		msg_print('You grab hold of your backpack!')
				    else
				      begin
					i1 := randint(inven_ctr);
					inven_destroy(i1);
				msg_print('Your backpack feels lighter.');
				      end;
				    if (randint(2) = 1) then
				      begin
				        msg_print('There is a puff of smoke!');
				        teleport_away(monptr,max_sight);
				      end;
				  end;
{Poison		}	  14 :	with py.flags do
				  begin
				    take_hit(damroll(damstr),ddesc);
				    prt_chp;
				    msg_print('You feel very sick.');
				    poisoned := poisoned+randint(level)+5;
				  end;
{Lose dexterity }         15 :	with py.flags do
				  begin
				    take_hit(damroll(damstr),ddesc);
				    if (sustain_dex) then
		msg_print('You feel clumsy for a moment, then it passes.')
				    else
				      begin
					msg_print('You feel more clumsy.');
				        py.stat.cdex := de_statp(py.stat.cdex);
				        prt_dexterity;
				      end;
				    prt_chp;
				  end;
{Lose constitution }      16 :	with py.flags do
				  begin
				    take_hit(damroll(damstr),ddesc);
				    if (sustain_con) then
		msg_print('Your body resists the effects of the disease.')
				    else
				      begin
					msg_print('Your health is damaged!');
				        py.stat.ccon := de_statp(py.stat.ccon);
				        prt_constitution;
				      end;
				    prt_chp;
				  end;
{Lose intelligence }      17 :	with py.flags do
				  begin
				    take_hit(damroll(damstr),ddesc);
				    if (sustain_int) then
				      begin
		msg_print('You feel your memories fading...');
		msg_print('Your memories are suddenly restored!');
				      end
				    else
				      begin
		msg_print('You feel your memories fading...');
				        py.stat.cint:=de_statp(py.stat.cint);
				        prt_intelligence;
				      end;
				    prt_chp;
				  end;
{Lose wisdom      }       18 :	with py.flags do
				  begin
				    take_hit(damroll(damstr),ddesc);
				    if (sustain_wis) then
				      msg_print('Your wisdom is sustained.')
				    else
				      begin
					msg_print('Your wisdom is drained.');
				        py.stat.cwis := de_statp(py.stat.cwis);
				        prt_wisdom;
				      end;
				    prt_chp;
				  end;
{Lose experience  }       19:	begin
				msg_print('You feel your life draining away!');
				  lose_exp(max_hp(damstr));
				end;
{Aggravate monster}	  20:	aggravate_monster(5);
{Disenchant	  }	  21:	begin
				  flag := false;
				  case randint(6) of
				    1 : i1 := 23;
				    2 : i1 := 26; 
				    3 : i1 := 27;
				    4 : i1 := 32;
				    5 : i1 := 28;
				    6 : i1 := 24;
				  end;
				  with inventory[i1] do
				    begin
				      if (tohit > 0) then
					begin
					  tohit := tohit - randint(2);
					  flag := true;
					end;
				      if (todam > 0) then
					begin
					  todam := todam - randint(2);
					  flag := true;
					end;
				      if (toac > 0) then
					begin
					  toac  := toac  - randint(2);
					  flag := true;
					end;
				    end;
				  if (flag) then
				    begin
		    msg_print('There is a static feeling in the air...');
				      py_bonuses(blank_treasure,1);
				    end;
				end;
{Eat food	  }	  22:	begin
				  if (find_range([80],i1,i2)) then
				    inven_destroy(i1);
				end;
{Eat light	  }	  23:	begin
				  with inventory[33] do
				    if (p1 > 0) then
				      begin
					p1 := p1 - 250 - randint(250);
					if (p1 < 1) then p1 := 1;
					msg_print('Your light dims...');
				      end;
				end;
			  99:	;
			  otherwise ;
	            	end
		      end
		    else
		      case adesc of
			  1,2,3,6  : msg_print(cdesc + 'misses you.');
			  otherwise ;
		      end
	          end
	      end
	  end;


	{ Make the move if possible, five choices		-RAK-	}
        function make_move(monptr : integer; mm : mm_type) : boolean;
	  var
		i1,i2,newy,newx		: integer;
		movebits		: unsigned;
		flag,tflag		: boolean;
	  begin
	    i1 := 1;
	    flag := false;
	    make_move := false;
	    movebits := c_list[m_list[monptr].mptr].cmove;
	    repeat
		{ Get new positon		}
	      newy := m_list[monptr].fy;
	      newx := m_list[monptr].fx;
	      move(mm[i1],newy,newx);
	      with cave[newy,newx] do
	        if (fval <> 15) then
		  begin
		    tflag := false;
		{ Floor is open?		}
		    if (fopen) then
		      tflag := true
		{ Creature moves through walls?	}
		    else if (uand(movebits,%X'40000') <> 0) then
		      tflag := true
		{ Creature can open doors?	}
		    else if (tptr > 0) then
	with t_list[tptr] do
	  with m_list[monptr] do
	    if (uand(movebits,%X'20000') <> 0) then
	      begin	{ Creature can open doors...			}
		case tval of
		  105 : begin	{ Closed doors...	}
			  if (p1 = 0) then		{ Closed doors	}
			    begin
			      tflag := true;
			      if (fm) then
				if (los(char_row,char_col,newy,newx)) then
				  begin
				    t_list[tptr] := door_list[1];
				    fopen := true;
				    lite_spot(newy,newx);
				    tflag := false;
				  end;
			    end
			  else if (p1 > 0) then		{ Locked doors	}
			    begin
			      if (randint(100-level) < 5) then
				p1 := 0;
			    end
			  else if (p1 < 0) then		{ Stuck doors	}
			    begin
			      if (randint(hp) > (10+abs(p1))) then
				p1 := 0;
			    end;
			end;
		  109 : begin	{ Secret doors...	}
			  tflag := true;
			  if (fm) then
			    if (los(char_row,char_col,newy,newx)) then
			      begin
				t_list[tptr] := door_list[1];
				fopen := true;
				lite_spot(newy,newx);
				tflag := false;
			      end;
			end;
		  otherwise ;
		end;
	      end
	    else
	      begin	{ Creature can not open doors, must bash them	}
		case tval of
		  105 : begin	{ Closed doors...	}
			  i2 := abs(p1) + 20;
			  if (randint(hp) > i2) then
			    begin
			      tflag := true;
			      if (fm) then
			        if (los(char_row,char_col,newy,newx)) then
				  begin
				    t_list[tptr] := door_list[1];
				    t_list[tptr].p1 := randint(2) - 1;
				    fopen := true;
				    lite_spot(newy,newx);
				    tflag := false;
				  end;
			    end
			end;
		  109 : ;	{ Secret doors...	}
		  otherwise ;
		end;
	      end;
		{ Glyph of warding present?	}
		    if (tflag) then		{ Scare Monster trap	}
		      if (tptr > 0) then
			if (t_list[tptr].tval = 102) then
			  if (t_list[tptr].subval = 99) then
			    tflag := false;
		{ Creature has attempted to move on player?	}
		    if (tflag) then
		      if (cptr = 1) then
		        begin
			  if (not (m_list[monptr].ml)) then
			    update_mon(monptr);
			  if (find_flag) then
			    begin
			      find_flag := false;
			      move_char(5);
			    end;
		          make_attack(monptr);
		{ Player has read a Confuse Monster?	}
		{ Monster gets a saving throw...	}
			  if (py.flags.confuse_monster) then
			    with m_list[monptr] do
			      with c_list[mptr] do
			        begin
			          msg_print('Your hands stop glowing.');
				  if ((randint(max_mons_level) < level) or
				      (uand(%X'1000',cdefense) <> 0)) then
		msg_print('The ' + name + ' is unaffected.')
				  else
				    begin
		msg_print('The ' + name + ' appears confused.');
				      confused := true;
				      py.flags.confuse_monster := false;
				    end;
			        end;
			  tflag := false;
			  flag  := true;
		        end
		{ Creature is attempting to move on other creature?	}
		      else if ((cptr > 1) and 
			((newy <> m_list[monptr].fy) or 
			 (newx <> m_list[monptr].fx))) then
		        begin
		{ Creature eats other creatures?	}
		          if (uand(movebits,%X'80000') <> 0) then
			    delete_monster(cptr)
			  else
			    tflag := false;
		        end;
		{ Creature has been allowed move...	}
		    if (tflag) then
		      with m_list[monptr] do
		        begin
		{ Pick up or eat an object		}
			  if (uand(movebits,%X'100000') <> 0) then
			    with cave[newy,newx] do
			      if (tptr > 0) then
			        if (t_list[tptr].tval < 100) then
			          delete_object(newy,newx);
		{ Move creature record			}
		          move_rec(fy,fx,newy,newx);
		          fy := newy;
		          fx := newx;
			  flag := true;
			  make_move := true;
		        end
		  end;
	      i1 := i1 + 1;
		{ Up to 5 attempts at moving, then give up...	}
	    until ((flag) or (i1 > 5));
	  end;


	{ Creatures can cast spells too.  (Dragon Breath)	-RAK-	}
	{ cast_spell := true if creature changes position	}
	{ took_turn  := true if creature casts a spell		}
	function cast_spell(	monptr 		: integer; 
				var took_turn 	: boolean) : boolean;
	  var
		i1			: unsigned;
		i2,i3,y,x		: integer;
		chance,thrown_spell	: integer;
		spell_choice		: array [1..31] of integer;
		cdesc,ddesc		: vtype;
		flag			: boolean;
	  begin
	    with m_list[monptr] do
	      with c_list[mptr] do
		begin
		  chance := int(uand(spells,%X'0000000F'));
		{ 1 in x chance of casting spell		}
		  if (randint(chance) <> 1) then
		    begin
		      cast_spell := false;
		      took_turn  := false;
		    end
		{ Must be within certain range			}
		  else if (cdis > max_spell_dis) then
		    begin
		      cast_spell := false;
		      took_turn  := false;
		    end
		{ Must have unobstructed Line-Of-Sight		}
		  else if (not(los(char_row,char_col,fy,fx))) then
		    begin
		      cast_spell := false;
		      took_turn  := false;
		    end
		  else	{ Creature is going to cast a spell	}
		    begin
		      took_turn  := true;
		      cast_spell := false;
		{ Describe the attack				}
		      flag := true;
		      if (not(ml)) then
			flag := false
		      else if ((uand(%X'10000',c_list[mptr].cmove) <> 0) and
			  (not(py.flags.see_inv))) then
			flag := false
		      else if (py.flags.blind > 0) then
			flag := false;
		      if (flag) then
		        cdesc := 'The ' + name + ' '
		      else
			cdesc := 'It ';
		      { For 'DIED_FROM' string	}
		      if (uand(%X'80000000',cmove) <> 0) then
		        ddesc := 'The ' + name
		      else
		        ddesc := '& ' + name;
		      inventory[inven_max].name   := ddesc;
		      inventory[inven_max].number := 1;
		      objdes(ddesc,inven_max,true);
		{ End DIED_FROM			}
		{ Extract all possible spells into spell_choice	}
		      i1 := uand(spells,%X'FFFFFFF0');
		      i3 := 0;
		      while (i1 <> 0) do
			begin
			  i2 := bit_pos(i1);
			  i3 := i3 + 1;
			  spell_choice[i3] := i2;
			end;
		{ Choose a spell to cast			}
		      thrown_spell := spell_choice[randint(i3)];
		{ Cast the spell...				}
		      case thrown_spell of
{Teleport Short} 5 :	teleport_away(monptr,4);
{Teleport Long } 6 :	teleport_away(monptr,max_sight);
{Teleport To   } 7 :	begin
			  cdesc := cdesc + 'casts a spell.';
			  msg_print(cdesc);
			  msg_print(' ');
			  teleport_to(fy,fx);
			end;
{Light Wound   } 8 :	begin
			  cdesc := cdesc + 'casts a spell.';
			  msg_print(cdesc);
			  if (player_saves(wis_adj+py.misc.lev)) then
			    msg_print('You resist the affects of the spell.')
			  else
			    take_hit(damroll('2d8'),ddesc);
			end;
{Serious Wound } 9 :	begin
			  cdesc := cdesc + 'casts a spell.';
			  msg_print(cdesc);
			  if (player_saves(wis_adj+py.misc.lev)) then
			    msg_print('You resist the affects of the spell.')
			  else
			    take_hit(damroll('8d8'),ddesc);
			end;
{Hold Person   }10 :	begin
			  cdesc := cdesc + 'casts a spell.';
			  msg_print(cdesc);
			  if (py.flags.free_act) then
			    msg_print('You are unaffected...')
			  else if (player_saves(wis_adj+py.misc.lev)) then
			    msg_print('You resist the affects of the spell.')
			  else if (py.flags.paralysis > 0) then
			    py.flags.paralysis:=py.flags.paralysis+2
			  else
			    py.flags.paralysis:=randint(5)+4;
			end;
{Cause Blindnes}11 :	begin
			  cdesc := cdesc + 'casts a spell.';
			  msg_print(cdesc);
			  if (player_saves(wis_adj+py.misc.lev)) then
			    msg_print('You resist the affects of the spell.')
			  else if (py.flags.blind > 0) then
 			    py.flags.blind := py.flags.blind + 6
			  else
			    begin
			      py.flags.blind := py.flags.blind+12+randint(3);
			      msg_print(' ');
			    end;
			end;
{Cause Confuse }12 :	begin
			  cdesc := cdesc + 'casts a spell.';
			  msg_print(cdesc);
			  if (player_saves(wis_adj+py.misc.lev)) then
			    msg_print('You resist the affects of the spell.')
			  else if (py.flags.confused > 0) then
			    py.flags.confused := py.flags.confused + 2
			  else
			    py.flags.confused := randint(5) + 3;
			end;			  
{Cause Fear    }13 :	begin
			  cdesc := cdesc + 'casts a spell.';
			  msg_print(cdesc);
			  if (player_saves(wis_adj+py.misc.lev)) then
			    msg_print('You resist the affects of the spell.')
			  else if (py.flags.afraid > 0) then
			    py.flags.afraid := py.flags.afraid + 2
			  else
			    py.flags.afraid := randint(5) + 3;
			end;			  
{Summon Monster}14 :	begin
			  cdesc := cdesc + 'magically summons a monster!';
			  msg_print(cdesc);
			  y := char_row;	
			  x := char_col;
			  summon_monster(y,x,false);
			  check_mon_lite(y,x);
			end;
{Summon Undead} 15 :	begin
			  cdesc := cdesc + 'magically summons an undead!';
			  msg_print(cdesc);
			  y := char_row;	
			  x := char_col;
			  summon_undead(y,x);
			  check_mon_lite(y,x);
			end;
{Slow Person  } 16 :	begin
			  cdesc := cdesc + 'casts a spell.';
			  msg_print(cdesc);
			  if (py.flags.free_act) then
			    msg_print('You are unaffected...')
			  else if (player_saves(wis_adj+py.misc.lev)) then
			    msg_print('You resist the affects of the spell.')
			  else if (py.flags.slow > 0) then
			    py.flags.slow := py.flags.slow + 2
			  else
			    py.flags.slow := randint(5) + 3;
			end;			  
{Breath Light } 20 :	begin
			  cdesc := cdesc + 'breathes lightning.';
			  msg_print(cdesc);
			  breath(1,char_row,char_col,trunc(hp/3.0),ddesc);
			end;
{Breath Gas   } 21 :	begin
			  cdesc := cdesc + 'breathes gas.';
			  msg_print(cdesc);
			  breath(2,char_row,char_col,trunc(hp/3.0),ddesc);
			end;
{Breath Acid  } 22 :	begin
			  cdesc := cdesc + 'breathes acid.';
			  msg_print(cdesc);
			  breath(3,char_row,char_col,trunc(hp/3.0),ddesc);
			end;
{Breath Frost } 23 :	begin
			  cdesc := cdesc + 'breathes frost.';
			  msg_print(cdesc);
			  breath(4,char_row,char_col,trunc(hp/3.0),ddesc);
			end;
{Breath Fire  } 24 :	begin
			  cdesc := cdesc + 'breathes fire.';
			  msg_print(cdesc);
			  breath(5,char_row,char_col,trunc(hp/3.0),ddesc);
			end;
		otherwise begin
			    msg_print('Creature cast unknown spell.');
			    cdesc := '';
			  end;
		      end;
		{ End of spells					}
		    end;
		end;
	  end;


	{ Main procedure for monster movement (MON_MOVE)	-RAK-	}
        begin
	  mon_move := false;
	  with c_list[m_list[monptr].mptr] do
	    begin
		{ Does the critter multiply?				}
	      if (uand(cmove,%X'00200000') <> 0) then
		if (max_mon_mult >= mon_tot_mult) then
		  if ((py.flags.rest mod mon_mult_adj) = 0) then
		    with m_list[monptr] do
		      begin
		        i3 := 0;
		        for i1 := fy-1 to fy+1 do
		          for i2 := fx-1 to fx+1 do
			    if (in_bounds(i1,i2)) then
			      if (cave[i1,i2].cptr > 1) then
			        i3 := i3 + 1;
		        if (i3 < 4) then
		          if (randint(i3*mon_mult_adj) = 1) then
		            multiply_monster(fy,fx,mptr,false);
		      end;
		{ Creature is confused?  Chance it becomes un-confused	}
	      move_test := false;
	      if (m_list[monptr].confused) then
		begin
		  mm[1] := randint(9);
		  mm[2] := randint(9);
		  mm[3] := randint(9);
		  mm[4] := randint(9);
		  mm[5] := randint(9);
		  mon_move := make_move(monptr,mm);
		  if (randint(8) = 1) then
		    m_list[monptr].confused := false;
		  move_test := true;
	        end
		{ Creature may cast a spell				}
	      else if (spells > 0) then
		mon_move := cast_spell(monptr,move_test);
	      if (not(move_test)) then
		begin
		{ 75% random movement					}
	          if ((randint(100) < 75) and
		    (uand(cmove,%X'00000020') <> 0)) then
	            begin
		      mm[1] := randint(9);
		      mm[2] := randint(9);
		      mm[3] := randint(9);
		      mm[4] := randint(9);
		      mm[5] := randint(9);
		      mon_move := make_move(monptr,mm);
	            end
		{ 40% random movement					}
	          else if ((randint(100) < 40) and
		    (uand(cmove,%X'00000010') <> 0))  then
	            begin
		      mm[1] := randint(9);
		      mm[2] := randint(9);
		      mm[3] := randint(9);
		      mm[4] := randint(9);
		      mm[5] := randint(9);
		      mon_move := make_move(monptr,mm);
	            end
		{ 20% random movement					}
	          else if ((randint(100) < 20) and 
		    (uand(cmove,%X'00000008') <> 0))  then
	            begin
		      mm[1] := randint(9);
		      mm[2] := randint(9);
		      mm[3] := randint(9);
		      mm[4] := randint(9);
		      mm[5] := randint(9);
		      mon_move := make_move(monptr,mm);
	            end
		{ Normal movement					}
	          else if (uand(cmove,%X'00000002') <> 0) then
		    begin
		      if (randint(200) = 1) then
		        begin
		          mm[1] := randint(9);
		          mm[2] := randint(9);
		          mm[3] := randint(9);
		          mm[4] := randint(9);
		          mm[5] := randint(9);
		        end
		      else
		        get_moves(monptr,mm);
		      mon_move := make_move(monptr,mm);
		    end
		{ Attack, but don't move				}
	          else if (uand(cmove,%X'00000001') <> 0) then
	            if (m_list[monptr].cdis < 2) then
		      begin
		        get_moves(monptr,mm);
		        mon_move := make_move(monptr,mm);
		      end
		end;
	    end;
        end;


	{ Main procedure for creatures				-RAK-	}
      begin
	if (muptr > 0) then
	  begin
	{ Process the monsters	}
	i1 := muptr;
	repeat
	  with m_list[i1] do
	    begin
	      cdis := distance(char_row,char_col,fy,fx);
	      if (attack) then	{ Attack is argument passed to CREATURE}
		begin
		  i3 := movement_rate(cspeed);
		  if (i3 > 0) then
	            for i2 := 1 to movement_rate(cspeed) do 
		      begin
	                if ((cdis <= c_list[mptr].aaf) or (ml)) then
		          begin
		            if (csleep > 0) then
			      if (py.flags.aggravate) then
			        csleep := 0
			      else if (py.flags.rest < 1) then
		                if (randint(10) > py.misc.stl) then
		                  csleep := csleep - trunc(75.0/cdis);
			      if (stuned > 0) then
			        stuned := stuned - 1;
		            if ((csleep <= 0) and (stuned <= 0)) then
		              begin
		                moldy := fy;
		                moldx := fx;
		                if (mon_move(i1)) then
			          if (ml) then
				    begin
				      ml := false;
			              if (test_light(moldy,moldx)) then
				        lite_spot(moldy,moldx)
				      else
			                unlite_spot(moldy,moldx);
			            end;
		              end;
		          end;
		        update_mon(i1);
		      end
		  else
		    update_mon(i1);
		end
	      else
		update_mon(i1);
	    end;
	  i1 := m_list[i1].nptr;
	until ((i1 = 0) or (moria_flag));
	{ End processing monsters	}
	  end;
      end;
