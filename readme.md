Jemulator is a work in progress gameboy emulator with cycle accurate instruction timing. Built for linux.
After execution, produces "Dcompile.txt" which contains each instruction in order that the program emulates and
the status of the cpu before every instruction. The file gets very large.

Current status:
	Hangs on Tetris 

What works:
	Joypad
	MMU
	Timers
	LCD Backgrounds
	
What needs to be worked on:
	Instructions
	LCD Sprites
	Heap management
	
Goal:
	Run Tetris
	
Dependencies:
	SDL2
	
Build Instructions:
	1. Clone repo locally
	2. Run "make"
	3. run "./game <Rom>.gb"
	4. ???
	5. Profit
