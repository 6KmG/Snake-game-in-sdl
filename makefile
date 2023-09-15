all:
	g++ -g snake_game.cpp -Iinclude -Llib -Wall -lmingw32 -lSDL2main -lSDL2 -o snake_game_debug; snake_game_debug
	# g++ snake_game.cpp -s -mwindows -Iinclude -Llib -Wall -lmingw32 -lSDL2main -lSDL2 -o snake_game; snake_game