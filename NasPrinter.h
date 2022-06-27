#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <math.h>
#include "material.h"



//nastran�����
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
	int NastranCalc(); //����nastran��������Ԫģ��
	int ReadResPCH200();
	int ReadResPCH(const vector<int>& CQUAD4id, const vector<int>&CBARid);	//��ȡ����Ԫģ���е�pch�ļ�
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
	vector<double> maxstress;//�洢���Ӧ��ֵ
	vector<int> maxstressID;//�洢���Ӧ����Ԫ���
	vector<double> maxstrain;//�洢���Ӧ��ֵ
	vector<int> maxstrainID;//�洢���Ӧ�䵥Ԫ���

	vector<double> maxstress2;//�洢���Ӧ��ֵ
	vector<int> maxstressID2;//�洢���Ӧ����Ԫ���
	vector<double> maxstrain2;//�洢���Ӧ��ֵ
	vector<int> maxstrainID2;//�洢���Ӧ�䵥Ԫ���
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
	{//�����ļ���style
		onefile,   //���ļ���дbdf
		multifile, //���ļ���дbdf
	};
	NasPrinter();
	int PrintBDF(C_STR path, C_STR name, OutputStyle type = OutputStyle::onefile);

	void AddCardShort(std::ofstream& ofs, C_VECTOR<std::string>& words);
	void AddCardLong(std::ofstream& ofs, C_VECTOR<std::string>& words);
	void AddCardShort(std::stringstream& ss, C_VECTOR<std::string>& words);//��ֱ��ofstream��
	void AddCardLong(std::stringstream& ss, C_VECTOR<std::string>& words);//��ֱ��ofstream��
	static std::string double2str(C_DOUBEL db, C_INT size);
	static std::string int2str(C_INT n) { return std::to_string(n); }
	
	std::stringstream ssHeader;
	std::stringstream ssForce;
	std::stringstream ssMesh;
	std::stringstream ssProperty;
	//����ģ��
	void addGRID(C_INT CID, C_INT CP, const Point& pt);//��ӽڵ㿨Ƭ
	void addCMASS2(C_INT EID, C_DOUBEL M, C_INT G1, C_INT C1);//��Ӽ��������� ��ά��Ԫ
	void addCBAR(C_INT EID, C_INT PID, C_INT GA, C_INT GB, C_DOUBEL X1, C_DOUBEL X2, C_DOUBEL X3);//��Ӹ˵�Ԫ һά��Ԫ
	void addCTRIA3(C_INT EID, C_INT PID, C_INT G1, C_INT G2, C_INT G3);//��������ε�Ԫ ��ά��Ԫ
	void addCQUAD4(C_INT EID, C_INT PID, C_INT G1, C_INT G2, C_INT G3, C_INT G4);//����ı��ε�Ԫ ��ά��Ԫ
	void addRBE2(C_INT EID, C_INT GN,C_INT CM, C_VECTOR<int> GMi);
	void addRBE3(C_INT EID, C_INT REDGRID, C_INT REFC, C_DOUBEL WT1, C_INT C1, C_VECTOR<int> G1j);//R��Ԫ RBE3
	//Լ�����غ�
	void addPLOAD(C_INT SID, C_DOUBEL P, C_INT G1, C_INT G2, C_INT G3, C_INT G4 = -1);//���ѹ��
	void addFORCE(C_INT SID, C_INT G, C_INT CID, C_DOUBEL F, C_DOUBEL N1, C_DOUBEL N2, C_DOUBEL N3);//��ӽڵ㼯����
	void addFORCE(C_INT SID, C_INT G, C_INT CID, C_DOUBEL F, const Point& pt);//��ӽڵ㼯����
	void addGRAV(C_INT SID, C_INT CID, C_DOUBEL A, C_DOUBEL N1, C_DOUBEL N2, C_DOUBEL N3);//��ӹ����غ�
	void addLOAD(C_INT SID, C_DOUBEL S, C_VECTOR<DoubleIntPair> LOADi);//��Ӿ�̬����غ�
	//���ϼ�����
	void addPSHELL(const PSHELL& ps);
	void addPBARL(const PBARL& pb);
	void addMAT8(const MAT8& mt);
	void addMAT1(const MAT1& mt);
	//ģ̬����
	void addEIGRL(C_INT SID, C_INT ND, C_INT MSGLVL);//���ģ̬��ȡ��Ƭ

	//�Ż�����
	void addDOPTPRM(C_VECTOR<std::string>command);
	//��Ʊ�����Ƭ
	void addDESVAR(C_INT ID, C_STR LABEL, C_DOUBEL XINIT, C_DOUBEL XLB, C_DOUBEL XUB, C_DOUBEL DELXV);//�����Ʊ�����Ƭ
	void addDVPREL1(C_INT ID, C_STR TYPE, C_INT PID, C_STR PNAME, C_VECTOR<DoubleIntPair> DVID_COEFi, C_DOUBEL C0=0, C_DOUBEL PMIN=-1e20, C_DOUBEL PMAX=1e20);
	//��Ӧ��Ƭ
	void addDRESP1(C_INT ID, C_STR LABLE, C_STR RTYPE, C_STR PTYPE, C_INT REGION, C_INT ATTA, C_INT ATTB, C_VECTOR<int> ATTi); //��������Ӧ
	void addDRESP1_WEIGHT(C_INT ID, C_STR LABEL);//��������Ӧ ������Ӧ
	void addDRESP1_FREQ(C_INT ID, C_STR LABEL, C_INT ATTA);//��������Ӧ ����ģ̬��Ӧ ����ATTA=ģ̬��Ӧ����
	void addDRESP1_STRESS(C_INT ID, C_STR LABEL, C_STR PTYPE, C_INT ATTA, C_VECTOR<int> ATTi); //��������Ӧ Ӧ����Ӧ ����ATTA=Ӧ��Ӧ����룻ATTi=������Ԫ���
	void addDCONSTR(C_INT DCID, C_INT RID, C_DOUBEL LALLOW, C_DOUBEL UALLOW);//Լ����Ƭ
private:
	std::string m_path;
	std::string m_Nmae;
	std::string positiveInt2str(C_INT n) { return (n < 0) ? "" : std::to_string(n); }//�������Ϊ������Ĭ�������

};