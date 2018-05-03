#pragma once
#include"stdafx.h"
#include"Matrix_classes.h"
#include"TQuaternion.h"
#include"math.h"
#include<exception>
using namespace std;


TVector::TVector(int i) {
	size = i;
	vector = new double[i];
	for (int i = 0; i < size; i++)vector[i] = 0;
}

TVector::~TVector() {
	delete[] vector;
	vector = NULL;
}

TVector::TVector() {
	size = 0;
	vector = new double[size];
}

TVector::TVector(const TVector &B) {
	size = B.getSize();
	vector = new double[size];
	for (int i = 0; i < size; i++)vector[i] = B.vector[i];
}

int TVector::getMinSize(TVector a, TVector b)const {
	if (a.size > b.size)return b.size;
	else return a.size;
}



TQuaternion TVector::operator*(const TQuaternion &_quat)
{
	TVector this_vect_copy(3);
	this_vect_copy[0] = vector[0];
	this_vect_copy[1] = vector[1];
	this_vect_copy[2] = vector[2];
	return TQuaternion(_quat.getsclr() - this_vect_copy *_quat.getvect(),this_vect_copy*_quat.getsclr() + this_vect_copy.crossProduct(_quat.getvect()));
}

TVector TVector::RotateByRodrigFormula(TVector _e, double _phi) const
{
	//ось вращения
	TVector e(_e * (1.0 / _e.getMagnitude()));

	TVector teta(3);
	teta[0] = 2 * tan(_phi / 2.0)*e[0];
	teta[1] = 2 * tan(_phi / 2.0)*e[1];
	teta[2] = 2 * tan(_phi / 2.0)*e[2];

	TVector this_vect_copy(3);
	this_vect_copy[0] = vector[0];
	this_vect_copy[1] = vector[1];
	this_vect_copy[2] = vector[2];
	//return e*(e*this_vect_copy)*(1 - cos(_phi)) + e.crossProduct(this_vect_copy)*sin(_phi) + this_vect_copy*cos(_phi);
	return TVector(this_vect_copy + (teta * (1.0 / (1+pow(tan(_phi/2),2)))).crossProduct(this_vect_copy + (teta * 0.5).crossProduct(this_vect_copy)) );
}

TVector TVector::RotateByQuaternion(const TQuaternion &_quat)
{
	TVector this_vect_copy(3);
	this_vect_copy[0] = vector[0];
	this_vect_copy[1] = vector[1];
	this_vect_copy[2] = vector[2];

	TQuaternion vectQuat(0.0,vector[0],vector[1],vector[2]);// this_vect_copy

	return TQuaternion(_quat * (vectQuat *_quat.conj())).getvect();
}

TVector& TVector::operator =(const TVector& B) {
	if (this == &B) return *this;
	size = B.getSize();
	delete[] vector;
	vector = new double[size];
	for (int i = 0; i < size; i++)vector[i] = B.vector[i];
	return *this;

}


TVector TVector::operator+(const TVector& _v)const {
	TVector buf(min(this->size,_v.getSize()));
	for (int i = 0; i < buf.getSize(); i++)
		buf[i] = (*this)[i] + _v[i];
	return buf;
}

TVector TVector::crossProduct(const TVector _v)const  {
	if (size == 3 || _v.getSize() == 3) {
		TVector result(3);

		result[0] = vector[1] * _v[2] - vector[2] * _v[1];
		result[1] = vector[2] * _v[0] - vector[0] * _v[2];
		result[2] = vector[0] * _v[1] - vector[1] * _v[0];
		return result;

	}
	else throw IncorrectSizeException();
}

double& TVector::operator[](int i) {
	if (i > size || i < 0) throw IncorrectIndexException();
	return vector[i];
}

double TVector::operator [](int i) const {
	if (i > size || i < 0) throw IncorrectIndexException();
	return vector[i];
}

int TVector::getSize()const {
	return size;
}

double TVector::getMagnitude() const {
	double res = 0;
	for (int i = 0; i < size; i++)res += pow(vector[i], 2);
	return sqrt(res);
}

TVector TVector::operator*(const double &d)const {
	TVector buf(size);
	for (int i = 0; i < size; i++)buf[i] = vector[i] * d;
	return buf;
}

TVector TVector::operator/(const double &d) const
{
	TVector buf(size);
	for (int i = 0; i < size; i++)buf[i] = vector[i] / d;
	return buf;
}

double TVector::operator*(const TVector& B)const {

	double summ = 0;
	for (int i = 0; i < getMinSize(*this, B); i++) {
		summ += vector[i] * B.vector[i];
	}
	return summ;
}

TVector TVector::operator*(const TMatrix& B) {
	int lim_size = 0;
	if (size >= B.getRowCount()) lim_size = B.getRowCount();
	else lim_size = size;

	TMatrix _B(B);

	TVector buf(lim_size);
	for (int k = 0; k < buf.getSize(); k++) {
		double sum_el = 0;
		for (int i = 0; i < lim_size; i++)
			sum_el += vector[i] * _B[i][k];
		buf[k] = sum_el;
		sum_el = 0;
	}
	return buf;
}

void TVector::setElement(int i, double d) {
	if (i < 0 || i > size)  throw IncorrectIndexException();
	vector[i] = d;
}

void TVector::setLenght(int new_size) {
	TVector buf(*this);

	delete[] vector;
	size = new_size;
	vector = new double[new_size];
	int lim = 0;
	if (size > new_size) lim = new_size;
	else
	{
		lim = size;
	}
	for (int i = 0; i < lim; i++)
		vector[i] = buf[i];
}