	{ Print Moria credits					-RAK-	}
[psect(misc1$code)]    procedure game_version;
      var
	tmp_str			: vtype;
      begin
	clear(1,1);
	writev(tmp_str,'               Moria Version M',cur_version:3:2);
put_buffer(tmp_str,1,1);
put_buffer('Version 0.1  : 03/25/83',2,1);
put_buffer('Version 1.0  : 05/01/84',3,1);
put_buffer('Version 2.0  : 07/10/84',4,1);
put_buffer('Version 3.0  : 11/20/84',5,1);
put_buffer('Version 4.0  : 01/20/85',6,1);
put_buffer('Modules :',8,1);
put_buffer('     V1.0  Dungeon Generator      - RAK',9,1);
put_buffer('           Character Generator    - RAK & JWT',10,1);
put_buffer('           Moria Module           - RAK',11,1);
put_buffer('           Miscellaneous          - RAK & JWT',12,1);
put_buffer('     V2.0  Town Level & Misc      - RAK',13,1);
put_buffer('     V3.0  Internal Help & Misc   - RAK',14,1);
put_buffer('     V4.0  Source Release Version - RAK',15,1);
put_buffer('     M4.4  DEC Internal fixes     - RHM',16,1);
put_buffer('Robert Alan Koeneke               Jimmey Wayne Todd Jr.',18,1);
put_buffer('Student/University of Oklahoma    Student/University of Oklahoma',19,1);
put_buffer('119 Crystal Bend                  1912 Tiffany Dr.',20,1);
put_buffer('Norman, OK 73069                  Norman, OK  73071',21,1);
put_buffer('(405)-321-2925                    (405) 360-6792',22,1);
	pause(24);
	draw_cave;
      end;


	{ Light up the dungeon					-RAK-	}
[psect(wizard$code)]  procedure wizard_light;
    var
	i1,i2,i3,i4	: integer;
	flag		: boolean;
    begin
      if (cave[char_row,char_col].pl) then
	flag := false
      else
	flag := true;
      for i1 := 1 to cur_height do
	for i2 := 1 to cur_width do
	  if (cave[i1,i2].fval in floor_set) then
	    for i3 := i1-1 to i1+1 do
	      for i4 := i2-1 to i2+1 do
		with cave[i3,i4] do
		  begin
		    pl := flag;
		    if (not(flag)) then
		      fm := false;
		  end;
      prt_map;
    end;



	{ Wizard routine for gaining on stats			-RAK-	}
[psect(wizard$code)]  procedure change_character;
    var
	tmp_val			: integer;
	tmp_str			: vtype;
    begin
      with py.stat do
	begin
	  prt('(3 - 118) Strength     = ',1,1);
	  get_string(tmp_str,1,26,10);
	  tmp_val := -999;
	  readv(tmp_str,tmp_val,error:=continue);
	  if ((tmp_val > 2) and (tmp_val < 119)) then
	    begin
	      str  := tmp_val;
	      cstr := tmp_val;
	      prt_strength;
	    end;
	  prt('(3 - 118) Intelligence = ',1,1);
	  get_string(tmp_str,1,26,10);
	  tmp_val := -999;
	  readv(tmp_str,tmp_val,error:=continue);
	  if ((tmp_val > 2) and (tmp_val < 119)) then
	    begin
	      int  := tmp_val;
	      cint := tmp_val;
	      prt_intelligence;
	    end;
	  prt('(3 - 118) Wisdom       = ',1,1);
	  get_string(tmp_str,1,26,10);
	  tmp_val := -999;
	  readv(tmp_str,tmp_val,error:=continue);
	  if ((tmp_val > 2) and (tmp_val < 119)) then
	    begin
	      wis  := tmp_val;
	      cwis := tmp_val;
	      prt_wisdom;
	    end;
	  prt('(3 - 118) Dexterity    = ',1,1);
	  get_string(tmp_str,1,26,10);
	  tmp_val := -999;
	  readv(tmp_str,tmp_val,error:=continue);
	  if ((tmp_val > 2) and (tmp_val < 119)) then
	    begin
	      dex  := tmp_val;
	      cdex := tmp_val;
	      prt_dexterity;
	    end;
	  prt('(3 - 118) Constitution = ',1,1);
	  get_string(tmp_str,1,26,10);
	  tmp_val := -999;
	  readv(tmp_str,tmp_val,error:=continue);
	  if ((tmp_val > 2) and (tmp_val < 119)) then
	    begin
	      con  := tmp_val;
	      ccon := tmp_val;
	      prt_constitution;
	    end;
	  prt('(3 - 118) Charisma     = ',1,1);
	  get_string(tmp_str,1,26,10);
	  tmp_val := -999;
	  readv(tmp_str,tmp_val,error:=continue);
	  if ((tmp_val > 2) and (tmp_val < 119)) then
	    begin
	      chr  := tmp_val;
	      cchr := tmp_val;
	      prt_charisma;
	    end;
	end;
      with py.misc do
	begin
	  prt('(1 - 32767) Hit points = ',1,1);
	  get_string(tmp_str,1,26,10);
	  tmp_val := -1;
	  readv(tmp_str,tmp_val,error:=continue);
	  if ((tmp_val > 0) and (tmp_val < 32768)) then
	    begin
	      mhp  := tmp_val;
	      chp  := tmp_val;
	      prt_mhp;
	      prt_chp;
	    end;
	  prt('(0 - 32767) Mana       = ',1,1);
	  get_string(tmp_str,1,26,10);
	  tmp_val := -999;
	  readv(tmp_str,tmp_val,error:=continue);
	  if ((tmp_val > -1) and (tmp_val < 32768)) then
	    begin
	      mana  := tmp_val;
	      cmana := tmp_val;
	      prt_cmana;
	    end;
	  writev(tmp_str,'Current=',srh:1,'  (0-200) Searching = ');
	  tmp_val := length(tmp_str);
	  prt(tmp_str,1,1);
	  get_string(tmp_str,1,tmp_val+1,10);
	  tmp_val := -999;
	  readv(tmp_str,tmp_val,error:=continue);
	  if ((tmp_val > -1) and (tmp_val < 201)) then
	    srh  := tmp_val;
	  writev(tmp_str,'Current=',stl:1,'  (0-10) Stealth = ');
	  tmp_val := length(tmp_str);
	  prt(tmp_str,1,1);
	  get_string(tmp_str,1,tmp_val+1,10);
	  tmp_val := -999;
	  readv(tmp_str,tmp_val,error:=continue);
	  if ((tmp_val > -1) and (tmp_val < 11)) then
	    stl  := tmp_val;
	  writev(tmp_str,'Current=',disarm:1,'  (0-200) Disarming = ');
	  tmp_val := length(tmp_str);
	  prt(tmp_str,1,1);
	  get_string(tmp_str,1,tmp_val+1,10);
	  tmp_val := -999;
	  readv(tmp_str,tmp_val,error:=continue);
	  if ((tmp_val > -1) and (tmp_val < 201)) then
	    disarm := tmp_val;
	  writev(tmp_str,'Current=',save:1,'  (0-100) Save = ');
	  tmp_val := length(tmp_str);
	  prt(tmp_str,1,1);
	  get_string(tmp_str,1,tmp_val+1,10);
	  tmp_val := -999;
	  readv(tmp_str,tmp_val,error:=continue);
	  if ((tmp_val > -1) and (tmp_val < 201)) then
	    save := tmp_val;
	  writev(tmp_str,'Current=',bth:1,'  (0-200) Base to hit = ');
	  tmp_val := length(tmp_str);
	  prt(tmp_str,1,1);
	  get_string(tmp_str,1,tmp_val+1,10);
	  tmp_val := -999;
	  readv(tmp_str,tmp_val,error:=continue);
	  if ((tmp_val > -1) and (tmp_val < 201)) then
	    bth  := tmp_val;
	  writev(tmp_str,'Current=',bthb:1,'  (0-200) Bows/Throwing = ');
	  tmp_val := length(tmp_str);
	  prt(tmp_str,1,1);
	  get_string(tmp_str,1,tmp_val+1,10);
	  tmp_val := -999;
	  readv(tmp_str,tmp_val,error:=continue);
	  if ((tmp_val > -1) and (tmp_val < 201)) then
	    bthb := tmp_val;
	  writev(tmp_str,'Current=',au:1,'  Gold = ');
	  tmp_val := length(tmp_str);
	  prt(tmp_str,1,1);
	  get_string(tmp_str,1,tmp_val+1,10);
	  tmp_val := -999;
	  readv(tmp_str,tmp_val,error:=continue);
	  if (tmp_val > -1) then
	    begin
	      au := tmp_val;
	      prt_gold;
	    end;
	end;
      erase_line(msg_line,msg_line);
      py_bonuses(blank_treasure,0);
    end;

	{ Wizard routine for creating objects			-RAK-	}
[psect(wizard$code)]  procedure wizard_create;
    var
	tmp_val			: integer;
	tmp_str			: vtype;
	flag			: boolean;
    begin
      msg_print('Warning: This routine can cause fatal error.');
      msg_print(' ');
      msg_flag := false;
      with inventory[inven_max] do
	begin
          prt('Name   : ',1,1);
          if (get_string(tmp_str,1,10,40)) then
	    name := tmp_str
	  else
	    name := '& Wizard Object!';
	  repeat
	    prt('Tval   : ',1,1);
	    get_string(tmp_str,1,10,10);
	    tmp_val := 0;
	    readv(tmp_str,tmp_val,error:=continue);
	    flag := true;
	    case tmp_val of
	      1,13,15 :	tchar := '~';
	      2  :	tchar := '&';
	      10 :	tchar := '{';
	      11 :	tchar := '{';
	      12 :	tchar := '{';
	      20 :	tchar := '}';
	      21 :	tchar := '/';
	      22 :	tchar := '\';
	      23 :	tchar := '|';
	      25 :	tchar := '\';
	      30 :	tchar := ']';
	      31 :	tchar := ']';
	      32 :	tchar := '(';
	      33 :	tchar := ']';
	      34 :	tchar := ')';
	      35 :	tchar := '[';
	      36 :	tchar := '(';
	      40 :	tchar := '"';
	      45 :	tchar := '=';
	      55 :	tchar := '_';
	      60 :	tchar := '-';
	      65 :	tchar := '-';
	      70,71 :	tchar := '?';
	      75,76,77:	tchar := '!';
	      80 :	tchar := ',';
	      90 :	tchar := '?';
	      91 :	tchar := '?';
	      otherwise	flag := false;
	    end;
	  until (flag);
	  tval := tmp_val;
	  prt('Subval : ',1,1);
	  get_string(tmp_str,1,10,10);
	  tmp_val := 1;
	  readv(tmp_str,tmp_val,error:=continue);
	  subval := tmp_val;
	  prt('Weight : ',1,1);
	  get_string(tmp_str,1,10,10);
	  tmp_val := 1;
	  readv(tmp_str,tmp_val,error:=continue);
	  weight := tmp_val;
	  prt('Number : ',1,1);
	  get_string(tmp_str,1,10,10);
	  tmp_val := 1;
	  readv(tmp_str,tmp_val,error:=continue);
	  number := tmp_val;
	  prt('Damage : ',1,1);
	  get_string(tmp_str,1,10,5);
	  damage := tmp_str;
	  prt('+To hit: ',1,1);
	  get_string(tmp_str,1,10,10);
	  tmp_val := 0;
	  readv(tmp_str,tmp_val,error:=continue);
	  tohit := tmp_val;
	  prt('+To dam: ',1,1);
	  get_string(tmp_str,1,10,10);
	  tmp_val := 0;
	  readv(tmp_str,tmp_val,error:=continue);
	  todam := tmp_val;
	  prt('AC     : ',1,1);
	  get_string(tmp_str,1,10,10);
	  tmp_val := 0;
	  readv(tmp_str,tmp_val,error:=continue);
	  ac := tmp_val;
	  prt('+To AC : ',1,1);
	  get_string(tmp_str,1,10,10);
	  tmp_val := 0;
	  readv(tmp_str,tmp_val,error:=continue);
	  toac := tmp_val;
	  prt('P1     : ',1,1);
	  get_string(tmp_str,1,10,10);
	  tmp_val := 0;
	  readv(tmp_str,tmp_val,error:=continue);
	  p1 := tmp_val;
	  prt('Flags (In HEX): ',1,1);
	  flags := get_hex_value(1,17,8);
	  prt('Cost : ',1,1);
	  get_string(tmp_str,1,10,10);
	  tmp_val := 0;
	  readv(tmp_str,tmp_val,error:=continue);
	  cost := tmp_val;
	  if (get_com('Allocate? (Y/N)',command)) then
	    case command of
		'y','Y':  begin
			    popt(tmp_val);
			    t_list[tmp_val] := inventory[inven_max];
			    with cave[char_row,char_col] do
			      begin
				if (tptr > 0) then
				  delete_object(char_row,char_col);
				tptr := tmp_val;
			      end;
			    msg_print('Allocated...');
			  end;
		otherwise msg_print('Aborted...');
	    end;
	  inventory[inven_max] := blank_treasure;
	end;
    end;
