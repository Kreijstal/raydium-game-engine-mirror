/*
    Raydium - CQFD Corp.
    http://raydium.cqfd-corp.org
    License: GPL - GNU General Public License, see "gpl.txt" file.
*/

#ifndef DONT_INCLUDE_HEADERS
#include "index.h"
#else
#include "headers/network.h"
#endif

// Raydium low-level Network API.

// TODO:
// 1 - protection against duplicate data packets (how ?!)
// 01/07/2004: done ? (need more tests)
// 2 - per client delays for servers
// end of 2004: done too (more test needed)

// need proto
char raydium_server_accept_new(struct sockaddr *from, char *name);
void raydium_network_broadcast(char type,char *buff);
void raydium_network_write(struct sockaddr *to, int from, char type,char *buff);
char raydium_network_netcall_add(void *ptr, int type, char tcp);


int raydium_network_propag_find(int type)
{
int i;

for(i=0;i<RAYDIUM_NETWORK_MAX_PROPAGS;i++)
  if(raydium_network_propag[i].state && raydium_network_propag[i].type==type)
	return i;
return -1;
}


void raydium_network_propag_recv(int type, char *buff)
{
int dec;
unsigned int version;
int i;

i=raydium_network_propag_find(type);
if(i<0)
    {
    raydium_log("network: ERROR: received an invalid propag' type ! (%i)",type);
    return;
    }

dec=RAYDIUM_NETWORK_PACKET_OFFSET;
memcpy(&version,buff+dec,sizeof(int));
if(version>raydium_network_propag[i].version) // this propag is newer than our
    {
    dec+=sizeof(int);    
    raydium_network_propag[i].version=version;    
    memcpy(raydium_network_propag[i].data,buff+dec,raydium_network_propag[i].size);
    }
}


void raydium_network_propag_refresh_id(int i)
{
char buff[RAYDIUM_NETWORK_PACKET_SIZE];
int dec;

if(raydium_network_mode==RAYDIUM_NETWORK_MODE_NONE) return;

if(i<0 || i>=RAYDIUM_NETWORK_MAX_PROPAGS || !raydium_network_propag[i].state)
    {
    raydium_log("network: ERROR: cannot refresh this propag': invalid id");
    return;
    }

raydium_network_propag[i].version++;
dec=RAYDIUM_NETWORK_PACKET_OFFSET;
memcpy(buff+dec,&raydium_network_propag[i].version,sizeof(int));
dec+=sizeof(int);
memcpy(buff+dec,raydium_network_propag[i].data,raydium_network_propag[i].size);
raydium_network_write(NULL,raydium_network_uid,raydium_network_propag[i].type,buff);
}


void raydium_network_propag_refresh(int type)
{
int i;

i=raydium_network_propag_find(type);
if(i<0)
    {
    raydium_log("network: ERROR: cannot refresh this propag': invalid type");
    return;
    }
raydium_network_propag_refresh_id(i);
}


void raydium_network_propag_refresh_all(void)
{
int i;

for(i=0;i<RAYDIUM_NETWORK_MAX_PROPAGS;i++)
  if(raydium_network_propag[i].state)
    raydium_network_propag_refresh_id(i);
}

int raydium_network_propag_add(int type, void *data, int size)
{
int i;

if(size>(RAYDIUM_NETWORK_PACKET_SIZE-RAYDIUM_NETWORK_PACKET_OFFSET-RAYDIUM_NETWORK_PROPAG_HEAD))
    {
    raydium_log("network: ERROR: propag' packet is too big for current network size");
    return -1;
    }

if(raydium_network_propag_find(type)>=0)
    {
    raydium_log("network: ERROR: propag' type already added !");
    return -1;
    }
    
for(i=0;i<RAYDIUM_NETWORK_MAX_PROPAGS;i++)
    if(!raydium_network_propag[i].state)
	{
	raydium_network_propag[i].state=1;
	raydium_network_propag[i].version=0;
	raydium_network_propag[i].type=type;
	raydium_network_propag[i].size=size;
	raydium_network_propag[i].data=data;
	raydium_network_netcall_add(raydium_network_propag_recv,type,1);
	return i;	
	}
raydium_log("network: ERROR: no more propag' slots !");
return -1;
}


void raydium_network_queue_element_init(raydium_network_Tcp *e)
{
e->state=0;
}


unsigned short raydium_network_queue_tcpid_gen(void)
{
// not very important.. used for easy client/server tcpid identification
//#ifdef RAYDIUM_NETWORK_ONLY_____
static unsigned short gen=100;
//#else
//static unsigned short gen=1000;
//#endif

gen++;
if(!gen) gen++;

return gen;
}


void raydium_network_queue_tcpid_known_add(int tcpid, int player)
{
	raydium_network_tcpid_i[raydium_network_tcpid_index]=tcpid; // mark this TCP-ID as "known"
	raydium_network_tcpid_p[raydium_network_tcpid_index]=player; // ... from this player
#ifdef DEBUG_NETWORK
	raydium_log("ACK adding tcpid=%i (player %i) to known packets",tcpid,player);
#endif	
	raydium_network_tcpid_index++;

	if(raydium_network_tcpid_index==RAYDIUM_NETWORK_TX_QUEUE_SIZE)
	    raydium_network_tcpid_index=0;
}

char raydium_network_queue_tcpid_known(unsigned short tcpid, unsigned short player)
{
int i;

if(!tcpid) return 0;

for(i=0;i<RAYDIUM_NETWORK_TX_QUEUE_SIZE;i++)
  if(raydium_network_tcpid_i[i]==tcpid &&
     raydium_network_tcpid_p[i]==player )
    return i;

return 0;
}

char raydium_network_queue_is_tcpid(int type)
{
int i=0;
for(i=0;i<RAYDIUM_NETWORK_MAX_NETCALLS;i++)
    if(raydium_network_netcall_type[i]==type && raydium_network_netcall_tcp[i])
	return 1;
return 0;
}


void raydium_network_queue_element_add(char *packet, struct sockaddr *to)
{
unsigned short tcpid;
raydium_network_Tcp *e;
memcpy(&tcpid,packet+2,sizeof(tcpid));


e=&raydium_network_queue[raydium_network_queue_index];

if(e->state)
    {
    raydium_network_queue_element_init(e);
    raydium_network_stat_lost++; // we're erasing an old waiting packet !
    }

e->state=1;
e->tcpid=tcpid;
memcpy(e->packet,packet,RAYDIUM_NETWORK_PACKET_SIZE);
e->time=raydium_timecall_clock();
e->retries_left=RAYDIUM_NETWORK_MAX_TRIES;
if(to) memcpy(&e->to,to,sizeof(struct sockaddr));
e->to_player=-1;

// if server mode, search for destination player
if(raydium_network_mode==RAYDIUM_NETWORK_MODE_SERVER)
    {
    int i;
    
    for(i=0;i<RAYDIUM_NETWORK_MAX_CLIENTS;i++)
	if(raydium_network_client[i] && &raydium_network_client_addr[i]==to)
	    break;
    
    if(i==RAYDIUM_NETWORK_MAX_CLIENTS) // not found
	{
	raydium_log("ERROR: server: TCP style: cannot find client");
	return;
	}
    e->to_player=i;
    }

raydium_network_queue_index++;
if(raydium_network_queue_index==RAYDIUM_NETWORK_TX_QUEUE_SIZE)
    raydium_network_queue_index=0;
}


unsigned long *raydium_network_internal_find_delay_addr(int player)
{
//if client
if(raydium_network_mode==RAYDIUM_NETWORK_MODE_CLIENT)
    return &raydium_netwok_queue_ack_delay_client;
else // (server)
    {
    if(player<0 || player>=RAYDIUM_NETWORK_MAX_CLIENTS)
	{
	raydium_log("ERROR: server: bad client id in resend queue ! SHOULD NEVER APPEND !");
	return NULL; // eeeerk !
	}
    return &raydium_netwok_queue_ack_delay_server[player];
    }
}


void raydium_network_queue_check_time(void)
{
unsigned long now;
unsigned long *delay;
raydium_network_Tcp *e;
int i;


// find current delay

for(i=0;i<RAYDIUM_NETWORK_TX_QUEUE_SIZE;i++)
  if(raydium_network_queue[i].state)
    {
    now=raydium_timecall_clock();
    e=&raydium_network_queue[i];

    delay=raydium_network_internal_find_delay_addr(e->to_player);
    
    // TCP style timeout: timeout=estimatedRTT*2
    if( now>(e->time+(*delay)*2) || e->time>now )
	{
	// resend packet (and do not add this packet to queue again !)
#ifdef DEBUG_NETWORK
	raydium_log("ACK re-asking: tcpid=%i",e->tcpid);
#endif
	raydium_network_write_notcp=1;
	raydium_network_write(&e->to,-1,e->packet[0],e->packet);
	raydium_network_stat_reemitted++;
	(*delay)*=2; // Karn/Partridge TCP algo
        if((*delay)/(double)raydium_timecall_clocks_per_sec>RAYDIUM_NETWORK_ACK_DELAY_MAX)
	    {
#ifdef DEBUG_NETWORK
	    raydium_log("ACK: slow network ! max ack delay reached");
#endif
	    (*delay)=raydium_timecall_clocks_per_sec*RAYDIUM_NETWORK_ACK_DELAY_MAX;
	    }
	e->retries_left--;
	e->time=now;
	if(e->retries_left==0) 
	    {
#ifdef DEBUG_NETWORK
	    raydium_log("ACK: packet lost, too many retries: tcpid=%i",e->tcpid);
#endif
	    raydium_network_queue_element_init(e);
	    raydium_network_stat_lost++;
	    }
	}
    }
}

void raydium_network_queue_ack_send(unsigned short tcpid, struct sockaddr *to)
{
char buff[RAYDIUM_NETWORK_PACKET_SIZE];
//unsigned short port;

memcpy(buff+RAYDIUM_NETWORK_PACKET_OFFSET,&tcpid,sizeof(tcpid));
raydium_network_write(to,raydium_network_uid,RAYDIUM_NETWORK_PACKET_ACK,buff);

//memcpy(&port,&to->sa_data[0],sizeof(port));
//raydium_log("ACK ---> %i.%i.%i.%i:%i",to->sa_data[2],to->sa_data[3],to->sa_data[4],to->sa_data[5],ntohs(port));
}


void raydium_network_queue_ack_recv(int type,char *buff)
{
unsigned short tcpid;
int i;
raydium_network_Tcp *e;
unsigned long now;
// TCP style weight average params
const float a=0.85;
const float b=0.15;

memcpy(&tcpid,buff+RAYDIUM_NETWORK_PACKET_OFFSET,sizeof(tcpid));

for(i=0;i<RAYDIUM_NETWORK_TX_QUEUE_SIZE;i++)
    {
    e=&raydium_network_queue[i];
    if(e->state && e->tcpid==tcpid)
	{
	// ACK is correct, deleting packet from queue
#ifdef DEBUG_NETWORK
	raydium_log("ACK recv ok: tcpid=%i",tcpid);
#endif
	now=raydium_timecall_clock();
	if(e->time<now)
	    {
	    unsigned long *delay;
	    
	    delay=raydium_network_internal_find_delay_addr(e->to_player);
	    // Based on original TCP adaptative retransmission algorithm :
	    *(delay)=a * (*delay) + b*(now - e->time);
#ifdef DEBUG_NETWORK
	    raydium_log("ACK delay re-eval: %.2f msec (inst=%.2f msec) (client %i)",(*delay)/(double)raydium_timecall_clocks_per_sec*1000,(now - e->time)/(double)raydium_timecall_clocks_per_sec*1000,e->to_player);
#endif	
	    }
	raydium_network_queue_element_init(e);	    
	return;
	}    
    }
raydium_network_stat_bogus_ack++;
#ifdef DEBUG_NETWORK
raydium_log("ACK bogus (double ack, probably): tcpid=%i",tcpid);
#endif
}


// -----------------------------------------------------------------------


void raydium_network_player_name(char *str)
{
#ifndef WIN32
{
struct passwd *pn;
pn=getpwuid(geteuid()); // ("old" kernels ok ?)
strncpy(str,pn->pw_name,RAYDIUM_MAX_NAME_LEN-1);
}
#else
{
DWORD s;
s=RAYDIUM_MAX_NAME_LEN-1;
GetUserName(str,&s);
}
#endif

if(!strlen(str))
    gethostname(str,RAYDIUM_MAX_NAME_LEN-1);
}

char raydium_network_set_socket_block(int block)
{
int ret=-1;
//int flags = fcntl(sock, F_GETFL); // bof... :)
#ifndef WIN32
if(!block) block=O_NONBLOCK; else block=0;
ret=fcntl(raydium_network_socket, F_SETFL, block);
#else
if(!block) block=1; else block=0;
ret = ioctlsocket(raydium_network_socket, FIONBIO, (unsigned long *)&block);
if(ret) ret=-1;
#endif

if(ret==-1)
    {
    raydium_log("ERROR ! network: cannot block/unblock socket");
    perror("System");
    return(0);
    }
return(1);
}

//int raydium_network_get_socket_block(void)
//{
//return !( fcntl(raydium_network_socket, F_GETFL) & O_NONBLOCK );
//}


char raydium_network_netcall_add(void *ptr, int type, char tcp)
{
int i;

for(i=0;i<RAYDIUM_NETWORK_MAX_NETCALLS;i++)
 if(raydium_network_netcall_type[i]<0)
 {
  raydium_network_netcall_func[i]=ptr;
  raydium_network_netcall_type[i]=type;
  raydium_network_netcall_tcp[i]=tcp;
  break;
 }

if(i==RAYDIUM_NETWORK_MAX_NETCALLS)
 {
  raydium_log("network: ERROR: no more netcalls !");
  return 0;
 }

return 1;
}


void raydium_network_netcall_exec(int type,char *buff)
{
char tmpbuff[RAYDIUM_NETWORK_PACKET_SIZE];
int i;
void (*f)(int, char*);

for(i=0;i<RAYDIUM_NETWORK_MAX_NETCALLS;i++)
 if(raydium_network_netcall_type[i]==type)
 {
    memcpy(tmpbuff,buff,RAYDIUM_NETWORK_PACKET_SIZE);
    f=raydium_network_netcall_func[i];
    f(type,tmpbuff);
 }
}

char raydium_network_timeout_check(void)
{
char str[RAYDIUM_NETWORK_PACKET_SIZE];
int i,n;
time_t now;
void (*f)(int);

time(&now);

 if(raydium_network_mode==RAYDIUM_NETWORK_MODE_SERVER)
 {
  for(i=n=0;i<RAYDIUM_NETWORK_MAX_CLIENTS;i++)
    if(raydium_network_client[i] && now>raydium_network_keepalive[i]+RAYDIUM_NETWORK_TIMEOUT)
    {
    raydium_log("network: TIMEOUT for client %i (%i sec): %s deleted !",i,RAYDIUM_NETWORK_TIMEOUT,raydium_network_name[i]);
    raydium_network_client[i]=0;
    if(raydium_network_on_disconnect)
       {
       f=raydium_network_on_disconnect;
       f(i);
       }
    raydium_network_name[i][0]=0;
    str[RAYDIUM_NETWORK_PACKET_OFFSET]=i;
    str[RAYDIUM_NETWORK_PACKET_OFFSET+1]=0;
    raydium_network_broadcast(RAYDIUM_NETWORK_PACKET_INFO_NAME,str);
    n++;
    }
 return n;
 }
return 0;
}

char raydium_network_init(void)
{
int i;

#ifdef WIN32
int ret;
WSADATA WSAData;
ret = WSAStartup(MAKEWORD(1,1), &WSAData);
if (ret) { raydium_log("network: FAILED ! (Winsock 2 Error: %i while asking for version 1.1)",ret); return(0); }
#endif

#ifdef RAYDIUM_NETWORK_ONLY
// Do all needed inits for network only mode:
raydium_timecall_init();
#endif

if(raydium_network_mode) close(raydium_network_socket);
raydium_network_uid=-1;
raydium_network_mode=RAYDIUM_NETWORK_MODE_NONE;
raydium_network_socket=-1;

for(i=0;i<RAYDIUM_NETWORK_MAX_CLIENTS;i++)
    {
    raydium_network_client[i]=0;
    raydium_network_name[i][0]=0;
    }

for(i=0;i<RAYDIUM_NETWORK_MAX_NETCALLS;i++)
    {
    raydium_network_netcall_type[i]=-1;
    raydium_network_netcall_func[i]=0;
    raydium_network_netcall_tcp[i]=0;
    }

for(i=0;i<RAYDIUM_NETWORK_TX_QUEUE_SIZE;i++)
    raydium_network_queue_element_init(&raydium_network_queue[i]);

for(i=0;i<RAYDIUM_NETWORK_TX_QUEUE_SIZE;i++)
    {
    raydium_network_tcpid_i[i]=0;
    raydium_network_tcpid_p[i]=0;
    }

for(i=0;i<RAYDIUM_NETWORK_MAX_PROPAGS;i++)
    raydium_network_propag[i].state=0;

raydium_network_queue_index=0;
raydium_network_tcpid_index=0;

raydium_network_on_connect=raydium_network_on_disconnect=NULL;
raydium_network_stat_rx=0;
raydium_network_stat_tx=0;
raydium_network_stat_lost=0;
raydium_network_stat_double=0;
raydium_network_stat_reemitted=0;
raydium_network_stat_bogus_ack=0;
raydium_network_netcall_add(raydium_network_queue_ack_recv,RAYDIUM_NETWORK_PACKET_ACK,0);
raydium_netwok_queue_ack_delay_client=raydium_timecall_clocks_per_sec; // 1sec default delay

for(i=0;i<RAYDIUM_NETWORK_MAX_CLIENTS;i++)
    raydium_netwok_queue_ack_delay_server[i]=raydium_timecall_clocks_per_sec; // 1sec default delay
    
raydium_network_write_notcp=0;

raydium_network_name_local[0]=0;
#ifndef RAYDIUM_NETWORK_ONLY
if(raydium_init_cli_option("name",raydium_network_name_local))
{
    if(strlen(raydium_network_name_local)==0)
	raydium_log("Warning: network: --name option needs an argument");
}
#endif

if(strlen(raydium_network_name_local)==0)
    raydium_network_player_name(raydium_network_name_local);


raydium_log("network: OK");
return(1);
}

// if from < 0, "from" field is not modified (used for broadcasts, mainly)
void raydium_network_write(struct sockaddr *to, int from, char type,char *buff)
{
int ret=-1;
unsigned short tcpid=0;

buff[0]=type;

if(from>=0)
    buff[1]=(char)from; // 256 clients MAX for now.

if(raydium_network_write_notcp==0 && raydium_network_queue_is_tcpid(type))
    tcpid=raydium_network_queue_tcpid_gen();
    
if(raydium_network_write_notcp==0) // do not erase tcpid packet's element if it's a re-send
    memcpy(buff+2,&tcpid,sizeof(unsigned short));

raydium_network_write_notcp=0;

raydium_network_stat_tx+=RAYDIUM_NETWORK_PACKET_SIZE;
if(raydium_network_mode==RAYDIUM_NETWORK_MODE_CLIENT)
    ret=send(raydium_network_socket, buff, RAYDIUM_NETWORK_PACKET_SIZE, 0);

else if(raydium_network_mode==RAYDIUM_NETWORK_MODE_SERVER)
    ret=sendto(raydium_network_socket, buff, RAYDIUM_NETWORK_PACKET_SIZE, 0, to, sizeof(struct sockaddr));

raydium_network_timeout_check();
//raydium_log("send ret: %i",ret);
if(ret<0)
    {
    raydium_log("network: ERROR sending ! (%i)",ret);
    return;
    }

if(tcpid)
    {
    raydium_network_queue_element_add(buff,to);
#ifdef DEBUG_NETWORK
    raydium_log("ACK asking to peer: tcpid=%i type=%i",tcpid,type);
#endif
    }    
}

void raydium_network_broadcast(char type,char *buff)
{
int i;

for(i=0;i<RAYDIUM_NETWORK_MAX_CLIENTS;i++)
 if(raydium_network_client[i])
  raydium_network_write(&raydium_network_client_addr[i],-1,type,buff);
}

// buff could be modified, even if return is RAYDIUM_NETWORK_DATA_NONE
char raydium_network_read(int *id, char *type, char *buff)
{
int i;
struct sockaddr from;
socklen_t len;
int ret;
char dbl=0;

raydium_network_timeout_check();
raydium_network_queue_check_time();

len=sizeof(struct sockaddr);
ret=recvfrom(raydium_network_socket,buff,RAYDIUM_NETWORK_PACKET_SIZE,0,&from,&len);

if(ret==RAYDIUM_NETWORK_PACKET_SIZE) 
 {
 unsigned short tcpid;
 
 *type=buff[0];
 *id=buff[1];
 raydium_network_stat_rx+=RAYDIUM_NETWORK_PACKET_SIZE;


 memcpy(&tcpid,buff+2,sizeof(unsigned short));
 if(tcpid)
    {
#ifdef DEBUG_NETWORK
    raydium_log("ACK read (and will send): tcpid=%i type=%i",tcpid,*type);
#endif
    if(raydium_network_queue_tcpid_known(tcpid,*id))
	{
	raydium_network_stat_double++;
	dbl=1;
#ifdef DEBUG_NETWORK
	raydium_log("ACK double: tcpid=%i type=%i",tcpid,*type);
#endif
	}
    /*else*/ raydium_network_queue_ack_send(tcpid,&from);
     raydium_network_queue_tcpid_known_add(tcpid,buff[1]);
    }
 
 if(dbl) // discard double packet
    return(RAYDIUM_NETWORK_DATA_NONE);
 
 raydium_network_netcall_exec(*type,buff);
 
 if( raydium_network_mode==RAYDIUM_NETWORK_MODE_SERVER && (*id>=0) && (*id<RAYDIUM_NETWORK_MAX_CLIENTS) )
    time(&raydium_network_keepalive[(*id)]); // update keepalive
 
 if(*type==RAYDIUM_NETWORK_PACKET_REQUEST_UID && raydium_network_mode==RAYDIUM_NETWORK_MODE_SERVER)
    {
    raydium_server_accept_new(&from,buff+RAYDIUM_NETWORK_PACKET_OFFSET);
    return(RAYDIUM_NETWORK_DATA_NONE);
    }

 if(*type==RAYDIUM_NETWORK_PACKET_ACK)
    return(RAYDIUM_NETWORK_DATA_NONE);

 if(*type==RAYDIUM_NETWORK_PACKET_INFO_NAME && raydium_network_mode==RAYDIUM_NETWORK_MODE_CLIENT)
    {
    i=buff[RAYDIUM_NETWORK_PACKET_OFFSET];
    strcpy(raydium_network_name[i],buff+RAYDIUM_NETWORK_PACKET_OFFSET+1);
    raydium_log("network: client %i is %s",i,raydium_network_name[i]);
    if(strlen(raydium_network_name[i]))
	raydium_network_propag_refresh_all(); // spread propags to this new client
    return(RAYDIUM_NETWORK_DATA_NONE);
    }


 return(RAYDIUM_NETWORK_DATA_OK);
 }
else if(errno==EAGAIN) return(RAYDIUM_NETWORK_DATA_NONE); // POSIX
else { 
#ifdef WIN32
	ret=WSAGetLastError();
	if(ret==WSAEWOULDBLOCK) return(RAYDIUM_NETWORK_DATA_NONE); // NON POSIX (GRRrrr)
#else
//	perror("System");	
#endif
//	raydium_log("ERROR ! network: error receiving ! (%i)",ret);
	return(RAYDIUM_NETWORK_DATA_ERROR);
     }
}


char raydium_network_read_flushed(int *id, char *type, char *buff)
{
char ret,data=0;
char save_buff[RAYDIUM_NETWORK_PACKET_SIZE];
int save_id;
char save_type;

do
 {
 // erreur !: meme si le retour est NONE, buff peut avoir ete modifie ! 
 // (et donc, on le lit pas tt a fait le dernier paquet de donnes)
 // solution: sauver buff avant et le restauter au final ? (lent ?)
 // 13/04/2004: solution en question mise en place: en test, premier resultats
 // probants
 ret=raydium_network_read(id,type,buff);
 if(ret==RAYDIUM_NETWORK_DATA_OK) 
    {
    data++;
    memcpy(save_buff,buff,RAYDIUM_NETWORK_PACKET_SIZE);
    save_id=*id;
    save_type=*type;
    }
 }while(ret==RAYDIUM_NETWORK_DATA_OK);
 
if(data) 
    {
    memcpy(buff,save_buff,RAYDIUM_NETWORK_PACKET_SIZE);
    *id=save_id;
    *type=save_type;
    return RAYDIUM_NETWORK_DATA_OK;
    }
else return ret;
}


char raydium_network_server_create(void)
{
struct sockaddr_in sock;
int ret;

raydium_network_start=time(NULL);
raydium_network_socket=socket(AF_INET,RAYDIUM_NETWORK_UDP,0);
if(raydium_network_socket==-1)
    {
    raydium_log("ERROR ! network: cannot create server socket");
    perror("System");
    return(0);
    }
raydium_log("network: server socket created");

sock.sin_family=AF_INET;
sock.sin_addr.s_addr=htonl(INADDR_ANY);
sock.sin_port=htons(RAYDIUM_NETWORK_PORT);
ret=bind(raydium_network_socket,(struct sockaddr *)&sock,sizeof(sock));
if(ret)
    {
    raydium_log("ERROR ! network: cannot open server socket (already used ?)");
    perror("System");
    return(0);
    }

raydium_log("network: server OK: waiting for clients (%i max) at udp port %i",RAYDIUM_NETWORK_MAX_CLIENTS,RAYDIUM_NETWORK_PORT);
raydium_network_mode=RAYDIUM_NETWORK_MODE_SERVER;
raydium_network_set_socket_block(0);
return(1);
}


// will resolv "server" name
char raydium_network_client_connect_to(char *server)
{
struct sockaddr_in sock;
int ret,empty;
char str[RAYDIUM_NETWORK_PACKET_SIZE];
char type;
struct hostent *server_addr;

raydium_network_start=time(NULL);
raydium_network_socket=socket(AF_INET,RAYDIUM_NETWORK_UDP,0);
if(raydium_network_socket==-1)
    {
    raydium_log("ERROR ! network: cannot create client socket");
    perror("System");
    return(0);
    }
raydium_log("network: client socket created");


server_addr = gethostbyname(server);
if(!server_addr) 
    {
    raydium_log("ERROR ! DNS/resolv error with \"%s\"",server);
    perror("System");
    return(0);
    }

//inet_pton(AF_INET,server,&sock.sin_addr);
memcpy((char*)(&(sock.sin_addr.s_addr)), server_addr->h_addr, server_addr->h_length);
sock.sin_family=AF_INET;
sock.sin_port=htons(RAYDIUM_NETWORK_PORT);

ret=connect(raydium_network_socket,(struct sockaddr *)&sock,sizeof(sock));
if(ret) 
    {
    raydium_log("ERROR ! local UDP socket error (contacting %s)",server);
    perror("System");
    return(0);
    }
raydium_log("network: connecting to %s and waiting UID...",server);
raydium_network_set_socket_block(1);
// needed now, because we use network_write
raydium_network_mode=RAYDIUM_NETWORK_MODE_CLIENT;
// we need to send request for uid (and send our name)
strcpy(str+RAYDIUM_NETWORK_PACKET_OFFSET,raydium_network_name_local);
raydium_network_write(NULL,-1,RAYDIUM_NETWORK_PACKET_REQUEST_UID,str);


// probably needs timeouts, here ...
if (raydium_network_read(&empty,&type,str)!=RAYDIUM_NETWORK_DATA_OK)
    {
    raydium_network_mode=RAYDIUM_NETWORK_MODE_NONE;
    raydium_log("ERROR ! network: cannot connect to server %s",server);
    perror("System");
    return(0);
    }

if(type==RAYDIUM_NETWORK_PACKET_ATTRIB_UID)
    {
    raydium_network_uid=str[RAYDIUM_NETWORK_PACKET_OFFSET];
    raydium_log("network: accepted as client %i",raydium_network_uid);
    raydium_network_set_socket_block(0);
    return(1);
    }

if(type==RAYDIUM_NETWORK_PACKET_ERROR_NO_MORE_PLACE)
    {
    raydium_network_mode=RAYDIUM_NETWORK_MODE_NONE;
    raydium_log("ERROR ! network: no more room (server said: %s)",str+RAYDIUM_NETWORK_PACKET_OFFSET);
    return(0);
    }

raydium_network_mode=RAYDIUM_NETWORK_MODE_NONE;
raydium_log("ERROR ! network: unknow server message type (%i). abort.",type);
return(0);

}

char raydium_server_accept_new(struct sockaddr *from, char *name)
{
int socklen,i,n;
char str[RAYDIUM_NETWORK_PACKET_SIZE];
void (*f)(int);

socklen=sizeof(struct sockaddr);

 for(i=0,n=-1;i<RAYDIUM_NETWORK_MAX_CLIENTS;i++)
  if(!raydium_network_client[i]) {n=i; break;}

 if(n<0) 
    { 
    // no more room in this server
    sprintf(str+RAYDIUM_NETWORK_PACKET_OFFSET,"Server limited to %i client(s)",RAYDIUM_NETWORK_MAX_CLIENTS);
    raydium_network_write(from,-1,RAYDIUM_NETWORK_PACKET_ERROR_NO_MORE_PLACE,str);
    return(0);
    }
 
 memcpy(&raydium_network_client_addr[n],from,sizeof(struct sockaddr));
 raydium_network_client[n]=1;
 time(&raydium_network_keepalive[n]); // first keepalive
 strcpy(raydium_network_name[n],name);
 raydium_netwok_queue_ack_delay_server[n]=raydium_timecall_clocks_per_sec; // 1sec default delay
 
 raydium_log("network: client %i connected as %s"/*,inet_ntoa(from->sin_addr)*/,n,name);
 
 /* send uid to client */
 str[RAYDIUM_NETWORK_PACKET_OFFSET]=n;
 raydium_network_write(from,-1,RAYDIUM_NETWORK_PACKET_ATTRIB_UID,str);

 // OnConnect:
 for(i=0;i<RAYDIUM_NETWORK_MAX_CLIENTS;i++)
    if(i!=n && raydium_network_client[i])
    {
    strcpy(str+RAYDIUM_NETWORK_PACKET_OFFSET+1,raydium_network_name[i]);
    str[RAYDIUM_NETWORK_PACKET_OFFSET]=i;
    raydium_network_write(from,i,RAYDIUM_NETWORK_PACKET_INFO_NAME,str);
    }
 strcpy(str+RAYDIUM_NETWORK_PACKET_OFFSET+1,raydium_network_name[n]); // send name to all others...
 str[RAYDIUM_NETWORK_PACKET_OFFSET]=n;
 raydium_network_broadcast(RAYDIUM_NETWORK_PACKET_INFO_NAME,str);    
 
 if(raydium_network_on_connect)
    {
    f=raydium_network_on_connect;
    f(n);
    }
    
 return(n);
}

void raydium_network_close(void)
{
close(raydium_network_socket);
#ifdef WIN32
WSACleanup();
#endif
}


void raydium_network_internal_server_delays_dump(void)
{
int i;

raydium_log("Network server delays:");
for(i=0;i<RAYDIUM_NETWORK_MAX_CLIENTS;i++)
         if(raydium_network_client[i])
	    raydium_log("player %i : %.2f msec (%s)",
		i,
		raydium_netwok_queue_ack_delay_server[i]/(double)raydium_timecall_clocks_per_sec*1000,
		raydium_network_name[i]);
}

void raydium_network_internal_dump(void)
{
time_t diff;
diff=time(NULL)-raydium_network_start;
raydium_log("Network stats:");
raydium_log("Rx: %i byte(s) / Tx: %i bytes(s) / %.2f min",raydium_network_stat_rx,raydium_network_stat_tx,diff/60.f);
raydium_log("Transfert rates: Rx: %.2f KB/s / Tx: %.2f KB/s",raydium_network_stat_rx/(float)diff/1024.f,raydium_network_stat_tx/(float)diff/1024.f);
raydium_log("Packets (err): Tx: %i re-emitted, Rx: %i doubles",raydium_network_stat_reemitted,raydium_network_stat_double);
raydium_log("Packets (err): Tx: %i erased or lost, bogus ACK: %i",raydium_network_stat_lost,raydium_network_stat_bogus_ack);
}
