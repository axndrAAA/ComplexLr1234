#pragma once
#include"stdafx.h"
#include "TIntegrator.h"
#include<math.h>
#include<algorithm>
#include<iostream>
using namespace std;

//отладочные функции show
/*void show(const TVector &vect, int size = 4) {
	for (int i = 0; i < vect.getSize(); i++)
		cout << vect[i] << " ";
	cout << endl;
}
void show(const TMatrix &vect, int size_y = 7, int size_x = 4) {
	for (int i = 0; i < vect.getRowCount(); i++) {
		for (int j = 0; j < vect.getColCount(); j++)
			cout << vect[i][j] << " ";
		cout << endl;
	}

	cout << endl;
}
*/
TIntegtator::TIntegtator(double _eps)
{
	Eps = _eps;
	//определение машинного нуля
	Zero = 0;
	double v = 1;
	while (1 + v > 1) {
		Zero = v;
		v = v / 2;
	}
}

void TIntegtator::setPrecision(double _pres)
{
	Eps = _pres;
}

double TIntegtator::getEpsMax()
{
	return Eps;
}

TDormanPrinceIntegrator::TDormanPrinceIntegrator(double _eps):TIntegtator(_eps)
{
}

void TDormanPrinceIntegrator::Run(TModel &mod)
{
	TMatrix k(7, mod.getCountOfUr());
	int iter1 = 0;

	TVector X(mod.getXO());
	TVector arg(mod.getXO());

	TVector X_out(mod.getCountOfUr());
	TVector X1(mod.getCountOfUr());
	TVector X2(mod.getCountOfUr());

	TVector b_Tt(6);

	double t = mod.getT0();
	double t_out = t;
	double h_new = 1e-3;
	double h = h_new;

	bool flag_range = false;

	while (t<=mod.getT1())
	{
		h = h_new;
		/*if (flag_range)break;
		if (t > mod.getT1()) { t_out = t = mod.getT1(); flag_range = true; }*/
		mod.getRight(X, t, k[0]);

		for (int i = 1; i < 7; i++) {
			//arg на исходную
			arg = X;
			//подготовка аргумента
			for (int ki = 0; ki < i; ki++)
				for (int argi = 0; argi < mod.getCountOfUr(); argi++)
					arg[argi] += h*a[i][ki] * k[ki][argi];
			//вызов тяжелой функции
			mod.getRight(arg, t + c[i] * h, k[i]);
			
		}
		//вычисление решений разных порядков
		X1 = X;
		X2 = X;
		for (int i = 0; i < 7; i++)
			for (int j = 0; j < mod.getCountOfUr(); j++) {
				X1[j] += h*b[i] * k[i][j];//6 порядок
				X2[j] += h*b1[i] * k[i][j];//7 порядок

			}


		//вычисление погрешности
		double sum_under_sqtr = 0;
		for (int i = 0; i < mod.getCountOfUr(); i++) {
			sum_under_sqtr += pow((h*(X1[i] - X2[i]) / std::max(std::max(1e-5, abs(X1[i])), std::max(abs(X[i]), 2.0 * Zero / getEpsMax()))), 2.0);
		}
		double eps = sqrt((1.0 / (double)mod.getCountOfUr()) * sum_under_sqtr);

		//новый шаг
		h_new = h / max(0.1, min(5.0, pow(eps / getEpsMax(), 0.2) / 0.9));
		iter1++;

		//считаем заново если погрешность велика
		if (iter1 >= iterr_limit) { flag = true; break; }

		if ((eps > Eps)) { continue; }

		//иначе - идем в плотную выдачу
		while (t_out <= t + h && t_out <= mod.getT1()) {
			X_out = X;

			double Teta = (t_out - t) / h;

			b_Tt[0] = Teta*(1.0 + Teta*(-1337.0 / 480.0 + Teta*(1039.0 / 360.0 + Teta*(-1163.0 / 1152.0))));
			b_Tt[1] = 0.0;
			b_Tt[2] = 100.0 * pow(Teta, 2)*(1054.0 / 9275.0 + Teta*(-4682.0 / 27825.0 + Teta*(379.0 / 5565.0))) / 3.0;
			b_Tt[3] = -5.0 * pow(Teta, 2)*(27.0 / 40.0 + Teta*(-9.0 / 5.0 + Teta*(83.0 / 96.0))) / 2.0;
			b_Tt[4] = 18225.0*pow(Teta, 2)*(-3.0 / 250.0 + Teta*(22.0 / 375.0 + Teta*(-37.0 / 600.0))) / 848.0;
			b_Tt[5] = -22.0*pow(Teta, 2)*(-3.0 / 10.0 + Teta*(29.0 / 30.0 + Teta*(-17.0 / 24.0))) / 7.0;


			for (int i = 0; i < 6; i++)
				for (int j = 0; j < mod.getCountOfUr(); j++)
					X_out[j] += h*b_Tt[i] * k[i][j];

			X = X;
			mod.AddResult(X_out, t_out);

			t_out += mod.getSamplingIncrement();
		}

		X = X1;
		t += h;
		
	}
	if (flag)mod.PreStop();

}
