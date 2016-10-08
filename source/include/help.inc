[psect(misc2$code)] procedure ident_char;
    var
	command         : char;
    begin
      if (get_com('Enter character to be identified :',command)) then
	case command of
	  ' ' : prt('  - An open pit.',1,1);
	  '!' : prt('! - A potion.',1,1);
	  '"' : prt('" - An amulet, periapt, or necklace.',1,1);
	  '#' : prt('# - A stone wall.',1,1);
	  '$' : prt('$ - Treasure.',1,1);
	 {'%' : prt('% - Not used.',1,1);}
	  '&' : prt('& - Treasure chest.',1,1);
	  '''': prt(''' - An open door.',1,1);
	  '(' : prt('( - Soft armor.',1,1);
	  ')' : prt(') - A shield.',1,1);
	  '*' : prt('* - Gems.',1,1);
	  '+' : prt('+ - A closed door.',1,1);
	  ',' : prt(', - Food or mushroom patch.',1,1);
	  '-' : prt('- - A wand',1,1);
	  '.' : prt('. - Floor.',1,1);
	  '/' : prt('/ - A pole weapon.',1,1);
	 {'0' : prt('0 - Not used.',1,1);}
	  '1' : prt('1 - Entrance to General Store.',1,1);
	  '2' : prt('2 - Entrance to Armory.',1,1);
	  '3' : prt('3 - Entrance to Weaponsmith.',1,1);
	  '4' : prt('4 - Entrance to Temple.',1,1);
	  '5' : prt('5 - Entrance to Alchemy shop.',1,1);
	  '6' : prt('6 - Entrance to Magic-Users store.',1,1);
	 {'7' : prt('7 - Not used.',1,1);}
	 {'8' : prt('8 - Not used.',1,1);}
	 {'9' : prt('9 - Not used.',1,1);}
	  ':' : prt(': - Rubble.',1,1);
	  ';' : prt('; - A loose rock.',1,1);
	  '<' : prt('< - An up staircase.',1,1);
	  '=' : prt('= - A ring.',1,1);
	  '>' : prt('> - A down staircase.',1,1);
	  '?' : prt('? - A scroll.',1,1);
	  '@' : prt(py.misc.name,1,1);
	  'A' : prt('A - Giant Ant Lion.',1,1);
	  'B' : prt('B - The Balrog.',1,1);
	  'C' : prt('C - Gelentanious Cube.',1,1);
	  'D' : prt('D - An Ancient Dragon (Beware).',1,1);
	  'E' : prt('E - Elemental.',1,1);
	  'F' : prt('F - Giant Fly.',1,1);
	  'G' : prt('G - Ghost.',1,1);
	  'H' : prt('H - Hobgoblin.',1,1);
	  'I' : prt('I - Invisible Stalker.',1,1);
	  'J' : prt('J - Jelly.',1,1);
	  'K' : prt('K - Killer Beetle.',1,1);
	  'L' : prt('L - Lich.',1,1);
	  'M' : prt('M - Mummy.',1,1);
	 {'N' : prt('N - Not used.',1,1);}
	  'O' : prt('O - Ooze.',1,1);
	  'P' : prt('P - Giant humanoid.',1,1);
	  'Q' : prt('Q - Quylthulg (Pulsing Flesh Mound).',1,1);
	  'R' : prt('R - Reptile.',1,1);
	  'S' : prt('S - Giant Scorpion.',1,1);
	  'T' : prt('T - Troll.',1,1);
	  'U' : prt('U - Umber Hulk.',1,1);
	  'V' : prt('V - Vampire.',1,1);
	  'W' : prt('W - Wight or Wraith.',1,1);
	  'X' : prt('X - Xorn.',1,1);
	  'Y' : prt('Y - Yeti.',1,1);
	 {'Z' : prt('Z - Not used.',1,1);}
	  '[' : prt('[ - Hard armor.',1,1);
	  '\' : prt('\ - A hafted weapon.',1,1);
	  ']' : prt('] - Misc. armor.',1,1);
	  '^' : prt('^ - A trap.',1,1);
	  '_' : prt('_ - A staff.',1,1);
	 {'`' : prt('` - Not used.',1,1);}
	  'a' : prt('a - Giant Ant.',1,1);
	  'b' : prt('b - Giant Bat.',1,1);
	  'c' : prt('c - Giant Centipede.',1,1);
	  'd' : prt('d - Dragon.',1,1);
	  'e' : prt('e - Floating Eye.',1,1);
	  'f' : prt('f - Giant Frog',1,1);
	  'g' : prt('g - Golem.',1,1);
	  'h' : prt('h - Harpy.',1,1);
	  'i' : prt('i - Icky Thing.',1,1);
	  'j' : prt('j - Jackal.',1,1);
	  'k' : prt('k - Kobold.',1,1);
	  'l' : prt('l - Giant Lice.',1,1);
	  'm' : prt('m - Mold.',1,1);
	  'n' : prt('n - Naga.',1,1);
	  'o' : prt('o - Orc or Ogre.',1,1);
	  'p' : prt('p - Person (Humanoid).',1,1);
	  'q' : prt('q - Quasit.',1,1);
	  'r' : prt('r - Rodent.',1,1);
	  's' : prt('s - Skeleton.',1,1);
	  't' : prt('t - Gaint tick.',1,1);
	 {'u' : prt('u - Not used.',1,1);}
	 {'v' : prt('v - Not used.',1,1);}
	  'w' : prt('w - Worm(s).',1,1);
	 {'x' : prt('x - Not used.',1,1);}
	  'y' : prt('y - Yeek.',1,1);
	  'z' : prt('z - Zombie.',1,1);
	  '{' : prt('{ - Arrow, bolt, or bullet.',1,1);
	  '|' : prt('| - A sword or dagger.',1,1);
	  '}' : prt('} - Bow, crossbow, or sling.',1,1);
	  '~' : prt('~ - Miscellaneous item.',1,1);
	  otherwise prt('Not Used.',1,1);
	end
    end;


	{ Help for available commands					}
[psect(misc2$code)] procedure help;
    begin
      clear(1,1);
prt('B <Dir> Bash (object/creature)|  q        Quaff a potion.',1,1);
prt('C       Display character.    |  r        Read a scroll.',2,1);
prt('D <Dir> Disarm a trap/chest.  |  s        Search for hidden doors.',3,1);
prt('E       Eat some food.        |  t        Take off an item.',4,1);
prt('F       Fill lamp with oil.   |  u        Use a staff.',5,1);
prt('L       Current location.     |  v        Version and credits.',6,1);
prt('P       Print map.            |  w        Wear/Wield an item.',7,1);
prt('R       Rest for a period.    |  x        Exchange weapon.',8,1);
prt('S       Search Mode.          |  /        Identify a character.',9,1);
prt('T <Dir> Tunnel.               |  ?        Display this panel.',10,1);
prt('a       Aim and fire a wand.  |',11,1);
prt('b       Browse a book.        |  ^M       Repeat the last message.',12,1);
prt('c <Dir> Close a door.         |  ^R       Redraw the screen.',13,1);
prt('d       Drop an item.         |  ^Y       Quit the game.',14,1);
prt('e       Equipment list.       |  ^Z       Save character and quit.',15,1);
prt('f       Fire/Throw an item.   |   $       Shell out of game.',16,1);
prt('h       Help on key commands. |',17,1);
prt('i       Inventory list.       |  < Go up an up-staircase.',18,1);
prt('j <Dir> Jam a door with spike.|  > Go down a down-staircase.',19,1);
prt('l <Dir> Look given direction. |  . <Dir>  Move in direction.',20,1);
prt('m       Cast a magic spell.   |  Movement: 7  8  9',21,1);
prt('o <Dir> Open a door/chest.    |            4     6    5 = Rest',22,1);
prt('p       Read a prayer.        |            1  2  3',23,1);
      pause(24);
      draw_cave;
    end;



	{ Help for available wizard commands				}
[psect(wizard$code)] procedure wizard_help;
    begin
      clear(1,1);
      if (wizard2) then
	begin
	  prt('^A - Remove Curse and Cure all maladies.',1,1);
	  prt('^B - Print random objects sample.',2,1);
	  prt('^D - Down/Up n levels.',3,1);
	  prt('^E - Change character.',4,1);
	  prt('^F - Delete monsters.',5,1);
	  prt('^G - Allocate treasures.',6,1);
	  prt('^H - Wizard Help.',7,1);
	  prt('^I - Identify.',8,1);
	  prt('^J - Gain experience.',9,1);
	  prt('^K - Summon monster.',10,1);
	  prt('^L - Wizard light.',11,1);
	  prt('^N - Print monster dictionary.',12,1);
	  prt('^P - Wizard password on/off.',13,1);
	  prt('^T - Teleport player.',14,1);
	  prt('^V - Restore lost character.',15,1);
	  prt('^W - Create any object *CAN CAUSE FATAL ERROR*',16,1);
	end
      else
	begin
	  prt('^A - Remove Curse and Cure all maladies.',1,1);
	  prt('^B - Print random objects sample.',2,1);
	  prt('^D - Down/Up n levels.',3,1);
	  prt('^H - Wizard Help.',4,1);
	  prt('^I - Identify.',5,1);
	  prt('^L - Wizard light.',6,1);
	  prt('^N - Print monster dictionary.',7,1);
	  prt('^P - Wizard password on/off.',8,1);
	  prt('^T - Teleport player.',9,1);
	  prt('^V - Restore lost character.',10,1);
	end;
      pause(24);
      draw_cave;
    end;



	{ Spawn a process to use HELP utility on the MORIA help library -RAK-}
[psect(misc2$code)] procedure moria_help(help_level : vtype);
    var
	dcl_command                     : varying [120] of char;
	flag_bits                       : unsigned;

	{ Spawn a shell and execute DCL command 		-RAK-	}
    [external(LIB$SPAWN)] function dcl(
	%DESCR command  : varying [a] of char;
	    dum2        : integer := %immed 0;
	    dum3        : integer := %immed 0;
	%REF flags      : unsigned;
	    dum5        : integer := %immed 0;
	    dum6        : integer := %immed 0;
	    dum7        : integer := %immed 0;
	    dum8        : integer := %immed 0;
	    dum9        : integer := %immed 0;
	    dum10       : integer := %immed 0 ) : integer;
	external;

    begin
      flag_bits := %X'00000006';
      prt('[Entering Moria Help Library, Use ^Z to resume game]',1,1);
      put_qio;
      dcl_command := 'HELP/PAGE/NOLIBLIST/LIBRARY='+MORIA_HLP;
      dcl_command := dcl_command + ' ' + help_level;
      dcl(dcl_command,flags:=flag_bits);
    end;
