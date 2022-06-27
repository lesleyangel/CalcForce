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

#define ELEM3		//定义开头封闭
// #define RBE333		//定义生成多点约束来集成集中质量
#define gekuang		//生成隔框
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
	bool ifUseCabinToCalcXsite = true;//使用舱段初始化信息计算轴向坐标点数
	//-----------------------参数设置-----------------------
	SetShapeFunc(info.ShapeFuncList);//设置弹身外形参数
	SetCabin(cabinlist);//设置舱段划分
	SetMassPoint(mplist, 2);//设置集中质量
	SetStruct(info.beamPID);//设置梁的属性，隔框参数根据舱段定义
	SetMeshSkin(info.faiNum, ifUseCabinToCalcXsite);//设置网格参数，周向网格数和轴向节点根据舱段定义
	SetBeamProperty(info.property_list, info.meterial_list, info.meterial8_list);//设置材料属性
	SetForce(info.CPinfo, info.EngineForce);//设置发动机推力
	Modal_order = info.Modal_order;
}

void ShapeMash::CalcModel()
{
	//参数计算
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
	//舱段划分修正
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
		cout << "舱段数量和材料属性数量不一样多！错误！！！！！" << endl;
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
	this->beamNum = beamPID.size();	//横梁数量
	this->beamPID = beamPID;		//横梁的材料属性编号
	if (bulkhead.size() == 0)//如果未定义隔框位置则根据舱段生成隔框位置
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
				this->bulkheadPID.push_back(CabinList[i].PBARL_list[j]);//隔框材料属性
			}
		}
	}
	else//若定义了隔框位置，则根据定义生成隔框位置
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
		cout << "当前未设置气动力数据！程序将不会生成气动力文件!" << endl;
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
		//获取隔框所在xsite中的位置（得到xsite中的ID）
		for (int n = bhNum; n < bulkhead.size(); n++)
		{
			if (abs(bulkhead[n]-x) < 1e-5)
			{
				bulkheadID[n] = i;
				bhNum++;
				break;
			}
		}
		//获取舱段在节点中的位置（得到总的节点PID）
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
		//获取发动机作用力在节点中的位置（得到Xid）
		if (i > 0 && x <=  EnginF.origin.getX())//别手误加到原点导致程序崩溃
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
			//得到点上的压强值
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
	//1表面蒙皮单元
	meshSkin.E.push_back(vector<vector<int>>((numEta-1) * numFai));

	for (int i = 0; i < numEta; i++)
	{
		for (int j = 0; j < numFai; j++)
		{
			meshSkin.P[i*numFai+j] = point[i][j];
			#ifdef ELEM3
			if (i == 0)//三角形单元
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
			else//四边形单元
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
	//2隔框单元
	meshSkin.E.push_back(vector<vector<int>>(bulkheadID.size() * numFai));
	for (int n = 0; n < bulkheadID.size(); n++)//每个隔框
	{
		int i = bulkheadID[n];
		for (int j = 0; j < numFai; j++)
		{
			if (j < numFai-1)
			{
				meshSkin.E[1][n*(numFai)+j].resize(3);
				meshSkin.E[1][n*(numFai)+j][0] = numFai*i+j;
				meshSkin.E[1][n*(numFai)+j][1] = numFai*i+(j+1);
				meshSkin.E[1][n*(numFai)+j][2] = bulkheadPID[n];//第三个参数代表该单元的材料属性
			}
			else if (j == numFai-1)
			{
				meshSkin.E[1][n*(numFai)+j].resize(3);
				meshSkin.E[1][n*(numFai)+j][0] = numFai*i+j;
				meshSkin.E[1][n*(numFai)+j][1] = numFai*i+0;
				meshSkin.E[1][n*(numFai)+j][2] = bulkheadPID[n];//第三个参数代表该单元的材料属性
			}
		}
	}
	//3横梁单元
	meshSkin.E.push_back(vector<vector<int>>((numEta-1) * (beamNum)));
	if (beamNum > 0)
	{
		int deltaBeam = numFai / beamNum;
		for (int j = 0; j < beamNum; j++)//每个横梁
		{
			for (int i = 0; i < numEta-1; i++)
			{
				meshSkin.E[2][j*(numEta-1)+i].resize(3);
				meshSkin.E[2][j*(numEta-1)+i][0] = numFai * i + (j * deltaBeam);
				meshSkin.E[2][j*(numEta-1)+i][1] = numFai * (i+1) + (j * deltaBeam);
				meshSkin.E[2][j*(numEta-1)+i][2] = beamPID[j];//第三个参数代表该单元的材料属性
			}
		}
	}
}

void ShapeMash::CalcMassPoint()
{
	masspoint.clear();
	if (MassDistriType == 0)//原始方法分布质量，按最近的隔框来分配均匀分布质量单元
	{
		for (int i = 0; i < Instrument.size(); i++)
		{
			if (abs(bulkhead[bulkheadID.size()-1] - Instrument[i].GetXc()) < 1e-5)//是否质量点在弹身末端
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
					if (abs(bulkhead[id] - Instrument[i].GetXc()) < 1e-5)//是否质量点在当前id位置处
					{
						if (masspoint.count(bulkheadID[id]) == 0)
							masspoint.insert(pair<int,double>(bulkheadID[id],Instrument[i].GetMass()));
						else
							masspoint[bulkheadID[id]] += Instrument[i].GetMass();
						break;
					}
					else if (bulkhead[id] < Instrument[i].GetXc() && Instrument[i].GetXc() < bulkhead[id+1])//是否质量点在id和id+1之间
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
	else//新的质量添加方法：增加参考点在质心位置，建立RBE3连接质量点到舱段上
	{
		for (int i = 0; i < Instrument.size(); i++)
		{
			if (Instrument[i].IsOnCabin)//在舱段上
			{
				for (int j = 0; j < CabinList.size(); j++)
				{
					if (CabinList[j].x0 < Instrument[i].GetXc() && Instrument[i].GetXc() <= CabinList[j].x1)
					{
						Instrument[i].cabinID = j;
						//通过所在舱段修正发动机质量（输入的质量包括壳体，因此集中质量要减去壳体质量）59257.3-68522
						double pointmass = Instrument[i].GetMass() - CabinList[j].mass;
						if (pointmass < 0)
						{
							cout << "输入的发动机舱段质量小于壳体质量！请检查输入值！" << endl;

							pointmass = 0;
						}
						Instrument[i].SetMass(pointmass);
					}
				}
			}
			else//在隔框上
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
	//计算舱段表面积和壁厚
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

		if (CabinList[id].SetTnotMass)//通过厚度计算质量
		{
			CabinList[id].mass = CabinList[id].T * GetMAT8(CabinList[id].MID).RHO * CabinList[id].Area;
		}
		else//通过质量计算厚度
		{
			CabinList[id].T = CabinList[id].mass / GetMAT8(CabinList[id].MID).RHO / CabinList[id].Area;
		}
	}
}

int ShapeMash::PrintMesh(string fileName, int SOL)
{
	NasPrinter np;
	const string names = fileName.substr(fileName.find_last_of("\\") + 1);

	int ID = 0;//节点编号
	int EID = 0;//单元编号
	int PID = 0;//截面or壳单元属性编号
	int SID = 0;//载荷or约束工况编号

	//-----------------------------单元节点信息----------------------------------
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
	for (int i = 0; i < meshSkin.E[0].size(); i++)//输出舱段气动单元
	{
		const int iID = meshSkin.E[0][i][0] / FaiNum;
		const int jID = meshSkin.E[0][i][0] % FaiNum;
		//定义材料属性
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
		//单元输出
#ifdef ELEM3
		if (meshSkin.E[0][i].size() == 3)//头锥顶部为三节点单元
		{
			const int G1 = meshSkin.E[0][i][0] + 1;//节点1 CID
			const int G2 = meshSkin.E[0][i][1] + 1;//节点2 CID
			const int G3 = meshSkin.E[0][i][2] + 1;//节点3 CID
#ifdef ONLY_2_MAT
			np.addCTRIA3(++EID, 1, G1, G2, G3);
#else
			np.addCTRIA3(++EID, PID, G1, G2, G3);
#endif
			//单元力输出
			const double press =-(
				meshSkin.P[meshSkin.E[0][i][0]].getP() +
				meshSkin.P[meshSkin.E[0][i][1]].getP() +
				meshSkin.P[meshSkin.E[0][i][2]].getP()) / 3.0;
			np.addPLOAD(SID, press, G1, G2, G3);
		}
		else//身后为四节点单元
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
			//单元力输出
			const double press = -(
				meshSkin.P[meshSkin.E[0][i][0]].getP() +
				meshSkin.P[meshSkin.E[0][i][1]].getP() +
				meshSkin.P[meshSkin.E[0][i][2]].getP() +
				meshSkin.P[meshSkin.E[0][i][3]].getP()) / 4.0;
			np.addPLOAD(SID, press, G1, G2, G3, G4);
		}
	}
	//----------------element----------------
	////------------------------需要隔框时请取消这部分注释-----------------------------------
#ifdef gekuang
	for (int i = 0; i < meshSkin.E[1].size(); i++)//输出结构隔框单元
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
			const double PIDnow = 2;//单元属性编号 PID
#else
			const double PIDnow = PID + meshSkin.E[1][i][2];//单元属性编号 PID
#endif
			const int GA = meshSkin.E[1][i][0] + 1;//节点1
			const int GB = meshSkin.E[1][i][1] + 1;//节点2
			np.addCBAR(++EID, PIDnow, GA, GB, orientX, orientY, orientZ);
		}
	}
#endif
	////------------------------需要隔框时请取消这部分注释-----------------------------------

	//----------------element----------------
	for (int i = 0; i < meshSkin.E[2].size(); i++)//输出结构横梁单元
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
			const int PIDnow = PID + meshSkin.E[2][i][2];//单元属性编号 PID
			
			const int GA = meshSkin.E[2][i][0] + 1;//节点1
			const int GB = meshSkin.E[2][i][1] + 1;//节点2
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
	if (MassDistriType == 0)//集中质量的添加
	{
		for (map<int, double>::iterator it = masspoint.begin(); it != masspoint.end(); it++)//输出质量单元
		{
			const double M = it->second / FaiNum;//单元的质量M
			for (int j = 0; j < FaiNum; j++)
			{
				const int G1 = (it->first) * FaiNum + j + 1;//节点号 G1
				np.addCMASS2(++EID, M, G1, 6);
			}
		}
	}
	else
	{
		for (int i = 0; i < Instrument.size(); i++)//创建质量点
		{
			if (Instrument[i].IsOnCabin)
			{//集中质量点
				if (Instrument[i].cabinID > 0)
				{
					//质量点
					ID++;
					np.addGRID(ID, 0, Instrument[i].pt);

					//多点约束刚性单元
					vector<int> G1j;//多个节点
					for (int j = CabinList[Instrument[i].cabinID].cabinPID0; j < CabinList[Instrument[i].cabinID].cabinPID1 + 1; j++)
					{
						G1j.push_back(j + 1);
					}
					
					np.addRBE3(++EID, ID, 123456, 1.0, 123, G1j);
					
				}
			}
			else
			{//分配到舱段
				//质量点
				ID++;
				np.addGRID(ID, 0, Instrument[i].pt);

				//多点约束刚性单元
				vector<int> G1j;//多个节点
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
	//添加发动机推力
	//推力作用点
	if (abs(EnginF.force.getX()) > 10e-5)
	{
		SID++;
		ID++;
		np.addGRID(ID, 0, EnginF.origin);
		//多点约束刚性单元
		vector<int> G1j;//多个节点
		for (int j = EnginF.getXid()*FaiNum; j < (EnginF.getXid() + 1)*FaiNum; j++)
		{
			G1j.push_back(j + 1);
		}
		//np.addRBE3(++EID, ID, 123456, 1.0, 123456, G1j);
		np.addRBE2(++EID, ID, 123456, G1j);
		np.addFORCE(SID, ID, 0, 1.0, EnginF.force);

	}
	//添加重力
	//double grav_a = 9.8;//加速度幅值
	//double grav_n1 = sin(aeroinfo.alpha * pi / 180.0);
	//double grav_n2 = -cos(aeroinfo.alpha * pi / 180.0);
	//double grav_n3 = 0;
	//np.addGRAV(++SID, 0, grav_a, grav_n1, grav_n2, grav_n3);

	//打印梁单元属性和材料属性

#ifdef ONLY_2_MAT
	property_list[0].PID = 2;
	property_list[0].MID = 2;
	np.addPBARL(property_list[0]);
	Material8_list[0].MID = 1;
	np.addMAT8(Material8_list[0]);
	Material_list[0].MID = 2;
	np.addMAT1(Material_list[0]);
#else
	for (int i = 0; i < property_list.size(); i++)//打印隔框梁截面属性
	{
		property_list[i].PID += PID;
		np.addPBARL(property_list[i]);
	}
	for (int i = 0; i < Material8_list.size(); i++)//打印舱段壳体材料属性
	{
		np.addMAT8(Material8_list[i]);
	}
	for (int i = 0; i < Material_list.size(); i++)//打印隔框材料属性
	{
		np.addMAT1(Material_list[i]);
	}
#endif

	vector<pair<double, int>> LOADi(SID);
	for (int i = 0; i < SID; i++)
	{
		LOADi[i] = pair<double, int>(1.0, i + 1);
	}
	np.addLOAD(++SID, 1.0, LOADi); //添加组合载荷

	//--------------------------------------------------
	switch (SOL)
	{
	case 101: {
		//执行控制部分
		//ofs3 << "NASTRAN parallel=8" << endl;//对应组合工况的SID
		np.ssHeader << "SOL 101" << endl;
		//工况控制命令
		np.ssHeader << "CEND" << endl;
		np.ssHeader << "TITLE = MSC.Nastran job for Missile" << endl;
		np.ssHeader << "ECHO = NONE" << endl;
		np.ssHeader << "SUBCASE 1" << endl;//第一个工况
		np.ssHeader << "   SUBTITLE=Default" << endl;
		np.ssHeader << "   LOAD = " << SID << endl;//对应组合工况的SID

											  //ofs3 << "   DISPLACEMENT(SORT1,REAL)=ALL" << endl;//输出位移
		np.ssHeader << "   STRESS(SORT1,PUNCH,REAL,VONMISES)=ALL" << endl;//,BILIN输出单元应力 ,PUNCH
		np.ssHeader << "   STRAIN(SORT1,PUNCH,REAL,VONMISES)=ALL" << endl;//,BILIN
		np.ssHeader << "   DISPLACEMENT(SORT1,REAL)=ALL" << endl;//
																		 //BEGIN BULK
		np.ssHeader << "$$------------------------------------------------------------------------------$" << endl;
		np.ssHeader << "$$                                Bulk Data Cards                               $" << endl;
		np.ssHeader << "$$------------------------------------------------------------------------------$" << endl;
		np.ssHeader << "BEGIN BULK" << endl;
		np.ssHeader << "PARAM   POST     0" << endl;//生成'.XBD'类型可视化文件
		np.ssHeader << "PARAM   PRTMAXIM YES" << endl;
		np.ssHeader << "PARAM,INREL,-2" << endl;//定义惯性释放
	}	break;
	case 103: {//模态分析
		//执行控制部分
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
		np.ssHeader << "PARAM,INREL,-2" << endl;//定义惯性释放
		//np.ssHeader << "EIGRL    1                       10      0" << endl;
		np.addEIGRL(1, this->Modal_order, 0);
	}	break;
	case 105: {//屈曲分析
		//执行控制部分
		np.ssHeader << "SOL 106" << endl;
		//工况控制命令
		np.ssHeader << "CEND" << endl;
		np.ssHeader << "TITLE = MSC.Nastran job for Single StructPart" << endl;
		np.ssHeader << "ECHO = NONE" << endl;
		//ofs3 << "SPC  = " << SID_SPC << endl;//对应组合工况的SID
		np.ssHeader << "SUBCASE 1" << endl;//第一个工况
		np.ssHeader << "PARAM,INREL,-2" << endl;//定义惯性释放
		np.ssHeader << "   NLPARM = 11753" << endl;
		np.ssHeader << "   METHOD = 1" << endl;
		np.ssHeader << "   LOAD = " << SID << endl;//对应组合工况的SID
		np.ssHeader << "   DISPLACEMENT(SORT1,REAL)=ALL" << endl;
		np.ssHeader << "   SPCFORCES(SORT1,REAL)=ALL" << endl;
		np.ssHeader << "   STRESS(SORT1,REAL,VONMISES,BILIN)=ALL" << endl;
		np.ssHeader << "$$------------------------------------------------------------------------------$" << endl;
		np.ssHeader << "$$                                Bulk Data Cards                               $" << endl;
		np.ssHeader << "$$------------------------------------------------------------------------------$" << endl;
		np.ssHeader << "BEGIN BULK" << endl;
		np.ssHeader << "PARAM   POST     0" << endl;//生成'.XBD'类型可视化文件
		np.ssHeader << "PARAM   AUTOSPC  NO" << endl;
		np.ssHeader << "PARAM   LGDISP   1" << endl;
		np.ssHeader << "PARAM   PRTMAXIM YES" << endl;
		np.ssHeader << "NLPARM  11753    10              AUTO    5       25" << endl;
		np.ssHeader << "PARAM   BUCKLE   2" << endl;
		np.ssHeader << "EIGRL    1                       5       0" << endl;
		np.ssHeader << "EIGRL   10                      5" << endl;//模态输出控制关键字 输出前五阶模态
	}	break;
	case 200: {//优化分析
		const int DESOBJ_ID = PSHELLlist.size() + 1;
		const int DESSUB_ID = 1;
		//执行控制部分
		np.ssHeader << "SOL 200" << endl;
		//工况控制命令
		np.ssHeader << "CEND" << endl;
		np.ssHeader << "TITLE = MSC.Nastran job for Single StructPart" << endl;
		np.ssHeader << "ECHO = NONE" << endl;
		np.ssHeader << "STRESS(SORT1,PUNCH,REAL,VONMISES)=ALL" << endl;//输出单元应力 ,PUNCH
		np.ssHeader << "STRAIN(SORT1,PUNCH,REAL,VONMISES)=ALL" << endl;//,BILIN
		np.ssHeader << "ANALYSIS = STATICS" << endl;//定义分析类型
		np.ssHeader << "LOAD = " << SID << endl;//对应组合工况的SID
		np.ssHeader << "DESOBJ(MIN) = "<< DESOBJ_ID << endl;//定义目标响应函数
		np.ssHeader << "DESSUB = " << DESSUB_ID << endl;
		np.ssHeader << "PARAM,NASPRT,0" << endl;
		np.ssHeader << "$$------------------------------------------------------------------------------$" << endl;
		np.ssHeader << "$$                                Bulk Data Cards                               $" << endl;
		np.ssHeader << "$$------------------------------------------------------------------------------$" << endl;
		np.ssHeader << "BEGIN BULK" << endl;
		np.ssHeader << "PARAM    POST    0" << endl;
		np.ssHeader << "PARAM   PRTMAXIM YES" << endl;
		np.ssHeader << "PARAM,INREL,-2" << endl;//定义惯性释放

		for (int ID = 1; ID < PSHELLlist.size()+1; ID++)//定义变量
		{
			const PSHELL& ps = PSHELLlist[ID - 1];
			const MAT8& mat8 = Material8_list[ps.MID - 1];
			np.addDESVAR(ID, "T" + to_string(ID), ps.T, ps.t_min, ps.t_max, 0.01);//定义变量
			np.addDVPREL1(ID, "PSHELL", ID, "T", vector<pair<double, int>>(1, pair<double, int>(1, ID)));//连接变量到参数

			np.addDRESP1_STRESS(ID, "xy" + to_string(ID), "PSHELL", 9, vector<int>(1, ID));//定义应力响应CabinElem
			//np.addDRESP1_STRESS(ID, "xy" + to_string(ID), "ELEM", 9, CabinElem[ID]);//定义应力响应CabinElem
			np.addDCONSTR(DESSUB_ID, ID, mat8.stress_min, mat8.stress_max);//定义响应对应的约束
		}
		np.addDRESP1_WEIGHT(DESOBJ_ID, "weight");//定义质量约束
		np.addDOPTPRM({
			"DESMAX",np.int2str(100)//定义最大迭代次数为100
		});

	}	break;
	default:
		cout << "当前求解类型为：" << SOL << "    该类型暂不支持，请重新输入SOLtype_参数" << endl;
		return -1;
	}

	string::size_type iPos = (fileName.find_last_of('\\') + 1) == 0 ? fileName.find_last_of('/') + 1 : fileName.find_last_of('\\') + 1;
	const string datapath = fileName.substr(0, iPos);//获取文件路径

	np.PrintBDF(datapath, names, NasPrinter::onefile);
	return 0;
}

int find_process(string process_name)
{
	int count=0;//进程计数 
	PROCESSENTRY32 pe32;

	pe32.dwSize=sizeof(PROCESSENTRY32);
	HANDLE process_snapshot_handle=CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);//创建进程快照句柄

	if(process_snapshot_handle==INVALID_HANDLE_VALUE) return -1;//创建句柄失败

	bool is_exist=Process32First(process_snapshot_handle,&pe32);//找第一个
	while(is_exist)
	{
		if(!stricmp(process_name.c_str(),_bstr_t(pe32.szExeFile))) count++;//进程名不区分大小写
		is_exist=Process32Next(process_snapshot_handle,&pe32);//找下一个
	}
	return count;
}

int myNastran::NastranCalc()
{
	cout << "Nastran正在调用。。。。" << endl;
	string command = NastranPath + " " + BDFPath + " out=" +WorkPath;

	remove(logPath.c_str());//如果之前有同名log文件，则先删除原先的log文件
	remove(xdbPath.c_str());//如果之前有同名xbd文件，则先删除原先的log文件
	remove(pchPath.c_str());//如果之前有同名pch文件，则先删除原先的log文件
	system(command.c_str());

	
	
	//判断log文件是否生成成功
	clock_t brginTime = clock();
	while(true)
	{
		ifstream ifs(logPath);
		if (!ifs.fail())
		{
			//cout << logPath << " 文件已成功生成" << endl;
			cout << " | Nastran正在运行中。。。。" << endl;;
			break;
		}
		//else if (clock() - brginTime > 20000)
		//{
		//	cout << " | 未能正确运行Nastran！请检查 "<< BDFPath <<" 文件是否存在*******" << endl;
		//	//system("pause");
		//	return -1;
		//}
	}
	//判断nastran.exe是否运行结束
	while(true)
	{
		int count=0;
		string process_name="nastran.exe";

		count=find_process(process_name);
		if (count == 0)
		{
			cout << " | nastran.exe 运行结束" << endl;
			break;
		}
	}
	//读取log文件判断分析是否正常
	bool isFinishCalc = false;
	clock_t brginTime1 = clock();
	while(true)
	{
		ifstream ifs(logPath);
		string str_line;
		while (getline(ifs, str_line))
		{
			string::size_type idx = str_line.find("Analysis complete");
			if (idx != string::npos)//发现计算完成的标识符----跳出循环，准备提取数据
			{
				isFinishCalc = true;
				
				ifs.close();
				break;
			}
			
		}
		if (isFinishCalc)
		{
			//system("C:\\Users\\yycab\\Desktop\\bdf\\nastranTESTfile\\KillNastran.bat");//dos()杀掉nastran进程 懒得写 后面再加吧
			break;
		}
		else if (clock() - brginTime1 > 8000)
		{
			cout << " | 超过8s还没读到log分析成功，Nastran分析有误*******" << endl;
			//system("pause");
			return -1;
		}
	}
	//判断pch文件是否生成成功
	clock_t brginTime2 = clock();
	while(true)
	{
		ifstream ifs(pchPath);
		if (!ifs.fail())
		{
			cout << " | "<<pchPath << " 文件已成功生成" << endl;
			break;
		}
		else if (clock() - brginTime2 > 8000)
		{
			cout << " | 超过8s还未能正确生成 '.pch'！请检查 "<< pchPath <<" 文件是否存在*******" << endl;
			//system("pause");
			return -1;
		}
	}
	//Sleep(2000);//系统暂停两秒
	cout << "Nastran分析成功" << endl;
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
		cout << "Nastran计算失败！.pch 文件不存在！请检查输入参数" << endl;
		return -1;
	}
	else cout << "数据结果文件打开成功！ 正在分析数据。。" << endl;
	
	ElemType elemType = ElemType::undefine;
	LoadType loadtype = LoadType::undefine;//1单元应力 2单元应变
	psList.clear();

	string str_line;
	while (getline(ifs, str_line))
	{
		if (str_line.find("SUBTITLE") != string::npos)//发现标识符----准备提取数据
		{
			getline(ifs, str_line);//空读1行
			getline(ifs, str_line);

			if (str_line.find("STRAINS") != string::npos)			loadtype = LoadType::Strain;//cout << "找到了应变数据" << endl;
			else if (str_line.find("STRESSES") != string::npos)		loadtype = LoadType::Stress;//cout << "找到了应力数据" << endl;
			
			getline(ifs, str_line);
			getline(ifs, str_line);//空读两行
			getline(ifs, str_line);

			if (str_line.find("QUAD4") != string::npos)			elemType = ElemType::QUAD4;//cout << "找到了四边形单元" << endl;
			else if (str_line.find("TRIA3") != string::npos)	elemType = ElemType::TRIA3;//cout << "找到了三角形单元" << endl;
			else if (str_line.find("BAR") != string::npos)		elemType = ElemType::BAR;//cout << "找到了一维梁单元" << endl;
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
				maxstrain.resize(CQUAD4id.size(), 0);//存储最大应变值
				maxstrainID.resize(CQUAD4id.size(), 0);//存储最大应变单元编号
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
				elemType = ElemType::undefine;//单元读取完毕，清空类型编号以免出错
				loadtype = LoadType::undefine;//单元读取完毕，清空类型编号以免出错
			}	break;
			case ElemType::TRIA3:			break;
			case ElemType::BAR: {
				maxstrain2.resize(CBARid.size(), 0);//存储最大应变值
				maxstrainID2.resize(CBARid.size(), 0);//存储最大应变单元编号
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
					elemType = ElemType::undefine;//单元读取完毕，清空类型编号以免出错
					loadtype = LoadType::undefine;//单元读取完毕，清空类型编号以免出错
				}
			}	break;
			}
		}	break;
		case myNastran::LoadType::Stress: {
			switch (elemType){
			case ElemType::QUAD4: {
				maxstress.resize(CQUAD4id.size(), 0);//存储最大应力值
				maxstressID.resize(CQUAD4id.size(), 0);//存储最大应力单元编号
				int cabinID = 0;
				getline(ifs, str_line);
				while (str_line.find("$") == string::npos)//读到一直出现TITLE再停止
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
				elemType = ElemType::undefine;//单元读取完毕，清空类型编号以免出错
				loadtype = LoadType::undefine;//单元读取完毕，清空类型编号以免出错
			}	break;
			case ElemType::TRIA3:			break;
			case ElemType::BAR: {
				maxstress2.resize(CBARid.size(), 0);//存储最大应力值
				maxstressID2.resize(CBARid.size(), 0);//存储最大应力单元编号
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
					elemType = ElemType::undefine;//单元读取完毕，清空类型编号以免出错
					loadtype = LoadType::undefine;//单元读取完毕，清空类型编号以免出错
				}
			}	break;
			}
		}	break;
		default:
			break;
		}
	}
	cout << "分析完成！数据已保存到指定文件夹" << endl;
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
	//输出每个舱段的壳体质量
	//int cbNum = (ExistCabinNum > 0 && ExistCabinNum < CabinList.size()) ? ExistCabinNum : CabinList.size();

	for(int i = 0; i < CabinList.size(); i++)
	{
		mass += CabinList[i].Area * CabinList[i].T * Material8_list[CabinList[i].MID-1].RHO;
	}
	for (int i = 0; i < Instrument.size(); i++)
	{
		//发动机
		if (Instrument[i].IsOnCabin==true && Instrument[i].cabinID >= 0)
		{
			mass += Instrument[i].GetMass();
		}
		//部件
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
		cout << "梁截面属性编号已超出边界！" << endl;
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
		cout << "材料属性MAT1编号已超出边界！" << endl;
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
		cout << "材料属性MAT8编号已超出边界！" << endl;
		return MAT8();
	}
}

//Xid.clear(); EquMass.clear();}



