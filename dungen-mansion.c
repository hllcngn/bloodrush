#include "include.h"



// === MANSION LVL GEN ===

void mansion_make_lvl(int lvl){
int lvlroomsn = 2;// rand()%MAN_MAXROOMN + 1;
nethacklike_make_room(lvl);
for (int i = 0; i < lvlroomsn-1; i++)
	mansion_make_room(lvl);
mansion_make_doorway(lvl);
}

void mansion_make_room(int lvl){
int ry, rx, try = 0, l = roomsn;
roomlvl[l] = lvl;
do { try++;
ry = rand()%(LVLH-RMINH);
rx = rand()%(LVLW-RMINW);
} while ((wine_where(lvl, ry, rx) != -1
	|| get_closest_room(lvl, ry, rx).distance <= 4)
	&& try < TRYN);

if (try < TRYN){
//printf("room position: ry=%d, rx=%d\n", ry, rx);
	roomy[l] = ry; roomx[l] = rx;
	segment seg = get_closest_room(lvl, ry, rx);
//printf("closest room at: %d\n", seg.distance);
//printf("on axis: %c\n", seg.axis);
//printf("direction: %d\n", seg.direction);
	if (seg.axis == 'y'){
		roomh[l] = seg.distance;
		if (seg.direction == -1)
			//move_room_several(seg.distance-1, l, 'y', -1);
			roomy[l] -= seg.distance-1;
		roomw[l] = rand()%(RMAXW-RMINW)+RMINW;
		segment segx = get_distance(rx, roomx[seg.room]);
		if (segx.direction == -1){
			//move_room_several(segx.distance-roomw[seg.room],
			//		l, 'x', -1);
			roomx[l] -= segx.distance-roomw[seg.room];
			if (roomy[l]==roomy[seg.room]+roomh[seg.room]-1
			 || roomy[l]+roomh[l]-1==roomy[seg.room])
				roomx[l] -= 2;}
		else{	roomx[l] += segx.distance-(roomw[l]);
			if (roomy[l]==roomy[seg.room]+roomh[seg.room]-1
			 || roomy[l]+roomh[l]-1==roomy[seg.room])
				roomx[l] += 2;}}

	else{	roomw[l] = seg.distance;
		if (seg.direction == -1)
			roomx[l] -= seg.distance-1;
		roomh[l] = rand()%(RMAXH-RMINH)+RMINH;
		segment segy = get_distance(ry, roomy[seg.room]);
		if (segy.direction == -1){
			roomy[l] -= segy.distance-(roomh[seg.room]);
			if (roomx[l]==roomx[seg.room]+roomw[seg.room]-1
			 || roomx[l]+roomw[l]-1==roomx[seg.room])
				roomy[l] -= 2;}
		else{	roomy[l] += segy.distance-(roomh[l]);
			if (roomx[l]==roomx[seg.room]+roomw[seg.room]-1
			 || roomx[l]+roomw[l]-1==roomx[seg.room])
				roomy[l] += 2;}}

//printf("room dimensions: rh=%d, rw=%d\n", roomh[l], roomw[l]);
//printf("final room position: ry=%d, rx=%d\n", roomy[l], roomx[l]);
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

int move_room_several(int n, int l, int axis, int direction){
while (n > 0 && move_room(l, axis, direction)!=0) n--;}

int move_room(int l, int axis, int direction){
int i = 0;
if (axis == 'y'){
	if (direction == -1){
		while ((i = wine_where_next(i, roomlvl[l],
				roomy[l]-1, roomx[l])) != -1
			|| roomy[l]-1 == roomy[i]+roomh[i]-1){
		roomy[l]--; return 1;}
		return 0;}
	else{	while ((i = wine_where_next(i, roomlvl[l],
				roomy[l]+1, roomx[l])) != -1
			|| roomy[l]+1 == roomy[i]){
		roomy[l]++; return 1;}
		return 0;}
}else{	if (direction == -1){
		while ((i = wine_where_next(i, roomlvl[l],
				roomy[l], roomx[l]-1)) != -1
			|| roomx[l]-1 == roomx[i]+roomw[i]-1){
		roomx[l]--; return 1;}
		return 0;}
	else{	while ((i = wine_where_next(i, roomlvl[l],
				roomy[l], roomx[l]+1)) != -1
			|| roomx[l]+1 == roomx[i]){
		roomx[l]++; return 1;}
		return 0;}}}

int expand_room(int l, int axis, int direction){

}

void mansion_make_doorway(int lvl){
int room1 = -1;
while (room1 < roomsn-2){
	for (int i = room1+1; i < roomsn; i++)
		if (roomlvl[i] == lvl){
			room1 = i; break;}
	for (int i = room1; i < roomsn; i++){
		if (roomlvl[i] != lvl) continue;
		else if (roomy[i]+roomh[i]-1 == roomy[room1]){
			printf("coucou\n");
			if (roomx[i] <= roomx[room1]){
				int r = rand()%(roomw[i]-(roomx[room1]-roomx[i])-2)+1;
				room[room1][0][r] = 'Z';
				room[i][roomh[i]-1][roomx[room1]-roomx[i]+r] = 'Z';
			}
			else{	int r = rand()%(roomw[i]-(roomx[i]-roomx[room1])-2)+1;
				room[room1][0][r+roomx[room1]-roomx[i]] = 'Z';
				room[i][roomh[i]-1][r] = 'Z';
			}
		}
	//	else if (roomy[i] == roomy[room1]+roomh[room1])
	}
}
}
