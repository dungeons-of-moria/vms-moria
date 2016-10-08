	{ Moria game module					-RAK-	}
	{ The code in this section has gone through many revisions, and	}
	{ some of it could stand some more hard work...  -RAK-		}
[psect(moria$code)] procedure dungeon;
    var
	dir_val			: integer;	{ For movement		}
	y,x,moves		: integer;	{ For movement		}
	i1,i2,tmp1		: integer;	{ Temporaries		}
	old_chp,old_cmana	: integer;	{ Detect change		}
	regen_amount		: real;		{ Regenerate hp and mana}
	command			: char;		{ Last command		}
	out_val			: vtype;	{ For messages		}
	tmp_str			: vtype;	{ Temporary		}
	moria_flag		: boolean;	{ Next level when true	}
	reset_flag		: boolean;	{ Do not move creatures	}
	search_flag		: boolean;	{ Player is searching	}
	teleport_flag		: boolean;	{ Handle telport traps	}
	player_light		: boolean;	{ Player carrying light	}
	save_msg_flag		: boolean;	{ Msg flag after INKEY	}


	{ Pre-declaration of MOVE_CHAR				-RAK-	}
[psect(creature$code)] procedure move_char(dir : integer);
	forward;


	{ Pre-declaration of CREATURES				-RAK-	}
    procedure creatures(attack : boolean);
	forward;


	{ Changes stats up or down for magic items		-RAK-	}
    procedure change_stat(var stat : byteint; amount,factor : integer);
      var
		i1,i2,i3	: integer;
      begin
	i2 := amount * factor;
	i3 := abs(amount);
	for i1 := 1 to i3 do
	  if (i2 < 0) then
	    stat := de_statt(stat)
	  else
	    stat := in_statt(stat);
      end;


	{ Changes speed of monsters relative to player		-RAK-	}
	{ Note: When the player is sped up or slowed down, I simply	}
	{       change the speed of all the monsters.  This greatly	}
	{       simplified the logic...					}
    procedure change_speed(num : integer);
      var
		i1			: integer;
      begin
	py.flags.speed := py.flags.speed + num;
	i1 := muptr;
	while (i1 <> 0) do
	  begin
	    m_list[i1].cspeed := m_list[i1].cspeed + num;
	    i1 := m_list[i1].nptr;
	  end;
      end;


	{ Player bonuses					-RAK-	}
	{ When an item is worn or taken off, this re-adjusts the player	}
	{ bonuses.  Factor=1 : wear; Factor=-1 : removed		}
    procedure py_bonuses(tobj : treasure_type; factor : integer);
      var
	item_flags			: unsigned;
	i1,old_dis_ac			: integer;
      begin
	with py.flags do
	  begin
	    if (slow_digest) then
	      food_digested := food_digested + 1;
	    if (regenerate) then
	      food_digested := food_digested - 3;
	    see_inv	:= false;
	    teleport	:= false;
	    free_act	:= false;
	    slow_digest	:= false;
	    aggravate	:= false;
	    sustain_str	:= false;
	    sustain_int	:= false;
	    sustain_wis	:= false;
	    sustain_con	:= false;
	    sustain_dex	:= false;
	    sustain_chr	:= false;
	    fire_resist	:= false;
	    acid_resist	:= false;
	    cold_resist	:= false;
	    regenerate	:= false;
	    lght_resist	:= false;
	    ffall	:= false;
	  end;

	if (uand(%X'00000001',tobj.flags) <> 0) then
	  begin
	    change_stat(py.stat.cstr,tobj.p1,factor);
	    change_stat(py.stat.str,tobj.p1,factor);
	    print_stat := uor(%X'0001',print_stat);
	  end;
	if (uand(%X'00000002',tobj.flags) <> 0) then
	  begin
	    change_stat(py.stat.cdex,tobj.p1,factor);
	    change_stat(py.stat.dex,tobj.p1,factor);
	    print_stat := uor(%X'0002',print_stat);
	  end;
	if (uand(%X'00000004',tobj.flags) <> 0) then
	  begin
	    change_stat(py.stat.ccon,tobj.p1,factor);
	    change_stat(py.stat.con,tobj.p1,factor);
	    print_stat := uor(%X'0004',print_stat);
	  end;
	if (uand(%X'00000008',tobj.flags) <> 0) then
	  begin
	    change_stat(py.stat.cint,tobj.p1,factor);
	    change_stat(py.stat.int,tobj.p1,factor);
	    print_stat := uor(%X'0008',print_stat);
	  end;
	if (uand(%X'00000010',tobj.flags) <> 0) then
	  begin
	    change_stat(py.stat.cwis,tobj.p1,factor);
	    change_stat(py.stat.wis,tobj.p1,factor);
	    print_stat := uor(%X'0010',print_stat);
	  end;
	if (uand(%X'00000020',tobj.flags) <> 0) then
	  begin
	    change_stat(py.stat.cchr,tobj.p1,factor);
	    change_stat(py.stat.chr,tobj.p1,factor);
	    print_stat := uor(%X'0020',print_stat);
	  end;
	if (uand(%X'00000040',tobj.flags) <> 0) then
	  begin
	    py.misc.srh := py.misc.srh + (tobj.p1 * factor);
	    py.misc.fos := py.misc.fos - (tobj.p1 * factor);
	  end;
	if (uand(%X'00000100',tobj.flags) <> 0) then
	  py.misc.stl := py.misc.stl + 2*factor;
	if (uand(%X'00001000',tobj.flags) <> 0) then
	  begin
	    i1 := tobj.p1*factor;
	    change_speed(-i1);
	  end;
	if (uand(%X'08000000',tobj.flags) <> 0) then
	  if (factor > 0) then
	    py.flags.blind := py.flags.blind + 1000;
	if (uand(%X'10000000',tobj.flags) <> 0) then
	  if (factor > 0) then
	    py.flags.afraid := py.flags.afraid + 50;
	if (uand(%X'40000000',tobj.flags) <> 0) then
	  py.flags.see_infra := py.flags.see_infra + (tobj.p1 * factor);
	with py.misc do
	  begin
	    old_dis_ac := dis_ac;
	    ptohit  := tohit_adj;	{ Real To Hit	}
	    ptodam  := todam_adj;	{ Real To Dam	}
	    ptoac   := toac_adj;	{ Real To AC	}
	    pac     := 0;		{ Real AC	}
	    dis_th  := ptohit;	{ Display To Hit	}
	    dis_td  := ptodam;	{ Display To Dam	}
	    dis_ac  := 0;	{ Display To AC		}
	    dis_tac := ptoac;	{ Display AC		}
	    for i1 := 23 to inven_max-2 do
	      with inventory[i1] do
		if (tval > 0) then
		  begin
		    if (uand(%X'80000000',flags) = 0) then
		      begin
			pac    := pac    + ac;
			dis_ac := dis_ac + ac;
		      end;
		    ptohit := ptohit + tohit;
		    ptodam := ptodam + todam;
		    ptoac  := ptoac  + toac;
		    if (index(name,'^') = 0) then
		      begin
			dis_th  := dis_th  + tohit;
			dis_td  := dis_td  + todam;
			dis_tac := dis_tac + toac;
		      end;
		  end;
	    dis_ac := dis_ac + dis_tac;

		{ Add in temporary spell increases	}
	    with py.flags do
	      begin
		if (invuln > 0) then
		  begin
		    pac    := pac    + 100;
		    dis_ac := dis_ac + 100;
		  end;
		if (blessed > 0) then
		  begin
		    pac    := pac    + 2;
		    dis_ac := dis_ac + 2;
		  end;
		if (detect_inv > 0) then
		  see_inv := true;
	      end;

	    if (old_dis_ac <> dis_ac) then
	      print_stat := uor(%X'0040',print_stat);

	    item_flags := 0;
	    for i1 := 23 to inven_max-2 do
	      with inventory[i1] do
		item_flags := uor(item_flags,flags);
	    if (uand(%X'00000080',item_flags) <> 0) then
	      py.flags.slow_digest := true;
	    if (uand(%X'00000200',item_flags) <> 0) then
	      py.flags.aggravate := true;
	    if (uand(%X'00000400',item_flags) <> 0) then
	      py.flags.teleport := true;
	    if (uand(%X'00000800',item_flags) <> 0) then
	      py.flags.regenerate := true;
	    if (uand(%X'00080000',item_flags) <> 0) then
	      py.flags.fire_resist := true;
	    if (uand(%X'00100000',item_flags) <> 0) then
	      py.flags.acid_resist := true;
	    if (uand(%X'00200000',item_flags) <> 0) then
	      py.flags.cold_resist := true;
	    if (uand(%X'00800000',item_flags) <> 0) then
	      py.flags.free_act := true;
	    if (uand(%X'01000000',item_flags) <> 0) then
	      py.flags.see_inv := true;
	    if (uand(%X'02000000',item_flags) <> 0) then
	      py.flags.lght_resist := true;
	    if (uand(%X'04000000',item_flags) <> 0) then
	      py.flags.ffall := true;

	    for i1 := 23 to inven_max-2 do
	      with inventory[i1] do
	        if (uand(%X'00400000',flags) <> 0) then
	          case p1 of
		    1 :	py.flags.sustain_str := true;
		    2 :	py.flags.sustain_int := true;
		    3 :	py.flags.sustain_wis := true;
		    4 :	py.flags.sustain_con := true;
		    5 :	py.flags.sustain_dex := true;
		    6 :	py.flags.sustain_chr := true;
		    otherwise ;
	          end;

	    with py.flags do
	      begin
		if (slow_digest) then
		  food_digested := food_digested - 1;
		if (regenerate) then
		  food_digested := food_digested + 3;
	      end;
	  end;
      end;


	{ Returns a '*' for cursed items, a ')' for normal ones	-RAK-	}
	{ NOTE: '*' returned only if item has been identified...	}
    function cur_char1(item_val : integer) : char;
      begin
	with inventory[item_val] do
	  if (uand(%X'80000000',flags) = 0) then
	    cur_char1 := ')'	{ Not cursed...			}
	  else if (index(name,'^') > 0) then
	    cur_char1 := ')'	{ Cursed, but not identified	}
	  else
	    cur_char1 := '*';	{ Cursed and identified...	}
      end;


	{ Returns a '*' for cursed items, a ')' for normal ones	-RAK-	}
    function cur_char2(item_val : integer) : char;
      begin
	with inventory[item_val] do
	  if (uand(%X'80000000',flags) = 0) then
	    cur_char2 := ')'	{ Not cursed...	}
	  else
	    cur_char2 := '*';	{ Cursed...	}
      end;


	{ Comprehensive function block to handle all inventory	-RAK-	}
	{ and equipment routines.  Five kinds of calls can take place.	}
	{ Note that '?' is a special call for other routines to display	}
	{ only a portion of the inventory, and take no other action.	}
    function inven_command(command : char; r1,r2 : integer) : boolean;
      var
	com_val,scr_state		: integer;
	exit_flag,test_flag		: boolean;


	{ Displays inventory items from r1 to r2	-RAK-	}
      procedure show_inven(r1,r2 : integer);
	var
	  i1					: integer;
	  tmp_val,out_val			: vtype;
	begin
	  if (r1 > 0) then			{ R1 = 0 dummy call	}
	    begin
	      for i1 := r1 to r2 do		{ Print the items	}
	        begin
	          objdes(tmp_val,i1,true);
	          writev(out_val,chr(i1+96),cur_char1(i1),' ',tmp_val);
	          prt(out_val,i1+1,1);
	        end;
	      if (r2 < 22) then prt('',r2+2,1);	{ Clear line after	}
	      scr_state := 1;			{ Set state to 1	}
	    end;
	end;

	{ Displays equipment items from r1 to end	-RAK-	}
      procedure show_equip(r1 : integer);
	var
	  i1,i2				: integer;
	  prt1,prt2,out_val		: vtype;
	begin
	  if (r1 > equip_ctr) then	{ Last item gone		}
	    prt('',equip_ctr+3,1)
	  else if (r1 > 0) then		{ R1 = 0 dummy call		}
	    begin
	      i2 := 0;
	      for i1 := 23 to inven_max-1 do{ Range of equipment	}
	        begin
	          with inventory[i1] do
	            if (tval > 0) then
		      begin
		        i2 := i2 + 1;
		        if (i2 >= r1) then{ Display only given range	}
		          begin
	      		    case i1 of		{ Get position		}
			      23 :	prt1 := ' You are wielding   : ';
			      24 :	prt1 := ' Worn on head       : ';
			      25 :	prt1 := ' Worn around neck   : ';
			      26 :	prt1 := ' Worn on body       : ';
			      27 :	prt1 := ' Worn on arm        : ';
			      28 :	prt1 := ' Worn on hands      : ';
			      29 :	prt1 := ' Worn on right hand : ';
			      30 :	prt1 := ' Worn on left hand  : ';
			      31 :	prt1 := ' Worn on feet       : ';
			      32 :	prt1 := ' Worn about body    : ';
			      33 :	prt1 := ' Light source       : ';
			      34 :	prt1 := ' Secondary weapon   : ';
			      otherwise prt1 := ' Unknown value      : ';
			    end;
			    objdes(prt2,i1,true);
			    writev(out_val,chr(i2+96),cur_char2(i1),
								prt1,prt2);
			    prt(out_val,i2+2,1);
		          end;
		      end;
	        end;
	      if (wizard2) then
		begin
		  {writev(out_val,'Speed Factor is ',py.flags.speed(4:0));
		  prt(out_val,i2+3,1);}	{ Say what speed	}
{		  prt('',i2+4,1);}	{ Clear last line	}
		end
	      else
	        prt('',i2+4,1);	{ Clear last line	}
	      scr_state := 2;	{ Set state of screen	}
	    end;
	end;

	{ Remove item from equipment list		-RAK-	}
      function remove(item_val : integer) : integer;
        var
	  i1,i2,typ				: integer;
	  out_val,prt1,prt2			: vtype;
	  flag					: boolean;
        begin
	  i1 := 0;
	  flag := false;
	  typ  := inventory[item_val].tval;
	  repeat
	    i1 := i1 + 1;
	    with inventory[i1] do
	      if (typ > tval) then
		begin
		  for i2 :=  inven_ctr downto i1 do
		    inventory[i2+1] := inventory[i2];
		  inventory[i1]  := inventory[item_val];
		  inven_ctr      := inven_ctr   + 1;
		  equip_ctr      := equip_ctr   - 1;
		  flag := true;
		end;
	  until (flag);
	  case typ of
	    10,11,12,20,21,22,23,25 : prt1  := 'Was wielding ';
	    15			    : prt1  := 'Light source was ';
	    otherwise		      prt1  := 'Was wearing ';
	  end;
	  objdes(prt2,i1,true);
	  out_val := prt1 + prt2 + ' (' + chr(i1+96) + ')';
	  msg_print(out_val);
	  inventory[item_val] := blank_treasure;
	  if (item_val <> inven_max-1) then	{ For secondary weapon	}
	    py_bonuses(inventory[i1],-1);
	  remove := i1;
        end;

	{ Unwear routine, remove a piece of equipment	-RAK-	}
      procedure unwear;
	var
	  i1,i2,com_val				: integer;
	  exit_flag,test_flag			: boolean;
	  command				: char;
	  out_val				: vtype;
	begin
	  if (scr_state = 1) then
	    begin
	      clear(1,1);
	      show_equip(1);
	    end;
	  exit_flag := false;
	  repeat
	    writev(out_val,'(a-',chr(equip_ctr+96),', * for equipment list,',
		' ^Z to exit) ','Take off which one ?');
	    test_flag := false;
	    msg_print(out_val);
	    repeat
	      inkey(command);
	      com_val := ord(command);
	      case com_val of
		0,3,25,26,27 :	begin
				  test_flag := true;
				  exit_flag := true;
				end;
		42	 :	begin
				  clear(2,1);
				  show_equip(1);
				end;
		otherwise	begin
				  com_val := com_val - 96;
				  if ((com_val >= 1) and 
				      (com_val <= equip_ctr)) then 
				    test_flag := true;
				end;
	      end;
	    until (test_flag);
	    if (not(exit_flag)) then
	      begin
		reset_flag := false;	{ Player turn	}
		i1 := 0;
		i2 := 22;
		repeat
		  i2 := i2 + 1;
		  if (inventory[i2].tval > 0) then
		    i1 := i1 + 1;
		until (i1 = com_val);
		if (uand(%X'80000000',inventory[i2].flags) <> 0) then
		  begin
	            msg_print('Hmmm, it seems to be cursed...');
		    com_val := 0;
		  end
		else
		  remove(i2);
	      end;
	    if (scr_state = 0) then
	      exit_flag := true
	    else if (equip_ctr = 0) then
	      exit_flag := true
	    else if (inven_ctr > 21) then
	      exit_flag := true
	    else if (not(exit_flag)) then
	      show_equip(com_val);
	  until(exit_flag);
	  if (scr_state <> 0) then
	    if (equip_ctr = 0) then
	      clear(1,1)
	    else
	      prt('You are currently using -',1,1);
	end;

	{ Wear routine, wear or wield an item		-RAK-	}
      procedure wear;
	var
	  com_val,i1,i2,i3,tmp			: integer;
	  out_val,prt1,prt2			: vtype;
	  unwear_obj				: treasure_type;
	  exit_flag,test_flag			: boolean;
	begin
	  if (scr_state = 2) then
	    begin
	      clear(1,1);
	      show_inven(1,inven_ctr);
	    end;
	  exit_flag := false;
	  repeat
	    writev(out_val,'(a-',chr(inven_ctr+96),', * for equipment list,',
		' ^Z to exit) ','Wear/Wield which one ?');
	    test_flag := false;
	    msg_print(out_val);
	    repeat
	      inkey(command);
	      com_val := ord(command);
	      case com_val of
		0,3,25,26,27 :	begin
				  test_flag := true;
				  exit_flag := true;
				end;
		42	 :	begin
				  clear(2,1);
				  show_inven(1,inven_ctr);
				end;
		otherwise	begin
				  com_val := com_val - 96;
				  if ((com_val >= 1) and 
				      (com_val <= inven_ctr)) then 
				    test_flag := true;
				end;
	      end;
	    until (test_flag);
	    if (not(exit_flag)) then	{ Main logic for wearing	}
	      begin
		reset_flag := false;	{ Player turn	}
	        test_flag := true;
		case inventory[com_val].tval of	{ Slot for equipment	}
		  10	: i1 := 23;
		  11	: i1 := 23;
		  12	: i1 := 23;
		  15	: i1 := 33;
		  20	: i1 := 23;
		  21	: i1 := 23;
		  22	: i1 := 23;
		  23	: i1 := 23;
		  25	: i1 := 23;
		  30	: i1 := 31;
		  31	: i1 := 28;
		  32	: i1 := 32;
		  33	: i1 := 24;
		  34	: i1 := 27;
		  35	: i1 := 26;
		  36  	: i1 := 26;
		  40	: i1 := 25;
		  45	: if (inventory[29].tval = 0) then	{ Rings	}
			    i1 := 29
			  else
			    i1 := 30;
		  otherwise 
			  begin
			    msg_print('I don''t see how you can use that.');
			    test_flag := false;
			    com_val := 0;
			  end;
		end;
		if (test_flag) then
		  if (inventory[i1].tval > 0) then
		    begin
		      if (uand(%X'80000000',inventory[i1].flags) <> 0) then
		        begin
			  objdes(out_val,i1,false);
			  out_val := 'The ' + out_val + ' you are ';
			  case i1 of
			    23 : 	out_val := out_val + 'wielding ';
			    otherwise 	out_val := out_val + 'wearing ';
			  end;
			  msg_print(out_val + 'appears to be cursed.');
			  test_flag := false;
			  com_val := 0;
		        end
		      else if (inven_ctr > 21) then
			if (inventory[com_val].number > 1) then
			  if (inventory[com_val].subval < 512) then
			    begin
			msg_print('You will have to drop something first.');
			      test_flag := false;
			      com_val := 0;
			    end;
		    end;
		if (test_flag) then
		  begin
		    unwear_obj := inventory[i1];
		    inventory[i1] := inventory[com_val];
		    with inventory[i1] do
		      begin
				{ Fix for torches	}
		        if ((subval > 255) and (subval < 512)) then
		          begin
		            number := 1; 
		            subval := subval - 255;
		          end;
				{ Fix for weight	}
			inven_weight := inven_weight + weight*number;
		      end;
		    inven_destroy(com_val);	{ Subtracts weight	}
		    equip_ctr := equip_ctr + 1;
		    py_bonuses(inventory[i1],1);
		    if (unwear_obj.tval > 0) then
		      begin
			inventory[inven_max] := unwear_obj;
			tmp := remove(inven_max);
			if (tmp < com_val) then
			  com_val := tmp;
		      end;
		    case i1 of
		      23 : prt1 := 'You are wielding ';
		      33 : prt1 := 'Your light source is ';
		      otherwise prt1 := 'You are wearing ';
		    end;
		    objdes(prt2,i1,true);
		    i2 := 0;
		    i3 := 22;
		    repeat	{ Get the right letter of equipment	}
		      i3 := i3 + 1;
		      if (inventory[i3].tval > 0) then
			i2 := i2 + 1;
		    until (i3 = i1);
		    out_val := prt1 + prt2 + ' (' + chr(i2+96)+cur_char2(i1);
		    msg_print(out_val);
		  end;
	      end;
	    if (scr_state = 0) then
	      exit_flag := true
	    else if (inven_ctr = 0) then
	      exit_flag := true
	    else if (not(exit_flag)) then
	      show_inven(com_val,inven_ctr);
	  until(exit_flag);
	  if (scr_state <> 0) then prt('You are currently carrying -',1,1);
	end;

	{ Switch primary and secondary weapons		-RAK-	}
      procedure switch_weapon;
	var
	  prt1,prt2				: vtype;
	  tmp_obj				: treasure_type;
	begin
	  if (uand(%X'80000000',inventory[23].flags) <> 0) then
	    begin
	      objdes(prt1,23,false);
	      msg_print('The ' + prt1 + 
			' you are wielding appears to be cursed.');
	    end
	  else
	    begin
		{ Switch weapons	}
	      reset_flag := false;
	      tmp_obj := inventory[34];
	      inventory[34] := inventory[23];
	      inventory[23] := tmp_obj;
	      py_bonuses(inventory[34],-1);	{ Subtract bonuses	}
	      py_bonuses(inventory[23],1);	{ Add bonuses		}
	      if (inventory[23].tval > 0) then
	        begin
	          prt1 := 'Primary weapon   : ';
	          objdes(prt2,23,true);
	          msg_print(prt1 + prt2);
	        end;
	      if (inventory[34].tval > 0) then
	        begin
	          prt1 := 'Secondary weapon : ';
	          objdes(prt2,34,true);
	          msg_print(prt1 + prt2);
	        end;
	    end;
	  if (scr_state <> 0) then
	    begin
	      msg_print('');
	      clear(1,1);
	      prt('You are currently using -',1,1);
	      show_equip(1);
	    end;
	end;

	{ Main logic for INVEN_COMMAND			-RAK-	}
      begin
	inven_command := false;
	exit_flag := false;
	scr_state := 0;
	repeat
	  case command of
	    'i' : begin		{ Inventory	}
		    if (inven_ctr = 0) then
		      msg_print('You are not carrying anything.')
		    else if (scr_state <> 1) then
		      begin	{ Sets scr_state to 1		}
		        clear(1,1);
			prt('You are currently carrying -',1,1);
			show_inven(1,inven_ctr);
		      end;
		  end;
	    'e' : begin		{ Equipment	}
		    if (equip_ctr = 0) then
		      msg_print('You are not using any equipment.')
		    else if (scr_state <> 2) then
		      begin	{ Sets scr_state to 2		}
		        clear(1,1);
			prt('You are currently using -',1,1);
			show_equip(1);
		      end;
		  end;
	    't'	: begin		{ Take off	}
		    if (equip_ctr = 0) then
		      msg_print('You are not using any equipment.')
		    else if (inven_ctr > 21) then
		      msg_print('You will have to drop something first.')
		    else
		      unwear;	{ May set scr_state to 2	}
		  end;
	    'w'	: begin		{ Wear/wield	}
		    if (inven_ctr = 0) then
		      msg_print('You are not carrying anything.')
		    else
		      wear;	{ May set scr_state to 1	}
		  end;
	    'x' : begin
		    if (inventory[23].tval <> 0) then
		      switch_weapon
		    else if (inventory[34].tval <> 0) then
		      switch_weapon
		    else
		      msg_print('But you are wielding no weapons.');
		  end;
	{ Special function for other routines			}
	    '?' : begin	{ Displays part inven, returns	}
		    show_inven(r1,r2);
		    scr_state := 0;	{ Clear screen state	}
		  end;
	{ Nonsense command					}
	    otherwise ;
	  end;
	  if (scr_state > 0) then
	    begin
	      prt('<e>quip, <i>inven, <t>ake-off, <w>ear/wield, e<x>change, or ^Z to exit.',24,2);
	      test_flag := false;
	      repeat
	        inkey(command);
	        com_val := ord(command);
	        case com_val of
	          0,3,25,26,27,32 : begin	{ Exit from module	}
				    exit_flag := true;
				    test_flag := true;
				  end;
		  otherwise case command of	{ Module commands	}
			      'e' : test_flag := true;
			      'i' : test_flag := true;
			      't' : test_flag := true;
			      'w' : test_flag := true;
			      'x' : test_flag := true;
			      '?' : ;		{ Trap special feature	}
			      otherwise ;	{ Nonsense command	}
			    end;
	        end;
	      until (test_flag);
	      prt('',24,1);
	    end
	  else
	    exit_flag := true;
	until(exit_flag);
	if (scr_state > 0) then		{ If true, must redraw screen	}
	  inven_command := true;
      end;


	{ Get the ID of an item and return the CTR value of it	-RAK-	}
    function get_item(	var com_val	: integer;
			pmt	 	: vtype;
			var redraw	: boolean;
			i1,i2		: integer) : boolean;
      var
	  command 					: char;
	  out_val					: vtype;
	  test_flag					: boolean;
      begin
	get_item := false;
	com_val := 0;
	if (inven_ctr > 0) then
	  begin
	    writev(out_val,'(Items ',chr(i1+96),'-',chr(i2+96),
		   ', * for inventory list, ^Z to exit) ',pmt);
	    test_flag := false;
	    prt(out_val,1,1);
	    repeat
	      inkey(command);
	      com_val := ord(command);
	      case com_val of
		0,3,25,26,27 :	begin
				  test_flag := true;
				  reset_flag := true;
				end;
		42	 :	begin
				  clear(2,1);
				  inven_command('?',i1,i2);
				  redraw := true;
				end;
		otherwise	begin
				  com_val := com_val - 96;
				  if ((com_val >= i1) and 
				      (com_val <= i2)) then
				    begin
				      test_flag := true;
				      get_item := true;
				    end;
				end;
	      end;
	    until (test_flag);
	    erase_line(msg_line,msg_line);
	  end
	else
	  msg_print('You are not carrying anything.');
      end;

	{ I may have written the town level code, but I'm not exactly	}
	{ proud of it.  Adding the stores required some real slucky	}
	{ hooks which I have not had time to re-think.		-RAK-	}
	%INCLUDE 'MOR_INCLUDE:STORE2.INC'


	{ Calculates current boundries				-RAK-	}
    procedure panel_bounds;
      begin
	panel_row_min := (trunc(panel_row*(screen_height/2)) + 1);
	panel_row_max := panel_row_min + screen_height - 1;
	panel_row_prt := panel_row_min - 2;
	panel_col_min := (trunc(panel_col*(screen_width/2)) + 1);
	panel_col_max := panel_col_min + screen_width - 1;
	panel_col_prt := panel_col_min - 15;
      end;


	{ Given an row (y) and col (x), this routine detects  -RAK-	}
	{ when a move off the screen has occurred and figures new borders}
    function get_panel(y,x : integer) : boolean;
      var
		prow,pcol	: integer;
      begin
	prow := panel_row;
	pcol := panel_col;
	if ((y < panel_row_min + 2) or (y > panel_row_max - 2)) then
	  begin
	    prow := trunc((y - 2)/(screen_height/2));
	    if (prow > max_panel_rows) then
	      prow := max_panel_rows;
	  end;
	if ((x < panel_col_min + 3) or (x > panel_col_max - 3)) then
	  begin
	    pcol := trunc((x - 3)/(screen_width/2));
	    if (pcol > max_panel_cols) then
	      pcol := max_panel_cols;
	  end;
	if ((prow <> panel_row) or (pcol <> panel_col) or not(cave_flag)) then
	  begin
	    panel_row := prow;
	    panel_col := pcol;
	    panel_bounds;
	    get_panel := true;
	    cave_flag := true;
	  end
	else
	  get_panel := false;
      end;


	{ Tests a given point to see if it is within the screen	-RAK-	}
	{ boundries.							}
    function panel_contains(y,x : integer) : boolean;
      begin
	if ((y >= panel_row_min) and (y <= panel_row_max)) then
	  if ((x >= panel_col_min) and (x <= panel_col_max)) then
	    panel_contains := true
	  else
	    panel_contains := false
	else
	  panel_contains := false;
      end;


	{ Returns true if player has no light			-RAK-	}
    function no_light : boolean;
      begin
	no_light := false;
	with cave[char_row,char_col] do
	  if (not(tl)) then
	    if (not(pl)) then
	      no_light := true;
      end;


	{ Prompts for a direction				-RAK-	}
    function get_dir(prompt : vtype;
		     var dir,com_val,y,x : integer) : boolean;
      var
		temp_prompt	: vtype;
		flag		: boolean;
		command		: char;
      begin
	flag := false;
	temp_prompt := '(1 2 3 4 6 7 8 9) ' + prompt;
	prompt := '';
	repeat
	  if (get_com(prompt,command)) then
	    begin
	      com_val := ord(command);
	      dir := com_val - 48;
		{ Note that '5' is not a valid direction	}
	      if (dir in [1,2,3,4,6,7,8,9]) then
		begin
		  move(dir,y,x);
		  flag := true;
		  get_dir := true;
		end
	      else
		prompt := temp_prompt;
	    end
	  else
	    begin
	      reset_flag := true;
	      get_dir := false;
	      flag := true;
	    end;
	until (flag);
      end;



	{ Moves creature record from one space to another	-RAK-	}
    procedure move_rec(y1,x1,y2,x2 : integer);
      begin
	if ((y1 <> y2) or (x1 <> x2)) then
	  begin
	    cave[y2,x2].cptr := cave[y1,x1].cptr;
	    cave[y1,x1].cptr := 0
	  end
      end;


	{ Room is lit, make it appear				-RAK-	}
  procedure light_room(y,x : integer);
    var
	tmp1,tmp2		: integer;
	start_row,start_col	: integer;
	end_row,end_col		: integer;
	i1,i2			: integer;
	ypos,xpos		: integer;
	floor_str		: vtype;
	tmp_char		: char;


    procedure find_light(y1,x1,y2,x2 : integer);
      var
	i1,i2,i3,i4	: integer;
      begin
        for i1 := y1 to y2 do
	  for i2 := x1 to x2 do
	    if (cave[i1,i2].fval in [1,2]) then
	      begin
	        for i3 := i1-1 to i1+1 do
	          for i4 := i2-1 to i2+1 do
		    cave[i3,i4].pl := true;
		cave[i1,i2].fval := 2;
	      end;
      end;

    begin
      tmp1 := trunc(screen_height/2);
      tmp2 := trunc(screen_width /2);
      start_row := trunc(y/tmp1)*tmp1 + 1;
      start_col := trunc(x/tmp2)*tmp2 + 1;
      end_row := start_row + tmp1 - 1;
      end_col := start_col + tmp2 - 1;
      find_light(start_row,start_col,end_row,end_col);
      for i1 := start_row to end_row do
	begin
	  floor_str := '';
	  ypos := i1;
	  for i2 := start_col to end_col do
	    with cave[i1,i2] do
	      begin
	        if ((pl) or (fm)) then
		  begin
		    if (length(floor_str) = 0) then
		      xpos := i2;
		    loc_symbol(i1,i2,tmp_char);
	            floor_str := floor_str + tmp_char
		  end
	        else
		  if (length(floor_str) > 0) then
		    begin
		      print(floor_str,ypos,xpos);
		      floor_str := ''
		    end
	      end;
	  if (length(floor_str) > 0) then
	    print(floor_str,ypos,xpos)
	end;
    end;


	{ Lights up given location				-RAK-	}
    procedure lite_spot(y,x : integer);
      var
		spot_char	: vtype;
		temp		: char;
      begin
	if (panel_contains(y,x)) then
	  begin
	    loc_symbol(y,x,temp);
	    spot_char := temp;
	    print(spot_char,y,x)
	  end
      end;


	{ Blanks out given location				-RAK-	}
    procedure unlite_spot(y,x : integer);
      begin
	if (panel_contains(y,x)) then
	  print(' ',y,x);
      end;


	{ Package for moving the character's light about the screen	}
	{ Three cases : Normal, Finding, and Blind		-RAK-	}
  procedure move_light(y1,x1,y2,x2 : integer);

	{ Maximum of a minimum				-RAK-	}
	{ Macro routine included in MORIALIB.MLB	-RAK-	}
    function maxmin		(
		%immed	x	: integer;
		%immed	y	: integer;
		%immed	z	: integer
				) : integer;
	external;

	{ Minimum of a maximum				-RAK-	}
	{ Macro routine included in MORIALIB.MLB	-RAK-	}
    function minmax		(
		%immed	x	: integer;
		%immed	y	: integer;
		%immed	z	: integer
				) : integer;
	external;

	{ Given two sets of points, draw the block		}
    procedure draw_block(y1,x1,y2,x2 : integer);
      var
	i1,i2,xpos				: integer;
	topp,bott,left,righ			: integer;
	new_topp,new_bott,new_left,new_righ	: integer;
	floor_str,save_str			: vtype;
	tmp_char				: char;
	flag					: boolean;
      begin
	{ From uppermost to bottom most lines player was on...	}
	{ Points are guaranteed to be on the screen (I hope...)	}
	topp := maxmin(y1,y2,panel_row_min);
	bott := minmax(y1,y2,panel_row_max);
	left := maxmin(x1,x2,panel_col_min);
	righ := minmax(x1,x2,panel_col_max);
	new_topp := y2 - 1;	{ Margins for new things to appear}
	new_bott := y2 + 1;
	new_left := x2 - 1;
	new_righ := x2 + 1;
	for i1 := topp to bott do
	  begin
	    floor_str := '';	{ Null out print string		}
	    xpos      := 0;
	    save_str  := '';
	    for i2 := left to righ do	{ Leftmost to rightmost do}
	      begin
	        with cave[i1,i2] do
		  begin
		    if ((pl) or (fm)) then
		      if (((i1=y1) and (i2=x1)) or ((i1=y2) and (i2=x2))) then
		        flag := true
		      else
		        flag := false
		    else
		      begin
			flag := true;
			if (((i1 >= new_topp) and (i1 <= new_bott)) and
			    ((i2 >= new_left) and (i2 <= new_righ))) then
			  begin
			    if (tl) then
		              if (fval in pwall_set) then
		                pl := true
		              else if (tptr > 0) then
		                if (t_list[tptr].tval in light_set) then
			          if (not(fm)) then
			            fm := true;
			  end
		      end;
		    if ((pl) or (tl) or (fm)) then
		      loc_symbol(i1,i2,tmp_char)
		    else
		      tmp_char := ' ';
		    if (py.flags.image > 0) then
		      if (randint(12) = 1) then
			tmp_char := chr(randint(95) + 31);
		    if (flag) then
		      begin
			if (xpos = 0) then xpos := i2;
			if (length(save_str) > 0) then
			  begin
			    floor_str := floor_str + save_str;
			    save_str := '';
			  end;
		        floor_str := floor_str + tmp_char;
		      end
		    else if (xpos > 0) then
		      save_str := save_str + tmp_char;
		  end;
	      end;
	    if (xpos > 0) then
	      begin
		i2 := i1;	{ Var for PRINT cannot be loop index}
		print(floor_str,i2,xpos);
	      end;
	  end;
      end;


	{ Normal movement					}
    procedure sub1_move_light(y1,x1,y2,x2 : integer);
      var
	i1,i2					: integer;
      begin
	light_flag := true;
	for i1 := y1-1 to y1+1 do	{ Turn off lamp light	}
	  for i2 := x1-1 to x1+1 do
	    cave[i1,i2].tl := false;
	for i1 := y2-1 to y2+1 do
	  for i2 := x2-1 to x2+1 do
	    cave[i1,i2].tl := true;
	draw_block(y1,x1,y2,x2);	{ Redraw area		}
      end;

	{ When FIND_FLAG, light only permanent features		}
    procedure sub2_move_light(y1,x1,y2,x2 : integer);
      var
	i1,i2,xpos			: integer;
	floor_str,save_str		: vtype;
	tmp_char			: char;
	flag				: boolean;
      begin
	if (light_flag) then
	  begin
	    for i1 := y1-1 to y1+1 do
	      for i2 := x1-1 to x1+1 do
	        cave[i1,i2].tl := false;
	    draw_block(y1,x1,y1,x1);
	    light_flag := false;
	  end;
	for i1 := y2-1 to y2+1 do
	  begin
	    floor_str := '';
	    save_str  := '';
	    xpos := 0;
	    for i2 := x2-1 to x2+1 do
	      with cave[i1,i2] do
	        begin
		  flag := false;
		  if (not((fm) or (pl))) then
		    begin
		      tmp_char := ' ';
		      if (player_light) then
		        if (fval in pwall_set) then
		          begin
		            pl := true;	{ Turn on perm light	}
			    loc_symbol(i1,i2,tmp_char);
			    flag := true;
		          end
		        else
		          if (tptr > 0) then
		            if (t_list[tptr].tval in light_set) then
			      begin
			        fm := true;	{ Turn on field marker	}
			        loc_symbol(i1,i2,tmp_char);
			        flag := true;
			      end;
		    end
		  else
		    loc_symbol(i1,i2,tmp_char);
		  if (flag) then
		    begin
		      if (xpos = 0) then xpos := i2;
		      if (length(save_str) > 0) then
			begin
			  floor_str := floor_str + save_str;
			  save_str := '';
			end;
		      floor_str := floor_str + tmp_char;
		    end
		  else if (xpos > 0) then
		    save_str := save_str + tmp_char;
	        end;
	    if (xpos > 0) then
	      begin
		i2 := i1;
	        print(floor_str,i2,xpos);
	      end;
	  end;
      end;

	{ When blinded, move only the player symbol...		}
    procedure sub3_move_light(y1,yx1,y2,x2 : integer);
      var
	i1,i2			: integer;
      begin
	if (light_flag) then
	  begin
	    for i1 := y1-1 to y1+1 do
	      for i2 := x1-1 to x1+1 do
	        cave[i1,i2].tl := false;
	    light_flag := false;
	  end;
	print(' ',y1,x1);
	print('@',y2,x2);
      end;

	{ With no light, movement becomes involved...		}
    procedure sub4_move_light(y1,x1,y2,x2 : integer);
      var
	i1,i2			: integer;
      begin
	light_flag := true;
	if (cave[y1,x1].tl) then
	  begin
	    for i1 := y1-1 to y1+1 do
	      for i2 := x1-1 to x1+1 do
		begin
	          cave[i1,i2].tl := false;
		  if (test_light(i1,i2)) then
		    lite_spot(i1,i2)
		  else
		    unlite_spot(i1,i2);
		end;
	  end
	else if (test_light(y1,x1)) then
	  lite_spot(y1,x1)
	else
	  unlite_spot(y1,x1);
	print('@',y2,x2);
      end;

	{ Begin move_light procedure				}
    begin
      if (py.flags.blind > 0) then
	sub3_move_light(y1,x1,y2,x2)
      else if (find_flag) then
	sub2_move_light(y1,x1,y2,x2)
      else if (not(player_light)) then
	sub4_move_light(y1,x1,y2,x2)
      else
	sub1_move_light(y1,x1,y2,x2);
    end;


	{ Returns random co-ordinates				-RAK-	}
    procedure new_spot(var y,x : integer);
      begin
	repeat
	  y := randint(cur_height);
	  x := randint(cur_width);
        until (	(cave[y,x].fopen)	and 
		(cave[y,x].cptr = 0)	and
		(cave[y,x].tptr = 0));
      end;


	{ Search Mode enhancement				-RAK-	}
    procedure search_on;
      begin
	search_flag := true;
	change_speed(+1);
	py.flags.status := uor(py.flags.status,%X'00000100');
	prt_search;
	with py.flags do
	  food_digested := food_digested + 1;
      end;

    procedure search_off;
      begin
	search_flag := false;
	find_flag := false;
	move_char(5);
	change_speed(-1);
	py.flags.status := uand(py.flags.status,%X'FFFFFEFF');
	prt_search;
	with py.flags do
	  food_digested := food_digested - 1;
      end;


	{ Resting allows a player to safely restore his hp	-RAK-	}
    procedure rest;
      var
		rest_num		: integer;
		rest_str		: vtype;
      begin
	prt('Rest for how long? ',1,1);
	get_string(rest_str,1,20,10);
	rest_num := 0;
	readv(rest_str,rest_num,error:=continue);
	if (rest_num > 0) then
	  begin
	    if (search_flag) then
	      search_off;
	    py.flags.rest := rest_num;
	    py.flags.status := uor(py.flags.status,%X'00000200');
	    prt_rest;
	    with py.flags do
	      food_digested := food_digested - 1;
	    msg_print('Press any key to wake up...');
	    put_qio;
	  end
	else
	  erase_line(msg_line,msg_line);
      end;

    procedure rest_off;
      begin
	py.flags.rest := 0;
	py.flags.status := uand(py.flags.status,%X'FFFFFDFF');
	erase_line(1,1);
	prt_rest;
	with py.flags do
	  food_digested := food_digested + 1;
      end;


	{ Attacker's level and pluses, defender's AC		-RAK-	}
    function test_hit(bth,level,pth,ac : integer) : boolean;
      var
		i1			: integer;
      begin
	if (search_flag) then
	  search_off;
	if (py.flags.rest > 0) then
	  rest_off;
	i1 := bth + level*bth_lev_adj + pth*bth_plus_adj;
	if (randint(i1) > ac) then		{ Normal hit		}
	  test_hit := true
	else if (randint(20) = 1) then		{ Always hit 1/20	}
	  test_hit := true
	else					{ Missed		}
	  test_hit := false;
      end;


	{ Decreases players hit points and sets death flag if neccessary}
	{							-RAK-	}
    procedure take_hit(damage : integer; hit_from : vtype);
      begin
	if (py.flags.invuln > 0) then damage := 0;
	py.misc.chp := py.misc.chp - damage;
	if (search_flag) then
	  search_off;
	if (py.flags.rest > 0) then
	  rest_off;
	flush;
	if (py.misc.chp <= -1) then
	  begin
	    if (not(death)) then
	      begin		{ Hee, hee... Ain't I mean?	}
	        death := true;
		died_from := hit_from;
		total_winner := false;
	      end;
	    moria_flag := true;
	  end
	else
	  prt_chp;
      end;


	{ Given speed, returns number of moves this turn.	-RAK-	}
	{ NOTE: Player must always move at least once per iteration,	}
	{       a slowed player is handled by moving monsters faster	}
    function movement_rate(speed : integer) : integer;
      begin
	if (speed > 0) then
	  if (py.flags.rest > 0) then
	    movement_rate := 1
	  else
	    movement_rate := speed
	else
	  begin
	    if ((turn mod (abs(speed) + 2)) = 0) then
	      movement_rate := 1
	    else
	      movement_rate := 0;
	  end;
      end;


	{ Regenerate hit points					-RAK-	}
    procedure regenhp(percent : real);
      begin
	with py.misc do
	  chp := chp + mhp*percent + player$regen_hpbase;
      end;


	{ Regenerate mana points				-RAK-	}
    procedure regenmana(percent : real);
      begin
	with py.misc do
	  cmana := cmana + mana*percent + player$regen_mnbase;
      end;


	{ Change a trap from invisible to visible		-RAK-	}
	{ Note: Secret doors are handled here				}
    procedure change_trap(y,x : integer);
      var
		i3		: integer;
      begin
	with cave[y,x] do
	  if (t_list[tptr].tval in [101,109]) then
	    begin
	      i3 := tptr;
	      place_trap(y,x,2,t_list[i3].subval);
	      pusht(i3);
	      lite_spot(y,x);
	    end;
      end;


	{ Searches for hidden things...				-RAK-	}
    procedure search(y,x,chance : integer);
      var
		i1,i2		: integer;
      begin
	with py.flags do
	  if (confused+blind > 0) then
	    chance := trunc(chance/10.0)
	   else if (no_light) then
	    chance := trunc(chance/5.0);
	for i1 := (y - 1) to (y + 1) do
	  for i2 := (x - 1) to (x + 1) do
	    if (in_bounds(i1,i2)) then
	      if ((i1 <> y) or (i2 <> x)) then
	        if (randint(100) < chance) then
	          with cave[i1,i2] do
	{ Search for hidden objects		}
		    if (tptr > 0) then
		      with t_list[tptr] do
		{ Trap on floor?		}
		        if (tval = 101) then
		          begin
			    msg_print('You have found ' + name + '.');
			    change_trap(i1,i2);
			    find_flag := false;
		          end
		{ Secret door?			}
			else if (tval = 109) then
			  begin
			    msg_print('You have found a secret door.');
			    fval := corr_floor2.ftval;
			    change_trap(i1,i2);
			    find_flag := false;
			  end
		{ Chest is trapped?		}
		        else if (tval = 2) then
			  begin
			    if (flags > 1) then
			      if (index(name,'^') > 0) then
				begin
				  known2(name);
		msg_print('You have discovered a trap on the chest!');
				end;
			  end;
      end;


	{ Turns off Find_flag if something interesting appears	-RAK-	}
	{ BUG: Does not handle corridor/room corners, but I didn't want	}
	{      to add a lot of checking for such a minor detail		}
    procedure area_affect(dir,y,x : integer);
      var
		z		: array [1..3] of integer;
		i1,row,col	: integer;
      begin
	if (cave[y,x].fval = 4) then
	  begin
	    i1 := 0;
	    if (next_to4(y,x,[4,5,6]) > 2) then
	      find_flag := false;
	  end;
	if ((find_flag) and (py.flags.blind < 1)) then
	  begin
	    case dir of
		1 :	begin
			  z[1] := 4;
			  z[2] := 1;
			  z[3] := 3;
			end;
		2 :	begin
			  z[1] := 4;
			  z[2] := 2;
			  z[3] := 6;
			end;
		3 :	begin
			  z[1] := 2;
			  z[2] := 3;
			  z[3] := 6;
			end;
		4 :	begin
			  z[1] := 8;
			  z[2] := 4;
			  z[3] := 2;
			end;
		6 :	begin
			  z[1] := 2;
			  z[2] := 6;
			  z[3] := 8;
			end;
		7 :	begin
			  z[1] := 8;
			  z[2] := 7;
			  z[3] := 4;
			end;
		8 :	begin
			  z[1] := 4;
			  z[2] := 8;
			  z[3] := 6;
			end;
		9 :	begin
			  z[1] := 8;
			  z[2] := 9;
			  z[3] := 6;
			end;
	    end;
	    for i1 := 1 to 3 do
	      begin
	        row := y;
	        col := x;
	        if (move(z[i1],row,col)) then
	          with cave[row,col] do
		    begin
			{ Empty doorways	}
		      if (fval = 5) then
		        find_flag := false;
			{ Objects player can see}
			{ Including doors	}
		      if (find_flag) then
			if (player_light) then
			  begin
			    if (tptr > 0) then
			      if (not(t_list[tptr].tval in [101,109])) then
			        find_flag := false;
			  end
			else if ((tl) or (pl) or (fm)) then
		          if (tptr > 0) then
		            if (not(t_list[tptr].tval in [101,109])) then
		              find_flag := false;
			{ Creatures		}
		      if (find_flag) then
		        if ((tl) or (pl) or (player_light)) then
		          if (cptr > 1) then
			    with m_list[cptr] do
			      if (ml) then
		                find_flag := false;
		    end
	      end
	  end;
      end;


	{ Picks new direction when in find mode			-RAK-	}
    function pick_dir(dir : integer) : boolean;
      var
		z		: array [1..2] of integer;
		i1,y,x		: integer;
      begin
	if ((find_flag) and (next_to4(char_row,char_col,corr_set) = 2)) then
	  begin
	    case dir of
		1 :	begin
			  z[1] := 2;
			  z[2] := 4;
			end;
		2 :	begin
			  z[1] := 4;
			  z[2] := 6;
			end;
		3 :	begin
			  z[1] := 2;
			  z[2] := 6;
			end;
		4 :	begin
			  z[1] := 2;
			  z[2] := 8;
			end;
		6 :	begin
			  z[1] := 2;
			  z[2] := 8;
			end;
		7 :	begin
			  z[1] := 4;
			  z[2] := 8;
			end;
		8 :	begin
			  z[1] := 4;
			  z[2] := 6;
			end;
		9 :	begin
			  z[1] := 6;
			  z[2] := 8;
			end;
	    end;
	    pick_dir := false;
	    for i1 := 1 to 2 do
	      begin
		y := char_row;
		x := char_col;
		if (move(z[i1],y,x)) then
		  if (cave[y,x].fopen) then
		    begin
		      pick_dir := true;
		      com_val := z[i1] + 48
		    end
	      end
	  end
	else
	  begin
	    pick_dir := false;
	  end;
      end;


	{ AC gets worse						-RAK-	}
	{ Note: This routine affects magical AC bonuse so that stores	}
	{       can detect the damage.					}
    function minus_ac(typ_dam : integer) : boolean;
      var
	i1,i2			: integer;
	tmp			: array [1..5] of integer;
      begin
	i1 := 0;
	if (inventory[26].tval > 0) then
	  begin
	    i1 := i1 + 1;
	    tmp[i1] := 26;
	  end;
	if (inventory[27].tval > 0) then
	  begin
	    i1 := i1 + 1;
	    tmp[i1] := 27;
	  end;
	if (inventory[32].tval > 0) then
	  begin
	    i1 := i1 + 1;
	    tmp[i1] := 32;
	  end;
	if (inventory[28].tval > 0) then
	  begin
	    i1 := i1 + 1;
	    tmp[i1] := 28;
	  end;
	if (inventory[24].tval > 0) then
	  begin
	    i1 := i1 + 1;
	    tmp[i1] := 24;
	  end;
	minus_ac := false;
	if (i1 > 0) then
	  begin
	    i2 := tmp[randint(i1)];
	    with inventory[i2] do
	      if (uand(flags,typ_dam) <> 0) then
		begin
		  objdes(out_val,i2,false);
		  msg_print('Your ' + out_val + ' resists damage!');
		  minus_ac := true;
		end
	      else if ((ac+toac) > 0) then
	        begin
		  objdes(out_val,i2,false);
		  msg_print('Your ' + out_val + ' is damaged!');
		  toac := toac - 1;
		  py_bonuses(blank_treasure,0);
		  minus_ac := true;
	        end
	  end
      end;


	{ Corrode the unsuspecting person's armor		-RAK-	}
    procedure corrode_gas(kb_str : vtype);
      begin
	if (not (minus_ac(%X'00100000'))) then
	  take_hit(randint(8),kb_str);
	print_stat := uor(%X'0040',print_stat);
	if (inven_damage([23,33,34,35,65],5) > 0) then
	  msg_print('There is an acrid smell coming from your pack.');
      end;


	{ Poison gas the idiot...				-RAK-	}
    procedure poison_gas(dam : integer; kb_str : vtype);
      begin
	take_hit(dam,kb_str);
	print_stat := uor(%X'0040',print_stat);
	py.flags.poisoned := py.flags.poisoned + 12 + randint(dam);
      end;


	{ Burn the fool up...					-RAK-	}
    procedure fire_dam(dam : integer; kb_str : vtype);
      begin
	if (py.flags.fire_resist)then
	  dam := trunc(dam*0.5);
	if (py.flags.resist_heat > 0) then
	  dam := trunc(dam*0.5);
	take_hit(dam,kb_str);
	print_stat := uor(%X'0080',print_stat);
	if (inven_damage([12,20,21,22,30,31,32,36,55,70,71],3) > 0) then
	  msg_print('There is smoke coming from your pack!');
      end;


	{ Freeze him to death...				-RAK-	}
    procedure cold_dam(dam : integer; kb_str : vtype);
      begin
	if (py.flags.cold_resist)then
	  dam := trunc(dam*0.5);
	if (py.flags.resist_cold > 0) then
	  dam := trunc(dam*0.5);
	take_hit(dam,kb_str);
	print_stat := uor(%X'0080',print_stat);
	if (inven_damage([75,76],5) > 0) then
	  msg_print('Something shatters inside your pack!');
      end;


	{ Lightning bolt the sucker away...			-RAK-	}
    procedure light_dam(dam : integer; kb_str : vtype);
      begin
	if (py.flags.lght_resist) then
	  take_hit(trunc(dam*0.5),kb_str)
	else
	  take_hit(dam,kb_str);
	print_stat := uor(%X'0080',print_stat);
      end;


	{ Throw acid on the hapless victim			-RAK-	}
    procedure acid_dam(dam : integer; kb_str : vtype);
      var
		flag		: integer;
      begin
	flag := 0;
	if (minus_ac(%X'00100000')) then
	  flag := 1;
	if (py.flags.acid_resist) then
	  flag := flag + 2;
	case flag of
	  0 : take_hit(dam,kb_str);
	  1 : take_hit(trunc(dam*0.75),kb_str);
	  2 : take_hit(trunc(dam*0.5),kb_str);
	  3 : take_hit(trunc(dam*0.25),kb_str);
	end;
	print_stat := uor(%X'00C0',print_stat);
	if (inven_damage([1,2,11,12,20,21,22,30,31,32,36],3) > 0) then
	  msg_print('There is an acrid smell coming from your pack!');
      end;


	{ Teleport the player to a new location			-RAK-	}
    procedure teleport(dis : integer);
      var
		y,x	: integer;
      begin
	repeat
	  y := randint(cur_height);
	  x := randint(cur_width);
	  while (distance(y,x,char_row,char_col) > dis) do
	    begin
	      y := y + trunc((char_row-y)/2);
	      x := x + trunc((char_col-x)/2);
	    end;
        until ((cave[y,x].fopen) and (cave[y,x].cptr  < 2));
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
	teleport_flag := false;
      end;


	{ Player hit a trap...	(Chuckle)			-RAK-	}
    procedure hit_trap(var y,x : integer);
      var
		i1,i2,ty,tx		: integer;
		dam			: integer;
      begin
	change_trap(y,x);
	lite_spot(char_row,char_col);
	find_flag := false;
	with cave[y,x] do
	with py.misc do
	  begin
	    dam := damroll(t_list[tptr].damage);
	    case t_list[tptr].subval of
{ Open pit}   1	: begin
		    msg_print('You fell into a pit!');
		    if (py.flags.ffall) then
		      msg_print('You gently float down.')
		    else
		      take_hit(dam,'an open pit.');
		  end;
{ Arrow trap} 2	: begin
		    if (test_hit(125,0,0,pac+ptoac)) then
		      begin
			take_hit(dam,'an arrow trap.');
			msg_print('An arrow hits you.');
		      end
		    else
		      msg_print('An arrow barely misses you.');
		  end;
{ Covered pit}3	: begin
		    msg_print('You fell into a covered pit.');
		    if (py.flags.ffall) then
		      msg_print('You gently float down.')
		    else
		      take_hit(dam,'a covered pit.');
		      place_trap(y,x,2,1);
		  end;
{ Trap door}  4	: begin
		    msg_print('You fell through a trap door!');
		    msg_print(' ');
		    moria_flag := true;
		    dun_level := dun_level + 1;
		    if (py.flags.ffall) then
		      msg_print('You gently float down.')
		    else
		      take_hit(dam,'a trap door.');
		  end;
{ Sleep gas}  5	: if (py.flags.paralysis = 0) then
		    begin
		      msg_print('A strange white mist surrounds you!');
		      if (py.flags.free_act) then
			msg_print('You are unaffected.')
		      else
			begin
			  msg_print('You fall asleep.');
			  py.flags.paralysis := py.flags.paralysis + 
							randint(10) + 4;
			end
		    end;
{ Hid Obj}    6	: begin
		    fm := false;
		    pusht(tptr);
		    place_object(y,x);
		    msg_print('Hmmm, there was something under this rock.');
		  end;
 { STR Dart}  7	: begin
		    if (test_hit(125,0,0,pac+ptoac)) then
		      begin
			if (not py.flags.sustain_str) then
			  begin
			    py.stat.cstr := de_statp(py.stat.cstr);
			    take_hit(dam,'a dart trap.');
			    print_stat := uor(%X'0001',print_stat);
			    msg_print('A small dart weakens you!');
			  end
			else
			  msg_print('A small dart hits you.');
		      end
		    else
		      msg_print('A small dart barely misses you.');
		  end;
{ Teleport}   8	: begin
		    teleport_flag := true;
		    msg_print('You hit a teleport trap!');
		  end;
{ Rockfall}   9	: begin
		    take_hit(dam,'falling rock.');
		    pusht(tptr);
		    place_rubble(y,x);
		    msg_print('You are hit by falling rock');
		  end;
{ Corrode gas}10: begin
		    corrode_gas('corrosion gas.');
		    msg_print('A strange red gas surrounds you.');
		  end;
{ Summon mon} 11: begin
		    fm := false;	{ Rune disappears...	}
		    pusht(tptr);
		    tptr := 0;
		    for i1 := 1 to (2+randint(3)) do
		      begin
		        ty := char_row;
		        tx := char_col;
		        summon_monster(ty,tx,false);
		      end;
		  end;
{ Fire trap}  12: begin
		    fire_dam(dam,'a fire trap.');
		    msg_print('You are enveloped in flames!');
		  end;
{ Acid trap}  13: begin
		    acid_dam(dam,'an acid trap.');
		    msg_print('You are splashed with acid!');
		  end;
{ Poison gas} 14: begin
		    poison_gas(dam,'a poison gas trap.');
		    msg_print('A pungent green gas surrounds you!');
		  end;
{ Blind Gas } 15: begin
		    msg_print('A black gas surrounds you!');
		    with py.flags do
		      blind := blind + randint(50) + 50;
		  end;
{ Confuse Gas}16: with py.flags do
		    begin
	      msg_print('A gas of scintillating colors surrounds you!');
		      confused := confused + randint(15) + 15;
		    end;
{ Slow Dart}  17: begin
		    if (test_hit(125,0,0,pac+ptoac)) then
		      begin
			take_hit(dam,'a dart trap.');
			msg_print('A small dart hits you!');
			with py.flags do
			  slow := slow + randint(20) + 10;
		      end
		    else
		      msg_print('A small dart barely misses you.');
		  end;
{ CON Dart}   18: begin
		    if (test_hit(125,0,0,pac+ptoac)) then
		      begin
			if (not py.flags.sustain_con) then
			  begin
			    py.stat.ccon := de_statp(py.stat.ccon);
			    take_hit(dam,'a dart trap.');
			    print_stat := uor(%X'0004',print_stat);
			    msg_print('A small dart weakens you!');
			  end
			else
			  msg_print('A small dart hits you.');
		      end
		    else
		      msg_print('A small dart barely misses you.');
		  end;
{Secret Door} 19: ;
{ Scare Mon}  99: ;

			{ Town level traps are special, the stores...	}
{ General    }101: enter_store(1);
{ Armory     }102: enter_store(2);
{ Weaponsmith}103: enter_store(3);
{ Temple     }104: enter_store(4);
{ Alchemy    }105: enter_store(5);
{ Magic-User }106: enter_store(6);

		otherwise	msg_print('Unknown trap value');
	    end
	  end
      end;


	{ Return spell number and failure chance		-RAK-	}
    function cast_spell(prompt       : vtype;
			item_val     : integer; 
			var sn,sc    : integer;
			var redraw   : boolean) : boolean;
      var
		i2		: unsigned;
		i1,i3		: integer;
		spell		: spl_type;
      begin
	i1 := 0;
	i2 := inventory[item_val].flags;
	repeat
	  i3 := bit_pos(i2);
	  if (i3 > 0) then
	    with magic_spell[py.misc.pclass,i3] do
	      if (slevel <= py.misc.lev) then
	        if (learned) then
	          begin
	            i1 := i1 + 1;
		    spell[i1].splnum := i3;
	          end;
	until(i2 = 0);
	if (i1 > 0) then
	  cast_spell := get_spell(spell,i1,sn,sc,prompt,redraw);
	if (redraw) then 
	  draw_cave;
      end;


	{ Finds range of item in inventory list			-RAK-	}
    function find_range(item_val : obj_set; var i2,i3 : integer) : boolean;
      var
		i1		: integer;
		flag		: boolean;
      begin
	i1 := 0;
	i2 := 0;
	i3 := 0;
	flag := false;
	while(i1 < inven_ctr) do
	  begin
	    i1 := i1 + 1;
	    if ((inventory[i1].tval in item_val) and (not (flag))) then
	      begin
	        flag := true;
	        i2 := i1;
	      end;
	    if ((not(inventory[i1].tval in item_val)) and (flag) and 
								(i3 = 0)) then
	      i3 := i1 - 1;
	  end;
	if ((flag) and (i3 = 0)) then
	  i3 := inven_ctr;
	find_range := flag;
      end;
	

	{ Examine a Book					-RAK-	}
    procedure examine_book;
      var
		i2			: unsigned;
		i1,i3,item_val		: integer;
		redraw,flag		: boolean;
		dummy			: char;
		out_val			: vtype;
      begin
	redraw := false;
	if (not(find_range([90,91],i1,i3))) then
	  msg_print('You are not carrying any books.')
	else if (get_item(item_val,'Which Book?',redraw,i1,i3)) then
	  begin
	    flag := true;
	    with inventory[item_val] do
	      if (class[py.misc.pclass].mspell) then
	        begin
		  if (tval <> 90) then
		    begin
		      msg_print('You do not understand the language.');
		      flag := false;
		    end;
		end
	      else if (class[py.misc.pclass].pspell) then
	        begin
		  if (tval <> 91) then
		    begin
		      msg_print('You do not understand the language.');
		      flag := false;
		    end;
		end
	      else
		begin
		  msg_print('You do not understand the language.');
		  flag := false;
	        end;
	    if (flag) then
	      begin
	        redraw := true;
	        i1 := 0;
	        i2 := inventory[item_val].flags;
	        clear(1,1);
	writev(out_val,'   Name                         Level  Mana   Known');
	        prt(out_val,1,1);
	        repeat
	          i3 := bit_pos(i2);
	          if (i3 > 0) then
		    with magic_spell[py.misc.pclass,i3] do
		      begin
		        i1 := i1 + 1;
		        if (slevel < 99) then
		          begin
			    writev(out_val,chr(96+i1),') ',pad(sname,' ',30),
				slevel:2,'     ',smana:2,'   ',learned);
			    prt(out_val,i1+1,1);
		          end
		        else
		          prt('',i1+1,1);
		      end;
	        until (i2 = 0);
	        prt('[Press any key to continue]',24,20);
	        inkey(dummy);
	      end;
	  end;
	if (redraw) then draw_cave;
      end;


	{ Player is on an object.  Many things can happen BASED -RAK-	}
	{ on the TVAL of the object.  Traps are set off, money and most	}
	{ objects are picked up.  Some objects, such as open doors, just}
	{ sit there...							}
    procedure carry(y,x : integer);
      var
	item_val			: integer;
	out_val				: vtype;
      begin
	find_flag := false;
	with cave[y,x] do
	  begin
	    inventory[inven_max] := t_list[tptr];
		{ There's GOLD in them thar hills!	}
	    if (t_list[tptr].tval = 100) then
	      begin
		pusht(tptr);
		tptr := 0;
		with inventory[inven_max] do
		  begin
		    py.misc.au := py.misc.au + cost;
		    writev(out_val,'You have found ',cost:1,
				' gold pieces worth of ',name,'.');
		  end;
		prt_gold;
		msg_print(out_val);
	      end
		{ OPPS!					}
	    else if (t_list[tptr].tval in trap_set) then
	      hit_trap(y,x)
		{ Attempt to pick up an object.		}
	    else if (t_list[tptr].tval < 100) then
	      begin
		if (inven_check_weight) then	{ Weight limit check	}
		  if (inven_check_num) then	{ Too many objects?	}
		    begin			{ Okay, pick it up	}
		      pusht(tptr);
		      tptr := 0;
		      inven_carry(item_val);
		      objdes(out_val,item_val,true);
		      out_val := 'You have ' + out_val + ' (' +
				chr(item_val+96) + cur_char1(item_val);
		      msg_print(out_val);
		    end
		  else
		    msg_print('You can''t carry that many items.')
		else
		  msg_print('You can''t carry that much weight.');
	      end;
	  end;
      end;


	{ Drop an object being carried				-RAK-	}
	{ Note: Only one object per floor spot...			}
    procedure drop;
      var
		com_val,i1,i2	: integer;
		redraw		: boolean;
		out_val		: vtype;
      begin
	if (inven_ctr > 0) then
	  begin
	    if (get_item(com_val,'Which one? ',redraw,1,inven_ctr)) then
	      begin
		if (redraw) then draw_cave;
	        with cave[char_row,char_col] do
	          if (tptr > 0) then
	            msg_print('There is something there already.')
	          else
		    begin
		      inven_drop(com_val,char_row,char_col);
		      objdes(out_val,inven_max,true);
		      out_val := 'Dropped ' + out_val;
		      msg_print(out_val);
		    end
	      end
	    else if (redraw) then
	      draw_cave;
	  end
	else
	  msg_print('You are not carrying anything.');
      end;


	{ Deletes a monster entry from the level		-RAK-	}
    procedure delete_monster(i2 : integer);
      var
		i1,i3		: integer;
      begin
	i1 := muptr;
	i3 := m_list[i2].nptr;
	if (i1 = i2) then
	  muptr := i3
	else
	  begin
	    while (m_list[i1].nptr <> i2) do
	      i1 := m_list[i1].nptr;
	    m_list[i1].nptr := i3;
	  end;
	with m_list[i2] do
	  begin
	    cave[fy,fx].cptr := 0;
	    if (ml) then
	      with cave[fy,fx] do
		if ((pl) or (tl)) then
		  lite_spot(fy,fx)
		else
		  unlite_spot(fy,fx);
	    pushm(i2);
	  end;
	mon_tot_mult := mon_tot_mult - 1;
      end;


	{ Makes sure new creature gets lit up			-RAK-	}
  procedure check_mon_lite(y,x : integer);
    begin
      with cave[y,x] do
	if (cptr > 1) then
	  if (not(m_list[cptr].ml)) then
	    if ((tl) or (pl)) then
	      if (los(char_row,char_col,y,x)) then
		begin
		  m_list[cptr].ml := true;
		  lite_spot(y,x);
		end;
    end;


	{ Places creature adjacent to given location		-RAK-	}
	{ Rats and Flys are fun!					}
  procedure multiply_monster(y,x,z : integer; slp : boolean);
    var
	i1,i2,i3		: integer;
    begin
      i1 := 0;
      repeat
	i2 := y - 2 + randint(3);
	i3 := x - 2 + randint(3);
	if (in_bounds(i2,i3)) then
	  with cave[i2,i3] do
	    if (fval in floor_set) then
	      if ((tptr = 0) and (cptr <> 1)) then
		begin
		  if (cptr > 1) then	{ Creature there already?	}
		    begin
			{ Some critters are canabalistic!	}
		      if (uand(c_list[z].cmove,%X'00080000') <> 0) then
		        begin
		          delete_monster(cptr);
			  place_monster(i2,i3,z,slp);
			  check_mon_lite(i2,i3);
			  mon_tot_mult := mon_tot_mult + 1;
			end;
		    end
		  else
			{ All clear, place a monster	}
		    begin
		      place_monster(i2,i3,z,slp);
		      check_mon_lite(i2,i3);
		      mon_tot_mult := mon_tot_mult + 1;
		    end;
		  i1 := 18;
		end;
	i1 := i1 + 1;
      until (i1 > 18);
    end;


	{ Creates objects nearby the coordinates given		-RAK-	}
	{ BUG: Because of the range, objects can actually be placed into}
	{      areas closed off to the player, this is rarely noticable,}
	{      and never a problem to the game.				}
  procedure summon_object(y,x,num,typ : integer);
    var
	i1,i2,i3		: integer;
    begin
      repeat
	i1 := 0;
        repeat
	  i2 := y - 3 + randint(5);
	  i3 := x - 3 + randint(5);
	  if (in_bounds(i2,i3)) then
	    with cave[i2,i3] do
	      if (fval in floor_set) then
	        if (tptr = 0) then
		  begin
		    case typ of			{ Select type of object	}
		      1 :  place_object(i2,i3);
		      2 :  place_gold(i2,i3);
		      3 :  if (randint(100) < 50) then
			     place_object(i2,i3)
			   else
			     place_gold(i2,i3);
		      otherwise ;
		    end;
		    if (test_light(i2,i3)) then
		      lite_spot(i2,i3);
		    i1 := 10;
		  end;
	  i1 := i1 + 1;
        until (i1 > 10);
	num := num - 1;
      until (num = 0);
    end;


	{ Deletes object from given location			-RAK-	}
  function delete_object(y,x : integer) : boolean;
    begin
      delete_object := false;
      with cave[y,x] do
	begin
	  if (t_list[tptr].tval = 109) then
	    fval := corr_floor3.ftval;
	  fopen := true;
	  pusht(tptr);
	  tptr := 0;
	  fm := false;
	  if (test_light(y,x)) then
	    begin
	      lite_spot(y,x);
	      delete_object := true;
	    end
	  else
	    unlite_spot(y,x);
	end;
    end;


	{ Allocates objects upon a creatures death		-RAK-	}
	{ Oh well, another creature bites the dust...  Reward the victor}
	{ based on flags set in the main creature record		}
  procedure monster_death(y,x : integer; flags : unsigned);
    var
	i1 		: integer;
    begin
      if (uand(flags,%X'01000000') <> 0) then
	i1 := 1
      else
	i1 := 0;
      if (uand(flags,%X'02000000') <> 0) then
	i1 := i1 + 2;
      if (uand(flags,%X'04000000') <> 0) then
	if (randint(100) < 60) then
	  summon_object(y,x,1,i1);
      if (uand(flags,%X'08000000') <> 0) then
	if (randint(100) < 90) then
	  summon_object(y,x,1,i1);
      if (uand(flags,%X'10000000') <> 0) then
	summon_object(y,x,randint(2),i1);
      if (uand(flags,%X'20000000') <> 0) then
	summon_object(y,x,damroll('2d2'),i1);
      if (uand(flags,%X'40000000') <> 0) then
	summon_object(y,x,damroll('4d3'),i1);
      if (uand(flags,%X'80000000') <> 0) then
	begin
	  total_winner := true;
	  prt_winner;
	  msg_print('*** CONGRATULATIONS *** You have won the game...');
	  msg_print('Use <CONTROL>-Y when you are ready to quit.');
	end;
    end;


	{ Decreases monsters hit points and deletes monster in needed.	}
	{ (Picking on my babies...)				-RAK-	}
    function mon_take_hit(monptr,dam : integer) : integer;
      var
	acc_tmp			: real;
      begin
	with m_list[monptr] do
	  begin
	    hp := hp - dam;
	    csleep := 0;
	    if (hp < 0) then
	      begin
		monster_death(fy,fx,c_list[mptr].cmove);
		with c_list[mptr] do
		  with py.misc do
		    begin
		      acc_tmp := mexp*((level+0.1)/lev);
		      i1 := trunc(acc_tmp);
		      acc_exp := acc_exp + (acc_tmp - i1);
		      if (acc_exp > 1) then
			begin
			  i1 := i1 + 1;
			  acc_exp := acc_exp - 1.0;
			end;
		      exp := exp + i1;
		      if (i1 > 0) then prt_experience;
		    end;
		mon_take_hit := mptr;
		delete_monster(monptr);
	      end
	    else
	      mon_take_hit := 0;
	  end
      end;


	{ Special damage due to magical abilities of object	-RAK-	}
    function tot_dam(item    : treasure_type;  tdam : integer;
		     monster : creature_type) : integer;
      begin
	with item do
	  if (tval in [10,11,12,20,21,22,23,77]) then
	    with monster do
	      begin
		{ Slay Dragon	}
	        if ((uand(cdefense,%X'0001') <> 0) and
	            (uand(flags,%X'00002000') <> 0)) then
		  tdam := tdam*4
		{ Slay Undead	}
	        else if ((uand(cdefense,%X'0008') <> 0) and
	                 (uand(flags,%X'00010000') <> 0)) then
		  tdam := tdam*3
		{ Slay Monster	}
	        else if ((uand(cdefense,%X'0002') <> 0) and
	                 (uand(flags,%X'00004000') <> 0)) then
		  tdam := tdam*2
		{ Slay Evil	}
	        else if ((uand(cdefense,%X'0004') <> 0) and
	                 (uand(flags,%X'00008000') <> 0)) then
		  tdam := tdam*2
		{ Frost		}
	        else if ((uand(cdefense,%X'0010') <> 0) and
	                 (uand(flags,%X'00020000') <> 0)) then
		  tdam := trunc(tdam*1.5)
		{ Fire		}
	        else if ((uand(cdefense,%X'0020') <> 0) and
	                 (uand(flags,%X'00040000') <> 0)) then
		  tdam := trunc(tdam*1.5);
	    end;
	tot_dam := tdam;
      end;


	{ Player attacks a (poor, defenseless) creature		-RAK-	}
    function py_attack(y,x : integer) : boolean;
      var
	i1,i2,i3,blows,tot_tohit	: integer;
	m_name,out_val			: vtype;
      begin
	py_attack := false;
	i1 := cave[y,x].cptr;
	i2 := m_list[i1].mptr;
	m_list[i1].csleep := 0;
		{ Does the player know what he's fighting?	}
	if ((uand(%X'10000',c_list[i2].cmove) <> 0) and
	    (not(py.flags.see_inv))) then
	  m_name := 'it'
	else if (py.flags.blind > 0) then
	  m_name := 'it'
	else if (not(m_list[i1].ml)) then
	  m_name := 'it'
	else
	  m_name := 'the ' + c_list[i2].name;
	if (inventory[23].tval > 0) then	{ Proper weapon	}
	  blows := attack_blows(inventory[23].weight,tot_tohit)
	else					{ Bare hands?	}
	  begin
	    blows := 2;
	    tot_tohit := -3;
	  end;
	if (inventory[23].tval in [10,11,12]) then { Fix for arrows}
	  blows := 1;
	tot_tohit := tot_tohit + py.misc.ptohit;
	{ Loop for number of blows, trying to hit the critter...	}
	with py.misc do
	  repeat
	    if (test_hit(bth,lev,tot_tohit,c_list[i2].ac)) then
	      begin
	        writev(out_val,'You hit ',m_name,'.');
	        msg_print(out_val);
		with inventory[23] do
		  begin
	            if (tval > 0) then		{ Weapon?	}
		      begin
		        i3 := damroll(damage);
		        i3 := tot_dam(inventory[23],i3,c_list[i2]);
		        i3 := critical_blow(weight,tot_tohit,i3);
		      end
	            else			{ Bare hands!?	}
		      begin
		        i3 := damroll(bare_hands);
		        i3 := critical_blow(1,0,i3);
		      end;
		  end;
	        i3 := i3 + ptodam;
	        if (i3 < 0) then i3 := 0;
	{ See if we done it in...				}
	        with m_list[i1] do
		  if (mon_take_hit(i1,i3) > 0) then
		    begin
		      writev(out_val,'You have slain ',m_name,'.');
		      msg_print(out_val);
		      blows := 0;
		      py_attack := false;
		    end
		  else
		    py_attack := true;	{ If creature hit, but alive...}
		  with inventory[23] do
		    if (tval in [10,11,12]) then	{ Use missles up}
		      begin
			number := number - 1;
			if (number <= 0) then
			  begin
			    inven_weight := inven_weight - weight;
			    equip_ctr := equip_ctr - 1;
			    inventory[inven_max] := inventory[23];
			    inventory[23] := blank_treasure;
			    py_bonuses(inventory[inven_max],-1);
			  end;
		      end;
	      end
	    else
	      begin
	        writev(out_val,'You miss ',m_name,'.');
	        msg_print(out_val);
	      end;
	    blows := blows - 1;
	  until (blows < 1);
      end;


	{ Moves player from one space to another...		-RAK-	}
	{ Note: This rountine has been pre-declared; see that for argument}
    procedure move_char;
      var
		test_row,test_col	: integer;
		panrow,pancol		: integer;
		i1,i2			: integer;
      begin
	test_row := char_row;
	test_col := char_col;
	if (py.flags.confused > 0) then		{ Confused?		}
	  if (randint(4) > 1) then		{ 75% random movement	}
	    if (dir <> 5) then			{ Never random if sitting}
	      begin
	        dir := randint(9);
	        find_flag := false;
	      end;
	if (move(dir,test_row,test_col)) then	{ Legal move?		}
	  with cave[test_row,test_col] do
	    if (cptr < 2) then			{ No creature?		}
	      begin
		if (fopen) then			{ Open floor spot	}
		  begin
			{ Move character record (-1)		}
		    move_rec(char_row,char_col,test_row,test_col);
			{ Check for new panel			}
		    if (get_panel(test_row,test_col)) then
		      prt_map;
			{ Check to see if he should stop	}
		    if (find_flag) then
		      area_affect(dir,test_row,test_col);
			{ Check to see if he notices something	}
		    if (py.flags.blind < 1) then
		      if ((randint(py.misc.fos) = 1) or (search_flag)) then
		        search(test_row,test_col,py.misc.srh);
			{ An object is beneath him...		}
		    if (tptr > 0) then
		      carry(test_row,test_col);
			{ Move the light source			}
		    move_light(char_row,char_col,test_row,test_col);
			{ A room of light should be lit...	}
		    if (fval = lopen_floor.ftval) then
		      begin
			if (py.flags.blind < 1) then
			  if (not(pl)) then
			    light_room(test_row,test_col);
		      end
			{ In doorway of light-room?		}
		    else if (fval in [5,6]) then
		      if (py.flags.blind < 1) then
		        begin
			  for i1 := (test_row - 1) to (test_row + 1) do
			    for i2 := (test_col - 1) to (test_col + 1) do
			      if (in_bounds(i1,i2)) then
				with cave[i1,i2] do
				  if (fval = lopen_floor.ftval) then
				    if (not(pl)) then
				      light_room(i1,i2);
		        end;
			{ Make final assignments of char co-ords}
		    char_row := test_row;
		    char_col := test_col;
	          end
		else	{Can't move onto floor space}
			{ Try a new direction if in find mode	}
		  if (not(pick_dir(dir))) then
		    begin
		      if (find_flag) then
		        begin
		          find_flag := false;
		          move_char(5);
		        end
		      else if (tptr > 0) then
		        begin
			  reset_flag := true;
		          if (t_list[tptr].tval = 103) then
		            msg_print('There is rubble blocking your way.')
		          else if (t_list[tptr].tval = 105) then
			    msg_print('There is a closed door blocking your way.');
		        end
		      else
			reset_flag := true;
	            end
	      end
	    else	{ Attacking a creature!	}
	      begin
		if (find_flag) then
		  begin
		    find_flag := false;
		    move_light(char_row,char_col,char_row,char_col);
		  end;
		if (py.flags.afraid < 1) then	{ Coward?	}
		  py_attack(test_row,test_col)
		else				{ Coward!	}
		  msg_print('You are too afraid!');
	      end
      end;


	{ Chests have traps too...				-RAK-	}
	{ Note: Chest traps are based on the FLAGS value		}
    procedure chest_trap(y,x : integer);
      var
	i1,i2,i3		: integer;
      begin
	with t_list[cave[y,x].tptr] do
	  begin
	    if (uand(%X'00000010',flags) <> 0) then
	      begin
		msg_print('A small needle has pricked you!');
		if (not py.flags.sustain_str) then
		  begin
		    py.stat.cstr := de_statp(py.stat.cstr);
		    take_hit(damroll('1d4'),'a poison needle.');
		    print_stat := uor(%X'0001',print_stat);
		    msg_print('You feel weakened!');
		  end
		else
		  msg_print('You are unaffected.');
	      end;
	    if (uand(%X'00000020',flags) <> 0) then
	      begin
		msg_print('A small needle has pricked you!');
		take_hit(damroll('1d6'),'a poison needle.');
		py.flags.poisoned := py.flags.poisoned + 10 + randint(20);
	      end;
	    if (uand(%X'00000040',flags) <> 0) then
	      begin
		msg_print('A puff of yellow gas surrounds you!');
		if (py.flags.free_act) then
		  msg_print('You are unaffected.')
		else
		  begin
		    msg_print('You choke and pass out.');
		    py.flags.paralysis := 10 + randint(20);
		  end;
	      end;
	    if (uand(%X'00000080',flags) <> 0) then
	      begin
		msg_print('There is a sudden explosion!');
		delete_object(y,x);
		take_hit(damroll('5d8'),'an exploding chest.');
	      end;
	    if (uand(%X'00000100',flags) <> 0) then
	      begin
		for i1 := 1 to 3 do
		  begin
		    i2 := y;
		    i3 := x;
		    summon_monster(i2,i3,false);
		  end;
	      end;
	  end;
      end;


	{ Opens a closed door or closed chest...		-RAK-	}
    procedure openobject;
      var
		y,x,tmp		: integer;
		flag		: boolean;
      begin
	y := char_row;
	x := char_col;
	if (get_dir('Which direction?',tmp,tmp,y,x)) then
	  begin
	    with cave[y,x] do
	      if (tptr > 0) then
			{ Closed door		}
	        if (t_list[tptr].tval = 105) then
		  with t_list[tptr] do
		    begin
		      if (p1 > 0) then	{ It's locked...	}
			begin
		          with py.misc do
		            tmp := disarm + lev + 2*todis_adj + int_adj;
			  if (py.flags.confused > 0) then
			    msg_print('You are too confused to pick the lock.')
			  else if ((tmp-p1) > randint(100)) then
			    begin
			      msg_print('You have picked the lock.');
			      py.misc.exp := py.misc.exp + 1;
			      prt_experience;
			      p1 := 0;
			    end
			  else
			    msg_print('You failed to pick the lock.');
			end
		      else if (p1 < 0) then	{ It's stuck	}
			msg_print('It appears to be stuck.');
		      if (p1 = 0) then
	                begin
			  t_list[tptr] := door_list[1];
			  fopen := true;
			  lite_spot(y,x);
	                end;
		    end
			{ Open a closed chest...		}
		else if (t_list[tptr].tval = 2) then
		  begin
		    with py.misc do
		      tmp := disarm + lev + 2*todis_adj + int_adj;
		    with t_list[tptr] do
		      begin
			flag := false;
		        if (uand(%X'00000001',flags) <> 0) then
			  if (py.flags.confused > 0) then
			    msg_print('You are too confused to pick the lock.')
			  else if ((tmp-(2*level)) > randint(100)) then
			    begin
			      msg_print('You have picked the lock.');
			      flag := true;
			      py.misc.exp := py.misc.exp + level;
			      prt_experience;
			    end
			  else
			    msg_print('You failed to pick the lock.')
			else
			  flag := true;
			if (flag) then
			  begin
			    flags := uand(%X'FFFFFFFE',flags);
			    tmp := index(name,' (');
			    if (tmp > 0) then
			      name := substr(name,1,tmp-1);
			    name := name + ' (Empty)';
			    known2(name);
			    cost := 0;
			  end;
			flag := false;
			{ Was chest still trapped?  (Snicker)	}
		        if (uand(%X'00000001',flags) = 0) then
			  begin
			    chest_trap(y,x);
			    if (tptr > 0) then
			      flag := true
			  end;
		      end;
			{ Chest treasure is allocted as if a creature	}
			{ had been killed...				}
		    if (flag) then
		      begin
			monster_death(y,x,t_list[tptr].flags);
			t_list[tptr].flags := 0;
		      end;
		  end
	        else
	          msg_print('I do not see anything you can open there.')
	      else
	        msg_print('I do not see anything you can open there.')
	  end;
      end;


	{ Closes an open door...				-RAK-	}
    procedure closeobject;
      var
		y,x,tmp		: integer;
      begin
	y := char_row;
	x := char_col;
	if (get_dir('Which direction?',tmp,tmp,y,x)) then
	  begin
	    with cave[y,x] do
	      if (tptr > 0) then
	        if (t_list[tptr].tval = 104) then
		  if (cptr = 0) then
		    if (t_list[tptr].p1 = 0) then
	              begin
		        t_list[tptr] := door_list[2];
		        fopen := false;
		        lite_spot(y,x);
	              end
		    else
		      msg_print('The door appears to be broken.')
		  else
		    msg_print('The ' + c_list[m_list[cptr].mptr].name +
				' is in your way!')
	        else
	          msg_print('I do not see anything you can close there.')
	      else
	        msg_print('I do not see anything you can close there.')
	  end;
      end;


	{ Go up one level					-RAK-	}
    procedure go_up;
      begin
	with cave[char_row,char_col] do
	if (tptr > 0) then
	  if (t_list[tptr].tval = 107) then
	    begin
	      dun_level := dun_level - 1;
	      moria_flag := true;
	      msg_print('You enter a maze of up staircases.');
	      msg_print('You pass through a one-way door.');
	    end
	  else
	    msg_print('I see no up staircase here.')
	else
	  msg_print('I see no up staircase here.');
      end;


	{ Go down one level					-RAK-	}
    procedure go_down;
      begin
	with cave[char_row,char_col] do
	if (tptr > 0) then
	  if (t_list[tptr].tval = 108) then
	    begin
	      dun_level := dun_level + 1;
	      moria_flag := true;
	      msg_print('You enter a maze of down staircases.');
	      msg_print('You pass through a one-way door.');
	    end
	  else
	    msg_print('I see no down staircase here.')
	else
	  msg_print('I see no down staircase here.');
      end;


	{ Tunneling through real wall: 10,11,12			-RAK-	}
	{ Used by TUNNEL and WALL_TO_MUD				}
      function twall(y,x,t1,t2 : integer) : boolean;
	begin
	  twall := false;
	  with cave[y,x] do
	    if (t1 > t2) then
	      begin
		if (next_to4(y,x,[1,2]) > 0) then
		  begin
		    fval  := corr_floor2.ftval;
		    fopen := corr_floor2.ftopen;
		  end
		else
		  begin
		    fval  := corr_floor1.ftval;
		    fopen := corr_floor1.ftopen;
		  end;
		if (test_light(y,x)) then
		  if (panel_contains(y,x)) then
		    begin
		      if (tptr > 0) then
		        msg_print('You have found something!');
		      lite_spot(y,x);
		    end;
	        fm := false;
	        pl := false;
		twall := true;
	      end;
	end;


	{ Tunnels through rubble and walls			-RAK-	}
	{ Must take into account: secret doors, special tools		}
    procedure tunnel;
      var
		y,x,i1,i2,tabil		: integer;
      begin
	y := char_row;
	x := char_col;
	if (get_dir('Which direction?',i1,i1,y,x)) then
	  with cave[y,x] do
	    begin
	{ Compute the digging ability of player; based on 	}
	{ strength, and type of tool used			}
	      tabil := py.stat.cstr;
	      if (inventory[23].tval > 0) then
		with inventory[23] do
		  if (uand(%X'20000000',flags) <> 0) then
		    tabil := tabil + 25 + p1*50;
	{ Regular walls; Granite, magma intrusion, quartz vein	}
	{ Don't forget the boundry walls, made of titanium (255)}
	      case fval of
		10 : begin
			i1 := randint(1200) + 80;
			if (twall(y,x,tabil,i1)) then
			  msg_print('You have finished the tunnel.')
			else 
			  msg_print('You tunnel into the granite wall.');
		      end;
		11 : begin
			i1 := randint(600) + 10;
			if (twall(y,x,tabil,i1)) then
			  msg_print('You have finished the tunnel.')
			else 
			  msg_print('You tunnel into the magma intrusion.');
		      end;
		12 : begin
			i1 := randint(400) + 10;
			if (twall(y,x,tabil,i1)) then
			  msg_print('You have finished the tunnel.')
			else 
			  msg_print('You tunnel into the quartz vein.');
		      end;
		15 : msg_print('This seems to be permanent rock.');
		otherwise begin
	{ Is there an object in the way?  (Rubble and secret doors)}
		  if (tptr > 0) then
		    begin
		{ Rubble...	}
		      if (t_list[tptr].tval = 103) then
		        begin
		          if (tabil > randint(180)) then
		            begin
		              pusht(tptr);
		              tptr := 0;
		              fm := false;
		              fopen := true;
		              msg_print('You have removed the rubble.');
		              if (randint(10) = 1) then
			        begin
			          place_object(y,x);
			          if (test_light(y,x)) then
			            msg_print('You have found something!');
			        end;
		              lite_spot(y,x);
		            end
		          else
		            msg_print('You dig in the rubble...');
		        end
		{ Secret doors...}
		      else if (t_list[tptr].tval = 109) then
		        begin
		          msg_print('You tunnel into the granite wall.');
		          search(char_row,char_col,py.misc.srh);
		        end
	              else
		        msg_print('You can''t tunnel through that.');
		    end
		  else
		    msg_print('Tunnel through what?  Empty air???');
		end;
	      end;
	    end;
      end;


	{ Disarms a trap					-RAK-	}
    procedure disarm_trap;
      var
	y,x,i1,tdir				: integer;
	tot,t1,t2,t3,t4,t5			: integer;
      begin
	y := char_row;
	x := char_col;
	if (get_dir('Which direction?',tdir,i1,y,x)) then
	  with cave[y,x] do
	    begin
	      if (tptr > 0) then
		begin
		  t1 := py.misc.disarm;	{ Ability to disarm	}
		  t2 := py.misc.lev;	{ Level adjustment	}
		  t3 := 2*todis_adj;	{ Dexterity adjustment	}
		  t4 := int_adj;	{ Intelligence adjustment}
		  tot := t1 + t2 + t3 + t4;
		  if (py.flags.blind > 0) then
		    tot := trunc(tot/5.0)
		  else if (no_light) then 
		    tot := trunc(tot/2.0);
		  if (py.flags.confused > 0) then
		    tot := trunc(tot/3.0);
		  i1 := t_list[tptr].tval;
		  t5 := t_list[tptr].level;
		  if (i1 = 102) then		{ Floor trap	}
		    with t_list[tptr] do
		      begin
		        if ((tot - t5) > randint(100)) then
		          begin
		            msg_print('You have disarmed the trap.');
			    py.misc.exp := py.misc.exp + p1;
			    fm := false;
		            pusht(tptr);
		            tptr := 0;
			    move_char(tdir);
		            lite_spot(y,x);
			    prt_experience;
			  end
		        else if (randint(tot) > 5) then
		          msg_print('You failed to disarm the trap.')
		        else
			  begin
			    msg_print('You set the trap off!');
			    move_char(tdir);
			  end;
		      end
		  else if (i1 = 2) then		{ Chest trap	}
		    with t_list[tptr] do
		      begin
			if (index(name,'^') > 0) then
			  msg_print('I don''t see a trap...')
			else if (uand(%X'000001F0',flags) <> 0) then
			  begin
		            if ((tot - t5) > randint(100)) then
			      begin
				flags := uand(%X'FFFFFE0F',flags);
				i1 := index(name,' (');
				if (i1 > 0) then
				  name := substr(name,1,i1-1);
				if (uand(%X'00000001',flags) <> 0) then
				  name := name + ' (Locked)'
				else
				  name := name + ' (Disarmed)';
				msg_print('You have disarmed the chest.');
				known2(name);
			        py.misc.exp := py.misc.exp + t5;
			        prt_experience;
			      end
			    else if (randint(tot) > 5) then
			      msg_print('You failed to disarm the chest.')
			    else
			      begin
				msg_print('You set a trap off!');
				known2(name);
				chest_trap(y,x);
			      end;
			  end
			else
			  msg_print('The chest was not trapped.');
		      end
	          else
		    msg_print('I do not see anything to disarm there.');
		end
	      else
		msg_print('I do not see anything to disarm there.');
	    end
      end;


	{ Look at an object, trap, or monster			-RAK-	}
	{ Note: Looking is a free move, see where invoked...		}
    procedure look;
      var
		i1,i2,y,x	: integer;
		dir,dummy	: integer;
		flag		: boolean;
      begin
	flag := false;
	y := char_row;
	x := char_col;
	if (get_dir('Look which direction?',dir,dummy,y,x)) then
	  if (py.flags.blind < 1) then
	    begin
	      y := char_row;
	      x := char_col;
	      i1 := 0;
	      repeat
	        move(dir,y,x);
	        with cave[y,x] do
		  begin
		    if (cptr > 1) then
		      if (m_list[cptr].ml) then
			begin
			  i2 := m_list[cptr].mptr;
			  writev(out_val,'You see a ',c_list[i2].name,'.');
			  msg_print(out_val);
			  flag := true;
			end;
		    if ((tl) or (pl) or (fm)) then
		      begin
	                if (tptr > 0) then
		          if (t_list[tptr].tval = 109) then
			    msg_print('You see a granite wall.')
		          else if (t_list[tptr].tval <> 101) then
		            begin
		              inventory[inven_max] := t_list[tptr];
		              objdes(out_val,inven_max,true);
		              msg_print('You see ' + out_val);
		              flag := true;
		            end;
		        if (not(fopen)) then
		          begin
			    flag := true;
			    case fval of
			      10 : msg_print('You see a granite wall.');
			      11 : msg_print('You see some dark rock.');
			      12 : msg_print('You see a quartz vein.');
			      15 : msg_print('You see a granite wall.');
			      otherwise ;
			    end;
			  end;
		      end;
		    end;
		i1 := i1 + 1;
	      until ((not cave[y,x].fopen) or (i1 > max_sight));
	      if (not flag) then
	        msg_print('You see nothing of interest in that direction.');
	    end
	  else
	    msg_print('You can''t see a damn thing!');
      end;


	{ Add to the players food time				-RAK-	}
    procedure add_food(num : integer);
      begin
	with py.flags do
	  begin
	    if (food < 0) then food := 0;
	    food := food + num;
	    if (food > player_food_full) then msg_print('You are full.');
	    if (food > player_food_max) then
	      begin
		msg_print('You''re getting fat from eating so much.');
		food := player_food_max;
	      end;
	  end;
      end;


	{ Describe number of remaining charges...		-RAK-	}
    procedure desc_charges(item_val : integer);
      var
	rem_num			: integer;
	out_val			: vtype;
      begin
	if (index(inventory[item_val].name,'^') = 0) then
	  begin
            rem_num := inventory[item_val].p1;
	    writev(out_val,'You have ',rem_num:1,' charges remaining.');
            msg_print(out_val);
	  end;
      end;


	{ Describe amount of item remaining...			-RAK-	}
    procedure desc_remain(item_val : integer);
      var
	out_val			: vtype;
      begin
        inventory[inven_max] := inventory[item_val];
        with inventory[inven_max] do
	  number := number - 1;
        objdes(out_val,inven_max,true);
        out_val := 'You have ' + substr(out_val,1,(length(out_val)-1)) + '.';
        msg_print(out_val);
      end;


	{ Throw an object across the dungeon...			-RAK-	}
	{ Note: Flasks of oil do fire damage				}
	{ Note: Extra damage and chance of hitting when missles are used}
	{       with correct weapon.  I.E.  wield bow and throw arrow.	}
    procedure throw_object;
      var
	item_val,tbth,tpth,tdam,tdis		: integer;
	y_dumy,x_dumy,dumy			: integer;
	y,x,oldy,oldx,dir,cur_dis		: integer;
	redraw,flag				: boolean;
	out_val					: vtype;

      procedure inven_throw(item_val : integer);
	begin
	  inventory[inven_max] := inventory[item_val];
	  inventory[inven_max].number := 1;
	  with inventory[item_val] do
	    begin
	      if ((number > 1) and (subval > 511)) then
		begin
		  number := number - 1;
		  inven_weight := inven_weight - weight;
		end
	      else
		inven_destroy(item_val);
	    end;
	end;

      procedure facts(var tbth,tpth,tdam,tdis : integer);
	var
		tmp_weight			: integer;
	begin
	  with inventory[inven_max] do
	    begin
	      if (weight < 1) then 
		tmp_weight := 1
	      else
		tmp_weight := weight;

		{ Throwing objects			}
	      tdam := damroll(damage) + todam;
	      tbth := trunc(py.misc.bthb*0.75);
	      tpth := py.misc.ptohit  + tohit;
	      tdis := trunc(((py.stat.cstr+20)*10)/tmp_weight);
	      if (tdis > 10) then tdis := 10;

		{ Using Bows, slings, or crossbows	}
	      if (inventory[23].tval = 20) then
		case inventory[23].p1 of
		  1 : if (tval = 10) then	{ Sling and Bullet	 }
			begin
			  tbth := py.misc.bthb;
			  tpth := tpth + inventory[23].tohit;
			  tdam := tdam + 2;
			  tdis := 20;
			end;
		  2 : if (tval = 12) then	{ Short Bow and Arrow	 }
			begin
			  tbth := py.misc.bthb;
			  tpth := tpth + inventory[23].tohit;
			  tdam := tdam + 2;
			  tdis := 25;
			end;
		  3 : if (tval = 12) then	{ Long Bow and Arrow	 }
			begin
			  tbth := py.misc.bthb;
			  tpth := tpth + inventory[23].tohit;
			  tdam := tdam + 3;
			  tdis := 30;
			end;
		  4 : if (tval = 12) then	{ Composite Bow and Arrow}
			begin
			  tbth := py.misc.bthb;
			  tpth := tpth + inventory[23].tohit;
			  tdam := tdam + 4;
			  tdis := 35;
			end;
		  5 : if (tval = 11) then	{ Light Crossbow and Bolt}
			begin
			  tbth := py.misc.bthb;
			  tpth := tpth + inventory[23].tohit;
			  tdam := tdam + 2;
			  tdis := 25;
			end;
		  6 : if (tval = 11) then	{ Heavy Crossbow and Bolt}
			begin
			  tbth := py.misc.bthb;
			  tpth := tpth + inventory[23].tohit;
			  tdam := tdam + 4;
			  tdis := 35;
			end;
		end
	    end;
	end;

      procedure drop_throw(y,x : integer);
	var
		i1,i2,i3,cur_pos		: integer;
		flag				: boolean;
		out_val				: vtype;
	begin
	  flag := false;
	  i1 := y;
	  i2 := x;
	  i3 := 0;
	  if (randint(10) > 1) then
	    repeat
	      if (in_bounds(i1,i2)) then
	        with cave[i1,i2] do
		  if (fopen) then
		    if (tptr = 0) then
		      flag := true;
	      if (not(flag)) then
		begin
		  i1 := y + randint(3) - 2;
		  i2 := x + randint(3) - 2;
		  i3 := i3 + 1;
		end;
	    until((flag) or (i3 > 9));
	  if (flag) then
	    begin
              popt(cur_pos);
	      cave[i1,i2].tptr := cur_pos;
	      t_list[cur_pos] := inventory[inven_max];
	      if (test_light(i1,i2)) then
		lite_spot(i1,i2);
	    end
	  else
	    begin
	      objdes(out_val,inven_max,false);
	      msg_print('The ' + out_val + ' dissapears.');
	    end;
	end;

      begin
	redraw := false;
	if (inven_ctr = 0) then
	  msg_print('But you are not carrying anything.')
	else if (get_item(item_val,'Fire/Throw which one?',
						redraw,1,inven_ctr)) then
	  begin
	    if (redraw) then
	      draw_cave;
	    y_dumy := char_row;
	    x_dumy := char_col;
	    if (get_dir('Which direction?',dir,dumy,y_dumy,x_dumy)) then
	      begin
		desc_remain(item_val);
		if (py.flags.confused > 0) then
		  begin
		    msg_print('You are confused...');
		    repeat
		      dir := randint(9);
		    until(dir <> 5);
		  end;
		inven_throw(item_val);
		facts(tbth,tpth,tdam,tdis);
		with inventory[inven_max] do
		  begin
		    flag := false;
		    y := char_row;
		    x := char_col;
		    oldy := char_row;
		    oldx := char_col;
		    cur_dis := 0;
		    repeat
		      move(dir,y,x);
		      cur_dis := cur_dis + 1;
		      if (test_light(oldy,oldx)) then
		        lite_spot(oldy,oldx);
		      if (cur_dis > tdis) then flag := true;
		      with cave[y,x] do
		        begin
			  if ((fopen) and (not(flag))) then
			    begin
			      if (cptr > 1) then
			        begin
				  flag := true;
				  with m_list[cptr] do
				    begin
				      tbth := tbth - cur_dis;
				      if (test_hit(tbth,py.misc.lev,tpth,
							c_list[mptr].ac)) then
				        begin
				          i1 := mptr;
					  objdes(out_val,inven_max,false);
	msg_print('The ' + out_val + ' hits the ' + c_list[i1].name + '.');
	tdam := tot_dam(inventory[inven_max],tdam,c_list[i1]);
	with inventory[inven_max] do
	  tdam := critical_blow(weight,tpth,tdam);
	i1 := mon_take_hit(cptr,tdam);
	if (i1 > 0) then
	  msg_print('You have killed the ' + c_list[i1].name + '.');
				        end
				      else
				        drop_throw(oldy,oldx);
				    end;
			        end
			      else
			        begin
				  if (panel_contains(y,x)) then
				    if (test_light(y,x)) then
				      print(tchar,y,x);
			        end;
			    end
			  else
			    begin
			      flag := true;
			      drop_throw(oldy,oldx);
			    end;
		        end;
		      oldy := y;
		      oldx := x;
	            until (flag);
		  end;
	      end
	  end
	else
	  if (redraw) then
	    draw_cave;
      end;


	{ Bash open a door or chest				-RAK-	}
	{ Note: Affected by strength and weight of character		}
    procedure bash;
      var
	y,x,tmp					: integer;
	old_ptodam,old_ptohit,old_bth		: integer;
      begin
	y := char_row;
	x := char_col;
	if (get_dir('Which direction?',tmp,tmp,y,x)) then
	  begin
	    with cave[y,x] do
	      if (cptr > 1) then
		begin
		  if (py.flags.afraid > 0) then
		    msg_print('You are afraid!')
		  else 
		    begin
			{ Save old values of attacking	}
		      inventory[inven_max] := inventory[23];
		      old_ptohit := py.misc.ptohit;
		      old_ptodam := py.misc.ptodam;
		      old_bth    := py.misc.bth;
			{ Use these values		}
		      inventory[23] := blank_treasure;
		      with inventory[23] do
			begin
			  damage := inventory[27].damage;
			  weight := py.stat.cstr;
			  tval   := 1;
			end;
		      with py do
			begin
		          misc.bth    := trunc((stat.cstr+misc.wt)/6.0);
		          misc.ptohit := 0;
			  misc.ptodam := trunc(misc.wt/75.0) + 1;
			end;
		      if (py_attack(y,x)) then
			with m_list[cptr] do
			  begin
			    stuned := randint(2) + 1;
			    if (stuned > 24) then stuned := 24;
			    msg_print('The ' + c_list[mptr].name
					+ ' appears stunned!');
			end;
			{ Restore old values		}
		      inventory[23] := inventory[inven_max];
		      py.misc.ptohit := old_ptohit;
		      py.misc.ptodam := old_ptodam;
		      py.misc.bth    := old_bth;
		      if (randint(140) > py.stat.cdex) then
			begin
			  msg_print('You are off-balance.');
			  py.flags.paralysis := randint(3);
			end;
		    end;
		end
	      else if (tptr > 0) then
		with t_list[tptr] do
	          if (tval = 105) then
		    begin
		      msg_print('You smash into the door!');
		      with py do
		        if (test_hit(misc.wt+stat.cstr,0,0,abs(p1)+150)) then
			  begin
			    msg_print('The door crashes open!');
		            t_list[tptr] := door_list[1];
			    p1 := 1;
		            fopen := true;
		            lite_spot(y,x);
	                  end
		        else
			  begin
			    msg_print('The door holds firm.');
			    py.flags.paralysis := 2;
			  end;
		    end
		  else if (tval = 2) then
		    begin
		      if (randint(10) = 1) then
			begin
			  msg_print('You have destroyed the chest...');
			  msg_print('and it''s contents!');
			  name := '& ruined chest';
			  flags := 0;
			end
		      else if (uand(%X'00000001',flags) <> 0) then
			if (randint(10) = 1) then
			  begin
			    msg_print('The lock breaks open!');
			    flags := uand(%X'FFFFFFFE',flags);
			  end;
		    end
	          else
	            msg_print('I do not see anything you can bash there.')
	      else
	        msg_print('I do not see anything you can bash there.');
	  end;
      end;


	{ Jam a closed door					-RAK-	}
    procedure jamdoor;
      var
		y,x,tmp		: integer;
      begin
	y := char_row;
	x := char_col;
	if (get_dir('Which direction?',tmp,tmp,y,x)) then
	  begin
	    with cave[y,x] do
	      if (tptr > 0) then
		with t_list[tptr] do
	          if (tval = 105) then
		    if (cptr = 0) then
		      begin
			if (find_range([13],i1,i2)) then
			  begin
			    msg_print('You jam the door with a spike.');
			    with inventory[i1] do
			      if (number > 1) then
				number := number - 1
			      else
				inven_destroy(i1);
			    p1 := -abs(p1) - 20;
			  end
			else
			  msg_print('But you have no spikes...');
		      end
		    else
		      msg_print('The ' + c_list[m_list[cptr].mptr].name +
				' is in your way!')
	          else if (tval = 104) then
		    msg_print('The door must be closed first.')
		  else
	            msg_print('That isn''t a door!')
	      else
	        msg_print('That isn''t a door!');
	  end;
      end;


	{ Refill the players lamp				-RAK-	}
    procedure refill_lamp;
      var
	i1,i2,i3			: integer;
	out_val				: vtype;
      begin
	i3 := inventory[33].subval;
	if ((i3 > 0) and (i3 < 10)) then
	  if (find_range([77],i1,i2)) then
	    begin
	      msg_print('Your lamp is full.');
	      with inventory[33] do
		begin
	          p1 := p1 + inventory[i1].p1;
		  if (p1 > obj$lamp_max) then p1 := obj$lamp_max;
		end;
	      desc_remain(i1);
	      inven_destroy(i1);
	    end
	  else
	    msg_print('You have no oil.')
	else
	  msg_print('But you are not using a lamp.');
      end;

	{ General spells and misc routines	}
	%INCLUDE 'MOR_INCLUDE:SPELLS.INC'
	{ Wizard and debug routines		}
	%INCLUDE 'MOR_INCLUDE:WIZARD.INC'
	{ Creature movement and attacks		}
	%INCLUDE 'MOR_INCLUDE:CREATURE.INC'
	{ Using objects				}
	%INCLUDE 'MOR_INCLUDE:SCROLLS.INC'
	%INCLUDE 'MOR_INCLUDE:POTIONS.INC'
	%INCLUDE 'MOR_INCLUDE:EAT.INC'
	%INCLUDE 'MOR_INCLUDE:WANDS.INC'
	%INCLUDE 'MOR_INCLUDE:STAFFS.INC'
	{ Spell casting				}
	%INCLUDE 'MOR_INCLUDE:MAGIC.INC'
	%INCLUDE 'MOR_INCLUDE:PRAYER.INC'

	{ Main procedure for dungeon...				-RAK-	}
	{ Note: There is a lot of prelinimary magic going on here at first}
    begin
	{ Check light status for setup		}
      with inventory[33] do
	if (p1 > 0) then
	  player_light := true
	else
	  player_light := false;
	{ Check for a maximum level		}
      with py.misc do
	if (dun_level > max_lev) then max_lev := dun_level;
	{ Set up the character co-ords		}
      if ((char_row = -1) or (char_col = -1)) then
        new_spot(char_row,char_col);
	{ Reset flags and initialize variables	}
      moria_flag    := false;
      cave_flag     := false;
      find_flag     := false;
      search_flag   := false;
      teleport_flag := false;
      mon_tot_mult  := 0;
      cave[char_row,char_col].cptr := 1;
      old_chp   := trunc(py.misc.chp);
      old_cmana := trunc(py.misc.cmana);
	{ Light up the area around character	}
      move_char(5);
	{ Light, but do not move critters	}
      creatures(false);
	{ Print the depth			}
      prt_depth;

	{ Loop until dead, or new level			}
      repeat

	{ Increment turn counter			}
	turn := turn + 1;
	{ Check for game hours				}
	if (not(wizard1)) then
	  if ((turn mod 250) = 1) then
	    if (not(check_time)) then
	      if (closing_flag > 4) then
		begin
		  if (search_flag) then
		    search_off;
		  if (py.flags.rest > 0) then
		    rest_off;
		  find_flag := false;
		  msg_print('The gates to Moria are now closed.');
		  msg_print('');
	          repeat
	            save_char;
	          until(false);
		end
	      else
		begin
		  if (search_flag) then
		    search_off;
		  if (py.flags.rest > 0) then
		    rest_off;
		  find_flag := false;
		  move_char(5);
	          closing_flag := closing_flag + 1;
		  msg_print('The gates to Moria are closing...');
		  msg_print('Please finish up or save your game.');
		  msg_print('');
		end;

	{ Check for creature generation			}
	if (randint(max_malloc_chance) = 1) then
	  alloc_monster(floor_set,1,max_sight,false);
	{ Screen may need updating, used mostly for stats}
	if (print_stat > 0) then
	  begin
	    if (uand(%X'0001',print_stat) <> 0) then
	      prt_strength;
	    if (uand(%X'0002',print_stat) <> 0) then
	      prt_dexterity;
	    if (uand(%X'0004',print_stat) <> 0) then
	      prt_constitution;
	    if (uand(%X'0008',print_stat) <> 0) then
	      prt_intelligence;
	    if (uand(%X'0010',print_stat) <> 0) then
	      prt_wisdom;
	    if (uand(%X'0020',print_stat) <> 0) then
	      prt_charisma;
	    if (uand(%X'0040',print_stat) <> 0) then
	      prt_pac;
	    if (uand(%X'0100',print_stat) <> 0) then
	      prt_mhp;
	    if (uand(%X'0200',print_stat) <> 0) then
	      prt_title;
	    if (uand(%X'0400',print_stat) <> 0) then
	      prt_level;
	  end;
	{ Check light status				}
	with inventory[33] do
	  if (player_light) then
	    if (p1 > 0) then
	      begin
	        p1 := p1 - 1;
		if (p1 = 0) then
		  begin
		    msg_print('Your light has gone out!');
	            player_light := false;
		    find_flag := false;
		    move_light(char_row,char_col,char_row,char_col);
		  end
		else if (p1 < 40) then
		  if (randint(5) = 1) then
		    begin
		      if (find_flag) then
			begin
			  find_flag := false;
			  move_light(char_row,char_col,char_row,char_col);
			end;
		      msg_print('Your light is growing faint.');
		    end;
	      end
	    else
	      begin
	        player_light := false;
		find_flag := false;
		move_light(char_row,char_col,char_row,char_col);
	      end
	  else if (p1 > 0) then
	    begin
	      p1 := p1 - 1;
	      player_light := true;
	      move_light(char_row,char_col,char_row,char_col);
	    end;

	{ Update counters and messages			}
	with py.flags do
	  begin
		{ Check food status		}
	    regen_amount := player$regen_normal;
	    if (food < player_food_alert) then
	      begin
	        if (food < player_food_weak) then
		  begin
	            if (food < 0) then
	              regen_amount := 0
		    else if (food < player_food_faint) then
		      regen_amount := player$regen_faint
	            else if (food < player_food_weak) then
	              regen_amount := player$regen_weak;
		    if (uand(%X'00000002',status) = 0) then
		      begin
		        status := uor(%X'00000003',status);
		        msg_print('You are getting weak from hunger.');
		        if (find_flag) then
		          begin
			    find_flag := false;
			    move_char(5);
		          end;
		        prt_hunger;
		      end;
		    if (food < player_food_faint) then
		      if (randint(8) = 1) then
		        begin
		          paralysis := paralysis + randint(5);
		          msg_print('You faint from the lack of food.');
		          if (find_flag) then
		            begin
			      find_flag := false;
			      move_char(5);
		            end;
		        end;
		  end
	        else
	          begin
		    if (uand(%X'00000001',status) = 0) then
		      begin
		        status := uor(%X'00000001',status);
		        msg_print('You are getting hungry.');
		        if (find_flag) then
		          begin
			    find_flag := false;
			    move_char(5);
		          end;
		        prt_hunger;
		      end;
		  end;
	      end;
	{ Food consumtion	}
	{ Note: Speeded up characters really burn up the food!	}
	    if (speed < 0) then
	      food := food - (speed*speed) - food_digested
	    else
	      food := food - food_digested;
	{ Regenerate		}
	    with py.misc do
	      begin
	        if (regenerate) then regen_amount := regen_amount*2;
	        if (py.flags.poisoned < 1) then
	          if (chp < mhp) then
	            regenhp(regen_amount);
	        if (cmana < mana) then
		  regenmana(regen_amount);
	      end;
	{ Blindness		}
	    if (blind > 0) then
	      begin
		if (uand(%X'00000004',status) = 0) then
		  begin
		    status := uor(%X'00000004',status);
		    prt_map;
		    prt_blind;
		    if (search_flag) then
		      search_off;
		  end;
	        blind := blind - 1;
		if (blind = 0) then
		  begin
		    status := uand(%X'FFFFFFFB',status);
		    prt_blind;
		    prt_map;
		    msg_print('The veil of darkness lifts.');
		    if (find_flag) then
		      begin
			find_flag := false;
			move_char(5);
		      end;
		  end;
	      end;
	{ Confusion		}
	    if (confused > 0) then
	      begin
		if (uand(%X'00000008',status) = 0) then
		  begin
		    status := uor(%X'00000008',status);
		    prt_confused;
		  end;
		confused := confused - 1;
		if (confused = 0) then
		  begin
		    status := uand(%X'FFFFFFF7',status);
		    prt_confused;
		    msg_print('You feel less confused now.');
		    if (find_flag) then
		      begin
			find_flag := false;
			move_char(5);
		      end;
		  end;
	      end;
	{ Afraid		}
	    if (afraid > 0) then
	      begin
		if (uand(%X'00000010',status) = 0) then
		  begin
		    if ((shero+hero) > 0) then
		      afraid := 0
		    else
		      begin
		        status := uor(%X'00000010',status);
		        prt_afraid;
		      end;
		  end
		else if ((shero+hero) > 0) then
		  afraid := 1;
		afraid := afraid - 1;
		if (afraid = 0) then
		  begin
		    status := uand(%X'FFFFFFEF',status);
		    prt_afraid;
		    msg_print('You feel bolder now.');
		    if (find_flag) then
		      begin
			find_flag := false;
			move_char(5);
		      end;
		  end;
	      end;
	{ Poisoned		}
	    if (poisoned > 0) then
	      begin
		if (uand(%X'00000020',status) = 0) then
		  begin
		    status := uor(%X'00000020',status);
		    prt_poisoned;
		  end;
		poisoned := poisoned - 1;
		if (poisoned = 0) then
		  begin
		    status := uand(%X'FFFFFFDF',status);
		    prt_poisoned;
		    msg_print('You feel better.');
		    if (find_flag) then
		      begin
			find_flag := false;
			move_char(5);
		      end;
		  end
		else
		  begin
		    case con_adj of
			-4	: take_hit(4,'poison.');
			-3,-2	: take_hit(3,'poison.');
			-1	: take_hit(2,'poison.');
			0	: take_hit(1,'poison.');
			1,2,3	: if ((turn mod 2) = 0) then
				    take_hit(1,'poison.');
			4,5	: if ((turn mod 3) = 0) then
				    take_hit(1,'poison.');
			6	: if ((turn mod 4) = 0) then
				    take_hit(1,'poison.');
		    end;
		  end;
	      end;
	{ Fast			}
	    if (fast > 0) then
	      begin
		if (uand(%X'00000040',status) = 0) then
		  begin
		    status := uor(%X'00000040',status);
		    msg_print('You feel yourself moving faster.');
		    change_speed(-1);
		    if (find_flag) then
		      begin
			find_flag := false;
			move_char(5);
		      end;
		  end;
		fast := fast - 1;
		if (fast = 0) then
		  begin
		    status := uand(%X'FFFFFFBF',status);
		    msg_print('You feel yourself slow down.');
		    change_speed(+1);
		    if (find_flag) then
		      begin
			find_flag := false;
			move_char(5);
		      end;
		  end;
	      end;
	{ Slow			}
	    if (slow > 0) then
	      begin
		if (uand(%X'00000080',status) = 0) then
		  begin
		    status := uor(%X'00000080',status);
		    msg_print('You feel yourself moving slower.');
		    change_speed(+1);
		    if (find_flag) then
		      begin
			find_flag := false;
			move_char(5);
		      end;
		  end;
		slow := slow - 1;
		if (slow = 0) then
		  begin
		    status := uand(%X'FFFFFF7F',status);
		    msg_print('You feel yourself speed up.');
		    change_speed(-1);
		    if (find_flag) then
		      begin
			find_flag := false;
			move_char(5);
		      end;
		  end;
	      end;
	{ Resting is over?	}
	    if (rest > 0) then
	      begin
		  { Hibernate every 20 iterations so that process does	}
		  { not eat up system...				}
		  { NOTE: Remove comments for VMS version 4.0 or greater}
		  {       INKEY_DELAY takes care of hibernation for	}
		  {       VMS 3.7 or less				}
		if ((rest mod 20) = 1) then sleep(1);
		rest := rest - 1;
		  { Test for any key being hit to abort rest.  Also,	}
		  { this will do a PUT_QIO which updates the screen...	}
		  { One more side benifit; since inkey_delay hibernates	}
		  { small amount before executing, this makes resting	}
		  { less CPU intensive...				}
	        inkey_delay(command,0);
		if (rest = 0) then		{ Resting over		}
		  rest_off
		else if (command <> null) then	{ Resting aborted	}
		  rest_off;
	      end;
	{ Halucinating?  (Random characters appear!)}
	    if (image > 0) then
	      begin
	        image := image - 1;
		if (image = 0) then
		  draw_cave;
	      end;
	{ Paralysis		}
	    if (paralysis > 0) then
	      begin
	        paralysis := paralysis - 1;
		if (rest > 0) then
		  rest_off;
		if (search_flag) then
		  search_off;
	      end;
	{ Protection from evil counter}
	    if (protevil > 0) then protevil := protevil - 1;
	{ Inulnerability	}
	    if (invuln > 0) then
	      begin
		if (uand(%X'00001000',status) = 0) then
		  begin
		    status := uor(%X'00001000',status);
		    if (find_flag) then
		      begin
			find_flag := false;
			move_char(5);
		      end;
		    msg_print('Your skin turns into steel!');
		    py.misc.pac := py.misc.pac + 100;
		    py.misc.dis_ac := py.misc.dis_ac + 100;
		    prt_pac;
		  end;
		invuln := invuln - 1;
		if (invuln = 0) then
		  begin
		    status := uand(%X'FFFFEFFF',status);
		    if (find_flag) then
		      begin
			find_flag := false;
			move_char(5);
		      end;
		    msg_print('Your skin returns to normal...');
		    py.misc.pac := py.misc.pac - 100;
		    py.misc.dis_ac := py.misc.dis_ac - 100;
		    prt_pac;
		  end;
	      end;
	{ Heroism	}
	    if (hero > 0) then
	      begin
		if (uand(%X'00002000',status) = 0) then
		  begin
		    status := uor(%X'00002000',status);
		    if (find_flag) then
		      begin
			find_flag := false;
			move_char(5);
		      end;
		    with py.misc do
		      begin
			mhp := mhp + 10;
			chp := chp + 10;
			bth := bth + 12;
			bthb:= bthb+ 12;
		      end;
		    msg_print('You feel like a HERO!');
		    prt_mhp;
		  end;
		hero := hero - 1;
		if (hero = 0) then
		  begin
		    status := uand(%X'FFFFDFFF',status);
		    if (find_flag) then
		      begin
			find_flag := false;
			move_char(5);
		      end;
		    with py.misc do
		      begin
			mhp := mhp - 10;
			if (chp > mhp) then chp := mhp;
			bth := bth - 12;
			bthb:= bthb- 12;
		      end;
		    msg_print('The heroism wears off.');
		    prt_mhp;
		  end;
	      end;
	{ Super Heroism	}
	    if (shero > 0) then
	      begin
		if (uand(%X'00004000',status) = 0) then
		  begin
		    status := uor(%X'00004000',status);
		    if (find_flag) then
		      begin
			find_flag := false;
			move_char(5);
		      end;
		    with py.misc do
		      begin
			mhp := mhp + 20;
			chp := chp + 20;
			bth := bth + 24;
			bthb:= bthb+ 24;
		      end;
		    msg_print('You feel like a SUPER HERO!');
		    prt_mhp;
		  end;
		shero := shero - 1;
		if (shero = 0) then
		  begin
		    status := uand(%X'FFFFBFFF',status);
		    if (find_flag) then
		      begin
			find_flag := false;
			move_char(5);
		      end;
		    with py.misc do
		      begin
			mhp := mhp - 20;
			if (chp > mhp) then chp := mhp;
			bth := bth - 24;
			bthb:= bthb- 24;
		      end;
		    msg_print('The super heroism wears off.');
		    prt_mhp;
		  end;
	      end;
	{ Blessed	}
	    if (blessed > 0) then
	      begin
		if (uand(%X'00008000',status) = 0) then
		  begin
		    status := uor(%X'00008000',status);
		    if (find_flag) then
		      begin
			find_flag := false;
			move_char(5);
		      end;
		    with py.misc do
		      begin
			bth := bth + 5;
			bthb:= bthb+ 5;
			pac := pac + 2;
			dis_ac := dis_ac + 2;
		      end;
		    msg_print('You feel righteous!');
		    prt_mhp;
		    prt_pac;
		  end;
		blessed := blessed - 1;
		if (blessed = 0) then
		  begin
		    status := uand(%X'FFFF7FFF',status);
		    if (find_flag) then
		      begin
			find_flag := false;
			move_char(5);
		      end;
		    with py.misc do
		      begin
			bth := bth - 5;
			bthb:= bthb- 5;
			pac := pac - 2;
			dis_ac := dis_ac - 2;
		      end;
		    msg_print('The prayer has expired.');
		    prt_mhp;
		    prt_pac;
		  end;
	      end;
	{ Resist Heat	}
	    if (resist_heat > 0) then resist_heat := resist_heat - 1;
	{ Resist Cold	}
	    if (resist_cold > 0) then resist_cold := resist_cold - 1;
	{ Detect Invisible	}
	    if (detect_inv > 0) then
	      begin
		if (uand(%X'00010000',status) = 0) then
		  begin
		    status := uor(%X'00010000',status);
		    see_inv := true;
		  end;
		detect_inv := detect_inv - 1;
		if (detect_inv = 0) then
		  begin
		    status := uand(%X'FFFEFFFF',status);
		    see_inv := false;
		    py_bonuses(blank_treasure,0);
		  end;
	      end;
	{ Timed infra-vision	}
	    if (tim_infra > 0) then
	      begin
		if (uand(%X'00020000',status) = 0) then
		  begin
		    status := uor(%X'00020000',status);
		    see_infra := see_infra + 1;
		  end;
		tim_infra := tim_infra - 1;
		if (tim_infra = 0) then
		  begin
		    status := uand(%X'FFFDFFFF',status);
		    see_infra := see_infra - 1;
		  end;
	      end;
	{ Word-of-Recall  Note: Word-of-Recall is a delayed action	}
	    if (word_recall > 0) then
	      if (word_recall = 1) then
		begin
	  	  if (dun_level > 0) then
		    begin
		      msg_print('You feel yourself yanked upwards!');
		      dun_level := 0;
		    end
		  else if (py.misc.max_lev > 0) then
		    begin
		      msg_print('You feel yourself yanked downwards!');
		      dun_level := py.misc.max_lev;
		    end;
		  moria_flag := true;
		  paralysis := paralysis + 1;
		  word_recall := 0;
		end
	      else
		word_recall := word_recall - 1;

	{ Check hit points for adjusting...			}
	      with py.misc do
	        if (not(find_flag)) then
	          if (py.flags.rest < 1) then
	            begin
	              if (old_chp <> trunc(chp)) then
	                begin
		          if (chp > mhp) then chp := mhp;
	                  prt_chp;
	                  old_chp := trunc(chp);
	                end;
		      if (old_cmana <> trunc(cmana)) then
		        begin
		          if (cmana > mana) then cmana := mana;
		          prt_cmana;
		          old_cmana := trunc(cmana);
		        end
	            end;
	  end;
	if ((py.flags.paralysis < 1) and 	{ Accept a command?	}
	    (py.flags.rest < 1) and
	    (not(death))) then
	{ Accept a command and execute it				}
	  repeat
	    print_stat := 0;
	    reset_flag := false;
		{ Random teleportation	}
	    if (py.flags.teleport) then
	      if (randint(100) = 1) then
	        begin
		  find_flag := false;
		  teleport(40);
	        end;
	    if (not (find_flag)) then
	      begin
		print('',char_row,char_col);
		save_msg_flag := msg_flag;
		inkey(command);
		if (save_msg_flag) then erase_line(msg_line,msg_line);
		com_val := ord(command);
	      end;
	{ Commands are executed in following case statement		}
	  case com_val of

		0,3,25 :begin					{^Y = exit    }
			  if (get_com('Quit? (Y/N)',command)) then
			    case command of
			      'y','Y':  begin
					  if (total_winner) then
					    begin
					      moria_flag := true;
					      death      := true;
					    end
			  		  else
					    begin
					      clear(1,1);
					      exit;
					    end;
					end;
			      otherwise ;
			    end;
			  reset_flag := true;
			end;
		13 :	begin					{^M = repeat  }
			  msg_print(old_msg);
			  reset_flag := true;
			end;
		16 :    if (wizard1) then			{^P = password}
			  begin
			    msg_print('Wizard mode off.');
			    wizard1 := false;
			    wizard2 := false;
			    reset_flag := true;
			    no_controly;
			  end
			else
			  begin
			    if (check_pswd) then
			      begin
			        msg_print('Wizard mode on.');
				controly;
			      end;
			    reset_flag := true;
			  end;
		18 :	begin					{^R = redraw  }
			  draw_cave;
			  reset_flag := true;
			end;
		26 :	begin					{^Z = save    }
			  if (total_winner) then
			    begin
	msg_print('You are a Total Winner, your character must be retired...');
	msg_print('Use <Control>-Y to when you are ready to quit.');
			    end
			  else
			    begin
			      if (search_flag) then search_off;
			      save_char;
			    end;
			end;
		36 :	begin					{$  = Shell   }
			  if (wizard1) then
                          begin
                          clear(1,1);
	writeln('[Entering DCL shell, type "EOJ" to resume your game]');
			  writeln;
			  controly;
			  shell_out;
			  no_controly;
			  clear(1,1);
			  draw_cave;
			  reset_flag := true;
                          end;
			end;
		46 :	begin					{. = find     }
			  y := char_row;
			  x := char_col;
			  if (get_dir('Which direction?',dir_val,
							com_val,y,x)) then
			    begin
			      find_flag := true;
			      move_char(dir_val);
			    end
			end;
		47 :	begin					{/ = identify }
			  ident_char;
			  reset_flag := true;
			end;
		49 :	move_char(com_val - 48);	{ Move dir 1	}
		50 :	move_char(com_val - 48);	{ Move dir 2	}
		51 :	move_char(com_val - 48);	{ Move dir 3	}
		52 :	move_char(com_val - 48);	{ Move dir 4	}
		53 :	begin				{ Rest one turn	}
			  move_char(com_val - 48);
			  sleep(0);	{ Sleep 1/10 a second}
			  flush;
			end;
		54 :	move_char(com_val - 48);	{ Move dir 6	}
		55 :	move_char(com_val - 48);	{ Move dir 7	}
		56 :	move_char(com_val - 48);	{ Move dir 8	}
		57 :	move_char(com_val - 48);	{ Move dir 9	}
		60 :	go_up;					{< = go up    }
		62 :	go_down;				{> = go down  }
		63 :	begin					{? = help     }
			  help;
			  reset_flag := true;	{ Free move	}
			end;
	{	65 :	begin	{ Debug feature for weight	
			  tmp1 := 0;
			  for i1 := 1 to inven_max-1 do
			    with inventory[i1] do
			      tmp1 := tmp1 + weight*number;
			  writev(out_val,inven_weight:1,' | ',tmp1:1);
			  msg_print(out_val);
			end;					}
		66 :	bash;					{B = bash     }
		67 :	begin					{C = character}
			  if (get_com('Print to file? (Y/N)',command)) then
			    case command of
			      'y','Y':  file_character;
			      'n','N':  begin
					  change_name;
					  draw_cave;
				        end;
			      otherwise ;
			    end;
			  reset_flag := true;	{ Free move	}
			end;
		68 :	disarm_trap;				{D = disarm   }
		69 :    eat;					{E = eat      }
		70 :	refill_lamp;				{F = refill   }
		76 :	begin					{L = location }
			  reset_flag := true;	{ Free move	}
			  if ((py.flags.blind > 0) or (no_light)) then
			    msg_print('You can''t see your map.')
			  else
			    begin
			      writev(out_val,
			      'Section [',
			      (trunc((char_row-1)/outpage_height)+1):1,',',
			      (trunc((char_col-1)/outpage_width )+1):1,
			      ']; Location = [',char_row:1,',',char_col:1,']');
			      msg_print(out_val);
			    end;
			end;
		80:	begin					{P = print map}
			  reset_flag := true;	{ Free move	}
			  if ((py.flags.blind > 0) or (no_light)) then
			    msg_print('You can''t see to draw a map.')
			  else
			    print_map;
			end;
		82 :	rest;					{R = rest     }
		83 :	if (search_flag) then			{S = srh mode }
			  begin
			    search_off;
			    reset_flag := true;	{ Free move	}
			  end
			else if (py.flags.blind > 0) then
		  msg_print('You are incapable of searching while blind.')
			else
			  begin
			    search_on;
			    reset_flag := true;	{ Free move	}
			  end;
		84 :	tunnel;					{T = tunnel   }
		97 :	aim;					{a = aim      }
		98 :	examine_book;				{b = browse   }
		99 :	closeobject;				{c = close    }
		100:	drop;					{d = drop     }
		101:	begin					{e = equipment}
			  reset_flag := true;	{ Free move	}
			  if (inven_command('e',0,0)) then draw_cave;
			end;
		102:	throw_object;				{f = throw    }
		104:	begin					{h = moria hlp}
			  moria_help('');
			  draw_cave;
			  reset_flag := true;	{ Free move	}
			end;
		105:	begin					{i = inventory}
			  reset_flag := true;	{ Free move	}
			  if (inven_command('i',0,0)) then draw_cave;
			end;
		106:	jamdoor;				{j = jam      }
		108:	begin					{l = look     }
			  look;
			  reset_flag := true;	{ Free move	}
			end;
		109:	cast;					{m = magick   }
		111:	openobject;				{o = open     }
		112:	pray;					{p = pray     }
		113:	quaff;					{q = quaff    }
		114:	read;					{r = read     }
		115:	if (py.flags.blind > 0) then		{s = search   }
		  msg_print('You are incapable of searching while blind.')
			else
			  search(char_row,char_col,py.misc.srh);
		116:	begin					{t = unwear   }
			  reset_flag := true;
			  if (inven_command('t',0,0)) then draw_cave;
			end;
		117:	use;					{u = use staff}
		118:	game_version;				{v = version  }
		119:	begin					{w = wear     }
			  reset_flag := true;
			  if (inven_command('w',0,0)) then draw_cave;
			end;
		120:	begin					{x = exchange }
			  reset_flag := true;
			  if (inven_command('x',0,0)) then draw_cave;
			end;
   		otherwise  if (wizard1) then
		  begin
		    reset_flag := true;	{ Wizard commands are free moves}
	      case com_val of
		1  :	begin 					{^A = Cure all}
			  remove_curse;
			  cure_blindness;
			  cure_confusion;
			  cure_poison;
			  remove_fear;
			  with py.stat do
			    begin
			      cstr := str;
			      cint := int;
			      cwis := wis;
			      cdex := dex;
			      ccon := con;
			      cchr := chr;
			    end;
			  with py.flags do
			    begin
			      if (slow > 1) then
				slow := 1;
			      if (image > 1) then
				image := 1;
			    end;
			end;
		2  :	print_objects;				{^B = objects }
		4  :	begin					{^D = up/down }
			  prt('Go to which level (0 -1200) ? ',1,1);
			  get_string(tmp_str,1,31,10);
			  i1 := -1;
			  readv(tmp_str,i1,error:=continue);
			  if (i1 > -1) then
			    begin
			      dun_level := i1;
			      if (dun_level > 1200) then
				dun_level := 1200;
	    		      moria_flag := true;
			    end
			  else
			    erase_line(msg_line,msg_line);
	  		end;
		8  :    wizard_help;				{^H = wizhelp }
		9  :	ident_spell;				{^I = identify}
		14 :	print_monsters;				{^N = mon map }
		12 :	wizard_light;				{^L = wizlight}
		20 :	teleport(100);				{^T = teleport}
		22 :	restore_char;				{^V = restore }
		otherwise if (wizard2) then
	      case com_val of
		5  :	change_character;			{^E = wizchar }
		6  :	mass_genocide;				{^F = genocide}
		7  :	alloc_object(floor_set,5,10);		{^G = treasure}
		10 :	begin					{^J = gain exp}
			  py.misc.exp := 2*py.misc.exp;
			  prt_experience;
			end;
		11 :	begin					{^K = summon  }
			  y := char_row;
			  x := char_col;
			  summon_monster(y,x,true);
			  creatures(false);
			end;
		23 :	wizard_create;				{^W = create  }
		otherwise  prt('Type ''?'' for help...',1,1);
	      end
		else
	          prt('Type ''?'' for help...',1,1);
	      end;
		  end
			  else
				begin
				  prt('Type ''?'' for help...',1,1);
				  reset_flag := true;
				end;



	  end;
	{ End of commands						}
	  until (not(reset_flag) or (moria_flag));
	{ Teleport?			}
	if (teleport_flag) then teleport(100);
	{ Move the creatures		}
	if (not moria_flag) then creatures(true);
	{ Exit when moria_flag is set	}
      until (moria_flag);
    end;
