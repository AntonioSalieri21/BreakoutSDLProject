all:
	gcc main.c functions.c -o Breakout -lSDL2 -lSDL2_mixer -lSDL2_image -lSDL2_ttf -lm -L/usr/lib/x86_64-linux-gnu -fsanitize=address
