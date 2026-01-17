#include "include.h"



// === NETHACK-LIKE LVL GEN ===

void nethacklike_make_lvl(int lvl){
int lvlroomsn = NHL_MINROOMN+rand()%(NHL_MAXROOMN+1-NHL_MINROOMN);
for (int i = 0; i < lvlroomsn; i++)
	nethacklike_make_room(lvl);
nethacklike_make_corr();}

void nethacklike_make_room(int lvl){
//finding suitable random parameters
int rh, rw, ry, rx, try = 0;
int l = roomsn;
roomlvl[l] = lvl;
do { try++;
rh = rand()%(RMAXH-RMINH)+RMINH;
rw = rand()%(RMAXW-RMINW)+RMINW;
ry = rand()%(LVLH-rh);
rx = rand()%(LVLW-rw);
} while (!nethacklike_room_ok(lvl, rh, rw, ry, rx)
	&& try < TRYN);
//initializations & allocations
roomh[l] = rh; roomw[l] = rw;
roomy[l] = ry; roomx[l] = rx;
make_empty_room(l);
roomsn++;
}

void make_empty_room(int l){
int rh = roomh[l], rw = roomw[l];
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


int nethacklike_room_ok(int lvl, int rh, int rw, int ry, int rx){
for (int i = 0; i < roomsn; i++){
	if (roomlvl[i] != lvl) continue;
	//checking corners
	if (wine_where(lvl, ry,    rx)    != -1
	 || wine_where(lvl, ry,    rx+rw) != -1
	 || wine_where(lvl, ry+rh, rx)    != -1
	 || wine_where(lvl, ry+rh, rx+rw) != -1) return 0;
	//checking overlap
	if (((ry < roomy[i]          && ry+rh > roomy[i])
	  || (ry < roomy[i]+roomh[i] && ry+rh > roomy[i]+roomh[i]))
	 && ((rx < roomx[i]          && rx+rw > roomx[i])
	  || (rx < roomx[i]+roomw[i] && rx+rw > roomx[i]+roomw[i])))
		return 0;}//TODO some rooms still overlap
return 1;
}


void nethacklike_make_corr(void){

}

//by hllcngn
