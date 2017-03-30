#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<string.h>
#include<stdlib.h>
#include<netinet/in.h>

long BinaryToDecimal(long x);
long DecimalToBinary(long y);

int main()
{
int server_sock, client_sock;

struct sockaddr_in ServAddress;
struct sockaddr_in CliAddress;//server sends to client on this address
int CliAddressLength;
int rcv, pid;

char buffer[100],str[100];
int i=0,j=0,k=0,space_count=0;
long a=0, b=0,bin1=0,bin2=0,bin_answer=0,answer=0;
char num1[20],num2[20],operator[1];

server_sock=socket(AF_INET,SOCK_STREAM,0);

if (server_sock < 0)
        {
        printf("**SOCKET NOT CREATED**\n");
    exit(0);
    }
printf("SOCKET CREATED**\n");

memset(&ServAddress,0,sizeof(ServAddress));
ServAddress.sin_family=AF_INET;
ServAddress.sin_addr.s_addr=htonl(INADDR_ANY);
ServAddress.sin_port=htons(5004);

if ( (bind(server_sock,(struct sockaddr *)&ServAddress, sizeof(ServAddress))< 0))
        {
        printf("**BIND ERROR**\n");
    exit(0);
        }
printf("**BIND COMPLETE**\n");

listen(server_sock,5);

while(1)
        {
        printf("**SERVER RUNNING**\n");
        CliAddressLength=sizeof(CliAddress);
        client_sock=accept(server_sock,(struct sockaddr*)&CliAddress,&CliAddressLength);

        pid=fork();
        if(pid==0)
                {
                while(1)
                        {
                        rcv=recv(client_sock,buffer,100,0);
                        if (rcv!=0) printf("**FROM CLIENT:%s\n",buffer);
                        if(rcv==0)
                                {
                                close(client_sock);
                                break;
                                }
                        //str=buffer;
                        //while( buffer!=NULL )
                        //{
                                for(i=0;i<100;i++)
                                {
                                        if(buffer[i]==' ')
                                        {
                                        i++;
                                        space_count++;
                                        }
                                        if(buffer[i]=='\0')
                                        i=100;
                                        else
                                        {
                                        if(space_count==0)
                                                {
                                                num1[j]=buffer[i];
                                                j++;
                                                }
                                        else if (space_count==1)
                                                {
                                                num2[k]=buffer[i];
                                                k++;
                                                }
                                        else if (space_count==2)
                                                {
                                                operator[0]=buffer[i];
                                                space_count++;
                                                }
                                        }
                                 }
                        //}
                        printf("Num1: %s\n",num1);
                        printf("Num2: %s\n",num2);
                        printf("Operator: %c\n", operator[0]);
                        a= atoi (num1);
                        b= atoi (num2);
                        printf("Binary Equation: %d %c %d\n", a, operator[0], b);
                        bin1= BinaryToDecimal(a);
                        bin2= BinaryToDecimal(b);
                        printf("Decimal Equation: %ld %c %ld\n",bin1, operator[0], bin2);
                        if( operator[0]=='+') answer= bin1+bin2;
                        else if (operator[0]=='-') answer= bin1-bin2;
                        else if (operator[0]=='*') answer= bin1*bin2;
                        else if (operator[0]=='/') answer= bin1/bin2;
                        else printf("Invalid Operator\n");
                        printf("Decimal Answer:%d\n",answer);
                        bin_answer= DecimalToBinary(answer);
                        printf("Binary Answer: %ld\n", bin_answer);
                        sprintf(buffer,"%ld",bin_answer);
                        printf("Sending To Client: %s\n\n",buffer);

                        buffer[rcv]=0;
                        send(client_sock,buffer,rcv,0);
                        //printf("**FROM CLIENT**:%s\n",buffer);
                        }
                exit(0);
                }
        else
                {
                close(client_sock);
                }
        }
return 0;
}

long BinaryToDecimal(long n)
{

int decimalNumber = 0, i = 0, remainder;
    while (n!=0)
    {
        remainder = n%10;
        n /= 10;
        decimalNumber += remainder*pow(2,i);
        ++i;
    }
    return decimalNumber;
}

long DecimalToBinary(long n)
{
    int remainder;
    long binary = 0, i = 1;

    while(n != 0) {
        remainder = n%2;
        n = n/2;
        binary= binary + (remainder*i);
        i = i*10;
    }
    return binary;
}

