#ifndef INCLUDE_H
#define INCLUDE_H

#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>
#include <term.h>

#define TRYN 200
#define LARGEVALUE 1000

// dungeon defines
#define LVLH 13//20
#define LVLW 35//60
#define LVLSN 5
#define ROOMSN 100
#define MOBN 30

//nethacklike generation
#define NHL_MAXROOMN 5
#define RMAXH 10
#define RMAXW 20
#define RMINH 5
#define RMINW 8

//masion generation
#define MAN_MAXROOMN 30


typedef struct{
	int distance;
	int direction;
	int axis;
	int room;}segment;

// global variables
// dungeon
extern int lvlsn;
extern int roomsn;
extern int ***room;
extern int *roomlvl;
extern int *roomh, *roomw, *roomy, *roomx;

// player
extern int pllvl, plroom;
extern int ply, plx;

// mobs
extern int *moblvl, *mobroom;
extern int *moby, *mobx;


// prototypes
//main.c
void wine_init(void);
void wine_end(void);
int  wine_where(int lvl, int y, int x);
int wine_where_next(int j, int lvl, int y, int x);
void wine_make_mobs(void);
void wine_disp(void);
void wine_plmov(int c);

void blood_init(void);
void blood_end(void);
void blood_set_cap(const char *str, ...);
void blood_clr(void);
void blood_curmov(int y, int x);
void blood_red(void);

//dungen-nethacklike.c
void nethacklike_make_lvl(int lvl);
void nethacklike_make_room(int lvl);
int  nethacklike_room_ok(int lvl, int rh, int rw, int ry, int rx);
void nethacklike_make_corr(void);
void make_empty_room(int l);

//dungen-mansion.c
void mansion_make_lvl(int lvl);
void mansion_make_room(int lvl);
segment get_closest_room(int lvl, int y, int x);
segment get_distance(int p1, int p2);
int move_room_several(int n, int l, int axis, int direction);
int move_room(int l, int axis, int direction);
int expand_room(int l, int axis, int direction);
void mansion_make_doorway(int lvl);

#endif
