#pragma once
#include"stdafx.h"
#include "TQuaternion.h"

TQuaternion::TQuaternion()
{
	vector = TVector(3);
	scalar = 0;
}

TQuaternion::TQuaternion(double l0, double l1, double l2, double l3):TQuaternion()
{
	scalar = l0;
	vector[0] = l1;
	vector[1] = l2;
	vector[2] = l3;
}

TQuaternion::TQuaternion(TVector vect, double fi) :TQuaternion()
{
	scalar = (cos(fi/2.0));
	//vector = vect*(1.0/vect.getMagnitude()) *sin(fi/2.0);
	vector = vect/vect.getMagnitude() *sin(fi / 2.0);
}

TQuaternion::TQuaternion(double fi, TVector vect)
{
	vector = vect;
	scalar = fi;
}

TQuaternion::TQuaternion(TQuaternion & _qat) :TQuaternion()
{
	scalar = _qat.scalar;
	vector = _qat.vector;
}

TQuaternion & TQuaternion::operator=(const TQuaternion & _quat)
{
	this->scalar = _quat.scalar;
	this->vector = _quat.vector;
	return *this;
	// TODO: вставьте здесь оператор return
}

TQuaternion TQuaternion::operator+(const TQuaternion & _qat)const
{	
	return TQuaternion(this->scalar + _qat.scalar,this->vector + _qat.vector);
}

TQuaternion TQuaternion::operator*(const TQuaternion & _qat)const
{
	double sclr_part = scalar*_qat.scalar - vector*_qat.vector;
	TVector vect_part(3);
	vect_part = _qat.vector * scalar + vector *_qat.scalar + vector.crossProduct( _qat.vector);
	return TQuaternion(sclr_part,vect_part);
}

TQuaternion TQuaternion::operator*(const TVector & _vect)const
{
	//fixed
	return TQuaternion(-(vector * _vect), _vect*scalar + vector.crossProduct(_vect));
}

double TQuaternion::getNorm()
{
	double norm = pow(scalar, 2);
	for (int i = 0; i < vector.getSize(); i++)
		norm += pow(vector[i], 2);
	return (norm);
}

TQuaternion TQuaternion::conj()const
{
	return TQuaternion(scalar,vector * (-1.0));
}

void TQuaternion::normate()
{
	double norm = pow(scalar, 2);
	for (int i = 0; i < vector.getSize(); i++)
		norm += pow(vector[i], 2);
	norm = sqrt(norm);
	scalar /= norm;
	vector[0] /= norm;
	vector[1] /= norm;
	vector[2] /= norm;
}

double TQuaternion::operator[](int i) const
{
	if (i == 0) return scalar;
	else return vector[i-1];
	
}

double & TQuaternion::operator[](int i)
{
	if (i == 0) return scalar;
	else return vector[i-1];
}

double TQuaternion::getsclr() const
{
	return scalar;
}

void TQuaternion::setsclr(double  _sclr)
{
	scalar = _sclr;
}

TVector TQuaternion::getvect() const
{
	return vector;
}

void TQuaternion::setvect(TVector _vct)
{
	vector = _vct;
}
