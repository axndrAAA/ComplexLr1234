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
	const double W = 0.0, psiW = 0.0;
	const double ro = 1.225;

	TMatrix m;
	
public:
	Ecranoplan();
	Ecranoplan(double _t0, double _T1, double _smplinc = 0.01);

	TVector *getRight(const TVector &arg_v, double _t, TVector &k_i);
	vector<double> getXYZvk(const TVector &arg, vector<double> abGamma);
	TVector getXYZa(vector<double> LS, vector<double> ang, double  S);
	vector<double> getXYZ(double h, double nu, double V);
	vector<double> getAlphaBettaGammaA(double Nu, double GAMMA, double Hi, double Teta);
	vector<double> getM(const TVector &arg,double P);
	double getPshoulder();
	
};

