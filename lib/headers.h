#include<fstream>

using namespace std;
void lets_pause (){
	TTimer * timer = new TTimer("gSystem->ProcessEvents();", 50, kFALSE);
	timer->TurnOn();
	timer->Reset();
	std::cout << "q/Q to quit, other to continuee: ";
	char kkey;
	std::cin.get(kkey);
	if( kkey == 'q' || kkey == 'Q') 			 throw std::exception(); //std::exit(0); //gSystem->Exit(0); // 
	timer->TurnOff();
	delete timer;
}

void next_run (){
	TTimer * timer = new TTimer("gSystem->ProcessEvents();", 50, kFALSE);
	timer->TurnOn();
	timer->Reset();
	std::cout << "Next run? (Q/q to quit): ";
	char kkey;
	std::cin.get(kkey);
	if( kkey == 'q' || kkey == 'Q') 			 throw std::exception(); //std::exit(0); //gSystem->Exit(0); // 
	timer->TurnOff();
	delete timer;
}

int lets_pause (int &counter){
	TTimer * timer = new TTimer("gSystem->ProcessEvents();", 50, kFALSE);
	timer->TurnOn();
	timer->Reset();
	std::cout << "q/Q to quit, r to go back to last item, c to break loop, other to not print and keep going.";
	char kkey;
	std::cin.get(kkey);
	if( kkey == 'q' || kkey == 'Q') 			throw std::exception();//std::exit(0); //gSystem->Exit(0); // 
	if( kkey == 'r' || kkey == 'R') 			counter=counter-2;
	timer->TurnOff();
	delete timer;
        if( kkey == 'C' || kkey == 'c')  return 1;
        else return 0;
}
#include"mylib.h"
#include"lib.h"
#include"Waveform.h"
#include"Event.h"
#include"Cuts.h"
#include"NoiseTools.h"
#include"Run.h"
#include"RunCollection.h"
#include"Decon.h"
