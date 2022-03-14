/*
Esta macro preprocesa los datos, convirtiendo los ficheros .txt en un fichero root con una estructura determinada para facilitar el análisis con las macros.
*/

#include"lib/FirstDataProcess.h"

void FirstDataProcess()
{
	vector<int> OV = {3, 4, 5}; // OV a los que se toman los datos
	vector<int> wvl = {270, 360, 405, 420}; // wavelengths de las fuentes de luz
	string ipath = ""; // path donde se encuentran las carpetas de RT y LN2 con los .txt

	ReadAndDumpBinary("test_practica.dat","test_practica.root");
	// MEDIDAS A RT
	// ------------------ Calibracion ----------------------------
	/*for (int ov:OV) {
		ReadAndDumpBinary(ipath+Form("/RT/Calibracion/OV%i--",ov), Form("ROOT/OV%i_SPE_RT.root",ov),-1,-1);
	} //*/
	// ------------------ Luz ----------------------------
	/*for(int wl:wvl) {for (int ov:OV) {
		ReadAndDumpOscilloscope(ipath+Form("/RT/LUZ/%i/OV%i--",wl,ov), Form("ROOT/%inm_OV%i_RT.root",wl,ov),-1,-1);
	}} //*/


	// MEDIDAS EN LN2
	// ------------------ Calibracion ----------------------------
	/*for (int ov:OV) {
		ReadAndDumpOscilloscope(ipath+Form("/LN2/Calibracion/OV%i--",ov), Form("ROOT/OV%i_SPE_LN2.root",ov),-1,-1);
	} //*/

	// ------------------ Luz ----------------------------
	/*for(int wl:wvl) {for (int ov:OV) {
		ReadAndDumpOscilloscope(ipath+Form("/LN2/LUZ/%i/OV%i--",wl,ov), Form("ROOT/%inm_OV%i_LN2.root",wl,ov),-1,-1);
	}} //*/
}

