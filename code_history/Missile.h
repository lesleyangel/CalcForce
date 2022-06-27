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
	int SOLtype;//默认是退出分析= -1
	double ElemNumRatio = 0.5;
};



class Minuteman3 : public ShapeMash
{
public:
	//int ReadFromFile_old(string filepath);//工字梁、舱段材料各向同性
	//int ReadFromFile_old_2(string filepath);//C型梁、舱段材料二维正交异性
	int ReadFromFile(const string& filepath);//C型梁、舱段材料二维正交异性、输入文件格式可以变更参数顺序
	//int ReadFromFile_old_3(string filepath);//2021.11.21 输出阶段去掉了隔框(从输出_mesh中去掉的 要复原需要在printmesh中打开注释))、舱段材料二维正交异性
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