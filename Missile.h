#pragma once
#include "ShapeDesign.h"

struct Minuteman3Info
{
	int readFile(const string& path);
	struct PathInfo
	{
		string PNas{ "" };
		string PBdf{ "" };
		string PNam{ "" };
		string POut{ "" };
		string PAer{ "" };
	};
	struct MassInfo
	{
		double MM{ 0 };
		double MX{ 0 };
		double MY{ 0 };
		double MZ{ 0 };
	};
	struct EngineInfo
	{
		double EM{ 0 };
		double EXc{ 0 }, EYc{ 0 }, EZc{ 0 };
		double EXf{ 0 }, EYf{ 0 }, EZf{ 0 };
		double EFx{ 0 }, EFy{ 0 }, EFz{ 0 };
	};
	struct CabinInfo
	{
		double CLength, CRadius, CT;
		double CE1, CE2, CNu12;
		double CG12, CG1Z, CG2Z;
		double CRho, CSite;
		double t_min{ -1e20 }, t_max{ 1e20 };//SOL 200
		double stress_min{ -1e20 }, stress_max{ 1e20 };//SOL 200
	};
	struct BH
	{
		double BE, BRho, BNu;
		double BD1, BD2, BD3, BD4;
	};
	PathInfo path_list;
	vector<MassInfo> mass_list;
	vector<EngineInfo> Engin_list;
	vector<CabinInfo> Cabin_list;
	vector<BH> bh_list;
	int Modal_order{ 10 };//SOL 103
	int nowCase, nowCabinNum, nowBHnum;
	int nowEngine; //= 0
	int SOLtype;//Ĭ�����˳�����= -1
	double ElemNumRatio = 0.5;
};



class Minuteman3 : public ShapeMash
{
public:
	//int ReadFromFile_old(string filepath);//���������նβ��ϸ���ͬ��
	//int ReadFromFile_old_2(string filepath);//C�������նβ��϶�ά��������
	int ReadFromFile(const string& filepath);//C�������նβ��϶�ά�������ԡ������ļ���ʽ���Ա������˳��
	//int ReadFromFile_old_3(string filepath);//2021.11.21 ����׶�ȥ���˸���(�����_mesh��ȥ���� Ҫ��ԭ��Ҫ��printmesh�д�ע��))���նβ��϶�ά��������
	Minuteman3Info mt3info;
	int calcInitInfo();
	InitInfo initinfo;
	ReadCP aero;
};

class TestTube : public ShapeMash
{
public:
	int ReadFromFile(const string &filepath);
};