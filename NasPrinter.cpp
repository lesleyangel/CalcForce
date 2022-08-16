#include "NasPrinter.h"
#include <direct.h>
#include <iomanip>

using namespace std;
#define C_INT const int
#define C_DOUBEL const double
#define C_STR const std::string&
#define C_VECTOR(type) const std::vector<type>&
NasPrinter::NasPrinter() 
{ 
	ssHeader.clear();	ssHeader.str("");
	ssForce.clear();	ssForce.str("");
	ssForce.clear();	ssMesh.str("");
	ssProperty.clear();	ssProperty.str("");
}
int NasPrinter::PrintBDF(const std::string& path, const std::string & name, OutputStyle type)
{	
	int state = _mkdir(path.c_str());

	switch (type)
	{
	case OutputStyle::onefile:{
		ofstream ofs;
		ofs.open(path + name + ".bdf", ios::trunc);
		ofs << ssHeader.str();
		ofs << ssMesh.str();
		ofs << ssForce.str();
		ofs << ssProperty.str();
		ofs << "ENDDATA" << endl;
		ofs.close();
		return 0;
	}	break;
	case OutputStyle::multifile: {
		ofstream ofsHeader,ofsForce,ofsMesh,ofsProperty;
		//
		ofsHeader.open(path + name + ".bdf", ios::trunc);
		ofsHeader << ssHeader.str();
		ofsHeader << "include '" << (name + "_mesh.bdf'") << endl;//ͷ�ļ��ڲ�ʹ�����·��
		ofsHeader << "include '" << (name + "_property.bdf'") << endl;//ͷ�ļ��ڲ�ʹ�����·��
		ofsHeader << "include '" << (name + "_force.bdf'") << endl;//ͷ�ļ��ڲ�ʹ�����·��
		ofsHeader << "ENDDATA" << endl;
		ofsHeader.close();
		//
		ofsForce.open(path + name + "_force.bdf", ios::trunc);
		ofsForce << ssForce.str();
		ofsForce.close();
		//
		ofsMesh.open(path + name + "_mesh.bdf", ios::trunc);
		ofsMesh << ssMesh.str();
		ofsMesh.close();
		//
		ofsProperty.open(path + name + "_property.bdf", ios::trunc);
		ofsProperty << ssProperty.str();
		ofsProperty.close();
		return 0;
	}	break;
	default:
		return -1;
		break;
	}
	
	return state;
}

void NasPrinter::AddCardShort(std::ofstream& ofs, const std::vector<std::string>& words)
{
	int listNum = 0;
	ofs << std::left;
	ofs << setw(8) << words[0].substr(0, 8);//��һ���ؼ���
	ofs << std::right;
	for (size_t i = 1; i < words.size() - 1; i++)
	{
		listNum++;
		ofs << setw(8) << words[i].substr(0, 8);
		if (listNum % 8 == 0)	ofs << "\n        ";
	}
	ofs << setw(8) << words[words.size() - 1];

	ofs << endl;
}

void NasPrinter::AddCardLong(std::ofstream& ofs, const std::vector<std::string>& words)
{
	int listNum = 0;
	ofs << std::left;

	ofs << setw(8) << words[0].substr(0,8) + "*";//��һ���ؼ���
	ofs << std::right;
	for (size_t i = 1; i < words.size() - 1; i++)
	{
		listNum++;
		ofs << setw(16) << words[i].substr(0, 16);
		if (listNum % 4 == 0)	ofs << "*\n*       ";
	}
	ofs << setw(16) << words[words.size() - 1];

	ofs << endl;
}

void NasPrinter::AddCardShort(std::stringstream& ofs, const std::vector<std::string>& words)
{
	//ofs.str("");//��ջ�����
	int listNum = 0;
	ofs << std::left;
	ofs << setw(8) << words[0].substr(0, 8);;//��һ���ؼ���
	ofs << std::right;
	for (size_t i = 1; i < words.size()-1; i++)
	{
		listNum++;
		ofs << setw(8) << words[i].substr(0, 8);;
		if (listNum % 8 == 0)	ofs << "\n        ";
	}
	ofs << setw(8) << words[words.size() - 1];

	ofs << endl;
}

void NasPrinter::AddCardLong(std::stringstream& ofs, const std::vector<std::string>& words)
{
	//ofs.str("");//��ջ�����
	int listNum = 0;
	ofs << std::left;

	ofs << setw(8) << words[0].substr(0, 8) + "*";//��һ���ؼ���
	ofs << std::right;
	for (size_t i = 1; i < words.size() - 1; i++)
	{
		listNum++;
		ofs << setw(16) << words[i].substr(0, 16);
		if (listNum % 4 == 0)	ofs << "*\n*       ";
	}
	ofs << setw(16) << words[words.size() - 1];

	ofs << endl;
}

string NasPrinter::double2str(const double& db, int size)
{

	int sss = (int)db;
	stringstream ss;
	const double fabsdb = abs(db);
	int sign = 1;
	if (db < 0)
	{
		size--;
		sign = -1;
	}
	//����
	int xx = (int)fabsdb;
	if ((fabsdb - (int)fabsdb) < 0.4 && abs((int)fabsdb - fabsdb) < 1e-5)//����
	{
		return to_string((int)db) + ".";
	}
	if ((fabsdb - (int)fabsdb >= 0.4 && abs((int)fabsdb - fabsdb + 1) < 1e-5))//����
	{
		return to_string((int)db + sign) + ".";
	}
	//������
	if (fabsdb > std::pow(10.0, size))
	{
		ss << setprecision(size - 6) << db;
	}
	else if(fabsdb >= 1)
	{
		ss << setprecision(size - 1) << db;
	}
	else if (fabsdb > 1e-1)
	{
		ss << setprecision(size - 2) << db;
	}
	else if (fabsdb > 1e-2)
	{
		ss << setprecision(size - 3) << db;
	}
	else if (fabsdb > 1e-3)
	{
		ss << setprecision(size - 4) << db;
	}
	else if (fabsdb > 1e-4)
	{
		ss << setprecision(size - 5) << db;
	}
	else
	{
		ss << setprecision(size - 6) << db;
	}
	//auto aaaa = (int)stof(ss.str());
	//double aaaa1 = stod(ss.str());
	if (abs((int)stof(ss.str()) - stod(ss.str())) < 1e-5) //�ж��Ƿ�Ϊ����
	{
		return ss.str() + ".";
	}
	else
	{
		return ss.str();
	}
	
}

void NasPrinter::addGRID(C_INT CID, C_INT CP, const Point & pt)
{
	if (abs(pt.getZ()+3)<1e-5)
	{
		string sss = double2str(pt.getZ(), 16);
		int a = 1;
	}
	
	AddCardLong(ssMesh, {
		"GRID",						//��Ƭ��
		int2str(CID),				//�ڵ��� CID
		int2str(CP),				//����ϵ���
		double2str(pt.getX(),16),	//x����
		double2str(pt.getY(),16),	//y����
		double2str(pt.getZ(),16),	//z����
	});
}

void NasPrinter::addCMASS2(C_INT EID, C_DOUBEL M, C_INT G1, C_INT C1)
{
	AddCardShort(ssMesh, {
		"CMASS2",					//��Ƭ��
		int2str(EID),				//��Ԫ��� EID
		double2str(M, 8),			//��Ԫ������M
		int2str(G1),				//�ڵ�� G1
		int2str(C1),				//������ C1
	});
}

void NasPrinter::addCBAR(C_INT EID, C_INT PID, C_INT GA, C_INT GB, C_DOUBEL X1, C_DOUBEL X2, C_DOUBEL X3)
{
	AddCardShort(ssMesh, {
		"CBAR",							//��Ƭ��
		int2str(EID),					//��Ԫ��� EID
		int2str(PID),					//��Ԫ���Ա�� PID
		int2str(GA),					//�ڵ�1
		int2str(GB),					//�ڵ�2
		double2str(X1, 8),				//һά��Ԫ����ָ��
		double2str(X2, 8),				//һά��Ԫ����ָ��
		double2str(X3, 8),				//һά��Ԫ����ָ��
	});
}

void NasPrinter::addCTRIA3(C_INT EID, C_INT PID, C_INT G1, C_INT G2, C_INT G3)
{
	AddCardShort(ssMesh, {
		"CTRIA3",		//��Ƭ��
		int2str(EID),	//��Ԫ��� EID
		int2str(PID),	//��Ԫ���Ա�� PID
		int2str(G1),	//�ڵ�1 CID
		int2str(G2),	//�ڵ�2 CID
		int2str(G3),	//�ڵ�3 CID
	});
}

void NasPrinter::addCQUAD4(C_INT EID, C_INT PID, C_INT G1, C_INT G2, C_INT G3, C_INT G4)
{
	AddCardShort(ssMesh, {
		"CQUAD4",		//��Ƭ��
		int2str(EID),	//��Ԫ��� EID
		int2str(PID),	//��Ԫ���Ա�� PID
		int2str(G1),	//�ڵ�1 CID
		int2str(G2),	//�ڵ�2 CID
		int2str(G3),	//�ڵ�3 CID
		int2str(G4),	//�ڵ�4 CID
	});
}

void NasPrinter::addRBE2(C_INT EID, C_INT GN,C_INT CM, C_VECTOR(int) GMi)
{
	vector<string> strList = {
		"RBE2",			//��Ƭ��
		int2str(EID),	//���ݿ����
		int2str(GN),	//�������ɶȽڵ�ı�ţ��ýڵ���������ɶ��Ƕ������ɶȣ�
		int2str(CM),	//�ӽڵ�Ƕ������ɶȷ���
	};
	strList.reserve(strList.size() + GMi.size());
	for (size_t i = 0; i < GMi.size(); i++)
	{
		strList.push_back(int2str(GMi[i]));//����ڵ�
	}
	AddCardShort(ssMesh, strList);
}

void NasPrinter::addRBE3(C_INT EID, C_INT REDGRID, C_INT REFC, C_DOUBEL WT1, C_INT C1, C_VECTOR(int) G1j)
{
	vector<string> strList = {
		"RBE3",				//��Ƭ��
		int2str(EID),		//���ݿ����
		"",					//
		int2str(REDGRID),	//�ο��ڵ���
		int2str(REFC),		//�ο��ڵ����ɶȷ�����
		double2str(WT1, 8),	//�ڵ�G1j��λ�����ļ�Ȩֵ������ϵ����
		int2str(C1),		//�ڵ�G1j��λ�Ʒ�����
	};
	strList.reserve(strList.size() + G1j.size());
	for (size_t j = 0; j < G1j.size(); j++)
	{
		strList.push_back(int2str(G1j[j]));//����ڵ�
	}
	AddCardShort(ssMesh, strList);
}

void NasPrinter::addPLOAD(C_INT SID, C_DOUBEL P, C_INT G1, C_INT G2, C_INT G3, C_INT G4)
{
	AddCardShort(ssForce, {
		"PLOAD",			//��Ƭ��
		int2str(SID),		//�غɱ�� SID
		double2str(P, 8),	//ѹ��ֵ
		int2str(G1),		//�ڵ�1
		int2str(G2),		//�ڵ�2
		int2str(G3),		//�ڵ�3
		positiveInt2str(G4),//�ڵ�4 ȡĬ��ֵ-1ʱĬ�����Ϊ��
	});
}

void NasPrinter::addFORCE(C_INT SID, C_INT G, C_INT CID, C_DOUBEL F, C_DOUBEL N1, C_DOUBEL N2, C_DOUBEL N3)
{
	AddCardLong(ssForce, {
		"FORCE",				//��Ƭ��
		int2str(SID),			//SID
		int2str(G),				//�ڵ���
		int2str(CID),			//�ֲ�����ϵ��� Ĭ�Ͽɲ���=0��ȫ������ϵ��
		double2str(F, 16),		//�������ķ�ֵ
		double2str(N1,16),		//������������ϵ�е�ʸ������
		double2str(N2,16),		//������������ϵ�е�ʸ������
		double2str(N3,16),		//������������ϵ�е�ʸ������
	});
}

void NasPrinter::addFORCE(C_INT SID, C_INT G, C_INT CID, C_DOUBEL F, const Point & pt)
{
	addFORCE(SID, G, CID, F, pt.getX(), pt.getY(), pt.getZ());
}

void NasPrinter::addGRAV(C_INT SID, C_INT CID, C_DOUBEL A, C_DOUBEL N1, C_DOUBEL N2, C_DOUBEL N3)
{
	AddCardLong(ssForce, {
		"GRAV",					//��Ƭ��
		int2str(SID),			//�غɱ�� SID
		int2str(CID),			//����ϵ���
		double2str(A, 8),		//���ٶȷ�ֵ
		double2str(N1, 8),		//���ٶȷ�������1
		double2str(N2, 8),		//���ٶȷ�������2
		double2str(N3, 8),		//���ٶȷ�������3
	});
}

void NasPrinter::addLOAD(C_INT SID, C_DOUBEL S, C_VECTOR(DoubleIntPair) LOADi)
{
	vector<string> strList = {
		"LOAD",				//��Ƭ��
		int2str(SID),		//���ݿ����
		double2str(S, 8),	//��ϵ��
	};
	strList.reserve(strList.size() + LOADi.size());
	for (size_t i = 0; i < LOADi.size(); i++)
	{
		strList.push_back(double2str(LOADi[i].first, 8));	//�����غɵ�ϵ��
		strList.push_back(int2str(LOADi[i].second));			//�����غ����ݿ����
	}
	AddCardShort(ssForce, strList);
}

void NasPrinter::addPSHELL(const PSHELL & ps)
{
	ssProperty << "$-- Property " << ps.name << " --$" << endl;
	AddCardShort(ssProperty, {
		"PSHELL",			//��Ƭ��
		int2str(ps.PID),	//�������Ա�� PID
		int2str(ps.MID),
		double2str(ps.T, 8),//���
	});
}

void NasPrinter::addPBARL(const PBARL & pb)
{
	ssProperty << "$-- Property " << pb.name << " --$" << endl;
	vector<string> strList = {
		"PBARL",			//��Ƭ��
		int2str(pb.PID),	//�������Ա�� PID
		int2str(pb.MID),
		"",
		pb.TYPE,
		"","","",""	
	};
	//const size_t orginSize = strList.size();
	strList.reserve(strList.size() + pb.DIM.size());
	for (size_t i = 0; i < pb.DIM.size(); i++)
	{
		strList.push_back(double2str(pb.DIM[i], 8));	//�����غɵ�ϵ��
	}
	AddCardShort(ssProperty, strList);
}

void NasPrinter::addMAT8(const MAT8 & mt)
{
	ssProperty << "$-- Material " << mt.name << " --$" << endl;
	AddCardShort(ssProperty, {
		"MAT8",
		int2str(mt.MID),
		double2str(mt.E1, 8),
		double2str(mt.E2, 8),
		double2str(mt.NU12, 8),
		double2str(mt.G12, 8),
		double2str(mt.G1Z, 8),
		double2str(mt.G2Z, 8),
		double2str(mt.RHO, 8),
	});
}

void NasPrinter::addMAT1(const MAT1 & mt)
{
	ssProperty << "$-- Material " << mt.name << " --$" << endl;
	AddCardShort(ssProperty, {
		"MAT1",
		int2str(mt.MID),
		double2str(mt.E, 8),
		"",
		double2str(mt.Nu, 8),
		double2str(mt.RHO, 8),
	});
}

void NasPrinter::addEIGRL(C_INT SID, C_INT ND, C_INT MSGLVL)
{
	//Lanczos����ȡģ̬
	AddCardShort(ssHeader, {
		"EIGRL",
		int2str(SID),		//����ţ�����>0��
		"","",
		int2str(ND),		//Number of Desired Roots��������������>0,��ո�
		int2str(MSGLVL),	//Diagnostic Output Level�Ի�����Ϣ������0����Ϣ��1ÿ��ƽ�ơ���ӡ��õ�����ֵ��>1���ӶԻ������
	});
}

void NasPrinter::addDOPTPRM(C_VECTOR(std::string) command)
{
	vector<string> strList = {
		"DOPTPRM",
	};
	strList.reserve(strList.size() + command.size());
	for (size_t i = 0; i < command.size(); i++)
	{
		strList.push_back(command[i]);
	}
	AddCardShort(ssHeader, strList);
}

void NasPrinter::addDESVAR(C_INT ID, C_STR LABEL, C_DOUBEL XINIT, C_DOUBEL XLB, C_DOUBEL XUB, C_DOUBEL DELXV)
{
	//���������ṩһ����ֵ���ļ���
	//������Ʊ����Ŀ�Ƭ
	AddCardLong(ssHeader, {
		"DESVAR",
		int2str(ID),			//��Ʊ�����ID�ţ�int>0��
		LABEL,					//�û�ָ������Ʊ�������
		double2str(XINIT,16),	//��Ʊ����ĳ�ʼֵ��double XLB<=XINIT<=XUB��
		double2str(XLB, 16),		//��Ʊ��������ޣ�double nasĬ��ֵ-1.0e20��
		double2str(XUB, 16),		//��Ʊ��������ޣ�double nasĬ��ֵ+1.0e20��
		double2str(DELXV,16),	//��Ʊ����ĸñ仯�����ʼֵ�ı�ֵ�����ֵ��double>0��
	});
}

void NasPrinter::addDVPREL1(C_INT ID, C_STR TYPE, C_INT PID, C_STR PNAME, C_VECTOR(DoubleIntPair) DVID_COEFi, C_DOUBEL C0, C_DOUBEL PMIN, C_DOUBEL PMAX)
{
	vector<string> strList = {
		"DVPREL1",
		int2str(ID),			//����Ʊ������ӵ�ID�ţ�int>0��
		TYPE,					//�������͵����֣��硰PBAR������PBEAM���ȣ�string��
		int2str(PID),			//���Ե�ID�ţ�int>0��
		PNAME,					//�������֣��硰T������A���������Կ�Ƭ����Field����δ֪���ߵ�Ԫ���Ա����ַ���Word����λ�ã�sring��int>0��
		double2str(PMIN, 8),	//�������������Сֵ��double��nasĬ��ֵ-1.0e20��
		double2str(PMAX, 8),	//��������������ֵ��double��nasĬ��ֵ+1.0e20��
		double2str(C0, 8),		//���Թ�ϵ�ĳ����double��nasĬ��ֵΪ0.0��
		"",
	};
	strList.reserve(strList.size() + DVID_COEFi.size());
	for (size_t i = 0; i < DVID_COEFi.size(); i++)
	{
		strList.push_back(int2str(DVID_COEFi[i].second));//DVIDi DESVAR��Ƭ��ID�ţ�int>0��
		strList.push_back(double2str(DVID_COEFi[i].first, 8));//COEFi ���Թ�ϵ��һ����ϵ����double��
	}
	AddCardShort(ssHeader, strList);
}

void NasPrinter::addDRESP1(C_INT ID, C_STR LABLE, C_STR RTYPE, C_STR PTYPE, C_INT REGION, C_INT ATTA, C_INT ATTB, C_VECTOR(int) ATTi)
{
	vector<string> strList = {
		"DRESP1",
		int2str(ID),
		LABLE,
		RTYPE,
		PTYPE,
		positiveInt2str(REGION),
		positiveInt2str(ATTA),
		positiveInt2str(ATTB),
	};
	strList.reserve(strList.size() + ATTi.size());
	for (size_t i = 0; i < ATTi.size(); i++)
	{
		strList.push_back(int2str(ATTi[i]));
	}
	AddCardShort(ssHeader, strList);
}

void NasPrinter::addDRESP1_WEIGHT(C_INT ID, C_STR LABEL)
{
	addDRESP1(ID, LABEL, "WEIGHT", "", -1, -1, -1, vector<int>(0));
}

void NasPrinter::addDRESP1_FREQ(C_INT ID, C_STR LABEL, C_INT ATTA)
{
	addDRESP1(ID, LABEL, "FREQ","", -1, ATTA, -1, vector<int>(0));
}

void NasPrinter::addDRESP1_STRESS(C_INT ID, C_STR LABEL, C_STR PTYPE, C_INT ATTA, C_VECTOR(int) ATTi)
{
	addDRESP1(ID, LABEL, "STRESS", PTYPE, -1, ATTA, -1, ATTi);
}

void NasPrinter::addDCONSTR(C_INT DCID, C_INT RID, C_DOUBEL LALLOW, C_DOUBEL UALLOW)
{
	//����Լ��
	AddCardLong(ssHeader, {
		"DCONSTR",
		int2str(DCID),			//���Լ����ID�ţ�int>0��
		int2str(RID),			//�����Ӧ��DRESPi��Ƭ�������ID�ţ�int>0��
		double2str(LALLOW, 16),	//��Ӧ�������ޣ�double nasĬ��ֵ+1.0e20��
		double2str(UALLOW, 16),	//��Ӧ�������ޣ�double nasĬ��ֵ+1.0e20��
	});
}

