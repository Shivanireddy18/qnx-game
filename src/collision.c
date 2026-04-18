/*
this file handles all collision detection

like:
- enemy bullets hitting player
- player bullets hitting enemies
- player bullets hitting boss

also updates:
- score
- lives
- explosion effects
- game over condition

i separated this because collision logic can get complex
and mixing it with movement will make code confusing

so this file is basically deciding "what happens after objects touch"
*/
#include "common.h"

void* collision_thread(void* arg){

    while(1){

        pthread_mutex_lock(&lock);  /* lock needed because we update lives, score etc. */

        strcpy(game.active_thread,"COLLISION");  /* just showing collision thread on UI */
        game.collision_load = rand()%100;        /* random number, just for display */

        for(int i=0;i<MAX_E_LASERS;i++){
            if(game.eLasers[i].active){  /* check only active enemy bullets */

                if(game.eLasers[i].y>420 && game.eLasers[i].y<470){  /* near player area */

                    if(fabs(game.eLasers[i].x - game.player) < 20){  /* x close → hit */

                        if(game.hit_cooldown==0){  /* cooldown so one bullet doesn’t hit multiple times */
                            game.lives--;          /* reduce life */
                            game.hit_cooldown=15;  /* wait before next hit */
                        }

                        game.eLasers[i].active=0;  /* remove bullet after hit */
                    }
                }
            }
        }

        if(game.hit_cooldown>0) game.hit_cooldown--;  /* reduce cooldown every loop */

        for(int l=0;l<MAX_LASERS;l++){
            if(game.lasers[l].active){  /* check player bullets */

                for(int i=0;i<MAX_ENEMIES;i++){
                    if(fabs(game.lasers[l].x-game.enemies[i].x)<25 &&
                       fabs(game.lasers[l].y-game.enemies[i].y)<25){  /* simple hit check */

                        game.score += 10;  /* add score */

                        game.explosion_x = game.enemies[i].x;
                        game.explosion_y = game.enemies[i].y;
                        game.explosion_active = 1;  /* show explosion */

                        game.enemies[i].y = 0;  /* send enemy back to top */

                        game.lasers[l].active = 0;  /* remove bullet */
                    }
                }

                if(game.boss_active){  /* check boss hit */
                    if(fabs(game.lasers[l].x-game.boss_x)<50 &&
                       fabs(game.lasers[l].y-game.boss_y)<50){  /* bigger area for boss */

                        game.boss_health -= 5;  /* reduce boss health */

                        game.explosion_x = game.boss_x;
                        game.explosion_y = game.boss_y;
                        game.explosion_active = 1;  /* explosion on boss */

                        game.lasers[l].active = 0;  /* remove bullet */

                        if(game.boss_health <= 0){  /* boss dead */
                            game.boss_active = 0;
                            game.score += 200;  /* big score */
                        }
                    }
                }
            }
        }

        if(game.lives <= 0) game.game_over = 1;  /* no lives → game over */

        pthread_mutex_unlock(&lock);  /* release lock */

        usleep(50000);  /* delay so not too fast */
    }
}
