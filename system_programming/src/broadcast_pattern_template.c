/*
	this code writes my implementation of a design pattern Observer / Broadcast 
	pattern via POSIX sync mechansims
	which are : mutex, semaphores, condition variabels.
*/

/* quick explanation of the design pattern */
/*
	(wikipidia) 
	- it's a design pattern, which an object (subject/ event source/ event stream)
	maintains a list of it's dependencies (observers) and automatically notify them 
	of any state changes.
	- one to many dependincies 
*/


