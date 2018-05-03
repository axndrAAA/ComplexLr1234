#pragma once
#include"Matrix_classes.h"
#include<math.h>
#include<iostream>

class TQuaternion {
private:
	double scalar;
	TVector vector;

public:
	TQuaternion();
	TQuaternion(double l0, double l1, double l2, double l3);
	TQuaternion(TVector vect, double fi);
	TQuaternion( double fi, TVector vect);

	TQuaternion(TQuaternion& _qat);
	TQuaternion& operator = (const TQuaternion& _quat);


	TQuaternion operator +(const TQuaternion &_qat)const;
	TQuaternion operator *(const TQuaternion &_qat)const;
	TQuaternion operator *(const TVector &_vect)const;
	double getNorm();
	//вычисление сопряженного кватерниона
	TQuaternion conj() const;
	//нормирование
	void normate();

	double operator[](int i)const;
	double& operator[](int i);

	double getsclr()const;
	void setsclr(double);

	TVector getvect()const;
	void setvect(TVector);
};