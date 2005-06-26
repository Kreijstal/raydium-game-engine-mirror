#ifndef _ODE_NET_H
#define _ODE_NET_H
extern int raydium_ode_network_MaxElementsPerPacket (void);
extern int raydium_network_nid_element_find (int nid);
extern void raydium_ode_network_newdel_event (int type, char *buff);
extern void raydium_ode_network_nidwho_event (int type, char *buff);
extern void raydium_ode_network_explosion_event (int type, char *buff);
extern void raydium_ode_network_init (void);
extern char raydium_ode_network_TimeToSend (void);
extern void raydium_ode_network_element_send (short nelems, int *e);
extern void raydium_ode_network_element_send_all (void);
extern void raydium_ode_network_element_send_random (int nelems);
extern void raydium_ode_network_element_send_iterative (int nelems);
extern void raydium_ode_network_nidwho (int nid);
extern void raydium_ode_network_apply (raydium_ode_network_Event * ev);
extern void raydium_ode_network_read (void);
extern void raydium_ode_network_element_new (int e);
extern void raydium_ode_network_element_delete (int e);
extern void raydium_ode_network_explosion_send (raydium_ode_network_Explosion * exp);
extern char raydium_ode_network_element_isdistant (int elem);
extern char raydium_ode_network_element_isdistant_name (char *elem);
extern char raydium_ode_network_element_distantowner(int elem);
extern char raydium_ode_network_element_distantowner_name(char *elem);
extern void raydium_ode_network_element_trajectory_correct (int elem);
#endif
