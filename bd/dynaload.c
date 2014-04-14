
//
// 6-Sep-1998
// demo, runtime dynamic loading of DLL
//
// 15-Sep-99: fixed sscanf call under BltGetVar (removed addr-of operator on char buffers)

#include <stdlib.h>
#include <stdio.h>

#define BLT_WIN32
//#define BLT_OS232

#define BLT_DYNALOAD
#include "bullet_3.h"

#ifdef BLT_WIN32

 #define DLLNAME "b3w32d30.dll" // select the correct dll name
 //#define DLLNAME "b3w32_30.dll"

 #define WIN32_LEAN_AND_MEAN
 #include "windows.h"

 typedef HANDLE T_HANDLE;
 typedef PROC T_PFN;

 #define LOADLIBRARY(dllname) do {                 \
          dllHandle = LoadLibrary(dllname);        \
          if (dllHandle == 0) rc = GetLastError(); \
       } while(0)

 #define UNLOADLIBRARY(handle) do {                \
           rc = FreeLibrary(dllHandle);            \
           if (rc == 0) {                          \
              rc = GetLastError();                 \
           }                                       \
           else {                                  \
              rc = 0;                              \
           }                                       \
        } while(0)

 #define GETPROCADDR(func,dllhandle,procname) do { \
           rc = 0;                                 \
           func = GetProcAddress(dllhandle,procname); \
           if (func == 0) rc = GetLastError();     \
        } while(0)

#else

 #ifdef BLT_OS232

  // in OS/2, if a path is not specified the ".DLL" extension must not be part of name (Warp3 and later)
  //          if a path is specified, then the .ext must be included

  //#define DLLNAME ".\\b3o32_30.DLL"
  #define DLLNAME "b3o32d30"  // select the correct dll name
  //#define DLLNAME "b3o32_30"

  #define INCL_BASE
  #include "os2.h"

  typedef HMODULE T_HANDLE;
  typedef PFN T_PFN;

  #define LOADLIBRARY(dllname) do {                  \
            rc = DosLoadModule(miscBuffer,           \
                               sizeof(miscBuffer),   \
                               dllname,              \
                               &dllHandle);          \
            if (rc) printf("Load Module, problem with '%s'\n",miscBuffer); \
         } while(0)

  #define UNLOADLIBRARY(handle) do {                 \
            rc = DosFreeModule(dllHandle);           \
         } while(0)

   #define GETPROCADDR(func,dllhandle,procname) do { \
             rc = DosQueryProcAddr(dllhandle,0,procname,&func); \
          } while(0)

 UCHAR miscBuffer[260] = "";

 #endif
#endif


int main(void) {

 TBLT_RETC rc = 0;
 ULONG verDos = 0;
 T_HANDLE dllHandle = 0;
 T_PFN tmpFnPtr;


 BLTGETVERSION BltGetVersion = 0;
 BLTGETVAR BltGetVar = 0;

 setbuf(stdout,NULL);

 printf("dynaload.c is using DLLNAME='%s'\n",DLLNAME);

 LOADLIBRARY(DLLNAME);
 if (rc) {
    printf("DLL load of '%s' failed, rc=%u\n",DLLNAME,rc);
    return 1;
 }

 // get OS version number

 GETPROCADDR(tmpFnPtr,dllHandle,"_BltGetVersion");
 if (rc) {
    printf("BltGetVersion GetProcAddr failed, rc=%u\n",rc);
    return 1;
 }
 BltGetVersion = (BLTGETVERSION)tmpFnPtr;
 rc = BltGetVersion(&verDos);

#ifdef BLT_WIN32
{
 char win32sPlatformStr[] = "Win32s";
 char win9xPlatformStr[] = "Win9x";
 char ntPlatformStr[] = "WinNT";
 char unkPlatformStr[] = "unknown platform";
 char *tPtr = 0;

 switch(verDos >> 16) {
 case 0:
    tPtr = win32sPlatformStr;
    break;
 case 1:
    tPtr = win9xPlatformStr;
    break;
 case 2:
    tPtr = ntPlatformStr;
    break;
 default:
    tPtr = unkPlatformStr;
 }
 verDos = verDos & 0xFFFF;
 printf("OS version is %u.%.2u on %s\n",verDos/100,verDos%100,tPtr);
}
#else
 printf("OS version is %u.%.2u (0x%X)\n",verDos/100,verDos%100);
#endif

 // get Bullet version string
{
 UCHAR bVerStr[8],bBuildStr[8],bOSstr[8],bBitStr[8],bCPUstr[8];
 UCHAR bCPUmodeStr[8],bCodeStr[8],bMMstr[8],bLibStr[8];
 UCHAR bMonthStr[8],bDayStr[8],bYearStr[8],bTimeStr[8];

 GETPROCADDR(tmpFnPtr,dllHandle,"BltGetVar");
 if (rc) {
    printf("BltGetVar GetProcAddr failed, rc=%u\n",rc);
    goto ExitNow;
 }
 BltGetVar = (BLTGETVAR)tmpFnPtr;
 sscanf((UCHAR*)BltGetVar(BLTVAR_VERSION,0),
     "%7s %7s %7s %7s %7s %7s %7s %7s %7s %7s %7s %7s %7s",
     bVerStr,bBuildStr,bOSstr,bBitStr,bCPUstr,bCPUmodeStr,bCodeStr,bMMstr,bLibStr,
     bMonthStr,bDayStr,bYearStr,bTimeStr);
 printf("[Bullet %s][%s %s %s][%s %s-bit][%s-%s]\n",bVerStr,bMonthStr,bDayStr,bYearStr,
                                                   bOSstr,bBitStr,bCodeStr,bBuildStr);
}

ExitNow:
 UNLOADLIBRARY(dllHandle);
 if (rc) {
    printf("unload DLL failed, rc=%u\n",rc);
 }

 printf("Done.\n");
 return (int)rc;
}
