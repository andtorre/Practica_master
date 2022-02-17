/*
Macro para obtener la waveform promedio de un run.
La macro lee los ficheros con los runes guardados en ROOT, y crea un fichero en AnalysisROOT con la waveform media de todos los eventos
*/
#include "lib/headers.h"

void Analyse_min_max_given(int OV, int wvl, double range1, double range2, std::vector<int> triggerchannels,int NEvts=-1, string adc="DT5725")
{
	ana::Run_t myrun(1, {{Form("ROOT/OV%i_SPE_RT.root", OV),"ADC0"}}, adc, range1, range2, 10, -1);
 	// ana::Run_t myrun(1, {{Form("ROOT/OV%i_SPE_LN2.root", OV),"ADC0"}}, adc, range1, range2, 10, -1);
	// ana::Run_t myrun(ch, {{Form("ROOT/%inm_OV%i_RT.root", wvl, OV),"ADC0"}}, adc, range1, range2, 50, -1);
	// ana::Run_t myrun(ch, {{Form("ROOT/%inm_OV%i_LN2.root", wvl, OV),"ADC0"}}, adc, range1, range2, 50, -1);

	std::vector<double> Gains={1,1,1}; // Ganancias en pC. No relevante en este caso
	std::vector<double> SPEAmp={1,1,1}; // Amplitud del SPE en cuentas de ADC. No relevante en este caso
	myrun.SetGains(Gains); // Necesario para evitar errores en funciones que usamos. No relevante en este caso
  	myrun.SetSPEAmps(SPEAmp); // Necesario para evitar errores en funciones que usamos. No relevante en este caso
  	myrun.Process();
  	myrun.SelectChannels({0}); // Selecciona los canales a procesar incluidos en myrun, en estos casos siempre habrá 1

	// --- Aplicamos cortes (hay muchos más, estos son unos ejemplos) ---
  	// myrun.SetCutPeakTimeRange(range1,range2); // Cortamos señales con el máximo de amplitud fuera de los tiempos indicados (en sec).
   	// myrun.SetCutPedSTD(); // Cortamos señales con Pedestal STD > pedcut en ADC
   	// myrun.SetCutMaxAmplitudeRange(-1000,1000); //Pedimos una amplitud mínima y máxima para la amplitud en el rango de tiempo indicado en Analyse
	// myrun.SetCutTriggerWaveformCuts({0}); // Para seleccionar únicamente los eventos tras los cortes

	myrun.ParSet->setADCAmplitudeThreshold(-1000); // Para alinear las señales dado un valor de amplitud que deban superar. Valor absurdo para no desplazar los eventos
	myrun.SetMaximumWaveformsToProcess(NEvts); // Número de waveforms a añadir
  
	// Función para generar la waveform media y volcarla a un .root por si se quiere recuperar el histograma
	myrun.Plot36(0, OV, wvl, "ScintProfFirstSignalBin",Form("AnalysisROOT/OV%i_RT_ScintProfFirstSignalBin.root", OV), 0,1);
	// myrun.Plot36(0, OV, wvl, "ScintProfFirstSignalBin",Form("AnalysisROOT/OV%i_LN2_ScintProfFirstSignalBin.root", OV), 0,1);
	// myrun.Plot36(0, OV, wvl, "ScintProfFirstSignalBin",Form("AnalysisROOT/OV%i_%inm_RT_ScintProfFirstSignalBin.root", OV), 0,1);
	// myrun.Plot36(0, OV, wvl, "ScintProfFirstSignalBin",Form("AnalysisROOT/OV%i_%inm_LN2_ScintProfFirstSignalBin.root", OV), 0,1);
  
	myrun.Close();
}


void AverageWaveform()
{
	vector<int> OV = {3,4,5};
	vector<int> wvl = {260, 370, 405, 420};
	
	for (int ov:OV){
		Analyse_min_max_given(ov, 1, 0.04e-6, 0.24e-6, {0}, -1, "Osc");
		next_run();
	} //*/
}
