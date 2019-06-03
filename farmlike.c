#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>

int playerHP = 20;
int playerX, playerY;
int width = 60, height = 30;
int ch;

int randomRange(int min, int max);

void drawPlayer();
void drawMenu(int x, int height);
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
                map[x][y] = 1;

    map[width / 2][height / 2] = 0;
    playerX = width / 2;
    playerY = height / 2;


    

    while(1){
        clear();
        //draw the map
        for(int x = 0; x < width; x++){
            for(int y = 0; y < height; y++){
                if(map[x][y] == 1) // wall
                    mvaddch(y, x, (char)178);
            }
        }
        
        drawPlayer();
        drawMenu(width, height);

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
        if(playerY > 0)
            playerY --;
        break;
    case KEY_DOWN:
        if(playerY < height - 1)
            playerY++;
            break;
    case KEY_LEFT:
        if(playerX > 0)
            playerX--;
            break;
    case KEY_RIGHT:
        if(playerX < width - 1)
            playerX++;
            break;
    
    default:
        break;
    }
}

void drawPlayer(){
    mvaddch(playerY, playerX, '@');
}

void drawMenu(int x, int height){
    for(int y = 0; y < height; y++){
        mvaddch(y, x, (char)186);
    }

    mvprintw(1, x + 3, "HP: %d", playerHP);
}

int randomRange(int min, int max){
    return (rand() % (max - min + 1)) + min;
}