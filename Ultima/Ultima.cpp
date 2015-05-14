#include "Ultima.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

//Funciones Exportadas
__declspec(dllexport) double __stdcall Multiply(double a, double b)
{
	return a * b;
}

__declspec(dllexport) int __stdcall Texto(LPSTR a)
{
	cout << a;
	return 0;
}
__declspec(dllexport) int __stdcall testInsert(LPSTR a, double b)
{
	sqlite3 *db = openDb();
	sqlite3_stmt *stmt;
	std::string qry = "INSERT INTO PruebaCarga (Campo1, Campo2) VALUES (?, ?)";
	int rc = sqlite3_prepare_v2(db, qry.c_str(), -1, &stmt, NULL);
	if (rc != SQLITE_OK) {
		sqlite3_close_v2(db);
		return 2;
	}
	
	rc = sqlite3_bind_text(stmt, 1, a, -1, SQLITE_STATIC);
	rc = sqlite3_bind_double(stmt, 2, b);
	
	rc = sqlite3_step(stmt);
	if ((rc != SQLITE_DONE) && (rc != SQLITE_ROW)) {
		sqlite3_close_v2(db);
		return 3;
	}

	sqlite3_finalize(stmt);
	
	sqlite3_close(db);
	sqlite3_shutdown();

	return 0;
}
__declspec(dllexport) LPSTR __stdcall retornaString(LPSTR a)
{
	return a;
}

__declspec(dllexport) long int __stdcall timestampToLong(LPSTR tmstmp)
{
	string aux = string(tmstmp);
	return stringToLong(aux.substr(0,4) + aux.substr(5,2) + aux.substr(8,2));
}
__declspec(dllexport) int __stdcall arreglaSabana()
{
	char* errorMessage;
	sqlite3 *db = openDb();
	string qry;

	sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, &errorMessage);
	qry = "UPDATE SBIF_A_INT_OPE SET BOOK_DT = arreglaFecha(BOOK_DT) WHERE BOOK_DT IS NOT NULL";
	sqlite3_exec(db, qry.c_str(), NULL, NULL, &errorMessage);
	//sqlite3_exec(db, "COMMIT TRANSACTION", NULL, NULL, &errorMessage);

	//sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, &errorMessage);
	qry = "UPDATE SBIF_A_INT_OPE SET STRT_DT = arreglaFecha(STRT_DT) WHERE STRT_DT IS NOT NULL";
	sqlite3_exec(db, qry.c_str(), NULL, NULL, &errorMessage);
	//sqlite3_exec(db, "COMMIT TRANSACTION", NULL, NULL, &errorMessage);	

	//sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, &errorMessage);	
	qry = "UPDATE SBIF_A_INT_OPE SET END_DT = arreglaFecha(END_DT) WHERE END_DT IS NOT NULL";
	sqlite3_exec(db, qry.c_str(), NULL, NULL, &errorMessage);
	//sqlite3_exec(db, "COMMIT TRANSACTION", NULL, NULL, &errorMessage);

	//sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, &errorMessage);
	qry = "UPDATE SBIF_A_INT_OPE SET SRC_ID = trim(SRC_ID)";
	sqlite3_exec(db, qry.c_str(), NULL, NULL, &errorMessage);
	sqlite3_exec(db, "COMMIT TRANSACTION", NULL, NULL, &errorMessage);

	//sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, &errorMessage);
	qry = "UPDATE SBIF_A_INT_OPE SET PROD = trim(PROD)";
	sqlite3_exec(db, qry.c_str(), NULL, NULL, &errorMessage);
	sqlite3_exec(db, "COMMIT TRANSACTION", NULL, NULL, &errorMessage);

	//sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, &errorMessage);
	//qry = "UPDATE SBIF_A_INT_OPE SET CON_NO = trim(CON_NO)";
	//sqlite3_exec(db, qry.c_str(), NULL, NULL, &errorMessage);
	//sqlite3_exec(db, "COMMIT TRANSACTION", NULL, NULL, &errorMessage);

	//sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, &errorMessage);
	qry = "UPDATE SBIF_A_INT_OPE SET CCY = trim(CCY)";
	sqlite3_exec(db, qry.c_str(), NULL, NULL, &errorMessage);
	//sqlite3_exec(db, "COMMIT TRANSACTION", NULL, NULL, &errorMessage);
	
	//sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, &errorMessage);
	qry = "UPDATE SBIF_A_INT_OPE SET IDENT_CLI = trim(IDENT_CLI)";
	sqlite3_exec(db, qry.c_str(), NULL, NULL, &errorMessage);
	//sqlite3_exec(db, "COMMIT TRANSACTION", NULL, NULL, &errorMessage);

	sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, &errorMessage);
	qry = "UPDATE SBIF_A_INT_OPE SET OCY_NOM_AMT = 0 WHERE CON_NO IN (SELECT CON_NO FROM Ops_OP29_EnCuentasDeOrden)";
	sqlite3_exec(db, qry.c_str(), NULL, NULL, &errorMessage);
	sqlite3_exec(db, "COMMIT TRANSACTION", NULL, NULL, &errorMessage);

	sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, &errorMessage);
	qry = "UPDATE SBIF_A_INT_OPE SET OCY_NOM_AMT = 0 WHERE CON_NO IN (SELECT CON_NO FROM Ops_OP07_NoIncluir)";
	sqlite3_exec(db, qry.c_str(), NULL, NULL, &errorMessage);
	sqlite3_exec(db, "COMMIT TRANSACTION", NULL, NULL, &errorMessage);

	sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, &errorMessage);
	qry = "UPDATE SBIF_A_INT_OPE SET POOL_RT = (SELECT Valor FROM ValorTasa WHERE idTasa = 1 AND Fecha = BOOK_DT) WHERE SRC_ID = \'OP03\' AND CCY = \'CHEZ\'";
	sqlite3_exec(db, qry.c_str(), NULL, NULL, &errorMessage);
	sqlite3_exec(db, "COMMIT TRANSACTION", NULL, NULL, &errorMessage);

	sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, &errorMessage);
	qry = "UPDATE SBIF_A_INT_OPE SET POOL_RT = (SELECT Valor FROM ValorTasa WHERE idTasa = 2 AND Fecha = BOOK_DT) WHERE SRC_ID = \'OP03\' AND CCY = \'US.D\'";
	sqlite3_exec(db, qry.c_str(), NULL, NULL, &errorMessage);
	sqlite3_exec(db, "COMMIT TRANSACTION", NULL, NULL, &errorMessage);

	sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, &errorMessage);
	qry = "UPDATE SBIF_A_INT_OPE SET POOL_RT = 0 WHERE SRC_ID = \'OP03\' AND POOL_RT ISNULL";
	sqlite3_exec(db, qry.c_str(), NULL, NULL, &errorMessage);
	sqlite3_exec(db, "COMMIT TRANSACTION", NULL, NULL, &errorMessage);

	sqlite3_close(db);
	sqlite3_shutdown();
	
	return 0;			//Retorna el código de éxito
}

__declspec(dllexport) long __stdcall insertaSabana()
{
	char* errorMessage;
	sqlite3 *db = openDb();
	sqlite3_stmt *stmt;
	string qry;
	int rc;

	long bookDT = getBookDtFromSBIF(db);
	qry = "DELETE FROM ConsolidadoOperacionesSigir WHERE BOOK_DT = " + longToString(bookDT) + " AND SRC_ID <> \'OP100\' AND SRC_ID <> \'OP101\' AND SRC_ID <> \'OP102\' AND SRC_ID <> \'OP103\'";
	rc = sqlite3_prepare_v2(db, qry.c_str(), -1, &stmt, NULL);
	//rc = sqlite3_bind_int64(stmt, 1, bookDT);
	sqlite3_exec(db, qry.c_str(), NULL, NULL, &errorMessage);
	sqlite3_finalize(stmt);

	qry = "INSERT INTO ConsolidadoOperacionesSigir (BOOK_DT, SRC_ID, CON_STA, DLNQ_STA, PROD, IDENT_CLI, CC, CON_NO, STRT_DT, END_DT, CCY, OCY_NOM_AMT_SIGN, OCY_NOM_AMT, OCY_INT_AMT_SING, OCY_INT_AMT, INT_RT, RT_METH, POOL_RT, OCY_ORIG_NOM_AMT) SELECT BOOK_DT, SRC_ID, CON_STA, DLNQ_STA, PROD, IDENT_CLI, CC, CON_NO, STRT_DT, END_DT, CCY, OCY_NOM_AMT_SIGN, OCY_NOM_AMT, OCY_INT_AMT_SING, OCY_INT_AMT, INT_RT, RT_METH, POOL_RT, OCY_ORIG_NOM_AMT FROM SBIF_A_INT_OPE";
	sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, &errorMessage);
	{
		sqlite3_exec(db, qry.c_str(), NULL, NULL, &errorMessage);
	}
	sqlite3_exec(db, "COMMIT TRANSACTION", NULL, NULL, &errorMessage);

	sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, &errorMessage);
	qry = "UPDATE ConsolidadoOperacionesSigir SET POOL_RT = INT_RT WHERE PROD = \'PHIPOT\'";
	sqlite3_exec(db, qry.c_str(), NULL, NULL, &errorMessage);
	sqlite3_exec(db, "COMMIT TRANSACTION", NULL, NULL, &errorMessage);

	/*sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, &errorMessage);
	qry = "UPDATE ConsolidadoOperacionesSigir SET POOL_RT = INT_RT WHERE PROD = \'BONO\'";
	sqlite3_exec(db, qry.c_str(), NULL, NULL, &errorMessage);
	sqlite3_exec(db, "COMMIT TRANSACTION", NULL, NULL, &errorMessage);*/

	/*sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, &errorMessage);
	qry = "UPDATE ConsolidadoOperacionesSigir SET POOL_RT = INT_RT WHERE PROD = \'DEP\'";
	sqlite3_exec(db, qry.c_str(), NULL, NULL, &errorMessage);
	sqlite3_exec(db, "COMMIT TRANSACTION", NULL, NULL, &errorMessage);
	*/

	sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, &errorMessage);
	qry = "DELETE FROM ConsolidadoOperacionesSigir WHERE SRC_ID = \'OP14\'";
	sqlite3_exec(db, qry.c_str(), NULL, NULL, &errorMessage);
	sqlite3_exec(db, "COMMIT TRANSACTION", NULL, NULL, &errorMessage);

	sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, &errorMessage);
	qry = "DELETE FROM ConsolidadoOperacionesSigir WHERE SRC_ID = \'OP15\'";
	sqlite3_exec(db, qry.c_str(), NULL, NULL, &errorMessage);
	sqlite3_exec(db, "COMMIT TRANSACTION", NULL, NULL, &errorMessage);

	sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, &errorMessage);
	qry = "DELETE FROM ConsolidadoOperacionesSigir WHERE SRC_ID = \'OP52\'";
	sqlite3_exec(db, qry.c_str(), NULL, NULL, &errorMessage);
	sqlite3_exec(db, "COMMIT TRANSACTION", NULL, NULL, &errorMessage);

	sqlite3_close(db);
	sqlite3_shutdown();

	return 0;
}

__declspec(dllexport) long __stdcall actualizaTasasCtasCorrientes(long fecha, long periodo)
{
	char* errorMessage;
	sqlite3 *db = openDb();
	sqlite3_stmt *stmt;
	string qry;
	int rc;
	string strFecha;
	string strPeriodo;
	strFecha = longToString(fecha);
	strPeriodo = longToString(periodo);
	//qry = "UPDATE ConsolidadoOperacionesSigir SET POOL_RT = 0 WHERE PROD = \'CTACTE\' AND BOOK_DT = " + strFecha;
	//rc = sqlite3_prepare_v2(db, qry.c_str(), -1, &stmt, NULL);
	//sqlite3_exec(db, qry.c_str(), NULL, NULL, &errorMessage);
	//sqlite3_finalize(stmt);
	
	qry = "UPDATE ConsolidadoOperacionesSigir SET POOL_RT = (SELECT Tasa FROM TasaRemuneracionCtaCorriente WHERE Periodo = " + strPeriodo + " AND IdSegmento = (SELECT IdSegmento FROM ClienteSegmento WHERE IDENT_CLI = ConsolidadoOperacionesSigir.IDENT_CLI)) WHERE PROD = \'CTACTE\' AND CCY = \'CHEZ\' AND BOOK_DT = " + strFecha;

	rc = sqlite3_prepare_v2(db, qry.c_str(), -1, &stmt, NULL);
	sqlite3_exec(db, qry.c_str(), NULL, NULL, &errorMessage);
	sqlite3_finalize(stmt);

	qry = "UPDATE ConsolidadoOperacionesSigir SET POOL_RT = (SELECT Valor FROM ValorTasa WHERE idTasa = 1 AND Fecha = " + strFecha + ") WHERE PROD = \'CTACTE\' AND CCY = \'CHEZ\' AND BOOK_DT = " + strFecha + " AND POOL_RT ISNULL";

	rc = sqlite3_prepare_v2(db, qry.c_str(), -1, &stmt, NULL);
	sqlite3_exec(db, qry.c_str(), NULL, NULL, &errorMessage);

	sqlite3_finalize(stmt);

	qry = "UPDATE ConsolidadoOperacionesSigir SET POOL_RT = (SELECT Valor FROM ValorTasa WHERE idTasa = 2 AND Fecha = " + strFecha + ") WHERE PROD = \'CTACTE\' AND CCY = \'US.D\' AND BOOK_DT = " + strFecha + " AND POOL_RT ISNULL";

	rc = sqlite3_prepare_v2(db, qry.c_str(), -1, &stmt, NULL);
	sqlite3_exec(db, qry.c_str(), NULL, NULL, &errorMessage);

	sqlite3_finalize(stmt);

	sqlite3_close(db);
	sqlite3_shutdown();

	return 0;
}

__declspec(dllexport) long __stdcall actualizaTasasSobregiros(long fecha)
{
	char* errorMessage;
	sqlite3 *db = openDb();
	sqlite3_stmt *stmt;
	string qry;
	int rc;
	string strFecha;
	strFecha = longToString(fecha);
	qry = "UPDATE ConsolidadoOperacionesSigir SET POOL_RT = (SELECT Valor FROM ValorTasa WHERE idTasa = 1 AND Fecha = ConsolidadoOperacionesSigir.BOOK_DT) WHERE SRC_ID = \'OP30\' AND BOOK_DT = " + strFecha;

	rc = sqlite3_prepare_v2(db, qry.c_str(), -1, &stmt, NULL);

	sqlite3_exec(db, qry.c_str(), NULL, NULL, &errorMessage);

	sqlite3_finalize(stmt);
	sqlite3_close(db);
	sqlite3_shutdown();

	return 0;
}

__declspec(dllexport) long __stdcall actualizaTasasCapital(long fecha)
{
	char* errorMessage;
	sqlite3 *db = openDb();
	sqlite3_stmt *stmt;
	string qry;
	int rc;
	string strFecha;
	strFecha = longToString(fecha);
	qry = "UPDATE ConsolidadoOperacionesSigir SET POOL_RT = (SELECT Valor FROM ValorTasa WHERE idTasa = 1 AND Fecha = ConsolidadoOperacionesSigir.BOOK_DT) WHERE PROD = \'CAPITAL\' AND BOOK_DT = " + strFecha;

	rc = sqlite3_prepare_v2(db, qry.c_str(), -1, &stmt, NULL);

	sqlite3_exec(db, qry.c_str(), NULL, NULL, &errorMessage);

	sqlite3_finalize(stmt);
	sqlite3_close(db);
	sqlite3_shutdown();

	return 0;
}

__declspec(dllexport) long __stdcall actualizaTasasValesVista(long fecha)
{
	char* errorMessage;
	sqlite3 *db = openDb();
	sqlite3_stmt *stmt;
	string qry;
	int rc;
	string strFecha;
	strFecha = longToString(fecha);
	qry = "UPDATE ConsolidadoOperacionesSigir SET POOL_RT = (SELECT Valor FROM ValorTasa WHERE idTasa = 1 AND Fecha = ConsolidadoOperacionesSigir.BOOK_DT) WHERE SRC_ID = \'OP33\' AND BOOK_DT = " + strFecha;

	rc = sqlite3_prepare_v2(db, qry.c_str(), -1, &stmt, NULL);

	sqlite3_exec(db, qry.c_str(), NULL, NULL, &errorMessage);

	sqlite3_finalize(stmt);
	sqlite3_close(db);
	sqlite3_shutdown();

	return 0;
}

__declspec(dllexport) long __stdcall actualizaTasasLetPag(long fecha)
{
	char* errorMessage;
	sqlite3 *db = openDb();
	sqlite3_stmt *stmt;
	string qry;
	int rc;
	string strFecha;
	strFecha = longToString(fecha);
	qry = "UPDATE ConsolidadoOperacionesSigir SET POOL_RT = (SELECT Valor FROM ValorTasa WHERE idTasa = 1 AND Fecha = ConsolidadoOperacionesSigir.BOOK_DT) WHERE SRC_ID = \'OP21\' AND BOOK_DT = " + strFecha;

	rc = sqlite3_prepare_v2(db, qry.c_str(), -1, &stmt, NULL);

	sqlite3_exec(db, qry.c_str(), NULL, NULL, &errorMessage);

	sqlite3_finalize(stmt);
	sqlite3_close(db);
	sqlite3_shutdown();

	return 0;
}

__declspec(dllexport) long __stdcall actualizaTasasBonosTrading(long fecha)
{
	char* errorMessage;
	sqlite3 *db = openDb();
	sqlite3_stmt *stmt;
	string qry;
	int rc;
	string strFecha;
	strFecha = longToString(fecha);
	qry = "UPDATE ConsolidadoOperacionesSigir SET POOL_RT = (SELECT Valor FROM ValorTasa WHERE idTasa = 1 AND Fecha = ConsolidadoOperacionesSigir.BOOK_DT) WHERE PROD = \'BONTRAD\' AND BOOK_DT = " + strFecha;

	rc = sqlite3_prepare_v2(db, qry.c_str(), -1, &stmt, NULL);

	sqlite3_exec(db, qry.c_str(), NULL, NULL, &errorMessage);

	sqlite3_finalize(stmt);
	sqlite3_close(db);
	sqlite3_shutdown();

	return 0;
}
//
//Funciones no exportadas Disco C
sqlite3* openDb()
{   
	//Aquí establecemos la conexión con la base de datos y cargamos las extensiones de la 
	//DLL cuadrado.dll
	
	//Conexión a la BBDD. La variable baseDatos debe cambiarse según la instalación.
	const char* baseDatos = "C:\\Creasys\\Banking\\BackEnd\\Banking.sqlite";
	sqlite3* db;
	int rc = 0;
	sqlite3_initialize();
	rc = sqlite3_open_v2(baseDatos, &db, SQLITE_OPEN_READWRITE, NULL);
	if (rc != SQLITE_OK) {
		sqlite3_close(db);
		throw("No se pudo abrir la BBDD");
	}
	sqlite3_exec(db, "PRAGMA foreign_keys = ON", NULL, NULL, NULL);
	//Carga de las extensiones. Las variables nombreExtension y puntoEntrada deben cambiarse
	//según la instalación
	rc = sqlite3_enable_load_extension(db, 1);
	const char* nombreExtension = "C:\\Creasys\\Banking\\BackEnd\\cuadrado.dll";
	const char* puntoEntrada = "sqlite3_cuadrado_init";
	rc = sqlite3_load_extension(db, nombreExtension, puntoEntrada, NULL);
	return db;
}
long int stringToLong(string strng)
{
	istringstream ss(strng);
	long int result;
	return ss >> result ? result : 0;
}

string longToString(long i)
{
	stringstream ss;
	ss << i;
	return ss.str();
}

long getBookDtFromSBIF(sqlite3* db)
{
	sqlite3_stmt *stmt;
	string qry = "SELECT BOOK_DT FROM SBIF_A_INT_OPE";
	int rc = sqlite3_prepare_v2(db, qry.c_str(), -1, &stmt, NULL);

	if (rc != SQLITE_OK)
	{
		sqlite3_close_v2(db);
		const char* a = sqlite3_errmsg(db);
		throw("Error al preparar el statement");
	}
	
	if (sqlite3_step(stmt) == SQLITE_ROW)
	{
		long result = sqlite3_column_int(stmt, 0);
		sqlite3_finalize(stmt);
		return result;
	}

	sqlite3_finalize(stmt);
	throw("Error al ejecutar la consulta");
}