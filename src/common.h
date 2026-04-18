/*
this file is like the backbone of the whole project

i put all common things here so every file can use the same data
like structures, constants and global variables

main thing here is GameState structure
which stores everything like player, enemies, bullets, score etc.

all threads access this same structure so game stays consistent

also declared all thread functions here so they can be used anywhere

basically:
this file = shared memory + definitions for whole project
*/
#ifndef COMMON_H   /* to avoid multiple include issue (if this file included many times) */
#define COMMON_H

#include <stdio.h>      /* normal printing etc. */
#include <stdlib.h>     /* for rand() */
#include <pthread.h>    /* for threads */
#include <unistd.h>     /* for sleep/usleep */
#include <string.h>     /* for strcpy */
#include <time.h>       /* for srand(time) */
#include <math.h>       /* for sin, cos, fabs */
#include <sys/socket.h> /* socket stuff for network */
#include <netinet/in.h> /* internet structs */
#include <arpa/inet.h>  /* ip related */

#define PORT 9000          /* server port (frontend connects here) */
#define MAX_ENEMIES 3      /* keeping small for now */
#define MAX_LASERS 10      /* player bullets max */
#define MAX_E_LASERS 5     /* enemy bullets max */

/* simple enemy struct → only position needed */
typedef struct { float x,y; } Enemy;

/* laser struct → position + active flag (active=1 means currently used) */
typedef struct { float x,y; int active; } Laser;

/* this is the main game structure → everything stored here */
typedef struct {

float player;  /* player x position (only horizontal movement) */

int score,lives,game_over;  /* basic game info */

Enemy enemies[MAX_ENEMIES];  /* all enemies */
Laser lasers[MAX_LASERS];    /* player bullets */
Laser eLasers[MAX_E_LASERS]; /* enemy bullets */

float boss_x,boss_y;  /* boss position */
int boss_active,boss_health;  /* boss state */

int explosion_x,explosion_y,explosion_active;
/* used for showing explosion on screen */

int auto_defense,smart_dodge;
/* auto mode + dodge feature */

int defense_cooldown,hit_cooldown;
/* cooldowns so actions don’t repeat too fast (important for gameplay) */

int physics_load,ai_load,collision_load,network_load;
/* just for showing thread load (not actual CPU load) */

char active_thread[20];  /* to show which thread is running */
char threat[20];         /* LOW / MEDIUM / HIGH based on enemies */

} GameState;

/* shared variables → used in all threads */
extern GameState game;        /* main game data (everyone reads/writes this) */
extern pthread_mutex_t lock;  /* needed because multiple threads use same data */

/* functions */
void init_game();  /* used to initialize/reset everything */

/* threads */
void* physics_thread(void*);    /* handles movement, spawning etc. */
void* collision_thread(void*);  /* handles all hit detection */
void* ai_thread(void*);         /* auto defense logic */
void* network_thread(void*);    /* connects backend with frontend */

#endif
