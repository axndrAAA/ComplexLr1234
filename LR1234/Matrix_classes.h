#pragma once
#include <string>
//раздебажено
class TMatrix;
class TQuaternion;
class TVector {
private:
	int size;
	double* vector;

public:	class IncorrectIndexException {};
		class IncorrectSizeException {};

public:	 TVector(int);
		 TVector();
		  ~TVector();
		 TVector(const TVector&);
		 TVector& operator =(const TVector&);
		 TVector operator +(const TVector&)const throw(IncorrectIndexException);
		 //векторное произведение
		 TVector crossProduct(const TVector) const throw(IncorrectSizeException);// вект произв
		 double& operator [](int) throw(IncorrectIndexException);
		 double operator [] (int) const throw(IncorrectIndexException);
		 int getSize() const;
		 //модуль вектора
		 double getMagnitude() const;//длина
		 TVector operator *(const double&) const;
		 TVector operator /(const double&) const;
		 //скалярное произведение
		 double operator *(const TVector&)const throw(IncorrectIndexException);//?
		 TVector operator*(const TMatrix& B) throw(IncorrectIndexException);
		 void setElement(int, double) throw(IncorrectIndexException);
		 void setLenght(int) throw(IncorrectIndexException);
		 int getMinSize(TVector, TVector)const;


		 //TQuaternion
		 TQuaternion operator*(const TQuaternion&);
		 //кажись работает
		 TVector RotateByRodrigFormula(TVector, double)const;
		 //возможно не верно
		 TVector RotateByQuaternion(const TQuaternion&);
};

//раздебаженo
class TSymmetricMatrix;
class TMatrix {
protected:
	int size_i;
	int size_j;
	TVector *matrix;
public:	class IncorrectIndexException {};
		class IncorrectSizeException {};
		class OtherException {};
public:
	TMatrix();
	TMatrix(int, int);
	 ~TMatrix();
	TMatrix(const TMatrix&);
	TMatrix(const TSymmetricMatrix&);
	TMatrix& operator =(const TMatrix&) throw(IncorrectIndexException);
	TMatrix operator +(const TMatrix&) throw(IncorrectIndexException);
	bool checkSymmetric() const;
	//транспонирование
	TMatrix flip();
	int getColCount() const;
	int getRowCount() const;
	TVector& operator [](int);
	TVector operator [](int)const;
	//обращение
	TMatrix inverse();
	TMatrix operator *(double);
	TMatrix operator *(const TMatrix&)  throw(IncorrectIndexException);
	TVector operator *(const TVector&)  throw(IncorrectIndexException);
	void SetElement(int, int, double)  throw(IncorrectIndexException);
	void setSize(int, int)  throw(IncorrectIndexException);

};
//раздебежено
class TSymmetricMatrix : public TMatrix {


public:
	TSymmetricMatrix();
	TSymmetricMatrix(int, int);
	TSymmetricMatrix(const TMatrix&);


	TSymmetricMatrix& operator =(const TMatrix& B) throw(IncorrectIndexException);


public:
	TMatrix inverse();
	void setElement(int, int, double)  throw(IncorrectIndexException);

};

int min(int a, int b);
void show(TMatrix _m);

void show(TVector _m);
