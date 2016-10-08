	{ Convert an integer into a system bin time		-RAK-	}
	{ NOTE: Int_time is number of 1/100 seconds			}
	{	Max value = 5999					}
[psect(misc2$code)] procedure convert_time(
		    int_time	: unsigned;
		var bin_time	: quad_type);
    type
	time_type = packed array [1..13] of char;
    var
	time_str	: time_type;
	secs,tics	: unsigned;
	out_val		: varying[2] of char;

    [asynchronous,external(SYS$BINTIM)] function $bin_time(
		%stdescr	give_str	: time_type;
		var		slp_time	: quad_type
						) : integer;
	external;

    begin
      time_str := '0 00:00:00.00';
      bin_time.l0 := 0;
      bin_time.l1 := 0;
      tics := int_time mod 100;
      secs := int_time div 100;
      if (secs > 0) then
	begin
	  if (secs > 59) then secs := 59;
	  writev(out_val,secs:2);
	  time_str[10] := out_val[2];
	  if (secs > 9) then time_str[9] := out_val[1];
	end;
      if (tics > 0) then
	begin
	  writev(out_val,tics:2);
	  time_str[13] := out_val[2];
	  if (tics > 9) then time_str[12] := out_val[1];
	end;
      $bin_time(time_str,bin_time);
    end;


	{ Set timer for hibernation				-RAK-	}
    [asynchronous,external(SYS$SETIMR)] function set_time(
	%immed efn	: integer := %immed 5;
	var bintime	: quad_type;
	%ref astadr	: integer := %immed 0;
	%immed reqidt	: integer := %immed 0) : integer;
	external;


	{ Hibernate 						-RAK-	}
    [asynchronous,external(SYS$WAITFR)] function hibernate(
	%immed efn	: integer := %immed 5) : integer;
	external;


	{ Sleep for given time					-RAK-	}
	{ NOTE: Int_time is in seconds					}
[psect(misc2$code)] procedure sleep(int_time : unsigned);
    var
	bin_time	: quad_type;
    begin
      convert_time(int_time*100,bin_time);
      set_time(bintime:=bin_time);
      hibernate;
    end;


	{ Setup system time format for io_pause.  		-RAK-	}
	{ NOTE: IO$MOR_IOPAUSE is a constant				}
	{	IO$BIN_PAUSE is a variable used to store results	}
	{ NOTE: Remove or comment out for VMS 4.0 or greater		}
[psect(setup$code)] procedure setup_io_pause;
    begin
      convert_time(IO$MOR_IOPAUSE,IO$BIN_PAUSE);
    end;


	{ Turns SYSPRV off if 0; on if 1;			-RAK-	}
	{ This is needed if image is installed with SYSPRV because	}
	{ user could write on system areas.  By turning the priv off	}
	{ system areas are secure					}
[psect(setup$code)] procedure priv_switch(switch_val : integer);
    type
	priv_field=	record	{ Quad word needed for priv mask}
			  low	: unsigned;
			  high	: unsigned;
			end;
    var
	priv_mask	: priv_field;

	{ Turn off SYSPRV					-RAK-	}
    [external(SYS$SETPRV)] function $setprv(
	%immed enbflg	: integer := %immed 0;
	var privs	: priv_field;
	%immed prmflg	: integer := %immed 0;
	%immed prvprv	: integer := %immed 0) : integer;
	external;

    begin
      priv_mask.low  := %X'10000000';	{ SYSPRV	}
      priv_mask.high := %X'00000000';
      $setprv(enbflg:=switch_val,privs:=priv_mask);
    end;


	{ Spawn a shell						-RAK-	}
    [external(LIB$SPAWN)] function shell_out(
	command_str	: integer := %immed 0;
	input_file 	: integer := %immed 0;
	output_file	: integer := %immed 0;
	flags		: integer := %immed 0;
	process_name	: integer := %immed 0;
	process_id	: integer := %immed 0;
	comp_status	: integer := %immed 0;
	comp_efn	: integer := %immed 0;
	comp_astadr	: integer := %immed 0;
	comp_astprm	: integer := %immed 0 ) : integer;
	external;


	{ Turn off Control-Y					-RAK-	}
[psect(setup$code)] procedure no_controly;
    var
	bit_mask	: unsigned;

    [external(LIB$DISABLE_CTRL)] function y_off(
	var mask	: unsigned;
	    old_mask	: integer := %immed 0) : integer;
	external;

    begin
      bit_mask := %X'02000000';	{ No Control-Y	}
      y_off(mask:=bit_mask);
    end;


	{ Turn on Control-Y					-RAK-	}
[psect(setup$code)] procedure controly;
    var
	bit_mask	: unsigned;

    [external(LIB$ENABLE_CTRL)] function y_on(
	var mask	: unsigned;
	    old_mask	: integer := %immed 0) : integer;
	external;

    begin
      bit_mask := %X'02000000';	{ Control-Y	}
      y_on(mask:=bit_mask);
    end;


	{ Dump IO to buffer					-RAK-	}
	{ NOTE: Source is PUTQIO.MAR					}
  procedure put_buffer		(
		%ref	out_str	:	varying [a] of char;
		%immed	row	:	integer;
		%immed	col	:	integer
				);
	external;


  	{ Dump the IO buffer to terminal			-RAK-	}
	{ NOTE: Source is PUTQIO.MAR					}
  procedure put_qio;
	external;


[psect(setup$code)] procedure exit;

	{ Immediate exit from program					}
  [external(SYS$EXIT)] function $exit(
	%immed status	: integer := %immed 1) : integer;
	external;

      begin
	controly;	{ Turn control-Y back on	}
	put_qio;	{ Dump any remaining buffer	}
	$exit;		{ exit from game		}
      end;


	{ Initializes I/O channel for use with INKEY			}
[psect(setup$code)] procedure init_channel;
    type
      ttype = packed array [1..3] of char;
    var
      status		: integer;
      terminal		: ttype;

  [external(SYS$ASSIGN)] function assign(
	%stdescr terminal	: ttype;
	var channel		: [volatile] integer;
	acmode			: integer := %immed 0;
	mbxnam	  		: integer := %immed 0) : integer; 
	external;

    begin
      terminal := 'TT:';
      status := assign(terminal,channel);
      if (not odd(status)) then
        begin
	  writeln('Channel could not be assigned <Status - ',status:4,'>');
	  exit;
        end
    end;


	{ QIOW definition					-RAK-	}
  [asynchronous,external(SYS$QIOW)] function qiow_read(
	%immed efn		: integer := %immed 1;
	%immed chan  		: integer;
	%immed func		: integer := %immed 0;
	%immed isob		: integer := %immed 0;
	%immed astadr		: integer := %immed 0;
	%immed astprm		: integer := %immed 0;
	%ref get_char		: [unsafe] char := %immed 0;
	%immed buff_len		: integer := %immed 0;
	%immed delay_time	: integer := %immed 0;
	%immed p4		: integer := %immed 0;
	%immed p5		: integer := %immed 0;
	%immed p6		: integer := %immed 0) : integer;
	external;

	{ Gets single character from keyboard and returns		}
[psect(io$code)] procedure inkey(var getchar : char);
    var
	status			: integer;
    begin
      put_qio;			{ Dump IO buffer		}
	{ Allow device driver to catch up			}
	{ NOTE: Remove or comment out for VMS 4.0 or greater	}
      set_time(bintime:=IO$BIN_PAUSE);
      hibernate;
	{ Now read				}
      qiow_read(chan:=channel,
		func:=IO$MOR_INPUT,
		get_char:=getchar,
		buff_len:=1	);
      msg_flag := false;
    end;


	{ Gets single character from keyboard and returns		}
[psect(io$code)] procedure inkey_delay	(
			var getchar	: char;
			delay		: integer
					);
    var
	status			: integer;
    begin
      put_qio;			{ Dump the IO buffer		}
	{ Allow device driver to catch up			}
	{ NOTE: Remove or comment out for VMS 4.0 or greater	}
      set_time(bintime:=IO$BIN_PAUSE);
      hibernate;
	{ Now read				}
      getchar := null;		{ Blank out return character	}
      qiow_read(chan:=channel,
		func:=IO$MOR_DELAY,
		get_char:=getchar,
		buff_len:=1,
		delay_time:=delay );
    end;


	{ Flush the buffer					-RAK-	}
[psect(io$code)] procedure flush;
    begin
	{ Allow device driver to catch up			}
	{ NOTE: Remove or comment out for VMS 4.0 or greater	}
      set_time(bintime:=IO$BIN_PAUSE);
      hibernate;
	{ Now flush				}
      qiow_read(chan:=channel,func:=IO$MOR_IPURGE);
    end;


	{ Flush buffer before input				-RAK-	}
[psect(io$code)] procedure inkey_flush(var x : char);
    begin
      put_qio;	{ Dup the IO buffer	}
      if (not(wizard1)) then flush;
      inkey(x);
    end;


	{ Retrieves foreign string input with game command	-JWT-	}
  [external(LIB$GET_FOREIGN)] procedure get_foreign(
	%descr msg_str		: vtype;
	%descr prompt		: vtype   := %immed 0;
	%ref len		: integer := %immed 0); 
	external;


	{ Clears given line of text				-RAK-	}
[psect(io$code)] procedure erase_line		(
		row		:	integer;
		col		:	integer
				);
    begin
      put_buffer(cursor_erl,row,col);
    end;


	{ Clears screen at given row, column				}
[psect(io$code)] procedure clear(row,col : integer);
    var
	i1			: integer;
    begin
      for i1 := 2 to 23 do used_line[i1] := false;
      put_buffer(cursor_erp,row,col);
      put_qio;	{ Dump the Clear Sequence	}
    end;


	{ Outputs a line to a given interpolated y,x position	-RAK-	}
[psect(io$code)] procedure print(
		str_buff	: varying[a] of char;
		row		: integer;
		col		: integer
				);
    begin
      row := row - panel_row_prt;{ Real co-ords convert to screen positions }
      col := col - panel_col_prt;
      used_line[row] := true;
      put_buffer(str_buff,row,col)
    end;


	{ Outputs a line to a given y,x position		-RAK-	}
[psect(io$code)] procedure prt(
		str_buff	: varying[a] of char;
		row		: integer;
		col		: integer
				);
    begin
      put_buffer(cursor_erl+str_buff,row,col);
    end;


	{ Outputs message to top line of screen				}
[psect(io$code)] procedure msg_print(str_buff	: varying[a] of char);
    var
	old_len		: integer;
	in_char		: char;
    begin
      if (msg_flag) then
	begin
	  old_len := length(old_msg) + 1;
	  put_buffer(' -more-',msg_line,old_len);
	  repeat
	    inkey(in_char);
	  until (ord(in_char) in [3,13,25,26,27,32]);
	end;
      put_buffer(cursor_erl+str_buff,msg_line,msg_line);
      old_msg := str_buff;
      msg_flag := true;
    end;


	{ Prompts (optional) and returns ord value of input char	}
	{ Function returns false if <ESCAPE>,CNTL/(Y,C,Z) is input	}
[psect(io$code)] function get_com	(
				prompt		: varying[a] of char;
				var command	: char
					) : boolean;
    var
	com_val		: integer;
    begin
      if (length(prompt) > 1) then prt(prompt,1,1);
      inkey(command);
      com_val := ord(command);
      case com_val of
	3,25,26,27	: get_com := false;
	otherwise	  get_com := true;
      end;
      erase_line(msg_line,msg_line);
      msg_flag := false;
    end;


	{ Gets a string terminated by <RETURN>				}
	{ Function returns false if <ESCAPE>,CNTL/(Y,C,Z) is input	}
[psect(io$code)] function get_string	(
			var in_str	: varying[a] of char;
			row,column,slen : integer
					) : boolean;
    var
	start_col,end_col,i1	: integer;
	x			: char;
	tmp			: vtype;
	flag,abort		: boolean;
	
    begin
      abort := false;
      flag  := false;
      in_str:= '';
      put_buffer(pad(in_str,' ',slen),row,column);
      put_buffer('',row,column);
      start_col := column;
      end_col := column + slen - 1;
      repeat
	inkey(x);
	case ord(x) of
	 3,25,26,27 :	abort := true;
		13  : 	flag  := true;
		127 : 	begin
			  if (column > start_col) then
			    begin
			      column := column - 1;
			      put_buffer(' '+chr(8),row,column);
			      in_str := substr(in_str,1,length(in_str)-1);
			    end;
			end;
	 otherwise	begin
			  tmp := x;
			  put_buffer(tmp,row,column);
			  in_str := in_str + tmp;
			  column := column + 1;
			  if (column > end_col) then
			    flag := true;
			end;
	end;
      until (flag or abort);
      if (abort) then
	get_string := false
      else
	begin			{ Remove trailing blanks	}
	  i1 := length(in_str);
	  if (i1 > 1) then
	    begin
	      while ((in_str[i1] = ' ') and (i1 > 1)) do
	        i1 := i1 - 1;
	      in_str := substr(in_str,1,i1);
	    end;
	  get_string := true;
	end;
    end;


	{ Return integer value of hex string			-RAK-	}
[psect(wizard$code)] function get_hex_value(row,col,slen : integer) : integer;
    type
	pack_type		= packed array [1..9] of char;
    var
	bin_val			: integer;
	tmp_str			: vtype;
	pack_str		: pack_type;

    [asynchronous,external(OTS$CVT_TZ_L)] function convert_hex_to_bin(
		%stdescr hex_str	: pack_type;
		%ref	 hex_val 	: integer;
		%immed	 val_size	: integer := %immed 4;
		%immed	 flags		: integer := %immed 1) : integer;
		external;

    begin
      get_hex_value := 0;
      if (get_string(tmp_str,row,col,slen)) then
	if (length(tmp_str) <= 8) then
	  begin
	    pack_str := pad(tmp_str,' ',9);
	    if (odd(convert_hex_to_bin(pack_str,bin_val))) then
	      get_hex_value := bin_val;
	  end;
    end;



	{ Pauses for user response before returning		-RAK-	}
[psect(misc2$code)] procedure pause(prt_line : integer);
    var
	dummy			: char;
    begin
      prt('[Press any key to continue]',prt_line,24);
      inkey(dummy);
      erase_line(24,1);
    end;


	{ Pauses for user response before returning		-RAK-	}
	{ NOTE: Delay is for players trying to roll up "perfect"	}
	{	characters.  Make them wait a bit...			}
[psect(misc2$code)] procedure pause_exit(
		prt_line	: integer;
		delay		: integer);
    var
	dummy			: char;
    begin
      prt('[Press any key to continue, or <Control>-Z to exit]',prt_line,11);
      inkey(dummy);
      case ord(dummy) of
	3,25,26 :	begin
			  erase_line(prt_line,1);
			  if (delay > 0) then sleep(delay);
			  exit;
			end;
	otherwise;
      end;
      erase_line(prt_line,1);
    end;


	{ Returns the image path for Moria			-RAK-	}
	{ Path is returned in a VARYING[80] of char			}
[psect(setup$code)] procedure get_paths;
    type
	word	= 0..65535;
	rec_jpi	= record
			pathinfo : packed record
				     pathlen		: word;
				     jpi$_imagname	: word;
		  		   end;
			ptr_path	: ^path;
			ptr_pathlen	: ^integer;
			endlist		: integer
		  end;
	path		= packed array [1..128] of char;
    var
	i1		: integer;
	tmp_str		: path;
	image_path	: vtype;
	flag		: boolean;

	{ Call JPI and return the image path as a packed 128	-RAK-	}
    function get_jpi_path : path;
      var
	status		: integer;
	user		: path;
	jpirec		: rec_jpi;

	{ GETJPI definition	}
      [asynchronous,external(SYS$GETJPI)] function $getjpi(
		%immed	p1	: integer := %immed 0;
		%immed	p2	: integer := %immed 0;
		%immed	p3	: integer := %immed 0;
		var	itmlst	: rec_jpi;
		%immed	p4	: integer := %immed 0;
		%immed	p5	: integer := %immed 0;
		%immed	p6	: integer := %immed 0) : integer;
		external;

      begin
	with jpirec do
	  begin
	    pathinfo.pathlen		:= 128;		{ Image length	}
	    pathinfo.jpi$_imagname	:= %x207;	{ Image path	}
	    new (ptr_path);
	    pad(ptr_path^,' ',128);
	    new (ptr_pathlen);
	    ptr_pathlen^		:= 0;
	    endlist			:= 0;
	  end;
	status := $getjpi(itmlst:=jpirec);
	if (not(odd(status))) then
	  begin
	    clear(1,1);
  	    put_buffer('Error in retrieving image path.',1,1);
	    exit;
	  end
	else
	  get_jpi_path := jpirec.ptr_path^;
      end;

    begin
      tmp_str := get_jpi_path;
      i1 := 0;
      flag := false;
      image_path := '';
      repeat
	i1 := i1 + 1;
	if (tmp_str[i1] = ']') then flag := true;
	image_path := image_path + tmp_str[i1];
	if (i1 > 127) then flag := true;
      until(flag);
      MORIA_HOU := image_path + 'HOURS.DAT';
      MORIA_MOR := image_path + 'MORIA.DAT';
      MORIA_MAS := image_path + 'MORIACHR.DAT';
      MORIA_TOP := image_path + 'MORIATOP.DAT';
      MORIA_HLP := image_path + 'MORIAHLP.HLB';
    end;
