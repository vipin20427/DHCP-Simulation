#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <linux/if.h>



#define SERVICEPORT 7790
#define OFFERPORT 7791
#define REQUESTPORT 7792
#define ACKNOWLEGEMENTPORT 7793
#define BUFSIZE 2048

#define OP 1
#define HTYPE 2
#define HLEN 3
#define HOPS 4
#define XID 5
#define SEC 6
#define FLAGS 7
#define CIADDR 8
#define YIADDR 9
#define SIADDR 10
#define GIADDR 11
#define CHADDR 12
#define SNAME 13
#define FILE 14
#define OPTIONS 15

#define FIELD1 0
#define FIELD2 2
#define FIELD3 4
#define FIELD4 6
#define FIELD5 8
#define FIELD6 10
#define FIELD7 12
 




struct Packet
{
   /*Message op code / message type.1 = BOOTREQUEST, 2 = BOOTREPLY*/
   char op;

   /* Hardware address type (e.g., '1' = 10Mb Ethernet)*/
   char htype;

   /*Hardware address length (e.g. '6' for 10Mb Ethernet)*/
   char hlen;

   /*Client sets to zero, optionally used by relay agents when booting via a relay agent.*/
   char hops;

   /*Transaction ID.A random number chosen by the client, used by the
    client and server to associate the request message with it's and response.*/
   int xid;

   /*Seconds passed since client began the request process.Filled in by client.*/
   int sec;

   /*Flags*/
   int flags;

   /*Client IP address.Filled in by client if it knows it's IP address 
   (from previouse requests or from manual configurations). and can respond to ARP requests */
   char ciaddr[16];

   /*your' (client) IP address. Server's response to client.*/
   char yiaddr[16];

   /*Server IP address. Address of sending server or of the next server to use in the next bootstrap process step. */
   char siaddr[16];

   /*Relay agent IP address, used in booting via a relay agent.*/
   char giaddr[16];

   /*Client hardware address.*/
   char chaddr[16];

   /*Optional server host name. Null terminated string.*/
   char sname[65];

   /*Boot file name.Null terminated string; "generic" name or
    null in request, fully qualified directory-path name in reply.*/
   char file[129];
   
   /*Optional (BOOTP semantics) parameters field.In real DHCP messages at
    least one option (message type) must always be present, so this field is never empty.*/
   char options[512];
};

typedef struct Packet DHCPPacket;

/* DHCP_DISCOVER(DHCPPacket *packet):
   returns -1 if error 0 otherwise
   input : packet to be recieved from client
   method: connects to the client and recieves message.
*/
int dhcp_discover(DHCPPacket *packet);

/*
  DHCP_OFFER(DHCPPacket *dhcppacket):
  returns -1 if error or 0 otherwise
  input : null
  method: sends the packet from the server  to client and also the range of ip address
*/
int dhcp_offer(DHCPPacket *dhcppacket);

/* DHCP_REQUEST(DHCPPACKET *PACKET):
  return -1 if error or 0 otherwise		
  input : packet containing request 
*/
int dhcp_request(DHCPPacket *packet);


/* DHCP_ACKNOWLEGMENT(DHCPPACKET *PACKET):
  return -1 if error or 0 otherwise
  input : packet containing acknowlegement
*/
int dhcp_acknowlegment(DHCPPacket *packet);

/* to get the mac address of the machine */
void getMacAddress(char *macAddress);

/* get char equvivalent of int*/
char getCharOfNum(int num);

/*call Delay*/
void callDelay();

/*display Packet*/
void displayPacket(DHCPPacket *dhcppacket,char name[]);

int ip = 2;
