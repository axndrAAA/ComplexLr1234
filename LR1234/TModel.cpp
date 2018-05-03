#pragma once
#include"stdafx.h"
#include "TModel.h"
#define _USE_MATH_DEFINES
#include<math.h>
#include<cmath>
#include<iostream>

TModel::TModel(double _T0, double _T1, double _smplinc)
{
	T0 = _T0;
	T1 = _T1;
	SamplingIncrement = _smplinc;
	double d = (T1 - T0) / SamplingIncrement;
	if (d - (int)d < SamplingIncrement) {
		if (!(int)d) d++;		
		result_size = d + 1;
	}
	else result_size = d + 2;
	//инициализация Result должна быть  в наследнике!
}

void TModel::AddResult(TVector & vect, double t)
{


	TVector v(vect.getSize() + 1);
	for (int i = 0; i < vect.getSize(); i++)
	{
		v[i] = vect[i];
	}
	v[v.getSize() - 1] = t;

	Result.push_back(v);

	/*
	Result[iterat][0] = t;
	for (int i = 1; i < count_of_ur + 1; i++) {
		Result[iterat][i] = vect[i - 1];
	}	 
	iterat++;*/
}

int TModel::getCountOfUr()
{
	return count_of_ur;
}

TVector TModel::getXO()
{
	return X0;
}

double TModel::getSamplingIncrement()
{
	return SamplingIncrement;
}

void TModel::setSamplingIncrement(double _smplinc)
{
	SamplingIncrement = _smplinc;
}

double TModel::getT0()
{
	return T0;
}

double TModel::getT1()
{
	return T1;
}

int TModel::getResultsize()
{
	return result_size;
}

void TModel::PreStop()
{
	//Result.setSize(iterat, count_of_ur + 1);

	result_size = iterat;
}

void TModel::setX0(const TVector & _x0)
{
	X0 = _x0;
}

void TModel::setIntegrateLimits(double _T0, double _T1, double _smplinc)
{
	T0 = _T0;
	T1 = _T1;
	SamplingIncrement = _smplinc;
	double d = (T1 - T0) / SamplingIncrement;
	if (d - (int)d < SamplingIncrement) {
		if (!(int)d) d++;
		result_size = d +1; //+ 1;
	}
	else result_size = d;// +2;
	//Result.setSize(result_size, count_of_ur + 1);
	iterat = 0;
}

TArenstorfModel::TArenstorfModel(double _t0, double _T1, double _smplinc):TModel(_t0,_T1,_smplinc)
{
	count_of_ur = 4;
	//
	//Result.setSize(result_size, count_of_ur);
	//
	TVector _x0(count_of_ur);
	_x0[0] = 0.994;
	_x0[1] = 0.0;
	_x0[2] = 0.0;
	_x0[3] = -2.00158510637908252240537862224;
	X0 = _x0;
	//X0 = new TVector(count_of_ur);
	
}

TVector * TArenstorfModel::getRight(const TVector &arg_v, double _t, TVector &k_i) {
	double D1 = pow((pow((arg_v[0] + mass), 2.0) + pow(arg_v[1], 2.0)), 1.5);
	double D2 = pow((pow((arg_v[0] - (1 - mass)), 2.0) + pow(arg_v[1], 2.0)), 1.5);

	k_i[0] = arg_v[2];
	k_i[1] = arg_v[3];
	k_i[2] = arg_v[0] + 2.0 * arg_v[3] - (1 - mass)*(arg_v[0] + mass) / D1 - mass * (arg_v[0] - (1 - mass)) / D2;
	k_i[3] = arg_v[1] - 2.0 * arg_v[2] - (1 - mass)*arg_v[1] / D1 - mass*arg_v[1] / D2;

	return &k_i;
}

TSunClockModel::TSunClockModel(double _T0, double _T1, double _smplinc, bool _DayLongMode ):TModel(_T0,_T1,_smplinc)
{
	DayLongMode = _DayLongMode;
	count_of_ur = 6;
	//
	if (!DayLongMode) {
		//Result.setSize(result_size, count_of_ur + 1);
		Result_sh.setSize(result_size, 4);
	}
		
	//
	TVector _x0(count_of_ur);
	//На дату 01.01.2016 0:00 (JD = 2457388.5):
	_x0[0] = -2.439276441307208e7;//X
	_x0[1] = 1.332124993767647e8;//Y
	_x0[2] = 5.772281789058973e7;//Z
	_x0[3] = -2.984063142864898e1;//Vx
	_x0[4] = -4.727555397178124;//Vy
	_x0[5] = -2.049346856618893;//Vz
	

	//На дату 21.03.2016 0:00 (JD = 2457468.5):
	/*_x0[0] = -1.484512468045209e8;//X
	_x0[1] = -1.117361282251260e6;//Y
	_x0[2] = -5.101193558950569e5;//Z
	_x0[3] = -1.813507346095267e-1;//Vx
	_x0[4] = -2.742106189673085e1;//Vy
	_x0[5] = -1.188731012157166e1;//Vz*/
	X0 = _x0;
}

TVector * TSunClockModel::getRight(const TVector & arg_v, double _t, TVector & k_i)
{
	double mod = sqrt((arg_v[0] * arg_v[0]) + (arg_v[1] * arg_v[1]) + (arg_v[2] * arg_v[2]));
	k_i[0] = arg_v[3];
	k_i[1] = arg_v[4];
	k_i[2] = arg_v[5];
	k_i[3] = -my_s*arg_v[0] / (mod*mod*mod);
	k_i[4] = -my_s*arg_v[1] / (mod*mod*mod);
	k_i[5] = -my_s*arg_v[2] / (mod*mod*mod);
	
	return &k_i;
}

void TSunClockModel::AddResult(TVector & vect, double t)
{
	
	if (!SunClockMode && !DayLongMode) { TModel::AddResult(vect, t); return; }
	if (DayLongMode && !SunClockMode) { AddResult_long_day_evolution(vect, t); return; }
	double s = sg_0 + Omega*t + lambda;//все значения в радианах

	TVector r(3);
	r[0] = (R_e + h_gnom)*cos(phi)*cos(s);
	r[1] = (R_e + h_gnom)*cos(phi)*sin(s);
	r[2] = (R_e + h_gnom)*sin(phi);
	r = r*(1.0 / r.getMagnitude());//r0
	TVector r_g(r*h_gnom);

	TVector rv0(3);
	rv0[0] = vect[0];
	rv0[1] = vect[1];
	rv0[2] = vect[2];
	rv0 = rv0*(1.0 / rv0.getMagnitude());
	TVector rv_zv(rv0*(- h_gnom / (rv0*r)));

	TVector r_sh(r_g + rv_zv);
	
	//вектор тени в местной СК
	
	r_sh = GSKtoTSK(s)*r_sh;

	//контроль день/ночь
	if (((r_g*(1.0 / r_g.getMagnitude()))*rv0) > 0.0) { r_sh = r_sh*0.0; }

	//контроль длинны вектора тени
	if (r_sh.getMagnitude() >= 0.03) {
		if(r_sh_max_happend){
			r_sh = r_sh*(0.01 / r_sh.getMagnitude());
			r_sh_max = r_sh;
			r_sh_max_happend = false;
		}
		
		r_sh = r_sh_max*0.0;
	}

	//перевод км в м
	r_sh[0] = r_sh[0] * 1000.0;
	r_sh[1] = r_sh[1] * 1000.0;
	r_sh[2] = r_sh[2] * 1000.0;

	//вывод вектора
	Result_sh[iterat][0] = t;
	Result_sh[iterat][1] = r_sh[0];
	Result_sh[iterat][2] = r_sh[2];
	//контрольный вывод значений cos(alpha)
	double cos_alpha = Result_sh[iterat][3] = (r_g*(1.0 / r_g.getMagnitude()))*rv0;

	iterat++;

}

void TSunClockModel::AddResult_long_day_evolution(TVector & vect, double _t)
{
	double t = _t + 3600.0*3.0;

	double s = sg_0 + Omega*_t + lambda;//все значения в радианах

	TVector r(3);
	r[0] = (R_e + h_gnom)*cos(phi)*cos(s);
	r[1] = (R_e + h_gnom)*cos(phi)*sin(s);
	r[2] = (R_e + h_gnom)*sin(phi);
	r = r*(1.0 / r.getMagnitude());//r0
	TVector r_g(r*h_gnom);

	TVector rv0(3);
	rv0[0] = vect[0];
	rv0[1] = vect[1];
	rv0[2] = vect[2];
	rv0 = rv0*(1.0 / rv0.getMagnitude());
	

	//еслиcos(alpha) > 0 ночь
	//			     < 0 день	
	double cos_alpha = (r_g*(1.0 / r_g.getMagnitude()))*rv0;
	
	double current_day = (int)(t / (60.0*60.0*24.0));
	//отметка о факте рассвета/заката
	if (cos_alpha * prev_cos_alpha < 0) {
		//если сейчас восход
		if (cos_alpha < 0) {
			//от начала дня в сек
			voshod_time = (t - (60.0 * 60.0 * 24.0 * current_day));
		zakat_time = 0.0;
		}
		//если сейчас закат
		else
		{
			//от начала дня в сек
			zakat_time = (t - (60.0 * 60.0 * 24.0 * current_day));
			Result_day_size[iterat][0] = current_day;
			//вывод длительности светового дня, с приведением к часам (изначально в сек.)
			Result_day_size[iterat][1] = (zakat_time - voshod_time)/3600.0;


			//далее рассчет и анализ длительности дня между 8:00 и 20:00 
			//для ЗИМНЕГО времени
			//8:00 от начала дня в сек
			double t_8_00 = 60.0*60.0 * 8.0;
			//20:00от начала дня в сек
			double t_20_00 = t_8_00 + 60.0*60.0 * 12.0;

			double inc_hour = 3600.0;
			
			double v_t = voshod_time;
			double z_t = zakat_time;
			//при зимнем времени
			if (voshod_time <= t_8_00) {
				voshod_time = t_8_00;								
			}
			if (zakat_time >= t_20_00) {
				zakat_time = t_20_00;
			}
			//вывод длительности дня между 8:00 и 20:00 при постоянном зимнем времени
			Result_day_size[iterat][2] = (zakat_time - voshod_time) / 3600.0;

			//при летнем времени
			t += 3600.0;
			voshod_time = v_t + inc_hour;
			zakat_time = z_t + inc_hour;
			if (voshod_time <= t_8_00) {
				voshod_time = t_8_00;
			}
			if (zakat_time >= t_20_00) {
				zakat_time = t_20_00;
			}
			//вывод длительности дня между 8:00 и 20:00 при постоянном летнем времени
			Result_day_size[iterat][3] = (zakat_time - voshod_time) / 3600.0;

			t -=3600.0;//возврат к зимнему
			voshod_time = v_t;
			zakat_time = z_t;

			//при наличии перевода зимнее/летнее время
			if (t > 3600.0*24.0*(31.0 + 29.0 + 27.0) + 3600.0*2.0) {
				t += 3600.0;
				//inc_hour *= 1.0;
				voshod_time = v_t + inc_hour;
				zakat_time = z_t + inc_hour;
			}
			if (t > ((getJD(2016, 10, 30, 0, 0, 0) - getJD(2016, 1, 1, 0, 0, 0))*3600.0*24.0 + 3600.0*3.0)) {
				t -= 3600.0;
				inc_hour *= -1.0;
				voshod_time = v_t;
				zakat_time = z_t;
			}
			
			
			if (voshod_time <= t_8_00) {
				voshod_time = t_8_00;
			}
			if (zakat_time >= t_20_00) {
				zakat_time = t_20_00;
			}
			//вывод длительности дня между 8:00 и 20:00 при наличии перехода с летнего на зимнее
			Result_day_size[iterat][4] = (zakat_time - voshod_time) / 3600.0;

			iterat++;


		}
	}


	prev_cos_alpha = cos_alpha;

}

void TSunClockModel::setIntegrateLimits(double _T0, double _T1, double _smplinc)
{
	(this)->TModel::setIntegrateLimits(_T0, _T1, _smplinc);

	if(SunClockMode)Result_sh.setSize(result_size, 4);
	//только для моделирования длительности дня
	if (DayLongMode) { result_size = 365; Result_day_size.setSize(result_size, 5); }
}

void TSunClockModel::SunClockMode_ON()
{
	SunClockMode = true;
}

void TSunClockModel::DayLongModeON()
{
	SunClockMode = false;
	DayLongMode = true;
}

void TSunClockModel::SetCoordAndH(double _phi, double _lambda,double _hg)
{
	phi = _phi*M_PI/180.0;
	lambda = _lambda*M_PI / 180.0;
	h_gnom = _hg;
}

double TSunClockModel::getGrinvichTime(double year, double month, double day, double hour, double minute, double second)
{
	double t_szv = (getJD(year, month, day, hour, minute, second) - 2451544.5) / 36525.0;

	return (2*M_PI/86400.0)*fmod((24110.54841 + 8640184.812866*t_szv + 0.093104*t_szv*t_szv - 6.2e-6*t_szv*t_szv*t_szv),86400.0);
		
}

double TSunClockModel::getGrinvichTime(double JD)
{
	double t_szv = (JD - 2451544.5) / 36525.0;

	return (2.0 * M_PI / 86400.0)*fmod((24110.54841 + 8640184.812866*t_szv + 0.093104*t_szv*t_szv - 6.2e-6*t_szv*t_szv*t_szv), 86400.0);
}

double TSunClockModel::getJD(double year, double month, double day, double hour, double minute, double second)
{
	double a = div((14.0 - month),12).quot;
	double Y = year + 4800.0 - a;
	double M = month + 12.0 * a - 3.0;
	double JDN = day + div((153.0*M + 2.0),5).quot + 365.0*Y + div(Y,4).quot - div(Y, 100).quot + div(Y , 400).quot - 32045.0;

	return JDN + ((hour - 12.0) / 24.0) + (minute / 1440.0) + (second / 86400.0);
}

TMatrix TSunClockModel::GSKtoTSK(double sg_tek)
{
	TMatrix A(3, 3);
	
	TVector yt(3);
	yt[0] = cos(phi)*cos(sg_tek);
	yt[1] = cos(phi)*sin(sg_tek);
	yt[2] = sin(phi);
	TVector z(3);
	z[0] = 0.0;
	z[1] = 0.0;
	z[2] = 1.0;
	TVector zt(z.crossProduct(yt)*(1.0/(z.getMagnitude()*yt.getMagnitude())));
	TVector xt(yt.crossProduct(zt));

	A[0] = xt;
	A[1] = yt;
	A[2] = zt;
	
	return A;
}

void TSunClockModel::setSg0(double _sg0)
{
	sg_0 = _sg0;
}
