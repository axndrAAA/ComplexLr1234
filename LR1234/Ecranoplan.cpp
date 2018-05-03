#include "stdafx.h"
#include "Ecranoplan.h"
#include<math.h>


Ecranoplan::Ecranoplan()
{
}

Ecranoplan::Ecranoplan(double _t0, double _T1, double _smplinc)
{
	TMatrix _m(3, 3);
	double Delta = I[0] * I[1] * I[2] - I[2] * Ixy;
	_m[0][0] = I[1] * I[2] / Delta;
	_m[0][1] = I[1] * I[2] / Delta;
	_m[0][2] = 0.0;

	_m[1][0] = _m[0][1];
	_m[1][1] = I[1] * I[2] / Delta;
	_m[1][2] = 0.0;

	_m[2][0] = 0.0;
	_m[2][1] = 0.0;
	_m[2][2] = (I[0] * I[1] * I[2] - Ixy * Ixy) / Delta;

	m = _m;



}



TVector * Ecranoplan::getRight(const TVector & arg, double _t, TVector & k_i)
{
	double mg = arg[12]*G;
	vector<double> abGamma = getAlphaBettaGammaA();
	vector<double> XYZvk = getXYZvk(abGamma);
	
	double P = 0.0;
	vector<double> M = getM(arg, P);


	k_i[0] = (-mg * sin(arg[1]) + P * cos(phi_P_X1) - XYZvk[0]) / arg[12];
	k_i[1] = (-mg * sin(arg[1]) + P * cos(phi_P_X1)*(sin(abGamma[0])*cos(abGamma[2]) + cos(abGamma[0])*sin(abGamma[1])*sin(abGamma[2])) + XYZvk[1]) / (arg[12] * k_i[0]);
	k_i[2] = (P*cos(phi_P_X1)*(sin(abGamma[0])*sin(abGamma[2]) - cos(abGamma[0])*sin(abGamma[1])*cos(abGamma[2])) + XYZvk[2]) / (arg[12] * k_i[0] * cos(phi_P_X1));
	
	k_i[3] = arg[0] * cos(arg[1])*cos(arg[2]);
	k_i[4] = arg[0] * sin(arg[1]);
	k_i[5] = arg[0] * cos(arg[1])*sin(arg[2]);
	
	k_i[6] = m[0][0] * M[0] + m[0][1] * M[1] + m[0][2] * M[2];
	k_i[7] = m[1][0] * M[0] + m[1][1] * M[1] + m[1][2] * M[2];
	k_i[8] = m[2][0] * M[0] + m[2][1] * M[1] + m[2][2] * M[2];
	
	k_i[9] = (arg[7] * cos(arg[9]) - arg[8] * sin(arg[9])) / cos(arg[11]);
	k_i[10] = arg[6] * sin(arg[9]) + arg[8] * cos(arg[9]);
	k_i[11] = arg[6] - tan(arg[11])*(arg[7] * cos(arg[9]) - arg[8] * sin(arg[9]));
	k_i[12] = -msec;


	return &k_i;
}

vector<double> Ecranoplan::getXYZvk(vector<double> LS,vector<double> ang,double  S)
{
	double Xa = LS[0];
	double Ya = LS[1]*cos(ang[2]) - LS[2] *sin(ang[2]);
	LS[0] = getCx()*S;
	LS[1] = getCy()*S;
	LS[2] = getCz()*S;


}

vector<double> Ecranoplan::getM(const TVector & arg,double P)
{
	vector<double> ret(3);
	ret[0] = Mx + (I[1] - I[2])*arg[7] * arg[8] - Ixy * arg[6] * arg[7];
	ret[1] = My + (I[2] - I[1])*arg[8] * arg[6] - Ixy * arg[6] * arg[7];
	ret[2] = getMz() + P*getPshoulder() + (I[0] - I[2])*arg[6] * arg[7] - Ixy * (arg[6]* arg[6] -  arg[7]* arg[7]);
	return ret;
}

double Ecranoplan::getMz()
{
	return 1.0;
}

double Ecranoplan::getCx()
{
	return 1.0;
}

double Ecranoplan::getCy()
{
	return 1.0;
}

double Ecranoplan::getCz()
{
	return 1.0;
}
