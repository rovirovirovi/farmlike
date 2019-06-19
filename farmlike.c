#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>

int playerHP = 20;
int playerX, playerY;
int width = 60, height = 30;
int scrW, scrH;
int targetX = 0, targetY = 1;
int ch;

int day = 1;

int food = 0;
int foodSeeds = 0;
int bombs = 0;
int bombSeeds = 0;

int randomRange(int min, int max);

void drawPlayer(int map[width][height]);
void drawMenu(int x, int height, int[width][height]);
void playerInput(int chr, int map[width][height]);
void passDay(int map[width][height]);

int main(){
    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();
    curs_set(0); // cursor invisible


    getmaxyx(stdscr, scrH, scrW);
    
    int map[width][height]; 

    for(int x = 0; x < width; x++)
        for(int y = 0; y < height; y++)
            if(randomRange(0,100) > 5)
                map[x][y] = 0;
            else
                map[x][y] = randomRange(10, 29); // food plant

    map[width / 2][height / 2] = 0;
    playerX = width / 2;
    playerY = height / 2;


    while(1){
        clear();
        //draw the map
        for(int x = 0; x < width; x++){
            for(int y = 0; y < height; y++){
                if(map[x][y] == 0)
                {
                    //do nothing lol
                }
                else if(map[x][y]/10 == 1){
                    if(map[x][y] % 10 <= 3) // food plant
                        mvaddch(y, x, 'x');
                    else if(map[x][y] % 10 <= 8) // food plant
                        mvaddch(y, x, 'l');
                    else if(map[x][y] % 10 <= 9) // food plant
                        mvaddch(y, x, 'f');
                }
                else if(map[x][y]/10 == 2){
                    if(map[x][y] % 10 <= 3) // bomb plant
                        mvaddch(y, x, 'x');
                    else if(map[x][y] % 10 <= 8) // bomb plant
                        mvaddch(y, x, 'o');
                    else if(map[x][y] % 10 <= 9) // bomb plant
                        mvaddch(y, x, 'O');
                }
                
                
            }
        }
        
        drawPlayer(map);
        drawMenu(width, height, map);

        ch = getch();
        if(ch == 'q')
            break;
        else
            playerInput(ch, map);

        refresh();
    }

    refresh();
    endwin();
    return 0;
}

void playerInput(int chr, int map[width][height]){
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
    if(chr == 'd'){
        passDay(map);
    }
    else if(chr == 'e'){
        if(map[playerX+targetX][playerY+targetY] > 0){
            if(map[playerX+targetX][playerY+targetY] % 10 == 9){
                if(map[playerX+targetX][playerY+targetY] / 10 == 1){
                    food++;
                    foodSeeds += randomRange(1,3);
                    map[playerX+targetX][playerY+targetY] = 0;
                }
                else if(map[playerX+targetX][playerY+targetY] / 10 == 2){
                    bombs++;
                    bombSeeds += randomRange(1,3);
                    map[playerX+targetX][playerY+targetY] = 0;
                }
            }   
        }
    }
    else if(chr == 'f'){
        if(map[playerX+targetX][playerY+targetY] / 10 == 0 && foodSeeds > 0){
            foodSeeds --;
            map[playerX+targetX][playerY+targetY] = 10;
        }
    }
    else if(chr == 'b'){
        if(map[playerX+targetX][playerY+targetY] / 10 == 0 && bombSeeds > 0){
            bombSeeds --;
            map[playerX+targetX][playerY+targetY] = 20;
        }
    }
}

void drawPlayer(int map[width][height]){
    mvaddch(playerY, playerX, '@');
    
    if(map[playerX + targetX][playerY + targetY] / 10 >= 1){
        mvaddch(playerY + targetY, playerX + targetX, 'X');
    }
    else
    {
        mvaddch(playerY + targetY, playerX + targetX, 'V');
    }
    
}

void drawMenu(int x, int height, int map[width][height]){
    for(int y = 0; y < height; y++){
        mvaddch(y, x, (char)186);
    }
    
    mvprintw(0, x+3, "DAY: %d", day);
    mvprintw(2, x + 3, "HP: %d", playerHP);
    

    if(map[playerX + targetX][playerY + targetY] / 10 >= 1){
        if(map[playerX + targetX][playerY + targetY] / 10 == 1)
            mvprintw(7, x + 3, "TARGET: food plant");
        else if(map[playerX + targetX][playerY + targetY] / 10 == 2)
            mvprintw(7, x + 3, "TARGET: bomb plant");

        mvprintw(8, x+3, "GROWTH: %d", map[playerX+targetX][playerY+targetY]%10);
        if(map[playerX+targetX][playerY+targetY] % 10 == 9)
            mvprintw(9, x+3, "[E] to HARVEST");
    }
    else{
        mvprintw(7, x+3, "TARGET: empty");
    }

    mvprintw(11, x+3, "FOOD: %d", food);
    mvprintw(12, x + 3, "FOOD SEEDS: %d", foodSeeds);
    mvprintw(13, x+3, "BOMBS: %d", bombs);
    mvprintw(14, x + 3, "BOMB SEEDS: %d", bombSeeds);   

    if(map[playerX+targetX][playerY+targetY] / 10 == 0 && foodSeeds > 0)
        mvprintw(16, x+3, "[F] to PLANT FOOD");
    if(map[playerX+targetX][playerY+targetY] / 10 == 0 && bombSeeds > 0)
        mvprintw(17, x+3, "[B] to BOMB FOOD");
    
}

void passDay(int map[width][height]){
    day++;
    for(int x = 0; x < width; x++){
        for(int y = 0; y < height; y++){
            if(map[x][y] / 10 >= 1 && map[x][y] % 10 < 9){
                map[x][y] ++;
            }
        }
    }
}

int randomRange(int min, int max){
    return (rand() % (max - min + 1)) + min;
}