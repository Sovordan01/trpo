#include <curses.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

char map[30][120];
int x;
int y;

int attack = 2;
int hp = 60;
int max_hp = 60;
int count_move = 0;

bool monstro_spawn = FALSE;
bool player_spawn = FALSE;
bool trap_spawn = FALSE;

bool heal_potion = FALSE;
bool sword_item= FALSE;

int room_create;

int dlvl = 1;

struct monsters{
    int x;
    int y;
    int hp;
    int type;
    bool awaken;
};
struct monsters monster[10];

class MapGenerator{

public:

    int cols;
    int rows;

    int dungeon_gen(int rows, int cols){


    if (!room_create)
    {
        int room_y, room_x;
        int room_size_x, room_size_y;
        int r_center_y, r_center_x;
        int r_old_center_y, r_old_center_x;
        int room_num = rand() % 8 + 5;

        bool collisian;

        for (int wall = 0; wall <= rows; wall++)
        {
            for (int floor = 0; floor <= cols; floor++)
            {
                if (wall == 0 || wall == rows - 1 || wall == 1 || floor == 0 || floor == cols)
                {
                    map[wall][floor] = '%';
                }
                else
                {
                    map[wall][floor] = '#';
                }
            }
        }


        while(room_create < room_num)
        {
            int troom_y_counter = 0;
            do
            {
                collisian = 0;

                room_y = rand() % (rows - 3) + 1;
                room_x = rand() % (cols - 3) + 1;

                room_size_x = rand() % 7 + 6;
                room_size_y = rand() % 7 + 6;

                troom_y_counter++;

                if(troom_y_counter > 100)
                {
                    room_y = 3;
                    room_x = 3;
                    room_size_x = 3;
                    room_size_y = 3;
                    break;
                }

                for(int wall = room_y; wall <= room_y + room_size_x; wall++)
                {
                    for(int floor = room_x; floor <= room_x + room_size_y; floor++)
                    {
                        if (map[wall][floor] == '%' || map[wall][floor] == ' ' || map[wall-2][floor] == ' ' || map[wall][floor-2] == ' ' || map[wall+2][floor] == ' ' || map[wall][floor+2] == ' ')
                        {
                            collisian = 1;
                            wall = room_y + room_size_x + 1;
                            break;
                        }
                    }
                }
            }
            while (collisian == 1);

            for(int wall = room_y; wall <= room_y + room_size_x; wall++)
            {
                for(int floor = room_x; floor <= room_x + room_size_y; floor++)
                {
                    if (map[wall][floor] == '%')
                    {
                        wall = room_y + room_size_x + 1;
                        break;
                    }
                    else
                    {
                        map[wall][floor] = ' ';
                    }
                }
            }

            room_create++;

            if(room_create > 1)
            {
                r_old_center_y = r_center_y;
                r_old_center_x = r_center_x;
            }

            r_center_y = room_y + (room_size_x / 2);
            r_center_x = room_x + (room_size_y / 2);

            if(room_create > 1)
            {
                int path_y;

                for (path_y = r_old_center_y; path_y != r_center_y; )
                {
                    if(map[path_y][r_old_center_x != '%'])
                    {
                        map[path_y][r_old_center_x] = ' ';
                    }

                    if(r_old_center_y < r_center_y)
                    {
                        path_y++;
                    }
                    else if(r_old_center_y > r_center_y)
                    {
                        path_y--;
                    }
                }

                for (int path_x = r_old_center_x; path_x != r_center_x; )
                {
                    if (map[path_y][path_x] != '%')
                    {
                        map[path_y][path_x] = ' ';
                    }

                    if (r_old_center_x < r_center_x)
                    {
                        path_x++;
                    }
                    else if (r_old_center_x > r_center_x)
                    {
                         path_x--;
                    }
                }
            }
        }

        int Sy, Sx;

        do
        {
            Sy = rand() % (rows - 3) + 1;
            Sx = rand() % (cols - 3) + 1;
        }
        while(map[Sy][Sx] != ' ');

        map[Sy][Sx] = '>';
    }
    return 0;
}

};
class ItemsGetting{
public:

    int items_getting(int rows, int cols)
    {
        if(map[y][x] == 'H')
        {
            hp = max_hp;
            map[y][x] = ' ';
            heal_potion = FALSE;
        }
        if(map[y][x] == 'S')
        {
            attack *=2;
            map[y][x] = ' ';
            sword_item = TRUE;
        }
        return 0;
    }
};
class TrapDmg{
public:
    int trap_dmg(int rows, int cols)
    {
        if (map[y][x] == '^')
        {
            hp -= rand() % dlvl + 2;
        }
        return 0;
    }
};
class Intro{
public:

    int intro_ui(void)
    {
        int c = 0;

        printw("\t\t\t\t\t\t RogueLike\n\n"
        "\t How to play:\n"
        "\t ---------------------------------------------------------------\n"
        "\t You can move or attack on wasd(WASD).\n"
        "\t If you want to the next level, stand on > and press f.\n\n\n"

        "\t \Legend:\n"
        "\t ---------------------------------------------------------------\n"
        "\t ABCDE... - mobs. The older the letter, the more damage and HP.\n"
        "\t h - heal_potion\n"
        "\t s - Sword. It gives you an attack buff for the rest of the run.\n"
        "\t ^ - Traps. If you step on them you will take damage.\n");

        c = getch();
        clear();

        return 0;
    }
};
class MonstersTurn{
public:
    int monster_turn(int rows, int cols)
    {
        int dist_x;
        int dist_y;

        for(int m = 0; m < 15; m++)
        {
            if(monster[m].hp < 1)
            {
                continue;
            }

            dist_x = abs(monster[m].x - x);
            dist_y = abs(monster[m].y - y);

            if(dist_x < 5 && dist_y < 5)
            {
                monster[m].awaken = 1;
            }

            if(monster[m].awaken == 0)
            {
                continue;
            }

            int dir_x = monster[m].x;
            int dir_y = monster[m].y;


            if(dist_x > dist_y)
            {

                if(dir_x < x)
                {
                    dir_x++;
                }
                else
                {
                    dir_x--;
                }
            }
            else
            {
                if(dir_y < y)
                {
                    dir_y++;
                }
                else
                {
                    dir_y--;
                }
            }

            if(map[dir_y][dir_x] == '#' || map[dir_y][dir_x] == '%')
            {
                dir_x = monster[m].x;
                dir_y = monster[m].y;

                if(dir_y < y)
                {
                    dir_y++;
                }
                else
                {
                    dir_y--;
                }

                if(dir_x < x)
                {
                    dir_x++;
                }
                else
                {
                    dir_x--;
                }
            }

            if(dist_y < 2 && dist_x < 2)
            {
               hp -= (rand() % 2) + monster[m].type - 64;
            }
            else if(map[dir_y][dir_x] == ' ')
            {
                map[monster[m].y][monster[m].x] = ' ';
                monster[m].y = dir_y;
                monster[m].x = dir_x;
                map[monster[m].y][monster[m].x] = 'E';
            }
        }
    }
};
class Battle{
public:

    int rows;
    int cols;
    int dir_y;
    int dir_x;

    int battle(int rows, int cols, int dir_y, int dir_x)
    {

        for(int m = 0; m < 15; m++)
        {
            if(dir_y == monster[m].y && dir_x == monster[m].x)
            {
                if (monster[m].hp <= 0)
                {
                    map[dir_y][dir_x] = ' ';
                }
                else
                {
                    monster[m].hp -=attack;
                }
                break;
            }
        }
        return 0;
    }
};
class Action{
public:
    int action(int c, int rows, int cols)
    {
        Battle Battle;
        int dir_y = y;
        int dir_x = x;

        if(c == 'w' || c == 'W')
        {
            dir_y--;
            count_move++;
        }
        else if (c == 'a' || c == 'A')
        {
            dir_x--;
            count_move++;
        }
        else if (c == 's' || c == 'S')
        {
            dir_y++;
            count_move++;
        }
        else if (c == 'd'  || c == 'D')
        {
            dir_x++;
            count_move++;
        }
        else if ((c == 'f'  || c == 'F') && map[y][x] == '>')
        {
            monstro_spawn = 0;
            player_spawn = 0;
            room_create = 0;
            heal_potion = 0;
            sword_item = 0;

            return 1;
        }

        if(map[dir_y][dir_x] == ' ' || map[dir_y][dir_x] == '>' || map[dir_y][dir_x] == '^' || map[dir_y][dir_x] == 'H' || map[dir_y][dir_x] == 'S')
        {
            y = dir_y;
            x = dir_x;
        }
        else if(map[dir_y][dir_x] == 'E')
        {
            Battle.battle(rows, cols, dir_y, dir_x);
        }

        return 0;
    }
};
class MapDraw{
public:
    int rows;
    int cols;
    int dungeon_draw(int rows, int cols)
    {


    for (int wall = 0; wall <= rows; wall++)
    {
        for (int floor = 0; floor <= cols; floor++)
        {
        if (wall == 0 || wall == rows){
            mvaddch(wall, floor,' ');
        }
        else if (map[wall][floor] == 'H' && ((wall > y - 5 && floor > x - 5) && (wall < y + 5 && floor < x + 5)))
        {
                mvaddch(wall, floor, 'H');
        }
        else if (map[wall][floor] == 'S' && ((wall > y - 5 && floor > x - 5) && (wall < y + 5 && floor < x + 5)))
        {
                mvaddch(wall, floor, 'S');
        }
        else if (map[wall][floor] == '>' && ((wall > y - 5 && floor > x - 5) && (wall < y + 5 && floor < x + 5)))
        {
                mvaddch(wall, floor, '>');
        }
        else if (map[wall][floor] == '^' && ((wall > y - 5 && floor > x - 5) && (wall < y + 5 && floor < x + 5)))
        {
                mvaddch(wall, floor, '^');
        }
        else if (map[wall][floor] == '%')
        {
                mvaddch(wall, floor,'%');
        }
        else if (map[wall][floor] == ' ')
        {
                mvaddch(wall, floor,' ');
        }
        else if (map[wall][floor] == '#')
        {
                mvaddch(wall, floor, '#');
        }
        else if (map[wall][floor] == 'E' && ((wall > y - 5 && floor > x - 5) && (wall < y + 5 && floor < x + 5) && (wall != '#' & floor != '#')))
        {
           for(int m = 0; m < 10; m++)
            {
                if(monster[m].y == wall && monster[m].x == floor)
                {
                    if(monster[m].hp < dlvl / 2 + 2)
                    {
                            mvaddch(wall, floor, monster[m].type);
                    }
                    else if(monster[m].hp < dlvl + 2)
                    {
                            mvaddch(wall, floor, monster[m].type);
                    }
                    else
                    {
                            mvaddch(wall, floor, monster[m].type);
                    }
                }
                standend();
            }
        }
        }
    }
    mvprintw(0, 0, "HP: %d  / %d \t Attack: %d \t lvl: %d", hp, max_hp, attack, dlvl - 1);

    return 0;
    }
};
class Spawn{

public:
    int rows;
    int cols;

    int respawn(int rows, int cols)
    {
         if(!monstro_spawn)
        {
            int monstro_y;
            int monstro_x;

            for(int m = 0; m < 10; m++)
            {
                do
                {
                    monstro_y = rand() % rows;
                    monstro_x = rand() % cols;

                }
                while(map[monstro_y][monstro_x] != ' ');

                monster[m].y = monstro_y;
                monster[m].x = monstro_x;
                monster[m].hp = rand() % dlvl + 2;

                if (dlvl == 1 && !(rand() % 4))
                {
                    monster[m].hp = 1;
                }

                if (rand() % 3)
                {
                    monster[m].hp = dlvl + 2;
                }

                monster[m].type = rand() % dlvl + 65;
                monster[m].awaken = 0;
                map[monster[m].y][monster[m].x] = 'E';
            }
            monstro_spawn = TRUE;
        }
         if(!trap_spawn)
        {
            int Trap_y;
            int Trap_x;

            if(count_move % 10 == 0 && count_move != 0 && map[y][x+1] != '#' && map[y][x-1] != '#' && map[y-1][x] != '#' && map[y+1][x] != '#')
            {
                for(int m = 0; m < 2; m++)
                {
                    do
                    {
                        Trap_y = rand() % rows;
                        Trap_x = rand() % cols;

                    }
                    while(map[Trap_y][Trap_x] != ' ');
                    map[Trap_y][Trap_x] = '^';
                }
            }
        }
        if (!player_spawn)
        {
            do
            {
                y = rand() % rows;
                x = rand() % cols;

            }
            while(map[y][x] == '#' || map[y][x] == '%');
            player_spawn = TRUE;
        }
        if(!heal_potion)
        {
            int heal_y, heal_x;

            do
            {
                heal_y = rand() % rows;
                heal_x = rand() % cols;

            }
            while(map[heal_y][heal_x] == '#' || map[heal_y][heal_x] == '%');

            map[heal_y][heal_x] = 'H';
            heal_potion = TRUE;

        }
        if(!sword_item)
        {
            int sword_y, sword_x;

            do
            {
                sword_y = rand() % rows;
                sword_x = rand() % cols;

            }
            while(map[sword_y][sword_x] == '#' || map[sword_y][sword_x] == '%');
            map[sword_y][sword_x] = 'S';
            sword_item = 1;
        }
        return 0;
    }
};
class GameLoop{
public:
    int game_loop(int c, int rows, int cols)
    {
        int new_lvl;


        MapGenerator MapGenerator;
        ItemsGetting ItemsGetting;
        TrapDmg TrapDmg;
        MonstersTurn MonstersTurn;
        Action Action;
        MapDraw MapDraw;
        Spawn Spawn;

        srand(time(NULL));

        MapGenerator.dungeon_gen(rows, cols);

        Spawn.respawn(rows, cols);


        if(c !=0)
        {
          new_lvl = Action.action(c, rows, cols);
        }

        MonstersTurn.monster_turn(rows, cols);
        TrapDmg.trap_dmg(rows, cols);
        ItemsGetting.items_getting(rows, cols);
        MapDraw.dungeon_draw(rows, cols);


        mvaddch(y,x, '@');

        if (new_lvl)
        {
            clear();
            mvprintw(15, 37, "Welcome to next level: %d. Press any key to continue", dlvl++);
            getch();
            clear();
        }

        if(hp < 1)
        {
            clear();
            mvprintw(12, 50, "GAME OVER");
        }


        c = getch();

        return c;
    }
};

int main(){
    int c;

    int cols;
    int rows;

    GameLoop GameLoop;
    Intro Intro;

	initscr();

	keypad(stdscr, TRUE);
	noecho();
	curs_set(0);

	getmaxyx(stdscr, rows, cols);

    c = Intro.intro_ui();

    while (TRUE)
    {
        c = GameLoop.game_loop(c, rows - 1, cols - 1);
    }
    endwin();
}
