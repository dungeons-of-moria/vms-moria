[inherit('moria.env')] module a;
{  TERMDEF : uses the values returned by SYS$GETDVI to set up the proper}
{       addressing codes.  New terminals can be added, or existing ones }
{       changed wihtout re-compiling the main source.  You can use      }
{       compile.com by specifying:                                      }
{               $ cterm :== @DISK_NAME:[FILE_PATH]compile termdef       }
[global] procedure termdef;
  type
	term_type       =       packed array [1..3] of char;
	dvi_type        =       record
		item_len        : wordint;
		item_code       : wordint;
		buff_add        : ^integer;
		len_add         : ^integer;
		end_item        : integer;
	end;
  var
	dvi_buff        : dvi_type;
	i1              : integer;
	tmp_str         : varying[10] of char;
	tmp             : char;
	escape          : char;

  [external(SYS$GETDVI)] function get_dvi	(
		 efn            : integer := %immed 0;
		 chan           : integer := %immed 0;
	%stdescr terminal       : term_type;
	%ref     itmlst         : dvi_type;
		 isob           : integer := %immed 0;
		 astadr         : integer := %immed 0;
		 astprm         : integer := %immed 0;
		 undefined      : integer := %immed 0
						) : integer;
	external;

  begin
    escape := chr(27);
    with dvi_buff do
      begin
	item_len  := 4;
	item_code := 6;
	new(buff_add);
	new(len_add);
	end_item  := 0;
      end;
    get_dvi(terminal:='TT:',itmlst:=dvi_buff);
	{ Add new terminals in this case statement.  The case number is }
	{ returned by SYS$GETVI.  Terminals are either row then col, or }
	{ col then row.                                                 }
	{   ROW_FIRST should be true if the row is given first.         }
	{   CURSOR_ERL is the sequence for erase-to-end-of-line.        }
	{   CURSOR_ERP is the sequence for erase-to-end-of-page.        }
	{   CURLEN_R is the length of the ROW portion of cursor address }
	{   CURLEN_C is the length of the COL portion of cursor address }
	{   CURLEN_L is CURLEN_R + CURLEN_C                             }
	{   CURSOR_R is the ROW cursor portion characters               }
	{   CURSOR_C is the COL cursor portion characters               }
    case dvi_buff.buff_add^ of
	17 :    { ADM-3A (/FT2)                 }
		begin
		  row_first := true;    { Sequence is row,col   }
		  cursor_erl := chr(24);
		  cursor_erp := chr(23);
		  curlen_r   := 3;
		  curlen_c   := 1;
		  cursor_l   := 4;
		  for i1 := 1 to 24 do
		    begin
		      tmp := chr(i1+31);                        { Row char}
		      cursor_r[i1] := escape + '=' + tmp;       { Row part}
		    end;
		  for i1 := 1 to 80 do
		    begin
		      tmp := chr(i1+31);                        { Col char}
		      cursor_c[i1] := tmp;                      { Col part}
		    end;
		end;
	18 :    { ADDS100 (/FT3)                        }
		begin
		  row_first := true;    { Sequence is row,col   }
		  cursor_erl := escape + 'K';
		  cursor_erp := escape + 'k';
		  curlen_r   := 3;
		  curlen_c   := 1;
		  cursor_l   := 4;
		  for i1 := 1 to 24 do
		    begin
		      tmp := chr(i1+31);                        { Row char}
		      cursor_r[i1] := escape + 'Y' + tmp;       { Row part}
		    end;
		  for i1 := 1 to 80 do
		    begin
		      tmp := chr(i1+31);                        { Col char}
		      cursor_c[i1] := tmp;                      { Col part}
		    end;
		end;
	19 :    { IBM3101 (/FT4)                        }
		begin
		  row_first := true;    { Sequence is row,col   }
		  cursor_erl := escape + 'I';
		  cursor_erp := escape + 'J';
		  curlen_r   := 3;
		  curlen_c   := 1;
		  cursor_l   := 4;
		  for i1 := 1 to 24 do
		    begin
		      tmp := chr(i1+39);                        { Row char}
		      cursor_r[i1] := escape + 'Y' + tmp;       { Row part}
		    end;
		  for i1 := 1 to 80 do
		    begin
		      tmp := chr(i1+39);                        { Col char}
		      cursor_c[i1] := tmp;                      { Col part}
		    end;
		end;
	16 :    { Teleray 10 (/FT1)                     }
		begin
		  row_first := true;    { Sequence is row,col   }
		  cursor_erl := escape + 'K';
		  cursor_erp := escape + 'J';
		  curlen_r   := 3;
		  curlen_c   := 1;
		  cursor_l   := 4;
		  for i1 := 1 to 24 do
		    begin
		      tmp := chr(i1+31);                        { Row char}
		      cursor_r[i1] := escape + 'Y' + tmp;       { Row part}
		    end;
		  for i1 := 1 to 80 do
		    begin
		      tmp := chr(i1+31);                        { Col char}
		      cursor_c[i1] := tmp;                      { Col part}
		    end;
		end;
	64 :    { VT52 (/VT52)                          }
		begin
		  row_first := true;    { Sequence is row,col   }
		  cursor_erl := escape + 'K';
		  cursor_erp := escape + 'J';
		  curlen_r   := 3;
		  curlen_c   := 1;
		  cursor_l   := 4;
		  for i1 := 1 to 24 do
		    begin
		      tmp := chr(i1+31);                        { Row char}
		      cursor_r[i1] := escape + 'Y' + tmp;       { Row part}
		    end;
		  for i1 := 1 to 80 do
		    begin
		      tmp := chr(i1+31);                        { Col char}
		      cursor_c[i1] := tmp;                      { Col part}
		    end;
		end;
	96,110: { VT100 and ANSI X3.64 standard (/VT100)                }
		{ VT200 series terminals                                }
		{ Note that the row and column strings must always      }
		{ of the same length                                    }
		begin
		  row_first := true;    { Sequence is row,col   }
		  cursor_erl := escape + '[K';
		  cursor_erp := escape + '[J';
		  curlen_r   := 4;
		  curlen_c   := 4;
		  cursor_l   := 8;
		  for i1 := 1 to 24 do
		    begin
		      writev(tmp_str,'00',i1:1);                { Row chars}
		      tmp_str := substr(tmp_str,length(tmp_str)-1,2);
		      cursor_r[i1] := escape + '[' + tmp_str;   { Row part }
		    end;
		  for i1 := 1 to 80 do
		    begin
		      writev(tmp_str,'00',i1:1);                { Col chars}
		      tmp_str := substr(tmp_str,length(tmp_str)-1,2);
		      cursor_c[i1] := ';' + tmp_str + 'H';      { Col part }
		    end;
		end;
	otherwise
		begin
		  writeln('*** ERROR : Terminal not supported ***');
		  writeln('See TERMDEF.PAS for definning new terminals.');
		  writeln('*** Terminals supported:');
		  writeln('    VT52         Set Terminal/VT52');
		  writeln('    VT100        Set Terminal/VT100');
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
