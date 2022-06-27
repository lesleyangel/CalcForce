#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <math.h>
#include "material.h"



//nastran求解器
class myNastran
{
public:
	enum class LoadType
	{
		undefine, Strain, Stress
	};
	enum class ElemType
	{
		undefine, QUAD4, TRIA3, BAR
	};

	string NastranPath{ "" };
	string BDFPath{ "" };
	string WorkPath{ "" };
	string KillNastranBatPath{ "" };
	void CalcFilePath();
	int NastranCalc(); //调用nastran计算有限元模型
	int ReadResPCH200();
	int ReadResPCH(const vector<int>& CQUAD4id, const vector<int>&CBARid);	//读取有限元模型中的pch文件
	const vector<double>& GetMaxStress() { return maxstress; }
	const vector<int>& GetMaxStressID() { return maxstressID; }
	const vector<double>& GetMaxStrain() { return maxstrain; }
	const vector<int>& GetMaxStrainID() { return maxstrainID; }
	const vector<double>& GetMaxStressBar() { return maxstress2; }
	const vector<int>& GetMaxStressIDBar() { return maxstressID2; }
	const vector<double>& GetMaxStrainBar() { return maxstrain2; }
	const vector<int>& GetMaxStrainIDBar() { return maxstrainID2; }
	const vector<PSHELL>& GetPSHELLlist() { return psList; }
private:
	string logPath{ "" };
	string xdbPath{ "" };
	string pchPath{ "" };
	vector<PSHELL> psList{ vector<PSHELL>(0) };
	vector<double> maxstress;//存储最大应力值
	vector<int> maxstressID;//存储最大应力单元编号
	vector<double> maxstrain;//存储最大应变值
	vector<int> maxstrainID;//存储最大应变单元编号

	vector<double> maxstress2;//存储最大应力值
	vector<int> maxstressID2;//存储最大应力单元编号
	vector<double> maxstrain2;//存储最大应变值
	vector<int> maxstrainID2;//存储最大应变单元编号
};


class NasPrinter
{
	using C_INT = const int;
	using C_DOUBEL = const double;
	using C_STR = const std::string &;
	template<typename T>
	using C_VECTOR = const std::vector<T> &;
	using DoubleIntPair = std::pair<double, int>;
public:
	enum OutputStyle
	{//生成文件的style
		onefile,   //单文件编写bdf
		multifile, //多文件编写bdf
	};
	NasPrinter();
	int PrintBDF(C_STR path, C_STR name, OutputStyle type = OutputStyle::onefile);

	void AddCardShort(std::ofstream& ofs, C_VECTOR<std::string>& words);
	void AddCardLong(std::ofstream& ofs, C_VECTOR<std::string>& words);
	void AddCardShort(std::stringstream& ss, C_VECTOR<std::string>& words);//比直接ofstream快
	void AddCardLong(std::stringstream& ss, C_VECTOR<std::string>& words);//比直接ofstream快
	static std::string double2str(C_DOUBEL db, C_INT size);
	static std::string int2str(C_INT n) { return std::to_string(n); }
	
	std::stringstream ssHeader;
	std::stringstream ssForce;
	std::stringstream ssMesh;
	std::stringstream ssProperty;
	//几何模型
	void addGRID(C_INT CID, C_INT CP, const Point& pt);//添加节点卡片
	void addCMASS2(C_INT EID, C_DOUBEL M, C_INT G1, C_INT C1);//添加集中质量点 零维单元
	void addCBAR(C_INT EID, C_INT PID, C_INT GA, C_INT GB, C_DOUBEL X1, C_DOUBEL X2, C_DOUBEL X3);//添加杆单元 一维单元
	void addCTRIA3(C_INT EID, C_INT PID, C_INT G1, C_INT G2, C_INT G3);//添加三角形单元 二维单元
	void addCQUAD4(C_INT EID, C_INT PID, C_INT G1, C_INT G2, C_INT G3, C_INT G4);//添加四边形单元 二维单元
	void addRBE2(C_INT EID, C_INT GN,C_INT CM, C_VECTOR<int> GMi);
	void addRBE3(C_INT EID, C_INT REDGRID, C_INT REFC, C_DOUBEL WT1, C_INT C1, C_VECTOR<int> G1j);//R单元 RBE3
	//约束和载荷
	void addPLOAD(C_INT SID, C_DOUBEL P, C_INT G1, C_INT G2, C_INT G3, C_INT G4 = -1);//添加压力
	void addFORCE(C_INT SID, C_INT G, C_INT CID, C_DOUBEL F, C_DOUBEL N1, C_DOUBEL N2, C_DOUBEL N3);//添加节点集中力
	void addFORCE(C_INT SID, C_INT G, C_INT CID, C_DOUBEL F, const Point& pt);//添加节点集中力
	void addGRAV(C_INT SID, C_INT CID, C_DOUBEL A, C_DOUBEL N1, C_DOUBEL N2, C_DOUBEL N3);//添加惯性载荷
	void addLOAD(C_INT SID, C_DOUBEL S, C_VECTOR<DoubleIntPair> LOADi);//添加静态组合载荷
	//材料及属性
	void addPSHELL(const PSHELL& ps);
	void addPBARL(const PBARL& pb);
	void addMAT8(const MAT8& mt);
	void addMAT1(const MAT1& mt);
	//模态分析
	void addEIGRL(C_INT SID, C_INT ND, C_INT MSGLVL);//添加模态提取卡片

	//优化分析
	void addDOPTPRM(C_VECTOR<std::string>command);
	//设计变量卡片
	void addDESVAR(C_INT ID, C_STR LABEL, C_DOUBEL XINIT, C_DOUBEL XLB, C_DOUBEL XUB, C_DOUBEL DELXV);//添加设计变量卡片
	void addDVPREL1(C_INT ID, C_STR TYPE, C_INT PID, C_STR PNAME, C_VECTOR<DoubleIntPair> DVID_COEFi, C_DOUBEL C0=0, C_DOUBEL PMIN=-1e20, C_DOUBEL PMAX=1e20);
	//响应卡片
	void addDRESP1(C_INT ID, C_STR LABLE, C_STR RTYPE, C_STR PTYPE, C_INT REGION, C_INT ATTA, C_INT ATTB, C_VECTOR<int> ATTi); //定义Ⅰ阶相应
	void addDRESP1_WEIGHT(C_INT ID, C_STR LABEL);//定义Ⅰ阶相应 重量响应
	void addDRESP1_FREQ(C_INT ID, C_STR LABEL, C_INT ATTA);//定义Ⅰ阶相应 正交模态响应 输入ATTA=模态响应阶数
	void addDRESP1_STRESS(C_INT ID, C_STR LABEL, C_STR PTYPE, C_INT ATTA, C_VECTOR<int> ATTi); //定义Ⅰ阶相应 应力响应 输入ATTA=应力应变代码；ATTi=包含单元编号
	void addDCONSTR(C_INT DCID, C_INT RID, C_DOUBEL LALLOW, C_DOUBEL UALLOW);//约束卡片
private:
	std::string m_path;
	std::string m_Nmae;
	std::string positiveInt2str(C_INT n) { return (n < 0) ? "" : std::to_string(n); }//如果输入为负数则默认输出空

};