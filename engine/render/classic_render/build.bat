gcc -std=c17 main.c src/*.c -O3 -I SDL2/include -L SDL2/lib -lmingw32 -lSDL2main -lSDL2 -o bin/main.exe