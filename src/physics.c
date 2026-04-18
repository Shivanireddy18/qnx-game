/*
this file handles all movement in the game

like:
- enemies moving down
- bullets moving
- boss movement
- enemy shooting
- spawning logic

i separated this into physics thread so movement runs continuously
and smoothly without blocking other parts

also threat level is calculated here based on enemies

so this file is mainly responsible for "what is happening on screen"
*/
#include "common.h"

void* physics_thread(void* arg){

    while(1){

        pthread_mutex_lock(&lock);  /* lock because this thread updates positions, bullets etc., shared data */

        strcpy(game.active_thread,"PHYSICS");  /* just to show physics thread is running in UI */
        game.physics_load = rand()%100;        /* random value, just for display not actual load */

        int activeEnemies=0;  /* count how many enemies are visible */

        for(int i=0;i<MAX_ENEMIES;i++){
            if(game.enemies[i].y>0 && game.enemies[i].y<400) activeEnemies++;  /* enemy inside screen → count */
        }

        if(activeEnemies<=1) strcpy(game.threat,"LOW");        /* less enemies */
        else if(activeEnemies==2) strcpy(game.threat,"MEDIUM");/* medium */
        else strcpy(game.threat,"HIGH");                      /* many enemies */

        if(!game.game_over){  /* stop everything if game is over */

            for(int i=0;i<MAX_ENEMIES;i++){

                game.enemies[i].y += 3;  /* enemies slowly moving down */

                if(game.enemies[i].y > 500){
                    game.enemies[i].y = 0;          /* send back to top */
                    game.enemies[i].x = rand()%800; /* random x so not same path */
                }

                if(rand()%25==1){  /* random shooting, not always */
                    for(int j=0;j<MAX_E_LASERS;j++){
                        if(!game.eLasers[j].active){
                            game.eLasers[j].active=1;
                            game.eLasers[j].x=game.enemies[i].x;  /* bullet starts from enemy */
                            game.eLasers[j].y=game.enemies[i].y;
                            break;  /* assign one bullet only */
                        }
                    }
                }
            }

            for(int i=0;i<MAX_LASERS;i++){
                if(game.lasers[i].active){
                    game.lasers[i].y -= 12;  /* player bullet goes up fast */
                    if(game.lasers[i].y < 0) game.lasers[i].active = 0;  /* remove when out */
                }
            }

            for(int i=0;i<MAX_E_LASERS;i++){
                if(game.eLasers[i].active){
                    game.eLasers[i].y += 8;  /* enemy bullet goes down */
                    if(game.eLasers[i].y > 500) game.eLasers[i].active = 0;  /* remove when out */
                }
            }

            if(!game.boss_active && rand()%300==1){  /* boss comes rarely */
                game.boss_active=1;
                game.boss_x=300;   /* starting x */
                game.boss_y=50;    /* starting y */
                game.boss_health=100;  /* full health */
            }

            if(game.boss_active){
                static float t=0;  /* used for smooth movement */
                t += 0.1;
                game.boss_x += sin(t)*3;  /* left-right movement */
                game.boss_y += cos(t)*2;  /* slight up-down */
            }
        }

        pthread_mutex_unlock(&lock);  /* release lock */

        usleep(40000);  /* delay so game speed is controlled */
    }
}
