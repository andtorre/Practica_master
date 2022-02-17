namespace ana
{
  class HistogramName_t {
  public:
    string histname="";

    int OpChannel=-1;
    int Run=-1;
    string SN="";
    //~ string Temp="";
    int Timestamp=0;
    double Voltage=0;
    string Mode="";
    string Option="";
    //~ int folder = -1;
    //~ int Wavelength = -1;
    
  public:
    HistogramName_t(string n): histname(n)
    {
      int first= histname.find("_ch",0)+3;
      int last= histname.find("_",first+1);
      string chnumb = histname.substr(first, last-first);
      first= histname.find("_ADC",0)+1;
      last= histname.find("_",first+1);
      SN = histname.substr(first, last-first);
      //~ Temp = histname.substr(first, last-first);
      OpChannel = stoi(chnumb);

      first= last+1;
      last= histname.find("V_",first+1);
      string voltnumb = histname.substr(first, last-first); //cout << voltnumb << endl;
      //~ string foldnumb = histname.substr(first, last-first); //cout << voltnumb << endl;

      Voltage = stof(voltnumb);
      //~ fold = stof(foldnumb);

      first= histname.find("Run",0)+3;	
      last= histname.find("_",first); 
      Run=stoi(histname.substr(first,last-first));

;

    }
    HistogramName_t(int r, int oc, string sn, double volt, string m, int firstt, string o): Run (r), OpChannel (oc), SN (sn), Voltage(volt), Mode (m), Timestamp (firstt), Option (o)
    {
       histname=(Form("Run%i_ch%i_%s_%.0fV_%s_%i_%s",Run,OpChannel,SN.c_str(),Voltage,Mode.c_str(),Timestamp,Option.c_str()));
    }
    int GetOpChannel(){  return OpChannel;}
    string GetPMTSN(){  return SN;}
    int GetRun(){  return Run;}
    double GetVoltage(){  return Voltage;}
    
    /*HistogramName_t(string T, int oc, int fold, int wl, int run): Temp (T), OpChannel (oc), folder (fold), Wavelength (wl), Run (r)
    {
       histname=(Form("%s_ch%i_%i_%inm_%i",Temp.c_str(),OpChannel,folder,Wavelength,Run,Timestamp,Option));
    }
    int GetOpChannel(){  return OpChannel;}
    string GetTemp(){  return Temp;}
    int GetRun(){  return Run;}
    double GetWavelength(){  return Wavelength;}*/
  
  };
}
