#pragma once
using namespace std;
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
//#include "myNastran.h"
#include "method.h"

class section
{
public:
	std::string name;
	std::vector<double> data;

};

//����Ԫ�������Կ�Ƭ
class PBARL
{
public:
	PBARL() { PID = 0;MID = 0; }

	std::string name;
	int PID;
	int MID;
	std::string TYPE;
	std::vector<double> DIM;
	void printPBARL(std::ofstream& ofs);
};

//�ǵ�Ԫ���Կ�Ƭ
class PSHELL
{
public:
	PSHELL() { PID = 0; MID = 0; NSM = 0;}

	std::string name;
	int PID;
	int MID;
	double T{ 0 };//���
	double t_min{ -1e20 };
	double t_max{ 1e20 };
	double NSM;//�ǽṹ����
	void printPSHELL(std::ofstream& ofs);
};

//�������Կ�Ƭ
class MAT1
{
public:
	MAT1() { MID = 0; E = 2.1e11; Nu = 0.3; RHO = 2700; }

	std::string name;
	int MID;
	double E;
	double Nu;
	double RHO;
	void printMAT1(std::ofstream& ofs);
};

//��ά�����������Բ���
class MAT8
{
public:
	MAT8(){ MID = 0;E1 = E2 = 2.1e+011; NU12=0.3; G12=G1Z=G2Z=1.5e6; RHO=2.7e+003;}
	std::string name;
	int MID;
	double E1;
	double E2;
	double NU12;
	double G12;
	double G1Z;
	double G2Z;
	double RHO;
	double stress_min{ -1e20 };
	double stress_max{ 1e20 };
	void printMAT8(std::ofstream& ofs);
};

