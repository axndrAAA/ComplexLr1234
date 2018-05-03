#pragma once
#include"Matrix_classes.h"
#include<list>
class TModel {

protected:
	double const G = 9.807;
	double SamplingIncrement;
	double T0, T1;
	int count_of_ur = 1;

	TVector X0;
	int iterat = 0;
	int result_size;
public:
	//TMatrix Result;
	std::list<TVector> Result;

	~TModel() {}
	TModel() {};
	TModel(double _T0, double _T1, double _smplinc = 0.01);

	virtual TVector *getRight(const TVector &arg_v, double _t, TVector &k_i) = 0;

	void virtual AddResult(TVector &vect, double t);
	int getCountOfUr();
	TVector getXO();
	double getSamplingIncrement();
	void setSamplingIncrement(double _smplinc);
	double getT0();
	double getT1();
	int getResultsize();
	void PreStop();
	void setX0(const TVector &_x0);
	void setIntegrateLimits(double _T0, double _T1, double _smplinc = 0.01);
};

class TArenstorfModel : public TModel {
private:

	const double mass = 0.012277471;
public:
	TArenstorfModel() { count_of_ur = 4; }
	~TArenstorfModel() {}
	TArenstorfModel(double _t0, double _T1, double _smplinc = 0.01);
	TVector *getRight(const TVector &arg_v, double _t, TVector &k_i);
};

class TSunClockModel :public TModel {
private:
	double const my_s = 132712.43994e6;

	double lambda;//географическая долгота места расположения гномона
	double phi; //географическая широта места расположения гномона
	double h_gnom;//высота гномона от МГП [KM]!!!!

	double const Omega = 7.292115e-5;//угловая скорость вращения Земли
	double R_e = 6371.3;//радиус земли
	//звёздное время гринвичское на момент начала моделирования (по умолчанию 01.01.2016 0:00 (JD = 2457388.5))
	double sg_0 = 0.0;

	bool SunClockMode = false;
	bool DayLongMode = false;

	bool r_sh_max_happend = true;

	bool is_summer_time = true;
	bool is_time_switch = false;
	
	TVector r_sh_max;

	double prev_cos_alpha = 0.0;
	double voshod_time = 0.0;
	double zakat_time = 0.0;
public:
	//матрица выходных координат для построения тени
	TMatrix Result_sh;
	//номердня длительностьдня ДДот8до20(зим) ДДот8до20(лет) ДДот8до20(зим/лет)
	TMatrix Result_day_size;
	
	TSunClockModel() { count_of_ur = 6;}
	TSunClockModel(double _T0, double _T1, double _smplinc = 0.01,bool _DayLongMode = false);
	TVector *getRight(const TVector &arg_v, double _t, TVector &k_i);
	void AddResult(TVector &vect, double t);
	void AddResult_long_day_evolution(TVector &vect, double t);

	void setIntegrateLimits(double _T0, double _T1, double _smplinc = 0.01);
	void SunClockMode_ON();
	void DayLongModeON();
	//широта долгота и высота[km]
	void SetCoordAndH(double _phi, double _lambda,double _hg);
	//в радианах
	double getGrinvichTime(double year, double month, double day, double hour, double minute, double second);//в радианах
	static double getGrinvichTime(double JD);//в радианах
	static double getJD(double year, double month, double day, double hour, double minute, double second);
	TMatrix GSKtoTSK(double sg_tek);
	void setSg0(double _sg0);
};