#pragma once
#include "ShapeInfo.h"
#include "NasPrinter.h"
class InitInfo
{
public:
	
	vector<Point> ShapeFuncList;//���β���
	vector<MassPointInitInfo> mpinit;//�豸��������
	vector<CabinInitInfo> cbinit;//�նβ���
	vector<int> beamPID;//��������
	int faiNum = 100;//
	int Modal_order = 10;
	ConcentratedForce EngineForce;
	CP CPinfo;
	vector<PBARL> property_list;
	vector<MAT1> meterial_list;
	vector<MAT8> meterial8_list;
};

//�������������
class ShapeMash
{
public:
	enum class MassDistribution{
		UseRBE3,	//ͨ�����������Ͷ��Լ����������
		Distribute, //ͨ�����������������
	};
	ShapeMash();
	void SetInitInfo(const InitInfo& info);

	void CalcModel();
	virtual int ReadFromFile(const string& filepath) = 0;
	void SetShapeFunc(vector<Point> SF);//���õ������β���
	void SetMeshSkin(int fainum, vector<double> xsite = vector<double>(0));//�����������������������������ڵ�����
	void SetMeshSkin(int fainum, bool ifUseCabin = true,int xnum = 0);
	void SetStruct(vector<int> beamPID, vector<double> bulkhead = vector<double>(0),vector<int>bulkheadPID = vector<int>(0));//�������͸��������
	void SetMassPoint(vector<MassPoint> ins,MassDistribution MassDistriType){this->Instrument = ins;this->MassDistriType = MassDistriType;}//���ü�������
	void SetCabin(const vector<CabinInfo>& cabinList);//���òնλ���
	void SetBeamProperty(vector<PBARL> property_list, vector<MAT1> Material_list, vector<MAT8> Material8_list = vector<MAT8>(0));//���ò�������
	int updateCabinListT(const vector<PSHELL>&ps);
	void SetForce(CP aeroinfo,ConcentratedForce ef){this->aeroinfo = aeroinfo;this->EnginF=ef;}//1��������2����������

	int PrintMesh(string fileName,int SOL);//��ӡ�ṹ������Ԫģ�� ����ֵ��bat�ļ�·��
	void PrintPress(string fileNmae);
	//void RunNastran();
	//vector<vector<Point>> GetPoint(){return point;}
	vector<CabinInfo> GetCabinList(){return CabinList;}
	const Mesh& GetMesh(){return meshSkin;}
	double GetStructMass();//����ṹ����
	double GetCabinStructMass(int _n);//���ĳһ�նνṹ����
	double GetWorkConditionMass(int _n);//���ĳһ�����ṹ����

protected:
private:
	void CalcPoint();		//����ģ�ͽڵ�
	void CalcMesh();		//����ģ������
	int CalcMassPoint();	//������������Ϣ��������������ӵ�ģ��
	void CalcCabin();		
	CP aeroinfo;				//������
	ConcentratedForce EnginF;	//����������
	myNastran myNast;			//nastran�����
	vector<Point> ShapeFunc;	//��������(x������뵥������)
	vector<MassPoint> Instrument;//�����豸����
	MassDistribution MassDistriType;			//0ԭʼ�����ֲ�����;1������ӵķ�ʽ�ֲ�����
	vector<CabinInfo> CabinList;//�նλ���
	//double ExistCabinNum;		//��¼���ڵĲն���
	map<int,double> masspoint;
	int FaiNum;					//��תһ�ܵĵ���
	vector<double> Xsite;		//x����(x������뵥������)

	int beamNum;				//��������
	vector<int> beamPID;		//�����Ľ������Ա��
	vector<double> bulkhead;	//����λ�ã�size����������
	vector<int> bulkheadID;		//�����ڵ㼯���е�x��λ������ID
	vector<int> bulkheadPID;	//ÿ������Ľ������Ա��
	//vector<int> bulkheadElemID;	//ÿ������ĵ�Ԫ���

	vector<vector<Field<double>>> point;//���һ������������x�ᣬ�ڶ�����������������
	Mesh meshSkin;				//����Ԫ�ڵ�����͵�Ԫ���(1���浥Ԫ��2����Ԫ��3������Ԫ)
	//PBARL bk_sec;	//�����������
	//PBARL bm_sec;	//������������
	//MAT1 bk_mat;	//�����������
	//MAT1 bm_mat;	//������������
	PBARL GetPBARL(int id);
	MAT1 GetMAT1(int id);
	MAT8 GetMAT8(int id);
	vector<PBARL> property_list;
	vector<MAT1> Material_list;
	vector<MAT8> Material8_list;
	//string NastranBatPath;
	//SOL103
	int Modal_order = 10;
};