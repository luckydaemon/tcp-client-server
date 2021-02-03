#include <stdio.h> 
#include <cstdlib>
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 
#include <iostream>
#include <gmp.h>
#include <gmpxx.h>
#include <iomanip>


#define PORT 32001 

bool is_number(std::string s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

std::string coding(std::string number)
{
    std::string ans;
    if (number.size() <= 3)
    {
        if (std::stoi(number)<=127)
            {
                char byte = (char) (int) std::stoi(number);
                ans = byte;
                return ans;
            }
    }
    mpz_class hexNum(number);
    std::string hexStr =  hexNum.get_str(16);
    if ((hexStr.size() % 2) == 1)
    hexStr.insert(0,"0");
    ans.append(std::string(1,128+hexStr.size()/2)); 
    for (int i = 0; i < hexStr.size(); i = i + 2)
    {
        char  firstHalfOfOctet = std::stoi(std::string(1,hexStr[i]),0,16) << 4;
        char  secondHalfOfOctet = std::stoi(std::string(1,hexStr[i+1]),0,16);
        char sum = (char)firstHalfOfOctet +  (char)secondHalfOfOctet;
        ans.append(std::string(1,sum));
    }
    return ans;
}

int main(int argc, char const *argv[]) 
{   
    if (argc != 2)
        {
            std::cout<< "Wrong number of arguments. Type in one argument to send"<< std::endl;
            return -1;
        }
    if (!is_number(argv[1]))
        {
            std::cout<< "Wrong argument. Type in one numeric argument to send"<< std::endl;
            return -1;
        }

    std::string codedMessage = coding (argv[1]);
    int newSocket = 0; 
    struct sockaddr_in serv_addr; 
    if ((newSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
        std::cout<< "Socket creation error"<< std::endl; 
        return -1; 
    } 
   
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(PORT); 
       
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)  
    { 
        std::cout<< "Invalid address/ Address not supported"<< std::endl; 
        return -1; 
    } 
   
    if (connect(newSocket, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    { 
        std::cout<< "Connection Failed"<< std::endl; 
        return -1; 
    } 
    send(newSocket , codedMessage.c_str() , sizeof(codedMessage) , 0 );  
    std::cout<<"Message sent"<< std::endl; 
    return 0; 
} 