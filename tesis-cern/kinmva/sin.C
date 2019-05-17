
void sin()
{
double   pi=3.1415926;
TF1 *fa1 = new TF1("fa1","((1/sqrt(2.*pi))*(1/sqrt(x))*exp(-x/2.))",2,12);
fa1->SetLineColor(kBlue);
TCanvas *cs = new TCanvas("cs","cs",100,100,700,900);
fa1->Draw();
cout<<fa1->Integral(2,1e50)<<endl;
cs->SaveAs("tmu.jpg");


}
