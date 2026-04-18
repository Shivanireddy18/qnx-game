/*
this file connects frontend (browser) and backend (QNX C code)

it works like a small server:
- receives input from frontend (left, right, shoot, auto etc.)
- updates game state based on input
- sends updated game data as JSON

so frontend does not directly access game data
everything goes through this network layer

this is important because:
UI (JS) and logic (C) are separated

so this file is basically the communication bridge
*/
#include "common.h"  /* including common stuff like structs and globals */

void* network_thread(void* arg){

int server_fd, client_fd;  /* server_fd = main server, client_fd = for each request */
struct sockaddr_in addr;   /* structure to store IP + port */
char buffer[30000], request[1024];  /* buffer = response, request = incoming data */

/* creating socket */
server_fd = socket(AF_INET, SOCK_STREAM, 0);  /* AF_INET = IPv4, SOCK_STREAM = TCP connection */

/* setting address details */
addr.sin_family = AF_INET;  /* using IPv4 */
addr.sin_port = htons(PORT);  /* converting port number to network format */
addr.sin_addr.s_addr = INADDR_ANY;  /* accept from any IP (localhost or others) */

/* binding socket with address */
bind(server_fd, (struct sockaddr*)&addr, sizeof(addr));  /* attach socket to port */

/* start listening for connections */
listen(server_fd, 5);  /* max 5 clients in queue */

printf("QNX RUNNING...\n");  /* just to confirm server started */

while(1){

pthread_mutex_lock(&lock);  /* locking because we update shared game info */

strcpy(game.active_thread,"NETWORK");  /* just for UI to show current thread */
game.network_load = rand()%100;        /* random value just to display load */

pthread_mutex_unlock(&lock);  /* unlock after updating */

/* wait for client (browser) request */
client_fd = accept(server_fd, NULL, NULL);  /* this will wait until request comes */

/* receive request data */
int len = recv(client_fd, request, sizeof(request)-1, 0);  /* reading HTTP request */

if(len <= 0){  /* if nothing received or error */
    close(client_fd);  /* close connection */
    continue;          /* skip this loop */
}

request[len] = '\0';   /* important: making request string properly end */

/* checking which request came */

/* start game request */
if(strncmp(request, "GET /start", 10) == 0){
    init_game();  /* reset everything */
}

/* move left (only if auto OFF) */
else if(strncmp(request, "GET /left", 9) == 0 && !game.auto_defense){
    game.player -= 20;  /* move player left */
}

/* move right */
else if(strncmp(request, "GET /right", 10) == 0 && !game.auto_defense){
    game.player += 20;  /* move player right */
}

/* shoot bullet */
else if(strncmp(request, "GET /shoot", 10) == 0){
    for(int i=0;i<MAX_LASERS;i++){
        if(!game.lasers[i].active){  /* find empty bullet slot */
            game.lasers[i].active = 1;  /* activate bullet */
            game.lasers[i].x = game.player;  /* start from player */
            game.lasers[i].y = 430;          /* fixed y near player */
            break;  /* stop after one bullet */
        }
    }
}

/* turn auto ON */
else if(strncmp(request, "GET /auto_on", 12) == 0){
    game.auto_defense = 1;  /* AI takes control */
    printf("AUTO ON\n");    /* debug print */
}

/* turn auto OFF */
else if(strncmp(request, "GET /auto_off", 13) == 0){
    game.auto_defense = 0;  /* manual control */
    printf("AUTO OFF\n");
}

/* enable dodge */
else if(strncmp(request, "GET /dodge_on", 13) == 0){
    game.smart_dodge = 1;
}

/* disable dodge */
else if(strncmp(request, "GET /dodge_off", 14) == 0){
    game.smart_dodge = 0;
}

/* now sending response back to frontend */

sprintf(buffer,
"HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nAccess-Control-Allow-Origin: *\r\n\r\n"  /* HTTP header */
"{\"player\":%.2f,\"score\":%d,\"lives\":%d,\"game_over\":%d,"  /* basic info */
"\"enemy1x\":%.2f,\"enemy1y\":%.2f,"  /* enemy 1 */
"\"enemy2x\":%.2f,\"enemy2y\":%.2f,"  /* enemy 2 */
"\"enemy3x\":%.2f,\"enemy3y\":%.2f,"  /* enemy 3 */
"\"bossx\":%.2f,\"bossy\":%.2f,\"boss\":%d,\"boss_health\":%d,"  /* boss info */
"\"explosion_x\":%d,\"explosion_y\":%d,\"explosion\":%d,"  /* explosion */
"\"lasers\":[{\"x\":%.2f,\"y\":%.2f,\"active\":%d},{\"x\":%.2f,\"y\":%.2f,\"active\":%d}],"  /* player bullets */
"\"eLasers\":[{\"x\":%.2f,\"y\":%.2f,\"active\":%d},{\"x\":%.2f,\"y\":%.2f,\"active\":%d}],"  /* enemy bullets */
"\"auto_defense\":%d,\"smart_dodge\":%d,"  /* AI settings */
"\"active\":\"%s\",\"threat\":\"%s\"}",  /* status info */

game.player,game.score,game.lives,game.game_over,
game.enemies[0].x,game.enemies[0].y,
game.enemies[1].x,game.enemies[1].y,
game.enemies[2].x,game.enemies[2].y,
game.boss_x,game.boss_y,game.boss_active,game.boss_health,
game.explosion_x,game.explosion_y,game.explosion_active,
game.lasers[0].x,game.lasers[0].y,game.lasers[0].active,
game.lasers[1].x,game.lasers[1].y,game.lasers[1].active,
game.eLasers[0].x,game.eLasers[0].y,game.eLasers[0].active,
game.eLasers[1].x,game.eLasers[1].y,game.eLasers[1].active,
game.auto_defense,game.smart_dodge,
game.active_thread,game.threat
);  /* converting game data into JSON format so frontend can use it */

send(client_fd, buffer, strlen(buffer), 0);  /* sending data to browser */

close(client_fd);  /* closing connection after response */
}
}
