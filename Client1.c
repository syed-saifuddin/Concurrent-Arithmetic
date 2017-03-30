#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>

int main(int argc , char *argv[])
{
    int sock;
    struct sockaddr_in server;
    char message[1000], reply[1000];

    /*Create a Socket*/
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1)
    {
        printf("**SOCKET NOT CREATED**\n");
    }
    else printf("**SOCKET READY**\n");

    server.sin_addr.s_addr = inet_addr("127.0.0.1"); //IP Address
    server.sin_family = AF_INET;
    server.sin_port = htons(5004); //Port Number

    /*Establish Connection to Server*/
  if( connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
     printf("**CONNECTION ERROR**\n");
     return 1;
     }
   else printf("**CONNECTED**\n");

  /*Reading from input11.dat*/

   FILE *fp;

   /* opening file for reading */
   fp = fopen("input1.dat" , "r");
   if(fp == NULL)
   {
      perror("**ERROR OPENING FILE**\n");
      return(-1);
   }
   if( fgets (message,100, fp)!=NULL )
   {
      /* writing content to stdout */
      printf("Sending to Server:");
      puts(message);
   }
   fclose(fp);

    /*Communicate with Server*/

        /*Send your Message*/
        if( send(sock , message , strlen(message) , 0) < 0)
        {
            printf("**Sending Error**\n");
            return 1;
        }

        /*Reply from Server*/
        if( recv(sock , reply , 2000 , 0) < 0)
        {
            printf("**Recieve Error**\n");
           exit(0);
                // break;
        }

        printf("Server reply:%s\n ",reply);
    FILE *fp1 = fopen("output1.txt","w" );
    if (fp1 != NULL)
    {
        fputs(reply, fp1);
        fclose(fp1);
    }

        close (sock); //Close the socket
    return 0;
}
