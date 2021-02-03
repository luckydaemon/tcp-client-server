#include <stdio.h>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <string.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <gmp.h>
#include <gmpxx.h>
#include <pthread.h>
#include <iomanip>

char buffer[128];
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

std::string decoding(std::string code)
{ 
    std::string res;
    if (code.size() == 1)
    {
        res = std::to_string((int)code[0]);
        return res;
    }
    for (int i = 1; i < code.size(); i++)
    {   
        char temp = (int)code[i];
        char lsb = (int)(temp & 0x0f);
        char msb = (temp >> 4) & 0x0f;
        std::stringstream lsbstream;
        lsbstream << std::hex << (int)lsb;
        std::stringstream msbstream;
        msbstream << std::hex << (int)msb;
        res.append(msbstream.str());
        res.append(lsbstream.str());
    }
    mpz_class ret(res, 16);
    std::string ans(ret.get_str(10)); 
    return ans;
}

void * socketThread(void *arg)
{
    int newSocket = *((int *)arg);
    pthread_mutex_lock(&lock);
    int valread = read( newSocket , buffer, 128);     
    std::string temp(buffer);
    std::cout<<decoding(temp)<<std::endl;  
    pthread_mutex_unlock(&lock);
    sleep(1);
    std::cout<<"Exit socketThread"<<std::endl;
    close(newSocket);
    pthread_exit(NULL);
}
int main(void)
{

  int serverSocket, newSocket;
  struct sockaddr_in serv_addr;
  struct sockaddr_storage serv_storage;
  socklen_t addr_size;
  serverSocket = socket(PF_INET, SOCK_STREAM, 0);
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(32001);
  serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  memset(serv_addr.sin_zero, '\0', sizeof(serv_addr.sin_zero));
  bind(serverSocket, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
  if(listen(serverSocket,50)==0)
    std::cout<<"Listening"<<std::endl;
  else
    std::cout<<"Error"<<std::endl;
    pthread_t tid[60];
    int i = 0;
    while(1)
    {
        addr_size = sizeof serv_storage;
        newSocket = accept(serverSocket, (struct sockaddr *) &serv_storage, &addr_size);
        if( pthread_create(&tid[i++], NULL, socketThread, &newSocket) != 0 )
           std::cout<<"Failed to create thread"<<std::endl;;
        if( i <= 50)
        {
          i = 0;
          while(i < 50)
          {
            pthread_join(tid[i++],NULL);
          }
          i = 0;
        }
    }
  return 0;
}