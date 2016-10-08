$!
$!  MORIA build command file:	example of how to build a moria executable
$!	from the source.  Note that in this example moria is built with
$!	no error checking on.  This greatly increases the efficiency of
$!	the program, but should only be used with de-bugged versions.
$!
$!
$ Setup:
$	on warning then continue
$	on error then goto error_trap
$!
$ Build_paths:
$	cur_path := 'f$directory()'
$	path_dist 	:= 'cur_path'
$	cur_len = 'f$length(cur_path)' - 1
$	cur_path := 'f$extract(0,cur_len,cur_path)'
$	path_source 	:= 'cur_path'.source]
$	path_include 	:= 'cur_path'.source.include]
$	path_macro	:= 'cur_path'.source.macro]
$	path_execute	:= 'cur_path'.execute]
$!
$ define_logicals:
$	define/nolog mor_main 	'path_dist'
$	define/nolog mor_source	'path_source'
$	define/nolog mor_include	'path_include'
$	define/nolog mor_macro	'path_macro'
$	define/nolog mor_execute	'path_execute'
$!
$ START:
$	if p1.eqs."?"		then goto HELP
$	if p1.eqs."LINK"	then goto LINK_STEP
$	if p1.eqs."TERMDEF"	then goto COMPILE_TERMDEF
$	if p1.eqs."MACROS"	then goto COMPILE_MACROS
$	if p1.eqs."MORIA"	then goto COMPILE_MORIA
$	if p1.eqs.""		then goto COMPILE_MORIA
$!
$ BAD_PARAM:
$	write sys$output "Unrecognized parameter : ",p1
$	exit
$!
$ HELP:
$	type sys$input
BUILD.COM : 	Accepts one optional parameter.  By default, all steps are
		executed.  If parameter is used, only certain steps are
		executed.

	Parameters:	P1
			?	- display this help
				- Compile all source, re-link moria
			LINK	- re-link moria
			TERMDEF - compile termdef, re-link moria
			MACROS  - compile all macro routines, re-link moria
			MORIA	- compile moria & termdef, re-link moria

$	exit
$!
$ COMPILE_MORIA:
$	set def mor_source
$	delete moria.lis;*
$	delete moria.obj;*
$	delete moria.env;*
$	write sys$output "Compiling MORIA.PAS."
$	pascal moria.pas /nocheck/nodebug/nolist
$	write sys$output "MORIA.PAS compiled."
$	purge moria.*
$!
$ COMPILE_TERMDEF:
$	set def mor_source
$	delete termdef.lis;*
$	delete termdef.obj;*
$	write sys$output "Compiling TERMDEF.PAS."
$	pascal termdef /nocheck/nodebug/nolist
$	write sys$output "TERMDEF.PAS compiled."
$	if p1.nes."" then goto LINK_STEP
$!
$ COMPILE_MACROS:
$ 	set def mor_macro
$	delete morialib.olb;*
$	delete *.obj;*
$	write sys$output "Compiling MACROS."
$	macro bitpos/nodebug/nolist
$	write sys$output "BITPOS compiled."
$	macro distance/nodebug/nolist
$	write sys$output "DISTANCE compiled."
$	macro insert/nodebug/nolist
$	write sys$output "INSERT compiled."
$	macro maxmin/nodebug/nolist
$	write sys$output "MAXMIN compiled."
$	macro minmax/nodebug/nolist
$	write sys$output "MINMAX compiled."
$	macro putqio/nodebug/nolist
$	write sys$output "PUTQIO compiled."
$	macro randint/nodebug/nolist
$	write sys$output "RANDINT compiled."
$	macro randrep/nodebug/nolist
$	write sys$output "RANDREP compiled."
$	macro handler/nodebug/nolist
$	write sys$output "HANDLER Compiled."
$	library/create morialib.olb
$	library morialib bitpos
$	library morialib distance
$	library morialib insert
$	library morialib maxmin
$	library morialib minmax
$	library morialib putqio
$	library morialib randint
$	library morialib randrep
$	library morialib handler
$	write sys$output "MORIALIB.OLB built."
$	if p1.nes."" then goto LINK_STEP
$!
$ LINK_STEP:
$	set def mor_source
$	delete moria.map;*
$	write sys$output "Linking."
$	link /notrace/nodebug moria,termdef,mor_macro:morialib/library
$	write sys$output "MORIA linked."
$	rename moria.exe mor_execute:moria.exe
$	purge mor_execute:moria.exe
$       library /create/help mor_execute:moriahlp mor_execute:moriahlp
$	set def mor_main
$ 	exit
$!
$ ERROR_TRAP:
$	write sys$output "***Error resulted in termination***"
$	set def mor_main
$ exit
