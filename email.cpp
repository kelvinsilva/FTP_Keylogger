#include "email.h"

using namespace std;


socketclass::socketclass(){

    setsocket(string("127.0.0.1"), 80);
    buffersize = 5000;


}

socketclass::socketclass(string ipsock, int portnom, int buff){

    setsocket(ipsock, portnom);
    buffersize = buff;

}


void socketclass::changebuffersize(int bufsz){

    buffersize = bufsz;

}



void socketclass::initsocket(){

    if (WSAStartup(MAKEWORD(2,2), &wsa)!=0)  {
              //more error checking
            int x = WSAGetLastError();

            stringstream ss;

                    ss <<"Error: " << x << " From WSAGetLastError() From WSAStart() !";

            MessageBox(NULL, ss.str().c_str(), " Winsock Error! ", MB_OK | MB_ICONINFORMATION);
       }


    //MessageBox(NULL, ipaddress.c_str(), "debug", NULL);
    server.sin_addr.s_addr = inet_addr(ipaddress.c_str());      //initialize members of server structure
    server.sin_family = AF_INET;
    server.sin_port = htons(portnumber);

}

int socketclass::connectsocket(){

    if ((sa = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET){
             int x = WSAGetLastError();

            stringstream ss;

                    ss <<"Error: " << x << " From WSAGetLastError() from socket() function!";

            MessageBox(NULL, ss.str().c_str(), " Winsock Error! ", MB_OK | MB_ICONINFORMATION);
    }

     if (connect(sa, (struct sockaddr *)&server, sizeof(server)) < 0){

         int x = WSAGetLastError();

         stringstream ss;

         ss <<"Error: " << x << " From WSAGetLastError() From connect() function!";

         MessageBox(NULL, ss.str().c_str(), " Winsock Error! ", MB_OK | MB_ICONINFORMATION);
        return -1;

    }

    return 1;
}

char socketclass::read_socket(){

    char server_reply[buffersize];

        int counter = 0;

                while (counter < buffersize){ //make sure read buffer is clear!
                    server_reply[counter] = '\0';
                    counter++;

                }

            if ((recv(sa, server_reply, buffersize, 0)) == SOCKET_ERROR){     //error checking combined with recv function

                int x = WSAGetLastError();

                stringstream ss;


                    ss <<"Error: " << x << " From WSAGetLastError()" << " From recv() function! ";

                MessageBox(NULL, ss.str().c_str(), "Winsock Error! ", MB_OK | MB_ICONINFORMATION);


                return 0;

            }


            //again used for debugging:
            //cout << server_reply;
        return *server_reply; //Useless return statement

}

int socketclass::send_socket(string mess){

        if (send(sa, mess.c_str(), mess.size(), 0 ) < 0){

            int x = WSAGetLastError();

            stringstream ss;

                ss <<"Error: " << x << " From WSAGetLastError()" << " send() function!";

            MessageBox(NULL, ss.str().c_str(), "Winsock Error!", MB_OK | MB_ICONINFORMATION);

            return -1;

        }
        //cout << mess;

        return 0;


}

void socketclass::setsocket(string ip, int port){

    portnumber = port;
    ipaddress = ip;

    initsocket();

}

void socketclass::endsocket(){

    closesocket(sa);

}

socketclass::~socketclass(){
    WSACleanup();
    //debug: MessageBox(NULL, "dtor socket", "dtor socket", NULL);
    //dtor
}

/*s*************************************************************************************************************s*/

email::email():socketclass(){



        emailuser = "";
        emailpasswd = "";
        emailfrom = "";
        emailto = "";
        emailsubj = "";
        emailmessage = "";

           ehlo = "EHLO MAIL\r\n";
           authloginuser = "AUTH LOGIN " + emailuser + "\r\n";
           passwd = emailpasswd + "\r\n";
           mailfrom = "MAIL FROM: <" + emailfrom + ">\r\n";
           rcptto = "RCPT TO: <" + emailto + ">\r\n";
           datacmd = "DATA\r\n";
           data = "From: " + emailfrom + "\r\nTo: " + emailto + "\r\nSubject: " + emailsubj + "\r\n\r\n" + emailmessage + "\r\n.\r\n";
           quit = "QUIT\r\n";



}

email::email(string usr, string pass, string from, string to, string subj, string message):socketclass(){


        emailuser = usr;
        emailpasswd = pass;
        emailfrom = from;
        emailto = to;
        emailsubj = subj;
        emailmessage = message;

           ehlo = "EHLO MAIL\r\n";
           authloginuser = "AUTH LOGIN " + emailuser + "\r\n";
           passwd = emailpasswd + "\r\n";
           mailfrom = "MAIL FROM: <" + emailfrom + ">\r\n";
           rcptto = "RCPT TO: <" + to + ">\r\n";
           datacmd = "DATA\r\n";
           data = "From: " + emailfrom + "\r\nTo: " + emailto + "\r\nSubject: " + emailsubj + "\r\n\r\n" + emailmessage + "\r\n.\r\n";
           quit = "QUIT";

}

email::email(string ip, int port, string usr, string pass, string from, string to, string subj, string message):socketclass(ip, port, 5000){



        emailuser = usr;
        emailpasswd = pass;
        emailfrom = from;
        emailto = to;
        emailsubj = subj;
        emailmessage = message;

           ehlo = "EHLO MAIL\r\n";
           authloginuser = "AUTH LOGIN " + emailuser + "\r\n";
           passwd = emailpasswd + "\r\n";
           mailfrom = "MAIL FROM: <" + emailfrom + ">\r\n";
           rcptto = "RCPT TO: <" + to + ">\r\n";
           datacmd = "DATA\r\n";
           data = "From: " + emailfrom + "\r\nTo: " + emailto + "\r\nSubject: " + emailsubj + "\r\n\r\n" + emailmessage + "\r\n.\r\n";
           quit = "QUIT";




}

void email::setemailuser(string emailusr){

    emailuser = emailusr;
    authloginuser = "AUTH LOGIN";

}

void email::setemailpasswd(string emailpsd){

    emailpasswd = emailpsd;
    passwd = emailpsd + "\r\n";

}

void email::setemailfrom(string frm){

    emailfrom = frm;
    mailfrom = "MAIL FROM: <" + frm + ">\r\n";
    data = "From: " + mailfrom + "\r\nTo: " + emailto + "\r\nSubject: " + emailsubj + "\r\n\r\n" + emailmessage + "\r\n.\r\n";

}

void email::setemailto(string t){

    emailto = t;
    rcptto = "RCPT TO: <" + emailto + ">\r\n";
    data = "From: " + mailfrom + "\r\nTo: " + emailto + "\r\nSubject: " + emailsubj + "\r\n\r\n" + emailmessage + "\r\n.\r\n";

}

void email::setemailsubj(string sub){

    emailsubj = sub;
    data = "From: " + mailfrom + "\r\nTo: " + emailto + "\r\nSubject: " + emailsubj + "\r\n\r\n" + emailmessage + "\r\n.\r\n";

}



void email::setemailmessage(string msg){

    emailmessage = msg;
    data = "From: " + mailfrom + "\r\nTo: " + emailto + "\r\nSubject: " + emailsubj + "\r\n\r\n" + emailmessage + "\r\n.\r\n";

}


int email::sendmail(){


    if (connectsocket() < 0){ //connect to gmx server. must change structure values and add tls to conect gmail.
        return -1;
    }

        read_socket(); /* SMTP Server logon string */
        send_socket(ehlo); /* introduce ourselves */

        read_socket(); /*Read reply */
        send_socket(authloginuser);

        read_socket(); /* Sender OK */
        send_socket(passwd);
        read_socket();

        send_socket(mailfrom);

        read_socket(); // Sender OK */
        send_socket(rcptto);
        read_socket(); // Recipient OK*/
        send_socket(datacmd);
        read_socket();
        send_socket(data);// body to follow*/
        read_socket();
        send_socket(quit);
        endsocket();
        //=====Close socket and finish=====*/
        //wsa is not terminated here. only the socket. we reinitialize the socket at the begining of this function


        return 1;

}

email::~email(){



}

