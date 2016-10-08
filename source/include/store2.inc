	{ Comments vary...					-RAK-	}
	{ Comment one : Finished haggling				}
[psect(store$code)] procedure prt_comment1;
      begin
	msg_flag := false;
	case randint(14) of
	  1  : msg_print('Done!');
	  2  : msg_print('Accepted!');
	  3  : msg_print('Fine...');
	  4  : msg_print('Agreed!');
	  5  : msg_print('Ok...');
	  6  : msg_print('Taken!');
	  7  : msg_print('You drive a hard bargin, but taken...');
	  8  : msg_print('You''ll force me bankrupt, but it''s a deal...');
	  9  : msg_print('Sigh...  I''ll take it...');
	 10  : msg_print('My poor sick children may starve, but done!');
	 11  : msg_print('Finally!  I accept...');
	 12  : msg_print('Robbed again...');
	 13  : msg_print('A pleasure to do business with you!');
	 14  : msg_print('My spouse shall skin me, but accepted.');
	end;
      end;

	{ %A1 is offer, %A2 is asking...		}
[psect(store$code)] procedure prt_comment2(offer,asking,final : integer);
      var
	comment				: vtype;
      begin
	if (final > 0) then
	  case randint(3) of
	    1 : comment := '%A2 is my final offer; take it or leave it...';
	    2 : comment := 'I''ll give you no more than %A2.';
	    3 : comment := 'My patience grows thin...  %A2 is final.';
	  end
	else
	  case randint(16) of
	    1 : comment := '%A1 for such a fine item?  HA!  No less than %A2.';
	    2 : comment := '%A1 is an insult!  Try %A2 gold pieces...';
	    3 : comment := '%A1???  Thou would rob my poor starving children?';
	    4 : comment := 'Why I''ll take no less than %A2 gold pieces.';
	    5 : comment := 'Ha!  No less than %A2 gold pieces.';
	    6 : comment := 'Thou blackheart!  No less than %A2 gold pieces.';
	    7 : comment := '%A1 is far too little, how about %A2?';
	    8 : comment := 'I paid more than %A1 for it myself, try %A2.';
	    9 : comment := '%A1?  Are you mad???  How about %A2 gold pieces?';
	   10 : comment := 'As scrap this would bring %A1.  Try %A2 in gold.';
	   11 : comment := 'May fleas of a 1000 orcs molest you.  I want %A2.';
	   12 : comment := 'My mother you can get for %A1, this costs %A2.';
	   13 : comment := 'May your chickens grow lips.  I want %A2 in gold!';
	   14 : comment := 'Sell this for such a pittance.  Give me %A2 gold.';
	   15 : comment := 'May the Balrog find you tasty!  %A2 gold pieces?';
	   16 : comment := 'Your mother was a Troll!  %A2 or I''ll tell...';
	  end;
	insert_num(comment,'%A1',offer,false);
	insert_num(comment,'%A2',asking,false);
	msg_print(comment);
      end;

[psect(store$code)] procedure prt_comment3(offer,asking,final : integer);
      var
	comment				: vtype;
      begin
	if (final > 0) then
	  case randint(3) of
	    1 : comment := 'I''ll pay no more than %A1; take it or leave it.';
	    2 : comment := 'You''ll get no more than %A1 from me...';
	    3 : comment := '%A1 and that''s final.';
	  end
	else
	  case randint(15) of
	    1 : comment := '%A2 for that piece of junk?  No more than %A1';
	    2 : comment := 'For %A2 I could own ten of those.  Try %A1.';
	    3 : comment := '%A2?  NEVER!  %A1 is more like it...';
	    4 : comment := 'Let''s be resonable... How about %A1 gold pieces?';
	    5 : comment := '%A1 gold for that junk, no more...';
	    6 : comment := '%A1 gold pieces and be thankful for it!';
	    7 : comment := '%A1 gold pieces and not a copper more...';
	    8 : comment := '%A2 gold?  HA!  %A1 is more like it...';
	    9 : comment := 'Try about %A1 gold...';
	   10 : comment := 'I wouldn''t pay %A2 for your children, try %A1.';
	   11 : comment := '*CHOKE* For that!?  Let''s say %A1.';
	   12 : comment := 'How about %A1.';
	   13 : comment := 'That looks war surplus!  Say %A1 gold.';
	   14 : comment := 'I''ll buy it as scrap for %A1.';
	   15 : comment := '%A2 is too much, let us say %A1 gold.';
	  end;
	insert_num(comment,'%A1',offer,false);
	insert_num(comment,'%A2',asking,false);
	msg_print(comment);
      end;

	{ Kick 'da bum out...					-RAK-	}
[psect(store$code)] procedure prt_comment4;
      begin
	msg_flag := false;
	case randint(5) of
	  1 :	begin
		  msg_print('ENOUGH!  Thou hath abused me once too often!');
		  msg_print('Out of my place!');
		  msg_print(' ');
		end;
	  2 :	begin
		  msg_print('THAT DOES IT!  You shall waste my time no more!');
		  msg_print('out... Out... OUT!!!');
		  msg_print(' ');
		end;
	  3 :	begin
		  msg_print('This is getting no where...  I''m going home!');
		  msg_print('Come back tomorrow...');
		  msg_print(' ');
		end;
	  4 :	begin
		  msg_print('BAH!  No more shall you insult me!');
		  msg_print('Leave my place...  Begone!');
		  msg_print(' ');
		end;
	  5 :	begin
		  msg_print('Begone!  I have had enough abuse for one day.');
		  msg_print('Come back when thou art richer...');
		  msg_print(' ');
		end;
	end;
	msg_flag := false;
      end;

[psect(store$code)] procedure prt_comment5;
      begin
	case randint(10) of
	  1  :	msg_print('You will have to do better than that!');
	  2  :	msg_print('That''s an insult!');
	  3  :	msg_print('Do you wish to do business or not?');
	  4  :	msg_print('Hah!  Try again...');
	  5  :	msg_print('Ridiculus!');
	  6  :	msg_print('You''ve got to be kidding!');
	  7  :	msg_print('You better be kidding!!');
	  8  :	msg_print('You try my patience.');
	  9  :	msg_print('I don''t hear you.');
	 10  :	msg_print('Hmmm, nice weather we''re having...');
	end;
      end;


[psect(store$code)] procedure prt_comment6;
      begin
	case randint(5) of
	  1  :	msg_print('I must of heard you wrong...');
	  2  :	msg_print('What was that?');
	  3  :	msg_print('I''m sorry, say that again...');
	  4  :	msg_print('What did you say?');
	  5  :	msg_print('Sorry, what was that again?');
	end;
      end;


	{ Displays the set of commands				-RAK-	}
[psect(store$code)] procedure display_commands;
      begin
prt('You may:',21,1);
prt(' p) Purchase an item.           b) Browse store''s inventory.',22,1);
prt(' s) Sell an item.               i) Inventory and Equipment Lists.',23,1);
prt('^Z) Exit from Building.        ^R) Redraw the screen.',24,1);
      end;


	{ Displays the set of commands				-RAK-	}
[psect(store$code)] procedure haggle_commands(typ : integer);
      begin
	if (typ = -1) then
	  prt('Specify an asking-price in gold pieces.',22,1)
	else
	  prt('Specify an offer in gold pieces.',22,1);
	prt('^Z) Quit Haggeling.',23,1);
	prt('',24,1);
      end;


	{ Displays a store's inventory				-RAK-	}
[psect(store$code)] procedure display_inventory(store_num,start : integer);
      var
	i1,i2,stop			: integer;
	out_val1,out_val2		: vtype;
      begin
	with store[store_num] do
	    begin
	      i1 := ((start-1) mod 12);
	      stop := (((start-1) div 12) + 1)*12;
	      if (stop > store_ctr) then stop := store_ctr;
	      while (start <= stop) do
	        begin
		  inventory[inven_max] := store_inven[start].sitem;
		  with inventory[inven_max] do
		    if ((subval > 255) and (subval < 512)) then
		      number := 1;
		  objdes(out_val1,inven_max,true);
		  writev(out_val2,chr(97+i1),') ',out_val1);
		  prt(out_val2,i1+6,1);
		  if (store_inven[start].scost < 0) then
		    begin
		      i2 := abs(store_inven[start].scost);
		      i2 := i2 + trunc(i2*chr_adj);
		      writev(out_val2,i2:6);
		    end
		  else
		    writev(out_val2,store_inven[start].scost:6,' [Fixed]');
		  prt(out_val2,i1+6,60);
		  i1 := i1 + 1;
		  start := start + 1;
	        end;
	      if (i1 < 12) then
	        for i2 := 1 to (12 - i1 + 1) do
		  prt('',i2+i1+5,1);
	    end;
      end;


	{ Re-displays only a single cost			-RAK-	}
[psect(store$code)] procedure display_cost(store_num,pos : integer);
      var
	i1				: integer;
	out_val				: vtype;
      begin
	with store[store_num] do
	  begin
	    i1 := ((pos-1) mod 12);
	    if (store_inven[pos].scost < 0) then
	      begin
		i2 := abs(store_inven[pos].scost);
		i2 := i2 + trunc(i2*chr_adj);
		writev(out_val,i2:6);
	      end
	    else
	      writev(out_val,store_inven[pos].scost:6,' [Fixed]');
	    prt(out_val,i1+6,60);
	  end;
      end;


	{ Displays players gold					-RAK-	}
[psect(store$code)] procedure store_prt_gold;
      var
	out_val			: vtype;
      begin
	writev(out_val,'Gold Remaining : ',py.misc.au:1);
        prt(out_val,19,18);
      end;


	{ Displays store					-RAK-	}
[psect(store$code)] procedure display_store(store_num,cur_top : integer);
      begin
	with store[store_num] do
	  begin
	    clear(1,1);
	    prt(owners[owner].owner_name,4,10);
	    prt('   Item',5,1);
	    prt('Asking Price',5,61);
	    store_prt_gold;
	    display_commands;
	    display_inventory(store_num,cur_top);
	  end;
      end;


	{ Get the ID of a store item and return it's value	-RAK-	}
[psect(store$code)] function get_store_item(
				var com_val	: integer;
				pmt	 	: vtype;
				i1,i2		: integer) : boolean;
      var
		command 	: char;
		out_val		: vtype;
		flag		: boolean;
      begin
	com_val := 0;
	flag := true;
	writev(out_val,'(Items ',chr(i1+96),'-',chr(i2+96),
					', ^Z to exit) ',pmt);
	while (((com_val < i1) or (com_val > i2)) and (flag)) do
	  begin
	    prt(out_val,1,1);
	    inkey(command);
	    com_val := ord(command);
	    case com_val of
		3,25,26,27 :	flag := false;
		otherwise com_val := com_val - 96;
	    end;
	  end;
	msg_flag := false;
	erase_line(msg_line,msg_line);
	get_store_item := flag;
      end;


	{ Increase the insult counter and get pissed if too many -RAK-	}
[psect(store$code)] function increase_insults(store_num : integer) : boolean;
      begin
	increase_insults := false;
	with store[store_num] do
	  begin
	    insult_cur := insult_cur + 1;
	    if (insult_cur > owners[owner].insult_max) then
	      begin
		prt_comment4;
		insult_cur := 0;
		store_open := turn + 2500 + randint(2500);
		increase_insults := true;
	      end;
	  end;
      end;


	{ Decrease insults					-RAK-	}
[psect(store$code)] procedure decrease_insults(store_num : integer);
      begin
	with store[store_num] do
	  begin
	    insult_cur := insult_cur - 2;
	    if (insult_cur < 0) then insult_cur := 0;
	  end;
      end;


	{ Have insulted while haggling				-RAK-	}
[psect(store$code)] function haggle_insults(store_num : integer) : boolean;
	begin
	  haggle_insults := false;
	  if (increase_insults(store_num)) then
	    haggle_insults := true
	  else
	    prt_comment5;
	end;

[psect(store$code)] function recieve_offer(
				store_num		: integer;
				comment 		: vtype;
 				var new_offer 		: integer;
				last_offer,factor	: integer) : integer;
	var
		flag				: boolean;

	function get_haggle(comment : vtype; var num : integer) : boolean;
	  var
		i1,clen			: integer;
		out_val			: vtype;
		flag			: boolean;
	  begin
	    flag := true;
	    i1 := 0;
	    clen := length(comment) + 1;
	    repeat
	      msg_print(comment);
	      msg_flag := false;
	      if (not(get_string(out_val,1,clen,40))) then
		begin
	          flag := false;
		  erase_line(msg_line,msg_line);
		end;
	      readv(out_val,i1,error:=continue);
	    until((i1 <> 0) or not(flag));
	    if (flag) then num := i1;
	    get_haggle := flag;
	  end;

	begin
	  recieve_offer := 0;
	  flag := false;
	  repeat
	    if (get_haggle(comment,new_offer)) then
	      begin
	        if (new_offer*factor >= last_offer*factor) then 
	          flag := true
	        else if (haggle_insults(store_num)) then
		  begin
		    recieve_offer := 2;
		    flag := true;
		  end
	      end
	    else
	      begin
	        recieve_offer := 1;
	        flag := true;
	      end;
	  until (flag);
        end;


	{ Haggling routine					-RAK-	}
[psect(store$code)] function purchase_haggle(
				store_num	: integer;
				var price	: integer;
				item		: treasure_type) : integer;
      var
	max_sell,min_sell,max_buy		: integer;
	cost,cur_ask,final_ask,min_offer	: integer;
	last_offer,new_offer,final_flag,x3	: integer;
	x1,x2					: real;
	min_per,max_per				: real;
	flag,loop_flag				: boolean;
	out_val,comment				: vtype;

      begin
	flag := false;
	purchase_haggle := 0;
	price := 0;
	final_flag := 0;
	msg_flag := false;
	with store[store_num] do
	  with owners[owner] do
	    begin
	      cost := sell_price(store_num,max_sell,min_sell,item);
	      max_sell := max_sell + trunc(max_sell*chr_adj);
	      if (max_sell < 0) then max_sell := 1;
	      min_sell := min_sell + trunc(min_sell*chr_adj);
	      if (min_sell < 0) then min_sell := 1;
	      max_buy  := trunc(cost*(1-max_inflate));
	      min_per  := haggle_per;
	      max_per  := min_per*3.0;
	    end;
	haggle_commands(1);
	cur_ask   := max_sell;
	final_ask := min_sell;
	min_offer := max_buy;
	last_offer := min_offer;
	comment := 'Asking : ';
	repeat
	  repeat
	    loop_flag := true;
	    writev(out_val,comment,cur_ask:1);
	    put_buffer(out_val,2,1);
	    case recieve_offer(store_num,'What do you offer? ',
			     new_offer,last_offer,1) of
	      1 : begin
		    purchase_haggle := 1;
		    flag   := true;
		  end;
	      2 : begin
		    purchase_haggle := 2;
		    flag   := true;
		  end;
	      otherwise if (new_offer > cur_ask) then
			  begin
			    prt_comment6;
			    loop_flag := false;
			  end
		        else if (new_offer = cur_ask) then
	                  begin
			    flag := true;
			    price := new_offer;
			  end;
	    end;
	  until ((flag) or (loop_flag));
	  if (not(flag)) then
	    begin
	      x1 := (new_offer - last_offer)/(cur_ask - last_offer);
	      if (x1 < min_per) then
		begin
		  flag := haggle_insults(store_num);
		  if (flag) then purchase_haggle := 2;
		end
	      else
		begin
		  if (x1 > max_per) then 
		    begin
		      x1 := x1*0.75;
		      if (x1 < max_per) then x1 := max_per;
		    end;
	          x2 := (x1 + (randint(5) - 3)/100.0);
	          x3 := trunc((cur_ask-new_offer)*x2) + 1;
		  cur_ask := cur_ask - x3;
		  if (cur_ask < final_ask) then
		    begin
		      cur_ask := final_ask;
		      comment := 'Final Offer : ';
		      final_flag := final_flag + 1;
		      if (final_flag > 3) then
			begin
			  if (increase_insults(store_num)) then
			    purchase_haggle := 2
			  else
			    purchase_haggle := 1;
			  flag := true;
			end;
		    end
		  else if (new_offer >= cur_ask) then
	            begin
		      flag := true;
		      price := new_offer;
		    end;
		  if (not(flag)) then
		    begin
	              last_offer := new_offer;
		      prt('',2,1);
	              writev(out_val,'Your last offer : ',last_offer:1);
	              put_buffer(out_val,2,40);
		      prt_comment2(last_offer,cur_ask,final_flag);
		    end;
	        end;
	    end;
	until (flag);
	prt('',2,1);
	display_commands;
      end;


	{ Haggling routine					-RAK-	}
[psect(store$code)] function sell_haggle(
				store_num	: integer;
				var price	: integer;
				item		: treasure_type) : integer;
      var
	max_sell,max_buy,min_buy		: integer;
	cost,cur_ask,final_ask,min_offer	: integer;
	last_offer,new_offer,final_flag,x3	: integer;
	max_gold				: integer;
	x1,x2					: real;
	min_per,max_per				: real;
	flag,loop_flag				: boolean;
	comment,out_val				: vtype;

      begin
	flag := false;
	sell_haggle := 0;
	price := 0;
	final_flag := 0;
	msg_flag := false;
	with store[store_num] do
	  begin
	    cost := item_value(item);
	    if (cost < 1) then
	      begin
		sell_haggle := 3;
		flag := true;
	      end
	    else
	      with owners[owner] do
	        begin
		  cost := cost - trunc(cost*chr_adj) -
			  trunc(cost*rgold_adj[owner_race,py.misc.prace]);
		  if (cost < 1) then cost := 1;
	          max_sell := trunc(cost*(1+max_inflate));
	          max_buy  := trunc(cost*(1-max_inflate));
	          min_buy  := trunc(cost*(1-min_inflate));
		  if (min_buy < max_buy) then min_buy := max_buy;
	          min_per  := haggle_per;
	          max_per  := min_per*3.0;
		  max_gold := max_cost;
	        end;
	  end;
	if (not(flag)) then
	  begin
	    haggle_commands(-1);
	    if (max_buy > max_gold) then
	      begin
		final_flag:= 1;
		comment   := 'Final offer : ';
	        cur_ask   := max_gold;
		final_ask := max_gold;
msg_print('I am sorry, but I have not the money to afford such a fine item.');
msg_print(' ');
	      end
	    else
	      begin
		cur_ask   := max_buy;
	        final_ask := min_buy;
		if (final_ask > max_gold) then
		  final_ask := max_gold;
	        comment := 'Offer : ';
	      end;
	    min_offer := max_sell;
	    last_offer := min_offer;
	    if (cur_ask < 1) then cur_ask := 1;
	    repeat
	      repeat
	        loop_flag := true;
	        writev(out_val,comment,cur_ask:1);
	        put_buffer(out_val,2,1);
	        case recieve_offer(store_num,'What price do you ask? ',
				 new_offer,last_offer,-1) of
	          1 : begin
		        sell_haggle := 1;
		        flag   := true;
		      end;
	          2 : begin
		        sell_haggle := 2;
		        flag   := true;
		      end;
	          otherwise if (new_offer < cur_ask) then
	                      begin
				prt_comment6;
				loop_flag := false;
			      end
			    else if (new_offer = cur_ask) then
			      begin
			        flag := true;
			        price := new_offer;
			      end;
	        end;
	      until ((flag) or (loop_flag));
	      if (not(flag)) then
	        begin
		  msg_flag := false;
	          x1 := (last_offer - new_offer)/(last_offer - cur_ask);
	          if (x1 < min_per) then
		    begin
		      flag := haggle_insults(store_num);
		      if (flag) then sell_haggle := 2;
		    end
	          else
		    begin
		      if (x1 > max_per) then 
		        begin
		          x1 := x1*0.75;
		          if (x1 < max_per) then x1 := max_per;
		        end;
	              x2 := (x1 + (randint(5) - 3)/100.0);
	              x3 := trunc((new_offer-cur_ask)*x2) + 1;
		      cur_ask := cur_ask + x3;
		      if (cur_ask > final_ask) then
		        begin
		          cur_ask := final_ask;
		          comment := 'Final Offer : ';
		          final_flag := final_flag + 1;
		          if (final_flag > 3) then
			    begin
			      if (increase_insults(store_num)) then
			        sell_haggle := 2
			      else
			        sell_haggle := 1;
			      flag := true;
			    end;
		        end
		      else if (new_offer <= cur_ask) then
			begin
			  flag := true;
			  price := new_offer;
			end;

		      if (not(flag)) then
		        begin
	                  last_offer := new_offer;
		          prt('',2,1);
	                  writev(out_val,'Your last bid   : ',last_offer:1);
	                  put_buffer(out_val,2,40);
			  prt_comment3(cur_ask,last_offer,final_flag);
		        end;
	            end;
	        end;
	    until (flag);
	    prt('',2,1);
	    display_commands;
	  end;
      end;


	{ Buy an item from a store				-RAK-	}
[psect(store$code)] function store_purchase(
			store_num 	: integer;
			var cur_top 	: integer) : boolean;
      var
	i1,item_val,price			: integer;
	item_new,choice				: integer;
	save_number				: integer;
	out_val					: vtype;
      begin
	store_purchase := false;
	with store[store_num] do
	  begin
		{ i1 = number of objects shown on screen	}
	    if (cur_top = 13) then
	      i1 := store_ctr - 12
	    else if (store_ctr > 12) then
	      i1 := 12
	    else
	      i1 := store_ctr;
	    if (store_ctr < 1) then
	      msg_print('I am currently out of stock.')
		{ Get the item number to be bought		}
	    else if (get_store_item(item_val,
			'Which item are you interested in? ',1,i1)) then
	      begin
		item_val := item_val + cur_top - 1;	{ true item_val	}
		inventory[inven_max] := store_inven[item_val].sitem;
		with inventory[inven_max] do
		  if ((subval > 255) and (subval < 512)) then
		    begin
		      save_number := number;
		      number := 1;
		    end
		  else
		    save_number := 1;
		if (inven_check_weight) then
		  if (inven_check_num) then
		    begin
		      if (store_inven[item_val].scost > 0) then
			begin
			  price := store_inven[item_val].scost;
			  choice := 0;
			end
		      else
	choice := purchase_haggle(store_num,price,inventory[inven_max]);
		      case choice of
			0 : begin
			      if (py.misc.au >= price) then
				begin
				  prt_comment1;
				  decrease_insults(store_num);
				  py.misc.au := py.misc.au - price;
				  store_destroy(store_num,item_val,true);
				  inven_carry(item_new);
				  objdes(out_val,item_new,true);
	out_val := 'You have ' + out_val + ' (' + chr(item_new+96) + ')';
				  msg_print(out_val);
				  if (cur_top > store_ctr) then
				    begin
				      cur_top := 1;
				      display_inventory(store_num,cur_top);
				    end
				  else
				    with store_inven[item_val] do
				      if (save_number > 1) then
				        begin
					  if (scost < 0) then
					    begin
				              scost := price;
					      display_cost(store_num,item_val);
					    end;
				        end
				      else
				        display_inventory(store_num,item_val);
				  store_prt_gold;
				end
			      else
				begin
				  if (increase_insults(store_num)) then
				    store_purchase := true
				  else
				    begin
				prt_comment1;
				msg_print('Liar!  You have not the gold!');
				    end;
				end
			    end;
			2 : store_purchase := true;
			otherwise ;
		      end;
		      prt('',2,1);
		    end
		  else
		    prt('You cannot carry that many different items.',1,1)
		else
		  prt('You can not carry that much weight.',1,1);
	      end;
	  end;
      end;


	{ Sell an item to the store				-RAK-	}
[psect(store$code)] function store_sell(store_num,cur_top : integer) : boolean;
      var
	item_val,i1			: integer;
	item_pos,price			: integer;
	redraw				: boolean;
	out_val				: vtype;
      begin
	store_sell := false;
	with store[store_num] do
	  begin
	    redraw := false;
	    if (get_item(item_val,'Which one? ',redraw,1,inven_ctr)) then
	      begin
		if (redraw) then display_store(store_num,cur_top);
		inventory[inven_max] := inventory[item_val];
		with inventory[inven_max] do
		  if ((subval > 255) and (subval < 512)) then
		    number := 1;
		objdes(out_val,inven_max,true);
		out_val := 'Selling ' +out_val+ ' (' + chr(item_val+96) + ')';
		msg_print(out_val);
		msg_print(' ');
		if (inventory[inven_max].tval in store_buy[store_num]) then
		  if (store_check_num(store_num)) then
		    case sell_haggle(store_num,price,inventory[inven_max]) of
		      0 : begin
		            prt_comment1;
			    py.misc.au := py.misc.au + price;
		            inven_destroy(item_val);
		            store_carry(store_num,item_pos);
			    if (item_pos > 0) then
			      if (item_pos < 13) then
			        if (cur_top < 13) then
				  display_inventory(store_num,item_pos)
			        else
				  display_inventory(store_num,cur_top)
			      else if (cur_top > 12) then
			        display_inventory(store_num,item_pos);
			    store_prt_gold;
			  end;
		      2 : store_sell := true;
		      3 : begin
			    msg_print('How dare you!');
			    msg_print('I will not buy that!');
			    store_sell := increase_insults(store_num);
		          end;
		      otherwise ;
		    end
		  else
		    prt('I have not the room in my store to keep it...',1,1)
		else
		  prt('I do not buy such items.',1,1);
	      end
	    else if (redraw) then
	      display_store(store_num,cur_top);
	  end;
      end;


	{ Entering a store					-RAK-	}
[psect(store$code)] procedure enter_store(store_num : integer);
      var
	com_val,cur_top			: integer;
	command				: char;
	exit_flag			: boolean;
      begin
	with store[store_num] do
	  if (store_open < turn) then
	    begin
	      exit_flag := false;
	      cur_top := 1;
	      display_store(store_num,cur_top);
	      repeat
		if (get_com('',command)) then
		  begin
		    msg_flag := false;
		    com_val := ord(command);
		    case com_val of
		      18      : display_store(store_num,cur_top);
		      98      : begin
				  if (cur_top = 1) then
				    if (store_ctr > 12) then
				      begin
				        cur_top := 13;
					display_inventory(store_num,cur_top);
				      end
				    else
				      prt('Entire inventory is shown.',1,1)
				  else
				    begin
				      cur_top := 1;
				      display_inventory(store_num,cur_top);
				    end
				end;
		      101     : begin	{ Equipment List	}
				  if (inven_command('e',0,0)) then
				    display_store(store_num,cur_top);
				end;
		      105     : begin	{ Inventory		}
				  if (inven_command('i',0,0)) then
				    display_store(store_num,cur_top);
				end;
		      116     : begin	{ Take off		}
				  if (inven_command('t',0,0)) then
				    display_store(store_num,cur_top);
				end;
		      119     : begin	{ Wear			}
				  if (inven_command('w',0,0)) then
				    display_store(store_num,cur_top);
				end;
		      120     : begin	{ Switch weapon		}
				  if (inven_command('x',0,0)) then
				    display_store(store_num,cur_top);
				end;
		      112     : exit_flag := store_purchase(store_num,cur_top);
		      115     : exit_flag := store_sell(store_num,cur_top);
		      otherwise prt('Invalid Command.',1,1);
		    end;
		  end
		else
		  exit_flag := true;
	      until(exit_flag);
	      draw_cave;
	    end
	  else
	    msg_print('The doors are locked.');
      end;
