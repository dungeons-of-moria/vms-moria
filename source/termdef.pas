[inherit('moria.env')] module a;
{  TERMDEF : uses the values returned by SYS$GETDVI to set up the proper}
{	addressing codes.  New terminals can be added, or existing ones	}
{	changed wihtout re-compiling the main source.  You can use	}
{	compile.com by specifying:					}
{		$ cterm :== @DISK_NAME:[FILE_PATH]compile termdef	}

[global] procedure termdef;
  type
	term_type	=	packed array [1..3] of char;
	dvi_type	=	record
		item_len	: wordint;
		item_code	: wordint;
		buff_add	: ^integer;
		len_add		: ^integer;
		end_item	: integer;
	end;
  const
	tt$m_eightbit = 32768;
	tt2$m_ansicrt = 16777216;
	tt2$m_deccrt2 = 1073741824;
	dvi$_devtype = 6;
	dvi$_devdepend = 10;
	dvi$_devdepend2 = 28;
  var
	dvi_buff	: dvi_type;
	i1		: integer;
	tmp_str		: varying[10] of char;
	tmp		: char;
	escape		: char;
	csi		: char;
	devdepend	: integer;
	devdepend2	: integer;
	term_code	: integer;
	ansi_crt,
	eightbit	: boolean;
  [external(SYS$GETDVI)] function get_dvi	(
		 efn		: integer := %immed 0;
		 chan		: integer := %immed 0;
	%stdescr terminal	: term_type;
	%ref	 itmlst		: dvi_type;
		 isob		: integer := %immed 0;
		 astadr		: integer := %immed 0;
		 astprm		: integer := %immed 0;
		 undefined	: integer := %immed 0
						) : integer;
	external;

  begin
    escape := chr(27);
    csi := chr(155);
    with dvi_buff do
      begin
	item_len  := 4;
	item_code := dvi$_devtype;
	new(buff_add);
	new(len_add);
	end_item  := 0;
      end;
    get_dvi(terminal:='TT:',itmlst:=dvi_buff);
    term_code := dvi_buff.buff_add^;
    with dvi_buff do
	item_code := dvi$_devdepend;
    get_dvi(terminal:='TT:',itmlst:=dvi_buff);
    devdepend := dvi_buff.buff_add^;
    with dvi_buff do
	item_code := dvi$_devdepend2;
    get_dvi(terminal:='TT:',itmlst:=dvi_buff);
    devdepend2 := dvi_buff.buff_add^;
    	{ Determine the generic terminal type.				}
	{ set the following variables:					}
	{	ANSI_CRT true if TT2$M_ANSICRT is set			}
	{	EIGHTBIT true if TT$M_EIGHTBIT and TT2$M_DECCRT2 set	}
	if uand(devdepend2,tt2$m_ansicrt) <> 0 then	{ It's an ANSI crt }
		term_code := 96;			{ Force to VT100 type }
	eightbit := (uand(devdepend2,tt2$m_deccrt2) <> 0) and
		    (uand(devdepend,tt$m_eightbit) <> 0);
	{ Add new terminals in this case statement.  The case number is	}
	{ returned by SYS$GETVI.  Terminals are either row then col, or	}
	{ col then row.							}
	{   ROW_FIRST should be true if the row is given first.  	}
	{   CURSOR_ERL is the sequence for erase-to-end-of-line.	}
	{   CURSOR_ERP is the sequence for erase-to-end-of-page.	}
	{   CURLEN_R is the length of the ROW portion of cursor address	}
	{   CURLEN_C is the length of the COL portion of cursor address	}
	{   CURLEN_L is CURLEN_R + CURLEN_C				}
	{   CURSOR_R is the ROW cursor portion characters		}
	{   CURSOR_C is the COL cursor portion characters		}
    case term_code of
	17 :	{ ADM-3A (/FT2)			}
		begin
		  row_first := true;	{ Sequence is row,col	}
		  cursor_erl := chr(24);
		  cursor_erp := chr(23);
		  curlen_r   := 3;
		  curlen_c   := 1;
		  cursor_l   := 4;
		  for i1 := 1 to 24 do
		    begin
		      tmp := chr(i1+31);			{ Row char}
		      cursor_r[i1] := escape + '=' + tmp;	{ Row part}
		    end;
		  for i1 := 1 to 80 do
		    begin
		      tmp := chr(i1+31);			{ Col char}
		      cursor_c[i1] := tmp;			{ Col part}
		    end;
		end;
	18 :	{ ADDS100 (/FT3)			}
		begin
		  row_first := true;	{ Sequence is row,col	}
		  cursor_erl := escape + 'K';
		  cursor_erp := escape + 'k';
		  curlen_r   := 3;
		  curlen_c   := 1;
		  cursor_l   := 4;
		  for i1 := 1 to 24 do
		    begin
		      tmp := chr(i1+31);			{ Row char}
		      cursor_r[i1] := escape + 'Y' + tmp;	{ Row part}
		    end;
		  for i1 := 1 to 80 do
		    begin
		      tmp := chr(i1+31);			{ Col char}
		      cursor_c[i1] := tmp;			{ Col part}
		    end;
		end;
	19 :	{ IBM3101 (/FT4)			}
		begin
		  row_first := true;	{ Sequence is row,col	}
		  cursor_erl := escape + 'I';
		  cursor_erp := escape + 'J';
		  curlen_r   := 3;
		  curlen_c   := 1;
		  cursor_l   := 4;
		  for i1 := 1 to 24 do
		    begin
		      tmp := chr(i1+39);			{ Row char}
		      cursor_r[i1] := escape + 'Y' + tmp;	{ Row part}
		    end;
		  for i1 := 1 to 80 do
		    begin
		      tmp := chr(i1+39);			{ Col char}
		      cursor_c[i1] := tmp;			{ Col part}
		    end;
		end;
	16 :	{ Teleray 10 (/FT1)			}
		begin
		  row_first := true;	{ Sequence is row,col	}
		  cursor_erl := escape + 'K';
		  cursor_erp := escape + 'J';
		  curlen_r   := 3;
		  curlen_c   := 1;
		  cursor_l   := 4;
		  for i1 := 1 to 24 do
		    begin
		      tmp := chr(i1+31);			{ Row char}
		      cursor_r[i1] := escape + 'Y' + tmp;	{ Row part}
		    end;
		  for i1 := 1 to 80 do
		    begin
		      tmp := chr(i1+31);			{ Col char}
		      cursor_c[i1] := tmp;			{ Col part}
		    end;
		end;
	64 :	{ VT52 (/VT52)				}
		begin
		  row_first := true;	{ Sequence is row,col	}
		  cursor_erl := escape + 'K';
		  cursor_erp := escape + 'J';
		  curlen_r   := 3;
		  curlen_c   := 1;
		  cursor_l   := 4;
		  for i1 := 1 to 24 do
		    begin
		      tmp := chr(i1+31);			{ Row char}
		      cursor_r[i1] := escape + 'Y' + tmp;	{ Row part}
		    end;
		  for i1 := 1 to 80 do
		    begin
		      tmp := chr(i1+31);			{ Col char}
		      cursor_c[i1] := tmp;			{ Col part}
		    end;
		end;
	96 :    { VT100 and ANSI X3.64 standard	(/VT100)}
		{ Note that the row and column strings must always	}
		{ of the same length					}
		begin
		  row_first := true;	{ Sequence is row,col	}
		  if eightbit then
			begin
			  cursor_erl := csi + 'K';
		  	  cursor_erp := csi + 'J';
		  	  curlen_r   := 3;
		  	  curlen_c   := 4;
		  	  cursor_l   := 7;
			end else
			begin
			  cursor_erl := escape + '[K';
			  cursor_erp := escape + '[J';
			  curlen_r   := 4;
			  curlen_c   := 4;
			  cursor_l   := 8;
			end;
		  for i1 := 1 to 24 do
		    begin
		      writev(tmp_str,'00',i1:1);		{ Row chars}
		      tmp_str := substr(tmp_str,length(tmp_str)-1,2);
		      if eightbit then
			  cursor_r[i1] := csi + tmp_str		{ Row part }
		      else
		          cursor_r[i1] := escape + '[' + tmp_str; { Row part }
		    end;
		  for i1 := 1 to 80 do
		    begin
		      writev(tmp_str,'00',i1:1);		{ Col chars}
		      tmp_str := substr(tmp_str,length(tmp_str)-1,2);
		      cursor_c[i1] := ';' + tmp_str + 'H';	{ Col part }
		    end;
		end;
	otherwise
		begin
		  writeln('*** ERROR : Terminal not supported ***');
		  writeln('See TERMDEF.PAS for definning new terminals.');
		  writeln('*** Terminals supported:');
		  writeln('    VT52         Set Terminal/VT52');
		  writeln('    VT100        Set Terminal/VT100');
		  writeln('    VT200	    Set Terminal/Device=VT2xx');
		  writeln('    Teleray 10   Set Terminal/FT1');
		  writeln('    ADM-3A       Set Terminal/FT2');
		  writeln('    ADDS100      Set Terminal/FT3');
		  writeln('    IBM3101      Set Terminal/FT4');
		  writeln;
		  exit;
		end;
    end;
  end;
end.
