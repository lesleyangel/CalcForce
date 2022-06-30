#pragma once
#include "ShapeInfo.h"
#include "NasPrinter.h"
class InitInfo
{
public:
	
	vector<Point> ShapeFuncList;//外形参数
	vector<MassPointInitInfo> mpinit;//设备质量参数
	vector<CabinInitInfo> cbinit;//舱段参数
	vector<int> beamPID;//横梁定义
	int faiNum = 100;//
	int Modal_order = 10;
	ConcentratedForce EngineForce;
	CP CPinfo;
	vector<PBARL> property_list;
	vector<MAT1> meterial_list;
	vector<MAT8> meterial8_list;
};

//网格生成求解器
class ShapeMash
{
public:
	enum class MassDistribution{
		UseRBE3,	//通过集中质量和多点约束进行连接
		Distribute, //通过质量分配进行连接
	};
	ShapeMash();
	void SetInitInfo(const InitInfo& info);

	void CalcModel();
	virtual int ReadFromFile(const string& filepath) = 0;
	void SetShapeFunc(vector<Point> SF);//设置弹身外形参数
	void SetMeshSkin(int fainum, vector<double> xsite = vector<double>(0));//设置网格参数，周向网格数和轴向节点坐标
	void SetMeshSkin(int fainum, bool ifUseCabin = true,int xnum = 0);
	void SetStruct(vector<int> beamPID, vector<double> bulkhead = vector<double>(0),vector<int>bulkheadPID = vector<int>(0));//设置梁和隔框的属性
	void SetMassPoint(vector<MassPoint> ins,MassDistribution MassDistriType){this->Instrument = ins;this->MassDistriType = MassDistriType;}//设置集中质量
	void SetCabin(const vector<CabinInfo>& cabinList);//设置舱段划分
	void SetBeamProperty(vector<PBARL> property_list, vector<MAT1> Material_list, vector<MAT8> Material8_list = vector<MAT8>(0));//设置材料属性
	int updateCabinListT(const vector<PSHELL>&ps);
	void SetForce(CP aeroinfo,ConcentratedForce ef){this->aeroinfo = aeroinfo;this->EnginF=ef;}//1气动力，2发动机推力

	int PrintMesh(string fileName,int SOL);//打印结构的有限元模型 返回值：bat文件路径
	void PrintPress(string fileNmae);
	//void RunNastran();
	//vector<vector<Point>> GetPoint(){return point;}
	vector<CabinInfo> GetCabinList(){return CabinList;}
	const Mesh& GetMesh(){return meshSkin;}
	double GetStructMass();//输出结构质量
	double GetCabinStructMass(int _n);//输出某一舱段结构质量
	double GetWorkConditionMass(int _n);//输出某一工况结构质量

protected:
private:
	void CalcPoint();		//计算模型节点
	void CalcMesh();		//计算模型网格
	int CalcMassPoint();	//计算质量点信息，将集中质量添加到模型
	void CalcCabin();		
	CP aeroinfo;				//气动力
	ConcentratedForce EnginF;	//发动机推力
	myNastran myNast;			//nastran求解器
	vector<Point> ShapeFunc;	//外形坐标(x坐标必须单调增加)
	vector<MassPoint> Instrument;//箭上设备仪器
	MassDistribution MassDistriType;			//0原始方法分布质量;1多点连接的方式分布质量
	vector<CabinInfo> CabinList;//舱段划分
	//double ExistCabinNum;		//记录存在的舱段数
	map<int,double> masspoint;
	int FaiNum;					//旋转一周的点数
	vector<double> Xsite;		//x坐标(x坐标必须单调增加)

	int beamNum;				//横梁数量
	vector<int> beamPID;		//横梁的截面属性编号
	vector<double> bulkhead;	//隔框位置（size代表数量）
	vector<int> bulkheadID;		//隔框在点集合中的x轴位置坐标ID
	vector<int> bulkheadPID;	//每个隔框的截面属性编号
	//vector<int> bulkheadElemID;	//每个隔框的单元编号

	vector<vector<Field<double>>> point;//点第一个坐标是沿着x轴，第二个坐标是沿着周向
	Mesh meshSkin;				//有限元节点坐标和单元编号(1表面单元；2隔框单元；3横梁单元)
	//PBARL bk_sec;	//隔框界面属性
	//PBARL bm_sec;	//横梁截面属性
	//MAT1 bk_mat;	//隔框材料属性
	//MAT1 bm_mat;	//横梁材料属性
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