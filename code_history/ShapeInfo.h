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

//������������Ϣ
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
	bool IsOnCabin;//�����ڲն���(������) ���������������������(����)
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

//�ն���Ϣ
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
	int MID;	//����ʹ�õĲ������Ա��
				//MAT1 mt1;
				//int PID;
	PSHELL ps;	//����ĵ�Ԫ����
	double T;	//������
	double t_min{ -1e20 };
	double t_max{ 1e20 };
	double extramass;
	bool SetTnotMass; //�������ʹ�ú��(true)��������(false) Ĭ��ʹ��T
	int SiteNum;			//վ������
	int bulkheadNum;		//���������
	vector<int> PBARL_list;//����Ĳ��������б�
	int cabinPID0;	//�ն���ʼ�Ľڵ���id
	int cabinPID1;	//�նν����Ľڵ���id
	bool operator<(const CabinInfo& cab) { return (*this).x1 < cab.x1; }
	bool operator>(const CabinInfo& cab) { return (*this).x1 > cab.x1; }
	bool operator==(const CabinInfo& cab) { return (*this).x1 == cab.x1; }

};
static bool sort_token(const CabinInfo& c1, const CabinInfo& c2)
{
	return c1.x1 < c2.x1;
}