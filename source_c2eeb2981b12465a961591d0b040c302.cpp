/** Тетрис v 0.0.2
 * Автор: Михаил Кормановский
 * Школа: Школа программистов
 * Год: 2017
 */
/* Комментарий для читателей моего кода. Код имеет следующие особенности:
 *  1. В процессе разработки код довольно быстро изменялся, и я не всегда успевал
 * комментировать его в достаточной степени.
 *  2. В начале своего пути я был исключительно веб-программистом, разрабатывал сайты (и продолжаю
 * это делать), в связи с этим в коде можно увидеть некоторые идеи из веба (функции show_footer(), show_page())
 *  3. Самой сложной частью для меня оказался поворот блока (функция _rotate_prepare()), алгоритм продумывался
 * на листочке бумаги, и в нем нет комментариев. При необходимости могу объяснить его.
*/
#include <iostream>
#include <fstream>
#include <conio.h>
#include <windows.h>
#include <clocale>
using namespace std;
//Возможно, будет использовано в будущих версиях
string VERSION_STRING = "0.0.2";
const double VERSION = 0.02;
//Поле (0 - пусто, 1-14 - занято)
int field[10][20] = {
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
};
//Цвет текущего блока
int current_block_color = 0;
//Тип текущего блока
int current_block_type = -1;
//Поворот текущего блока
int current_block_rotation = 0;
//Текущий блок. Блоки по 4 клетки
//Структура блока: {{x1, y1}, {x2, y2}, {x3, y3}, {x4, y4}}
int current_block[4][2];
//Очки
int score = 0;
//Количество убранных линий
int dropped_lines = 0;
//handle для смены цветов
HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
//Стартовый размер массива с результатами
const int START_RESULTS_SIZE = 100;
//Размер массива с результатами в данный момент
int results_size = 0;
//логины пользователей
string *logins = new string[START_RESULTS_SIZE];
//результаты пользователей
int *results = new int[START_RESULTS_SIZE];
//Имя текущего пользователя
string username;
//Рекорд текущего пользователя
int record = 0;
//Текущая страница
string current_page;
//Локализация
int current_locale = 0;
string l10n[2][16] = {
    {
        "Score: ",
        " PAUSED",
        "  Leaderboard",
        "  Place Name Score",
        "  Press \"P\" to Play.",
        "  Press \"T\" to go to the game title page.",
        "  Press \"H\" to go to \"Help\"",
        "  Press \"L\" to go to \"Leaderboard\"",
        "  Game Over!",
        "  v0.0.2",
        "  Author: Mikhail Kormanowsky",
        "  School: Moscow Programmers School",
        "\n\n  HELP\n\n  How to play\n\n  1. Press \"P\" when you are not on game page (when you don`t see the field and falling block).\n  2. Enter your name (it is used to store your records).\n  3. Play and use the following keys to interact with the game:\n    Press \"A\" to move left the current block.\n    Press \"S\" if you want the current block to fall down immediately.  \n    Press \"D\" to move right the current block.  \n    Press \"F\" to rotate left the current block.  \n    Press \"G\" to rotate right the current block.  \n    Press \"P\" to play/pause the game.  \n  4. When you see message \"Game over!\" follow the instructions you see on the screen.  \n  5. When tired close the window. Your records are saved!\n\n  How score is counted\n\n  The following algorithm is used to count your score:\n    1. The program finds L = 1 + 2 + ... + (N-1) + N where N is the count of lines you dropped  \n    2. The program multiplies L by 100 and adds to your score the result.  \n      Table of the count of points you get when you drop lines:  \n        1 - 100  \n        2 - 300  \n        3 - 600  \n        4 - 1000  \n        etc.",
        "  Please enter your name:",
        "\n\n\n  What`s new?\n\n",
        "  v0.0.2 (2017-09-05)\n\n  Added Russian language support\n\n  v0.0.1 (2017-08-30)\n\n  Initial release\n\n\n"


    },
    {
        "Счет: ",
        " ПАУЗА",
        "  Лидеры",
        "  Место Имя Счет",
        "  Нажмите \"P\", чтобы Играть.",
        "  Нажмите \"T\", чтобы перейти к заставке игры.",
        "  Нажмите \"H\", чтобы перейти в раздел \"Помощь\"",
        "  Нажмите \"L\", чтобы перейти в раздел \"Лидеры\"",
        "  Игра окончена!",
        "  Версия 0.0.2",
        "  Автор: Михаил Кормановский",
        "  Школа: Московская Школа Программистов",
        "\n\n  ПОМОЩЬ\n\n  Как играть\n\n  1. Нажмите \"P\" когда Вы находитесь не на странице игры (когда Вы не видите игровое поле и падающий блок).\n  2. Введите свое имя (оно будет использовано для хранения рекордов).\n  3. Играйте, используя следующие клавиши для взаимодействия с игрой:\n    Нажимайте \"A\", чтобы сместить текущий блок влево.\n    Нажимайте \"S\", чтобы текущий блок немедленно упал вниз.  \n    Нажимайте \"D\" , чтобы сместить текущий блок вправо.  \n    Нажимайте \"F\", чтобы повернуть текущий блок влево.  \n    Нажимайте \"G\", чтобы сместить текущий блок вправо.  \n    Нажимайте \"P\", чтобы приостановить/продолжить игру.  \n  4. Когда увидите сообщение \"Игра окончена!\", следуйте инструкциям на экране.  \n  5. Если Вы устали, просто закройте окно. Ваши рекорды сохранены!\n\n  Подсчет очков\n\n  Для подсчета очков используется следующий алгоритм:\n    1. Программа находит L = 1 + 2 + ... + (N-1) + N, где N - количество убранных линий.  \n    2. Программа умножает L на 100, полученное количество программа добавляет к Вашему счету.  \n      Таблица очков, заработанных за удаление линий:  \n        1 - 100  \n        2 - 300  \n        3 - 600  \n        4 - 1000  \n       и т.д.",
        "  Пожалуйста, введите имя:",
        "\n\n\n  Что нового?\n\n",
        "  Версия 0.0.2 (2017-09-05)\n\n  Добавлена поддержка русского языка\n\n  Версия 0.0.1 (2017-08-30)\n\n  Начальная версия\n\n\n"
    }
};
//Получение локализованной строки по ее id
string get_localized(int str_id){
    return l10n[current_locale][str_id];
}
//Сокращение для get_localized
string $(int str_id){
    return get_localized(str_id);
}

//Количество кадров
int animation_frames = 0;
//Получение случайного числа в интервале
int get_random_number(int start, int end){
    return (rand() % (end - start + 1)) + start;
}
//Получение случайного цвета (1-14, так как черный - цвет фона, а белый - цвет рамки)
int get_random_color(){
    return get_random_number(1, 14);
}
//Получение случайного типа блока (0-6)
int get_random_type(){
    return get_random_number(0, 6);
}
//Получение случайной начальной левой верхней точки
int get_random_start_point(){
    return get_random_number(0,6);
}
//Получение случайного поворота
int get_random_rotation(){
    return get_random_number(0, 3);
}
//Установка цвета фона консоли
void set_console_background_color(int color){
    SetConsoleTextAttribute(h, color * 16);
}
//Сброс фона консоли (черный)
void clear_console_background_color(){
    SetConsoleTextAttribute(h, 0x000F);
}
//Существует ли активный блок?
bool is_block_generated = false;
//Перемещали ли активный блок?
bool has_block_moved = false;
//Игра на паузе или нет?
bool paused = false;
//Стирание блока
void erase_block(){
    //Стираем
    for(int i = 0; i < 4; ++i){
        int x = current_block[i][0], y = current_block[i][1];
        field[x][y] = 0;
    }
}
//Запись блока на поле
void draw_block(){
    for(int i = 0; i < 4; ++i){
        int x = current_block[i][0], y = current_block[i][1];
            field[x][y] = current_block_color;

    }
}
//Новый блок
void generate_block(){
    //Получение случайного цвета нового блока
    current_block_color = get_random_color();
    //Получение случайного типа нового блока. Типы: https://ru.wikipedia.org/wiki/Тетрис#/media/File:Tetrominoes_IJLO_STZ_Worlds.svg
    current_block_type = get_random_type();
    //Левая верхняя точка нового блока
    int start_point = get_random_start_point();
    //Координаты нового блока
    switch(current_block_type){
        case 0:
            current_block[0][0] = start_point;
            current_block[1][0] = start_point + 1;
            current_block[2][0] = start_point + 2;
            current_block[3][0] = start_point + 3;
            current_block[0][1] = 0;
            current_block[1][1] = 0;
            current_block[2][1] = 0;
            current_block[3][1] = 0;
            break;
        case 1:
            current_block[0][0] = start_point;
            current_block[1][0] = start_point;
            current_block[2][0] = start_point + 1;
            current_block[3][0] = start_point + 2;
            current_block[0][1] = 0;
            current_block[1][1] = 1;
            current_block[2][1] = 1;
            current_block[3][1] = 1;
            break;
        case 2:
            current_block[0][0] = start_point;
            current_block[1][0] = start_point + 1;
            current_block[2][0] = start_point + 2;
            current_block[3][0] = start_point + 2;
            current_block[0][1] = 1;
            current_block[1][1] = 1;
            current_block[2][1] = 0;
            current_block[3][1] = 1;
            break;
        case 3:
            current_block[0][0] = start_point;
            current_block[1][0] = start_point;
            current_block[2][0] = start_point + 1;
            current_block[3][0] = start_point + 1;
            current_block[0][1] = 0;
            current_block[1][1] = 1;
            current_block[2][1] = 0;
            current_block[3][1] = 1;
            break;
        case 4:
            current_block[0][0] = start_point;
            current_block[1][0] = start_point + 1;
            current_block[2][0] = start_point + 1;
            current_block[3][0] = start_point + 2;
            current_block[0][1] = 1;
            current_block[1][1] = 0;
            current_block[2][1] = 1;
            current_block[3][1] = 0;
            break;
        case 5:
            current_block[0][0] = start_point;
            current_block[1][0] = start_point + 1;
            current_block[2][0] = start_point + 1;
            current_block[3][0] = start_point + 2;
            current_block[0][1] = 1;
            current_block[1][1] = 0;
            current_block[2][1] = 1;
            current_block[3][1] = 1;
            break;
        case 6:
            current_block[0][0] = start_point;
            current_block[1][0] = start_point + 1;
            current_block[2][0] = start_point + 1;
            current_block[3][0] = start_point + 2;
            current_block[0][1] = 0;
            current_block[1][1] = 0;
            current_block[2][1] = 1;
            current_block[3][1] = 1;
            break;
    }
    //Запись блока
    draw_block();
    //Изменение переменных
    is_block_generated = true;
    has_block_moved = false;
    current_block_rotation = 0;


}
//Клетка занята или нет
bool square_free(int x, int y){
    return !field[x][y] && (x > -1) && (x < 10) && (y > -1) && (y < 20);
}
//Является ли клетка частью текущего блока
bool square_part_of_current_block(int x, int y){
    for(int i = 0; i < 4; ++i){
        int* s = current_block[i];
        int X = s[0], Y = s[1];
        if(X == x && Y == y) return true;
    }
    return false;
}
//Можно ли перемещаться
bool can_move_current_block(string direction){
    int offsetX, offsetY;
    if(direction == "down"){
        offsetX = 0;
        offsetY = 1;
    }else if(direction == "left"){
        offsetX = -1;
        offsetY = 0;
    }else if(direction == "right"){
        offsetX = 1;
        offsetY = 0;
    }else{
        offsetX = 0;
        offsetY = 0;
    }
    for(int i = 0; i < 4; ++i){
        int x = current_block[i][0] + offsetX, y = current_block[i][1] + offsetY;
        if((!square_free(x, y) && !square_part_of_current_block(x, y))){
            return false;
        }
    }
    return true;
}
//Перемещение
void move_current_block(string direction){
    erase_block();
    int offsetX, offsetY;
    if(direction == "down"){
        offsetX = 0;
        offsetY = 1;
    }else if(direction == "left"){
        offsetX = -1;
        offsetY = 0;
    }else if(direction == "right"){
        offsetX = 1;
        offsetY = 0;
    }else{
        offsetX = 0;
        offsetY = 0;
    }
    //Стирание

    for(int i = 0; i < 4; ++i){
        current_block[i][0] += offsetX;
        current_block[i][1] += offsetY;
    }
    //Запись блока
    draw_block();
    //Говорим, что переместились
    has_block_moved = true;
}
//Вычисление координат поворота
void _rotate_prepare(string direction){
    //Тип O. Не поворачивается
    if(current_block_type == 3){
        return;
    //Тип I
    }else if(current_block_type == 0){
        int* center = current_block[2];
        for(int i = 0; i < 4; ++i){
            if(!(current_block_rotation%2)){
                current_block[i][1] += (center[0] - current_block[i][0]);
                current_block[i][0] = center[0];
            }else{
                current_block[i][0] += (center[1] - current_block[i][1]);
                current_block[i][1] = center[1];
            }
        }
    //Типы S и Z
    }else if(current_block_type == 4 || current_block_type == 6){
        //Для типов S и Z достаточно переместить 2 точки на 2 пункта
        int m = (current_block_type == 4)?-1:1;
        if(!(current_block_rotation%2)){
            current_block[3][0] -= 2;
            current_block[0][1] += m*2;

        }else{
            current_block[3][0] += 2;
            current_block[0][1] -= m*2;
        }
    //Тип T
    }else if(current_block_type == 5){
        int sx, sy;
        if(direction == "right"){
            if(current_block_rotation == 0){
                current_block[0][0]++;
                current_block[0][1]++;
            }else if(current_block_rotation == 1){
                current_block[0][0]--;
                current_block[0][1]++;
            }else if(current_block_rotation == 2){
                current_block[0][0]--;
                current_block[0][1]--;

            }else if(current_block_rotation == 3){
                current_block[0][0]++;
                current_block[0][1]--;
            }
            if(current_block_rotation%2){
                sx = current_block[3][0];
                sy = current_block[3][1];
                current_block[3][0] = current_block[0][0];
                current_block[3][1] = current_block[0][1];
                current_block[0][0] = sx;
                current_block[0][1] = sy;
            }else{
                sx = current_block[1][0];
                sy = current_block[1][1];
                current_block[1][0] = current_block[0][0];
                current_block[1][1] = current_block[0][1];
                current_block[0][0] = sx;
                current_block[0][1] = sy;
            }
        }else if(direction == "left"){
            if(!(current_block_rotation%2)){
                sx = current_block[3][0];
                sy = current_block[3][1];
                current_block[3][0] = current_block[0][0];
                current_block[3][1] = current_block[0][1];
                current_block[0][0] = sx;
                current_block[0][1] = sy;
            }else{
                sx = current_block[1][0];
                sy = current_block[1][1];
                current_block[1][0] = current_block[0][0];
                current_block[1][1] = current_block[0][1];
                current_block[0][0] = sx;
                current_block[0][1] = sy;
            }
            if(current_block_rotation == 0){
               current_block[0][0]--;
               current_block[0][1]++;
            }else if(current_block_rotation == 1){
                current_block[0][0]--;
                current_block[0][1]--;
            }else if(current_block_rotation == 2){
                current_block[0][0]++;
                current_block[0][1]--;

            }else if(current_block_rotation == 3){
                current_block[0][0]++;
                current_block[0][1]++;
            }

        }

    }
    //Типы J, L
    else if(current_block_type == 1 || current_block_type == 2){
        int fi = (current_block_type == 1)?0:2,si = (current_block_type == 1)?1:0;
        if(direction == "right"){
            if(current_block_rotation == 0){
                if(current_block_type == 1){
                    current_block[fi][0] += 2;
                }else{
                    current_block[fi][1] += 2;
                }
                current_block[si][0]++;
                current_block[si][1]--;
                current_block[3][0]--;
                current_block[3][1]++;
            }else if(current_block_rotation == 1){
                if(current_block_type == 1){
                    current_block[fi][1] += 2;
                }else{
                    current_block[fi][0] -= 2;
                }
                current_block[si][0]++;
                current_block[si][1]++;
                current_block[3][0]--;
                current_block[3][1]--;
            }else if(current_block_rotation == 2){
                if(current_block_type == 1){
                    current_block[fi][0] -= 2;
                }else{
                    current_block[fi][1] -= 2;
                }
                current_block[si][0]--;
                current_block[si][1]++;
                current_block[3][0]++;
                current_block[3][1]--;
            }else if(current_block_rotation == 3){
                if(current_block_type == 1){
                    current_block[fi][1] -= 2;
                }else{
                    current_block[fi][0] += 2;
                }
                current_block[si][0]--;
                current_block[si][1]--;
                current_block[3][0]++;
                current_block[3][1]++;
            }
        }else if(direction == "left"){
            if(current_block_rotation == 0){
                if(current_block_type == 1){
                    current_block[fi][1] += 2;
                }else{
                    current_block[fi][0] -= 2;
                }
                current_block[si][0]++;
                current_block[si][1]++;
                current_block[3][0]--;
                current_block[3][1]--;
            }else if(current_block_rotation == 1){
                if(current_block_type == 1){
                    current_block[fi][0] -= 2;
                }else{
                    current_block[fi][1] -= 2;
                }
                current_block[si][0]--;
                current_block[si][1]++;
                current_block[3][0]++;
                current_block[3][1]--;
            }else if(current_block_rotation == 2){
                if(current_block_type == 1){
                    current_block[fi][1] -= 2;
                }else{
                    current_block[fi][0] += 2;
                }
                current_block[si][0]--;
                current_block[si][1]--;
                current_block[3][0]++;
                current_block[3][1]++;
            }else if(current_block_rotation == 3){
                if(current_block_type == 1){
                    current_block[fi][0] += 2;
                }else{
                    current_block[fi][1] += 2;
                }
                current_block[si][0]++;
                current_block[si][1]--;
                current_block[3][0]--;
                current_block[3][1]++;
            }
        }
    }

}
//Можно ли поворачивать
bool can_rotate(string direction){
    int _block[4][2];
    for(int i = 0; i < 4; ++i){
        for(int j = 0; j < 2; ++j){
            _block[i][j] = current_block[i][j];
        }
    }
    _rotate_prepare(direction);
    bool r = true;
    for(int i = 0; i < 4; ++i){
        int x = current_block[i][0], y = current_block[i][1];
        if((!square_free(x, y) && !square_part_of_current_block(x, y))){
           r = false; break;
        }
    }
    for(int i = 0; i < 4; ++i){
        for(int j = 0; j < 2; ++j){
            current_block[i][j] = _block[i][j];
        }
    }
    return r;
}
//Поворот
void rotate_current_block(string direction){
    erase_block();
    _rotate_prepare(direction);
    //Изменение переменной поворота
    if(direction == "right"){
        if(current_block_rotation + 1 > 3){
            current_block_rotation = 0;
        }else{
            current_block_rotation += 1;
        }
    }else{
        if(current_block_rotation - 1 < 0){
            current_block_rotation = 3;
        }else{
            current_block_rotation -= 1;
        }
    }
    draw_block();
}
//Отрисовка состояния
void draw_state(){
    cout << endl << endl << $(0) << score;
    if(paused) cout << $(1);
    cout << endl << endl;
    set_console_background_color(15);
    for(int i = 0; i < 22; ++i){
        cout << " ";
    }
    clear_console_background_color();
    cout << endl;
    for(int i = 0; i < 20; ++i){
        for(int $ = 0; $ < 2; ++$){
        set_console_background_color(15);
        cout << " ";

        for(int j = 0; j < 10; ++j){
            set_console_background_color(field[j][i]);
            cout << "  ";
        }
        set_console_background_color(15);
        cout << " ";
        cout << endl;
        }
    }
    clear_console_background_color();
    set_console_background_color(15);
    for(int i = 0; i < 22; ++i){
        cout << " ";
    }
    clear_console_background_color();
    animation_frames++;

}
//Нажатия на клавиатуре
bool process_keyboard(){
    char c = 0;
    if (kbhit()) c = getch();
    //Лево
    if(c == 'a' && is_block_generated && can_move_current_block("left") && !paused){
        move_current_block("left");
        return true;
    }
    //Право
    else if(c == 'd' && is_block_generated && can_move_current_block("right") && !paused){
        move_current_block("right");
        return true;
    }
    //Упасть совсем
    else if(c == 's' && is_block_generated && !paused){
        while(can_move_current_block("down"))
            move_current_block("down");
        return true;
    }
    //Поворот влево
    else if(c == 'f' && is_block_generated && can_rotate("left")  && !paused){
        rotate_current_block("left");
        return true;
    }
    //Поворот вправо
    else if(c == 'g' && is_block_generated && can_rotate("right") && !paused){
        rotate_current_block("right");
        return true;
    }
    //Пауза-Играть
    else if(c == 'p'){
        paused = !paused;
        return true;

    }
    return false;
}
//Можем ли мы убрать строку
bool can_drop_line(){
    bool r = true;
    for(int i = 0; i < 10; ++i){
        if(!field[i][19]){
            r = false;
            break;
        }
    }
    return r;
}
//Убрать строку
void drop_line(){
    for(int i = 18; i > -1; --i){
        for(int j = 0; j < 10; ++j){
            field[j][i+1] = field[j][i];
        }
    }
    for(int j = 0; j < 10; ++j){
        field[j][0] = 0;
    }
    dropped_lines++;
}
//Нажатия на клавиатуре
string process_footer_keyboard(){
    char c = 0;
    if (kbhit()) c = getch();
    if(c == 'p' && current_page != "name"){
        return "name";
    }else if(c == 'h' && current_page != "help"){
        return "help";
    }else if(c == 't' && current_page != "title"){
        return "title";
    }else if(c == 'l' && current_page != "leaderboard"){
        return "leaderboard";
    }
    return "none";
}
//Расширить массив с пользователями
void expand_leaderboard(){
    results_size++;
    if(results_size < START_RESULTS_SIZE) return;
    string *t = new string[results_size];
    int *j = new int[results_size];
    for(int k = 0; k < results_size - 1; ++k){
        t[k] = logins[k];
        j[k] = results[k];
    }
    delete [] logins;
    delete [] results;
    logins = t;
    results = j;
}
//Удалить массив с пользователями
void clear_memory(){
    delete [] results;
    delete [] logins;
}
//Зарегистрирован ли пользователь
bool user_registered(){
    for(int i = 0; i < results_size; ++i){
        if(logins[i] == username) return true;
    }
    return false;
}
//Чтение leaderboard-а
void read_leaderboard(){
    fstream f("leaderboard.txt", ios::in);
    int now = 0, j;
    string t;
    while(f >> t >> j){
        if(now>=results_size) expand_leaderboard();
        logins[now] = t;
        results[now] = j;
        ++now;

    }
    f.close();

}
//Очистка leaderboard
void clean_leaderboard(){
    fstream f("leaderboard.txt", ios::out | ios::trunc);
    f.close();
}
//Запись leaderboard
void write_leaderboard(){
    fstream f("leaderboard.txt", ios::out);
    for(int j = 0; j < results_size; ++j){
        string u = logins[j];
        int k = results[j];
        f << u << " " << k << endl;
    }
    f.close();
}
//Окончание игры
void finish_game(){
    for(int i = 0; i < 10; ++i){
        for(int j = 0; j < 20; ++j){
            field[i][j] = 0;
        }
    }
    current_block_color = 0;
    current_block_type = -1;
    current_block_rotation = 0;
    for(int i = 0; i < 4; ++i){
        for(int j = 0; j < 2; ++j){
            current_block[i][j] = -100;
        }
    }
    dropped_lines = 0;
    is_block_generated = false;
    has_block_moved = false;
    paused = false;
    if(user_registered()){
        for(int i = 0; i < results_size; ++i){
            if(logins[i] == username && results[i] < score){
                results[i] = score;
                return;
            }
        }
    }else{
        expand_leaderboard();
        logins[results_size - 1] = username;
        results[results_size - 1] = score;
    }
    clean_leaderboard();
    write_leaderboard();
    username = "";
    score = 0;
}
//Сортировка пузырьком (сразу 2 массива - логины и результаты)
void bubble_sort_desc_leaderboard(){
    for(int k = 0; k < results_size; ++k){
         for(int i = 0; i < results_size - 1; ++i){
             if(results[i+1] > results[i]){
                 int temp = results[i];
                 string temp2 = logins[i];
                 results[i] = results[i+1];
                 results[i+1] = temp;
                 logins[i] = logins[i+1];
                 logins[i+1] = temp2;
             }
         }
    }
}
//Показ доски лидеров
void show_leaderboard(){
    read_leaderboard();
    bubble_sort_desc_leaderboard();
    int place = 1, max = results[0];
    cout << endl << endl << $(2) << endl << endl <<
            $(3) << endl << endl;
    for(int i = 0; i < results_size; ++i){
        if(results[i] < max){
            max = results[i];
            place++;
        }
        cout << "  " << place << " " << logins[i] << " " << results[i] << endl;
    }
}
//Показ футера
void show_footer(){
    cout  << endl << endl << $(4) << endl
          << $(5) << endl
          << $(6) << endl
          << $(7) << endl;
}
//Показ страницы
void show_page(string page){
    current_page = page;
    system("CLS");
    clear_console_background_color();
    if(page == "field"){
        while(true){
            if(paused){
                process_keyboard();
                continue;
            }
            //Задержка. Каждые 15 кадров задержка уменьшается на 10мс
            Sleep(500 - animation_frames / 15 * 10);
            //Очистка
            system("CLS");
            //Изменение поля
            //Если нет блока, генерируем его
            if(!is_block_generated){
                generate_block();
            }
            //Если не нажали клавиши
            bool kb = process_keyboard();
            //Рисование
            draw_state();
            if(!kb){
                //Почистить низ поля (убрать целые линии)
                if(can_drop_line()){
                    while(can_drop_line()){
                        drop_line();
                    }
                    int _score = 0;
                    for(int i = 1; i <= dropped_lines; ++i){
                        _score += i;
                    }
                    score += _score * 100;
                    dropped_lines = 0;
                }else
                //Если можно падать, падаем
                if(can_move_current_block("down")){
                    move_current_block("down");
                }else{
                    //Если падать нельзя, проверяем, если мы не можем падать и мы не падали, то игра окончена
                    if(!has_block_moved){
                        show_page("over");
                        break;
                    }
                    //Говорим программе в следующий проход создать новый блок
                    is_block_generated = false;
                }
            }
        }
    }else if(page == "over"){
        finish_game();
        cout << endl << $(8);
        show_footer();
        while(true){
            string newPage = process_footer_keyboard();
            if(newPage != "none"){
                show_page(newPage); break;
            }
        }
    }else if(page == "title"){
        cout << endl << endl;
        for(int i = 0; i < 2; ++i){
            cout << "  ";
            set_console_background_color(15);
            cout << "      ";
            clear_console_background_color();
            cout << "  ";
            set_console_background_color(15);
            cout << "      ";
            clear_console_background_color();
            cout << "  ";
            set_console_background_color(15);
            cout << "      ";
            clear_console_background_color();
            cout << "  ";
            set_console_background_color(15);
            cout << "    ";
            clear_console_background_color();
            cout << "    ";
            set_console_background_color(15);
            cout << "  ";
            clear_console_background_color();
            cout << "    ";
            set_console_background_color(15);
            cout << "    ";
            clear_console_background_color();
            cout << endl;
        }
        for(int i = 0; i < 2; ++i){
            cout << "    ";
            set_console_background_color(15);
            cout << "  ";
            clear_console_background_color();
            cout << "    ";
            set_console_background_color(15);
            cout << "  ";
            clear_console_background_color();
            cout << "        ";
            set_console_background_color(15);
            cout << "  ";
            clear_console_background_color();
            cout << "    ";
            set_console_background_color(15);
            cout << "  ";
            clear_console_background_color();
            cout << "  ";
            set_console_background_color(15);
            cout << "  ";
            clear_console_background_color();
            cout << "  ";
            set_console_background_color(15);
            cout << "  ";
            clear_console_background_color();
            cout << "  ";
            set_console_background_color(15);
            cout << "  ";
            clear_console_background_color();
            cout << "  ";
            cout << endl;
        }
        for(int i = 0; i < 2; ++i){
            cout << "    ";
            set_console_background_color(15);
            cout << "  ";
            clear_console_background_color();
            cout << "    ";
            set_console_background_color(15);
            cout << "      ";
            clear_console_background_color();
            cout << "    ";
            set_console_background_color(15);
            cout << "  ";
            clear_console_background_color();
            cout << "    ";
            set_console_background_color(15);
            cout << "    ";
            clear_console_background_color();
            cout << "    ";
            set_console_background_color(15);
            cout << "  ";
            clear_console_background_color();
            cout << "    ";
            set_console_background_color(15);
            cout << "  ";
            clear_console_background_color();
            cout << endl;
        }
        for(int i = 0; i < 2; ++i){
            cout << "    ";
            set_console_background_color(15);
            cout << "  ";
            clear_console_background_color();
            cout << "    ";
            set_console_background_color(15);
            cout << "  ";
            clear_console_background_color();
            cout << "        ";
            set_console_background_color(15);
            cout << "  ";
            clear_console_background_color();
            cout << "    ";
            set_console_background_color(15);
            cout << "  ";
            clear_console_background_color();
            cout << "  ";
            set_console_background_color(15);
            cout << "  ";
            clear_console_background_color();
            cout << "  ";
            set_console_background_color(15);
            cout << "  ";
            clear_console_background_color();
            cout << "      ";
            set_console_background_color(15);
            cout << "  ";
            clear_console_background_color();
            cout << endl;
        }
        for(int i = 0; i < 2; ++i){
            cout << "    ";
            set_console_background_color(15);
            cout << "  ";
            clear_console_background_color();
            cout << "    ";
            set_console_background_color(15);
            cout << "      ";
            clear_console_background_color();
            cout << "    ";
            set_console_background_color(15);
            cout << "  ";
            clear_console_background_color();
            cout << "    ";
            set_console_background_color(15);
            cout << "  ";
            clear_console_background_color();
            cout << "  ";
            set_console_background_color(15);
            cout << "  ";
            clear_console_background_color();
            cout << "  ";
            set_console_background_color(15);
            cout << "  ";
            clear_console_background_color();
            cout << "  ";
            set_console_background_color(15);
            cout << "    ";
            clear_console_background_color();
            cout << endl;
        }
        cout << endl <<
             $(9) << endl << $(10) << endl
             << $(11) << $(14) << $(15);
        show_footer();
        while(true){
            string newPage = process_footer_keyboard();
            if(newPage != "none"){
                show_page(newPage); break;
            }
        }
    }else if(page == "help"){
        cout << $(12);
             show_footer();
        while(true){
            string newPage = process_footer_keyboard();
            if(newPage != "none"){
                show_page(newPage); break;
            }
        }
    }else if(page == "name"){
        //Запросим имя
        cout << endl << endl << $(13) << endl << endl << "  ";
        cin >> username;
        show_page("field");
    }else if(page == "leaderboard"){
        show_leaderboard();
        show_footer();
        while(true){
            string newPage = process_footer_keyboard();
            if(newPage != "none"){
                show_page(newPage); break;
            }
        }
    }else if(page == "locale"){
        cout << "Please choose your locale (0 - English, 1 - Russian):" << endl;
        cin >> current_locale;
        switch(current_locale){
            case 1:

                setlocale(LC_CTYPE, "rus");
            break;
        }

        show_page("title");
    }else{
        return;
    }
}
//Запуск игры
void start(){
    //Покажем заставку
    show_page("locale");
    //По окончании очистим память
    clear_memory();
}
//main
int main()
{
    //Начинаем...
    start();
    return 0;
}
