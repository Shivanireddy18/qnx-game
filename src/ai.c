/*
this file handles auto defense (AI part)

when auto mode is ON:
- it checks incoming enemy bullets (high priority)
- if danger is there → it dodges
- if no danger → it targets nearest enemy and attacks

so it is like simple decision making:
survive first → then attack

i separated this into AI thread so it keeps analyzing continuously
and reacts faster without blocking physics or collision

this is the main logic that makes the game "smart"
*/
#include "common.h"

void* ai_thread(void* arg){

    while(1){

        pthread_mutex_lock(&lock);  /* locking because AI also modifies player position + bullets */

        strcpy(game.active_thread,"AI");  /* just to show AI thread running on UI */
        game.ai_load = rand()%100;        /* random value, just for display */

        if(game.auto_defense){  /* only run AI when auto mode is ON */

            float target_x = game.player;  /* where AI wants to move */
            float min_dist = 9999;         /* used to find closest object */
            int danger = 0;                /* flag to detect if bullet is dangerous */

            /* checking enemy bullets first (most important because survival) */
            for(int i=0;i<MAX_E_LASERS;i++){
                if(game.eLasers[i].active){

                    float dy = fabs(game.eLasers[i].y - 450);  /* distance from player Y */
                    float dx = fabs(game.eLasers[i].x - game.player);  /* horizontal distance */

                    if(dy < min_dist){  /* finding closest bullet */
                        min_dist = dy;
                        target_x = game.eLasers[i].x;  /* track that bullet */
                    }

                    if(game.eLasers[i].y > 300 && dx < 80){
                    /* bullet is close and aligned → dangerous */
                        danger = 1;
                    }
                }
            }

            /* if no bullet threat, then focus on enemies */
            if(min_dist == 9999){
                for(int i=0;i<MAX_ENEMIES;i++){

                    float dy = fabs(game.enemies[i].y - 450);  /* distance from player */

                    if(dy < min_dist){
                        min_dist = dy;
                        target_x = game.enemies[i].x;  /* target nearest enemy */
                    }
                }
            }

            /* dodge mode → priority is survival */
            if(game.smart_dodge && danger){

                if(target_x > game.player){
                    game.player -= 15;  /* move opposite side of danger */
                }else{
                    game.player += 15;
                }
            }

            /* attack mode → no danger so focus on shooting */
            else{

                if(fabs(target_x - game.player) > 10){
                /* move closer to target */

                    if(target_x > game.player){
                        game.player += 10;
                    }else{
                        game.player -= 10;
                    }
                }

                if(game.defense_cooldown > 0)
                    game.defense_cooldown--;
                /* reduce cooldown gradually */

                if(fabs(target_x - game.player) < 25 && game.defense_cooldown == 0){
                /* if aligned with target and cooldown ready */

                    for(int i=0;i<MAX_LASERS;i++){
                        if(!game.lasers[i].active){  /* find free bullet slot */

                            game.lasers[i].active = 1;
                            game.lasers[i].x = game.player;  /* shoot from player */
                            game.lasers[i].y = 430;

                            game.defense_cooldown = 8;  /* delay next shot */
                            break;
                        }
                    }
                }
            }
        }

        if(game.player < 20) game.player = 20;   /* prevent going out of left side */
        if(game.player > 780) game.player = 780; /* prevent going out of right side */

        pthread_mutex_unlock(&lock);  /* release lock */

        usleep(20000);  /* faster loop so AI reacts quickly */
    }
}
