#pragma once
#include"TModel.h"
#include<vector>
using namespace std;
class Ecranoplan:public TModel
{
private:
	vector<double> I = { 14932.18, 42147.14, 30376.11 };
	double Ixy = 0;
	double Bsgcp = 7.84;
	double S = 37.67;
	double m0 = 4200;
	double msec = 0.023;
	double lp = 0.0;
	double phi_P_X1 = 0.0;
	double My = 1.2;
	double Mx = My;

	TMatrix m;
	
public:
	Ecranoplan();
	Ecranoplan(double _t0, double _T1, double _smplinc = 0.01);

	TVector *getRight(const TVector &arg_v, double _t, TVector &k_i);
	vector<double> getXYZvk(vector<double> ang);
	vector<double> getAlphaBettaGammaA();
	vector<double> getM(const TVector &arg,double P);
	double getMz();
	double getPshoulder();
	double getCx();
	double getCy();
	double getCz();

	
};

