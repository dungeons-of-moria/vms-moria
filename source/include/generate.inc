	{ Generates a random dungeon level			-RAK-	}
[psect(generate$code)] procedure generate_cave;
    type
	coords = record
		y	: integer;
		x	: integer;
	end;
    var
	doorstk			: array [1..100] of coords;
	doorptr			: integer;


	{ Always picks a correct direction		}
      procedure correct_dir(var rdir,cdir : integer; y1,x1,y2,x2 : integer);
	var
		test_dir	: integer;
	begin
	  if (y1 < y2) then
	    rdir :=  1
	  else if (y1 = y2) then
	    rdir :=  0
	  else
	    rdir := -1;
	  if (x1 < x2) then
	    cdir :=  1
	  else if (x1 = x2) then
	    cdir :=  0
	  else
	    cdir := -1;
	  if ((rdir <> 0) and (cdir <> 0)) then
	    case randint(2) of
		1 :	rdir := 0;
		2 :	cdir := 0
	    end
	end;


	{ Chance of wandering direction			}
      procedure rand_dir(var rdir,cdir : integer; 
			     y1,x1,y2,x2,chance : integer);
	begin
	  case randint(chance) of
		1 :	begin
			  rdir := -1;
			  cdir :=  0
			end;
		2 :	begin
			  rdir :=  1;
			  cdir :=  0
			end;
		3 :	begin
			  rdir :=  0;
			  cdir := -1
			end;
		4 :	begin
			  rdir :=  0;
			  cdir :=  1
			end;
		otherwise correct_dir(rdir,cdir,y1,x1,y2,x2)
	  end
	end;
    

	{ Blanks out entire cave				-RAK-	}
    procedure blank_cave;
      var
	i1,i2	: integer;
      begin
        for i1 := 1 to max_height do
	  for i2 := 1 to max_width do
	    cave[i1,i2] := blank_floor;
      end;


	{ Fills in empty spots with desired rock		-RAK-	}
	{ Note: 9 is a temporary value.				}
    procedure fill_cave(fill : floor_type);
      var
	i1,i2	: integer;
      begin
        for i1 := 2 to cur_height-1 do
	  for i2 := 2 to cur_width-1 do
	    with cave[i1,i2] do
	      if (fval in [0,8,9]) then
	        begin
		  fval := fill.ftval;
		  fopen := fill.ftopen;
	        end;
      end;


	{ Places indestructable rock around edges of dungeon	-RAK-	}
    procedure place_boundry;
      var
	i1	: integer;
      begin
	for i1 := 1 to cur_height do
	  begin
	    cave[i1,1].fval          := boundry_wall.ftval;
	    cave[i1,1].fopen         := boundry_wall.ftopen;
	    cave[i1,cur_width].fval  := boundry_wall.ftval;
	    cave[i1,cur_width].fopen := boundry_wall.ftopen;
	  end;
	for i1 := 1 to cur_width do
	  begin
	    cave[1,i1].fval           := boundry_wall.ftval;
	    cave[1,i1].fopen          := boundry_wall.ftopen;
	    cave[cur_height,i1].fval  := boundry_wall.ftval;
	    cave[cur_height,i1].fopen := boundry_wall.ftopen;
	  end;
      end;


	{ Places "streamers" of rock through dungeon		-RAK-	}
    procedure place_streamer(rock : floor_type; treas_chance : integer);
      var
	i1,y,x,dir,ty,tx,t1,t2		: integer;
	flag				: boolean;
      begin

	{ Choose starting point and direction		}
	y := trunc(cur_height/2.0) + 11 - randint(23);
	x := trunc(cur_width/2.0)  + 16 - randint(33);

	dir := randint(8);	{ Number 1-4, 6-9	}
	if (dir > 4) then dir := dir + 1;

	{ Place streamer into dungeon			}
	flag := false;	{ Set to true when y,x are out-of-bounds}
	t1 := 2*dun_str_rng + 1;	{ Constants	}
	t2 :=   dun_str_rng + 1;
	repeat
	  for i1 := 1 to dun_str_den do
	    begin
	      ty := y + randint(t1) - t2;
	      tx := x + randint(t1) - t2;
	      if (in_bounds(ty,tx)) then
	        with cave[ty,tx] do
		  if (fval = rock_wall1.ftval) then
		    begin
		      fval := rock.ftval;
		      fopen := rock.ftopen;
		      if (randint(treas_chance) = 1) then
		        place_gold(ty,tx);
		    end;
	    end;
	  if (not(move(dir,y,x))) then flag := true;
	until(flag);
      end;


	{ Place a trap with a given displacement of point	-RAK-	}
    procedure vault_trap(y,x,yd,xd,num : integer);
      var
	count,y1,x1,i1		: integer;
	flag			: boolean;
      begin
	for i1 := 1 to num do
	  begin
	    flag := false;
	    count := 0;
	    repeat
	      y1 := y - yd - 1 + randint(2*yd+1);
	      x1 := x - xd - 1 + randint(2*xd+1);
	      with cave[y1,x1] do
	        if (fval in floor_set) then
	          if (tptr = 0) then
		    begin
		      place_trap(y1,x1,1,randint(max_trapa));
		      flag := true;
		    end;
	      count := count + 1;
	    until((flag) or (count > 5));
	  end;
      end;


	{ Place a trap with a given displacement of point	-RAK-	}
    procedure vault_monster(y,x,num : integer);
      var
		i1,y1,x1		: integer;
      begin
	for i1 := 1 to num do
	  begin
	    y1 := y;
	    x1 := x;
	    summon_monster(y1,x1,true);
	  end;
      end;


	{ Builds a room at a row,column coordinate		-RAK-	}
    procedure build_room(yval,xval : integer);
      var
		y_height,y_depth	: integer;
		x_left,x_right		: integer;
		i1,i2			: integer;
		cur_floor		: floor_type;
      begin
	if (dun_level <= randint(25)) then
	  cur_floor := lopen_floor	{ Floor with light	}
	else
	  cur_floor := dopen_floor;	{ Dark floor		}
	y_height := yval - randint(4);
	y_depth  := yval + randint(3);
	x_left   := xval - randint(11);
	x_right  := xval + randint(11);
	for i1 := y_height to y_depth do
	  for i2 := x_left to x_right do
	    begin
	      cave[i1,i2].fval  := cur_floor.ftval;
	      cave[i1,i2].fopen := cur_floor.ftopen;
	    end;
	for i1 := (y_height - 1) to (y_depth + 1) do
	  begin
	    cave[i1,x_left-1].fval   := rock_wall1.ftval;
	    cave[i1,x_left-1].fopen  := rock_wall1.ftopen;
	    cave[i1,x_right+1].fval  := rock_wall1.ftval;
	    cave[i1,x_right+1].fopen := rock_wall1.ftopen;
	  end;
	for i1 := x_left to x_right do
	  begin
	    cave[y_height-1,i1].fval  := rock_wall1.ftval;
	    cave[y_height-1,i1].fopen := rock_wall1.ftopen;
	    cave[y_depth+1,i1].fval   := rock_wall1.ftval;
	    cave[y_depth+1,i1].fopen  := rock_wall1.ftopen;
	  end
      end;


	{ Builds a room at a row,column coordinate		-RAK-	}
	{ Type 1 unusual rooms are several overlapping rectangular ones	}
    procedure build_type1(yval,xval : integer);
      var
		y_height,y_depth	: integer;
		x_left,x_right		: integer;
		i0,i1,i2		: integer;
		cur_floor		: floor_type;
      begin
	if (dun_level <= randint(25)) then
	  cur_floor := lopen_floor	{ Floor with light	}
	else
	  cur_floor := dopen_floor;	{ Dark floor		}
	for i0 := 1 to (1 + randint(2)) do
	  begin
	    y_height := yval - randint(4);
	    y_depth  := yval + randint(3);
	    x_left   := xval - randint(11);
	    x_right  := xval + randint(11);
	    for i1 := y_height to y_depth do
	      for i2 := x_left to x_right do
		begin
		  cave[i1,i2].fval  := cur_floor.ftval;
		  cave[i1,i2].fopen := cur_floor.ftopen;
		end;
	    for i1 := (y_height - 1) to (y_depth + 1) do
	      begin
		with cave[i1,x_left-1] do
		  if (fval <> cur_floor.ftval) then
		    begin
		      fval  := rock_wall1.ftval;
		      fopen := rock_wall1.ftopen;
		    end;
		with cave[i1,x_right+1] do
		  if (fval <> cur_floor.ftval) then
		    begin
		      fval  := rock_wall1.ftval;
		      fopen := rock_wall1.ftopen;
		    end;
	      end;
	    for i1 := x_left to x_right do
	      begin
		with cave[y_height-1,i1] do
		  if (fval <> cur_floor.ftval) then
		    begin
		      fval  := rock_wall1.ftval;
		      fopen := rock_wall1.ftopen;
		    end;
		with cave[y_depth+1,i1] do
		  if (fval <> cur_floor.ftval) then
		    begin
		      fval  := rock_wall1.ftval;
		      fopen := rock_wall1.ftopen;
		    end;
	      end;
	  end;
      end;


	{ Builds an unusual room at a row,column coordinate	-RAK-	}
	{ Type 2 unusual rooms all have an inner room:			}
	{   1 - Just an inner room with one door			}
	{   2 - An inner room within an inner room			}
	{   3 - An inner room with pillar(s)				}
	{   4 - Inner room has a maze					}
	{   5 - A set of four inner rooms				}
    procedure build_type2(yval,xval : integer);
      var
		y_height,y_depth	: integer;
		x_left,x_right		: integer;
		i1,i2			: integer;
		cur_floor		: floor_type;
      begin
	if (dun_level <= randint(30)) then
	  cur_floor := lopen_floor	{ Floor with light	}
	else
	  cur_floor := dopen_floor;	{ Dark floor		}
	y_height := yval - 4;
	y_depth  := yval + 4;
	x_left   := xval - 11;
	x_right  := xval + 11;
	for i1 := y_height to y_depth do
	  for i2 := x_left to x_right do
	    begin
	      cave[i1,i2].fval  := cur_floor.ftval;
	      cave[i1,i2].fopen := cur_floor.ftopen;
	    end;
	for i1 := (y_height - 1) to (y_depth + 1) do
	  begin
	    cave[i1,x_left-1].fval   := rock_wall1.ftval;
	    cave[i1,x_left-1].fopen  := rock_wall1.ftopen;
	    cave[i1,x_right+1].fval  := rock_wall1.ftval;
	    cave[i1,x_right+1].fopen := rock_wall1.ftopen;
	  end;
	for i1 := x_left to x_right do
	  begin
	    cave[y_height-1,i1].fval  := rock_wall1.ftval;
	    cave[y_height-1,i1].fopen := rock_wall1.ftopen;
	    cave[y_depth+1,i1].fval   := rock_wall1.ftval;
	    cave[y_depth+1,i1].fopen  := rock_wall1.ftopen;
	  end;
	{ The inner room		}
	y_height := y_height + 2;
	y_depth  := y_depth  - 2;
	x_left   := x_left   + 2;
	x_right  := x_right  - 2;
	for i1 := (y_height - 1) to (y_depth + 1) do
	  begin
	    cave[i1,x_left-1].fval   := 8;
	    cave[i1,x_right+1].fval  := 8;
	  end;
	for i1 := x_left to x_right do
	  begin
	    cave[y_height-1,i1].fval  := 8;
	    cave[y_depth+1,i1].fval   := 8;
	  end;
	{ Inner room varitions		}
	case randint(5) of
	  1 :	begin	{ Just an inner room...	}
		  case randint(4) of	{ Place a door	}
		    1 :	place_secret_door(y_height-1,xval);
		    2 :	place_secret_door(y_depth+1,xval);
		    3 :	place_secret_door(yval,x_left-1);
		    4 :	place_secret_door(yval,x_right+1);
		  end;
		  vault_monster(yval,xval,1);
		end;
	  2 :	begin	{ Treasure Vault	}
		  case randint(4) of	{ Place a door	}
		    1 :	place_secret_door(y_height-1,xval);
		    2 :	place_secret_door(y_depth+1,xval);
		    3 :	place_secret_door(yval,x_left-1);
		    4 :	place_secret_door(yval,x_right+1);
		  end;
		  for i1 := yval-1 to yval+1 do
		    begin
		      cave[i1,xval-1].fval   := 8;
		      cave[i1,xval+1].fval   := 8;
		    end;
		  cave[yval-1,xval].fval  := 8;
		  cave[yval+1,xval].fval  := 8;
		  case randint(4) of	{ Place a door	}
		    1 :	place_locked_door(yval-1,xval);
		    2 :	place_locked_door(yval+1,xval);
		    3 :	place_locked_door(yval,xval-1);
		    4 :	place_locked_door(yval,xval+1);
		  end;
			{ Place an object in the treasure vault	}
		  case randint(10) of
		    1 : place_up_stairs(yval,xval);
		    2 : place_down_stairs(yval,xval);
		    otherwise place_object(yval,xval);
		  end;
			{ Guard the treasure well		}
		  vault_monster(yval,xval,2+randint(3));
			{ If the monsters don't get 'em...	}
		  vault_trap(yval,xval,4,10,2+randint(3));
		end;
	  3 :	begin	{ Inner pillar(s)...	}
		  case randint(4) of	{ Place a door	}
		    1 :	place_secret_door(y_height-1,xval);
		    2 :	place_secret_door(y_depth+1,xval);
		    3 :	place_secret_door(yval,x_left-1);
		    4 :	place_secret_door(yval,x_right+1);
		  end;
		  for i1 := yval-1 to yval+1 do
		    for i2 := xval-1 to xval+1 do
		      cave[i1,i2].fval   := 8;
		  if (randint(2) = 1) then
		    begin
		      case randint(2) of
		        1 : begin
		              for i1 := yval-1 to yval+1 do
			        for i2 := xval-6 to xval-4 do
			          cave[i1,i2].fval   := 8;
		              for i1 := yval-1 to yval+1 do
			        for i2 := xval+4 to xval+6 do
			          cave[i1,i2].fval   := 8;
		            end;
		        2 : begin
		              for i1 := yval-1 to yval+1 do
			        for i2 := xval-7 to xval-5 do
			          cave[i1,i2].fval   := 8;
		              for i1 := yval-1 to yval+1 do
			        for i2 := xval+5 to xval+7 do
			          cave[i1,i2].fval   := 8;
		            end;
		      end;
		      if (randint(3) = 1) then	{ Inner rooms	}
			begin
			  for i1 := xval-5 to xval+5 do
			    begin
			      cave[yval-1,i1].fval := 8;
			      cave[yval+1,i1].fval := 8;
			    end;
			  case randint(2) of
			    1 : place_secret_door(yval+1,xval-3);
			    2 : place_secret_door(yval-1,xval-3);
			  end;
			  case randint(2) of
			    1 : place_secret_door(yval+1,xval+3);
			    2 : place_secret_door(yval-1,xval+3);
			  end;
			  if (randint(3) = 1) then place_object(yval,xval-2);
			  if (randint(3) = 1) then place_object(yval,xval+2);
			  vault_monster(yval,xval-2,randint(2));
			  vault_monster(yval,xval+2,randint(2));
			end;
		    end;
		end;
	  4 :	begin	{ Maze inside...	}
		  case randint(4) of	{ Place a door	}
		    1 :	place_secret_door(y_height-1,xval);
		    2 :	place_secret_door(y_depth+1,xval);
		    3 :	place_secret_door(yval,x_left-1);
		    4 :	place_secret_door(yval,x_right+1);
		  end;
		  for i1 := y_height to y_depth do
		    for i2 := x_left to x_right do
		      if (odd(i2+i1)) then
			cave[i1,i2].fval := 8;
		{ Monsters just love mazes...		}
		  vault_monster(yval,xval-5,randint(3));
		  vault_monster(yval,xval+5,randint(3));
		{ Traps make them entertaining...	}
		  vault_trap(yval,xval-3,2,8,randint(3));
		  vault_trap(yval,xval+3,2,8,randint(3));
		{ Mazes should have some treasure too..	}
		  for i1 := 1 to 3 do
		    random_object(yval,xval,1);
		end;
	  5 :	begin	{ Four small rooms...	}
		  for i1 := y_height to y_depth do
		    cave[i1,xval].fval := 8;
		  for i1 := x_left to x_right do
		    cave[yval,i1].fval := 8;
		  case randint(2) of
		    1 :	begin
			  i1 := randint(10);
			  place_secret_door(y_height-1,xval-i1);
			  place_secret_door(y_height-1,xval+i1);
			  place_secret_door(y_depth+1,xval-i1);
			  place_secret_door(y_depth+1,xval+i1);
			end;
		    2 :	begin
			  i1 := randint(3);
			  place_secret_door(yval+i1,x_left-1);
			  place_secret_door(yval-i1,x_left-1);
			  place_secret_door(yval+i1,x_right+1);
			  place_secret_door(yval-i1,x_right+1);
			end;
		  end;
		{ Treasure in each one...		}
		  random_object(yval,xval,2+randint(2));
		{ Gotta have some monsters...		}
		  vault_monster(yval+2,xval-4,randint(2));
		  vault_monster(yval+2,xval+4,randint(2));
		  vault_monster(yval-2,xval-4,randint(2));
		  vault_monster(yval-2,xval+4,randint(2));
		end;
	end;
      end;


	{ Builds a room at a row,column coordinate		-RAK-	}
	{ Type 3 unusual rooms are cross shaped				}
    procedure build_type3(yval,xval : integer);
      var
		y_height,y_depth	: integer;
		x_left,x_right		: integer;
		i0,i1,i2		: integer;
		cur_floor		: floor_type;
      begin
	if (dun_level <= randint(25)) then
	  cur_floor := lopen_floor	{ Floor with light	}
	else
	  cur_floor := dopen_floor;	{ Dark floor		}
	i0 := 2 + randint(2);
	y_height := yval - i0;
	y_depth  := yval + i0;
	x_left   := xval - 1;
	x_right  := xval + 1;
	for i1 := y_height to y_depth do
	  for i2 := x_left to x_right do
	    begin
	      cave[i1,i2].fval  := cur_floor.ftval;
	      cave[i1,i2].fopen := cur_floor.ftopen;
	    end;
	for i1 := (y_height - 1) to (y_depth + 1) do
	  begin
	    with cave[i1,x_left-1] do
	      begin
		fval  := rock_wall1.ftval;
		fopen := rock_wall1.ftopen;
	      end;
	    with cave[i1,x_right+1] do
	      begin
		fval  := rock_wall1.ftval;
		fopen := rock_wall1.ftopen;
	      end;
	  end;
	for i1 := x_left to x_right do
	  begin
	    with cave[y_height-1,i1] do
	      begin
		fval  := rock_wall1.ftval;
		fopen := rock_wall1.ftopen;
	      end;
	    with cave[y_depth+1,i1] do
	      begin
		fval  := rock_wall1.ftval;
		fopen := rock_wall1.ftopen;
	      end;
	  end;
	i0 := 2 + randint(9);
	y_height := yval - 1;
	y_depth  := yval + 1;
	x_left   := xval - i0;
	x_right  := xval + i0;
	for i1 := y_height to y_depth do
	  for i2 := x_left to x_right do
	    begin
	      cave[i1,i2].fval  := cur_floor.ftval;
	      cave[i1,i2].fopen := cur_floor.ftopen;
	    end;
	for i1 := (y_height - 1) to (y_depth + 1) do
	  begin
	    with cave[i1,x_left-1] do
	      if (fval <> cur_floor.ftval) then
		begin
		  fval  := rock_wall1.ftval;
		  fopen := rock_wall1.ftopen;
		end;
	    with cave[i1,x_right+1] do
	      if (fval <> cur_floor.ftval) then
		begin
		  fval  := rock_wall1.ftval;
		  fopen := rock_wall1.ftopen;
		end;
	  end;
	for i1 := x_left to x_right do
	  begin
	    with cave[y_height-1,i1] do
	      if (fval <> cur_floor.ftval) then
		begin
		  fval  := rock_wall1.ftval;
		  fopen := rock_wall1.ftopen;
		end;
	    with cave[y_depth+1,i1] do
	      if (fval <> cur_floor.ftval) then
		begin
		  fval  := rock_wall1.ftval;
		  fopen := rock_wall1.ftopen;
		end;
	  end;
	{ Special features...			}
	case randint(4) of
	  1 :	begin	{ Large middle pillar		}
		  for i1 := yval-1 to yval+1 do
		    for i2 := xval-1 to xval+1 do
		      cave[i1,i2].fval := 8;
		end;
	  2 :	begin	{ Inner treasure vault		}
		  for i1 := yval-1 to yval+1 do
		    begin
		      cave[i1,xval-1].fval   := 8;
		      cave[i1,xval+1].fval   := 8;
		    end;
		  cave[yval-1,xval].fval  := 8;
		  cave[yval+1,xval].fval  := 8;
		  case randint(4) of	{ Place a door	}
		    1 :	place_secret_door(yval-1,xval);
		    2 :	place_secret_door(yval+1,xval);
		    3 :	place_secret_door(yval,xval-1);
		    4 :	place_secret_door(yval,xval+1);
		  end;
		{ Place a treasure in the vault		}
		  place_object(yval,xval);
		{ Let's gaurd the treasure well...	}
		  vault_monster(yval,xval,2+randint(2));
		{ Traps naturally			}
		  vault_trap(yval,xval,4,4,1+randint(3));
		end;
	  3 :	begin
		  if (randint(3) = 1) then
		    begin
		      cave[yval-1,xval-2].fval := 8;
		      cave[yval+1,xval-2].fval := 8;
		      cave[yval-1,xval+2].fval := 8;
		      cave[yval-1,xval+2].fval := 8;
		      cave[yval-2,xval-1].fval := 8;
		      cave[yval-2,xval+1].fval := 8;
		      cave[yval+2,xval-1].fval := 8;
		      cave[yval+2,xval+1].fval := 8;
		      if (randint(3) = 1) then
			begin
			  place_secret_door(yval,xval-2);
			  place_secret_door(yval,xval+2);
			  place_secret_door(yval-2,xval);
			  place_secret_door(yval+2,xval);
			end;
		    end
		  else if (randint(3) = 1) then
		    begin
		      cave[yval,xval].fval := 8;
		      cave[yval-1,xval].fval := 8;
		      cave[yval+1,xval].fval := 8;
		      cave[yval,xval-1].fval := 8;
		      cave[yval,xval+1].fval := 8;
		    end
		  else if (randint(3) = 1) then
		    cave[yval,xval].fval := 8;
		end;
	  4 :	;
	end;
      end;


	{ Constructs a tunnel between two points		}
    procedure tunnel(row1,col1,row2,col2 : integer);
      var
	tmp_row,tmp_col		: integer;
	row_dir,col_dir 	: integer;
	i1,i2,tmp		: integer;
	tunstk			: array [1..1000] of coords;
	wallstk			: array [1..1000] of coords;
	tunptr			: integer;
	wallptr			: integer;
	stop_flag,door_flag	: boolean;


	{ Main procedure for Tunnel			}
	{ Note: 9 is a temporary value		}
      begin
	stop_flag := false;
	door_flag := false;
	tunptr    := 0;
	wallptr   := 0;
	correct_dir(row_dir,col_dir,row1,col1,row2,col2);
	repeat
	  if (randint(100) > dun_tun_chg) then
	    rand_dir(row_dir,col_dir,row1,col1,row2,col2,dun_tun_rnd);
	  tmp_row := row1 + row_dir;
	  tmp_col := col1 + col_dir;
	  while (not(in_bounds(tmp_row,tmp_col))) do
	    begin
	      rand_dir(row_dir,col_dir,row1,col1,row2,col2,dun_tun_rnd);
	      tmp_row := row1 + row_dir;
	      tmp_col := col1 + col_dir;
	    end;
	  with cave[tmp_row,tmp_col] do
	    if (fval = rock_wall1.ftval) then
	      begin
		row1 := tmp_row;
		col1 := tmp_col;
		if (wallptr < 1000) then
		  wallptr := wallptr + 1;
		wallstk[wallptr].y := row1;
		wallstk[wallptr].x := col1;
		for i1 := row1-1 to row1+1 do
		  for i2 := col1-1 to col1+1 do
		    if (in_bounds(i1,i2)) then
		      with cave[i1,i2] do
			if (fval in wall_set) then
			  fval := 9;
	      end
	    else if (fval = corr_floor1.ftval) then
	      begin
		row1 := tmp_row;
		col1 := tmp_col;
		if (not(door_flag)) then
		  begin
		    if (doorptr <= 100) then
		      begin
			doorptr := doorptr + 1;
			doorstk[doorptr].y := row1;
			doorstk[doorptr].x := col1;
		      end;
		    door_flag := true;
		  end;
		if (randint(100) > dun_tun_con) then
		  stop_flag := true;
	      end
	    else if (fval = 0) then
	      begin
		row1 := tmp_row;
		col1 := tmp_col;
		if (tunptr < 1000) then
		  tunptr := tunptr + 1;
		tunstk[tunptr].y := row1;
		tunstk[tunptr].x := col1;
		door_flag := false;
	      end
	    else if (fval <> 9) then
	      begin
		row1 := tmp_row;
		col1 := tmp_col;
	      end;
	until (((row1 = row2) and (col1 = col2)) or (stop_flag));
	for i1 := 1 to tunptr do
	  begin
	    cave[tunstk[i1].y,tunstk[i1].x].fval  := corr_floor1.ftval;
	    cave[tunstk[i1].y,tunstk[i1].x].fopen := corr_floor1.ftopen;
	  end;
	for i1 := 1 to wallptr do
	  with cave[wallstk[i1].y,wallstk[i1].x] do
	    if (fval = 9) then
	      begin
		if (randint(100) < dun_tun_pen) then
		  place_door(wallstk[i1].y,wallstk[i1].x)
		else
		  begin
		    fval  := corr_floor2.ftval;
		    fopen := corr_floor2.ftopen;
		  end;
	      end;
      end;


	{ Places door at y,x position if at least 2 walls found	}
    procedure try_door(y,x : integer);

      function next_to(y,x : integer) : boolean;
	begin
	  if (next_to8(y,x,[4,5,6]) > 2) then
	    if ((cave[y-1,x].fval in wall_set) and 
		(cave[y+1,x].fval in wall_set)) then
	      next_to := true
	    else if ((cave[y,x-1].fval in wall_set) and 
		     (cave[y,x+1].fval in wall_set)) then
	      next_to := true
	    else
	      next_to := false
	  else
	    next_to := false
	end;

      begin
	if (randint(100) > dun_tun_jct) then
	  if (cave[y,x].fval = corr_floor1.ftval) then
	    if (next_to(y,x)) then
	      place_door(y,x);
      end;


	{ Cave logic flow for generation of new dungeon		}
    procedure cave_gen;
      type
	spot_type = record
		endx	: integer;
		endy	: integer;
	end;
	room_type = array [1..20,1..20] of boolean;
      var
	room_map		: room_type;
	i1,i2,i3,i4		: integer;
	y1,x1,y2,x2		: integer;
	pick1,pick2		: integer;
	row_rooms,col_rooms	: integer;
	alloc_level		: integer;
	yloc			: array [1..400] of worlint;
	xloc			: array [1..400] of worlint;

      begin
        seed := get_seed;
        row_rooms := 2*trunc(cur_height/screen_height);
        col_rooms := 2*trunc(cur_width /screen_width);
        for i1 := 1 to row_rooms do
	  for i2 := 1 to col_rooms do
	    room_map[i1,i2] := false;
        for i1 := 1 to randnor(dun_roo_mea,2) do
	  room_map[randint(row_rooms),randint(col_rooms)] := true;
        i3 := 0;
        for i1 := 1 to row_rooms do
	  for i2 := 1 to col_rooms do
	    if (room_map[i1,i2] = true) then
	      begin
	        i3 := i3 + 1;
	        yloc[i3] := (i1-1)*(quart_height*2 + 1) + quart_height + 1;
	        xloc[i3] := (i2-1)*(quart_width*2  + 1) + quart_width  + 1;
		if (dun_level > randint(dun_unusual)) then
		  case randint(3) of
		    1 : build_type1(yloc[i3],xloc[i3]);
		    2 : build_type2(yloc[i3],xloc[i3]);
		    3 : build_type3(yloc[i3],xloc[i3]);
		  end
		else
	          build_room(yloc[i3],xloc[i3]);
	      end;
        for i4 := 1 to i3 do
	  begin
	    pick1 := randint(i3);
	    pick2 := randint(i3);
	    y1 := yloc[pick1];
	    x1 := xloc[pick1];
	    yloc[pick1] := yloc[pick2];
	    xloc[pick1] := xloc[pick2];
	    yloc[pick2] := y1;
	    xloc[pick2] := x1
	  end;
	doorptr := 0;
        for i4 := 1 to i3-1 do
	  begin
	    y1 := yloc[i4];
	    x1 := xloc[i4];
	    y2 := yloc[i4+1];
	    x2 := xloc[i4+1];
	    tunnel(y2,x2,y1,x1)
	  end;
        fill_cave(rock_wall1);
	for i1 := 1 to dun_str_mag do
	  place_streamer(rock_wall2,dun_str_mc);
	for i1 := 1 to dun_str_qua do
	  place_streamer(rock_wall3,dun_str_qc);
	place_boundry;
		{ Place intersection doors	}
	for i1 := 1 to doorptr do
	  begin
	    try_door(doorstk[i1].y,doorstk[i1].x-1);
	    try_door(doorstk[i1].y,doorstk[i1].x+1);
	    try_door(doorstk[i1].y-1,doorstk[i1].x);
	    try_door(doorstk[i1].y+1,doorstk[i1].x);
	  end;
	alloc_level := trunc(dun_level/3);
	if (alloc_level < 2) then
	  alloc_level := 2
	else if (alloc_level > 10) then
	  alloc_level := 10;
	place_stairs(2,randint(2)+2,3);
	place_stairs(1,randint(2),3);
	alloc_monster([1,2],(randint(8)+min_malloc_level+alloc_level),0,true);
	alloc_object([4],3,randint(alloc_level));
	alloc_object([1,2],5,randnor(treas_room_alloc,3));
	alloc_object([1,2,4],5,randnor(treas_any_alloc,3));
	alloc_object([1,2,4],4,randnor(treas_gold_alloc,3));
	alloc_object([1,2,4],1,randint(alloc_level));
	if (dun_level >= win_mon_appear) then place_win_monster;
      end;


	{ Builds a store at a row,column coordinate			}
    procedure build_store(store_num,y,x : integer);
      var
		yval,y_height,y_depth	: integer;
		xval,x_left,x_right	: integer;
		i1,i2,cur_pos		: integer;
      begin
	yval	 := (y-1)*10 + 6;
	xval     := (x-1)*16 + 17;
	y_height := yval - randint(3);
	y_depth  := yval + randint(4);
	x_left   := xval - randint(6);
	x_right  := xval + randint(6);
	for i1 := y_height to y_depth do
	  for i2 := x_left to x_right do
	    begin
	      cave[i1,i2].fval  := boundry_wall.ftval;
	      cave[i1,i2].fopen := boundry_wall.ftopen;
	    end;
	case randint(4) of
	  1 : begin
		i1 := randint(y_depth-y_height) + y_height - 1;
		i2 := x_left;
	      end;
	  2 : begin
		i1 := randint(y_depth-y_height) + y_height - 1;
		i2 := x_right;
	      end;
	  3 : begin
		i1 := y_depth;
		i2 := randint(x_right-x_left) + x_left - 1;
	      end;
	  4 : begin
		i1 := y_height;
		i2 := randint(x_right-x_left) + x_left - 1;
	      end;
	end;
	with cave[i1,i2] do
	  begin
	    fval  := corr_floor3.ftval;
	    fopen := corr_floor3.ftopen;
	    popt(cur_pos);
	    tptr := cur_pos;
	    t_list[cur_pos] := store_door[store_num];
	  end;
      end;


	{ Town logic flow for generation of new town		}
    procedure town_gen;
      type
	room_type = array [1..2,1..3] of boolean;
      var
	y,x			: integer;
	i1,i2,i3,i4,i5		: integer;
	rooms			: array [1..6] of integer;

      begin
	seed := town_seed;
        for i1 := 1 to 6 do rooms[i1] := i1;
	i4 := 6;
	for i1 := 1 to 2 do
	  for i2 := 1 to 3 do
	    begin
	      i3 := randint(i4);
	      build_store(rooms[i3],i1,i2);
	      for i5 := i3 to i4-1 do rooms[i5] := rooms[i5+1];
	      i4 := i4 - 1;
	    end;
        fill_cave(dopen_floor);
	place_boundry;
	if (odd(turn mod 5000)) then
	  begin		{ Night	}
	    for i1 := 1 to cur_height do
	      for i2 := 1 to cur_width do
		if (cave[i1,i2].fval <> dopen_floor.ftval) then
		  cave[i1,i2].pl := true;
	    place_stairs(2,1,0);
	    seed := get_seed;
	    alloc_monster([1,2],min_malloc_tn,3,true);
	    store_maint;
	  end
	else
	  begin		{ Day	}
            for i1 := 1 to cur_height do
	      for i2 := 1 to cur_width do
	        cave[i1,i2].pl := true;
	    place_stairs(2,1,0);
	    seed := get_seed;
	    alloc_monster([1,2],min_malloc_td,3,true);
	    store_maint;
	  end;
      end;


    begin
      panel_row_min	:= 0;
      panel_row_max	:= 0;
      panel_col_min	:= 0;
      panel_col_max	:= 0;
      char_row		:= -1;
      char_col		:= -1;

      tlink;
      mlink;
      blank_cave;

      if (dun_level = 0) then
	begin
	  cur_height := screen_height;
	  cur_width  := screen_width;
	  max_panel_rows := trunc(cur_height/screen_height)*2 - 2;
	  max_panel_cols := trunc(cur_width /screen_width )*2 - 2;
	  panel_row := max_panel_rows;
	  panel_col := max_panel_cols;
	  town_gen;
	end
      else
	begin
	  cur_height := max_height;
	  cur_width  := max_width;
	  max_panel_rows := trunc(cur_height/screen_height)*2 - 2;
	  max_panel_cols := trunc(cur_width /screen_width )*2 - 2;
	  panel_row := max_panel_rows;
	  panel_col := max_panel_cols;
	  cave_gen;
	end;
    end;
