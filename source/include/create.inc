[psect(create$code)] procedure create_character;


	{ Generates character's stats				-JWT-	}
  function get_stat : integer;
    var
	i,j : integer;
    begin
      i := randint(4) + randint(4) + randint(4) + 5;
      get_stat := i
    end;


	{ Changes stats by given amount				-JWT-	}
  function change_stat(cur_stat,amount : integer) : integer;
    var
	i : integer;
    begin
      if (amount < 0) then
	for i := -1 downto amount do
	  cur_stat := de_statp(cur_stat)
      else
	for i := 1 to amount do
	  cur_stat := in_statp(cur_stat);
      change_stat := cur_stat;
    end;


	{ Allows player to select a race			-JWT-	}
  function choose_race : boolean;
    var
	i2,i3,i4,i5		: integer;
	s			: char;
	exit_flag		: boolean;
    begin
      i2 := 1;
      i3 := 1;
      i4 := 3;
      i5 := 22;
      clear(21,1);
      prt('Choose a race (? for Help):',21,3);
      repeat
	put_buffer (chr(i3+96)+') '+race[i2].trace,i5,i4);
	i3 := i3 + 1;
	i4 := i4 + 15;
	if (i4 > 70) then
	  begin
	    i4 := 3;
	    i5 := i5 + 1
	  end;
	i2 := i2 + 1
      until (i2 > max_races);
      py.misc.race := '';
      put_buffer('',21,30);
      exit_flag := false;
      repeat
	inkey_flush(s);
	i2 := index('abcdefghijklmnopqrstuvwxyz',s);
	if ((i2 <= max_races) and (i2 >= 1)) then
	    with py do
	      with race[i2] do
		begin
		  misc.prace  := i2;
		  misc.race   := trace;
		  stat.str    := get_stat;
		  stat.int    := get_stat;
		  stat.wis    := get_stat;
		  stat.dex    := get_stat;
		  stat.con    := get_stat;
		  stat.chr    := get_stat;
		  stat.str    := change_stat(stat.str,str_adj);
		  stat.int    := change_stat(stat.int,int_adj);
		  stat.wis    := change_stat(stat.wis,wis_adj);
		  stat.dex    := change_stat(stat.dex,dex_adj);
		  stat.con    := change_stat(stat.con,con_adj);
		  stat.chr    := change_stat(stat.chr,chr_adj);
		  stat.cstr   := stat.str;
		  stat.cint   := stat.int;
		  stat.cwis   := stat.wis;
		  stat.cdex   := stat.dex;
		  stat.ccon   := stat.con;
		  stat.cchr   := stat.chr;
		  misc.srh    := srh;
		  misc.bth    := bth;
		  misc.bthb   := bthb;
		  misc.fos    := fos;
		  misc.stl    := stl;
		  misc.save   := bsav;
		  misc.hitdie := bhitdie;
		  misc.lev    := 1;
		  misc.ptodam := todam_adj;
		  misc.ptohit := tohit_adj;
		  misc.ptoac  := 0;
		  misc.pac    := toac_adj;
		  misc.expfact:= b_exp;
		  flags.see_infra := infra;
		  exit_flag := true;
		  choose_race := true;
		  put_buffer(py.misc.race,4,15);
	        end
	else if (s = '?') then
	  begin
	    moria_help('Character Races');
	    exit_flag := true;
	    choose_race := false;
	  end;
      until (exit_flag);
    end;


	{ Will print the history of a character			-JWT-	}
  procedure print_history;
    var
	i1		: integer;
    begin
      put_buffer('Character Background',14,28);
      for i1 := 1 to 5 do
	put_buffer(py.misc.history[i1],i1+14,5)
    end;


	{ Get the racial history, determines social class	-RAK-	}
	{ Assumtions:	Each race has init history beginning at 	}
	{		(race-1)*3+1					}
	{		All history parts are in accending order	}
  procedure get_history;
    var
	hist_ptr,cur_ptr,test_roll	: integer;
	start_pos,end_pos,cur_len	: integer;
	line_ctr,new_start,social_class	: integer;
	history_block			: varying [400] of char;
	flag				: boolean;
    begin
	{ Get a block of history text				}
      hist_ptr := (py.misc.prace-1)*3 + 1;
      history_block := '';
      social_class := randint(4);
      cur_ptr := 0;
      repeat
	flag := false;
	repeat
	  cur_ptr := cur_ptr + 1;
	  if (background[cur_ptr].chart = hist_ptr) then
	    begin
	      test_roll := randint(100);
	      while (test_roll > background[cur_ptr].roll) do
		cur_ptr := cur_ptr + 1;
	      with background[cur_ptr] do
		begin
		  history_block := history_block + info;
		  social_class := social_class + bonus;
		  if (hist_ptr > next) then cur_ptr := 0;
		    hist_ptr := next;
		end;
	      flag := true;
	    end;
	until(flag);
      until(hist_ptr < 1);
	{ Process block of history text for pretty output	}
      start_pos := 1;
      end_pos   := length(history_block);
      line_ctr  := 1;
      flag := false;
      while (history_block[end_pos] = ' ') do
	end_pos := end_pos - 1;
      repeat
	while (history_block[start_pos] = ' ') do
	  start_pos := start_pos + 1;
	cur_len := end_pos - start_pos + 1;
	if (cur_len > 70) then 
	  begin
	    cur_len := 70;
	    while (history_block[start_pos+cur_len-1] <> ' ') do
	      cur_len := cur_len - 1;
	    new_start := start_pos + cur_len;
	    while (history_block[start_pos+cur_len-1] = ' ') do
	      cur_len := cur_len - 1;
	  end
	else
	  flag := true;
	py.misc.history[line_ctr] := substr(history_block,start_pos,cur_len);
	line_ctr := line_ctr + 1;
	start_pos := new_start;
      until(flag);
	{ Compute social class for player			}
      if (social_class > 100) then 
	social_class := 100
      else if(social_class < 1) then
	social_class := 1;
      py.misc.sc := social_class;
    end;


	{ Gets the character's sex				-JWT-	}
  function get_sex : boolean;
    var
	s     			: char;
	exit_flag		: boolean;
    begin
      py.misc.sex := '';
      clear(21,1);
      prt('Choose a sex (? for Help):',21,3);
      prt('m) Male       f) Female',22,3);
      prt('',21,29);
      repeat
	inkey_flush(s);
	case s of
	  'f' : begin
		  py.misc.sex := 'Female';
		  prt(py.misc.sex,5,15);
		  exit_flag := true;
		  get_sex := true;
		end;
	  'm' : begin
		  py.misc.sex := 'Male';
		  prt(py.misc.sex,5,15);
		  exit_flag := true;
		  get_sex := true;
		end;
	  '?' : begin
		  moria_help('Character Sex');
		  exit_flag := true;
		  get_sex := false;
		end;
	  otherwise ;
	 end;
      until (exit_flag);
    end;


	{ Computes character's age, height, and weight		-JWT-	}
  procedure get_ahw;
    var
	i1 	: integer;
    begin
      i1 := py.misc.prace;
      py.misc.age := race[i1].b_age + randint(race[i1].m_age);
      case trunc((index(sex_type,py.misc.sex)+5)/6) of
      1 : begin
	    py.misc.ht := randnor(race[i1].f_b_ht,race[i1].f_m_ht);
	    py.misc.wt := randnor(race[i1].f_b_wt,race[i1].f_m_wt)
	  end;
      2 : begin
	    py.misc.ht := randnor(race[i1].m_b_ht,race[i1].m_m_ht);
	    py.misc.wt := randnor(race[i1].m_b_wt,race[i1].m_m_wt)
	  end
      end;
      py.misc.disarm := race[i1].b_dis + todis_adj;
    end;


	{ Gets a character class				-JWT-	}
  function get_class : boolean;
    var
	i1,i2,i3,i4,i5		: integer;
	cl			: array [0..max_class] of integer;
	s			: char;
	exit_flag		: boolean;
    begin
      for i2 := 1 to max_class do cl[i2] := 0;
      i1 := py.misc.prace;
      i2 := 1;
      i3 := 0;
      i4 := 3;
      i5 := 22;
      clear(21,1);
      prt('Choose a class (? for Help):',21,3);
      repeat
	if (uand(race[i1].tclass,bit_array[i2]) <> 0) then
	  begin
	    i3 := i3 + 1;
	    put_buffer (chr(i3+96)+') '+class[i2].title,i5,i4);
	    cl[i3] := i2;
	    i4 := i4 + 15;
	    if (i4 > 70) then
	      begin
		i4 := 3;
		i5 := i5 + 1
	      end;
	  end;
	i2 := i2 + 1;
      until (i2 > max_class);
      py.misc.pclass := 0;
      put_buffer('',21,31);
      exit_flag := false;
      repeat
	inkey_flush(s);
	i2 := index('abcdefghijklmnopqrstuvwxyz',s);
	if ((i2 <= i3) and (i2 >= 1)) then
	  begin
	    py.misc.tclass := class[cl[i2]].title;
	    py.misc.pclass := cl[i2];
	    exit_flag := true;
	    get_class := true;
	    clear(21,1);
	    put_buffer(py.misc.tclass,6,15);
	    with py.misc do
	      begin
	        hitdie := hitdie + class[pclass].adj_hd;
                mhp    := con_adj + hitdie;
	        chp	 := mhp;
                bth    := bth     + class[pclass].mbth;
	        bthb   := bthb    + class[pclass].mbthb;	{RAK}
                srh    := srh     + class[pclass].msrh;
                disarm := disarm  + class[pclass].mdis;
                fos    := fos     + class[pclass].mfos;
                stl    := stl     + class[pclass].mstl;
	        save   := save    + class[pclass].msav;
                title  := player_title[pclass,1];
	        expfact:= expfact + class[pclass].m_exp;
	      end;
	{ Adjust the stats for the class adjustment		-RAK-	}
            with py do
	      begin
	        stat.str  := change_stat(stat.str,class[misc.pclass].madj_str);
	        stat.int  := change_stat(stat.int,class[misc.pclass].madj_int);
	        stat.wis  := change_stat(stat.wis,class[misc.pclass].madj_wis);
	        stat.dex  := change_stat(stat.dex,class[misc.pclass].madj_dex);
	        stat.con  := change_stat(stat.con,class[misc.pclass].madj_con);
	        stat.chr  := change_stat(stat.chr,class[misc.pclass].madj_chr);
	        stat.cstr := stat.str;
	        stat.cint := stat.int;
	        stat.cwis := stat.wis;
	        stat.cdex := stat.dex;
	        stat.ccon := stat.con;
	        stat.cchr := stat.chr;
	        misc.ptodam := todam_adj;	{ Real values		}
	        misc.ptohit := tohit_adj;
	        misc.ptoac  := toac_adj;
	        misc.pac    := 0;
		misc.dis_td := misc.ptodam;	{ Displayed values	}
		misc.dis_th := misc.ptohit;
		misc.dis_tac:= misc.ptoac;
		misc.dis_ac := misc.pac;
	      end;
	  end
	else if (s = '?') then
	  begin
	    moria_help('Character Classes');
	    exit_flag := true;
	    get_class := false;
	  end;
      until(exit_flag);
    end;


  procedure get_money;
    var
	tmp			: integer;
    begin
      with py.stat do
	tmp := cstr + cint + cwis + cdex + ccon + cchr;
      with py.misc do
	begin
	  au := sc*6 + randint(25) + 325;	{ Social Class adj	}
	  au := au - tmp;			{ Stat adj		}
	  au := au + py.stat.cchr;		{ Charisma adj		}
	  if (au < 80) then au := 80;		{ Minimum		}
	end;
    end;


	{ ---------- M A I N  for Character Creation Routine ---------- }
	{							-JWT-	}

  begin
    with py do
      begin
	{ This delay may be reduced, but is recomended to keep players	}
	{ from continuously rolling up characters, which can be VERY	}
	{ expensive CPU wise.						}
	repeat
	  put_character;
	until(choose_race);
	while (not(get_sex)) do put_character;
	get_history;
	get_ahw;
	print_history;
	put_misc1;
	put_stats;
	while (not(get_class)) do
	  begin
	    put_character;
	    print_history;
	    put_misc1;
	    put_stats;
	  end;
	get_money;
	put_stats;
	put_misc2;
	put_misc3;
	get_name;
	pause_exit(24,player_exit_pause);
      end
  end;
