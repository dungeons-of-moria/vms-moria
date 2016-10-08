	{ Returns the value for any given object		-RAK-	}
[psect(store$code)] function item_value(item : treasure_type) : integer;

      function search_list(x1,x2 : integer) : integer;
	var
	  i1,i2		: integer;
	begin
	  i1 := 0;
	  i2 := 0;
	  repeat
	    i1 := i1 + 1;
	    with object_list[i1] do
	      if ((tval = x1) and (subval = x2)) then
		i2 := cost;
	  until ((i1 = max_objects) or (i2 > 0));
	  search_list := i2;
	end;

      begin
	with item do
	  begin
	    item_value := cost;
	    if (tval in [20,21,22,23,30,31,32,33,34,35,36]) then
	      begin	{ Weapons and armor	}
		if (index(name,'^') > 0) then
		  item_value := search_list(tval,subval)*number
		else if (tval in [20,21,22,23]) then
		  begin
		    if (tohit < 0) then
		      item_value := 0
		    else if (todam < 0) then
		      item_value := 0
		    else if (toac < 0) then
		      item_value := 0
		    else
		      item_value := (cost+(tohit+todam+toac)*100)*number;
		  end
		else
		  begin
		    if (toac < 0) then
		      item_value := 0
		    else
		      item_value := (cost+toac*100)*number;
		  end;
	      end
	    else if (tval in [10,11,12,13]) then
	      begin	{ Ammo			}
		if (index(name,'^') > 0) then
		  item_value := search_list(tval,1)*number
		else
		  begin
		    if (tohit < 0) then
		      item_value := 0
		    else if (todam < 0) then
		      item_value := 0
		    else if (toac < 0) then
		      item_value := 0
		    else
		      item_value := (cost+(tohit+todam+toac)*10)*number;
		  end;
	      end
	    else if (tval in [70,71,75,76,80]) then
	      begin	{ Potions, Scrolls, and Food	}
		if (index(name,'|') > 0) then
		  case tval of
		    70,71 : item_value :=  20;
		    75,76 : item_value :=  20;
		    80    : item_value :=   1;
		    otherwise ;
		  end
	      end
	    else if (tval in [40,45]) then
	      begin	{ Rings and amulets	}
		if (index(name,'|') > 0) then
		  case tval of
		    40 : item_value := 45;
		    45 : item_value := 45;
		    otherwise ;
		  end
		else if (index(name,'^') > 0) then
		  item_value := abs(cost);
	      end
	    else if (tval in [55,60,65]) then
	      begin	{ Wands rods, and staffs}
		if (index(name,'|') > 0) then
		  case tval of
		    55 : item_value := 70;
		    60 : item_value := 60;
		    65 : item_value := 50;
		    otherwise ;
		  end
		else if (index(name,'^') = 0) then
		  begin
		    item_value := cost + trunc(cost/20.0)*p1;
		  end;
	      end;
	  end;
      end;


	{ Asking price for an item				-RAK-	}
[psect(store$code)] function sell_price (snum 	: integer;
			 var max_sell,min_sell 	: integer;
			 item 			: treasure_type
					) : integer;
      var
	i1			: integer;
      begin
	with store[snum] do
	  begin
	    i1 := item_value(item);
	    if (item.cost > 0) then
	      begin
	        i1 := i1 +
		   trunc(i1*rgold_adj[owners[owner].owner_race,py.misc.prace]);
	        if (i1 < 1) then i1 := 1;
	        max_sell := trunc(i1*(1+owners[owner].max_inflate));
	        min_sell := trunc(i1*(1+owners[owner].min_inflate));
	        if (min_sell > max_sell) then min_sell := max_sell;
	        sell_price := i1;
	      end
	    else
	      begin
		max_sell := 0;
		min_sell := 0;
		sell_price := 0;
	      end;
	  end;
      end;


	{ Check to see if he will be carrying too many objects	-RAK-	}
[psect(store$code)] function store_check_num(store_num : integer) : boolean;
      var
	item_num,i1			: integer;
	flag				: boolean;
      begin
	store_check_num := false;
	with store[store_num] do
	  if (store_ctr < store_inven_max) then
	    store_check_num := true
	  else if ((inventory[inven_max].subval > 255) and 
		   (inventory[inven_max].subval < 512)) then
	    for i1 := 1 to store_ctr do
	      with store_inven[i1].sitem do
	        if (tval = inventory[inven_max].tval) then
		  if (subval = inventory[inven_max].subval) then
		    store_check_num := true;
      end;


	{ Add the item in INVEN_MAX to stores inventory.	-RAK-	}
[psect(store$code)] procedure store_carry(    store_num : integer;
					  var ipos 	: integer);
      var
	item_num,item_val		: integer;
	typ,subt,icost,dummy		: integer;
	flag				: boolean;

	{ Insert INVEN_MAX at given location	}
      procedure insert(store_num,pos,icost : integer);
        var
		i1	: integer;
        begin
	  with store[store_num] do
	    begin
	      for i1 :=  store_ctr downto pos do
	        store_inven[i1+1] := store_inven[i1];
	      store_inven[pos].sitem := inventory[inven_max];
	      store_inven[pos].scost := -icost;
	      store_ctr := store_ctr + 1;
	    end;
        end;

	{ Store_carry routine			}
      begin
	ipos := 0;
	identify(inventory[inven_max]);
	known2(inventory[inven_max].name);
	sell_price(store_num,icost,dummy,inventory[inven_max]);
	if (icost > 0) then
	  begin
	    with inventory[inven_max] do
	      with store[store_num] do
	        begin
	          item_val := 0;
	          item_num := number;
	          flag := false;
	          typ  := tval;
	          subt := subval;
	          repeat
	            item_val := item_val + 1;
	            with store_inven[item_val].sitem do
		      if (typ = tval) then
		        begin
		          if (subt = subval) then{ Adds to other item	}
		            if (subt > 255) then
			      begin
				if (number < 24) then 
				  number := number + item_num;
			        flag := true;
			      end
		        end
		      else if (typ > tval) then
		        begin		{ Insert into list		}
		          insert(store_num,item_val,icost);
		          flag := true;
			  ipos := item_val;
		        end;
	          until ((item_val >= store_ctr) or (flag));
	          if (not(flag)) then	{ Becomes last item in list	}
		    begin
		      insert(store_num,store_ctr+1,icost);
		      ipos := store_ctr;
		    end;
	        end;
	  end;
      end;



	{ Destroy an item in the stores inventory.  Note that if	}
	{ 'one_of' is false, an entire slot is destroyed	-RAK-	}
[psect(store$code)] procedure store_destroy(
			store_num,item_val 	: integer; 
			one_of 			: boolean);
      var
		i2 	: integer;
      begin
	with store[store_num] do
	  begin
	    inventory[inven_max] := store_inven[item_val].sitem;
	    with store_inven[item_val].sitem do
	      begin
	        if ((number > 1) and (subval < 512) and (one_of))  then
	          begin
	            number := number - 1;
		    inventory[inven_max].number := 1;
	          end
	        else
	          begin
		    for i2 := item_val to store_ctr-1 do
		      store_inven[i2] := store_inven[i2+1];
		    store_inven[store_ctr].sitem := blank_treasure;
		    store_inven[store_ctr].scost := 0;
		    store_ctr := store_ctr - 1;
	          end;
              end
	  end;
      end;



	{ Initializes the stores with owners			-RAK-	}
[psect(setup$code)] procedure store_init;
      var
	i1,i2,i3			: integer;
      begin
	i1 := max_owners div max_stores;
	for i2 := 1 to max_stores do
	  with store[i2] do
	    begin
	      owner := max_stores*(randint(i1)-1) + i2;
	      insult_cur := 0;
	      store_open := 0;
	      store_ctr  := 0;
	      for i3 := 1 to store_inven_max do 
		begin
		  store_inven[i3].sitem := blank_treasure;
		  store_inven[i3].scost := 0;
		end;
	    end;
      end;


	{ Creates an item and inserts it into store's inven	-RAK-	}
[psect(store$code)] procedure store_create(store_num : integer);
      var
	i1,tries,cur_pos,dummy			: integer;
      begin
	tries := 0;
	popt(cur_pos);
	with store[store_num] do
	  repeat
	    i1 := store_choice[store_num,randint(store$choices)];
	    t_list[cur_pos] := inventory_init[i1];
	    magic_treasure(cur_pos,obj_town_level);
	    inventory[inven_max] := t_list[cur_pos];
	    if (store_check_num(store_num)) then
	      with t_list[cur_pos] do
	        if (cost > 0) then	{ Item must be good	}
		  if (cost < owners[owner].max_cost) then
		    begin
		      store_carry(store_num,dummy);
		      tries := 10;
		    end;
	    tries := tries + 1;
	  until(tries > 3);
	pusht(cur_pos);
      end;


	{ Initialize and up-keep the store's inventory.		-RAK-	}
[psect(store$code)] procedure store_maint;
      var
	i1,i2,dummy		: integer;
      begin
	for i1 := 1 to max_stores do
	  with store[i1] do
	    begin
	      insult_cur := 0;
	      if (store_ctr > store$max_inven) then
	        for i2 := 1 to (store_ctr-store$max_inven+2) do
		  store_destroy(i1,randint(store_ctr),false)
	      else if (store_ctr < store$min_inven) then
		begin
	          for i2 := 1 to (store$min_inven-store_ctr+2) do
		    store_create(i1);
		end
	      else
		begin
		  for i2 := 1 to (1+randint(store$turn_around)) do
		    store_destroy(i1,randint(store_ctr),true);
		  for i2 := 1 to (1+randint(store$turn_around)) do
		    store_create(i1);
		end;
	    end;
      end;
