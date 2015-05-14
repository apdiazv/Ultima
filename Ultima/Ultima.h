#include <windows.h>
#include <string>
#include "Sqlite3/sqlite3.h"

using namespace std;

//Funciones exportadas
__declspec(dllexport) double __stdcall Multiply(double a, double b);
__declspec(dllexport) int __stdcall Texto(LPSTR a);
__declspec(dllexport) int __stdcall testInsert(LPSTR a, LPSTR b);
__declspec(dllexport) LPSTR __stdcall retornaString(LPSTR a);
__declspec(dllexport) int __stdcall loadOP(LPSTR path);
__declspec(dllexport) long int __stdcall timestampToLong(LPSTR tmstmp);
__declspec(dllexport) int __stdcall arreglaSabana();
__declspec(dllexport) long __stdcall insertaSabana();
__declspec(dllexport) long __stdcall actualizaTasasCtasCorrientes(long fecha, long periodo);
__declspec(dllexport) long __stdcall actualizaTasasSobregiros(long fecha);
__declspec(dllexport) long __stdcall actualizaTasasCapital(long fecha);
__declspec(dllexport) long __stdcall actualizaTasasValesVista(long fecha);
__declspec(dllexport) long __stdcall actualizaTasasLetPag(long fecha);
__declspec(dllexport) long __stdcall actualizaTasasBonosTrading(long fecha);


//Funciones no exportadas
sqlite3* openDb();
long int stringToLong(string strng);
string longToString(long i);
long getBookDtFromSBIF(sqlite3* db);