--------------------------------------------------------------------------------
				README
--------------------------------------------------------------------------------
1> Makefile is for listtest and makefile is for warmup1
To compile warmup1, please type 
	"make" or "make clean; make" or "make warmup1"
To compile listtest, 
	make -f Makefile clean 
	make -f Makefile make  

2> i> cs402.h , 
   ii> my402list.h , 
   iii> listtest.c and 
   iv> Makefile (for listtest) are taken from the webpage for cs402.
	On top of that, the page includes 
    v> warmup1.c (sort functionallity)
    vi> my402list.c ( DLL functions)	
    vii> makefile 
    viii> readme.txt		
_______________________________________________________________________________

3> Identical Time stamp condition will be satisfied only when 2 absolute numbers (given as input) are same. i.e if 2 transactions happen at same DATE-TIME-SEC.

	If user wants to treat transactions which may happen on same day but different times of the day, as identical (just like the program o/p where only dateMM DD YYYY is mentioned), plz uncomment lines 589-599  in warmup1.c

4>fout declared as static FILE* is set to stderr in main(). Can be changed as needed.

5>While running the test scripts on tcsh shell on nunki, 
#!/bin/tcsh ->line had to be added to the script. 

6>Cntl+D being EOF for stdin.

 -------------------------------------------------------------------------------
				FLOW
--------------------------------------------------------------------------------

1>Main() in warmup1.c takes in i/p arguments and verifies the file for read.

2>HandleData() when called in a loop for every line read by fgets(), validates the fields and stores them in a data structure.

3>sort_add() adds the data node to the My402List at the correct place by comparing with the list elements.This repeats till the loop exits.

4>printTransaction() prints the user data from all the My402ListElem nodes in the My402List.

5>The program frees all the allocated memory(free user data node and My402UnlinkAll()) and exits. 
-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------END-------------------------------------------  
