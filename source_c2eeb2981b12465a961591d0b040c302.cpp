/** Òåòðèñ v 0.0.2
 * Àâòîð: Ìèõàèë Êîðìàíîâñêèé
 * Øêîëà: Øêîëà ïðîãðàììèñòîâ
 * Ãîä: 2017
 */
/* Êîììåíòàðèé äëÿ ÷èòàòåëåé ìîåãî êîäà. Êîä èìååò ñëåäóþùèå îñîáåííîñòè:
 *  1. Â ïðîöåññå ðàçðàáîòêè êîä äîâîëüíî áûñòðî èçìåíÿëñÿ, è ÿ íå âñåãäà óñïåâàë
 * êîììåíòèðîâàòü åãî â äîñòàòî÷íîé ñòåïåíè.
 *  2. Â íà÷àëå ñâîåãî ïóòè ÿ áûë èñêëþ÷èòåëüíî âåá-ïðîãðàììèñòîì, ðàçðàáàòûâàë ñàéòû (è ïðîäîëæàþ
 * ýòî äåëàòü), â ñâÿçè ñ ýòèì â êîäå ìîæíî óâèäåòü íåêîòîðûå èäåè èç âåáà (ôóíêöèè show_footer(), show_page())
 *  3. Ñàìîé ñëîæíîé ÷àñòüþ äëÿ ìåíÿ îêàçàëñÿ ïîâîðîò áëîêà (ôóíêöèÿ _rotate_prepare()), àëãîðèòì ïðîäóìûâàëñÿ
 * íà ëèñòî÷êå áóìàãè, è â íåì íåò êîììåíòàðèåâ. Ïðè íåîáõîäèìîñòè ìîãó îáúÿñíèòü åãî.
*/
#include <iostream>
#include <fstream>
#include <conio.h>
#include <windows.h>
#include <clocale>
using namespace std;
//Âîçìîæíî, áóäåò èñïîëüçîâàíî â áóäóùèõ âåðñèÿõ
string VERSION_STRING = "0.0.2";
const double VERSION = 0.02;
//Ïîëå (0 - ïóñòî, 1-14 - çàíÿòî)
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
//Öâåò òåêóùåãî áëîêà
int current_block_color = 0;
//Òèï òåêóùåãî áëîêà
int current_block_type = -1;
//Ïîâîðîò òåêóùåãî áëîêà
int current_block_rotation = 0;
//Òåêóùèé áëîê. Áëîêè ïî 4 êëåòêè
//Ñòðóêòóðà áëîêà: {{x1, y1}, {x2, y2}, {x3, y3}, {x4, y4}}
int current_block[4][2];
//Î÷êè
int score = 0;
//Êîëè÷åñòâî óáðàííûõ ëèíèé
int dropped_lines = 0;
//handle äëÿ ñìåíû öâåòîâ
HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
//Ñòàðòîâûé ðàçìåð ìàññèâà ñ ðåçóëüòàòàìè
const int START_RESULTS_SIZE = 100;
//Ðàçìåð ìàññèâà ñ ðåçóëüòàòàìè â äàííûé ìîìåíò
int results_size = 0;
//ëîãèíû ïîëüçîâàòåëåé
string *logins = new string[START_RESULTS_SIZE];
//ðåçóëüòàòû ïîëüçîâàòåëåé
int *results = new int[START_RESULTS_SIZE];
//Èìÿ òåêóùåãî ïîëüçîâàòåëÿ
string username;
//Ðåêîðä òåêóùåãî ïîëüçîâàòåëÿ
int record = 0;
//Òåêóùàÿ ñòðàíèöà
string current_page;
//Ëîêàëèçàöèÿ
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
        "Ñ÷åò: ",
        " ÏÀÓÇÀ",
        "  Ëèäåðû",
        "  Ìåñòî Èìÿ Ñ÷åò",
        "  Íàæìèòå \"P\", ÷òîáû Èãðàòü.",
        "  Íàæìèòå \"T\", ÷òîáû ïåðåéòè ê çàñòàâêå èãðû.",
        "  Íàæìèòå \"H\", ÷òîáû ïåðåéòè â ðàçäåë \"Ïîìîùü\"",
        "  Íàæìèòå \"L\", ÷òîáû ïåðåéòè â ðàçäåë \"Ëèäåðû\"",
        "  Èãðà îêîí÷åíà!",
        "  Âåðñèÿ 0.0.2",
        "  Àâòîð: Ìèõàèë Êîðìàíîâñêèé",
        "  Øêîëà: Ìîñêîâñêàÿ Øêîëà Ïðîãðàììèñòîâ",
        "\n\n  ÏÎÌÎÙÜ\n\n  Êàê èãðàòü\n\n  1. Íàæìèòå \"P\" êîãäà Âû íàõîäèòåñü íå íà ñòðàíèöå èãðû (êîãäà Âû íå âèäèòå èãðîâîå ïîëå è ïàäàþùèé áëîê).\n  2. Ââåäèòå ñâîå èìÿ (îíî áóäåò èñïîëüçîâàíî äëÿ õðàíåíèÿ ðåêîðäîâ).\n  3. Èãðàéòå, èñïîëüçóÿ ñëåäóþùèå êëàâèøè äëÿ âçàèìîäåéñòâèÿ ñ èãðîé:\n    Íàæèìàéòå \"A\", ÷òîáû ñìåñòèòü òåêóùèé áëîê âëåâî.\n    Íàæèìàéòå \"S\", ÷òîáû òåêóùèé áëîê íåìåäëåííî óïàë âíèç.  \n    Íàæèìàéòå \"D\" , ÷òîáû ñìåñòèòü òåêóùèé áëîê âïðàâî.  \n    Íàæèìàéòå \"F\", ÷òîáû ïîâåðíóòü òåêóùèé áëîê âëåâî.  \n    Íàæèìàéòå \"G\", ÷òîáû ñìåñòèòü òåêóùèé áëîê âïðàâî.  \n    Íàæèìàéòå \"P\", ÷òîáû ïðèîñòàíîâèòü/ïðîäîëæèòü èãðó.  \n  4. Êîãäà óâèäèòå ñîîáùåíèå \"Èãðà îêîí÷åíà!\", ñëåäóéòå èíñòðóêöèÿì íà ýêðàíå.  \n  5. Åñëè Âû óñòàëè, ïðîñòî çàêðîéòå îêíî. Âàøè ðåêîðäû ñîõðàíåíû!\n\n  Ïîäñ÷åò î÷êîâ\n\n  Äëÿ ïîäñ÷åòà î÷êîâ èñïîëüçóåòñÿ ñëåäóþùèé àëãîðèòì:\n    1. Ïðîãðàììà íàõîäèò L = 1 + 2 + ... + (N-1) + N, ãäå N - êîëè÷åñòâî óáðàííûõ ëèíèé.  \n    2. Ïðîãðàììà óìíîæàåò L íà 100, ïîëó÷åííîå êîëè÷åñòâî ïðîãðàììà äîáàâëÿåò ê Âàøåìó ñ÷åòó.  \n      Òàáëèöà î÷êîâ, çàðàáîòàííûõ çà óäàëåíèå ëèíèé:  \n        1 - 100  \n        2 - 300  \n        3 - 600  \n        4 - 1000  \n       è ò.ä.",
        "  Ïîæàëóéñòà, ââåäèòå èìÿ:",
        "\n\n\n  ×òî íîâîãî?\n\n",
        "  Âåðñèÿ 0.0.2 (2017-09-05)\n\n  Äîáàâëåíà ïîääåðæêà ðóññêîãî ÿçûêà\n\n  Âåðñèÿ 0.0.1 (2017-08-30)\n\n  Íà÷àëüíàÿ âåðñèÿ\n\n\n"
    }
};
//Ïîëó÷åíèå ëîêàëèçîâàííîé ñòðîêè ïî åå id
string get_localized(int str_id){
    return l10n[current_locale][str_id];
}
//Ñîêðàùåíèå äëÿ get_localized
string $(int str_id){
    return get_localized(str_id);
}

//Êîëè÷åñòâî êàäðîâ
int animation_frames = 0;
//Ïîëó÷åíèå ñëó÷àéíîãî ÷èñëà â èíòåðâàëå
int get_random_number(int start, int end){
    return (rand() % (end - start + 1)) + start;
}
//Ïîëó÷åíèå ñëó÷àéíîãî öâåòà (1-14, òàê êàê ÷åðíûé - öâåò ôîíà, à áåëûé - öâåò ðàìêè)
int get_random_color(){
    return get_random_number(1, 14);
}
//Ïîëó÷åíèå ñëó÷àéíîãî òèïà áëîêà (0-6)
int get_random_type(){
    return get_random_number(0, 6);
}
//Ïîëó÷åíèå ñëó÷àéíîé íà÷àëüíîé ëåâîé âåðõíåé òî÷êè
int get_random_start_point(){
    return get_random_number(0,6);
}
//Ïîëó÷åíèå ñëó÷àéíîãî ïîâîðîòà
int get_random_rotation(){
    return get_random_number(0, 3);
}
//Óñòàíîâêà öâåòà ôîíà êîíñîëè
void set_console_background_color(int color){
    SetConsoleTextAttribute(h, color * 16);
}
//Ñáðîñ ôîíà êîíñîëè (÷åðíûé)
void clear_console_background_color(){
    SetConsoleTextAttribute(h, 0x000F);
}
//Ñóùåñòâóåò ëè àêòèâíûé áëîê?
bool is_block_generated = false;
//Ïåðåìåùàëè ëè àêòèâíûé áëîê?
bool has_block_moved = false;
//Èãðà íà ïàóçå èëè íåò?
bool paused = false;
//Ñòèðàíèå áëîêà
void erase_block(){
    //Ñòèðàåì
    for(int i = 0; i < 4; ++i){
        int x = current_block[i][0], y = current_block[i][1];
        field[x][y] = 0;
    }
}
//Çàïèñü áëîêà íà ïîëå
void draw_block(){
    for(int i = 0; i < 4; ++i){
        int x = current_block[i][0], y = current_block[i][1];
            field[x][y] = current_block_color;

    }
}
//Íîâûé áëîê
void generate_block(){
    //Ïîëó÷åíèå ñëó÷àéíîãî öâåòà íîâîãî áëîêà
    current_block_color = get_random_color();
    //Ïîëó÷åíèå ñëó÷àéíîãî òèïà íîâîãî áëîêà. Òèïû: https://ru.wikipedia.org/wiki/Òåòðèñ#/media/File:Tetrominoes_IJLO_STZ_Worlds.svg
    current_block_type = get_random_type();
    //Ëåâàÿ âåðõíÿÿ òî÷êà íîâîãî áëîêà
    int start_point = get_random_start_point();
    //Êîîðäèíàòû íîâîãî áëîêà
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
    //Çàïèñü áëîêà
    draw_block();
    //Èçìåíåíèå ïåðåìåííûõ
    is_block_generated = true;
    has_block_moved = false;
    current_block_rotation = 0;


}
//Êëåòêà çàíÿòà èëè íåò
bool square_free(int x, int y){
    return !field[x][y] && (x > -1) && (x < 10) && (y > -1) && (y < 20);
}
//ßâëÿåòñÿ ëè êëåòêà ÷àñòüþ òåêóùåãî áëîêà
bool square_part_of_current_block(int x, int y){
    for(int i = 0; i < 4; ++i){
        int* s = current_block[i];
        int X = s[0], Y = s[1];
        if(X == x && Y == y) return true;
    }
    return false;
}
//Ìîæíî ëè ïåðåìåùàòüñÿ
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
//Ïåðåìåùåíèå
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
    //Ñòèðàíèå

    for(int i = 0; i < 4; ++i){
        current_block[i][0] += offsetX;
        current_block[i][1] += offsetY;
    }
    //Çàïèñü áëîêà
    draw_block();
    //Ãîâîðèì, ÷òî ïåðåìåñòèëèñü
    has_block_moved = true;
}
//Âû÷èñëåíèå êîîðäèíàò ïîâîðîòà
void _rotate_prepare(string direction){
    //Òèï O. Íå ïîâîðà÷èâàåòñÿ
    if(current_block_type == 3){
        return;
    //Òèï I
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
    //Òèïû S è Z
    }else if(current_block_type == 4 || current_block_type == 6){
        //Äëÿ òèïîâ S è Z äîñòàòî÷íî ïåðåìåñòèòü 2 òî÷êè íà 2 ïóíêòà
        int m = (current_block_type == 4)?-1:1;
        if(!(current_block_rotation%2)){
            current_block[3][0] -= 2;
            current_block[0][1] += m*2;

        }else{
            current_block[3][0] += 2;
            current_block[0][1] -= m*2;
        }
    //Òèï T
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
    //Òèïû J, L
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
//Ìîæíî ëè ïîâîðà÷èâàòü
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
//Ïîâîðîò
void rotate_current_block(string direction){
    erase_block();
    _rotate_prepare(direction);
    //Èçìåíåíèå ïåðåìåííîé ïîâîðîòà
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
//Îòðèñîâêà ñîñòîÿíèÿ
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
//Íàæàòèÿ íà êëàâèàòóðå
bool process_keyboard(){
    char c = 0;
    if (kbhit()) c = getch();
    //Ëåâî
    if(c == 'a' && is_block_generated && can_move_current_block("left") && !paused){
        move_current_block("left");
        return true;
    }
    //Ïðàâî
    else if(c == 'd' && is_block_generated && can_move_current_block("right") && !paused){
        move_current_block("right");
        return true;
    }
    //Óïàñòü ñîâñåì
    else if(c == 's' && is_block_generated && !paused){
        while(can_move_current_block("down"))
            move_current_block("down");
        return true;
    }
    //Ïîâîðîò âëåâî
    else if(c == 'f' && is_block_generated && can_rotate("left")  && !paused){
        rotate_current_block("left");
        return true;
    }
    //Ïîâîðîò âïðàâî
    else if(c == 'g' && is_block_generated && can_rotate("right") && !paused){
        rotate_current_block("right");
        return true;
    }
    //Ïàóçà-Èãðàòü
    else if(c == 'p'){
        paused = !paused;
        return true;

    }
    return false;
}
//Ìîæåì ëè ìû óáðàòü ñòðîêó
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
//Óáðàòü ñòðîêó
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
//Íàæàòèÿ íà êëàâèàòóðå
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
//Ðàñøèðèòü ìàññèâ ñ ïîëüçîâàòåëÿìè
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
//Óäàëèòü ìàññèâ ñ ïîëüçîâàòåëÿìè
void clear_memory(){
    delete [] results;
    delete [] logins;
}
//Çàðåãèñòðèðîâàí ëè ïîëüçîâàòåëü
bool user_registered(){
    for(int i = 0; i < results_size; ++i){
        if(logins[i] == username) return true;
    }
    return false;
}
//×òåíèå leaderboard-à
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
//Î÷èñòêà leaderboard
void clean_leaderboard(){
    fstream f("leaderboard.txt", ios::out | ios::trunc);
    f.close();
}
//Çàïèñü leaderboard
void write_leaderboard(){
    fstream f("leaderboard.txt", ios::out);
    for(int j = 0; j < results_size; ++j){
        string u = logins[j];
        int k = results[j];
        f << u << " " << k << endl;
    }
    f.close();
}
//Îêîí÷àíèå èãðû
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
//Ñîðòèðîâêà ïóçûðüêîì (ñðàçó 2 ìàññèâà - ëîãèíû è ðåçóëüòàòû)
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
//Ïîêàç äîñêè ëèäåðîâ
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
//Ïîêàç ôóòåðà
void show_footer(){
    cout  << endl << endl << $(4) << endl
          << $(5) << endl
          << $(6) << endl
          << $(7) << endl;
}
//Ïîêàç ñòðàíèöû
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
            //Çàäåðæêà. Êàæäûå 15 êàäðîâ çàäåðæêà óìåíüøàåòñÿ íà 10ìñ
            Sleep(500 - animation_frames / 15 * 10);
            //Î÷èñòêà
            system("CLS");
            //Èçìåíåíèå ïîëÿ
            //Åñëè íåò áëîêà, ãåíåðèðóåì åãî
            if(!is_block_generated){
                generate_block();
            }
            //Åñëè íå íàæàëè êëàâèøè
            bool kb = process_keyboard();
            //Ðèñîâàíèå
            draw_state();
            if(!kb){
                //Ïî÷èñòèòü íèç ïîëÿ (óáðàòü öåëûå ëèíèè)
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
                //Åñëè ìîæíî ïàäàòü, ïàäàåì
                if(can_move_current_block("down")){
                    move_current_block("down");
                }else{
                    //Åñëè ïàäàòü íåëüçÿ, ïðîâåðÿåì, åñëè ìû íå ìîæåì ïàäàòü è ìû íå ïàäàëè, òî èãðà îêîí÷åíà
                    if(!has_block_moved){
                        show_page("over");
                        break;
                    }
                    //Ãîâîðèì ïðîãðàììå â ñëåäóþùèé ïðîõîä ñîçäàòü íîâûé áëîê
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
        //Çàïðîñèì èìÿ
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
//Çàïóñê èãðû
void start(){
    //Ïîêàæåì çàñòàâêó
    show_page("locale");
    //Ïî îêîí÷àíèè î÷èñòèì ïàìÿòü
    clear_memory();
}
//main
int main()
{
    //Íà÷èíàåì...
    start();
    return 0;
}

//
