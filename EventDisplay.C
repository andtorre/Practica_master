/*Macro de ejemplo para visualizar eventos y como afectan los cortes que queremos establecer a la selección de eventos*/

#include"lib/headers.h"

void Analyse(int OV, int wvl, double range1, double range2, std::vector<int> triggerchannels, string adc="DT5725")
{
	/* Argumentos de myrun:
		1. Canal. Irrelevante ahora
		2. Vector con la cantidad de .root a procesar. Lo hacemos de 1 en 1 en esta ocasión.
		3. Modelo de ADC usado. Eso implicaría un factor de conversión diferente para el cálculo de la carga.
		4. - 5. Rango de tiempos empleado para cortes o para integración de carga.
		6. Nº de bines para calcular el pedestal. Dependerá del sampling. Para el ADC 1bin = 4ns. Para el Osc puede variar, en este caso 1bin = 4ns
		7. Nº de eventos a procesar. El -1 indica que se procesan todos los almacenados en el .root
	*/
	// Se carga el .root que se quiera. Yo me dejo comentadas las posibles opciones para que sea más rápido escoger.
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

  	myrun.PlotPeakTimes(); // Histograma con los tiempos en los que se dan la amplitud máxima de cada evento
  	myrun.PlotPedSTDs(); // Histograma de la STD del pedestal de cada evento

	// --- Aplicamos cortes (hay muchos más, estos son unos ejemplos) ---
  	// myrun.SetCutPeakTimeRange(range1,range2); // Cortamos señales con el máximo de amplitud fuera de los tiempos indicados (en sec).
   	// myrun.SetCutPedSTD(); // Cortamos señales con Pedestal STD > pedcut en ADC
   	// myrun.SetCutMaxAmplitudeRange(-1000,1000); //Pedimos una amplitud mínima y máxima para la amplitud en el rango de tiempo indicado en Analyse

	// --- Plots de las waveforms individuales ---
  	myrun.LoopWaveforms(0,"",NULL); // Marca cuales nos estaríamos quitando con nuestros cortes. Para salir del loop se usa la letra c
	
	// --- Plot del espectro de carga ---
	auto h = myrun.TH1Charge(0,"Range","V*ns");
	h->Draw();gPad->Update();
	lets_pause();
	myrun.autofit(h); // Fit automático de las gaussianas que le indiquemos
	lets_pause();

 	myrun.Close();
}

void EventDisplay()
{
	/*Argumentos:
	1. OV
	2. Wavelength
	3. tiempo inicial donde empezar a buscar la señal de trigger (s) - range1
	4. tiempo final donde dejar de buscar la señal de trigger (s) - range2
	4. Vector con los canales de trigger. Irrelevante ahora
	5. Versión del ADC usada (DT5725 u Osciloscopio)
	*/
	Analyse(3, 0 /*wvl en caso de ser necesaria*/, 0.02e-6, 0.24e-6, {0}, "Osc");

}
