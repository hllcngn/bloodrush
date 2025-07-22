#include "include.h"



// === MANSION LVL GEN ===

void mansion_make_lvl(int lvl){
int lvlroomsn = 2;//rand()%MAN_MAXROOMN + 1;
nethacklike_make_room(lvl);
for (int i = 0; i < lvlroomsn-1; i++)
	mansion_make_room(lvl);
}

void mansion_make_room(int lvl){
int ry, rx, try = 0, l = roomsn;
roomlvl[l] = lvl;
do { try++;
ry = rand()%(LVLH-RMINH);
rx = rand()%(LVLW-RMINW);
} while ((wine_where(lvl, ry, rx) != -1
	|| get_closest_room(lvl, ry, rx).distance <= 2)
	&& try < TRYN);
if (try < TRYN){
printf("room position: ry=%d, rx=%d\n", ry, rx);
	roomy[l] = ry; roomx[l] = rx;
	segment seg = get_closest_room(lvl, ry, rx);
printf("closest room at: %d\n", seg.distance);
printf("on axis: %c\n", seg.axis);
printf("direction: %d\n", seg.direction);
	if (seg.axis == 'y'){
		roomh[l] = seg.distance;
		if (seg.direction == -1)
			roomy[l] -= seg.distance;
		roomw[l] = rand()%(RMAXW-RMINW)+RMINW;
		segment segx = get_distance(rx, roomx[seg.room]);
		if (segx.direction == -1)
			roomx[l] -= segx.distance-(roomw[seg.room]-2) < 0 ? 0
				: segx.distance-(roomw[seg.room]-2);
		else	roomx[l] += segx.distance-(roomw[l]-2) < 0 ? 0
				: segx.distance-(roomw[l]-2);
		//roomx[l] = roomx[seg.room];
	}
	else{	roomw[l] = seg.distance;
		if (seg.direction == -1)
			roomx[l] -= seg.distance;
		roomh[l] = rand()%(RMAXH-RMINH)+RMINH;
		segment segy = get_distance(ry, roomy[seg.room]);
		if (segy.direction == -1)
			roomy[l] -= segy.distance-(roomh[seg.room]-2) < 0 ? 0
				: segy.distance-(roomh[seg.room]-2);
		else	roomy[l] += segy.distance-(roomh[l]-2) < 0 ? 0
				: segy.distance-(roomh[l]-2);
		//roomy[l] = roomy[seg.room];
	}
printf("room dimensions: rh=%d, rw=%d\n", roomh[l], roomw[l]);
printf("final room position: ry=%d, rx=%d\n", roomy[l], roomx[l]);
	make_empty_room(l);
	roomsn++;}
}

segment get_closest_room(int lvl, int y, int x){
segment segy, segx;
segy.distance = segx.distance = LARGEVALUE;
for (int i = 0; i < roomsn; i++){
	if (roomlvl[i] != lvl) continue;
	if (segy.distance > get_distance(y, roomy[i]).distance){
		segy = get_distance(y, roomy[i]);
		segy.room = i;}
	if (segx.distance > get_distance(x, roomx[i]).distance){
		segx = get_distance(x, roomx[i]);
		segx.room = i;}}
if (segy.distance > segx.distance){
	segx.axis = 'x';
	return segx;}
segy.axis = 'y';
return segy;
}

segment get_distance(int p1, int p2){
segment seg;
seg.distance = p1 > p2 ? p1 - p2 : p2 - p1;
seg.distance++;
seg.direction = p1 > p2 ? -1 : 1;
return seg;}
