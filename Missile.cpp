#include "Missile.h"
#include "LoadFile.h"
//int Minuteman3::ReadFromFile_old(string path)
//{
//	//----------------------------参数读取----------------------------
//	vector<string> strList;//文件路径名称等信息
//	cout << "输入文件：" << path << endl;
//	ifstream ifs(path);
//	if (ifs.fail())
//	{
//		cout << "打开失败，文件不存在！" << endl;
//		return 0;
//	}
//	else
//	{
//		cout << "打开成功！" << endl;
//	}
//	string str_line;
//	for (int i = 0; getline(ifs, str_line);i++)
//	{
//		stringstream ss(str_line);
//		string str_tmp;
//		getline(ss, str_tmp, '=');
//		getline(ss, str_tmp, '=');
//		strList.push_back(str_tmp);
//	}
//	ifs.close();
//
//	//----------------------------参数处理----------------------------
//	const int pathID = 0;		vector<string> path_list(5);
//	const int nowCasID = 15;	int nowCase;	int nowCabinNum;
//	const int nowEngID = 16;	int nowEngine;
//
//	const int massNum = 2, skipLineM = 4;//质量个数、每个质量文件行数
//	vector<int> massID(massNum);	massID[0] = 6;		vector<vector<double>> mass_list(massNum);
//	for (int i = 1; i < massNum;i++)	massID[i] = massID[i-1] + skipLineM;
//
//	const int EnginNum = 3, skipLineE = 11;//发动机个数、每个发动机行数
//	vector<int> EnginID(EnginNum);	EnginID[0] = 18;	vector<vector<double>> Engin_list(EnginNum);
//	for (int i = 1; i < EnginNum;i++)	EnginID[i] = EnginID[i-1] + skipLineE;
//
//	const int CabinNum = 7, skipLineC = 6;//舱段个数、每个舱段行数
//	vector<int> CabinID(CabinNum);	CabinID[0] = 51;	vector<vector<double>> Cabin_list(CabinNum);
//	for (int i = 1; i < CabinNum;i++)	CabinID[i] = CabinID[i-1] + skipLineC;
//
//	const int bhNum = 10, skipLineH = 10;//隔框个数、每个隔框行数
//	vector<int> bhID(bhNum);		bhID[0] = 94;		vector<vector<double>> bh_list(bhNum);
//	for (int i = 1; i < bhNum;i++)		bhID[i] = bhID[i-1] + skipLineH;
//	//---------------------------------------------------------------------
//	//文件路径参数列表编号
//	const int PNas = 0;	const int PBdf = 1;	const int PNam = 2;	const int POut = 3;	const int PAer = 4;
//	for (int i = 0; i < path_list.size(); i++)
//	{
//		path_list[i] = strList[pathID+i];
//	}
//	//设置当前舱段数量
//	nowCase = atoi(strList[nowCasID].c_str());
//	switch (nowCase)
//	{
//	case 1: nowCabinNum = 7; break;
//	case 2: nowCabinNum = 5; break;
//	case 3: nowCabinNum = 3; break;
//	case 4: nowCabinNum = 1; break;
//	default: nowCabinNum = Cabin_list.size();
//	}
//	nowEngine = atoi(strList[nowEngID].c_str());
//	//质量块参数列表编号
//	const int MM = 0;	const int MX = 1;	const int MY = 2;	const int MZ = 3;
//	for (int i = 0; i < mass_list.size(); i++)
//	{
//		mass_list[i].resize(skipLineM);
//		for (int j = 0; j < skipLineM; j++)
//		{
//			mass_list[i][j] = atof(strList[massID[i]+j].c_str());
//		}
//	}
//	//发动机参数列表编号
//	const int EM = 0;	
//	const int EXc = 1;	const int EYc = 2;	const int EZc = 3;
//	const int EXf = 4;	const int EYf = 5;	const int EZf = 6;
//	const int EFx = 7;	const int EFy = 8;	const int EFz = 9;
//	for (int i = 0; i < Engin_list.size(); i++)
//	{
//		Engin_list[i].resize(skipLineE);
//		for (int j = 0; j < skipLineE; j++)
//		{
//			Engin_list[i][j] = atof(strList[EnginID[i]+j].c_str());
//		}
//	}
//	//舱段参数列表编号
//	const int CLength = 0;	const int CRadius = 1;	const int CT = 2;
//	const int CE = 3;		const int CRho = 4;		const int CNu = 5;	const int CSite = 6;//参数6代表绝对坐标位置
//	double cabin_site_x = 0;
//	for (int i = 0; i < Cabin_list.size(); i++)
//	{
//		Cabin_list[i].resize(skipLineC);
//		for (int j = 0; j < skipLineC; j++)
//		{
//			Cabin_list[i][j] = atof(strList[CabinID[i]+j].c_str());
//		}
//		cabin_site_x += Cabin_list[i][CLength];
//		if (Cabin_list[i].size() < CSite+1)			Cabin_list[i].push_back(cabin_site_x);
//		else										Cabin_list[i][CSite] = cabin_site_x;
//	}
//	//隔框参数列表编号
//	const int BE = 0;	const int BRho = 1;	const int BNu = 2;
//	const int BD1 = 3;	const int BD2 = 4;	const int BD3 = 5;	const int BD4 = 6;	const int BD5 = 7;	const int BD6 = 8;
//	for (int i = 0; i < bh_list.size(); i++)
//	{
//		bh_list[i].resize(BD6+1);
//		for (int j = 0; j < BD6+1; j++)
//		{
//			bh_list[i][j] = atof(strList[bhID[i]+j].c_str());
//		}
//	}
//
//
//	//----------------------------参数设置----------------------------
//	initinfo.ShapeFuncList.resize(9);
//	initinfo.ShapeFuncList[0]  = Point(0,0);			initinfo.ShapeFuncList[1]  = Point(500,177.277);	initinfo.ShapeFuncList[2]  = Point(1000,291.822);
//	initinfo.ShapeFuncList[3]  = Point(1500,386.266);	initinfo.ShapeFuncList[4]  = Point(2000,466.69);	initinfo.ShapeFuncList[5]  = Point(2500,535.162);
//	initinfo.ShapeFuncList[6]  = Point(3000,591.979);	initinfo.ShapeFuncList[7]  = Point(3500,635.746);	initinfo.ShapeFuncList[8]  = Point(4000,660);//8为头罩仓的结束位置
//	//initinfo.ShapeFuncList[9]  = Point(6000,660);		initinfo.ShapeFuncList[10] = Point(6700,660);		initinfo.ShapeFuncList[11] = Point(9817,660);
//	//initinfo.ShapeFuncList[12] = Point(10817,835);	initinfo.ShapeFuncList[13] = Point(17817,835);		initinfo.ShapeFuncList[14] = Point(18607,850);
//	for (int i = 0; i < initinfo.ShapeFuncList.size(); i++) initinfo.ShapeFuncList[i]= initinfo.ShapeFuncList[i]/1000;
//	double HeadEtaX = Cabin_list[0][CSite] / initinfo.ShapeFuncList[8].getX();
//	double HeadEtaY = Cabin_list[0][CRadius] / initinfo.ShapeFuncList[8].getY();
//	for (int i = 0; i < 8; i++)
//	{
//		double x_new = initinfo.ShapeFuncList[i].getX() *HeadEtaX;
//		double y_new = initinfo.ShapeFuncList[i].getY() *HeadEtaY;
//		initinfo.ShapeFuncList[i].SetSite(x_new, y_new);
//	}
//	for (int i = 1; i < nowCabinNum; i++)
//	{
//		initinfo.ShapeFuncList.push_back(Point(Cabin_list[i][CSite], Cabin_list[i][CRadius]));
//	}
//	//定义周向坐标点、梁数目
//	double n = 0.5;
//	initinfo.faiNum = 100 * n;	//定义周向坐标点数
//	vector<int> beamPID(0);	//设置零个横梁
//	//定义集中质量（质量、质心、名称（可缺省））
//	vector<MassPoint> mp;
//	for (int i = 0; i < mass_list.size(); i++)
//	{
//		mp.push_back(MassPoint(mass_list[i][MM], Point(mass_list[i][MX],mass_list[i][MY],mass_list[i][MZ]),false));
//	}
//	for (int i = 0; i < Engin_list.size(); i++)
//	{
//		mp.push_back(MassPoint(Engin_list[i][EM],Point(Engin_list[i][EXc],Engin_list[i][EYc],Engin_list[i][EZc]),true));
//	}
//	//定义舱段信息（ x1,  MID,  T, extramass, siteNum = 1,PBARL_list ）
//	vector<CabinInfo> cabinlist;
//	vector<int> siteNum(7); 
//	siteNum[0]=100;siteNum[1]=50;siteNum[2]=20;siteNum[3]=60;siteNum[4]=20;siteNum[5]=200;siteNum[6]=20;
//	for (int i=0;i<siteNum.size();i++) siteNum[i]*=n;
//
//	vector<vector<int>> PBARL_LIST(7);//隔框数据 这里是给死的
//	PBARL_LIST[0].push_back(1);	PBARL_LIST[0].push_back(1);//第一个隔框在零点位置 不会输出
//	PBARL_LIST[1].push_back(2);
//	PBARL_LIST[2].push_back(3);	PBARL_LIST[2].push_back(4);
//	PBARL_LIST[3].push_back(5);
//	PBARL_LIST[4].push_back(6);	PBARL_LIST[4].push_back(7);
//	PBARL_LIST[5].push_back(8);
//	PBARL_LIST[6].push_back(9);	PBARL_LIST[6].push_back(10);
//	for (int i = 0; i < nowCabinNum; i++)//设置舱段信息
//	{
//		cabinlist.push_back(CabinInfo(Cabin_list[i][CSite], i+1,Cabin_list[i][CT],0,siteNum[i],PBARL_LIST[i]));
//	}
//
//	bool ifUseCabinToCalcXsite = true;//使用舱段初始化信息计算轴向坐标点数
//	//气动力设置
//	ReadCP aero;
//	aero.readFile(path_list[PAer]);
//	ofstream ofs(path_list[POut]);
//
//	//定义隔框、横梁的截面属性和材料信息
//	vector<PBARL> property_list;
//	vector<MAT1> meterial_list;
//	for (int i = 0; i < CabinNum; i++)
//	{
//		//MAT1 mt;
//		//stringstream ss; ss << i+1;
//		//mt.name = "Cabin mat1 id = " + ss.str();
//		//mt.MID = i + 1;
//		//mt.E = Cabin_list[i][CE];
//		//mt.RHO = Cabin_list[i][CRho];
//		//mt.Nu = Cabin_list[i][CNu];
//		//meterial_list.push_back(mt);
//
//		MAT1 mt;
//		stringstream ss; ss << i+1;
//		mt.name = "Cabin mat1 id = " + ss.str();
//		mt.MID = i + 1;
//		mt.E = Cabin_list[i][CE];
//		mt.RHO = Cabin_list[i][CRho];
//		mt.Nu = Cabin_list[i][CNu];
//		meterial_list.push_back(mt);
//	}
//	for (int i = 0; i < bh_list.size(); i++)
//	{
//		PBARL pb;
//		stringstream ss; ss << i+1;
//		pb.name = "bulkhead property id = " + ss.str();
//		pb.PID = i + 1;
//		pb.MID = CabinNum + i + 1;
//		pb.TYPE = "I";
//		pb.DIM.resize(6);
//		pb.DIM[0] = bh_list[i][BD1]; pb.DIM[1] = bh_list[i][BD2]; pb.DIM[2] = bh_list[i][BD3]; 
//		pb.DIM[3] = bh_list[i][BD4]; pb.DIM[4] = bh_list[i][BD5]; pb.DIM[5] = bh_list[i][BD6]; 
//		property_list.push_back(pb);
//		MAT1 mt;
//		mt.name = "bulkhead mat1 id = " + ss.str();
//		mt.MID = CabinNum + i + 1;
//		mt.E = bh_list[i][BE];
//		mt.RHO = bh_list[i][BRho];
//		mt.Nu = bh_list[i][BNu];
//		meterial_list.push_back(mt);
//	}
//
//	//发动机推力设置
//	ConcentratedForce EngineF;
//	if (nowEngine < 1 || nowEngine > 3)		nowEngine = 1;
//	EngineF.force = Point(Engin_list[nowEngine-1][EFx],Engin_list[nowEngine-1][EFy],Engin_list[nowEngine-1][EFz]);
//	EngineF.origin = Point(Engin_list[nowEngine-1][EXf],Engin_list[nowEngine-1][EYf],Engin_list[nowEngine-1][EZf]);
//	for (int i = 0; i < aero.CP_vec.size(); i++)
//	{
//		
//		//-----------------------参数设置-----------------------
//		SetShapeFunc(initinfo.ShapeFuncList);
//		SetCabin(cabinlist);
//		SetMassPoint(mp, 2);//
//		SetStruct(beamPID);
//		SetMeshSkin(FaiNum,ifUseCabinToCalcXsite);
//		SetBeamProperty(property_list,meterial_list);
//		SetForce(aero.CP_vec[i], EngineF);
//		
//		cout << "正在进行第" << i+1 << "个工况的计算" << endl;
//		cout << "[   Alp = " << aero.CP_vec[i].alpha << "  Ma = " << aero.CP_vec[i].mach << "   ]" << endl;
//		//参数计算
//		CalcModel();
//		//结果展示阶段
//		vector<CabinInfo> cb_list_res = GetCabinList();
//		Mesh* mesh_res = GetMesh();
//		stringstream ss; ss << i+1;
//		string filepath = path_list[PBdf];
//		string filename = path_list[PNam] + "_" + ss.str();
//		PrintMesh(filepath + "\\" + filename, 101);
//		cout << "网格生成完成" << endl;
//		double mess = GetStructMass();
//
//
//		//调用nastran
//		myNastran nas;
//		nas.NastranPath = path_list[PNas];
//		nas.BDFPath = filepath + "\\" + filename + ".bdf";
//		nas.WorkPath = filepath;
//		nas.CalcFilePath();
//		nas.NastranCalc();
//		//给出单元属于哪个舱段
//		vector<int> CQUAD4_id;
//
//		for (int j = 0; j < cb_list_res.size(); j++)
//		{
//			CQUAD4_id.push_back(cb_list_res[j].cabinPID0);//点对应的舱段和单元编号对应舱段基本相似
//		}
//		vector<int> bhBAR_id;
//		for (int j = 0; j < bhNum; j++)
//		{
//			bhBAR_id.push_back(mesh_res->E[0].size() + j * FaiNum);
//		}
//		if (nas.ReadResPCH(CQUAD4_id,bhBAR_id) < 0)
//		{
//			//system("pause");
//			return -1;
//		}
//
//		ofs << "结构质量 = " << GetStructMass() << endl;
//		ofs << "[  " << "Alp = " << aero.CP_vec[i].alpha << "  Ma = " << aero.CP_vec[i].mach << "  ]" << endl;
//		for (int j = 0; j < cb_list_res.size(); j++)
//		{
//			ofs << "舱段" << j + 1 << "最大工况输出：" << endl;
//			ofs << "  最大应力值     = " << nas.GetMaxStress()[j] << endl;
//			Point maxStress =(mesh_res->P[mesh_res->E[0][nas.GetMaxStressID()[j]][0]] + 
//				mesh_res->P[mesh_res->E[0][nas.GetMaxStressID()[j]][1]] +
//				mesh_res->P[mesh_res->E[0][nas.GetMaxStressID()[j]][2]] +
//				mesh_res->P[mesh_res->E[0][nas.GetMaxStressID()[j]][3]]) / 4.0;
//			ofs << "  最大应力点坐标 = (" << maxStress.getX() << ", " << maxStress.getY() << ", " << maxStress.getZ() << ")" << endl;
//
//			ofs << "  最大应变值     = " << nas.GetMaxStrain()[j] << endl;
//			Point maxStrain =(mesh_res->P[mesh_res->E[0][nas.GetMaxStrainID()[j]][0]] + 
//				mesh_res->P[mesh_res->E[0][nas.GetMaxStrainID()[j]][1]] +
//				mesh_res->P[mesh_res->E[0][nas.GetMaxStrainID()[j]][2]] +
//				mesh_res->P[mesh_res->E[0][nas.GetMaxStrainID()[j]][3]]) / 4.0;
//			ofs << "  最大应变点坐标 = (" << maxStrain.getX() << ", " << maxStrain.getY() << ", " << maxStrain.getZ() << ")" << endl;
//		}
//		ofs << endl;
//		for (int j = 0; j < nas.GetMaxStressBar().size(); j++)
//		{
//			ofs << "隔框" << j + 1 << "最大工况输出：" << endl;
//			ofs << "  最大应力值     = " << nas.GetMaxStressBar()[j] << endl;
//			Point maxStress = (mesh_res->P[mesh_res->E[1][nas.GetMaxStressIDBar()[j]-mesh_res->E[0].size()][0]] +
//				mesh_res->P[mesh_res->E[1][nas.GetMaxStressIDBar()[j]-mesh_res->E[0].size()][1]]) / 2.0;
//			ofs << "  最大应力点坐标 = (" << maxStress.getX() << ", " << maxStress.getY() << ", " << maxStress.getZ() << ")" << endl;
//
//			ofs << "  最大应变值     = " << nas.GetMaxStrainBar()[j] << endl;
//			Point maxStrain = (mesh_res->P[mesh_res->E[1][nas.GetMaxStrainIDBar()[j]-mesh_res->E[0].size()][0]] +
//				mesh_res->P[mesh_res->E[1][nas.GetMaxStrainIDBar()[j]-mesh_res->E[0].size()][1]]) / 2.0;
//			ofs << "  最大应变点坐标 = (" << maxStrain.getX() << ", " << maxStrain.getY() << ", " << maxStrain.getZ() << ")" << endl;
//		}
//	}
//
//	ofs.close();
//	return 0;
//}
//
//int Minuteman3::ReadFromFile_old_2(string path)
//{
//	//----------------------------参数读取----------------------------
//	vector<string> strList;//文件路径名称等信息
//	cout << "输入文件：" << path << endl;
//	ifstream ifs(path);
//	if (ifs.fail())
//	{
//		cout << "打开失败，文件不存在！" << endl;
//		return -1;
//	}
//	else
//	{
//		cout << "打开成功！" << endl;
//	}
//	string str_line;
//	for (int i = 0; getline(ifs, str_line);i++)
//	{
//		stringstream ss(str_line);
//		string str_tmp;
//		getline(ss, str_tmp, '=');
//		getline(ss, str_tmp, '=');
//		strList.push_back(str_tmp);
//	}
//	ifs.close();
//
//	//----------------------------参数处理----------------------------
//	const int pathID = 0;		vector<string> path_list(5);
//	const int nowCasID = 14;	int nowCase;	int nowCabinNum;
//	const int nowEngID = 15;	int nowEngine;
//
//	const int massNum = 2, skipLineM = 4;//质量个数、每个质量文件行数
//	vector<int> massID(massNum);	massID[0] = 5;		vector<vector<double>> mass_list(massNum);
//	for (int i = 1; i < massNum;i++)	massID[i] = massID[i-1] + skipLineM;
//
//	const int EnginNum = 3, skipLineE = 10;//发动机个数、每个发动机行数
//	vector<int> EnginID(EnginNum);	EnginID[0] = 15;	vector<vector<double>> Engin_list(EnginNum);
//	for (int i = 1; i < EnginNum;i++)	EnginID[i] = EnginID[i-1] + skipLineE;
//
//	const int CabinNum = 7, skipLineC = 10;//舱段个数、每个舱段行数
//	vector<int> CabinID(CabinNum);	CabinID[0] = 45;	vector<vector<double>> Cabin_list(CabinNum);
//	for (int i = 1; i < CabinNum;i++)	CabinID[i] = CabinID[i-1] + skipLineC;
//
//	const int bhNum = 10, skipLineH = 7;//隔框个数、每个隔框行数
//	vector<int> bhID(bhNum);		bhID[0] = 115;		vector<vector<double>> bh_list(bhNum);
//	for (int i = 1; i < bhNum;i++)		bhID[i] = bhID[i-1] + skipLineH;
//	//---------------------------------------------------------------------
//	//文件路径参数列表编号
//	const int PNas = 0;	const int PBdf = 1;	const int PNam = 2;	const int POut = 3;	const int PAer = 4;
//	for (int i = 0; i < path_list.size(); i++)
//	{
//		path_list[i] = strList[pathID+i];
//	}
//	//设置当前舱段数量
//	nowCase = atoi(strList[nowCasID].c_str());
//	switch (nowCase)
//	{
//	case 1: nowCabinNum = 7; break;
//	case 2: nowCabinNum = 5; break;
//	case 3: nowCabinNum = 3; break;
//	case 4: nowCabinNum = 1; break;
//	default: nowCabinNum = Cabin_list.size();
//	}
//	nowEngine = atoi(strList[nowEngID].c_str());
//	//质量块参数列表编号
//	const int MM = 0;	const int MX = 1;	const int MY = 2;	const int MZ = 3;
//	for (int i = 0; i < mass_list.size(); i++)
//	{
//		mass_list[i].resize(skipLineM);
//		for (int j = 0; j < skipLineM; j++)
//		{
//			mass_list[i][j] = atof(strList[massID[i]+j].c_str());
//		}
//	}
//	//发动机参数列表编号
//	const int EM = 0;	
//	const int EXc = 1;	const int EYc = 2;	const int EZc = 3;
//	const int EXf = 4;	const int EYf = 5;	const int EZf = 6;
//	const int EFx = 7;	const int EFy = 8;	const int EFz = 9;
//	for (int i = 0; i < Engin_list.size(); i++)
//	{
//		Engin_list[i].resize(skipLineE);
//		for (int j = 0; j < skipLineE; j++)
//		{
//			Engin_list[i][j] = atof(strList[EnginID[i]+j].c_str());
//		}
//	}
//	//舱段参数列表编号
//	const int CLength = 0;	const int CRadius = 1;	const int CT = 2;
//	const int CE1 = 3;		const int CE2 = 4;		const int CNu12 = 5;	
//	const int CG12 = 6;		const int CG1Z = 7;		const int CG2Z = 8;
//	const int CRho = 9;		const int CSite = 10;
//	double cabin_site_x = 0;
//	for (int i = 0; i < Cabin_list.size(); i++)
//	{
//		Cabin_list[i].resize(skipLineC);
//		for (int j = 0; j < skipLineC; j++)
//		{
//			Cabin_list[i][j] = atof(strList[CabinID[i]+j].c_str());
//		}
//		cabin_site_x += Cabin_list[i][CLength];
//		if (Cabin_list[i].size() < CSite+1)			Cabin_list[i].push_back(cabin_site_x);
//		else										Cabin_list[i][CSite] = cabin_site_x;
//	}
//	//隔框参数列表编号
//	const int BE = 0;	const int BRho = 1;	const int BNu = 2;
//	const int BD1 = 3;	const int BD2 = 4;	const int BD3 = 5;	const int BD4 = 6;	//const int BD5 = 7;	const int BD6 = 8;
//	for (int i = 0; i < bh_list.size(); i++)
//	{
//		bh_list[i].resize(BD4+1);
//		for (int j = 0; j < BD4+1; j++)
//		{
//			bh_list[i][j] = atof(strList[bhID[i]+j].c_str());
//		}
//	}
//
//
//	//----------------------------参数设置----------------------------
//	initinfo.ShapeFuncList.resize(9);
//	initinfo.ShapeFuncList[0]  = Point(0,0);			initinfo.ShapeFuncList[1]  = Point(500,177.277);	initinfo.ShapeFuncList[2]  = Point(1000,291.822);
//	initinfo.ShapeFuncList[3]  = Point(1500,386.266);	initinfo.ShapeFuncList[4]  = Point(2000,466.69);	initinfo.ShapeFuncList[5]  = Point(2500,535.162);
//	initinfo.ShapeFuncList[6]  = Point(3000,591.979);	initinfo.ShapeFuncList[7]  = Point(3500,635.746);	initinfo.ShapeFuncList[8]  = Point(4000,660);//8为头罩仓的结束位置
//	//initinfo.ShapeFuncList[9]  = Point(6000,660);		initinfo.ShapeFuncList[10] = Point(6700,660);		initinfo.ShapeFuncList[11] = Point(9817,660);
//	//initinfo.ShapeFuncList[12] = Point(10817,835);	initinfo.ShapeFuncList[13] = Point(17817,835);		initinfo.ShapeFuncList[14] = Point(18607,850);
//	for (int i = 0; i < initinfo.ShapeFuncList.size(); i++) initinfo.ShapeFuncList[i]= initinfo.ShapeFuncList[i]/1000;
//	double HeadEtaX = Cabin_list[0][CSite] / initinfo.ShapeFuncList[8].getX();
//	double HeadEtaY = Cabin_list[0][CRadius] / initinfo.ShapeFuncList[8].getY();
//	for (int i = 0; i < 8; i++)
//	{
//		double x_new = initinfo.ShapeFuncList[i].getX() *HeadEtaX;
//		double y_new = initinfo.ShapeFuncList[i].getY() *HeadEtaY;
//		initinfo.ShapeFuncList[i].SetSite(x_new, y_new);
//	}
//	for (int i = 1; i < nowCabinNum; i++)
//	{
//		initinfo.ShapeFuncList.push_back(Point(Cabin_list[i][CSite], Cabin_list[i][CRadius]));
//	}
//	//定义周向坐标点、梁数目
//	double n = 0.25;
//	initinfo.faiNum = 100 * n;	//定义周向坐标点数
//	vector<int> beamPID(0);
//	//定义集中质量（质量、质心、名称（可缺省））
//	vector<MassPoint> mp;
//	for (int i = 0; i < mass_list.size(); i++)
//	{
//		mp.push_back(MassPoint(mass_list[i][MM], Point(mass_list[i][MX],mass_list[i][MY],mass_list[i][MZ]),false));
//	}
//	for (int i = 0; i < Engin_list.size(); i++)
//	{
//		mp.push_back(MassPoint(Engin_list[i][EM],Point(Engin_list[i][EXc],Engin_list[i][EYc],Engin_list[i][EZc]),true));
//	}
//	//定义舱段信息（ x1,  MID,  T, extramass, siteNum = 1,PBARL_list ）
//	vector<CabinInfo> cabinlist;
//	vector<int> siteNum(7); 
//	siteNum[0]=100;siteNum[1]=50;siteNum[2]=20;siteNum[3]=60;siteNum[4]=20;siteNum[5]=200;siteNum[6]=20;
//	for (int i=0;i<siteNum.size();i++) siteNum[i]*=n;
//
//	vector<vector<int>> PBARL_LIST(7);//隔框数据 这里是定死的7个舱段
//	PBARL_LIST[0].push_back(1);	PBARL_LIST[0].push_back(1);//第一个隔框在零点位置 不会输出
//	PBARL_LIST[1].push_back(2);
//	PBARL_LIST[2].push_back(3);	PBARL_LIST[2].push_back(4);
//	PBARL_LIST[3].push_back(5);
//	PBARL_LIST[4].push_back(6);	PBARL_LIST[4].push_back(7);
//	PBARL_LIST[5].push_back(8);
//	PBARL_LIST[6].push_back(9);	PBARL_LIST[6].push_back(10);
//	for (int i = 0; i < nowCabinNum; i++)//设置舱段信息
//	{
//		cabinlist.push_back(CabinInfo(Cabin_list[i][CSite], i+1,Cabin_list[i][CT],0,siteNum[i],PBARL_LIST[i]));
//	}
//
//	bool ifUseCabinToCalcXsite = true;//使用舱段初始化信息计算轴向坐标点数
//	//气动力设置
//	ReadCP aero;
//	aero.readFile(path_list[PAer]);
//	ofstream ofs(path_list[POut]);
//
//	//定义隔框、横梁的截面属性和材料信息
//	vector<PBARL> property_list;
//	vector<MAT1> meterial_list;
//	vector<MAT8> meterial8_list;
//	for (int i = 0; i < CabinNum; i++)
//	{
//		MAT8 mt;
//		stringstream ss; ss << i+1;
//		mt.name = "Cabin mat8 id = " + ss.str();
//		mt.MID = i + 1;
//		mt.E1 = Cabin_list[i][CE1];
//		mt.E2 = Cabin_list[i][CE2];
//		mt.NU12 = Cabin_list[i][CNu12];
//		mt.G12 = Cabin_list[i][CG12];
//		mt.G1Z = Cabin_list[i][CG1Z];
//		mt.G2Z = Cabin_list[i][CG2Z];
//		mt.RHO = Cabin_list[i][CRho];
//		meterial8_list.push_back(mt);
//	}
//	for (int i = 0; i < bh_list.size(); i++)
//	{
//		PBARL pb;
//		stringstream ss; ss << i+1;
//		pb.name = "bulkhead property id = " + ss.str();
//		pb.PID = i + 1;
//		pb.MID = CabinNum + i + 1;
//		pb.TYPE = "CHAN";//pb.TYPE = "I";
//		pb.DIM.resize(4);
//		pb.DIM[0] = bh_list[i][BD1]; pb.DIM[1] = bh_list[i][BD2]; pb.DIM[2] = bh_list[i][BD3]; 
//		pb.DIM[3] = bh_list[i][BD4]; //pb.DIM[4] = bh_list[i][BD5]; pb.DIM[5] = bh_list[i][BD6]; 
//		property_list.push_back(pb);
//		MAT1 mt;
//		mt.name = "bulkhead mat1 id = " + ss.str();
//		mt.MID = CabinNum + i + 1;
//		mt.E = bh_list[i][BE];
//		mt.RHO = bh_list[i][BRho];
//		mt.Nu = bh_list[i][BNu];
//		meterial_list.push_back(mt);
//	}
//
//	//发动机推力设置
//	ConcentratedForce EngineF;
//	if (nowEngine < 1 || nowEngine > 3)		nowEngine = 1;
//	EngineF.force = Point(Engin_list[nowEngine-1][EFx],Engin_list[nowEngine-1][EFy],Engin_list[nowEngine-1][EFz]);
//	EngineF.origin = Point(Engin_list[nowEngine-1][EXf],Engin_list[nowEngine-1][EYf],Engin_list[nowEngine-1][EZf]);
//	for (int i = 0; i < aero.CP_vec.size(); i++)
//	{
//
//		//-----------------------参数设置-----------------------
//		SetShapeFunc(initinfo.ShapeFuncList);
//		SetCabin(cabinlist);
//		SetMassPoint(mp, 2);//
//		SetStruct(beamPID);
//		SetMeshSkin(FaiNum,ifUseCabinToCalcXsite);
//		SetBeamProperty(property_list,meterial_list,meterial8_list);
//		SetForce(aero.CP_vec[i], EngineF);
//
//		cout << "正在进行第" << i+1 << "个工况的计算" << endl;
//		cout << "[   Alp = " << aero.CP_vec[i].alpha << "  Ma = " << aero.CP_vec[i].mach << "   ]" << endl;
//		//参数计算
//		CalcModel();
//		//结果展示阶段
//		vector<CabinInfo> cb_list_res = GetCabinList();
//		Mesh* mesh_res = GetMesh();
//		stringstream ss; ss << i+1;
//		string filepath = path_list[PBdf];
//		string filename = path_list[PNam] + "_" + ss.str();
//		PrintMesh(filepath + "\\" + filename, 101);
//		cout << "网格生成完成" << endl;
//		double mess = GetStructMass();
//
//
//		//调用nastran
//		myNastran nas;
//		nas.NastranPath = path_list[PNas];
//		nas.BDFPath = filepath + "\\" + filename + ".bdf";
//		nas.WorkPath = filepath;
//		nas.CalcFilePath();
//		nas.NastranCalc();
//		//给出单元属于哪个舱段
//		vector<int> CQUAD4_id;
//		for (int j = 0; j < cb_list_res.size(); j++)
//		{
//			CQUAD4_id.push_back(cb_list_res[j].cabinPID0);//点对应的舱段和单元编号对应舱段基本相似
//		}
//		vector<int> bhBAR_id;
//		for (int j = 0; j < bhNum; j++)
//		{
//			bhBAR_id.push_back(mesh_res->E[0].size() + j * FaiNum);
//		}
//		if (nas.ReadResPCH(CQUAD4_id,bhBAR_id) < 0)
//		{
//			//system("pause");
//			return -1;
//		}
//		
//
//		ofs << "结构质量 = " << GetStructMass() << endl;
//		ofs << "[  " << "Alp = " << aero.CP_vec[i].alpha << "  Ma = " << aero.CP_vec[i].mach << "  ]" << endl;
//		for (int j = 0; j < cb_list_res.size(); j++)
//		{
//			ofs << "舱段" << j + 1 << "最大工况输出：" << endl;
//			ofs << "  最大应力值     = " << nas.GetMaxStress()[j] << endl;
//			Point maxStress =(mesh_res->P[mesh_res->E[0][nas.GetMaxStressID()[j]][0]] + 
//				mesh_res->P[mesh_res->E[0][nas.GetMaxStressID()[j]][1]] +
//				mesh_res->P[mesh_res->E[0][nas.GetMaxStressID()[j]][2]] +
//				mesh_res->P[mesh_res->E[0][nas.GetMaxStressID()[j]][3]]) / 4.0;
//			ofs << "  最大应力点坐标 = (" << maxStress.getX() << ", " << maxStress.getY() << ", " << maxStress.getZ() << ")" << endl;
//
//			ofs << "  最大应变值     = " << nas.GetMaxStrain()[j] << endl;
//			Point maxStrain =(mesh_res->P[mesh_res->E[0][nas.GetMaxStrainID()[j]][0]] + 
//				mesh_res->P[mesh_res->E[0][nas.GetMaxStrainID()[j]][1]] +
//				mesh_res->P[mesh_res->E[0][nas.GetMaxStrainID()[j]][2]] +
//				mesh_res->P[mesh_res->E[0][nas.GetMaxStrainID()[j]][3]]) / 4.0;
//			ofs << "  最大应变点坐标 = (" << maxStrain.getX() << ", " << maxStrain.getY() << ", " << maxStrain.getZ() << ")" << endl;
//		}
//		ofs << endl;
//		for (int j = 0; j < nas.GetMaxStressBar().size(); j++)
//		{
//			ofs << "隔框" << j + 1 << "最大工况输出：" << endl;
//			ofs << "  最大应力值     = " << nas.GetMaxStressBar()[j] << endl;
//			Point maxStress = (mesh_res->P[mesh_res->E[1][nas.GetMaxStressIDBar()[j]-mesh_res->E[0].size()][0]] +
//				mesh_res->P[mesh_res->E[1][nas.GetMaxStressIDBar()[j]-mesh_res->E[0].size()][1]]) / 2.0;
//			ofs << "  最大应力点坐标 = (" << maxStress.getX() << ", " << maxStress.getY() << ", " << maxStress.getZ() << ")" << endl;
//
//			ofs << "  最大应变值     = " << nas.GetMaxStrainBar()[j] << endl;
//			Point maxStrain = (mesh_res->P[mesh_res->E[1][nas.GetMaxStrainIDBar()[j]-mesh_res->E[0].size()][0]] +
//				mesh_res->P[mesh_res->E[1][nas.GetMaxStrainIDBar()[j]-mesh_res->E[0].size()][1]]) / 2.0;
//			ofs << "  最大应变点坐标 = (" << maxStrain.getX() << ", " << maxStrain.getY() << ", " << maxStrain.getZ() << ")" << endl;
//		}
//	}
//
//	ofs.close();
//	return 0;
//}
//
//int Minuteman3::ReadFromFile_old_3(string path)
//{
//	//----------------------------参数处理----------------------------
//	const int pathID = 0;		vector<string> path_list(5);
//	const int nowCasID = 14;	int nowCase;	int nowCabinNum; int nowBHnum;
//	const int nowEngID = 15;	int nowEngine = 0;
//
//	const int massNum = 2, skipLineM = 4;//质量个数、每个质量文件行数
//	vector<int> massID(massNum);	massID[0] = 5;		vector<vector<double>> mass_list(massNum);
//	for (int i = 1; i < massNum;i++)	massID[i] = massID[i-1] + skipLineM;
//	for (int i = 0; i < mass_list.size(); i++)	 mass_list[i].resize(skipLineM); 
//
//	const int EnginNum = 3, skipLineE = 10;//发动机个数、每个发动机行数
//	vector<int> EnginID(EnginNum);	EnginID[0] = 15;	vector<vector<double>> Engin_list(EnginNum);
//	for (int i = 1; i < EnginNum;i++)	EnginID[i] = EnginID[i-1] + skipLineE;
//	for (int i = 0; i < Engin_list.size(); i++)	 Engin_list[i].resize(skipLineE); 
//
//	const int CabinNum = 7, skipLineC = 10;//舱段个数、每个舱段行数
//	vector<int> CabinID(CabinNum);	CabinID[0] = 45;	vector<vector<double>> Cabin_list(CabinNum);
//	for (int i = 1; i < CabinNum;i++)	CabinID[i] = CabinID[i-1] + skipLineC;
//	for (int i = 0; i < Cabin_list.size(); i++)	 Cabin_list[i].resize(skipLineC); 
//
//
//	const int bhNum = 10, skipLineH = 7;//隔框个数、每个隔框行数
//	vector<int> bhID(bhNum);		bhID[0] = 115;		vector<vector<double>> bh_list(bhNum);
//	for (int i = 1; i < bhNum;i++)		bhID[i] = bhID[i-1] + skipLineH;
//	for (int i = 0; i < bh_list.size(); i++)	 bh_list[i].resize(skipLineH); 
//	//---------------------------------------------------------------------
//	//-----------------文件路径参数列表编号
//	const int PNas = 0;	const int PBdf = 1;	const int PNam = 2;	const int POut = 3;	const int PAer = 4;
//	//-----------------设置当前舱段数量
//	//-----------------质量块参数列表编号
//	const int MM = 0;	const int MX = 1;	const int MY = 2;	const int MZ = 3;
//	//-----------------发动机参数列表编号
//	const int EM = 0;	
//	const int EXc = 1;	const int EYc = 2;	const int EZc = 3;
//	const int EXf = 4;	const int EYf = 5;	const int EZf = 6;
//	const int EFx = 7;	const int EFy = 8;	const int EFz = 9;
//	//-----------------舱段参数列表编号
//	const int CLength = 0;	const int CRadius = 1;	const int CT = 2;
//	const int CE1 = 3;		const int CE2 = 4;		const int CNu12 = 5;	
//	const int CG12 = 6;		const int CG1Z = 7;		const int CG2Z = 8;
//	const int CRho = 9;		const int CSite = 10;
//	double cabin_site_x = 0;
//	//-----------------隔框参数列表编号
//	const int BE = 0;	const int BRho = 1;	const int BNu = 2;
//	const int BD1 = 3;	const int BD2 = 4;	const int BD3 = 5;	const int BD4 = 6;	//const int BD5 = 7;	const int BD6 = 8;
//
//	//----------------------------参数读取----------------------------
//	vector<string> strList;//文件路径名称等信息
//	cout << "输入文件：" << path << endl;
//	ifstream ifs(path);
//	if (ifs.fail())
//	{
//		cout << "打开失败，文件不存在！" << endl;
//		return -1;
//	}
//	else
//	{
//		cout << "打开成功！" << endl;
//	}
//	string str_line;
//	for (int i = 0; getline(ifs, str_line);i++)
//	{
//		str_line.erase(remove_if(str_line.begin(), str_line.end(), isspace), str_line.end());//清除空格行
//		stringstream ss(str_line);
//		string str_tmp;
//		getline(ss, str_tmp, '_');
//		if (!str_tmp.compare("NastranPath"))	{	getline(ss, str_tmp, '=');getline(ss, str_tmp, '=');		path_list[PNas] = str_tmp;	}
//		else if (!str_tmp.compare("ModlePath"))	{	getline(ss, str_tmp, '=');getline(ss, str_tmp, '=');		path_list[PBdf] = str_tmp;	}
//		else if (!str_tmp.compare("ModleName"))	{	getline(ss, str_tmp, '=');getline(ss, str_tmp, '=');		path_list[PNam] = str_tmp;	}
//		else if (!str_tmp.compare("OutputPath")){	getline(ss, str_tmp, '=');getline(ss, str_tmp, '=');		path_list[POut] = str_tmp;	}
//		else if (!str_tmp.compare("AeroPath"))	{	getline(ss, str_tmp, '=');getline(ss, str_tmp, '=');		path_list[PAer] = str_tmp;	}
//		else if (!str_tmp.compare("NowCase"))	{	getline(ss, str_tmp, '=');getline(ss, str_tmp, '=');		nowCase = atoi(str_tmp.c_str());	}
//		else if (!str_tmp.compare("NowEngine"))	{	getline(ss, str_tmp, '=');getline(ss, str_tmp, '=');		nowEngine = atoi(str_tmp.c_str());	}
//		else if (!str_tmp.compare("part"))
//		{
//			getline(ss, str_tmp, '_');			int id = atoi(str_tmp.c_str())-1;
//			getline(ss, str_tmp, '=');			
//			if (!str_tmp.compare("mass"))	{	getline(ss, str_tmp, '=');		mass_list[id][MM] = atof(str_tmp.c_str());	}
//			else if (!str_tmp.compare("xc")){	getline(ss, str_tmp, '=');		mass_list[id][MX] = atof(str_tmp.c_str());	}
//			else if (!str_tmp.compare("yc")){	getline(ss, str_tmp, '=');		mass_list[id][MY] = atof(str_tmp.c_str());	}
//			else if (!str_tmp.compare("zc")){	getline(ss, str_tmp, '=');		mass_list[id][MZ] = atof(str_tmp.c_str());	}
//		}
//		else if (!str_tmp.compare("engine"))
//		{
//			getline(ss, str_tmp, '_');			int id = atoi(str_tmp.c_str())-1;
//			getline(ss, str_tmp, '=');			
//			if (!str_tmp.compare("mass"))	 {	getline(ss, str_tmp, '=');			Engin_list[id][EM] = atof(str_tmp.c_str());	}
//			else if (!str_tmp.compare("xc")) {	getline(ss, str_tmp, '=');			Engin_list[id][EXc] = atof(str_tmp.c_str());}
//			else if (!str_tmp.compare("yc")) {	getline(ss, str_tmp, '=');			Engin_list[id][EYc] = atof(str_tmp.c_str());}
//			else if (!str_tmp.compare("zc")) {	getline(ss, str_tmp, '=');			Engin_list[id][EZc] = atof(str_tmp.c_str());}
//			else if (!str_tmp.compare("Fsx")){	getline(ss, str_tmp, '=');			Engin_list[id][EXf] = atof(str_tmp.c_str());}
//			else if (!str_tmp.compare("Fsy")){	getline(ss, str_tmp, '=');			Engin_list[id][EYf] = atof(str_tmp.c_str());}
//			else if (!str_tmp.compare("Fsz")){	getline(ss, str_tmp, '=');			Engin_list[id][EZf] = atof(str_tmp.c_str());}
//			else if (!str_tmp.compare("Fx")) {	getline(ss, str_tmp, '=');			Engin_list[id][EFx] = atof(str_tmp.c_str());}
//			else if (!str_tmp.compare("Fy")) {	getline(ss, str_tmp, '=');			Engin_list[id][EFy] = atof(str_tmp.c_str());}
//			else if (!str_tmp.compare("Fz")) {	getline(ss, str_tmp, '=');			Engin_list[id][EFz] = atof(str_tmp.c_str());}
//		}
//		else if (!str_tmp.compare("cabin"))
//		{
//			getline(ss, str_tmp, '_');			int id = atoi(str_tmp.c_str())-1;
//			getline(ss, str_tmp, '=');			
//			if (!str_tmp.compare("length"))		{	getline(ss, str_tmp, '=');		Cabin_list[id][CLength] = atof(str_tmp.c_str());}
//			else if (!str_tmp.compare("radius")){	getline(ss, str_tmp, '=');		Cabin_list[id][CRadius] = atof(str_tmp.c_str());}
//			else if (!str_tmp.compare("T"))		{	getline(ss, str_tmp, '=');		Cabin_list[id][CT] = atof(str_tmp.c_str());		}
//			else if (!str_tmp.compare("E1"))	{	getline(ss, str_tmp, '=');		Cabin_list[id][CE1] = atof(str_tmp.c_str());	}
//			else if (!str_tmp.compare("E2"))	{	getline(ss, str_tmp, '=');		Cabin_list[id][CE2] = atof(str_tmp.c_str());	}
//			else if (!str_tmp.compare("NU12"))	{	getline(ss, str_tmp, '=');		Cabin_list[id][CNu12] = atof(str_tmp.c_str());	}
//			else if (!str_tmp.compare("G12"))	{	getline(ss, str_tmp, '=');		Cabin_list[id][CG12] = atof(str_tmp.c_str());	}
//			else if (!str_tmp.compare("G1Z"))	{	getline(ss, str_tmp, '=');		Cabin_list[id][CG1Z] = atof(str_tmp.c_str());	}
//			else if (!str_tmp.compare("G2Z"))	{	getline(ss, str_tmp, '=');		Cabin_list[id][CG2Z] = atof(str_tmp.c_str());	}
//			else if (!str_tmp.compare("rho"))	{	getline(ss, str_tmp, '=');		Cabin_list[id][CRho] = atof(str_tmp.c_str());	}
//		}
//		else if (!str_tmp.compare("bh"))
//		{
//			getline(ss, str_tmp, '_');			int id = atoi(str_tmp.c_str())-1;
//			getline(ss, str_tmp, '=');			
//			if (!str_tmp.compare("E"))			{	getline(ss, str_tmp, '=');		bh_list[id][BE] = atof(str_tmp.c_str());	}
//			else if (!str_tmp.compare("rho"))	{	getline(ss, str_tmp, '=');		bh_list[id][BRho] = atof(str_tmp.c_str());	}
//			else if (!str_tmp.compare("nu"))	{	getline(ss, str_tmp, '=');		bh_list[id][BNu] = atof(str_tmp.c_str());	}
//			else if (!str_tmp.compare("dim1"))	{	getline(ss, str_tmp, '=');		bh_list[id][BD1] = atof(str_tmp.c_str());	}
//			else if (!str_tmp.compare("dim2"))	{	getline(ss, str_tmp, '=');		bh_list[id][BD2] = atof(str_tmp.c_str());	}
//			else if (!str_tmp.compare("dim3"))	{	getline(ss, str_tmp, '=');		bh_list[id][BD3] = atof(str_tmp.c_str());	}
//			else if (!str_tmp.compare("dim4"))	{	getline(ss, str_tmp, '=');		bh_list[id][BD4] = atof(str_tmp.c_str());	}
//		}
//			
//	}
//	ifs.close();
//	//---------------------------------------------------------------------
//	switch (nowCase)
//	{
//	case 1: nowCabinNum = 7; nowBHnum = 10; break;
//	case 2: nowCabinNum = 5-1; nowBHnum = 7-2;  break;
//	case 3: nowCabinNum = 3-1; nowBHnum = 4-2;  break;
//	case 4: nowCabinNum = 1; nowBHnum = 1;  break;
//	default: nowCabinNum = Cabin_list.size(); nowBHnum = 10;
//	}
//	//
//	for (int i = 0; i < Cabin_list.size(); i++)
//	{
//		cabin_site_x += Cabin_list[i][CLength];
//		if (Cabin_list[i].size() < CSite+1)			Cabin_list[i].push_back(cabin_site_x);
//		else										Cabin_list[i][CSite] = cabin_site_x;
//	}
//
//
//	//----------------------------参数设置----------------------------
//	initinfo.ShapeFuncList.resize(9);
//	initinfo.ShapeFuncList[0]  = Point(0,0);			initinfo.ShapeFuncList[1]  = Point(500,177.277);	initinfo.ShapeFuncList[2]  = Point(1000,291.822);
//	initinfo.ShapeFuncList[3]  = Point(1500,386.266);	initinfo.ShapeFuncList[4]  = Point(2000,466.69);	initinfo.ShapeFuncList[5]  = Point(2500,535.162);
//	initinfo.ShapeFuncList[6]  = Point(3000,591.979);	initinfo.ShapeFuncList[7]  = Point(3500,635.746);	initinfo.ShapeFuncList[8]  = Point(4000,660);//8为头罩仓的结束位置
//	//initinfo.ShapeFuncList[9]  = Point(6000,660);		initinfo.ShapeFuncList[10] = Point(6700,660);		initinfo.ShapeFuncList[11] = Point(9817,660);
//	//initinfo.ShapeFuncList[12] = Point(10817,835);	initinfo.ShapeFuncList[13] = Point(17817,835);		initinfo.ShapeFuncList[14] = Point(18607,850);
//	for (int i = 0; i < initinfo.ShapeFuncList.size(); i++) initinfo.ShapeFuncList[i]= initinfo.ShapeFuncList[i]/1000;
//	double HeadEtaX = Cabin_list[0][CSite] / initinfo.ShapeFuncList[8].getX();
//	double HeadEtaY = Cabin_list[0][CRadius] / initinfo.ShapeFuncList[8].getY();
//	for (int i = 0; i < 8; i++)
//	{
//		double x_new = initinfo.ShapeFuncList[i].getX() *HeadEtaX;
//		double y_new = initinfo.ShapeFuncList[i].getY() *HeadEtaY;
//		initinfo.ShapeFuncList[i].SetSite(x_new, y_new);
//	}
//	for (int i = 1; i < nowCabinNum; i++)
//	{
//		initinfo.ShapeFuncList.push_back(Point(Cabin_list[i][CSite], Cabin_list[i][CRadius]));
//	}
//	//定义周向坐标点、梁数目
//	double n = 0.5;
//	initinfo.faiNum = 100 * n;	//定义周向坐标点数
//	vector<int> beamPID(0);
//	//定义集中质量（质量、质心、名称（可缺省））
//	vector<MassPoint> mp;
//	for (int i = 0; i < mass_list.size(); i++)
//	{
//		mp.push_back(MassPoint(mass_list[i][MM], Point(mass_list[i][MX],mass_list[i][MY],mass_list[i][MZ]),false));
//	}
//	for (int i = 0; i < Engin_list.size(); i++)
//	{
//		mp.push_back(MassPoint(Engin_list[i][EM],Point(Engin_list[i][EXc],Engin_list[i][EYc],Engin_list[i][EZc]),true));
//	}
//	//定义舱段信息（ x1,  MID,  T, extramass, siteNum = 1,PBARL_list ）
//	vector<CabinInfo> cabinlist;
//	vector<int> siteNum(7); 
//	siteNum[0]=100;siteNum[1]=50;siteNum[2]=20;siteNum[3]=60;siteNum[4]=20;siteNum[5]=200;siteNum[6]=20;
//	for (int i=0;i<siteNum.size();i++) siteNum[i]*=n;
//
//	vector<vector<int>> PBARL_LIST(7);//隔框数据 这里是定死的7个舱段
//	PBARL_LIST[0].push_back(1);	PBARL_LIST[0].push_back(1);//第一个隔框在零点位置 不会输出
//	PBARL_LIST[1].push_back(2);
//	PBARL_LIST[2].push_back(3);	PBARL_LIST[2].push_back(4);
//	PBARL_LIST[3].push_back(5);
//	PBARL_LIST[4].push_back(6);	PBARL_LIST[4].push_back(7);
//	PBARL_LIST[5].push_back(8);
//	PBARL_LIST[6].push_back(9);	PBARL_LIST[6].push_back(10);
//	for (int i = 0; i < nowCabinNum; i++)//设置舱段信息
//	{
//		cabinlist.push_back(CabinInfo(Cabin_list[i][CSite], i+1,Cabin_list[i][CT],0,siteNum[i],PBARL_LIST[i]));
//	}
//
//	bool ifUseCabinToCalcXsite = true;//使用舱段初始化信息计算轴向坐标点数
//	//气动力设置
//	ReadCP aero;
//	aero.readFile(path_list[PAer]);
//	if (aero.CP_vec.size() == 0)
//	{
//		cout << "当前未能读取到正确的气动力数据！分析终止" << endl;
//		//system("pause");
//		return -1;
//	}
//	ofstream ofs(path_list[POut]);
//
//	//定义隔框、横梁的截面属性和材料信息
//	vector<PBARL> property_list;
//	vector<MAT1> meterial_list;
//	vector<MAT8> meterial8_list;
//	for (int i = 0; i < CabinNum; i++)
//	{
//		MAT8 mt;
//		stringstream ss; ss << i+1;
//		mt.name = "Cabin mat8 id = " + ss.str();
//		mt.MID = i + 1;
//		mt.E1 = Cabin_list[i][CE1];
//		mt.E2 = Cabin_list[i][CE2];
//		mt.NU12 = Cabin_list[i][CNu12];
//		mt.G12 = Cabin_list[i][CG12];
//		mt.G1Z = Cabin_list[i][CG1Z];
//		mt.G2Z = Cabin_list[i][CG2Z];
//		mt.RHO = Cabin_list[i][CRho];
//		meterial8_list.push_back(mt);
//	}
//	for (int i = 0; i < bh_list.size(); i++)
//	{
//		PBARL pb;
//		stringstream ss; ss << i+1;
//		pb.name = "bulkhead property id = " + ss.str();
//		pb.PID = i + 1;
//		pb.MID = CabinNum + i + 1;
//		pb.TYPE = "CHAN";//pb.TYPE = "I";
//		pb.DIM.resize(4);
//		pb.DIM[0] = bh_list[i][BD1]; pb.DIM[1] = bh_list[i][BD2]; pb.DIM[2] = bh_list[i][BD3]; 
//		pb.DIM[3] = bh_list[i][BD4]; //pb.DIM[4] = bh_list[i][BD5]; pb.DIM[5] = bh_list[i][BD6]; 
//		property_list.push_back(pb);
//		MAT1 mt;
//		mt.name = "bulkhead mat1 id = " + ss.str();
//		mt.MID = CabinNum + i + 1;
//		mt.E = bh_list[i][BE];
//		mt.RHO = bh_list[i][BRho];
//		mt.Nu = bh_list[i][BNu];
//		meterial_list.push_back(mt);
//	}
//
//	//发动机推力设置
//	ConcentratedForce EngineF;
//	if (nowEngine < 0 || nowEngine > 3)		nowEngine = 0;
//	if (nowEngine > 0)
//	{
//		EngineF.force = Point(Engin_list[nowEngine-1][EFx],Engin_list[nowEngine-1][EFy],Engin_list[nowEngine-1][EFz]);
//		EngineF.origin = Point(Engin_list[nowEngine-1][EXf],Engin_list[nowEngine-1][EYf],Engin_list[nowEngine-1][EZf]);
//		if (EngineF.origin.getX() < 1e-3)
//		{
//			EngineF.origin.SetSite(Engin_list[nowEngine-1][EXc]);
//		}
//	}
//	else
//	{
//		EngineF.force = Point(0,0,0);
//		EngineF.origin = Point(0,0,0);
//	}
//
//	for (int i = 0; i < aero.CP_vec.size(); i++)
//	{
//
//		//-----------------------参数设置-----------------------
//		SetShapeFunc(initinfo.ShapeFuncList);
//		SetCabin(cabinlist);
//		SetMassPoint(mp, 2);//
//		SetStruct(beamPID);
//		SetMeshSkin(FaiNum,ifUseCabinToCalcXsite);
//		SetBeamProperty(property_list,meterial_list,meterial8_list);
//		SetForce(aero.CP_vec[i], EngineF);
//
//		cout << "正在进行第" << i+1 << "个工况的计算" << endl;
//		cout << "[   Alp = " << aero.CP_vec[i].alpha << "  Ma = " << aero.CP_vec[i].mach << "   ]" << endl;
//		//参数计算
//		CalcModel();
//		//结果展示阶段
//		vector<CabinInfo> cb_list_res = GetCabinList();
//		Mesh* mesh_res = GetMesh();
//		stringstream ss; ss << i+1;
//		string filepath = path_list[PBdf];
//		string filename = path_list[PNam] + "_" + ss.str();
//		PrintMesh(filepath + "\\" + filename, 101);
//		cout << "网格生成完成" << endl;
//		double mess = GetStructMass();
//
//
//		//调用nastran
//		myNastran nas;
//		nas.NastranPath = path_list[PNas];
//		nas.BDFPath = filepath + "\\" + filename + ".bdf";
//		nas.WorkPath = filepath;
//		nas.CalcFilePath();
//		int ifNastranRun = nas.NastranCalc();
//		if (ifNastranRun < 0)
//		{
//			cout << "Nastran计算未能正常进行" << endl;
//			//system("pause");
//			return -2;
//		}
//		//给出单元属于哪个舱段
//		vector<int> CQUAD4_id;
//		for (int j = 0; j < cb_list_res.size(); j++)
//		{
//			CQUAD4_id.push_back(cb_list_res[j].cabinPID0);//点对应的舱段和单元编号对应舱段基本相似
//		}
//		vector<int> bhBAR_id;
//		for (int j = 0; j < nowBHnum; j++)
//		{
//			bhBAR_id.push_back(mesh_res->E[0].size() + j * FaiNum);
//		}
//		if (nas.ReadResPCH(CQUAD4_id,bhBAR_id) < 0)
//		{
//			return -1;
//		}
//		//
//		ofs << "StructMass = " << GetStructMass() << endl;
//		ofs << "[  " << "Alp = " << aero.CP_vec[i].alpha << "  Ma = " << aero.CP_vec[i].mach << "  ]" << endl;
//		for (int j = 0; j < cb_list_res.size(); j++)
//		{
//			//ofs << "舱段" << j + 1 << "最大工况输出：" << endl;
//			ofs << "Cabin" << j+1 << "_MaxStress     = " << nas.GetMaxStress()[j] << endl;
//			Point maxStress =(mesh_res->P[mesh_res->E[0][nas.GetMaxStressID()[j]][0]] + 
//				mesh_res->P[mesh_res->E[0][nas.GetMaxStressID()[j]][1]] +
//				mesh_res->P[mesh_res->E[0][nas.GetMaxStressID()[j]][2]] +
//				mesh_res->P[mesh_res->E[0][nas.GetMaxStressID()[j]][3]]) / 4.0;
//			ofs << "Cabin" << j+1 << "_MaxStressSite = (" << maxStress.getX() << ", " << maxStress.getY() << ", " << maxStress.getZ() << ")" << endl;
//
//			ofs << "Cabin" << j+1 << "_MaxStrain     = " << nas.GetMaxStrain()[j] << endl;
//			Point maxStrain =(mesh_res->P[mesh_res->E[0][nas.GetMaxStrainID()[j]][0]] + 
//				mesh_res->P[mesh_res->E[0][nas.GetMaxStrainID()[j]][1]] +
//				mesh_res->P[mesh_res->E[0][nas.GetMaxStrainID()[j]][2]] +
//				mesh_res->P[mesh_res->E[0][nas.GetMaxStrainID()[j]][3]]) / 4.0;
//			ofs << "Cabin" << j+1 << "_MaxStrainSite = (" << maxStrain.getX() << ", " << maxStrain.getY() << ", " << maxStrain.getZ() << ")" << endl;
//		}
//		//------------------------需要隔框时请取消这部分注释-----------------------------------
//		ofs << endl;
//		for (int j = 0; j < nas.GetMaxStressBar().size(); j++)
//		{
//			//ofs << "隔框" << j + 1 << "最大工况输出：" << endl;
//			ofs << "BH" << j+1 << "_MaxStress     = " << nas.GetMaxStressBar()[j] << endl;
//			Point maxStress = (mesh_res->P[mesh_res->E[1][nas.GetMaxStressIDBar()[j]-mesh_res->E[0].size()][0]] +
//				mesh_res->P[mesh_res->E[1][nas.GetMaxStressIDBar()[j]-mesh_res->E[0].size()][1]]) / 2.0;
//			ofs << "BH" << j+1 << "_MaxStressSite = (" << maxStress.getX() << ", " << maxStress.getY() << ", " << maxStress.getZ() << ")" << endl;
//
//			ofs << "BH" << j+1 << "_MaxStrain     = " << nas.GetMaxStrainBar()[j] << endl;
//			Point maxStrain = (mesh_res->P[mesh_res->E[1][nas.GetMaxStrainIDBar()[j]-mesh_res->E[0].size()][0]] +
//				mesh_res->P[mesh_res->E[1][nas.GetMaxStrainIDBar()[j]-mesh_res->E[0].size()][1]]) / 2.0;
//			ofs << "BH" << j+1 << "_MaxStrainSite = (" << maxStrain.getX() << ", " << maxStrain.getY() << ", " << maxStrain.getZ() << ")" << endl;
//		}
//		//------------------------需要隔框时请取消这部分注释-----------------------------------
//	}
//
//	ofs.close();
//	return 0;
//}

int Minuteman3::ReadFromFile(const string& path)
{
	//----------------------------参数处理----------------------------
	const int massNum = 2;//质量块参数列表编号
	const int EnginNum = 3;//发动机参数列表编号
	const int CabinNum = 7;//舱段参数列表编号
	const int bhNum = 10;//隔框参数列表编号
	mt3info.mass_list.resize(massNum);
	mt3info.Engin_list.resize(EnginNum);
	mt3info.Cabin_list.resize(CabinNum);
	mt3info.bh_list.resize(bhNum);
	int state = mt3info.readFile(path);
	if (state < 0) return -1;
	calcInitInfo();
	ofstream ofs(mt3info.path_list.POut);
	ofs << std::left;
	for (int i = 0; i < aero.CP_vec.size(); i++)
	{
		initinfo.CPinfo = aero.CP_vec[i];
		//-----------------------参数设置-----------------------
		SetInitInfo(initinfo);

		cout << "正在进行第" << i+1 << "个工况的计算" << endl;
		cout << "[   Alp = " << aero.CP_vec[i].alpha << "  Ma = " << aero.CP_vec[i].mach << "   ]" << endl;
		//参数计算
		CalcModel();
		//结果展示阶段
		vector<CabinInfo> cb_list_res = GetCabinList();
		Mesh mesh_res = GetMesh();
		string filepath = mt3info.path_list.PBdf;
		string filename = mt3info.path_list.PNam + "_" + to_string(i + 1);
		cout << "当前求解类型为：" << mt3info.SOLtype << endl;
		int ifBDFout = PrintMesh(filepath + "\\" + filename, mt3info.SOLtype);
		if (ifBDFout < 0)
		{
			return -3;
		}
		cout << "网格生成完成" << endl;

		//调用nastran
		myNastran nas;
		nas.NastranPath = mt3info.path_list.PNas;
		nas.BDFPath = filepath + "\\" + filename + ".bdf";
		nas.WorkPath = filepath;
		nas.CalcFilePath();
		int ifNastranRun = nas.NastranCalc();
		if (ifNastranRun < 0)
		{
			cout << "Nastran计算未能正常进行" << endl;
			//system("pause");
			return -2;
		}
		if (mt3info.SOLtype != 101 && mt3info.SOLtype != 200)//101和200支持输出res文件
		{
			return mt3info.SOLtype;
		}
		//给出单元属于哪个舱段
		vector<int> CQUAD4_id(cb_list_res.size());
		for (int j = 0; j < CQUAD4_id.size(); j++)
		{
			CQUAD4_id[j] = cb_list_res[j].cabinPID0;//点对应的舱段和单元编号对应舱段基本相似
		}
		vector<int> bhBAR_id(mt3info.nowBHnum);
		for (int j = 0; j < mt3info.nowBHnum; j++)
		{
			bhBAR_id[j] = mesh_res.E[0].size() + j * initinfo.faiNum;
		}
		if (nas.ReadResPCH(CQUAD4_id,bhBAR_id) < 0)
		{//如果读取失败则退出
			return -1;
		}
		//
		
		stringstream ss_t, ss_1, ss_2,ss_3, ss_4;
		ss_t << std::left;
		ss_1 << std::left;
		ss_2 << std::left;
		ss_3 << std::left;
		ss_4 << std::left;
		auto out = [](const string &name, const string &type, const string &value, stringstream &ss){
			const int titlenum = 24;
			const int tpyenum = 17;
			ss << setw(titlenum) << name << setw(tpyenum) << type << value << "\n";
		};
		if (mt3info.SOLtype == 200)
		{
			const vector<PSHELL> ps = nas.GetPSHELLlist();
			int state = updateCabinListT(ps);//优化后更新舱段厚度
			if (state < 0) return -200;
			for (size_t j = 0; j < nas.GetPSHELLlist().size(); j++)
			{
				out("cabin_" + to_string(ps[j].PID) + "_T", "double", to_string(ps[j].T),ss_t);
			}
		}
		out("StructMass", "double", to_string(GetStructMass()), ss_t);
		out("Alp", "double", to_string(aero.CP_vec[i].alpha), ss_t);
		out("Ma", "double", to_string(aero.CP_vec[i].mach), ss_t);

		for (int j = 0; j < cb_list_res.size(); j++)
		{
			//ofs << "舱段" << j + 1 << "最大工况输出：" << endl;
			out("Cabin" + to_string(j + 1) + "_MaxStress", "double", to_string(nas.GetMaxStress()[j]), ss_1);
			const Point maxStress =(
				mesh_res.P[mesh_res.E[0][nas.GetMaxStressID()[j]][0]] + 
				mesh_res.P[mesh_res.E[0][nas.GetMaxStressID()[j]][1]] +
				mesh_res.P[mesh_res.E[0][nas.GetMaxStressID()[j]][2]] +
				mesh_res.P[mesh_res.E[0][nas.GetMaxStressID()[j]][3]]) / 4.0;
			const string str_res = to_string(maxStress.getX()) + "," + to_string(maxStress.getY()) + "," + to_string(maxStress.getZ());
			out("Cabin" + to_string(j + 1) + "_MaxStressSite", "double_vector", str_res, ss_2);

			out("Cabin" + to_string(j + 1) + "_MaxStrain", "double", to_string(nas.GetMaxStrain()[j]), ss_3);
			const Point maxStrain =(
				mesh_res.P[mesh_res.E[0][nas.GetMaxStrainID()[j]][0]] +
				mesh_res.P[mesh_res.E[0][nas.GetMaxStrainID()[j]][1]] +
				mesh_res.P[mesh_res.E[0][nas.GetMaxStrainID()[j]][2]] +
				mesh_res.P[mesh_res.E[0][nas.GetMaxStrainID()[j]][3]]) / 4.0;
			const string str_res2 = to_string(maxStrain.getX()) + "," + to_string(maxStrain.getY()) + "," + to_string(maxStrain.getZ());
			out("Cabin" + to_string(j + 1) + "_MaxStrainSite", "double_vector", str_res2, ss_4);
		}
		ofs << ss_t.str() << ss_1.str()  << ss_2.str()  << ss_3.str()  << ss_4.str();
		////------------------------需要隔框时请取消这部分注释-----------------------------------
		//ofs << endl;
		//for (int j = 0; j < nas.GetMaxStressBar().size(); j++)
		//{
		//	//ofs << "隔框" << j + 1 << "最大工况输出：" << endl;
		//	ofs << "BH" << j+1 << "_MaxStress     = " << nas.GetMaxStressBar()[j] << endl;
		//	Point maxStress = (mesh_res->P[mesh_res->E[1][nas.GetMaxStressIDBar()[j]-mesh_res->E[0].size()][0]] +
		//		mesh_res->P[mesh_res->E[1][nas.GetMaxStressIDBar()[j]-mesh_res->E[0].size()][1]]) / 2.0;
		//	ofs << "BH" << j+1 << "_MaxStressSite = (" << maxStress.getX() << ", " << maxStress.getY() << ", " << maxStress.getZ() << ")" << endl;

		//	ofs << "BH" << j+1 << "_MaxStrain     = " << nas.GetMaxStrainBar()[j] << endl;
		//	Point maxStrain = (mesh_res->P[mesh_res->E[1][nas.GetMaxStrainIDBar()[j]-mesh_res->E[0].size()][0]] +
		//		mesh_res->P[mesh_res->E[1][nas.GetMaxStrainIDBar()[j]-mesh_res->E[0].size()][1]]) / 2.0;
		//	ofs << "BH" << j+1 << "_MaxStrainSite = (" << maxStrain.getX() << ", " << maxStrain.getY() << ", " << maxStrain.getZ() << ")" << endl;
		//}
		////------------------------需要隔框时请取消这部分注释-----------------------------------
	}

	ofs.close();
	return 0;
}

int Minuteman3::calcInitInfo()
{

	//----------------------------参数设置----------------------------
	//定义外形曲线
	const int HeadPrecision = 300;//头部网格精细度
	//冯卡门曲线函数
	auto phi = [](double x, double L){
		return acos(1 - 2 * x / L);
	};
	auto r = [](double Rd, double phi){
		return Rd / sqrt(pi) * sqrt(phi - 1.0 / 2.0 * sin(2 * phi));
	};
	
	initinfo.ShapeFuncList.resize(HeadPrecision);
	for (int i = 0; i < HeadPrecision; i++)
	{
		const double Rd = mt3info.Cabin_list[0].CRadius;
		const double L = mt3info.Cabin_list[0].CSite;
		const double x = L * (double)i / (double)(HeadPrecision - 1);
		const double rs = r(Rd, phi(x, L));
		initinfo.ShapeFuncList[i] = Point(x, rs);
	}
	initinfo.ShapeFuncList.reserve(initinfo.ShapeFuncList.size() + mt3info.nowCabinNum);
	for (int i = 1; i < mt3info.nowCabinNum; i++)
	{
		initinfo.ShapeFuncList.push_back(Point(mt3info.Cabin_list[i].CSite, mt3info.Cabin_list[i].CRadius));
	}
	//定义周向坐标点、梁数目
	initinfo.faiNum =(int)(initinfo.faiNum * mt3info.ElemNumRatio);	//定义周向坐标点数
	initinfo.beamPID = {};
	//定义集中质量（质量、质心、名称（可缺省））
	initinfo.mpinit.resize(0);
	for (int i = 0; i < mt3info.mass_list.size(); i++)
	{
		MassPointInitInfo temp;
		temp.mass = mt3info.mass_list[i].MM;
		temp.site = Point(mt3info.mass_list[i].MX, mt3info.mass_list[i].MY, mt3info.mass_list[i].MZ);
		temp.name = "noname";
		temp.type = MassType::Device;
		initinfo.mpinit.push_back(temp);
	}
	for (int i = 0; i < mt3info.Engin_list.size(); i++)
	{
		MassPointInitInfo temp;
		temp.mass = mt3info.Engin_list[i].EM;
		temp.site = Point(mt3info.Engin_list[i].EXc, mt3info.Engin_list[i].EYc, mt3info.Engin_list[i].EZc);
		temp.name = "noname";
		temp.type = MassType::Engine;
		initinfo.mpinit.push_back(temp);
	}
	//定义舱段信息（ x1,  MID,  T, extramass, siteNum = 1,PBARL_list ）
	vector<int> siteNum(7);
	siteNum[0] = 100; siteNum[1] = 50; siteNum[2] = 20; siteNum[3] = 60; siteNum[4] = 20; siteNum[5] = 200; siteNum[6] = 20;
	for (int i = 0; i < siteNum.size(); i++) siteNum[i] = (int)(siteNum[i]*mt3info.ElemNumRatio);

	vector<vector<int>> PBARL_LIST(7);//隔框数据 这里是定死的7个舱段
	PBARL_LIST[0].push_back(1);	PBARL_LIST[0].push_back(1);//第一个隔框在零点位置 不会输出
	PBARL_LIST[1].push_back(2);
	PBARL_LIST[2].push_back(3);	PBARL_LIST[2].push_back(4);
	PBARL_LIST[3].push_back(5);
	PBARL_LIST[4].push_back(6);	PBARL_LIST[4].push_back(7);
	PBARL_LIST[5].push_back(8);
	PBARL_LIST[6].push_back(9);	PBARL_LIST[6].push_back(10);
	initinfo.cbinit.resize(mt3info.nowCabinNum);
	for (int i = 0; i < mt3info.nowCabinNum; i++)//设置舱段信息
	{
		initinfo.cbinit[i].x1 = mt3info.Cabin_list[i].CSite;
		initinfo.cbinit[i].MID = i + 1;
		initinfo.cbinit[i].T = mt3info.Cabin_list[i].CT;
		initinfo.cbinit[i].extramass = 0;
		initinfo.cbinit[i].siteNum = siteNum[i];
		initinfo.cbinit[i].bhlist = PBARL_LIST[i];
		initinfo.cbinit[i].t_max = mt3info.Cabin_list[i].t_max;//SOL200
		initinfo.cbinit[i].t_min = mt3info.Cabin_list[i].t_min;//SOL200
	}

	bool ifUseCabinToCalcXsite = true;//使用舱段初始化信息计算轴向坐标点数
	//气动力设置
	
	aero.readFile(mt3info.path_list.PAer);
	if (aero.CP_vec.size() == 0)
	{
		cout << "当前未能读取到正确的气动力数据！分析终止" << endl;
		//system("pause");
		return -1;
	}
	ofstream ofs(mt3info.path_list.POut);

	//定义隔框、横梁的截面属性和材料信息
	//vector<PBARL> property_list;
	//vector<MAT1> meterial_list;
	//vector<MAT8> meterial8_list;
	for (int i = 0; i < mt3info.Cabin_list.size(); i++)
	{
		MAT8 mt;
		stringstream ss; ss << i + 1;
		mt.name = "Cabin mat8 id = " + ss.str();
		mt.MID = i + 1;
		mt.E1 = mt3info.Cabin_list[i].CE1;
		mt.E2 = mt3info.Cabin_list[i].CE2;
		mt.NU12 = mt3info.Cabin_list[i].CNu12;
		mt.G12 = mt3info.Cabin_list[i].CG12;
		mt.G1Z = mt3info.Cabin_list[i].CG1Z;
		mt.G2Z = mt3info.Cabin_list[i].CG2Z;
		mt.RHO = mt3info.Cabin_list[i].CRho;
		mt.stress_max = mt3info.Cabin_list[i].stress_max;//SOL200
		mt.stress_min = mt3info.Cabin_list[i].stress_min;//SOL200
		initinfo.meterial8_list.push_back(mt);
	}
	for (int i = 0; i < mt3info.bh_list.size(); i++)
	{
		PBARL pb;
		stringstream ss; ss << i + 1;
		pb.name = "bulkhead property id = " + ss.str();
		pb.PID = i + 1;
		pb.MID = mt3info.Cabin_list.size() + i + 1;
		pb.TYPE = "CHAN";//pb.TYPE = "I";
		pb.DIM.resize(4);
		pb.DIM[0] = mt3info.bh_list[i].BD1; pb.DIM[1] = mt3info.bh_list[i].BD2; pb.DIM[2] = mt3info.bh_list[i].BD3;
		pb.DIM[3] = mt3info.bh_list[i].BD4; //pb.DIM[4] = bh_list[i].BD5]; pb.DIM[5] = bh_list[i].BD6]; 
		initinfo.property_list.push_back(pb);
		MAT1 mt;
		mt.name = "bulkhead mat1 id = " + ss.str();
		mt.MID = mt3info.Cabin_list.size() + i + 1;
		mt.E = mt3info.bh_list[i].BE;
		mt.RHO = mt3info.bh_list[i].BRho;
		mt.Nu = mt3info.bh_list[i].BNu;
		initinfo.meterial_list.push_back(mt);
	}

	//发动机推力设置
	//ConcentratedForce EngineF;
	if (mt3info.nowEngine < 0 || mt3info.nowEngine > 3)
	{
		cout << "当前发动机参数设置无效 nowEngine：" << mt3info.nowEngine << "，以默认设为0" << endl;
		mt3info.nowEngine = 0;
	}
	if (mt3info.nowEngine > 0)
	{
		initinfo.EngineForce.force = Point(
			mt3info.Engin_list[mt3info.nowEngine - 1].EFx, 
			mt3info.Engin_list[mt3info.nowEngine - 1].EFy, 
			mt3info.Engin_list[mt3info.nowEngine - 1].EFz);
		initinfo.EngineForce.origin = Point(
			mt3info.Engin_list[mt3info.nowEngine - 1].EXf, 
			mt3info.Engin_list[mt3info.nowEngine - 1].EYf, 
			mt3info.Engin_list[mt3info.nowEngine - 1].EZf);
		if (initinfo.EngineForce.origin.getX() < 1e-3)
		{
			initinfo.EngineForce.origin.SetSite(mt3info.Engin_list[mt3info.nowEngine - 1].EXc);
		}
	}
	else//=0表示添加推力
	{
		initinfo.EngineForce.force = Point(0, 0, 0);
		initinfo.EngineForce.origin = Point(0, 0, 0);
	}
	initinfo.Modal_order = mt3info.Modal_order;//SOL103
	return 0;
}

int Minuteman3Info::readFile(const string& path)
{
	cout << "输入文件：" << path << endl;
	LoadInput inp(path);
	int state = inp.readFile();
	if (state < 0)
	{
		cout << "打开失败，文件不存在！" << endl;
		return -1;
	}
	else
	{
		cout << "打开成功！" << endl;
	}

	inp.update("NastranPath_",	this->path_list.PNas);
	inp.update("ModlePath_",	this->path_list.PBdf);
	inp.update("ModleName_",	this->path_list.PNam);
	inp.update("OutputPath_",	this->path_list.POut);
	inp.update("AeroPath_",		this->path_list.PAer);
	inp.update("NowCase_",		this->nowCase);
	inp.update("SOLtype_",		this->SOLtype);
	inp.update("NowEngine_",	this->nowEngine);
	inp.update("ElemNumRatio", this->ElemNumRatio);

	for (size_t id = 0; id < mass_list.size();id++)//设备质量参数
	{
		const string str_id = "part_" + to_string(id + 1) + "_";
		inp.update(str_id + "mass", this->mass_list[id].MM);
		inp.update(str_id + "xc",	this->mass_list[id].MX);
		inp.update(str_id + "yc",	this->mass_list[id].MY);
		inp.update(str_id + "zc",	this->mass_list[id].MZ);
	}
	for (size_t id = 0; id < Engin_list.size(); id++)//发动机参数
	{
		const string str_id = "engine_" + to_string(id + 1) + "_";
		inp.update(str_id + "mass", this->Engin_list[id].EM);
		inp.update(str_id + "xc",	this->Engin_list[id].EXc);
		inp.update(str_id + "yc",	this->Engin_list[id].EYc);
		inp.update(str_id + "zc",	this->Engin_list[id].EZc);
		inp.update(str_id + "Fsx",	this->Engin_list[id].EXf);
		inp.update(str_id + "Fsy",	this->Engin_list[id].EYf);
		inp.update(str_id + "Fsz",	this->Engin_list[id].EZf);
		inp.update(str_id + "Fx",	this->Engin_list[id].EFx);
		inp.update(str_id + "Fy",	this->Engin_list[id].EFy);
		inp.update(str_id + "Fz",	this->Engin_list[id].EFz);
	}
	for (size_t id = 0; id < Cabin_list.size(); id++)//舱段参数
	{
		const string str_id = "cabin_" + to_string(id + 1) + "_";
		inp.update(str_id + "length", this->Cabin_list[id].CLength);
		// this->Cabin_list[id].CLength /= 2.0;
		inp.update(str_id + "radius", this->Cabin_list[id].CRadius);
		inp.update(str_id + "T", this->Cabin_list[id].CT);
		inp.update(str_id + "E1", this->Cabin_list[id].CE1);
		inp.update(str_id + "E2", this->Cabin_list[id].CE2);
		inp.update(str_id + "NU12", this->Cabin_list[id].CNu12);
		inp.update(str_id + "G12", this->Cabin_list[id].CG12);
		inp.update(str_id + "G1Z", this->Cabin_list[id].CG1Z);
		inp.update(str_id + "G2Z", this->Cabin_list[id].CG2Z);
		inp.update(str_id + "rho", this->Cabin_list[id].CRho);
	
	}
	for (size_t id = 0; id < bh_list.size(); id++)//隔框参数
	{
		const string str_id = "bh_" + to_string(id + 1) + "_";
		inp.update(str_id + "E",	this->bh_list[id].BE);
		inp.update(str_id + "rho",	this->bh_list[id].BRho);
		inp.update(str_id + "nu",	this->bh_list[id].BNu);
		inp.update(str_id + "dim1", this->bh_list[id].BD1);
		inp.update(str_id + "dim2", this->bh_list[id].BD2);
		inp.update(str_id + "dim3", this->bh_list[id].BD3);
		inp.update(str_id + "dim4", this->bh_list[id].BD4);
	}
	switch (SOLtype)//特定分析需要的参数
	{
	case 103:
		inp.update("Modal_order", this->Modal_order);								//模态阶数
		break;
	case 200:
		for (size_t id = 0; id < Cabin_list.size(); id++)//舱段参数
		{
			const string str_id = "cabin_" + to_string(id + 1) + "_";
			inp.update(str_id + "T_min", this->Cabin_list[id].t_min);				//舱段厚度最小值
			inp.update(str_id + "T_max", this->Cabin_list[id].t_max);				//舱段厚度最大值
			inp.update(str_id + "stress_min", this->Cabin_list[id].stress_min);		//舱段材料应力最小值
			inp.update(str_id + "stress_max", this->Cabin_list[id].stress_max);		//舱段材料应力最大值
		}
		break;
	}

	switch (nowCase)
	{
	case 1: nowCabinNum = 7;	nowBHnum = 10;		break;
	case 2: nowCabinNum = 5 - 1;	nowBHnum = 7 - 2;		break;
	case 3: nowCabinNum = 3 - 1;	nowBHnum = 4 - 2;		break;
	case 4: nowCabinNum = 1;	nowBHnum = 1;		break;
	default: cout << "NowCase_参数错误！程序退出" << endl; return-1;// nowCabinNum = Cabin_list.size(); nowBHnum = 10;
	}
	double cabin_site_x = 0;
	for (size_t i = 0; i < Cabin_list.size(); i++)
	{
		cabin_site_x += Cabin_list[i].CLength;
		Cabin_list[i].CSite = cabin_site_x;
	}
	//输入假想隔框的属性
	for (size_t id = 0; id < bh_list.size(); id++)
	{
		bh_list[id].BE = 2.1e+011;
		bh_list[id].BRho = 2.7e+003;
		bh_list[id].BNu = 0.3;
		bh_list[id].BD1 = 0.08;
		bh_list[id].BD2 = 0.064;
		bh_list[id].BD3 = 0.064;
		bh_list[id].BD4 = 0.024;
	}
	return 0;
}


int TestTube::ReadFromFile(const string &path)
{
	//
	Minuteman3Info mt3info;
	const int massNum = 2;//质量块参数列表编号
	const int EnginNum = 3;//发动机参数列表编号
	const int CabinNum = 7;//舱段参数列表编号
	const int bhNum = 10;//隔框参数列表编号
	mt3info.mass_list.resize(massNum);
	mt3info.Engin_list.resize(EnginNum);
	mt3info.Cabin_list.resize(CabinNum);
	mt3info.bh_list.resize(bhNum);
	int state = mt3info.readFile(path);
	if (state < 0) return -1;
	//
	InitInfo initinfo;
	//形状函数
	initinfo.ShapeFuncList = {
		Point(0, 3),
		Point(10, 4),
	};
	//集中质量点
	initinfo.mpinit.resize(0);
	//舱段信息
	const double Ratio = 8;
	CabinInitInfo cabininfo;
	cabininfo.x1 = 10;//结束位置
	cabininfo.MID = 1;//材料属性编号
	cabininfo.T = 0.1;//壳体厚度
	cabininfo.siteNum = (int)(30 * Ratio); //舱段轴向网格数
	cabininfo.extramass = 0;//额外质量
	cabininfo.bhlist = {};//隔框数量和材料属性
	initinfo.cbinit = {
		cabininfo
	};
	//横梁信息
	initinfo.beamPID = {};
	//发动机推力参数
	initinfo.EngineForce.force = Point(1, 0, 0);
	initinfo.EngineForce.origin = Point(0, 0, 0);
	//
	initinfo.CPinfo.mach = 0;
	initinfo.CPinfo.alpha = 0;
	//材料属性设置
	MAT8 mat8;
	mat8.MID = 1;
	mat8.E1 *= 10;
	mat8.E2 *= 10;
	initinfo.meterial8_list = {
		mat8,
	};
	for (int i = 0; i < mt3info.bh_list.size(); i++)
	{
		PBARL pb;
		stringstream ss; ss << i + 1;
		pb.name = "bulkhead property id = " + ss.str();
		pb.PID = i + 1;
		pb.MID = mt3info.Cabin_list.size() + i + 1;
		pb.TYPE = "CHAN";//pb.TYPE = "I";
		pb.DIM.resize(4);
		pb.DIM[0] = mt3info.bh_list[i].BD1; pb.DIM[1] = mt3info.bh_list[i].BD2; pb.DIM[2] = mt3info.bh_list[i].BD3;
		pb.DIM[3] = mt3info.bh_list[i].BD4; //pb.DIM[4] = bh_list[i].BD5]; pb.DIM[5] = bh_list[i].BD6]; 
		initinfo.property_list.push_back(pb);
		MAT1 mt;
		mt.name = "bulkhead mat1 id = " + ss.str();
		mt.MID = mt3info.Cabin_list.size() + i + 1;
		mt.E = mt3info.bh_list[i].BE;//*10
		mt.RHO = mt3info.bh_list[i].BRho;
		mt.Nu = mt3info.bh_list[i].BNu;
		initinfo.meterial_list.push_back(mt);
	}
	//
	initinfo.faiNum = (int)(initinfo.faiNum * Ratio); //定义周向坐标点数
	//
	ReadCP aero;
	aero.readFile(mt3info.path_list.PAer);
	if (aero.CP_vec.size() == 0)
	{
		cout << "当前未能读取到正确的气动力数据！分析终止" << endl;
		//system("pause");
		return -1;
	}
	SetInitInfo(initinfo);

	//cout << "正在进行第" << i+1 << "个工况的计算" << endl;
	//cout << "[   Alp = " << aero.CP_vec[i].alpha << "  Ma = " << aero.CP_vec[i].mach << "   ]" << endl;
	//参数计算
	CalcModel();
	//结果展示阶段
	vector<CabinInfo> cb_list_res = GetCabinList();
	Mesh mesh_res = GetMesh();
	string filepath = mt3info.path_list.PBdf;
	string filename = mt3info.path_list.PNam + "_" + to_string(1);
	cout << "当前求解类型为：" << mt3info.SOLtype << endl;
	int ifBDFout = PrintMesh(filepath + "\\" + filename, mt3info.SOLtype);
	if (ifBDFout < 0)
	{
		return -3;
	}
	cout << "网格生成完成" << endl;
	//调用nastran
	myNastran nas;
	nas.NastranPath = mt3info.path_list.PNas;
	nas.BDFPath = filepath + "\\" + filename + ".bdf";
	nas.WorkPath = filepath;
	nas.CalcFilePath();
	int ifNastranRun = nas.NastranCalc();
	if (ifNastranRun < 0)
	{
		cout << "Nastran计算未能正常进行" << endl;
		//system("pause");
		return -2;
	}
	return -1;
}