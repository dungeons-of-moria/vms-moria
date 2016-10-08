	{ Handles the gravestone and top-twenty routines	-RAK-	}
[psect(death$code)] procedure upon_death;
    type
	word	= 0..65535;
	recj	= record
			unameinfo	: packed record
				unamelen	: word;
				jpi$_username	: word;
			end;
			ptr_uname	: ^usernam;
			ptr_unamelen	: ^integer;
			endlist		: integer
	end;
	usernam		= packed array [1..12] of char;


	{ function returns the players USERNAME			-JWT-	}
    function get_username : usernam;
      var
	user		: usernam;
	icode		: integer;
	jpirec		: recj;

	{ calls GETJPI routine to return the USERNAME		-JWT-	}
      function sys$getjpi	(%immed	p1	: integer;
			 	%immed	p2	: integer;
			 	%immed	p3	: integer;
			 	var	itmlst	: recj;
			 	%immed	p4	: integer;
			 	%immed	p5	: integer;
			 	%immed	p6	: integer) : integer;
        external;

      begin
	with jpirec do
	  begin
	    unameinfo.unamelen		:= 12;
	    unameinfo.jpi$_username	:= %x202;
	    new (ptr_uname);
	    ptr_uname^			:= '            ';
	    new (ptr_unamelen);
	    ptr_unamelen^		:= 0;
	    endlist			:= 0
	  end;
	icode := SYS$GETJPI (0,0,0,jpirec,0,0,0);
	if not odd(icode) then
	  begin
	    writeln('Error in GETJPI process');
	    halt
	  end
	else
	  get_username := jpirec.ptr_uname^
	end;



	{ Centers a string within a 31 character string		-JWT-	}
    function fill_str (p1 : vtype) : vtype;
      var
	s1	: vtype;
	i1	: integer;
      begin
        s1 := '';
        i1 := trunc(length(p1) / 2);
        fill_str := substr(pad(s1,' ',15-i1) + pad(p1,' ',31),1,31);
      end;


	{ Prints a line to the screen efficiently		-RAK-	}
    procedure dprint(str : vtype; row : integer);
      var
	i1,i2,nblanks,xpos			: integer;
	prt_str					: vtype;
      begin
	prt_str := '';
	nblanks := 0;
        xpos := 0;
	for i1 := 1 to length(str) do
	  begin
	    if (str[i1] = ' ') then
	      begin
		if (xpos > 0) then
		  begin
		    nblanks := nblanks + 1;
		    if (nblanks > 5) then
		      begin
			nblanks := 0;
			put_buffer(prt_str,row,xpos);
			prt_str := '';
			xpos := 0;
		      end
		  end;
	      end
	    else
	      begin
		if (xpos = 0) then xpos := i1;
		if (nblanks > 0) then
		  begin
		    for i2 := 1 to nblanks do
		      prt_str := prt_str + ' ';
		    nblanks := 0;
		  end;
		prt_str := prt_str + str[i1];
	      end;
	  end;
	if (xpos > 0) then
	  put_buffer(prt_str,row,xpos);
      end;


	{ Prints the gravestone of the character		-RAK-	}
    procedure print_tomb;
      var
	str1,str2,str3,str4,str5,str6,str7,str8	: vtype;
	dstr					: array [0..19] of vtype;
	fnam					: vtype;
	command					: char;
	f1					: text;
	i1					: integer;
	day					: packed array [1..11] of char;
	flag					: boolean;
      begin
        date(day);
	str1 := fill_str(py.misc.name);
	str2 := fill_str(py.misc.title);
	str3 := fill_str(py.misc.tclass);
        writev(str4,'Level : ',py.misc.lev:1);
	str4 := fill_str(str4);
        writev(str5,py.misc.exp:1,' Exp');
	str5 := fill_str(str5);
	writev(str6,py.misc.au:1,' Au');
	str6 := fill_str(str6);
	writev(str7,'Died on Level : ',dun_level:1);
	str7 := fill_str(str7);
	str8 := fill_str(died_from);
dstr[00] := ' ';
dstr[01] := '               _______________________';
dstr[02] := '              /                       \         ___';
dstr[03] := '             /                         \ ___   /   \      ___';
dstr[04] := '            /            RIP            \   \  :   :     /   \';
dstr[05] := '           /                             \  : _;,,,;_    :   :';
dstr[06] := '          /'+str1+                       '\,;_          _;,,,;_';
dstr[07] := '         |               the               |   ___';
dstr[08] := '         | '+str2+                       ' |  /   \';
dstr[09] := '         |                                 |  :   :';
dstr[10] := '         | '+str3+                       ' | _;,,,;_   ____';
dstr[11] := '         | '+str4+                       ' |          /    \';
dstr[12] := '         | '+str5+                       ' |          :    :';
dstr[13] := '         | '+str6+                       ' |          :    :';
dstr[14] := '         | '+str7+                       ' |         _;,,,,;_';
dstr[15] := '         |            killed by            |';
dstr[16] := '         | '+str8+                       ' |';
dstr[17] := '         |           '+day+    '           |';
dstr[18] := '        *|   *     *     *    *   *     *  | *';
dstr[19] := '________)/\\_)_/___(\/___(//_\)/_\//__\\(/_|_)_______';
	clear(1,1);
	for i1 := 0 to 19 do
	  dprint(dstr[i1],i1+1);
	flush;
	if (get_com('Print to file? (Y/N)',command)) then
	  case command of
	    'y','Y':  begin
			prt('Enter Filename:',1,1);
			flag := false;
			repeat
			  if (get_string(fnam,1,17,60)) then
			    begin
			      if (length(fnam) = 0) then fnam:='MORIACHR.DIE';
			      open (f1,file_name:=fnam,error:=continue);
			      if (status(f1) <> 0) then
			        prt('Error creating> ' + fnam,2,1)
			      else
			        begin
				  flag := true;
				  rewrite(f1,error:=continue);
				  for i1 := 0 to 19 do
				    writeln(f1,dstr[i1],error:=continue);
				end;
			      close(f1,error:=continue);
	  		    end
			  else
			    flag := true;
			until(flag);
		      end;
	    otherwise ;
	  end;
      end;


	{ Calculates the total number of points earned		-JWT-	}
    function total_points : integer;
      begin
	with py.misc do
	  total_points := max_exp + (100*py.misc.max_lev);
      end;


	{ Enters a players name on the top twenty list		-JWT-	}
    procedure top_twenty;
      var
	list		: array [1..20] of vtype;
	blank		: packed array [1..13] of char;
	i1,i2,i3,i4	: integer;
	n1		: integer;
	trys		: integer;
	o1,o2		: vtype;
	f1		: text;
	flag,file_flag	: boolean;
      begin
	if (wizard1) then exit;
	clear(1,1);
	for i1 := 1 to 20 do
	  list[i1] := '';
	n1 := 1;
	priv_switch(1);
	trys := 0;
	file_flag := false;
	repeat
	  open (f1,file_name:=moria_top,
		organization:=sequential,history:=old,
		sharing:=none,error:=continue);
	  if (status(f1) = 2) then
	    begin
	      trys := trys + 1;
	      if (trys > 5) then
		file_flag := true
	      else
		sleep(2);
	    end
	  else
	    file_flag := true;
	until(file_flag);
	if ((status(f1) <> 0) and (status(f1) <> 2)) then 
	  open (f1,file_name:=moria_top,
		organization:=sequential,history:=new,
		sharing:=none,error:=continue);
	if (status(f1) <> 0) then
	  begin
	    writeln('Error in opening ',moria_top);
	    writeln('Please contact local Moria Wizard.');
	    exit;
	  end;
	reset(f1);
	while ((not eof(f1)) and (n1 <= 20)) do
	  begin
	    readln(f1,list[n1],error:=continue);
	    n1 := n1 + 1;
	  end;
	n1 := n1 - 1;
	i1 := 1;
	i3 := total_points;
	flag := false;
	while ((i1 <= n1) and (not flag)) do
	  begin
	    readv(list[i1],blank,i4);
	    if (i4 < i3) then
	      flag := true
	    else
	      i1 := i1 + 1;
	  end;
	if ((flag) or (n1 = 0) or (n1 < 20)) then
	  begin
	    for i2 := 19 downto i1 do
	      list[i2+1] := list[i2];
	    o1 := get_username;
	    case py.misc.lev of
	      1		: writev(o2,py.misc.lev:1,'st level ');
	      2		: writev(o2,py.misc.lev:1,'nd level ');
	      3		: writev(o2,py.misc.lev:1,'rd level ');
	      otherwise	  writev(o2,py.misc.lev:1,'th level ');
	    end;
	    writev(list[i1],pad(o1,' ',13),i3:7,'  ',
		py.misc.name,', a ',o2,py.misc.race,' ',
		py.misc.tclass,'.');
	    if (n1 < 20) then
	      n1 := n1 + 1;
	    flag := false;
	  end;
	rewrite(f1);
	for i1 := 1 to n1 do
	  writeln(f1,list[i1]);
	close(f1);
	priv_switch(0);
	put_buffer('Username       Points  Character that died.',1,1);
	for i1 := 1 to n1 do
	  put_buffer(list[i1],i1+1,1);
	put_buffer('',i1+2,1);
	put_qio;
      end;


	{ Change the player into a King!			-RAK-	}
    procedure kingly;
      begin
	{ Change the character attributes...		}
	dun_level := 0;
	died_from := 'Ripe Old Age';
	with py.misc do
	  begin
	    lev := lev + max_player_level;
	    if (sex[1] = 'M') then
	      begin
		title  := 'Magnificent';
	        tclass := '*King*';
	      end
	    else
	      begin
		title  := 'Beautiful';
	        tclass := '*Queen*';
	      end;
	    au := au + 250000;
	    max_exp := max_exp + 5000000;
	    exp := max_exp;
	  end;
	{ Let the player know that he did good...	}
	clear(1,1);
	dprint('                                  #',2);
	dprint('                                #####',3);
	dprint('                                  #',4);
	dprint('                            ,,,  $$$  ,,,',5);
	dprint('                        ,,=$   "$$$$$"   $=,,',6);
	dprint('                      ,$$        $$$        $$,',7);
	dprint('                      *>         <*>         <*',8);
	dprint('                      $$         $$$         $$',9);
	dprint('                      "$$        $$$        $$"',10);
	dprint('                       "$$       $$$       $$"',11);
	dprint('                        *#########*#########*',12);
	dprint('                        *#########*#########*',13);
	dprint('                          Veni, Vidi, Vici!',16);
	dprint('                     I came, I saw, I conquered!',17);
	dprint('                      All Hail the Mighty King!',18);
	flush;
	pause(24);
      end;


	{ What happens upon dying...				-RAK-	}
    begin
      if (total_winner) then kingly;
      print_tomb;
      top_twenty;
      exit;
    end;
