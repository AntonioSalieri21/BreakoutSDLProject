# BreakoutSDLProject
Breakout project in SDL

//Main info
Project contains two scenes: main level with difficulty settings and level to play
Level can be loaded from simple text file with name "level.txt" on same location as functions.c 

//Level construction info
Level.txt is created with 3 types of symbols:
    1,2,3,4,5 - represents blocks with corresponding HP and score of this block
    '#' - represents indestructible blocks
    ' ' - represents empty space
You can create as large levels as you want, but recommended maximum size is: 
    16 blocks in width 
    8 blocks in height

//Code info
Code is written in two files: main.c with start of program and functions.c with all functions. Main
variables are on the top of functions.c file so they can be used in all code without unnecessary arguments in functions.
Functions can be separated in four groups:
    Initialization functions (initialization of SDL, loading textures e.t.c)
    Update functions (ball physics, collision)
    Render functions (rendering of objects on scene)
    Additional functions

In header.h are written structes such as Brick or Player, included all needed libraries and declared functions

Collision of ball with paddle:
    1. Get angle of collision
    2. Adding 90 degrees
    3. Normalizing it
    4. Adding to direction of ball
    5. Move it up

Collision of ball with bricks:
    1. Get intersection of ball in brick
    2. Based on it decide whether ball is under of brick or from side
    3. Move ball
    4. Change direction



