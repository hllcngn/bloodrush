#include "include.h"

// global variables
// dungeon
int lvlsn;
int roomsn;
int ***room;
int *roomlvl;
int *roomh, *roomw, *roomy, *roomx;

// player
int pllvl, plroom;
int ply, plx;

// mobs
int *moblvl, *mobroom;
int *moby, *mobx;



// === M A I N ===

int main(int ac, char **av){
srand(time(NULL));
wine_init();
blood_init();
int q = 1; while (q){// game loop
wine_disp();
char c; read(0, &c, 1);
switch(c){
case 27: q = 0; break;
case 'w':
case 'a':
case 's':
case 'd': wine_plmov(c); break;}
}		// game loop end
blood_end();
wine_end();
return 0;}



// === W I N E ===

void wine_init(void){
//initializations
lvlsn = LVLSN;
//allocations
//TODO dynamically realloc components
//or, it's just ints - I can have those just large
roomlvl= malloc(sizeof(int)*ROOMSN);
roomh  = malloc(sizeof(int)*ROOMSN);
roomw  = malloc(sizeof(int)*ROOMSN);
roomy  = malloc(sizeof(int)*ROOMSN);
roomx  = malloc(sizeof(int)*ROOMSN);
room   = malloc(sizeof(int**)*ROOMSN);
//making dungeon
roomsn = 0;
nethacklike_make_lvl(0);
wine_make_mobs();
//creating player
plroom = 0;//TODO do sth if there aren't any rooms on lvl0
for (int i = 0; i < roomsn; i++)
	if (roomlvl[i] == 0){ plroom = i; break; }
ply = rand()%(roomh[plroom]-2)+1;
plx = rand()%(roomw[plroom]-2)+1;
}

void wine_end(void){
for (int i = 0; i < roomsn; i++){
	for (int j = 0; j < roomh[i]; j++)
		free(room[i][j]);
	free(room[i]);}
free(room); free(roomlvl);
free(roomh); free(roomw); free(roomy); free(roomx);
free(moblvl); free(mobroom); free(moby); free(mobx);
}

int wine_where(int lvl, int y, int x){
for (int i = 0; i < roomsn; i++)
	if (roomlvl[i] == lvl)
		if (y >= roomy[i] && y <= roomy[i]+roomh[i]
		 && x >= roomx[i] && x <= roomx[i]+roomw[i])
			return i;
return -1;
}

void wine_make_mobs(void){
moblvl = malloc(sizeof(int)*MOBN);
mobroom= malloc(sizeof(int)*MOBN);
moby   = malloc(sizeof(int)*MOBN);
mobx   = malloc(sizeof(int)*MOBN);
for (int i = 0; i < MOBN; i++){
	mobroom[i] = rand()%roomsn;
	moblvl[i] = roomlvl[mobroom[i]];
	moby[i] = rand()%(roomh[mobroom[i]]-2)+1;
	mobx[i] = rand()%(roomw[mobroom[i]]-2)+1;}
}

void wine_disp(void){
blood_red();
// draw all rooms on pllvl
for (int j = 0; j < roomsn; j++){
	if (roomlvl[j] != pllvl) continue;
	for (int i = 0; i < roomh[j]; i++){
		blood_curmov(roomy[j]+i, roomx[j]);
		printf("%ls\n", room[j][i]);}}
  //TODO sometimes a room won't display properly
  //towards the left side of the screen
// draw player
blood_curmov(roomy[plroom]+ply, roomx[plroom]+plx);
printf("%c\n", '@');
// draw mobs
for (int i = 0; i < MOBN; i++){
	if (moblvl[i] == pllvl){
		blood_curmov(roomy[mobroom[i]]+moby[i], roomx[mobroom[i]]+mobx[i]);
		printf("%c\n", 'G');}}
}

void wine_plmov(int c){
switch(c){
case 'w': if (ply > 1)               ply--; break;
case 'a': if (plx > 1)               plx--; break;
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
//term.c_cc[VTIME] = 0;//read returns immediately
tcsetattr(0, TCSANOW, &term);//apply settings
blood_set_cap("civis", TRUE);//cursor invisible
blood_set_cap("smcup", TRUE);//alternate screen buffer
}

void blood_end(void){
blood_set_cap("cnorm", TRUE);//cursor visible
blood_set_cap("rmcup", TRUE);//exit alternate buffer
reset_shell_mode();
}

void blood_set_cap(const char *str, ...){
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

void blood_clr   (void)        { blood_set_cap("clear", TRUE); }
void blood_curmov(int y, int x){ blood_set_cap("cup", y, x); }
void blood_red   (void)        { blood_set_cap("setaf", COLOR_RED); }

//by hllcngn
