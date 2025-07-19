#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>
#include <term.h>
// dungeon defines
#define LVLH 20
#define LVLW 60
#define LVLSN 5
#define ROOMSN 20
#define RMAXH 10
#define RMAXW 20
#define RMINH 5
#define RMINW 8
// prototypes
void set_cap(const char *str, ...);
void wine_init(void);
void wine_end(void);
void wine_make_room(int l);
void wine_disp(void);
void wine_plmov(int c);
void blood_init(void);
void blood_end(void);
void blood_clr(void);
void blood_cumo(int y, int x);
void blood_red(void);

// dungeon
int lvlsn;
int roomsn;
int ***room;
int *roomh;
int *roomw;
int *roomy;
int *roomx;
int *rlvl;

// player
int pllvl;
int plroom;
int ply, plx;

// === MAIN ===
int main(int ac, char **av){
srand(time(NULL));
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
}//loop end

blood_end();
wine_end();
return 0;}

// === WINE ===
void wine_init(void){
lvlsn = LVLSN;
roomsn = ROOMSN;
rlvl = malloc(sizeof(int)*roomsn);
roomh = malloc(sizeof(int)*roomsn);
roomw = malloc(sizeof(int)*roomsn);
roomy = malloc(sizeof(int)*roomsn);
roomx = malloc(sizeof(int)*roomsn);
room = malloc(sizeof(int**)*roomsn);
for (int i = 0; i < roomsn; i++)
	wine_make_room(i);
plroom = 0;
ply = rand()%(roomh[plroom]-2)+1;
plx = rand()%(roomw[plroom]-2)+1;}

void wine_end(void){
for (int i = 0; i < roomsn; i++){
	for (int j = 0; j < roomh[i]; j++)
		free(room[i][j]);
	free(room[i]);}
free(room);
free(roomh); free(roomw); free(rlvl);}

void wine_make_room(int l){
//do {
rlvl[l] = rand()%lvlsn;
int rh = rand()%(RMAXH-RMINH)+RMINH;
int rw = rand()%(RMAXW-RMINW)+RMINW;
int ry = rand()%(LVLH-rh);
int rx = rand()%(LVLW-rw);
//} while (wine_room_ok(rlvl[l], rh, rw));
roomh[l] = rh; roomw[l] = rw;
roomy[l] = ry; roomx[l] = rx;
room[l] = malloc(sizeof(int*)*rh);
int **new = room[l];
for (int i = 0; i < rh; i++){
	new[i] = malloc(sizeof(int)*rw+1);}
new[0][rw] = '\0';
new[rh-1][rw] = '\0';
for (int i = 0; i < rw; i++){
	new[0][i] = '-';
	new[rh-1][i] = '-';}
for (int i = 1; i < rh-1; i++){
	new[i][0] = '|';
	new[i][rw-1] = '|';
	new[i][rw] = '\0';
	for (int j = 1; j < rw-1; j++)
		new[i][j] = '.';}}

/*
void wine_room_ok(int lvl, int rh, int rw){
for (int i = 0; i < roomsn && room[i]; i++)
	if (rlvl[i] == lvl)
		if (
}
*/

void wine_disp(void){
blood_red();
for (int j = 0; j < roomsn; j++){
	if (rlvl[j] != pllvl) continue;
	for (int i = 0; i < roomh[j]; i++){
		blood_cumo(roomy[j]+i, roomx[j]);
		printf("%ls\n", room[j][i]);}}
blood_cumo(roomy[plroom]+ply, roomx[plroom]+plx); printf("%c\n", '@');}

void wine_plmov(int c){
switch(c){
case 'w': if (ply > 1) ply--; break;
case 'a': if (plx > 1) plx--; break;
case 's': if (ply < roomh[plroom]-2) ply++; break;
case 'd': if (plx < roomw[plroom]-2) plx++; break;}}

// === BLOOD ===
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

// === MISC ===
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
