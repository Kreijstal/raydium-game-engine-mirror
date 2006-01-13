// This is ManiaDrive mini server.

#define RAYDIUM_NETWORK_ONLY
#include "raydium/index.c"

#define PARTY_TIMEOUT   (5*60)
#define REMAINING_SEND	5


#define NET_SCORE_TRACK	(RAYDIUM_NETWORK_PACKET_BASE+1)
#define NET_RESTART	(RAYDIUM_NETWORK_PACKET_BASE+2)
#define NET_CHANGE_MAP	(RAYDIUM_NETWORK_PACKET_BASE+3)
#define NET_REMAINING	(RAYDIUM_NETWORK_PACKET_BASE+4)
#define NET_SCORE_BASE	(RAYDIUM_NETWORK_PACKET_BASE+10)


char str[RAYDIUM_NETWORK_PACKET_SIZE];
char track[RAYDIUM_NETWORK_PACKET_SIZE];
float steps=0;
//char str_send[RAYDIUM_NETWORK_PACKET_SIZE];
//int freq;



void switch_track(void)
{
static int last=-1;

raydium_register_variable(track,RAYDIUM_REGISTER_STR,"track");
raydium_register_variable(&last,RAYDIUM_REGISTER_INT,"last");

raydium_php_exec("mania_server_tracks.php");

raydium_register_variable_unregister_last();
raydium_register_variable_unregister_last();
raydium_log("Switching to track '%s'",track);
}

void change_track(char *track)
{
char buff[RAYDIUM_NETWORK_PACKET_SIZE];

raydium_log("Broadcasting MNI track '%s'",track);
strcpy(buff+RAYDIUM_NETWORK_PACKET_OFFSET,track);
raydium_network_broadcast(NET_CHANGE_MAP,buff);
}


void send_time(GLfloat step)
{
char buff[RAYDIUM_NETWORK_PACKET_SIZE];
static float t=0;
float tt;

t+=step;
if(t>=REMAINING_SEND)
    {
    t=0;
    tt=PARTY_TIMEOUT-steps;
    //raydium_log("remaining time : broadcast (%.2f)",tt);
    memcpy(buff+RAYDIUM_NETWORK_PACKET_OFFSET,&tt,sizeof(float));
    strcpy(buff+RAYDIUM_NETWORK_PACKET_OFFSET+sizeof(float),track);
    raydium_network_broadcast(NET_REMAINING,buff);
    }
}


/*
void newClient(int client)
{
char buff[RAYDIUM_NETWORK_PACKET_SIZE];
float tt=0;
memcpy(buff+RAYDIUM_NETWORK_PACKET_OFFSET,&tt,sizeof(float));
strcpy(buff+RAYDIUM_NETWORK_PACKET_OFFSET+sizeof(float),track);
raydium_network_write(&raydium_network_client_addr[client],-1,NET_REMAINING,buff);
}
*/

void change_track_netcall(GLfloat step)
{
steps+=step;
//printf("%f\n",steps);
if(steps>=PARTY_TIMEOUT)
    {
    steps=0;
    // change map
    switch_track();
    change_track(track);
    }
}


void quit(int sig)
{
raydium_network_internal_dump();
exit(0);
}

void broad(int type,char *buff)
{
    raydium_network_broadcast(type,buff);
//    raydium_log("broadcasting %i",type);
}


void loop(void)
{
signed char type;
int id;

if(raydium_network_read(&id, &type, str)==RAYDIUM_NETWORK_DATA_OK)
    {
    raydium_network_broadcast(type,str);
//    printf(".");
    }
raydium_timecall_callback();
raydium_web_callback();
}


int main(int argc, char **argv)
{
int i;

setbuf(stdout,NULL);
signal(SIGINT,quit);
raydium_php_init();
raydium_network_init();
raydium_network_server_create();

raydium_web_init();
raydium_web_start("ManiaDrive Server");
raydium_web_extension_add("mni", "raw/unknown");

// Ugly hack, for now...
raydium_network_netcall_add(broad,RAYDIUM_NETWORK_PACKET_ODE_EXPLOSION,1);
raydium_network_netcall_add(broad,RAYDIUM_NETWORK_PACKET_ODE_NEWELEM,1);
raydium_network_netcall_add(broad,RAYDIUM_NETWORK_PACKET_ODE_REMELEM,1);
raydium_network_netcall_add(broad,RAYDIUM_NETWORK_PACKET_ODE_NIDWHO,1);
raydium_network_netcall_add(broad,NET_SCORE_TRACK,1);
raydium_network_netcall_add(broad,NET_RESTART,1);
for(i=0;i<RAYDIUM_NETWORK_MAX_CLIENTS;i++)
    raydium_network_netcall_add(broad,NET_SCORE_BASE+i,1);


raydium_timecall_add(change_track_netcall,-1);
raydium_timecall_add(send_time,-1);

switch_track();

do{
loop();
usleep(1); // MUST disable this with real servers
}while(1);

}
