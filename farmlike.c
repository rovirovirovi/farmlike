#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>

int playerHP = 20;
int playerX, playerY;
int width = 60, height = 30;
int targetX = 0, targetY = 1;
int ch;

int randomRange(int min, int max);

void drawPlayer(int map[width][height]);
void drawMenu(int x, int height, int[width][height]);
void playerInput(int chr);

int main(){
    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();
    curs_set(0); // cursor invisible

    
    int map[width][height]; 

    for(int x = 0; x < width; x++)
        for(int y = 0; y < height; y++)
            if(randomRange(0,100) > 5)
                map[x][y] = 0;
            else
                map[x][y] = 110; // food plant

    map[width / 2][height / 2] = 0;
    playerX = width / 2;
    playerY = height / 2;


    

    while(1){
        clear();
        //draw the map
        for(int x = 0; x < width; x++){
            for(int y = 0; y < height; y++){
                if(map[x][y] / 100 == 1) // food plant
                    mvaddch(y, x, 'f');
            }
        }
        
        drawPlayer(map);
        drawMenu(width, height, map);

        ch = getch();
        if(ch == 'q')
            break;
        else
            playerInput(ch);

        refresh();
    }

    refresh();
    endwin();
    return 0;
}

void playerInput(int chr){
    switch (chr)
    {
    case KEY_UP:
        if(playerY > 0){
            playerY --;
            targetY = -1;
            targetX = 0;
        }
        break;
    case KEY_DOWN:
        if(playerY < height - 1){
            playerY++;
            targetY = 1;
            targetX = 0;
        }
        break;
    case KEY_LEFT:
        if(playerX > 0){
            playerX--;
            targetX = -1;
            targetY = 0;
        }
        break;
    case KEY_RIGHT:
        if(playerX < width - 1){
            playerX++;
            targetX = 1;
            targetY = 0;
        }
        break;
    
    default:
        break;
    }
}

void drawPlayer(int map[width][height]){
    mvaddch(playerY, playerX, '@');
    if(map[playerX + targetX][playerY + targetY] == 110){
        mvaddch(playerY + targetY, playerX + targetX, 'v');
    }
    else
    {
        mvaddch(playerY + targetY, playerX + targetX, 'O');
    }
    
}

void drawMenu(int x, int height, int map[width][height]){
    for(int y = 0; y < height; y++){
        mvaddch(y, x, (char)186);
    }

    mvprintw(1, x + 3, "HP: %d", playerHP);
    mvprintw(3, x + 3, "HEAL SEEDS: %d", 7);
    mvprintw(4, x + 3, "BOMB SEEDS: %d", 13);

    if(map[playerX + targetX][playerY + targetY] == 110){

    }
}

int randomRange(int min, int max){
    return (rand() % (max - min + 1)) + min;
}