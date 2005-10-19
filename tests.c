/*
    Raydium - CQFD Corp.
    http://raydium.org/
    License: GPL - GNU General Public License, see "gpl.txt" file.
*/

// This is test6 mini server. Consider this as a "base-code" for all
// RayODE network apps.

#define RAYDIUM_NETWORK_ONLY
#include "raydium/index.c"

char str[RAYDIUM_NETWORK_PACKET_SIZE];
//char str_send[RAYDIUM_NETWORK_PACKET_SIZE];
//int freq;

void infos(void)
{
static int steps=0;
if(steps==10)
    {
    raydium_network_internal_server_delays_dump();
    steps=0;
    }
steps++;
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
char type;
int id;

if(raydium_network_read_flushed(&id, &type, str)==RAYDIUM_NETWORK_DATA_OK)
    {
    raydium_network_broadcast(type,str);
//    printf(".");
    }
raydium_timecall_callback();
}


int main(int argc, char **argv)
{
setbuf(stdout,NULL);
signal(SIGINT,quit);
raydium_network_init();
raydium_network_server_create();

// Ugly hack, for now...
raydium_network_netcall_add(broad,RAYDIUM_NETWORK_PACKET_ODE_EXPLOSION,1);
raydium_network_netcall_add(broad,RAYDIUM_NETWORK_PACKET_ODE_NEWELEM,1);
raydium_network_netcall_add(broad,RAYDIUM_NETWORK_PACKET_ODE_REMELEM,1);
raydium_network_netcall_add(broad,RAYDIUM_NETWORK_PACKET_ODE_NIDWHO,1);
raydium_network_netcall_add(broad,(RAYDIUM_NETWORK_PACKET_BASE+1),1);
raydium_network_netcall_add(broad,(RAYDIUM_NETWORK_PACKET_BASE+2),1);

raydium_timecall_add(infos,1);

do{
loop();
//usleep(1); // MUST disable this with real servers
}while(1);

}
