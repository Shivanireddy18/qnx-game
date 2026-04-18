/*
this is the main starting point of the game

basically here i am initializing everything and starting all the threads
like physics, ai, collision and network

each thread is doing one specific job so everything runs parallel
instead of writing everything in one loop (which becomes messy)

also this file connects all modules together using shared GameState

so main work here:
- initialize game values
- create threads
- keep program running

i kept this simple because actual logic is in other files
*/
#include "common.h"

GameState game;  /* this holds full game data, like player position, enemies, score, lives, bullets, boss etc.
                    all threads use this, so whatever happens in game is updated here */

pthread_mutex_t lock;  /* using mutex because many threads (physics, ai, collision, network) access same data
                          without this, two threads may update at same time and cause wrong values */


/* function used to start or reset the game */
void init_game(){

    game.player = 400;  /* player moves only in x direction, so keeping it in middle of screen (~800 width) */

    game.score = 0;     /* score starts from 0 */
    game.lives = 5;     /* player gets 5 chances */
    game.game_over = 0; /* 0 means game is still running */

    for(int i=0;i<MAX_ENEMIES;i++){
        game.enemies[i].x = (i+1)*200 + rand()%100;  /* placing enemies at different x positions
                                                        (200 gap) + random value so not fixed */
        game.enemies[i].y = rand()%200;              /* random y so enemies spawn at different heights */
    }

    for(int i=0;i<MAX_LASERS;i++)
        game.lasers[i].active = 0;  /* no player bullets active at start */

    for(int i=0;i<MAX_E_LASERS;i++)
        game.eLasers[i].active = 0; /* no enemy bullets active at start */

    game.auto_defense = 0;  /* auto mode off, player controls manually */
    game.smart_dodge = 1;   /* dodge feature on, used by ai */

    game.defense_cooldown = 0;  /* used to control shooting delay, so bullets not fired continuously */
    game.hit_cooldown = 0;      /* prevents player from losing multiple lives at same time */

    game.boss_active = 0;   /* boss not spawned initially */
    game.boss_health = 100; /* boss full health when appears */

    strcpy(game.active_thread,"IDLE");  /* just for showing which thread is running in UI */
    strcpy(game.threat,"LOW");          /* initial threat level */
}


int main(){

    srand(time(NULL));  /* sets random seed so rand() gives different values every run */

    pthread_mutex_init(&lock,NULL);  /* initializing mutex before threads start using shared data */

    init_game();  /* setting all initial values of game */

    pthread_t p,a,c,n;  /* thread variables for different modules */

    pthread_create(&p,NULL,physics_thread,NULL);
    /* physics thread handles movement of enemies, bullets and boss continuously */

    pthread_create(&a,NULL,ai_thread,NULL);
    /* ai thread handles auto defense, decides when to move and shoot */

    pthread_create(&c,NULL,collision_thread,NULL);
    /* collision thread checks if bullets hit enemies or player, updates score/lives */

    pthread_create(&n,NULL,network_thread,NULL);
    /* network thread connects with frontend, sends game data and receives inputs */

    while(1) sleep(1);
    /* main thread does nothing, just keeps program running so other threads can work */
}
