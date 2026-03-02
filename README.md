# RTOS-based-Paratrooper-Game

This is a take on an old paratrooper game where paratroopers are falling at different speeds and we have to catch them on a boat

My implementation:
- 16x8 LED matrix which is the game board
- 4 LEDs on the bottom row representing the boat
- 8 paratroopers falling at different frequencies
- Upon 3 misses, the game is over and the message is displayed on the terminal

What makes it different? Well, the answer, no interrupts or delays are used. It is a purely RTOS based Embedded App
- All functionalities are different tasks, scheduled to work in unison
- No polling in the code
- PIC C Compiler and its in built RTOS is used

Board Used: EasyPic connectivity board - MikroElektronika
Language Used: C
16x8 LED Matrix with a decoder connected to PORTB of the board


<video src="Paratrooper Game Demo" controls width="500"></video>
