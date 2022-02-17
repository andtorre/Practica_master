
#ifndef Decon_h
#define Decon_h

namespace ana
{
  class Decon_t
  {
    public:
    std::vector<double> response;
    int threshold=19;
  
    Decon_t(std::vector<double> res) : response(res) { }
    TH1D* Deconvolute(TH1D* h)
    {
       TH1D *h2 = (TH1D*)h->Clone("temp");
       TH1D *hres = (TH1D*)h->Clone("hres"); hres->Reset();
//       TCanvas *ct = new TCanvas("ct"); ct->Divide(2);
       while(1)
       {
           h2->GetXaxis()->UnZoom();
           int maxbin=h2->FindFirstBinAbove(threshold);// cout << maxbin << endl; 
           if(maxbin==-1) break;
           double N=h2->GetBinContent(maxbin)/response[0]; 
 //          cout <<"Found " << N <<" photons. " << h2->GetBinContent(maxbin) <<  " "  << response[0] << endl;// lets_pause();
           hres->Fill(h2->GetBinCenter(maxbin),N);
           for(int i=0; i<response.size();i++) h2->SetBinContent(maxbin+i,h2->GetBinContent(maxbin+i)-N*response[i]);
 //          ct->cd(1); h2->GetXaxis()->SetRangeUser(1e-6,4e-6); h2->Draw("HIST"); 
 //          ct->cd(2); hres->GetXaxis()->SetRangeUser(1e-6,2.5e-6);hres->Draw("HIST");
 //          ct->Modified(); ct->Update(); lets_pause();
       }
//       ct->cd(1); h2->GetXaxis()->SetRangeUser(1e-6,4e-6); h2->Draw("HIST"); 
//       ct->cd(2); hres->GetXaxis()->SetRangeUser(1e-6,2.5e-6);hres->Draw("HIST");
//       ct->Modified(); ct->Update(); lets_pause();

       h2->Delete();
       return hres;

    }


  };
}

#endif
