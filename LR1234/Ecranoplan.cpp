#include "stdafx.h"
#include "Ecranoplan.h"
#define _USE_MATH_DEFINES
#include<math.h>
#include"AeroAnglesAndCoef.h"


Ecranoplan::Ecranoplan()
{
}

Ecranoplan::Ecranoplan(double _t0, double _T1, double _smplinc)
{
	count_of_ur = 13;
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

	TVector _X0(13);
	_X0[0] = 40.8;
	_X0[4] = 1.568;
	_X0[11] = 6.2*M_PI / 180.0;
	_X0[12] = 4200;
	X0 = _X0;

}



TVector * Ecranoplan::getRight(const TVector & arg, double _t, TVector & k_i)
{
	double mg = arg[12]*G;
	vector<double> abGamma = getAlphaBettaGammaA(arg[11],arg[9],arg[10],arg[1]);
	vector<double> XYZvk = getXYZvk(arg,abGamma);	
	double P = 10000.0;
	vector<double> M = getM(arg, P);


	k_i[0] = (-mg * sin(arg[1]) + P * cos(phi_P_X1) - XYZvk[0]) / arg[12];
	k_i[1] = (-mg * sin(arg[1]) + P * cos(phi_P_X1)*(sin(abGamma[0])*cos(abGamma[2]) + cos(abGamma[0])*sin(abGamma[1])*sin(abGamma[2])) + XYZvk[1]) / (arg[12] * k_i[0]);
	k_i[2] = (P*cos(phi_P_X1)*(sin(abGamma[0])*sin(abGamma[2]) - cos(abGamma[0])*sin(abGamma[1])*cos(abGamma[2])) + XYZvk[2]) / (arg[12] * k_i[0] * cos(phi_P_X1));
	
	k_i[3] = arg[0] * cos(arg[1])*cos(arg[2]);
	k_i[4] = arg[0] * sin(arg[1]);
	k_i[5] = arg[0] * cos(arg[1])*sin(arg[2]);
	double xx = k_i[3];
	double yy = k_i[4];
	double zz = k_i[5];
	k_i[6] = m[0][0] * M[0] + m[0][1] * M[1] + m[0][2] * M[2];
	k_i[7] = m[1][0] * M[0] + m[1][1] * M[1] + m[1][2] * M[2];
	k_i[8] = m[2][0] * M[0] + m[2][1] * M[1] + m[2][2] * M[2];
	
	k_i[9] =  arg[6] - tan(arg[11])*(arg[7] * cos(arg[9]) - arg[8] * sin(arg[9]));
	k_i[10] =  (arg[7] * cos(arg[9]) - arg[8] * sin(arg[9])) / cos(arg[11]);
	k_i[11] = arg[6] * sin(arg[9]) + arg[8] * cos(arg[9]);
	k_i[12] = -msec;


	return &k_i;
}

vector<double> Ecranoplan::getXYZvk(const TVector &arg, vector<double> abGamma)
{
	double h = arg[4];
	double v = arg[11];// / 3.1415926535897932384626433832795 * 180.0;
	if (h < 0.2) {
		h = 0.2;
	}
	if (h > 10.0) {
		h = 10.0;
	}
	if (v < -4.0/180.0*M_PI) {
		v = -4.0/ 180.0*M_PI;
	}
	if (v > 12.0/ 180.0*M_PI) {
		v = 12.0/ 180.0*M_PI;
	}
	//TODO: до этого момента все проверено
	
	double Wvk = W * cos(W - psiW) * cos(arg[1]);
	double Wtheta = -W * cos(W - psiW) * sin(arg[1]);
	double Wpsi = W * sin(W - psiW);
	TVector Xa = getXYZa(getXYZ(h, arg[11], arg[0] - Wvk), abGamma, S);


	TMatrix a(3, 3);

	 

	//const double Wvk = W * cos(arg[2] - psiW) * cos(arg[1]);
	//const double Wtheta = -W * cos(arg[2] - psiW) * sin(arg[1]);
	//double Wpsi = W * sin(arg[2] - psiW);

	const double V_1 = sqrt(std::pow(arg[0], 2) * (1.0 - 2.0 * Wvk / arg[0] + std::pow(Wvk / arg[0], 2)));
	const double V_2 = sqrt(std::pow(arg[0], 2) * (std::pow(cos(arg[1]), 2) - 2.0 * Wvk / arg[0] + std::pow(Wvk / arg[0], 2)));

	a[0][0] = (arg[0] - Wvk) / V_1;
	a[0][1] = -Wtheta / V_1;
	a[0][2] = -Wpsi / V_1;

	a[1][0] = (std::pow(Wpsi, 2) * sin(arg[1]) + (arg[0] - Wvk) * Wtheta * cos(arg[1]) + std::pow(Wtheta, 2) * sin(arg[1])) / V_1 / V_2;
	a[1][1] = (std::pow(Wpsi, 2) * cos(arg[1]) + std::pow(arg[0] - Wvk, 2) * cos(arg[1]) + (arg[0] - Wvk) * Wtheta * sin(arg[1])) / V_1 / V_2;
	a[1][2] = ((arg[0] - Wvk) * Wpsi * sin(arg[1]) + Wpsi * Wtheta * cos(arg[1])) / V_1 / V_2;

	a[2][0] = Wtheta * cos(arg[1]) / V_2;
	a[2][1] = Wpsi * sin(arg[1]) / V_2;
	a[2][2] = ((arg[0] - Wvk) * cos(arg[1]) + Wtheta * sin(arg[1])) / V_2;

	

	
	//const double q = ro * std::pow(V, 2) / 2.0;

	
	

	TVector ret = a * Xa;
	vector<double> retVect = vector<double>(3);
	retVect[0] = ret[0];
	retVect[1] = ret[1];
	retVect[2] = ret[2];
	return retVect;
}

TVector Ecranoplan::getXYZa(vector<double> LS,vector<double> ang,double  S)
{
	TVector XYZvk(3);

	XYZvk[0] = LS[0];
	XYZvk[1] = LS[1]*cos(ang[2]) - LS[2] *sin(ang[2]);
	XYZvk[2] = LS[1] * sin(ang[2]) + LS[2] * cos(ang[2]);
	return XYZvk;
	
}

vector<double> Ecranoplan::getXYZ(double h, double nu,double V)
{
	vector<double> XYZ(3);
	double q = ro * std::pow(V, 2) / 2.0;
	XYZ[0] = getCx(h,nu)*S*q;
	double cyy = getCy(h, nu);
	XYZ[1] = cyy *S*q;
	XYZ[2] = cyy *S*q;
	return XYZ;
}

vector<double> Ecranoplan::getAlphaBettaGammaA(double Nu, double GAMMA, double Hi, double Teta)
{
	
	vector<double> ret = vector<double>(3);
	ret[0] = getAlpha(Nu, GAMMA, Hi, Teta);
	ret[1] = getBeta(Nu, GAMMA, Hi, Teta);
	ret[2] = getGammaA(Nu, GAMMA, Hi, Teta);
	return ret;
}

vector<double> Ecranoplan::getM(const TVector & arg,double P)
{
	vector<double> ret(3);
	ret[0] = Mx + (I[1] - I[2])*arg[7] * arg[8] - Ixy * arg[6] * arg[7];
	ret[1] = My + (I[2] - I[0])*arg[8] * arg[6] + Ixy * arg[6] * arg[7];
	ret[2] = getMz(arg[4],arg[11]) + P*getPshoulder() + (I[0] - I[1])*arg[6] * arg[7] - Ixy * (arg[6]* arg[6] -  arg[7]* arg[7]);
	return ret;
}

double Ecranoplan::getPshoulder()
{
	return 0.0;
}


