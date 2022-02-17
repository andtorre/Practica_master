

namespace waveana{

  class Pedestal_t
  {
    private:
    double Mean;
    double STD;
    double Charge;
    double Amp;
    bool StatusSTD;
    bool StatusBaseline;
    double BaselineAmpLimit;
    public:
    Pedestal_t(){}
    Pedestal_t(std::vector<double> *v, int end, int samples, double a)
    {
      BaselineAmpLimit=a;
      int i=0;
      do
      {
        ComputeBaseline(v,i*samples,samples);i++;//std::cout << i << "-  " << STD << ", Amp: " << Amp << ", Status: " << Status << " " <<std::endl;
      }while(!StatusBaseline && samples*(i+1)<end );
      i=0;
      do
      {
        ComputeSTD(v,i*samples,samples);i++;//std::cout << i << "-  " << STD << ", Amp: " << Amp << ", Status: " << Status << " " <<std::endl;
      }while(!StatusSTD && samples*(i+1)<end );

    }

    std::vector<double>PedestalVector(std::vector<double> *v, int samples, bool debug=false)
    {
      int i=0; std::vector<double>pp;
      std::vector<int> Vxmin, Vxmax; int xmin, xmax;
      do
      {
        ComputeBaseline(v,i*samples,samples);i++;//std::cout << i << "-  " << STD << ", Amp: " << Amp << ", Status: " << Status << " " <<std::endl;
        if(StatusBaseline){ pp.push_back(Mean);Vxmin.push_back(i*samples);Vxmax.push_back((i+1)*samples);}
      }while(samples*(i+1)<v->size() );
      if(debug)
      {
          TH1F * h1 = new TH1F("h1","h1",v->size(),-0.5,v->size()-0.5);
          for (int j=1;j<h1->GetSize()-1;j++) h1->SetBinContent(j,v->at(j-1));
          TH1F * h2 = new TH1F("h2","2",v->size(),-0.5,v->size()-0.5); h2->SetLineColor(2);
          std::vector<TLine*> l;
          for(int j=0; j<pp.size(); j++) l.push_back(new TLine(Vxmin[j],pp[j],Vxmax[j],pp[j]));          
          //for(int j=0; j<pp.size(); j++) for (int k=Vxmin[j]; k<Vxmax[j]; k++) h2->SetBinContent(k+1,pp[j]);
          h1->Draw("HIST");// h2->Draw("HIST SAME");
          for( int j=0; j<pp.size();j++){l[j]->SetLineColor(2);l[j]->SetLineWidth(2); l[j]->Draw();}
          lets_pause();
          delete h1;
          delete h2;
      }
      return pp;
    }
    private:
    void ComputeBaseline(std::vector<double>*v, int start, int samples)
    {
      long double sum = std::accumulate(v->begin()+start, v->begin()+start+samples, 0.0);
      Mean = sum / samples;
      // Mean = 3e-3;
      // Mean = 1e-3;
      // Mean = 0;

      long double sq_sum = std::inner_product(v->begin()+start, v->begin()+start+samples, v->begin()+start, 0.0);
      STD = std::sqrt(sq_sum / samples - Mean * Mean);
      Charge = 0.0;
      //~ Charge = sum*2e-9;
      const auto bounds = std::minmax_element(v->begin()+start, v->begin()+start+samples);
      Amp=*bounds.second-*bounds.first; //std::cout << "Min: " << *bounds.first << ", Max: " << *bounds.second << std::endl;
      if(Amp>BaselineAmpLimit) StatusBaseline=false; //6 for ADC v1720
      else StatusBaseline=true;      
    }
    void ComputeSTD(std::vector<double>*v, int start, int samples)
    {

      long double sq_sum = std::inner_product(v->begin()+start, v->begin()+start+samples, v->begin()+start, 0.0);
      long double sum = std::accumulate(v->begin()+start, v->begin()+start+samples, 0.0); sum/=samples;

      STD = std::sqrt(sq_sum / samples - sum * sum);
      const auto bounds = std::minmax_element(v->begin()+start, v->begin()+start+samples);
      double AmpTop=*bounds.second-sum;
      double AmpBot=*bounds.first-sum;
//      std::cout << "Computing ped std " << start << " " << samples << " " <<  *bounds.first-sum << " " << *bounds.second-sum << " " << STD << " " << std::sqrt(sq_sum/samples) << " " << sum << " " << sq_sum/samples-sum*sum << std::endl;
      
      if(AmpBot>6*AmpTop) StatusSTD=false;//A SPE is biasing my STD! 
      else StatusSTD=true;
    }
    public:
    double getMean(){return Mean;}
    double getSTD(){return STD;}
    double getCharge(){return Charge;}
    bool getStatus(){return StatusBaseline;}
    void setBaselineAmpLimit(double a){BaselineAmpLimit=a;}
  };

  class WaveAnaParameters
  {  public:
     int PedRange=30; /*# of samples to compute pedestal*/
     int PreTriggerRange=30;
     int TriggerRange=30;/*Range to compute the trigger charge*/
     double ConversionFactor=-(4096.0/2.0)*50.0;
     double range1=1e-6;
     double range2=2e-6;
     //~ double range3=0;
     //~ double range4=50e-9;
     
     double FixedPedestal=0;
     double t1=16e-9;
     double t2=112e-9;
     double t3=3008e-6;
     double ADCTriggerThreshold=3800;
     double ADCAmplitudeThreshold=50;
     double BaselineAmpLimit=3;
     void setPedRange(int p){PedRange=p;}
     void setTriggerRange(int p){TriggerRange=p;}
     void setConversionFactor(double p){ConversionFactor=p;}
     void setRange1(double p){range1=p;}
     void setRange2(double p){range2=p;}
     //~ void setRange3(double p){range3=p;}
     //~ void setRange4(double p){range4=p;}
     
     void setADCTriggerThreshold(double p){ADCTriggerThreshold=p;}
     void setADCAmplitudeThreshold(double p){ADCAmplitudeThreshold=p;}
     void setBaselineAmpLimit(double p){BaselineAmpLimit=p;}

     double getADCTriggerThreshold(){ return ADCTriggerThreshold;}
     double getADCAmplitudeThreshold(){ return ADCAmplitudeThreshold;}
     WaveAnaParameters(){}
  };

  class Waveform_t
  {
    private:
    double PedestalMean; //Volts,Coulombs
    double PedestalSTD;
    double PedestalCharge;
    bool   PedestalStatus;

    double TriggerCharge;
    double PreTriggerCharge=0;
    double PreTriggerSTD=0;
    double SignalToNoise;
    double SignalToNoiseAmplitude;
    double PeakTime; //peak time of the minimum in the signal
    double PeakTime_Range; //peakt time of the minimum in the signal within a given range
    double PeakTime_OutOfRange; //peakt time of the minimum in the signal out of a given range
    double PeakTime_OutOfMaxPeak; //peakt time of the minimum in the signal out of the PeakTime_Range vicinity

    double Charge; //total charge of the waveform.
    double Charge_Range; //Charge in the given range.
    double Charge_MaxPeakRange; //Charge of the max amplitude peak (t0-sampling,t0+100ns) found within the given range.
    double Charge_MaxPeakRange3bins; //Charge of the max amplitude peak (t0-sampling,t0+sampling) found within the given range.
    double Charge_MaxPeak; //Charge of the max amplitude peak (t0-20ns,t0+100ns) found within the full waveform.
    double SumOfSamples;
    double S2Charge;
    double S2AverageAmplitude;

    bool saturated;
    bool saturated2;
    double sampling;
    double timestamp;
    double FirstTimeSignalRange=-1;
    int nsamples;
    int width;

    double t1, t2, t3;
    double q1_MaxPeakRange=0.0;
    double q2_MaxPeakRange=0.0;
    double q3_MaxPeakRange=0.0;
    double qF_MaxPeakRange=0.0;
    double qI_MaxPeakRange=0.0;
    double qS_MaxPeakRange=0.0;
    double tStart_MaxPeakRange=-1;
    double tEnd_MaxPeakRange=-1;

    double MaxAmplitude; //signal amplitude of the minimum in the signal
    double MaxAmplitude_Range; //signal amplitude of the minimum in the signal within a given range
    double MaxAmplitude_OutOfRange;  //signal amplitude of the minimum in the signal out of the given range

    double MaxAmplitude_OutOfMaxPeak; //signal amplitude of the minimum in the signal out of the PeakTime_Range vicinity
    double MinimumSample;
    double MinimumSample_Range;
    double MaximumSample;
    double MaximumSample_Range;
    double FirstSampleBelowADCTriggerThresholdRange=-1;
    double FirstSampleAboveADCTriggerThresholdRange=-1;
    double FirstSampleBelowADCAmplitudeThresholdRange=-1;
    
    double SecondPeak_OutOfRange;
    
    vector<double_t> amps;
    vector<double_t> times;

    bool cut=false;
    public:
    Waveform_t(){}
    Waveform_t(std::vector<double> *t /*time*/,std::vector<double> *v /*amplitude*/, WaveAnaParameters *ParSet)
    //int PedRange/*# of samples to compute pedestal*/, int TriggerRange/*Range to compute the trigger charge*/, double ConversionFactor, double range1, double range2, double FixedPedestal=0, double t1=16e-9, double t2=112e-9, double t3=3008e-6, double adctt=3800)
    {

       double ADCTriggerThreshold=ParSet->ADCTriggerThreshold;
       double ADCAmplitudeThreshold=ParSet->ADCAmplitudeThreshold;
       int PedRange=ParSet->PedRange;
       int TriggerRange=ParSet->TriggerRange;
       int PreTriggerRange=ParSet->PreTriggerRange;
       double ConversionFactor=ParSet->ConversionFactor;
       double range1=ParSet->range1;
       double range2=ParSet->range2;
       //~ double range3=ParSet->range3;
       //~ double range4=ParSet->range4;
       
       double FixedPedestal=ParSet->FixedPedestal;
       double t1=ParSet->t1;
       double t2=ParSet->t2;
       double t3=ParSet->t3;

       sampling=t->at(1)-t->at(0);
      // sampling=2e-9;

       timestamp=t->at(0);
       nsamples=t->size();
/*       long double sum = std::accumulate(v->begin(), v->begin()+PedRange, 0.0);
       PedestalMean = sum / PedRange;

       long double sq_sum = std::inner_product(v->begin(), v->begin()+PedRange, v->begin(), 0.0);
       PedestalSTD = std::sqrt(sq_sum / PedRange - PedestalMean * PedestalMean);
       PedestalCharge = 0.0;
*/
//       cout << "Lets compute baseline computed " << endl;

       Pedestal_t p(v,(int)(range1/sampling),PedRange,ParSet->BaselineAmpLimit);
       PedestalMean = p.getMean();
       PedestalSTD = p.getSTD();
       //~ PedestalCharge = p.getCharge();
       PedestalStatus = p.getStatus();
//       cout << "Baseline computed " << PedestalMean << " " << PedestalSTD << " " << PedestalStatus << endl;
//       cout << "Lets compute TriggerCharge " << v->at(0) << " " << v->size() << " " << v[v->size()-1] << " " << PedRange << " " << PedRange+TriggerRange<< endl;

       if(PedRange+TriggerRange>=v->size()) TriggerRange=0;
       TriggerCharge=sampling*(std::accumulate(v->begin()+PedRange, v->begin()+PedRange+TriggerRange, 0.0)-TriggerRange*PedestalMean)/ConversionFactor;

//       cout << "Lets compute Charge " << v->at(0) << " " << v->size() << " " << v[v->size()-1] << endl;

       Charge = sampling*(std::accumulate(v->begin(), v->end(), 0.0) - v->size()*PedestalMean)/ConversionFactor; 
//       cout << "Lets compute sumOfSamples " << v->at(0) << " " << v->size() << " " << v[v->size()-1] << endl;
       
       SumOfSamples = (std::accumulate(v->begin(), v->end(), 0.0));
//       cout << "SumOfSamples " << SumOfSamples << endl;
        Charge_Range=0;
        double oldamp=0;
        saturated=false;
    saturated2 = false;
	int countercut=0;
	std::vector<double>saturations;
	MaximumSample=-1e3;
	MaximumSample_Range=-1e3;
	double rangemin=1e6;
	double rangemin2=1e6;
	double rangemin3=1e6; 
	
	int PeakBin=0; int PeakBin_Range=0;
    long double PreTriggerSumSQ=0;
    long double PreTriggerMean=0;
        
    double v_rbin=0;
    double v_lbin=0;

	for(int i=0; i<v->size();i++) // Valores con la media del pedestal (NECESARIO PARA SACAR LA WAVEFORM MEDIA!!!!)
	{
		if (MaximumSample<v->at(i))MaximumSample=v->at(i);
                if(t->at(i)>range1&&t->at(i)<range2){if (MaximumSample_Range<v->at(i)){MaximumSample_Range=v->at(i);}}

		if (rangemin>=v->at(i)){rangemin=v->at(i);PeakTime=t->at(i); PeakBin=i;}
		if (oldamp==v->at(i)) {countercut++;}
		else {oldamp=v->at(i); countercut=0;}
		if (countercut>4){ saturations.push_back(v->at(i));}
                if(t->at(i)>range1&&t->at(i)<range2)Charge_Range+=(v->at(i)-PedestalMean);
                // if(t->at(i)>range1&&t->at(i)<range2)Charge_Range+=(v->at(i));
                if(t->at(i)>range1&&t->at(i)<range2){if (rangemin2>v->at(i)){rangemin2=v->at(i);PeakTime_Range=t->at(i); PeakBin_Range=i;}}
                else{if (rangemin3>v->at(i)){rangemin3=v->at(i);PeakTime_OutOfRange=t->at(i);}}
                if(t->at(i)>range1&&t->at(i)<range2) if(FirstSampleBelowADCTriggerThresholdRange==-1 && v->at(i)<ADCTriggerThreshold ) FirstSampleBelowADCTriggerThresholdRange=t->at(i);
                if(t->at(i)>range1&&t->at(i)<range2) if(FirstSampleAboveADCTriggerThresholdRange==-1 && v->at(i)>ADCTriggerThreshold ) FirstSampleAboveADCTriggerThresholdRange=t->at(i);
                if(t->at(i)>range1&&t->at(i)<range2) if(FirstSampleBelowADCAmplitudeThresholdRange==-1 && v->at(i)<PedestalMean - ADCAmplitudeThreshold ) FirstSampleBelowADCAmplitudeThresholdRange=t->at(i);
                if(t->at(i)>range1&&t->at(i)<range2) if(FirstTimeSignalRange==-1 && v->at(i)<PedestalMean-5*PedestalSTD ) FirstTimeSignalRange=t->at(i);
                if(t->at(i)>=range1-PreTriggerRange*sampling && t->at(i)<range1) {PreTriggerCharge+=v->at(i)-PedestalMean; PreTriggerSumSQ+=(v->at(i))*(v->at(i));PreTriggerMean+=v->at(i);}
               
	
	amps.push_back(-(v->at(i)-PedestalMean));
	times.push_back(t->at(i));
	}//*/
	
	//~ for(int i=0; i<v->size();i++) // Valores sin la media del pedestal
	//~ {
		//~ if (MaximumSample<v->at(i))MaximumSample=v->at(i);
                //~ if(t->at(i)>range1&&t->at(i)<range2){if (MaximumSample_Range<v->at(i)){MaximumSample_Range=v->at(i);}}

		//~ if (rangemin>=v->at(i)){rangemin=v->at(i);PeakTime=t->at(i); PeakBin=i;}
		//~ if (oldamp==v->at(i)) {countercut++;}
		//~ else {oldamp=v->at(i); countercut=0;}
		//~ if (countercut>4){ saturations.push_back(v->at(i));}
                //~ if(t->at(i)>range1&&t->at(i)<range2)Charge_Range+=(v->at(i));
                //~ if(t->at(i)>range1&&t->at(i)<range2){if (rangemin2>v->at(i)){rangemin2=v->at(i);PeakTime_Range=t->at(i); PeakBin_Range=i;}}
                //~ else{if (rangemin3>v->at(i)){rangemin3=v->at(i);PeakTime_OutOfRange=t->at(i);}}
                //~ if(t->at(i)>range1&&t->at(i)<range2) if(FirstSampleBelowADCTriggerThresholdRange==-1 && v->at(i)<ADCTriggerThreshold ) FirstSampleBelowADCTriggerThresholdRange=t->at(i);
                //~ if(t->at(i)>range1&&t->at(i)<range2) if(FirstSampleAboveADCTriggerThresholdRange==-1 && v->at(i)>ADCTriggerThreshold ) FirstSampleAboveADCTriggerThresholdRange=t->at(i);
                //~ if(t->at(i)>range1&&t->at(i)<range2) if(FirstSampleBelowADCAmplitudeThresholdRange==-1 && v->at(i)<ADCAmplitudeThreshold ) FirstSampleBelowADCAmplitudeThresholdRange=t->at(i);
                //~ if(t->at(i)>range1&&t->at(i)<range2) if(FirstTimeSignalRange==-1 && v->at(i)<5*PedestalSTD ) FirstTimeSignalRange=t->at(i);
                //~ if(t->at(i)>=range1-PreTriggerRange*sampling && t->at(i)<range1) {PreTriggerCharge+=v->at(i); PreTriggerSumSQ+=(v->at(i))*(v->at(i));PreTriggerMean+=v->at(i);}
               
	
	//~ amps.push_back(-(v->at(i)-PedestalMean));
	//~ times.push_back(t->at(i));
	//~ }
	
	for(int i=0; i<v->size()-3;i++) if (v->at(i)==v->at(i+1)&&v->at(i)>=MaximumSample) saturated2=true;
	
	for(int i=0; i<v->size()-1;i++) {
		if(t->at(i)>range2){ if(rangemin3>v->at(i)){
				v_lbin = v->at(i-1); v_rbin = v->at(i+1);
				if (v->at(i)<=v_lbin && v->at(i)<=v_rbin) rangemin3 = v->at(i);}
				else continue;}
	}
	
        PreTriggerCharge=sampling*PreTriggerCharge/ConversionFactor;
        PreTriggerMean/=PreTriggerRange;
        PreTriggerSTD=TMath::Sqrt(PreTriggerSumSQ / PreTriggerRange - PreTriggerMean * PreTriggerMean);
        
        Charge_Range=sampling*Charge_Range/ConversionFactor;
        //~ Charge_Range=(sampling*Charge_Range-PedestalCharge)/ConversionFactor;
        
	double rangemin_OutOfMaxPeak=1e6;
        Charge_MaxPeakRange=0;
        Charge_MaxPeakRange3bins=0;
        Charge_MaxPeak=0;
        S2Charge=0;
        int q1_MaxPeakRange_counter=0;
        int q2_MaxPeakRange_counter=0;
        int q3_MaxPeakRange_counter=0;
        S2AverageAmplitude=0; int S2AverageAmplitude_counter=0; double Tolerance=1.0;

        for(int i=PeakBin_Range  ; i<v->size() && v->at(i)<=PedestalMean-Tolerance; i++){Charge_MaxPeakRange+=v->at(i)-PedestalMean; tEnd_MaxPeakRange=t->at(i);}
        for(int i=PeakBin_Range; i>=0       && v->at(i)<=PedestalMean-Tolerance; i--) {Charge_MaxPeakRange+=v->at(i)-PedestalMean; tStart_MaxPeakRange=t->at(i);}

        for(int i=0; i<v->size();i++)
        {

           if(t->at(i)<=PeakTime_Range-sampling||t->at(i)>PeakTime_Range+100e-9){ if (rangemin_OutOfMaxPeak>v->at(i)){rangemin_OutOfMaxPeak=v->at(i);PeakTime_OutOfMaxPeak=t->at(i);}}

           if(t->at(i)>=PeakTime_Range-sampling&&t->at(i)<=PeakTime_Range+sampling){ Charge_MaxPeakRange3bins+=v->at(i)-PedestalMean;}

           if(t->at(i)>PeakTime+8e-6&&t->at(i)<PeakTime+15e-6){ S2AverageAmplitude+=v->at(i); S2AverageAmplitude_counter++;}
           if(t->at(i)>PeakTime-20e-9&&t->at(i)<PeakTime+100e-9){ Charge_MaxPeak+=v->at(i)-PedestalMean;}

           if(t->at(i)>=PeakTime_Range-sampling&&t->at(i)<PeakTime_Range+t1){ q1_MaxPeakRange+=v->at(i)-PedestalMean; q1_MaxPeakRange_counter++;}

          //  if(t->at(i)>=PeakTime_Range-sampling&&t->at(i)<PeakTime_Range+t2){ q2_MaxPeakRange+=v->at(i)-PedestalMean; q2_MaxPeakRange_counter++;}
          if(t->at(i)>=0.2e-6&&t->at(i)<0.7e-6){ q2_MaxPeakRange+=v->at(i)-PedestalMean; q2_MaxPeakRange_counter++;}
          // if(t->at(i)>=0.15e-6&&t->at(i)<0.99e-6){ q2_MaxPeakRange+=v->at(i); q2_MaxPeakRange_counter++;}

           //~ if(t->at(i)>=PeakTime_Range-sampling&&t->at(i)<PeakTime_Range+t3){ q3_MaxPeakRange+=v->at(i)-PedestalMean; q3_MaxPeakRange_counter++;}
           if(t->at(i)>=0.05e-6&&t->at(i)<0.24e-6){ q3_MaxPeakRange+=v->at(i)-PedestalMean; q3_MaxPeakRange_counter++;}
          // if(t->at(i)>=0.0e-6&&t->at(i)<0.5e-6){ q3_MaxPeakRange+=v->at(i)-PedestalMean; q3_MaxPeakRange_counter++;}

        }
        S2Charge=sampling*(S2AverageAmplitude  - S2AverageAmplitude_counter*PedestalMean)/ConversionFactor; 
        S2AverageAmplitude=-(S2AverageAmplitude - S2AverageAmplitude_counter*PedestalMean) /S2AverageAmplitude_counter;
        
        Charge_MaxPeakRange=sampling*Charge_MaxPeakRange/ConversionFactor;
        Charge_MaxPeakRange3bins=sampling*Charge_MaxPeakRange3bins/ConversionFactor;
        q1_MaxPeakRange=sampling*q1_MaxPeakRange/ConversionFactor;
        q2_MaxPeakRange=sampling*q2_MaxPeakRange/ConversionFactor;
        q3_MaxPeakRange=sampling*q3_MaxPeakRange/ConversionFactor;

        Charge_MaxPeak=sampling*Charge_MaxPeak/ConversionFactor;
        MinimumSample=rangemin;
        MinimumSample_Range=rangemin2;
	      for (int i=0; i<saturations.size();i++) if(saturations[i]==MinimumSample || saturations[i]==rangemin) saturated=true;
	      // for (int i=0; i<saturations.size();i++) if(saturations[i]==MaximumSample || saturations[i]==rangemin) saturated=true;
        // if(MinimumSample<=1) saturated=true;
        //~ for(int i=0; i<PedRange;i++) if(PedestalMean>v->at(i)) PedestalCharge+= v->at(i)-PedestalMean;
        //~ for(int i=0; i<PedRange;i++) PedestalCharge+= v->at(i)-PedestalMean;
        for(int i=0; i<PedRange;i++) PedestalCharge+= v->at(i);
        MaxAmplitude=-rangemin+PedestalMean;
        MaxAmplitude_Range=-rangemin2+PedestalMean;
        MaxAmplitude_OutOfRange=-rangemin3+PedestalMean;
        MaxAmplitude_OutOfMaxPeak=-rangemin_OutOfMaxPeak+PedestalMean;
        
        SecondPeak_OutOfRange = -rangemin3+PedestalMean;

        SignalToNoiseAmplitude = MaxAmplitude/PedestalSTD;
        PedestalCharge=PedestalCharge*sampling/ConversionFactor;
        SignalToNoise = (TriggerCharge/TriggerRange)/(PedestalCharge/PedRange);

        double width_first;
        double width_last;
//        for(int i=PeakBin; PedestalMean-v->at(i)>MaxAmplitude*0.5;i--) width_first=t->at(i);
//        for(int i=PeakBin; PedestalMean-v->at(i)>MaxAmplitude*0.5;i++) width_last=t->at(i);
//        width=width_last-width_first;

//        std::cout << "\tMaxAmplitude: " << MaxAmplitude << "\tSignalToNoiseAmplitude: " << SignalToNoiseAmplitude << std::endl;
//        lets_pause();
    }
    void Print()
    {
        std::cout << " ---Printing event --- "<< std::endl;
        std::cout << "\tPedestalMean: " << PedestalMean <<"\tPedestalSTD: " << PedestalSTD<< std::endl;
        std::cout << "\tPedestalStatus: " << PedestalStatus << std::endl;
        std::cout << "\tTriggerCharge: " << TriggerCharge << "\tPedestalCharge: " << PedestalCharge << std::endl;
        std::cout << "\tPreTriggerCharge: " << PreTriggerCharge << "\tPreTriggerSTD: " << PreTriggerSTD << std::endl;
        std::cout << "\tSignalToNoise: " << SignalToNoise << std::endl;
        std::cout << "\tMaxAmplitude: " << MaxAmplitude << "\tSignalToNoiseAmplitude: " << SignalToNoiseAmplitude << std::endl;
        std::cout << "\tPeakTime: " << PeakTime << std::endl;
        std::cout << "\tCharge: " << Charge << std::endl;
        std::cout << "\tChargeRange: " << Charge_Range << std::endl;
        std::cout << "\tMaxAmplitude_Range: " << MaxAmplitude_Range << std::endl;
        std::cout << "\tPeakTime_Range: " << PeakTime_Range << std::endl;
        std::cout << "\tCharge_MaxPeakRange: " << Charge_MaxPeakRange << std::endl;
        std::cout << "\tStart_MaxPeakRange: " << tStart_MaxPeakRange << std::endl;
        std::cout << "\tEnd_MaxPeakRange: " << tEnd_MaxPeakRange << std::endl;
        std::cout << "\tMaxAmplitude_OutOfMaxPeak: " << MaxAmplitude_OutOfMaxPeak << std::endl;
        std::cout << "\tPeakTime_OutOfMaxPeak: " << PeakTime_OutOfMaxPeak << std::endl;
        std::cout << "\tMaxAmplitude_OutOfRange: " << MaxAmplitude_OutOfRange << std::endl;
        std::cout << "\tPeakTime_OutOfRange: " << PeakTime_OutOfRange << std::endl;
        std::cout << "\tsaturated: " << saturated << std::endl;
        std::cout << "\tsampling: " << sampling << std::endl;
        std::cout << "\tPeakTime: " << PeakTime << std::endl;
        std::cout << "\tS2AverageAmplitude: " << S2AverageAmplitude << std::endl;
        std::cout << "\tFirstSampleBelowADCTriggerThresholdRange: " << FirstSampleBelowADCTriggerThresholdRange << std::endl;
        std::cout << "\tFirstSampleAboveADCTriggerThresholdRange: " << FirstSampleAboveADCTriggerThresholdRange << std::endl;
        
        std::cout << "\tSecondPeak_Amp: " << SecondPeak_OutOfRange << std::endl;
        std::cout << "\tPeaks_relation: " << SecondPeak_OutOfRange/MaxAmplitude_Range<< std::endl;
        std::cout << "\tMaxSam: " << MaximumSample << std::endl;

        std::cout << "\tCUT: " << cut << std::endl;
    }

    double getFirstTimeSignalRange(){return FirstTimeSignalRange;}
    double getPedestalMean(){return PedestalMean;}
    double getPedestalSTD(){return PedestalSTD;}
    double getPedestalCharge(){return PedestalCharge;}
    bool   getPedestalStatus(){return PedestalStatus;}
    double getTriggerCharge(){return TriggerCharge;}
    double getPreTriggerSTD(){return PreTriggerSTD;}
    double getPreTriggerCharge(){return PreTriggerCharge;}
    double getSignalToNoise(){return SignalToNoise;}
    double getSignalToNoiseAmplitude(){return SignalToNoiseAmplitude;}; 
    double getCharge(){return Charge;}
    double getChargeRange(){return Charge_Range;}
    double getChargeMaxPeakRange(){return Charge_MaxPeakRange;}
    double getChargeMaxPeakRange3bins(){return Charge_MaxPeakRange3bins;}
    double getQ1MaxPeakRange(){return q1_MaxPeakRange;}
    double getQ2MaxPeakRange(){return q2_MaxPeakRange;}
    double getQ3MaxPeakRange(){return q3_MaxPeakRange;}
    double getTStartMaxPeakRange(){return tStart_MaxPeakRange;}
    double getTEndMaxPeakRange(){return tEnd_MaxPeakRange;}

    double getChargeMaxPeak(){return Charge_MaxPeak;}
    double getSumOfSamples(){return SumOfSamples;}
    double getSaturation(){return saturated;}
    double getSaturation2(){return saturated2;}
    double getSampling(){return sampling;}
    double getMaxAmplitude(){return MaxAmplitude;}
    double getMinimumSample(){return MinimumSample;}
    double getMinimumSampleRange(){return MinimumSample_Range;}
    double getMaximumSample(){return MaximumSample;}
    double getMaximumSample_Range(){return MaximumSample_Range;}
    double getMaxAmplitudeRange(){return MaxAmplitude_Range;}
    double getMaxAmplitudeOutOfRange(){return MaxAmplitude_OutOfRange;}
    double getMaxAmplitudeOutOfMaxPeak(){return MaxAmplitude_OutOfMaxPeak;}
    double getPeakTime(){return PeakTime;}
    double getPeakTimeRange(){return PeakTime_Range;}
    double getPeakTimeOutOfRange(){return PeakTime_OutOfRange;}
    double getPeakTimeOutOfMaxPeak(){return PeakTime_OutOfMaxPeak;}
    double getTimestamp(){return timestamp;}
    double getNSamples(){return nsamples;}
    double getWidth(){return width;}
    double getFirstSampleBelowADCTriggerThresholdRange(){ return FirstSampleBelowADCTriggerThresholdRange;}
    double getFirstSampleAboveADCTriggerThresholdRange(){ return FirstSampleAboveADCTriggerThresholdRange;}
    double getFirstSampleBelowADCAmplitudeThresholdRange(){ return FirstSampleBelowADCAmplitudeThresholdRange;}
    
    double getSecondPeak_OutOfRange () {return SecondPeak_OutOfRange;}
    
    vector<double_t> getAmplitudes() {return amps;}
    vector<double_t> getTimes() {return times;}

    double getS2Charge(){return S2Charge;}
    double getS2AverageAmplitude(){return S2AverageAmplitude;}
    bool   getCut(){return cut;}
    void   setCut(bool b){ cut=b;}
  };

}
