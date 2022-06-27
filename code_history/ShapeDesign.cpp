#include "ShapeDesign.h"
#include "NasPrinter.h"
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <sstream>
#include <windows.h>
#include <tlhelp32.h>
#include <comdef.h>
using namespace std;
//#include <cmath>
#define pi 3.1415926

#define ELEM3		//���忪ͷ���
// #define RBE333		//�������ɶ��Լ�������ɼ�������
#define gekuang		//���ɸ���
// #define ONLY_2_MAT
ShapeMash::ShapeMash() 
{ 
	ShapeFunc.clear(); 
	FaiNum = 2; Xsite.clear(); 
	//CabinMass = 0; 
	beamNum = 0; 
	bulkhead.clear(); 
	bulkheadID.clear();
	point.clear();
	Instrument.clear();
	masspoint.clear();
	CabinList.clear();
	bulkheadPID.clear();
	Material8_list.clear();
}

void ShapeMash::SetInitInfo(const InitInfo& info)
{
	vector<CabinInfo> cabinlist(info.cbinit.size());
	for (size_t i = 0; i < info.cbinit.size(); i++)
	{
		cabinlist[i] = CabinInfo(info.cbinit[i]);
	}
	vector<MassPoint> mplist(info.mpinit.size());
	for (size_t i = 0; i < info.mpinit.size(); i++)
	{
		mplist[i] = MassPoint(info.mpinit[i]);
	}
	bool ifUseCabinToCalcXsite = true;//ʹ�òնγ�ʼ����Ϣ���������������
	//-----------------------��������-----------------------
	SetShapeFunc(info.ShapeFuncList);//���õ������β���
	SetCabin(cabinlist);//���òնλ���
	SetMassPoint(mplist, 2);//���ü�������
	SetStruct(info.beamPID);//�����������ԣ�����������ݲնζ���
	SetMeshSkin(info.faiNum, ifUseCabinToCalcXsite);//�����������������������������ڵ���ݲնζ���
	SetBeamProperty(info.property_list, info.meterial_list, info.meterial8_list);//���ò�������
	SetForce(info.CPinfo, info.EngineForce);//���÷���������
	Modal_order = info.Modal_order;
}

void ShapeMash::CalcModel()
{
	//��������
	CalcPoint();
	CalcMesh();
	CalcCabin();
	CalcMassPoint();
}

void ShapeMash::SetShapeFunc(vector<Point> SF)
{
	sort(SF.begin(), SF.end(),sort_token_point);
	SF.erase(unique(SF.begin(),SF.end()),SF.end());
	this->ShapeFunc = SF;
}

void ShapeMash::SetMeshSkin(int fainum,vector<double> xsite /* = vector<double>(0) */)
{
	this->FaiNum = (fainum<1) ? 1 : fainum;
	if (xsite.size() == 0)
	{
		Xsite.resize(ShapeFunc.size());
		for (int i = 0; i < ShapeFunc.size(); i++)
			Xsite[i] = ShapeFunc[i].getX();
	}
	else
	{
		sort(xsite.begin(), xsite.end());
		xsite.erase(unique(xsite.begin(),xsite.end()),xsite.end());
		this->Xsite = xsite;
	}
}

void ShapeMash::SetMeshSkin(int fainum, bool ifUseCabin, int xnum /* = 0 */)
{
	if (ifUseCabin)
	{
		this->FaiNum = (fainum<1) ? 1 : fainum;
		for (int i = 0; i < CabinList.size(); i++)
		{
			double x0 = 0;
			if (i > 0) x0 = CabinList[i-1].x1;
			double length = CabinList[i].x1 - x0;
			if (CabinList[i].SiteNum < 1) CabinList[i].SiteNum = 1;
			for (int j = 0; j < CabinList[i].SiteNum; j++)
			{
				Xsite.push_back(x0 + (double)j * length / (double)CabinList[i].SiteNum);
			}
		}
		Xsite.push_back((ShapeFunc.end()-1)->getX());
	}
	else
	{
		this->FaiNum = (fainum<1) ? 1 : fainum;
		if (xnum < 1) xnum = 1;
		Xsite.clear();
		double dx = (ShapeFunc.end()-1)->getX() / xnum;
		for (int i = 0; i < CabinList.size(); i++)
		{
			double x0 = 0;
			if (i > 0) x0 = CabinList[i-1].x1;
			double length = CabinList[i].x1 - x0;
			int cabinNum = (length/dx < 2) ? 2 : length/dx;
			for (int j = 0; j < cabinNum; j++)
			{
				Xsite.push_back(x0 + (double)j * length / (double)cabinNum);
			}
		}
		Xsite.push_back((ShapeFunc.end()-1)->getX());
	}
}

void ShapeMash::SetCabin(const vector<CabinInfo>& cabinList)
{
	this->CabinList = cabinList;
	//�նλ�������
	sort(CabinList.begin(), CabinList.end(),sort_token);
	CabinList.erase(unique(CabinList.begin(),CabinList.end()),CabinList.end());
	for (vector<CabinInfo>::iterator it = CabinList.begin(); it != CabinList.end();)
	{
		if (it->x1 <= 0)					it = CabinList.erase(it);
		else if (it->x1 > (ShapeFunc.end()-1)->getX())	it = CabinList.erase(it);
		else								++it;
	}
	if (CabinList.size() == 0)
	{
		CabinList.resize(1);
		CabinList[0].x1 = (ShapeFunc.end()-1)->getX();
		CabinList[0].SiteNum = 10;
		CabinList[0].bulkheadNum = 5;
	}
	CabinList[0].x0 = 0;
	for (int i = 1; i < CabinList.size(); i++)
		CabinList[i].x0 = CabinList[i-1].x1;
	if ((CabinList.end()-1)->x1 != (ShapeFunc.end()-1)->getX())
		(CabinList.end()-1)->x1 = (ShapeFunc.end()-1)->getX();
}

void ShapeMash::SetBeamProperty(vector<PBARL> property_list, vector<MAT1> Material_list, vector<MAT8> Material8_list) 
{ 
	this->property_list = property_list;
	this->Material_list = Material_list;
	this->Material8_list = Material8_list;
}

int ShapeMash::updateCabinListT(const vector<PSHELL>& ps)
{
	if (CabinList.size()!= ps.size())
	{
		cout << "�ն������Ͳ�������������һ���࣡���󣡣�������" << endl;
		return -1;
	}
	for (size_t i = 0; i < CabinList.size(); i++)
	{
		CabinList[i].T = ps[CabinList[i].MID - 1].T;
	}
	return 0;
}

void ShapeMash::SetStruct(vector<int> beamPID, vector<double> bulkhead /* = vector<double>(0) */,vector<int>bulkheadPID/* = vector<int>(0)*/)
{
	this->beamNum = beamPID.size();	//��������
	this->beamPID = beamPID;		//�����Ĳ������Ա��
	if (bulkhead.size() == 0)//���δ�������λ������ݲն����ɸ���λ��
	{
		for (int i = 0; i < CabinList.size(); i++)
		{
			double x0 = 0;
			if (i > 0) x0 = CabinList[i-1].x1;
			double length = CabinList[i].x1 - x0;
			if (CabinList[i].bulkheadNum < 0) CabinList[i].bulkheadNum = 0;
			for (int j = 0; j < CabinList[i].bulkheadNum; j++)
			{
				bulkhead.push_back(x0 + (double)j * length / (double)CabinList[i].bulkheadNum);
				this->bulkheadPID.push_back(CabinList[i].PBARL_list[j]);//�����������
			}
		}
	}
	else//�������˸���λ�ã�����ݶ������ɸ���λ��
	{
		sort(bulkhead.begin(), bulkhead.end());
		bulkhead.erase(unique(bulkhead.begin(),bulkhead.end()),bulkhead.end());
	}
	this->bulkhead = bulkhead;
	this->bulkheadID.resize(bulkhead.size());
}

void ShapeMash::CalcPoint()
{
	if (aeroinfo.CPmatrix.size() == 0)
	{
		cout << "��ǰδ�������������ݣ����򽫲��������������ļ�!" << endl;
	}
	double deltaFai = 2 * pi / FaiNum;
	Xsite.insert(Xsite.end(), bulkhead.begin(), bulkhead.end());
	sort(Xsite.begin(), Xsite.end());
	for (vector<double>::iterator it = Xsite.begin(); it != Xsite.end()-1; it++)
	{
		if (abs(*(it) - *(it+1)) < 10e-6)
		{
			it = Xsite.erase(it);
			if (it != Xsite.begin())
			{
				it--;
			}
		}
	}
	//Xsite.erase(unique(Xsite.begin(),Xsite.end()),Xsite.end());
	point.resize(Xsite.size());
	int bhNum = 0;
	int cbNum1 = 0;
	int cbNum2 = 0;
	for (int i = 0; i < Xsite.size(); i++)
	{
		double x = Xsite[i];
		//��ȡ��������xsite�е�λ�ã��õ�xsite�е�ID��
		for (int n = bhNum; n < bulkhead.size(); n++)
		{
			if (abs(bulkhead[n]-x) < 1e-5)
			{
				bulkheadID[n] = i;
				bhNum++;
				break;
			}
		}
		//��ȡ�ն��ڽڵ��е�λ�ã��õ��ܵĽڵ�PID��
		for (int j = cbNum1; j < CabinList.size();j++)
		{
			if (CabinList[j].x0 <= x)
			{
				CabinList[j].cabinPID0 = i * FaiNum;
				cbNum1++;
				break;
			}
		}
		for (int j = cbNum2; j < CabinList.size(); j++)
		{
			if (CabinList[j].x1 <= x)
			{
				CabinList[j].cabinPID1 = i * FaiNum - 1;
				cbNum2++;
				break;
			}
		}
		//��ȡ�������������ڽڵ��е�λ�ã��õ�Xid��
		if (i > 0 && x <=  EnginF.origin.getX())//������ӵ�ԭ�㵼�³������
		{
			EnginF.SetXsiteID(i);
		}


		double radius = 0;
		point[i].resize(FaiNum);
		for (int j = 0;j < FaiNum; j++)
		{
			
			for (int n = 0; n < ShapeFunc.size(); n++)
			{
				if (abs(ShapeFunc[n].getX() - x) < 1e-5)
				{
					radius = ShapeFunc[n].getY();
					break;
				}
				else if (ShapeFunc[n].getX() < x && x <= ShapeFunc[n+1].getX())
				{
					double x0 = ShapeFunc[n].getX();
					double x1 = ShapeFunc[n+1].getX();
					double y0 = ShapeFunc[n].getY();
					double y1 = ShapeFunc[n+1].getY();
					radius = y0 + (y1-y0)*(x-x0)/(x1-x0);
					break;
				}
			}
			double phi_ = deltaFai*j;
			double y = radius * cos(phi_);
			double z = radius * sin(phi_);
			point[i][j] = Point(x,y,z);
			//�õ����ϵ�ѹǿֵ
			if (pi < phi_ && phi_ < 2*pi)
			{
				phi_ = 2*pi - phi_;
			}
			else if (phi_ >= 2*pi || phi_ < 0)
			{
				phi_ = 0;
			}
			point[i][j].setP(aeroinfo.getP(x, phi_));
		}
	}
}
// #define ELEM3
void ShapeMash::CalcMesh()
{
	int numEta = point.size();
	int numFai = point[0].size();

	meshSkin.P.resize(numEta * numFai);

	meshSkin.E.clear();
	//1������Ƥ��Ԫ
	meshSkin.E.push_back(vector<vector<int>>((numEta-1) * numFai));

	for (int i = 0; i < numEta; i++)
	{
		for (int j = 0; j < numFai; j++)
		{
			meshSkin.P[i*numFai+j] = point[i][j];
			#ifdef ELEM3
			if (i == 0)//�����ε�Ԫ
			{
				if (i < numEta-1 && j < numFai-1)
				{
					meshSkin.E[0][i*(numFai)+j].resize(3);
					meshSkin.E[0][i*(numFai)+j][0] = 0;
					meshSkin.E[0][i*(numFai)+j][1] = numFai*(i+1)+j;
					meshSkin.E[0][i*(numFai)+j][2] = numFai*(i+1)+(j+1);
				}
				else if (i < numEta-1 && j == numFai-1)
				{
					meshSkin.E[0][i*(numFai)+j].resize(3);
					meshSkin.E[0][i*(numFai)+j][0] = 0;
					meshSkin.E[0][i*(numFai)+j][1] = numFai*(i+1)+j;
					meshSkin.E[0][i*(numFai)+j][2] = numFai*(i+1)+0;
				}
			}
			else//�ı��ε�Ԫ
			# endif
			{
				if (i < numEta-1 && j < numFai-1)
				{
					meshSkin.E[0][i*(numFai)+j].resize(4);
					meshSkin.E[0][i*(numFai)+j][0] = numFai*i+j;
					meshSkin.E[0][i*(numFai)+j][1] = numFai*(i+1)+j;
					meshSkin.E[0][i*(numFai)+j][2] = numFai*(i+1)+(j+1);
					meshSkin.E[0][i*(numFai)+j][3] = numFai*i+(j+1);
				}
				else if (i < numEta-1 && j == numFai-1)
				{
					meshSkin.E[0][i*(numFai)+j].resize(5);
					meshSkin.E[0][i*(numFai)+j][0] = numFai*i+j;
					meshSkin.E[0][i*(numFai)+j][1] = numFai*(i+1)+j;
					meshSkin.E[0][i*(numFai)+j][2] = numFai*(i+1)+0;
					meshSkin.E[0][i*(numFai)+j][3] = numFai*i+0;
				}
			}
			
		}
	}
	//2����Ԫ
	meshSkin.E.push_back(vector<vector<int>>(bulkheadID.size() * numFai));
	for (int n = 0; n < bulkheadID.size(); n++)//ÿ������
	{
		int i = bulkheadID[n];
		for (int j = 0; j < numFai; j++)
		{
			if (j < numFai-1)
			{
				meshSkin.E[1][n*(numFai)+j].resize(3);
				meshSkin.E[1][n*(numFai)+j][0] = numFai*i+j;
				meshSkin.E[1][n*(numFai)+j][1] = numFai*i+(j+1);
				meshSkin.E[1][n*(numFai)+j][2] = bulkheadPID[n];//��������������õ�Ԫ�Ĳ�������
			}
			else if (j == numFai-1)
			{
				meshSkin.E[1][n*(numFai)+j].resize(3);
				meshSkin.E[1][n*(numFai)+j][0] = numFai*i+j;
				meshSkin.E[1][n*(numFai)+j][1] = numFai*i+0;
				meshSkin.E[1][n*(numFai)+j][2] = bulkheadPID[n];//��������������õ�Ԫ�Ĳ�������
			}
		}
	}
	//3������Ԫ
	meshSkin.E.push_back(vector<vector<int>>((numEta-1) * (beamNum)));
	if (beamNum > 0)
	{
		int deltaBeam = numFai / beamNum;
		for (int j = 0; j < beamNum; j++)//ÿ������
		{
			for (int i = 0; i < numEta-1; i++)
			{
				meshSkin.E[2][j*(numEta-1)+i].resize(3);
				meshSkin.E[2][j*(numEta-1)+i][0] = numFai * i + (j * deltaBeam);
				meshSkin.E[2][j*(numEta-1)+i][1] = numFai * (i+1) + (j * deltaBeam);
				meshSkin.E[2][j*(numEta-1)+i][2] = beamPID[j];//��������������õ�Ԫ�Ĳ�������
			}
		}
	}
}

void ShapeMash::CalcMassPoint()
{
	masspoint.clear();
	if (MassDistriType == 0)//ԭʼ�����ֲ�������������ĸ�����������ȷֲ�������Ԫ
	{
		for (int i = 0; i < Instrument.size(); i++)
		{
			if (abs(bulkhead[bulkheadID.size()-1] - Instrument[i].GetXc()) < 1e-5)//�Ƿ��������ڵ���ĩ��
			{
				if (masspoint.count(bulkheadID[bulkheadID.size()-1]) == 0)
					masspoint.insert(pair<int,double>(bulkheadID[bulkheadID.size()-1],Instrument[i].GetMass()));
				else
					masspoint[bulkheadID[bulkheadID.size()-1]] += Instrument[i].GetMass();
			}
			else
			{
				for (int id = 0; id < bulkheadID.size()-1; id++)
				{
					if (abs(bulkhead[id] - Instrument[i].GetXc()) < 1e-5)//�Ƿ��������ڵ�ǰidλ�ô�
					{
						if (masspoint.count(bulkheadID[id]) == 0)
							masspoint.insert(pair<int,double>(bulkheadID[id],Instrument[i].GetMass()));
						else
							masspoint[bulkheadID[id]] += Instrument[i].GetMass();
						break;
					}
					else if (bulkhead[id] < Instrument[i].GetXc() && Instrument[i].GetXc() < bulkhead[id+1])//�Ƿ���������id��id+1֮��
					{
						double m0 = Instrument[i].GetMass() * (bulkhead[id+1] - Instrument[i].GetXc()) / (bulkhead[id+1]-bulkhead[id]);
						double m1 = Instrument[i].GetMass() * (Instrument[i].GetXc() - bulkhead[id]) / (bulkhead[id+1]-bulkhead[id]);
						if (masspoint.count(bulkheadID[id]) == 0)
							masspoint.insert(pair<int,double>(bulkheadID[id],m0));
						else
							masspoint[bulkheadID[id]] += m0;
						if (masspoint.count(bulkheadID[id+1]) == 0)
							masspoint.insert(pair<int,double>(bulkheadID[id+1],m1));
						else
							masspoint[bulkheadID[id+1]] += m1;
						break;
					}
				}
			}
		}
	}
	else//�µ�������ӷ��������Ӳο���������λ�ã�����RBE3���������㵽�ն���
	{
		for (int i = 0; i < Instrument.size(); i++)
		{
			if (Instrument[i].IsOnCabin)//�ڲն���
			{
				for (int j = 0; j < CabinList.size(); j++)
				{
					if (CabinList[j].x0 < Instrument[i].GetXc() && Instrument[i].GetXc() <= CabinList[j].x1)
					{
						Instrument[i].cabinID = j;
						//ͨ�����ڲն���������������������������������壬��˼�������Ҫ��ȥ����������59257.3-68522
						double pointmass = Instrument[i].GetMass() - CabinList[j].mass;
						if (pointmass < 0)
						{
							cout << "����ķ������ն�����С�ڿ�����������������ֵ��" << endl;

							pointmass = 0;
						}
						Instrument[i].SetMass(pointmass);
					}
				}
			}
			else//�ڸ�����
			{
				for (int j = 0; j < bulkhead.size(); j++)
				{
					if (bulkhead[j] < Instrument[i].GetXc()) //&& Instrument[i].GetXc() <= CabinList[j]
					{
						Instrument[i].bulkheadID0 = j;
					}
					if (Instrument[i].GetXc() <= bulkhead[bulkhead.size()-j-1])
					{
						Instrument[i].bulkheadID1 = bulkhead.size()-j-1;
					}
				}
			}

		}
	}

}

void ShapeMash::CalcCabin()
{
	//����նα�����ͱں�
	double CabinArea = 0;
	for (int id = 0; id < CabinList.size(); id++)
	{
		for (int i = 0; i < point.size() - 1; i++)
		{
			if (CabinList[id].x0 <= point[i][0].getX() && point[i][0].getX() < CabinList[id].x1)
			{
				double length = sqrt(pow(point[i + 1][0].getX() - point[i][0].getX(), 2) + pow(point[i + 1][0].getY() - point[i][0].getY(), 2));
				CabinList[id].Area += 2 * pi * point[i][0].getY() * length;
			}
		}
		CabinArea += CabinList[id].Area;

		if (CabinList[id].SetTnotMass)//ͨ����ȼ�������
		{
			CabinList[id].mass = CabinList[id].T * GetMAT8(CabinList[id].MID).RHO * CabinList[id].Area;
		}
		else//ͨ������������
		{
			CabinList[id].T = CabinList[id].mass / GetMAT8(CabinList[id].MID).RHO / CabinList[id].Area;
		}
	}
}

int ShapeMash::PrintMesh(string fileName, int SOL)
{
	NasPrinter np;
	const string names = fileName.substr(fileName.find_last_of("\\") + 1);

	int ID = 0;//�ڵ���
	int EID = 0;//��Ԫ���
	int PID = 0;//����or�ǵ�Ԫ���Ա��
	int SID = 0;//�غ�orԼ���������

	//-----------------------------��Ԫ�ڵ���Ϣ----------------------------------
	//----------------node----------------
	for (int i = 0; i < meshSkin.P.size(); i++)
	{
		++ID;
		if (ID == 1 || !(meshSkin.P[i]==Point(0,0,0)))
		{
			np.addGRID(ID, 0, meshSkin.P[i]);
		}
		
	}
	//----------------element----------------
	int cabinID = 0;
	SID++;
	vector<PSHELL> PSHELLlist;
	for (int i = 0; i < meshSkin.E[0].size(); i++)//����ն�������Ԫ
	{
		const int iID = meshSkin.E[0][i][0] / FaiNum;
		const int jID = meshSkin.E[0][i][0] % FaiNum;
		//�����������
		if (cabinID < CabinList.size() && Xsite[iID] >= CabinList[cabinID].x0 && jID == 0)
		{
			//----------------property----------------
			PSHELL ps;
			ps.PID = ++PID;
			ps.MID = CabinList[cabinID].MID;
			ps.name = "cabin_" + to_string(cabinID + 1);
			ps.T = CabinList[cabinID].T;
			if (SOL == 200)
			{
				ps.t_min = CabinList[cabinID].t_min;
				ps.t_max = CabinList[cabinID].t_max;
			}
			PSHELLlist.push_back(ps);
#ifdef ONLY_2_MAT
			if (ps.PID == 1)
			{
				np.addPSHELL(ps);
			}
#else
			np.addPSHELL(ps);
#endif
			cabinID++;
		}
		//��Ԫ���
#ifdef ELEM3
		if (meshSkin.E[0][i].size() == 3)//ͷ׶����Ϊ���ڵ㵥Ԫ
		{
			const int G1 = meshSkin.E[0][i][0] + 1;//�ڵ�1 CID
			const int G2 = meshSkin.E[0][i][1] + 1;//�ڵ�2 CID
			const int G3 = meshSkin.E[0][i][2] + 1;//�ڵ�3 CID
#ifdef ONLY_2_MAT
			np.addCTRIA3(++EID, 1, G1, G2, G3);
#else
			np.addCTRIA3(++EID, PID, G1, G2, G3);
#endif
			//��Ԫ�����
			const double press =-(
				meshSkin.P[meshSkin.E[0][i][0]].getP() +
				meshSkin.P[meshSkin.E[0][i][1]].getP() +
				meshSkin.P[meshSkin.E[0][i][2]].getP()) / 3.0;
			np.addPLOAD(SID, press, G1, G2, G3);
		}
		else//���Ϊ�Ľڵ㵥Ԫ
#endif
		{
			const int G1 = meshSkin.E[0][i][0] + 1;
			const int G2 = meshSkin.E[0][i][1] + 1;
			const int G3 = meshSkin.E[0][i][2] + 1;
			const int G4 = meshSkin.E[0][i][3] + 1;
#ifdef ONLY_2_MAT
			np.addCQUAD4(++EID, 1, G1, G2, G3, G4);
#else
			np.addCQUAD4(++EID, PID, G1, G2, G3, G4);
#endif
			//��Ԫ�����
			const double press = -(
				meshSkin.P[meshSkin.E[0][i][0]].getP() +
				meshSkin.P[meshSkin.E[0][i][1]].getP() +
				meshSkin.P[meshSkin.E[0][i][2]].getP() +
				meshSkin.P[meshSkin.E[0][i][3]].getP()) / 4.0;
			np.addPLOAD(SID, press, G1, G2, G3, G4);
		}
	}
	//----------------element----------------
	////------------------------��Ҫ����ʱ��ȡ���ⲿ��ע��-----------------------------------
#ifdef gekuang
	for (int i = 0; i < meshSkin.E[1].size(); i++)//����ṹ����Ԫ
	{
		const int iID = meshSkin.E[1][i][0] / FaiNum;
		const int jID = meshSkin.E[1][i][0] % FaiNum;
		const int iID2 = meshSkin.E[1][i][1] / FaiNum;
		const int jID2 = meshSkin.E[1][i][1] % FaiNum;

		if ((point[iID][jID] - point[iID2][jID2]).Norm1() > 1e-5)
		{
			double orientX = 0, orientY = 0, orientZ = 0;
			if (abs(point[iID][jID].getY()) < 1e-5 && abs(point[iID][jID].getZ()) < 1e-5)
			{
				orientZ = 1;
			}
			else
			{
				orientY = point[iID][jID].getY();
				orientZ = point[iID][jID].getZ();
			}

#ifdef ONLY_2_MAT
			const double PIDnow = 2;//��Ԫ���Ա�� PID
#else
			const double PIDnow = PID + meshSkin.E[1][i][2];//��Ԫ���Ա�� PID
#endif
			const int GA = meshSkin.E[1][i][0] + 1;//�ڵ�1
			const int GB = meshSkin.E[1][i][1] + 1;//�ڵ�2
			np.addCBAR(++EID, PIDnow, GA, GB, orientX, orientY, orientZ);
		}
	}
#endif
	////------------------------��Ҫ����ʱ��ȡ���ⲿ��ע��-----------------------------------

	//----------------element----------------
	for (int i = 0; i < meshSkin.E[2].size(); i++)//����ṹ������Ԫ
	{
		const int iID = meshSkin.E[2][i][0] / FaiNum;
		const int jID = meshSkin.E[2][i][0] % FaiNum;
		const int iID2 = meshSkin.E[2][i][1] / FaiNum;
		const int jID2 = meshSkin.E[2][i][1] % FaiNum;
		if ((point[iID][jID] - point[iID2][jID2]).Norm1() > 1e-5)
		{
			double orientX = 0, orientY = 0, orientZ = 0;
			if (abs(point[iID][jID].getY()) < 1e-5 && abs(point[iID][jID].getZ()) < 1e-5)
			{
				orientZ = 1;
			}
			else
			{
				orientY = point[iID][jID].getY();
				orientZ = point[iID][jID].getZ();
			}
			const int PIDnow = PID + meshSkin.E[2][i][2];//��Ԫ���Ա�� PID
			
			const int GA = meshSkin.E[2][i][0] + 1;//�ڵ�1
			const int GB = meshSkin.E[2][i][1] + 1;//�ڵ�2
#ifdef ELEM3
			if(meshSkin.P[GA-1]==Point(0,0,0))
			{
				np.addCBAR(++EID, PIDnow, 1, GB, orientX, orientY, orientZ);
			}
			else
#endif
			{
				np.addCBAR(++EID, PIDnow, GA, GB, orientX, orientY, orientZ);
			}
			
		}
	}
#ifdef RBE333
	if (MassDistriType == 0)//�������������
	{
		for (map<int, double>::iterator it = masspoint.begin(); it != masspoint.end(); it++)//���������Ԫ
		{
			const double M = it->second / FaiNum;//��Ԫ������M
			for (int j = 0; j < FaiNum; j++)
			{
				const int G1 = (it->first) * FaiNum + j + 1;//�ڵ�� G1
				np.addCMASS2(++EID, M, G1, 6);
			}
		}
	}
	else
	{
		for (int i = 0; i < Instrument.size(); i++)//����������
		{
			if (Instrument[i].IsOnCabin)
			{//����������
				if (Instrument[i].cabinID > 0)
				{
					//������
					ID++;
					np.addGRID(ID, 0, Instrument[i].pt);

					//���Լ�����Ե�Ԫ
					vector<int> G1j;//����ڵ�
					for (int j = CabinList[Instrument[i].cabinID].cabinPID0; j < CabinList[Instrument[i].cabinID].cabinPID1 + 1; j++)
					{
						G1j.push_back(j + 1);
					}
					
					np.addRBE3(++EID, ID, 123456, 1.0, 123, G1j);
					
				}
			}
			else
			{//���䵽�ն�
				//������
				ID++;
				np.addGRID(ID, 0, Instrument[i].pt);

				//���Լ�����Ե�Ԫ
				vector<int> G1j;//����ڵ�
				if (bulkheadID.size() != 0)
				{
					for (int j = bulkheadID[Instrument[i].bulkheadID0] * FaiNum; j < (bulkheadID[Instrument[i].bulkheadID0] + 1)*FaiNum + 1; j++)
					{
						G1j.push_back(j + 1);
					}
					for (int j = bulkheadID[Instrument[i].bulkheadID1] * FaiNum; j < (bulkheadID[Instrument[i].bulkheadID1] + 1)*FaiNum + 1; j++)
					{
						G1j.push_back(j + 1);
					}
				}
				np.addRBE3(++EID, ID, 123456, 1.0, 123, G1j);
			}
		}
	}
#endif
	//��ӷ���������
	//�������õ�
	if (abs(EnginF.force.getX()) > 10e-5)
	{
		SID++;
		ID++;
		np.addGRID(ID, 0, EnginF.origin);
		//���Լ�����Ե�Ԫ
		vector<int> G1j;//����ڵ�
		for (int j = EnginF.getXid()*FaiNum; j < (EnginF.getXid() + 1)*FaiNum; j++)
		{
			G1j.push_back(j + 1);
		}
		//np.addRBE3(++EID, ID, 123456, 1.0, 123456, G1j);
		np.addRBE2(++EID, ID, 123456, G1j);
		np.addFORCE(SID, ID, 0, 1.0, EnginF.force);

	}
	//�������
	//double grav_a = 9.8;//���ٶȷ�ֵ
	//double grav_n1 = sin(aeroinfo.alpha * pi / 180.0);
	//double grav_n2 = -cos(aeroinfo.alpha * pi / 180.0);
	//double grav_n3 = 0;
	//np.addGRAV(++SID, 0, grav_a, grav_n1, grav_n2, grav_n3);

	//��ӡ����Ԫ���ԺͲ�������

#ifdef ONLY_2_MAT
	property_list[0].PID = 2;
	property_list[0].MID = 2;
	np.addPBARL(property_list[0]);
	Material8_list[0].MID = 1;
	np.addMAT8(Material8_list[0]);
	Material_list[0].MID = 2;
	np.addMAT1(Material_list[0]);
#else
	for (int i = 0; i < property_list.size(); i++)//��ӡ��������������
	{
		property_list[i].PID += PID;
		np.addPBARL(property_list[i]);
	}
	for (int i = 0; i < Material8_list.size(); i++)//��ӡ�նο����������
	{
		np.addMAT8(Material8_list[i]);
	}
	for (int i = 0; i < Material_list.size(); i++)//��ӡ�����������
	{
		np.addMAT1(Material_list[i]);
	}
#endif

	vector<pair<double, int>> LOADi(SID);
	for (int i = 0; i < SID; i++)
	{
		LOADi[i] = pair<double, int>(1.0, i + 1);
	}
	np.addLOAD(++SID, 1.0, LOADi); //�������غ�

	//--------------------------------------------------
	switch (SOL)
	{
	case 101: {
		//ִ�п��Ʋ���
		//ofs3 << "NASTRAN parallel=8" << endl;//��Ӧ��Ϲ�����SID
		np.ssHeader << "SOL 101" << endl;
		//������������
		np.ssHeader << "CEND" << endl;
		np.ssHeader << "TITLE = MSC.Nastran job for Missile" << endl;
		np.ssHeader << "ECHO = NONE" << endl;
		np.ssHeader << "SUBCASE 1" << endl;//��һ������
		np.ssHeader << "   SUBTITLE=Default" << endl;
		np.ssHeader << "   LOAD = " << SID << endl;//��Ӧ��Ϲ�����SID

											  //ofs3 << "   DISPLACEMENT(SORT1,REAL)=ALL" << endl;//���λ��
		np.ssHeader << "   STRESS(SORT1,PUNCH,REAL,VONMISES)=ALL" << endl;//,BILIN�����ԪӦ�� ,PUNCH
		np.ssHeader << "   STRAIN(SORT1,PUNCH,REAL,VONMISES)=ALL" << endl;//,BILIN
		np.ssHeader << "   DISPLACEMENT(SORT1,REAL)=ALL" << endl;//
																		 //BEGIN BULK
		np.ssHeader << "$$------------------------------------------------------------------------------$" << endl;
		np.ssHeader << "$$                                Bulk Data Cards                               $" << endl;
		np.ssHeader << "$$------------------------------------------------------------------------------$" << endl;
		np.ssHeader << "BEGIN BULK" << endl;
		np.ssHeader << "PARAM   POST     0" << endl;//����'.XBD'���Ϳ��ӻ��ļ�
		np.ssHeader << "PARAM   PRTMAXIM YES" << endl;
		np.ssHeader << "PARAM,INREL,-2" << endl;//��������ͷ�
	}	break;
	case 103: {//ģ̬����
		//ִ�п��Ʋ���
		np.ssHeader << "SOL 103" << endl;
		np.ssHeader << "CEND" << endl;
		np.ssHeader << "ECHO = NONE" << endl;
		np.ssHeader << "$ Using Nastran default values for RESVEC" << endl;
		np.ssHeader << "SUBCASE 1" << endl;
		np.ssHeader << "$ Subcase name : case0" << endl;
		np.ssHeader << "   SUBTITLE=case0" << endl;
		np.ssHeader << "   METHOD = 1" << endl;
		np.ssHeader << "   VECTOR(SORT1,REAL)=ALL" << endl;
		np.ssHeader << "   SPCFORCES(SORT1,REAL)=ALL" << endl;
		//BEGIN BULK
		np.ssHeader << "$$------------------------------------------------------------------------------$" << endl;
		np.ssHeader << "$$                                Bulk Data Cards                               $" << endl;
		np.ssHeader << "$$------------------------------------------------------------------------------$" << endl;
		np.ssHeader << "BEGIN BULK" << endl;
		np.ssHeader << "PARAM    POST    0" << endl;
		np.ssHeader << "PARAM   PRTMAXIM YES" << endl;
		np.ssHeader << "PARAM,INREL,-2" << endl;//��������ͷ�
		//np.ssHeader << "EIGRL    1                       10      0" << endl;
		np.addEIGRL(1, this->Modal_order, 0);
	}	break;
	case 105: {//��������
		//ִ�п��Ʋ���
		np.ssHeader << "SOL 106" << endl;
		//������������
		np.ssHeader << "CEND" << endl;
		np.ssHeader << "TITLE = MSC.Nastran job for Single StructPart" << endl;
		np.ssHeader << "ECHO = NONE" << endl;
		//ofs3 << "SPC  = " << SID_SPC << endl;//��Ӧ��Ϲ�����SID
		np.ssHeader << "SUBCASE 1" << endl;//��һ������
		np.ssHeader << "PARAM,INREL,-2" << endl;//��������ͷ�
		np.ssHeader << "   NLPARM = 11753" << endl;
		np.ssHeader << "   METHOD = 1" << endl;
		np.ssHeader << "   LOAD = " << SID << endl;//��Ӧ��Ϲ�����SID
		np.ssHeader << "   DISPLACEMENT(SORT1,REAL)=ALL" << endl;
		np.ssHeader << "   SPCFORCES(SORT1,REAL)=ALL" << endl;
		np.ssHeader << "   STRESS(SORT1,REAL,VONMISES,BILIN)=ALL" << endl;
		np.ssHeader << "$$------------------------------------------------------------------------------$" << endl;
		np.ssHeader << "$$                                Bulk Data Cards                               $" << endl;
		np.ssHeader << "$$------------------------------------------------------------------------------$" << endl;
		np.ssHeader << "BEGIN BULK" << endl;
		np.ssHeader << "PARAM   POST     0" << endl;//����'.XBD'���Ϳ��ӻ��ļ�
		np.ssHeader << "PARAM   AUTOSPC  NO" << endl;
		np.ssHeader << "PARAM   LGDISP   1" << endl;
		np.ssHeader << "PARAM   PRTMAXIM YES" << endl;
		np.ssHeader << "NLPARM  11753    10              AUTO    5       25" << endl;
		np.ssHeader << "PARAM   BUCKLE   2" << endl;
		np.ssHeader << "EIGRL    1                       5       0" << endl;
		np.ssHeader << "EIGRL   10                      5" << endl;//ģ̬������ƹؼ��� ���ǰ���ģ̬
	}	break;
	case 200: {//�Ż�����
		const int DESOBJ_ID = PSHELLlist.size() + 1;
		const int DESSUB_ID = 1;
		//ִ�п��Ʋ���
		np.ssHeader << "SOL 200" << endl;
		//������������
		np.ssHeader << "CEND" << endl;
		np.ssHeader << "TITLE = MSC.Nastran job for Single StructPart" << endl;
		np.ssHeader << "ECHO = NONE" << endl;
		np.ssHeader << "STRESS(SORT1,PUNCH,REAL,VONMISES)=ALL" << endl;//�����ԪӦ�� ,PUNCH
		np.ssHeader << "STRAIN(SORT1,PUNCH,REAL,VONMISES)=ALL" << endl;//,BILIN
		np.ssHeader << "ANALYSIS = STATICS" << endl;//�����������
		np.ssHeader << "LOAD = " << SID << endl;//��Ӧ��Ϲ�����SID
		np.ssHeader << "DESOBJ(MIN) = "<< DESOBJ_ID << endl;//����Ŀ����Ӧ����
		np.ssHeader << "DESSUB = " << DESSUB_ID << endl;
		np.ssHeader << "PARAM,NASPRT,0" << endl;
		np.ssHeader << "$$------------------------------------------------------------------------------$" << endl;
		np.ssHeader << "$$                                Bulk Data Cards                               $" << endl;
		np.ssHeader << "$$------------------------------------------------------------------------------$" << endl;
		np.ssHeader << "BEGIN BULK" << endl;
		np.ssHeader << "PARAM    POST    0" << endl;
		np.ssHeader << "PARAM   PRTMAXIM YES" << endl;
		np.ssHeader << "PARAM,INREL,-2" << endl;//��������ͷ�

		for (int ID = 1; ID < PSHELLlist.size()+1; ID++)//�������
		{
			const PSHELL& ps = PSHELLlist[ID - 1];
			const MAT8& mat8 = Material8_list[ps.MID - 1];
			np.addDESVAR(ID, "T" + to_string(ID), ps.T, ps.t_min, ps.t_max, 0.01);//�������
			np.addDVPREL1(ID, "PSHELL", ID, "T", vector<pair<double, int>>(1, pair<double, int>(1, ID)));//���ӱ���������

			np.addDRESP1_STRESS(ID, "xy" + to_string(ID), "PSHELL", 9, vector<int>(1, ID));//����Ӧ����ӦCabinElem
			//np.addDRESP1_STRESS(ID, "xy" + to_string(ID), "ELEM", 9, CabinElem[ID]);//����Ӧ����ӦCabinElem
			np.addDCONSTR(DESSUB_ID, ID, mat8.stress_min, mat8.stress_max);//������Ӧ��Ӧ��Լ��
		}
		np.addDRESP1_WEIGHT(DESOBJ_ID, "weight");//��������Լ��
		np.addDOPTPRM({
			"DESMAX",np.int2str(100)//��������������Ϊ100
		});

	}	break;
	default:
		cout << "��ǰ�������Ϊ��" << SOL << "    �������ݲ�֧�֣�����������SOLtype_����" << endl;
		return -1;
	}

	string::size_type iPos = (fileName.find_last_of('\\') + 1) == 0 ? fileName.find_last_of('/') + 1 : fileName.find_last_of('\\') + 1;
	const string datapath = fileName.substr(0, iPos);//��ȡ�ļ�·��

	np.PrintBDF(datapath, names, NasPrinter::onefile);
	return 0;
}

int find_process(string process_name)
{
	int count=0;//���̼��� 
	PROCESSENTRY32 pe32;

	pe32.dwSize=sizeof(PROCESSENTRY32);
	HANDLE process_snapshot_handle=CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);//�������̿��վ��

	if(process_snapshot_handle==INVALID_HANDLE_VALUE) return -1;//�������ʧ��

	bool is_exist=Process32First(process_snapshot_handle,&pe32);//�ҵ�һ��
	while(is_exist)
	{
		if(!stricmp(process_name.c_str(),_bstr_t(pe32.szExeFile))) count++;//�����������ִ�Сд
		is_exist=Process32Next(process_snapshot_handle,&pe32);//����һ��
	}
	return count;
}

int myNastran::NastranCalc()
{
	cout << "Nastran���ڵ��á�������" << endl;
	string command = NastranPath + " " + BDFPath + " out=" +WorkPath;

	remove(logPath.c_str());//���֮ǰ��ͬ��log�ļ�������ɾ��ԭ�ȵ�log�ļ�
	remove(xdbPath.c_str());//���֮ǰ��ͬ��xbd�ļ�������ɾ��ԭ�ȵ�log�ļ�
	remove(pchPath.c_str());//���֮ǰ��ͬ��pch�ļ�������ɾ��ԭ�ȵ�log�ļ�
	system(command.c_str());

	
	
	//�ж�log�ļ��Ƿ����ɳɹ�
	clock_t brginTime = clock();
	while(true)
	{
		ifstream ifs(logPath);
		if (!ifs.fail())
		{
			//cout << logPath << " �ļ��ѳɹ�����" << endl;
			cout << " | Nastran���������С�������" << endl;;
			break;
		}
		//else if (clock() - brginTime > 20000)
		//{
		//	cout << " | δ����ȷ����Nastran������ "<< BDFPath <<" �ļ��Ƿ����*******" << endl;
		//	//system("pause");
		//	return -1;
		//}
	}
	//�ж�nastran.exe�Ƿ����н���
	while(true)
	{
		int count=0;
		string process_name="nastran.exe";

		count=find_process(process_name);
		if (count == 0)
		{
			cout << " | nastran.exe ���н���" << endl;
			break;
		}
	}
	//��ȡlog�ļ��жϷ����Ƿ�����
	bool isFinishCalc = false;
	clock_t brginTime1 = clock();
	while(true)
	{
		ifstream ifs(logPath);
		string str_line;
		while (getline(ifs, str_line))
		{
			string::size_type idx = str_line.find("Analysis complete");
			if (idx != string::npos)//���ּ�����ɵı�ʶ��----����ѭ����׼����ȡ����
			{
				isFinishCalc = true;
				
				ifs.close();
				break;
			}
			
		}
		if (isFinishCalc)
		{
			//system("C:\\Users\\yycab\\Desktop\\bdf\\nastranTESTfile\\KillNastran.bat");//dos()ɱ��nastran���� ����д �����ټӰ�
			break;
		}
		else if (clock() - brginTime1 > 8000)
		{
			cout << " | ����8s��û����log�����ɹ���Nastran��������*******" << endl;
			//system("pause");
			return -1;
		}
	}
	//�ж�pch�ļ��Ƿ����ɳɹ�
	clock_t brginTime2 = clock();
	while(true)
	{
		ifstream ifs(pchPath);
		if (!ifs.fail())
		{
			cout << " | "<<pchPath << " �ļ��ѳɹ�����" << endl;
			break;
		}
		else if (clock() - brginTime2 > 8000)
		{
			cout << " | ����8s��δ����ȷ���� '.pch'������ "<< pchPath <<" �ļ��Ƿ����*******" << endl;
			//system("pause");
			return -1;
		}
	}
	//Sleep(2000);//ϵͳ��ͣ����
	cout << "Nastran�����ɹ�" << endl;
	return 0;
}

int myNastran::ReadResPCH200()
{

	return 0;
}

void myNastran::CalcFilePath()
{
	logPath = BDFPath.substr(0,BDFPath.find(".")) + ".log";
	xdbPath = BDFPath.substr(0,BDFPath.find(".")) + ".xdb";
	pchPath = BDFPath.substr(0,BDFPath.find(".")) + ".pch";
}

int myNastran::ReadResPCH(const vector<int>& CQUAD4id, const vector<int>&CBARid)
{
	ifstream ifs(pchPath);
	if (ifs.fail())
	{
		cout << "Nastran����ʧ�ܣ�.pch �ļ������ڣ������������" << endl;
		return -1;
	}
	else cout << "���ݽ���ļ��򿪳ɹ��� ���ڷ������ݡ���" << endl;
	
	ElemType elemType = ElemType::undefine;
	LoadType loadtype = LoadType::undefine;//1��ԪӦ�� 2��ԪӦ��
	psList.clear();

	string str_line;
	while (getline(ifs, str_line))
	{
		if (str_line.find("SUBTITLE") != string::npos)//���ֱ�ʶ��----׼����ȡ����
		{
			getline(ifs, str_line);//�ն�1��
			getline(ifs, str_line);

			if (str_line.find("STRAINS") != string::npos)			loadtype = LoadType::Strain;//cout << "�ҵ���Ӧ������" << endl;
			else if (str_line.find("STRESSES") != string::npos)		loadtype = LoadType::Stress;//cout << "�ҵ���Ӧ������" << endl;
			
			getline(ifs, str_line);
			getline(ifs, str_line);//�ն�����
			getline(ifs, str_line);

			if (str_line.find("QUAD4") != string::npos)			elemType = ElemType::QUAD4;//cout << "�ҵ����ı��ε�Ԫ" << endl;
			else if (str_line.find("TRIA3") != string::npos)	elemType = ElemType::TRIA3;//cout << "�ҵ��������ε�Ԫ" << endl;
			else if (str_line.find("BAR") != string::npos)		elemType = ElemType::BAR;//cout << "�ҵ���һά����Ԫ" << endl;
		}
		else if (str_line.find("PSHELL") != string::npos)
		{
			PSHELL ps;
			ps.PID = atoi(str_line.substr(8 + 16 * 0, 16).c_str());
			ps.MID = atoi(str_line.substr(8 + 16 * 1, 16).c_str());
			ps.T = atof(str_line.substr(8 + 16 * 2, 16).c_str());
			psList.push_back(ps);
		}
		switch (loadtype){
		case myNastran::LoadType::Strain: {
			switch (elemType)			{
			case ElemType::QUAD4: {
				maxstrain.resize(CQUAD4id.size(), 0);//�洢���Ӧ��ֵ
				maxstrainID.resize(CQUAD4id.size(), 0);//�洢���Ӧ�䵥Ԫ���
				int cabinID = 0;
				getline(ifs, str_line);
				while (str_line.find("$") == string::npos)
				{
					const int id = atoi(str_line.substr(0, 17).c_str());//1 
					getline(ifs, str_line);//2
					getline(ifs, str_line); double data = atof(str_line.substr(36, 18).c_str());//3
					//getline(ifs, str_line); double data = atof(str_line.substr(18, 35).c_str());//4

					if (cabinID + 1 < CQUAD4id.size() && id == CQUAD4id[cabinID + 1])
					{
						cabinID++;
					}
					if (abs(maxstrain[cabinID]) < abs(data))
					{
						maxstrain[cabinID] = data;
						maxstrainID[cabinID] = id;
					}
					for (int i = 0; i < 4; i++)
					{
						getline(ifs, str_line);
					}
				}
				elemType = ElemType::undefine;//��Ԫ��ȡ��ϣ�������ͱ���������
				loadtype = LoadType::undefine;//��Ԫ��ȡ��ϣ�������ͱ���������
			}	break;
			case ElemType::TRIA3:			break;
			case ElemType::BAR: {
				maxstrain2.resize(CBARid.size(), 0);//�洢���Ӧ��ֵ
				maxstrainID2.resize(CBARid.size(), 0);//�洢���Ӧ�䵥Ԫ���
				int bulkhID = 0;
				getline(ifs, str_line);
				while (str_line.find("$") == string::npos)
				{
					const int id = atoi(str_line.substr(0, 18).c_str());//1
					if (bulkhID + 1 < CBARid.size() && id == CBARid[bulkhID + 1] + 1)
					{
						bulkhID++;
					}
					getline(ifs, str_line); double dataA = atof(str_line.substr(54, 18).c_str());//2
					if (abs(maxstrain2[bulkhID] < abs(dataA)))
					{
						maxstrain2[bulkhID] = dataA;
						maxstrainID2[bulkhID] = id;
					}
					getline(ifs, str_line);//3
					getline(ifs, str_line);//4
					getline(ifs, str_line); double dataB = atof(str_line.substr(18, 18).c_str());//5
					if (abs(maxstrain2[bulkhID] < abs(dataB)))
					{
						maxstrain2[bulkhID] = dataB;
						maxstrainID2[bulkhID] = id;
					}
					getline(ifs, str_line);
					elemType = ElemType::undefine;//��Ԫ��ȡ��ϣ�������ͱ���������
					loadtype = LoadType::undefine;//��Ԫ��ȡ��ϣ�������ͱ���������
				}
			}	break;
			}
		}	break;
		case myNastran::LoadType::Stress: {
			switch (elemType){
			case ElemType::QUAD4: {
				maxstress.resize(CQUAD4id.size(), 0);//�洢���Ӧ��ֵ
				maxstressID.resize(CQUAD4id.size(), 0);//�洢���Ӧ����Ԫ���
				int cabinID = 0;
				getline(ifs, str_line);
				while (str_line.find("$") == string::npos)//����һֱ����TITLE��ֹͣ
				{
					const int id = atoi(str_line.substr(0, 17).c_str());//1 
					getline(ifs, str_line);//2
					getline(ifs, str_line); const double data = atof(str_line.substr(36, 18).c_str());//3
					//getline(ifs, str_line); double data = atof(str_line.substr(18, 18).c_str());//4
					if (cabinID + 1 < CQUAD4id.size() && id == CQUAD4id[cabinID + 1])
					{
						cabinID++;
					}

					if (abs(maxstress[cabinID]) < abs(data))
					{
						maxstress[cabinID] = data;
						maxstressID[cabinID] = id;
					}
					for (int i = 0; i < 4; i++)
					{
						getline(ifs, str_line);
					}
				}
				elemType = ElemType::undefine;//��Ԫ��ȡ��ϣ�������ͱ���������
				loadtype = LoadType::undefine;//��Ԫ��ȡ��ϣ�������ͱ���������
			}	break;
			case ElemType::TRIA3:			break;
			case ElemType::BAR: {
				maxstress2.resize(CBARid.size(), 0);//�洢���Ӧ��ֵ
				maxstressID2.resize(CBARid.size(), 0);//�洢���Ӧ����Ԫ���
				int bulkhID = 0;
				getline(ifs, str_line);
				while (str_line.find("$") == string::npos)
				{
					int id = atoi(str_line.substr(0, 18).c_str());//1
					if (bulkhID + 1 < CBARid.size() && id == CBARid[bulkhID + 1] + 1)
					{
						bulkhID++;
					}

					getline(ifs, str_line); double dataA = atof(str_line.substr(54, 18).c_str());//string dsadawd = str_line.substr(54,18);//2
					if (abs(maxstress2[bulkhID] < abs(dataA)))
					{
						maxstress2[bulkhID] = dataA;
						maxstressID2[bulkhID] = id;
					}
					getline(ifs, str_line);//3
					getline(ifs, str_line);//4
					getline(ifs, str_line); double dataB = atof(str_line.substr(18, 18).c_str());//5
					if (abs(maxstress2[bulkhID] < abs(dataB)))
					{
						maxstress2[bulkhID] = dataB;
						maxstressID2[bulkhID] = id;
					}
					getline(ifs, str_line);
					elemType = ElemType::undefine;//��Ԫ��ȡ��ϣ�������ͱ���������
					loadtype = LoadType::undefine;//��Ԫ��ȡ��ϣ�������ͱ���������
				}
			}	break;
			}
		}	break;
		default:
			break;
		}
	}
	cout << "������ɣ������ѱ��浽ָ���ļ���" << endl;
	return 0;
}

void ShapeMash::PrintPress(string fileNmae)
{
	ofstream ofs;
	ofs.open(fileNmae, ios::trunc);
	for (int i = 0; i < point.size(); i++)
	{
		for (int j = 0; j < point[i].size(); j++)
		{
			ofs << setw(16) << point[i][j].getP();
		}
		ofs << endl;
	}
	ofs.close();
}

double ShapeMash::GetStructMass()
{
	double mass = 0;
	//���ÿ���նεĿ�������
	//int cbNum = (ExistCabinNum > 0 && ExistCabinNum < CabinList.size()) ? ExistCabinNum : CabinList.size();

	for(int i = 0; i < CabinList.size(); i++)
	{
		mass += CabinList[i].Area * CabinList[i].T * Material8_list[CabinList[i].MID-1].RHO;
	}
	for (int i = 0; i < Instrument.size(); i++)
	{
		//������
		if (Instrument[i].IsOnCabin==true && Instrument[i].cabinID >= 0)
		{
			mass += Instrument[i].GetMass();
		}
		//����
		else if (Instrument[i].IsOnCabin==false)
		{
			mass += Instrument[i].GetMass();
		}
	}
	return mass;
}

PBARL ShapeMash::GetPBARL(int id)
{
	if (0 <= id && id < property_list.size())
	{
		return property_list[id];
	}
	else
	{
		cout << "���������Ա���ѳ����߽磡" << endl;
		return PBARL();
	}
}
MAT1 ShapeMash::GetMAT1(int id)
{
	if (0 <= id && id < Material_list.size())
	{
		return Material_list[id];
	}
	else
	{
		cout << "��������MAT1����ѳ����߽磡" << endl;
		return MAT1();
	}
}
MAT8 ShapeMash::GetMAT8(int id)
{
	id--;
	if (0 <= id && id < Material8_list.size())
	{
		return Material8_list[id];
	}
	else
	{
		cout << "��������MAT8����ѳ����߽磡" << endl;
		return MAT8();
	}
}

//Xid.clear(); EquMass.clear();}



