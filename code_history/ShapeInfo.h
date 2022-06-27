#pragma once
#include "method.h"
#include "material.h"
enum MassType {
	Engine,
	Device
};
struct MassPointInitInfo
{
	double mass;
	Point site;
	string name;
	MassType type;
};

//集中质量点信息
class MassPoint
{
public:
	MassPoint() { name.clear(); mass = 0; xc = 0; cabinID = -1; bulkheadID0 = bulkheadID1 = 0; IsOnCabin = false; }//Xid.clear(); EquMass.clear();}
	MassPoint(const MassPointInitInfo& info);
	MassPoint(double mass, Point pt, bool IsOnCabin = false, string name = "unknown");
	void SetMassPoint(double mass, Point pt, bool IsOnCabin = false, string name = "unknown") { this->mass = mass; this->pt = pt, this->xc = pt.getX(); this->name = name; this->IsOnCabin = IsOnCabin; }
	void SetMass(double mass) { this->mass = mass; }
	double GetXc() { return xc; }
	double GetMass() { return mass; }
	Point pt;
	int cabinID;
	int bulkheadID0;
	int bulkheadID1;
	bool IsOnCabin;//是则在舱段上(发动机) 否则在最近的两个隔框上(部件)
private:
	string name;
	double mass;
	double xc;

};

//enum PBARLtype {
//	PBARL1, PBARL2, PBARL3, PBARL4, PBARL5, PBARL6, PBARL7, PBARL8, PBARL9, PBARL10
//};

struct CabinInitInfo
{
	double x1;
	int MID;
	double T;
	int siteNum;
	double extramass;
	vector<int> bhlist;
	double t_min{ -1e20 };
	double t_max{ 1e20 };
};

//舱段信息
class CabinInfo
{
public:
	CabinInfo() { x0 = x1 = xc = mass = extramass = Area = T = SiteNum = bulkheadNum = MID = 0; SetTnotMass = true; PBARL_list.clear(); }
	CabinInfo(const CabinInitInfo& info);
	CabinInfo(double x1, int MID, double T, double extramass = 0, int siteNum = 1, vector<int> PBARL_list = vector<int>(0));
	double x0;
	double x1;
	double xc;
	double mass;

	double Area;
	int MID;	//壳体使用的材料属性编号
				//MAT1 mt1;
				//int PID;
	PSHELL ps;	//壳体的单元属性
	double T;	//壳体厚度
	double t_min{ -1e20 };
	double t_max{ 1e20 };
	double extramass;
	bool SetTnotMass; //定义材料使用厚度(true)还是质量(false) 默认使用T
	int SiteNum;			//站点数量
	int bulkheadNum;		//隔框的数量
	vector<int> PBARL_list;//隔框的材料属性列表
	int cabinPID0;	//舱段起始的节点编号id
	int cabinPID1;	//舱段结束的节点编号id
	bool operator<(const CabinInfo& cab) { return (*this).x1 < cab.x1; }
	bool operator>(const CabinInfo& cab) { return (*this).x1 > cab.x1; }
	bool operator==(const CabinInfo& cab) { return (*this).x1 == cab.x1; }

};
static bool sort_token(const CabinInfo& c1, const CabinInfo& c2)
{
	return c1.x1 < c2.x1;
}