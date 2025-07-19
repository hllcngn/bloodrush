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
int wine_room_ok(int lvl, int rh, int rw, int ry, int rx);
int wine_where(int lvl, int y, int x);
void wine_disp(void);
void wine_plmov(int c);
void blood_init(void);
void blood_end(void);
void blood_clr(void);
void blood_curmov(int y, int x);
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


// === M A I N ===

int main(int ac, char **av){
srand(time(NULL));
wine_init();
blood_init();
int q = 1; while (q){//game loop
wine_disp();
char c; read(0, &c, 1);
switch(c){
case 27: q = 0; break;
case 'w':
case 'a':
case 's':
case 'd': wine_plmov(c); break;}
}	//loop end
blood_end();
wine_end();
return 0;}


// === W I N E ===

void wine_init(void){
//initializations
lvlsn = LVLSN;
roomsn = ROOMSN;
//allocations
rlvl  = malloc(sizeof(int)*roomsn);
roomh = malloc(sizeof(int)*roomsn);
roomw = malloc(sizeof(int)*roomsn);
roomy = malloc(sizeof(int)*roomsn);
roomx = malloc(sizeof(int)*roomsn);
room  = malloc(sizeof(int**)*roomsn);
//making dungeon
for (int i = 0; i < roomsn; i++)
	wine_make_room(i);
plroom = 0;//TODO do sth if there aren't any rooms on lvl0
//creating player
for (int i = 0; i < roomsn; i++)
	if (rlvl[i] == 0){ plroom = i; break; }
ply = rand()%(roomh[plroom]-2)+1;
plx = rand()%(roomw[plroom]-2)+1;
}

void wine_end(void){
for (int i = 0; i < roomsn; i++){
	for (int j = 0; j < roomh[i]; j++)
		free(room[i][j]);
	free(room[i]);}
free(room);
free(roomh); free(roomw);
free(roomy); free(roomx);
free(rlvl);
}

void wine_make_room(int l){
//finding suitable random params
int rh, rw, ry, rx; int try = 0;
do { try++;
rlvl[l] = rand()%lvlsn;
rh = rand()%(RMAXH-RMINH)+RMINH;
rw = rand()%(RMAXW-RMINW)+RMINW;
ry = rand()%(LVLH-rh);
rx = rand()%(LVLW-rw);
} while (!wine_room_ok(rlvl[l], rh, rw, ry, rx) && try < 200);
//initializations & allocations
roomh[l] = rh; roomw[l] = rw;
roomy[l] = ry; roomx[l] = rx;
room[l] = malloc(sizeof(int*)*rh);
int **new = room[l];
for (int i = 0; i < rh; i++){
	new[i] = malloc(sizeof(int)*rw+1);}
//top & bottom rows
new[0][rw] = '\0';
new[rh-1][rw] = '\0';
for (int i = 0; i < rw; i++){
	new[0][i] = '-';
	new[rh-1][i] = '-';}
//rest of the rows
for (int i = 1; i < rh-1; i++){
	new[i][0] = '|';
	new[i][rw-1] = '|';
	new[i][rw] = '\0';
	for (int j = 1; j < rw-1; j++)
		new[i][j] = '.';}
}

int wine_room_ok(int lvl, int rh, int rw, int ry, int rx){
for (int i = 0; i < roomsn; i++){
	if (rlvl[i] != lvl) continue;
	//checking corners
	if (wine_where(lvl, ry, rx) != -1
	 || wine_where(lvl, ry, rx+rw) != -1
	 || wine_where(lvl, ry+rh, rx) != -1
	 || wine_where(lvl, ry+rh, rx+rw) != -1) return 0;
	//checking overlap
	if (((ry < roomy[i] && ry+rh > roomy[i])
	  || (ry < roomy[i]+roomh[i] && ry+rh > roomy[i]+roomh[i]))
	 && ((rx < roomx[i] && rx+rw > roomx[i])
	  || (rx < roomx[i]+roomw[i] && rx+rw > roomx[i]+roomw[i])))
		return 0;}
return 1;
}

int wine_in_room(int y, int x){}

int wine_where(int lvl, int y, int x){
for (int i = 0; i < roomsn; i++)
	if (lvl == rlvl[i])
		if (y >= roomy[i] && y <= roomy[i]+roomh[i]
		 && x >= roomx[i] && x <= roomx[i]+roomw[i])
			return i;
return -1;
}

void wine_disp(void){
blood_red();
//draw all rooms on pllvl
for (int j = 0; j < roomsn; j++){
	if (rlvl[j] != pllvl) continue;
	for (int i = 0; i < roomh[j]; i++){
		blood_curmov(roomy[j]+i, roomx[j]);
		printf("%ls\n", room[j][i]);}}
 //TODO sometimes a room won't display properly
 //towards the left side of the screen
//draw player
blood_curmov(roomy[plroom]+ply, roomx[plroom]+plx);
printf("%c\n", '@');}

void wine_plmov(int c){
switch(c){
case 'w': if (ply > 1) ply--; break;
case 'a': if (plx > 1) plx--; break;
case 's': if (ply < roomh[plroom]-2) ply++; break;
case 'd': if (plx < roomw[plroom]-2) plx++; break;}
}


// === B L O O D ===

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
set_cap("smcup", TRUE);//alternate screen buffer
}

void blood_end(void){
set_cap("cnorm", TRUE);//cursor visible
set_cap("rmcup", TRUE);//exit alternate buffer
reset_shell_mode();
}

void blood_clr(void){ set_cap("clear", TRUE); }
void blood_curmov(int y, int x){ set_cap("cup", y, x); }
void blood_red(void){ set_cap("setaf", COLOR_RED); }


// === M I S C ===
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
tputs(s2, 1, putchar);
}

//by hllcngn
