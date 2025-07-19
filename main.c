#include <unistd.h>
#include <stdlib.h>
#include <ncurses.h>
#include <term.h>
void set_cap(const char *str, ...);
void wine_init(void);
void wine_end(void);
void wine_disp(void);
void wine_plmov(int c);
void blood_init(void);
void blood_end(void);
void blood_clr(void);
void blood_cumo(int y, int x);
void blood_red(void);

int roomh, roomw;
int **room;

int lvl;
int **plroom;
int ply, plx;


int main(int ac, char **av){
wine_init();
blood_init();

int q = 1; while (q){
wine_disp();

char c;
read(0, &c, 1);
switch(c){
case 27: q = 0; break;
case 'w':
case 'a':
case 's':
case 'd': wine_plmov(c); break;
}
}//while end

blood_end();
wine_end();
return 0;}


void wine_init(void){
roomh = 10;
roomw = 20;
room = malloc(sizeof(int*)*roomh);
for (int i = 0; i < roomh; i++){
	room[i] = malloc(sizeof(int)*roomw+1);}
room[0][roomw] = '\0';
room[roomh-1][roomw] = '\0';
for (int i = 0; i < roomw; i++){
	room[0][i] = '-';
	room[roomh-1][i] = '-';}
for (int i = 1; i < roomh-1; i++){
	room[i][0] = '|';
	room[i][roomw-1] = '|';
	room[i][roomw] = '\0';
	for (int j = 1; j < roomw-1; j++)
		room[i][j] = '.';}
plroom = room;
ply = 5; plx = 5;}

void wine_end(void){
for (int i = 0; i < roomh; i++)
	free(room[i]);
free(room);}

void wine_disp(void){
blood_red();
blood_cumo(0, 0);
for (int i = 0; i < roomh; i++){
	printf("%ls\n", room[i]);}
blood_cumo(ply, plx);
printf("%c\n", '@');}

void wine_plmov(int c){
switch(c){
case 'w': if (ply > 1) ply--; break;
case 'a': if (plx > 1) plx--; break;
case 's': if (ply < roomh-2) ply++; break;
case 'd': if (plx < roomw-2) plx++; break;
}}


void blood_init(void){
setupterm(NULL, 1, NULL);
struct termios term;
tcgetattr(0, &term);//get terminal settings
term.c_lflag &= (~ECHO);//noecho
term.c_lflag &= (~ICANON);//turn off line editing
//the following two lines seem interchangeable
term.c_cc[VMIN] = 1;//read returns when 1 char is available
term.c_cc[VTIME] = 0;//read returns immediately
tcsetattr(0, TCSANOW, &term);//apply settings
set_cap("civis", TRUE);//cursor invisible
set_cap("smcup", TRUE);}//alternate screen buffer

void blood_end(void){
set_cap("cnorm", TRUE);//cursor visible
set_cap("rmcup", TRUE);//exit alternate buffer
reset_shell_mode();}

void blood_clr(void){ set_cap("clear", TRUE); }
void blood_cumo(int y, int x){ set_cap("cup", y, x); }
void blood_red(void){ set_cap("setaf", COLOR_RED); }


void set_cap(const char *str, ...){
va_list al;
va_start(al, str);
int args[2];
args[0] = va_arg(al, int);
args[1] = va_arg(al, int);
va_end(al);
char *s = tigetstr(str);
char *s2;
if (!args[1])//doesnt work if args[1]=0
	s2 = tiparm(s, args[0]);
else	s2 = tiparm(s, args[0], args[1]);
tputs(s2, 1, putchar);}
