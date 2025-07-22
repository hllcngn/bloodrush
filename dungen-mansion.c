#include "include.h"



// === MANSION LVL GEN ===

void mansion_make_lvl(int lvl){
int lvlroomsn = rand()%NHL_MAXROOMN + 1;
nethacklike_make_room(lvl);
for (int i = 0; i < lvlroomsn; i++)
	mansion_make_room(lvl);
}

void mansion_make_room(int lvl){
//finding suitable random parameters
int rh, rw, ry, rx, try = 0;
int l = roomsn;
roomlvl[l] = lvl;
do { try++;
ry = rand()%(LVLH-RMINH);
rx = rand()%(LVLW-RMINW);
} while (wine_where(lvl, ry, rx) != -1
	|| try < TRYN
	|| get_closest_room(lvl, ry, rx).distance <= 0);
roomy[l] = ry; roomx[l] = rx;
segment seg = get_closest_room(lvl, ry, rx);
if (seg.axis == 'y'){
	roomh[l] = seg.distance;
	if (seg.direction == -1)
		roomy[l] -= seg.distance;
	roomw[l] = rand()%(RMAXW-RMINW)+RMINW;}
else{	roomw[l] = seg.distance;
	if (seg.direction == -1)
		roomx[l] -= seg.distance;
	roomh[l] = rand()%(RMAXH-RMINH)+RMINH;}
make_empty_room(l);
roomsn++;
}

segment get_closest_room(int lvl, int y, int x){
int h1, h2 = LARGEVALUE, w1, w2 = LARGEVALUE;
int room1, room2;
segment segy, segx;
segy.distance = LARGEVALUE;
segx.distance = LARGEVALUE;
for (int i = 0; i < roomsn; i++){
	if (roomlvl[i] != lvl) continue;
	if (segy.distance > get_distance(y, roomy[i]).distance)
		segy = get_distance(y, roomy[i]);
	if (segx.distance > get_distance(x, roomx[i]).distance)
		segx = get_distance(x, roomx[i]);}
if (segy.distance > segx.distance){
	segx.axis = 'x';
	return segx;}
segy.axis = 'y';
return segy;
}

segment get_distance(int p1, int p2){
segment seg;
seg.distance = p1 > p2 ? p1 - p2 : p2 - p1;
seg.direction = p1 > p2 ? -1 : 1;
return seg;}
