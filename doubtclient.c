#include "dhcpclient.h"
#define SERVER_IP "127.0.0.1"

/*transaction number value*/
int transactionNumber = 0;

int main(void)
{
   DHCPPacket dhcppacket;
   dhcp_discover(&dhcppacket);
   callDelay();
   displayPacket(&dhcppacket,"DHCP DISCOVER PACKET");
  
    dhcp_offer(&dhcppacket);
   callDelay();
   displayPacket(&dhcppacket,"DHCP OFFER PACKET");
   
   dhcp_request(&dhcppacket);	
   callDelay();
   displayPacket(&dhcppacket,"DHCP REQUEST PACKET");
   
   dhcp_acknowlegment(&dhcppacket);
   displayPacket(&dhcppacket,"DHCP ACKNOWLEGEMENT PACKET");

   setIP("eth0",dhcppacket.yiaddr);
   system("ifconfig");
}


/* DHCP_DISCOVER*/
int dhcp_discover(DHCPPacket *dhcppacket)
{
  int socketFd;
  struct sockaddr_in myaddr;
  char my_message[BUFSIZE]; 
  struct sockaddr_in servaddr;
  int fields;
  int my_message_index = 0;
  int index = 0;
  int broadcastOn = 1;
  int broadcastOff = 0;
  
  char useless[600];
  
  //set the dhcppacket values
  dhcppacket->op = '1';   //BOOTREQUEST
  dhcppacket->htype = '1';
  dhcppacket->hlen = '6';
  dhcppacket->hops = '0';
  dhcppacket->xid = transactionNumber++;
  dhcppacket->sec = 0;
  dhcppacket->flags = 0;
  strncpy(dhcppacket->ciaddr,"0.0.0.0",sizeof(dhcppacket->ciaddr));
  strncpy(dhcppacket->yiaddr,"0.0.0.0",sizeof(dhcppacket->yiaddr));
  strncpy(dhcppacket->siaddr,"127.0.0.1",sizeof(dhcppacket->siaddr));
  strncpy(dhcppacket->giaddr,"0.0.0.0",sizeof(dhcppacket->giaddr));
  //getMacAddress(&(dhcppacket->chaddr));
  strncpy(dhcppacket->chaddr,"1234",sizeof(dhcppacket->chaddr));
  strncpy(dhcppacket->sname,"DHCP CONFIGURATION SERVER",sizeof(dhcppacket->sname));
  strncpy(dhcppacket->file,"\0",sizeof(dhcppacket->sname));
  strncpy(dhcppacket->options,"\0",sizeof(dhcppacket->sname));
  


  if((socketFd = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP)) < 0)
  { 
    printf("Error creating socket");
    exit(0);
  }
  //printf("\ncreating socket");
  
  memset((char *)&myaddr, 0, sizeof(myaddr));
  myaddr.sin_family = AF_INET;
  myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  myaddr.sin_port = htons(0);  
  

  if(setsockopt(socketFd, SOL_SOCKET, SO_BROADCAST, &broadcastOn,4) < -1)
  {
	printf("something failed");
	exit(0);
  }

  if (bind(socketFd, (struct sockaddr *)&myaddr, sizeof(myaddr)) < 0)
  {
		perror("bind failed");
		return 0;
  }       

  //printf("\nbinded socket");

  memset((char *)&servaddr,0,sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(SERVICEPORT);
  /* server address*/
  if (inet_aton(SERVER_IP, &servaddr.sin_addr)==0)
  {
      fprintf(stderr, "inet_aton() failed\n");
      exit(1);
  }

  printf("\n SENDING DISCOVER PACKET\n");
  for(fields = 1; fields <= 15 ; fields++)
  { 
     switch(fields)
     {

	case OP     :  my_message[my_message_index++] = dhcppacket->op;
		       my_message[my_message_index++] = '-';
		       break;
        case HTYPE  :  my_message[my_message_index++] = dhcppacket->htype;
		       my_message[my_message_index++] = '-';
		       break;
	case HLEN   :  my_message[my_message_index++] = dhcppacket->hlen;
		       my_message[my_message_index++] = '-';
		       break;
	case HOPS   :  my_message[my_message_index++] = dhcppacket->hops;
		       my_message[my_message_index++] = '-';
		       break;
	case XID    :  my_message[my_message_index++] = getCharOfNum(dhcppacket->xid);
		       my_message[my_message_index++] = '-';
		       break;
	case SEC    :  my_message[my_message_index++] = getCharOfNum(dhcppacket->sec);
		       my_message[my_message_index++] = '-';
		       break;
	case FLAGS  :  my_message[my_message_index++] = getCharOfNum(dhcppacket->flags);
		       my_message[my_message_index++] = '-';
		       break;
	case CIADDR :  index = 0;
		       strncpy(useless,dhcppacket->ciaddr,sizeof(useless));	
		       while((useless[index]) != '\0')
                       {
			 my_message[my_message_index++] = useless[index++];
		       }
		       my_message[my_message_index++] = '-';
		       break;	
	case YIADDR :  index = 0;
		       strncpy(useless,dhcppacket->yiaddr,sizeof(useless));	
		       while((useless[index]) != '\0')
                       {
			 my_message[my_message_index++] = useless[index++];
		       }
		       my_message[my_message_index++] = '-';
		       break;
	case SIADDR :  index = 0;
		       strncpy(useless,dhcppacket->siaddr,sizeof(useless));	
		       while((useless[index]) != '\0')
                       {
			 my_message[my_message_index++] = useless[index++];
		       }
		       my_message[my_message_index++] = '-';
		       break;
	case GIADDR :  index = 0;
		       strncpy(useless,dhcppacket->giaddr,sizeof(useless));	
		       while((useless[index]) != '\0')
                       {
			 my_message[my_message_index++] = useless[index++];
		       }
		       my_message[my_message_index++] = '-';
		       break;
	case CHADDR :  index = 0;
		       strncpy(useless,dhcppacket->chaddr,sizeof(useless));	
		       while((useless[index]) != '\0')
                       {
			 my_message[my_message_index++] = useless[index++];
		       }
		       my_message[my_message_index++] = '-';
		       break;
	case SNAME :   index = 0;
		       strncpy(useless,dhcppacket->sname,sizeof(useless));	
		       while((useless[index]) != '\0')
                       {
			 my_message[my_message_index++] = useless[index++];
		       }
		       my_message[my_message_index++] = '-';
		       break;
	case FILE  :   index = 0;
		       strncpy(useless,dhcppacket->file,sizeof(useless));	
		       while((useless[index]) != '\0')
                       {
			 my_message[my_message_index++] = useless[index++];
		       }
		       my_message[my_message_index++] = '-';
		       break;
	case OPTIONS : index = 0;
		       strncpy(useless,dhcppacket->options,sizeof(useless));	
		       while((useless[index]) != '\0')
                       {
			 my_message[my_message_index++] = useless[index++];
		       }
		       my_message[my_message_index++] = '-';	
		       break;

       }//end of switch
    }//end of for
    // printf("THIS IS THE CONSTRUCTED : %s",my_message);  
  
     if(sendto(socketFd,my_message,strlen(my_message),0,(struct sockaddr *)&servaddr,sizeof(servaddr)) < 0 )
     {	
        printf("Error sending");
        exit(0);	
     }
     //printf("\nsent\n");
}

// dhcp offer takes packet from server
int dhcp_offer(DHCPPacket *dhcppacket)
{
	struct sockaddr_in myaddr;	
	struct sockaddr_in remaddr;
	socklen_t addrlen = sizeof(remaddr);		
	int recvlen;			
	int fd;				
	unsigned char buf[BUFSIZE];	
        int index = 0;
        char useless[600];
	int uselessindex = 0;
	int field = 8;
        /*char process[100];
	int processindex = 0;
	char network[20];
        char subnetmask[20];
	char defaultgateway[20];
	int optionsfield = 0;*/	



        //printf("\n DHCP OFFER\n");
	if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		perror("cannot create socket\n");
		return 0;
	}
		
	memset((char *)&myaddr, 0, sizeof(myaddr));
	myaddr.sin_family = AF_INET;
	myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	myaddr.sin_port = htons(OFFERPORT);

	if (bind(fd, (struct sockaddr *)&myaddr, sizeof(myaddr)) < 0)
	{
		perror("bind failed");
		return 0;
	}

	//keep waiting for reqeust
	//printf("waiting for reply from server %d\n", OFFERPORT);
       
	 	recvlen = recvfrom(fd, buf, BUFSIZE, 0, (struct sockaddr *)&remaddr, &addrlen);
	//	printf("received %d bytes\n", recvlen);
		if (recvlen > 0)
	        {
	           buf[recvlen] = 0;
	  //         printf("\nreceived message: %s\n", buf);
	        }
	
	        //decode the message
		dhcppacket->op = buf[FIELD1];
		dhcppacket->htype = buf[FIELD2];
		dhcppacket->htype = buf[FIELD3];
		dhcppacket->hlen = buf[FIELD4];
		dhcppacket->xid = buf[FIELD5] - 48;
		dhcppacket->sec = buf[FIELD6] - 48;
		dhcppacket->flags = buf[FIELD7] - 48;
	
		index = 14;
		field = 8;
		while(index < strlen(buf))
		{
		//   printf("\nINDEX : %d\n",index);	
	 	   uselessindex = 0;
		   while(buf[index] != '-')
		     useless[uselessindex++] = buf[index++];
		   useless[uselessindex] = '\0';
		  // printf("\nUSELESS %s\n",useless);
		   index++;
		   switch(field)
		   {
		     case CIADDR : strncpy(dhcppacket->ciaddr,useless,sizeof(dhcppacket->ciaddr));break;
		     case YIADDR : strncpy(dhcppacket->yiaddr,useless,sizeof(dhcppacket->yiaddr));break;
		     case SIADDR : strncpy(dhcppacket->siaddr,useless,sizeof(dhcppacket->siaddr));break;
		     case GIADDR : strncpy(dhcppacket->giaddr,useless,sizeof(dhcppacket->giaddr));break;
		     case CHADDR : strncpy(dhcppacket->chaddr,useless,sizeof(dhcppacket->chaddr));break;
		     case SNAME  : strncpy(dhcppacket->sname,useless,sizeof(dhcppacket->sname));break;	
		     case FILE   : strncpy(dhcppacket->file,useless,sizeof(dhcppacket->file)); break;
		     case OPTIONS: strncpy(dhcppacket->options,useless,sizeof(dhcppacket->options));break;
		   }//end of switch
		 field++;
		}//end of while
		//printf("\n!!!DONE!!! %s!!\n",dhcppacket->options);  
	
	        //process the network id and subnet mask
                /*strncpy(process,dhcppacket->options,sizeof(process));
	
		optionsfield = 1;
		while(optionsfield != 4)
		{
		  //printf("\nPROCESSSSSSSSSSSING \n");
		   switch(optionsfield)
		   {
		      case NETWORK   : index = 0;
				       while(process[processindex] != '%')
				       {
				   	  network[index++] = process[processindex++];
				       }
				       network[index] = '\0';
				       processindex++;
				       optionsfield++;
				       break;
		      case SUBNETMASK: index = 0;
				       while(process[processindex] != '%')
				       {
					  subnetmask[index++] = process[processindex++];
				       }
				       subnetmask[index] = '\0';
				       processindex++;
				       optionsfield++;
				       break;		
		     case DEFAULTGATEWAY :index = 0;
				     	  while(process[processindex] != '%')
				     	  {
					    defaultgateway[index++] = process[processindex++];
				          }
					  defaultgateway[index] = '\0';
				          processindex++;
				          optionsfield++;
				          break;			     				
		    }//end of switch
	        }//end of while   
		//printf("\nNETWORK ID OFFERED: %s\n",network);
		//printf("\nSUBNETMASK  : %s\n",subnetmask);
		//printf("\nDEFAULTGATEWAY: %s\n",defaultgateway);*/
}

/*sends a request to server to grant ip assignment*/
int dhcp_request(DHCPPacket *dhcppacket)
{

  int socketFd;
  struct sockaddr_in myaddr;
  char my_message[BUFSIZE]; 
  struct sockaddr_in servaddr;
  int fields;
  int my_message_index = 0;
  int index = 0;
  int broadcastOn = 1;
  int broadcastOff = 0;
  char useless[600];
  
  //set the dhcppacket values
  dhcppacket->op = '1';   //BOOTREQUEST
  dhcppacket->htype = '1';
  dhcppacket->hlen = '6';
  dhcppacket->hops = '0';
  dhcppacket->xid = transactionNumber++;
  dhcppacket->sec = 0;
  dhcppacket->flags = 0;
  strncpy(dhcppacket->ciaddr,"0.0.0.0",sizeof(dhcppacket->ciaddr));
  strncpy(dhcppacket->yiaddr,"0.0.0.0",sizeof(dhcppacket->yiaddr));
  strncpy(dhcppacket->siaddr,"127.0.0.1",sizeof(dhcppacket->siaddr));
  strncpy(dhcppacket->giaddr,"0.0.0.0",sizeof(dhcppacket->giaddr));
  //getMacAddress(&(dhcppacket->chaddr));
  strncpy(dhcppacket->chaddr,"1234",sizeof(dhcppacket->chaddr));
  strncpy(dhcppacket->sname,"DHCP CONFIGURATION SERVER",sizeof(dhcppacket->sname));
  strncpy(dhcppacket->file,"\0",sizeof(dhcppacket->sname));
  strncpy(dhcppacket->options,"\0",sizeof(dhcppacket->sname));
  


  if((socketFd = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP)) < 0)
  { 
    printf("Error creating socket");
    exit(0);
  }
 // printf("\ncreating socket");
  
  memset((char *)&myaddr, 0, sizeof(myaddr));
  myaddr.sin_family = AF_INET;
  myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  myaddr.sin_port = htons(0);  
  

  if(setsockopt(socketFd, SOL_SOCKET, SO_BROADCAST, &broadcastOn,4) < -1)
  {
	printf("something failed");
	exit(0);
  }

  if (bind(socketFd, (struct sockaddr *)&myaddr, sizeof(myaddr)) < 0) {
		perror("bind failed");
		return 0;
	}       

  //printf("\nbinded socket");

  memset((char *)&servaddr,0,sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(REQUESTPORT);
  /* server address*/
  if (inet_aton(SERVER_IP, &servaddr.sin_addr)==0)
  {
      fprintf(stderr, "inet_aton() failed\n");
      exit(1);
  }

  printf("\n SENDING REQUEST PACKET\n");
  for(fields = 1; fields <= 15 ; fields++)
  { 
     switch(fields)
     {

	case OP     :  my_message[my_message_index++] = dhcppacket->op;
		       my_message[my_message_index++] = '-';
		       break;
        case HTYPE  :  my_message[my_message_index++] = dhcppacket->htype;
		       my_message[my_message_index++] = '-';
		       break;
	case HLEN   :  my_message[my_message_index++] = dhcppacket->hlen;
		       my_message[my_message_index++] = '-';
		       break;
	case HOPS   :  my_message[my_message_index++] = dhcppacket->hops;
		       my_message[my_message_index++] = '-';
		       break;
	case XID    :  my_message[my_message_index++] = getCharOfNum(dhcppacket->xid);
		       my_message[my_message_index++] = '-';
		       break;
	case SEC    :  my_message[my_message_index++] = getCharOfNum(dhcppacket->sec);
		       my_message[my_message_index++] = '-';
		       break;
	case FLAGS  :  my_message[my_message_index++] = getCharOfNum(dhcppacket->flags);
		       my_message[my_message_index++] = '-';
		       break;
	case CIADDR :  index = 0;
		       strncpy(useless,dhcppacket->ciaddr,sizeof(useless));	
		       while((useless[index]) != '\0')
                       {
			 my_message[my_message_index++] = useless[index++];
		       }
		       my_message[my_message_index++] = '-';
		       break;	
	case YIADDR :  index = 0;
		       strncpy(useless,dhcppacket->yiaddr,sizeof(useless));	
		       while((useless[index]) != '\0')
                       {
			 my_message[my_message_index++] = useless[index++];
		       }
		       my_message[my_message_index++] = '-';
		       break;
	case SIADDR :  index = 0;
		       strncpy(useless,dhcppacket->siaddr,sizeof(useless));	
		       while((useless[index]) != '\0')
                       {
			 my_message[my_message_index++] = useless[index++];
		       }
		       my_message[my_message_index++] = '-';
		       break;
	case GIADDR :  index = 0;
		       strncpy(useless,dhcppacket->giaddr,sizeof(useless));	
		       while((useless[index]) != '\0')
                       {
			 my_message[my_message_index++] = useless[index++];
		       }
		       my_message[my_message_index++] = '-';
		       break;
	case CHADDR :  index = 0;
		       strncpy(useless,dhcppacket->chaddr,sizeof(useless));	
		       while((useless[index]) != '\0')
                       {
			 my_message[my_message_index++] = useless[index++];
		       }
		       my_message[my_message_index++] = '-';
		       break;
	case SNAME :   index = 0;
		       strncpy(useless,dhcppacket->sname,sizeof(useless));	
		       while((useless[index]) != '\0')
                       {
			 my_message[my_message_index++] = useless[index++];
		       }
		       my_message[my_message_index++] = '-';
		       break;
	case FILE  :   index = 0;
		       strncpy(useless,dhcppacket->file,sizeof(useless));	
		       while((useless[index]) != '\0')
                       {
			 my_message[my_message_index++] = useless[index++];
		       }
		       my_message[my_message_index++] = '-';
		       break;
	case OPTIONS : index = 0;
		       strncpy(useless,dhcppacket->options,sizeof(useless));	
		       while((useless[index]) != '\0')
                       {
			 my_message[my_message_index++] = useless[index++];
		       }
		       my_message[my_message_index++] = '-';
		       break;

       }//end of switch
    }//end of for
     //printf("THIS IS THE CONSTRUCTED : %s",my_message);  
  
     if(sendto(socketFd,my_message,strlen(my_message),0,(struct sockaddr *)&servaddr,sizeof(servaddr)) < 0 )
     {	
        printf("Error sending");
        exit(0);	
     }
    // printf("\nsent\n");

}


int dhcp_acknowlegment(DHCPPacket *dhcppacket)
{
struct sockaddr_in myaddr;	
	struct sockaddr_in remaddr;
	socklen_t addrlen = sizeof(remaddr);		
	int recvlen;			
	int fd;				
	unsigned char buf[BUFSIZE];	
        int index = 0;
        char useless[600];
	int uselessindex = 0;
	int field = 8;
       /* char process[100];
	int processindex = 0;
	char network[20];
        char subnetmask[20];
	char defaultgateway[20];
	int optionsfield = 0;*/	



        //printf("\n DHCP ACKNOWLEGMENT\n");
	if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		perror("cannot create socket\n");
		return 0;
	}
		
	memset((char *)&myaddr, 0, sizeof(myaddr));
	myaddr.sin_family = AF_INET;
	myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	myaddr.sin_port = htons(ACKNOWLEGEMENTPORT);

	if (bind(fd, (struct sockaddr *)&myaddr, sizeof(myaddr)) < 0)
	{
		perror("bind failed");
		return 0;
	}

	//keep waiting for reqeust
	//printf("waiting for reply from server %d\n", OFFERPORT);
       
	 	recvlen = recvfrom(fd, buf, BUFSIZE, 0, (struct sockaddr *)&remaddr, &addrlen);
	//	printf("received %d bytes\n", recvlen);
		if (recvlen > 0)
	        {
	           buf[recvlen] = 0;
	  //         printf("\nreceived message: %s\n", buf);
	        }
	
	        //decode the message
		dhcppacket->op = buf[FIELD1];
		dhcppacket->htype = buf[FIELD2];
		dhcppacket->htype = buf[FIELD3];
		dhcppacket->hlen = buf[FIELD4];
		dhcppacket->xid = buf[FIELD5] - 48;
		dhcppacket->sec = buf[FIELD6] - 48;
		dhcppacket->flags = buf[FIELD7] - 48;
	
		index = 14;
		field = 8;
		while(index < strlen(buf))
		{
		   //printf("\nINDEX : %d\n",index);	
	 	   uselessindex = 0;
		   while(buf[index] != '-')
		     useless[uselessindex++] = buf[index++];
		   useless[uselessindex] = '\0';
		   //printf("\nUSELESS %s\n",useless);
		   index++;
		   switch(field)
		   {
		     case CIADDR : strncpy(dhcppacket->ciaddr,useless,sizeof(dhcppacket->ciaddr));break;
		     case YIADDR : strncpy(dhcppacket->yiaddr,useless,sizeof(dhcppacket->yiaddr));break;
		     case SIADDR : strncpy(dhcppacket->siaddr,useless,sizeof(dhcppacket->siaddr));break;
		     case GIADDR : strncpy(dhcppacket->giaddr,useless,sizeof(dhcppacket->giaddr));break;
		     case CHADDR : strncpy(dhcppacket->chaddr,useless,sizeof(dhcppacket->chaddr));break;
		     case SNAME  : strncpy(dhcppacket->sname,useless,sizeof(dhcppacket->sname));break;	
		     case FILE   : strncpy(dhcppacket->file,useless,sizeof(dhcppacket->file)); break;
		     case OPTIONS: strncpy(dhcppacket->options,useless,sizeof(dhcppacket->options));break;
		   }//end of switch
		 field++;
		}//end of while
		//printf("\n!!!DONE!!! %s!!\n",dhcppacket->options);  
	
	       /* //process the network id and subnet mask
                strncpy(process,dhcppacket->options,sizeof(process));
	
		optionsfield = 1;
		while(optionsfield != 4)
		{
		  //printf("\nPROCESSSSSSSSSSSING \n");
		   switch(optionsfield)
		   {
		      case NETWORK   : index = 0;
				       while(process[processindex] != '%')
				       {
				   	  network[index++] = process[processindex++];
				       }
				       network[index] = '\0';
				       processindex++;
				       optionsfield++;
				       break;
		      case SUBNETMASK: index = 0;
				       while(process[processindex] != '%')
				       {
					  subnetmask[index++] = process[processindex++];
				       }
				       subnetmask[index] = '\0';
				       processindex++;
				       optionsfield++;
				       break;		
		     case DEFAULTGATEWAY :index = 0;
				     	  while(process[processindex] != '%')
				     	  {
					    defaultgateway[index++] = process[processindex++];
				          }
					  defaultgateway[index] = '\0';
				          processindex++;
				          optionsfield++;
				          break;			     				
		    }//end of switch
	        }//end of while   
		//printf("\nNETWORK ID OFFERED: %s\n",network);
		//printf("\nSUBNETMASK  : %s\n",subnetmask);
		//printf("\nDEFAULTGATEWAY: %s\n",defaultgateway);
	        //printf("\nIP : %s",dhcppacket->yiaddr);*/

}





void displayPacket(DHCPPacket *dhcppacket,char name[])
{
   printf("\n==============%s===============\n",name);
   printf("|--------|"); printf("-----------|"); printf("----------|"); printf("----------|\n");
   printf("|  OP %c  ",dhcppacket->op);printf("|  HTYPE %c  ",dhcppacket->htype);printf("|  HLEN %c  ",dhcppacket->hlen);
   printf("|  HOPS %c  |",dhcppacket->hops);
   printf("\n|--------|"); printf("-----------|"); printf("----------|"); printf("----------|\n");
   printf("|		 XID %d	                   |\n",dhcppacket->xid);
   printf("|---------"); printf("------------"); printf("-----------"); printf("----------|\n");
   printf("|  SEC %d  \t     ",dhcppacket->sec);printf("|  FLAGS %d  \t   |\n",dhcppacket->flags);
   printf("|---------"); printf("------------"); printf("-----------"); printf("----------|\n");
   printf("|               CLIENT IP %s          |\n",dhcppacket->ciaddr);
   printf("|---------"); printf("------------"); printf("-----------"); printf("----------|\n");
   printf("|               YOUR IP %s            |\n",dhcppacket->yiaddr);
   printf("|---------"); printf("------------"); printf("-----------"); printf("----------|\n");
   printf("|               SERVER IP %s        |\n",dhcppacket->siaddr);
   printf("|---------"); printf("------------"); printf("-----------"); printf("----------|\n");
   printf("|               RELAY IP %s           |\n",dhcppacket->giaddr);
   printf("|---------"); printf("------------"); printf("-----------"); printf("----------|\n");
   printf("|               MAC ADDRESS %s           |\n",dhcppacket->chaddr);
   printf("|---------"); printf("------------"); printf("-----------"); printf("----------|\n");
   printf("|   SERVER NAME %s  |\n",dhcppacket->sname);
   printf("|---------"); printf("------------"); printf("-----------"); printf("----------|\n");
   printf("|               FILE %s                      |\n",dhcppacket->file);
   printf("|---------"); printf("------------"); printf("-----------"); printf("----------|\n");
   printf("| OPTIONS %s                   |\n",dhcppacket->options);
   printf("|---------"); printf("------------"); printf("-----------"); printf("----------|\n\n");
}

/*gets the mac address */
void getMacAddress(char *macAddress)
{
	struct ifreq s;
  int fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_IP);
  int index = 0;
  strcpy(s.ifr_name, "wlan0");
  if (0 == ioctl(fd, SIOCGIFHWADDR, &s))
  {
    int i;
    for (i = 0; i < 6; ++i)
    {
       macAddress[index++] = s.ifr_addr.sa_data[i];
       macAddress[index] = ':';
    }
    puts("\n");   
   }

}

/* get the char equivalent of int */
char getCharOfNum(int num)
{
  if(num == 0)
   return '0';
  if(num == 1)
   return '1';
  if(num == 2)
   return '2';
  if(num == 3)
   return '3';
  if(num == 4)
   return '4';
  if(num == 5)
   return '5';
  if(num == 6)
   return '6';
  if(num == 7)
   return '7';
  if(num == 8)
   return '8';
  if(num == 9)
   return '9';
  return '1';
}  

/* call delay*/
void callDelay()
{

  int i,j,k;
  for(i = 0; i < 100;i++)
   for( j = 0;j < 10; j++)
    for( k = 0;k < 10;k++)
    {
    }
}

void setIP(char *iface_name,char *ip_addr )
{

if(!iface_name)
		return;

	int sockfd;
	struct ifreq ifr;
	struct sockaddr_in sin;

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sockfd == -1){
		fprintf(stderr, "Could not get socket.\n");
		return ;
	}

	/* get interface name */
	strncpy(ifr.ifr_name, iface_name, IFNAMSIZ);

	/* Read interface flags */
	if (ioctl(sockfd, SIOCGIFFLAGS, &ifr) < 0) {
		fprintf(stderr, "ifdown: shutdown ");
		perror(ifr.ifr_name);
		return ;
	}

	/*
	* Expected in <net/if.h> according to
	* "UNIX Network Programming".
	*/
	#ifdef ifr_flags
	# define IRFFLAGS       ifr_flags
	#else   /* Present on kFreeBSD */
	# define IRFFLAGS       ifr_flagshigh
	#endif

	// If interface is down, bring it up
	if (!(ifr.IRFFLAGS & IFF_UP)) {
		fprintf(stdout, "Device is currently down..setting up.-- %u\n",ifr.IRFFLAGS);
		ifr.IRFFLAGS |= IFF_UP;
		if (ioctl(sockfd, SIOCSIFFLAGS, &ifr) < 0) {
			fprintf(stderr, "ifup: failed ");
			perror(ifr.ifr_name);
			return;
		}
	}

	sin.sin_family = AF_INET;

	// Convert IP from numbers and dots to binary notation
	inet_aton(ip_addr,&sin.sin_addr.s_addr);
	memcpy(&ifr.ifr_addr, &sin, sizeof(struct sockaddr));

	// Set interface address
	if (ioctl(sockfd, SIOCSIFADDR, &ifr) < 0) {
		fprintf(stderr, "Cannot set IP address. ");
		perror(ifr.ifr_name);
		return ;
	}
	#undef IRFFLAGS
	return;
}


