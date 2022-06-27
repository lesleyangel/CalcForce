#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include "Aero.h"
#include "Windows.h"
using namespace std;
//定义笛卡尔坐标系下的一个点
class Point
{
public:
	Point() { x = 0; y = 0; z = 0; }
	Point(double x, double y = 0, double z = 0) { SetSite(x, y, z); }

	//Point(mat point);
	void SetSite(double x, double y = 0, double z = 0) { this->x = x; this->y = y; this->z = z; }
	void setX(double x) { this->x = x; }
	void setY(double y) { this->y = y; }
	void setZ(double z) { this->z = z; }

	const double getX() const { return x; }
	const double getY() const { return y; }
	const double getZ() const { return z; }

	double Norm1() const { return abs(x) + abs(y) + abs(z); }
	double Norm2() const { return sqrt(x * x + y * y + z * z); }
	double Normi(int i) const { return pow(pow(abs(x), i) + pow(abs(y), i) + pow(abs(z), i), 1.0 / (double)i); }

	bool operator<(const Point& a) { return (*this).x < a.x; }
	bool operator>(const Point& a) { return (*this).x > a.x; }
	bool operator==(const Point& a) { return ((*this) - a).Norm1() < 1e-6; }
	Point operator+(const Point& a) { return Point((*this).x + a.x, (*this).y + a.y, (*this).z + a.z); }
	Point operator-(const Point& a) { return Point((*this).x - a.x, (*this).y - a.y, (*this).z - a.z); }
	Point operator-=(const Point& a) { (*this) = (*this) - a; return (*this); }
	Point operator+=(const Point& a) { (*this) = (*this) + a; return (*this); }
	Point operator-(const double& n) { return Point((*this).x - n, (*this).y - n, (*this).z - n); }
	Point operator/(const double& n) { return Point((*this).x / n, (*this).y / n, (*this).z / n); }
	Point operator*(const double& n) { return Point((*this).x * n, (*this).y * n, (*this).z * n); }
protected:
	double x;
	double y;
	double z;

	//Type<T> a;
};
static bool sort_token_point(const Point& p1, const Point& p2)
{
	return p1.getX() < p2.getX();
}

//inline Point::Point(mat point)
//{
//	if (point.n_rows == 1 && point.n_cols > 2)
//	{
//		SetSite(point(0), point(1), point(2));
//	}
//	else if (point.n_cols == 1 && point.n_rows > 2)
//	{
//		SetSite(point(0), point(1), point(2));
//	}
//	else
//		SetSite(0, 0, 0);
//}

inline Point cross(const Point& a, const Point& b)
{
	double x = a.getY() * b.getZ() - a.getZ() * b.getY();
	double y = a.getZ() * b.getX() - a.getX() * b.getZ();
	double z = a.getX() * b.getY() - a.getY() * b.getX();
	return Point(x, y, z);
}


template<class T>
class Field : public Point
{
public:
	Field() { x = 0; y = 0; z = 0; p = 0; }
	const Field& operator=(const Point&a) { this->x = a.getX(); this->y = a.getY(); this->z = a.getZ(); return *this; }
	Field(double x, double y = 0, double z = 0) { SetSite(x, y, z); p = 0; }
	Field(double x, double y, double z, T p) { SetSite(x, y, z); this->p = p; }
	Field operator+(const Field& a) { this->x += a.x; this->y += a.y; this->z += a.z; this->p += a.getP(); return *this; }
	void setP(T p) { this->p = p; }
	const T& getP() const { return p; }
private:
	T p;
};


//网格信息
class Mesh
{
public:
	Mesh() { P.clear(); E.clear(); }
	vector<Field<double>> P;
	vector<vector<vector<int>>> E;
};



//集中力
class  ConcentratedForce
{
public:
	ConcentratedForce() { Xid = 0; }
	Point origin;	//作用点
	Point force;	//作用力
	void SetXsiteID(int ID) { this->Xid = ID; }
	int getXid() { return Xid; }
private:
	int Xid;//作用力施加在单元里哪个点（在CalcPoint中得到该参数）
};

