#include "LoadFile.h"
#include <io.h>
#include <fstream>
#include <Windows.h>


//int LoadFile::readTrajInfoFromPath(const string& path)
//{
//	if (_access(path.c_str(), 0) == -1)
//	{
//		cout << "输入的文件夹不存在" << endl;
//		return -1;
//	}
//
//	//0
//	vector<int> veclist0 = { 0,5 };
//	auto filedata0 = readSingleFile(path + "\\0. ts t V O Oo H L.txt", veclist0, true);
//	if (filedata0 == nullptr) return -2;
//	for (size_t i = 0; i < filedata0->size(); i++)
//	{
//		//trajInfoList[(*filedata0)[i][0]].ts = (*filedata0)[i][0];
//		//trajInfoList[(*filedata0)[i][0]].H = (*filedata0)[i][1];
//		trajInfoList[(*filedata0)[i][0]].h = (*filedata0)[i][1];
//	}
//	delete filedata0;	filedata0 = nullptr;
//
//	//1
//	vector<int> veclist1 = { 0,2,3,4 };
//	auto filedata1 = readSingleFile(path + "\\1. ts t Vx Vy Vz X Y Z.txt", veclist1, true);
//	if (filedata1 == nullptr) return -2;
//	for (size_t i = 0; i < filedata1->size(); i++)
//	{
//		//trajInfoList[(*filedata1)[i][0]].Vx = (*filedata1)[i][1];
//		//trajInfoList[(*filedata1)[i][0]].Vy = (*filedata1)[i][2];
//		//trajInfoList[(*filedata1)[i][0]].Vz = (*filedata1)[i][3];
//		trajInfoList[(*filedata1)[i][0]].v_bx = (*filedata1)[i][1];
//		trajInfoList[(*filedata1)[i][0]].v_by = (*filedata1)[i][2];
//		trajInfoList[(*filedata1)[i][0]].v_bz = (*filedata1)[i][3];
//	}
//	delete filedata1;	filedata1 = nullptr;
//
//	//7
//	vector<int> veclist7 = { 0,3,5 };
//	auto filedata7 = readSingleFile(path + "\\7. ts t r lm fie B.txt", veclist7, true);
//	if (filedata7 == nullptr) return -2;
//	for (size_t i = 0; i < filedata7->size(); i++)
//	{
//		//trajInfoList[(*filedata7)[i][0]].lm = (*filedata7)[i][1];
//		//trajInfoList[(*filedata7)[i][0]].B = (*filedata7)[i][2];
//		trajInfoList[(*filedata7)[i][0]].lon = (*filedata7)[i][1];
//		trajInfoList[(*filedata7)[i][0]].lat = (*filedata7)[i][2];
//	}
//	delete filedata7;	filedata7 = nullptr;
//
//	//22
//	vector<int> veclist22 = { 0,2,3,4 };
//	auto filedata22 = readSingleFile(path + "\\22. ts t fipr pspr gmapr fi ps gma alf bt.txt", veclist22, true);
//	if (filedata22 == nullptr) return -2;
//	for (size_t i = 0; i < filedata22->size(); i++)
//	{
//		//trajInfoList[(*filedata22)[i][0]].fipr = (*filedata22)[i][1];
//		//trajInfoList[(*filedata22)[i][0]].pspr = (*filedata22)[i][2];
//		//trajInfoList[(*filedata22)[i][0]].gmapr = (*filedata22)[i][3];
//		trajInfoList[(*filedata22)[i][0]].phi.setValue((*filedata22)[i][1]);
//		trajInfoList[(*filedata22)[i][0]].psi.setValue((*filedata22)[i][2]);
//		trajInfoList[(*filedata22)[i][0]].gamma.setValue((*filedata22)[i][3]);
//	}
//	delete filedata22;	filedata22 = nullptr;
//
//	//26
//	vector<int> veclist26 = { 0,2,3,4 };
//	auto filedata26 = readSingleFile(path + "\\26. ts t fil psl gmal.txt", veclist26, true);
//	if (filedata26 == nullptr) return -2;
//	for (size_t i = 0; i < filedata26->size(); i++)
//	{
//		//trajInfoList[(*filedata26)[i][0]].fil = (*filedata26)[i][1];
//		//trajInfoList[(*filedata26)[i][0]].psl = (*filedata26)[i][2];
//		//trajInfoList[(*filedata26)[i][0]].gmal = (*filedata26)[i][3];
//		trajInfoList[(*filedata26)[i][0]].p.setValue((*filedata26)[i][1]);
//		trajInfoList[(*filedata26)[i][0]].q.setValue((*filedata26)[i][2]);
//		trajInfoList[(*filedata26)[i][0]].r.setValue((*filedata26)[i][3]);
//	}
//	delete filedata26;	filedata26 = nullptr;
//
//	return 0;
//}
//
//
//
//InitInfo LoadFile::getInitByTime(const double& time)
//{
//	if (trajInfoList.size() < 2)
//	{
//		cout << "读取弹道数据可能有误，将使用默认参数计算\n";
//		return InitInfo();
//	}
//	map<double, InitInfo>::iterator it_0 = trajInfoList.begin();
//	double t0 = it_0->first;
//	it_0++;
//	if (t0 == time)
//	{
//		cout << "读取弹道参数成功（time = " << t0 << "s）" << endl;
//		return it_0->second;
//	}
//	for (map<double,InitInfo>::iterator it = it_0;it!=trajInfoList.end();it++)
//	{
//		double t1 = it->first;
//		InitInfo& info = it->second;
//		if (t0 < time&&time <= t1)
//		{
//			cout << "读取弹道参数成功（time = " << t1 << "s）" << endl;
//			return it->second;
//		}
//		else
//		{
//			t0 = it->first;
//		}
//	}
//	cout << "读取弹道数据输入的时间参数（time = " << time << "s）已越界，将使用默认参数计算\n";
//	return InitInfo();
//}
//
//InitInfo LoadFile::getInitByHight(const double & h)
//{
//	if (trajInfoList.size() < 2)
//	{
//		cout << "读取弹道数据可能有误，将使用默认参数计算\n";
//		return InitInfo();
//	}
//	map<double, InitInfo>::iterator it_0 = trajInfoList.begin();
//	double h0 = it_0->second.h;
//	it_0++;
//	if (h0 == h)
//	{
//		cout << "读取弹道参数成功（hight = " << h << "m）" << endl;
//		return it_0->second;
//	}
//	for (map<double, InitInfo>::iterator it = it_0; it != trajInfoList.end(); it++)
//	{
//		double h1 = it->second.h;
//		InitInfo& info = it->second;
//		if (h0 < h&&h <= h1)
//		{
//			cout << "读取弹道参数成功（time = " << h1 << "m）" << endl;
//			return it->second;
//		}
//		else
//		{
//			h0 = it->second.h;
//		}
//	}
//	cout << "读取弹道数据输入的时间参数（time = " << h << "m）已越界，将使用默认参数计算\n";
//	return InitInfo();
//}
//
//vector<vector<double>>* LoadFile::readSingleFile(string path, vector<int> usfulCol, bool ifprintstate)
//{
//	ifstream ifs(path);
//	if (ifs.fail())
//	{
//		if (ifprintstate) cout << "打开失败 " << path << endl;
//		return nullptr;
//	}
//
//	vector<vector<double>>* data = new vector<vector<double>>;
//	string strLine;
//	while (getline(ifs, strLine))//读取每一行
//	{
//		vector<double> infolist(usfulCol.size());//记录每一行的数据
//		stringstream sstream(strLine);
//		int colID = 0;//文本文件内参数列数
//		int num = 0;//需要读取的参数id
//		double temp;
//		bool isnum = false;
//		while (sstream >> temp)//如果转换数字失败直接退出该行
//		{
//			if (colID == usfulCol[num])
//			{
//				infolist[num] = temp;
//				num++;
//				if (!(colID < usfulCol[usfulCol.size()-1]))//如果读取完最后一个数值则停止
//				{
//					break;
//				}
//			}
//			colID++;
//			isnum = true;
//		}
//		if (isnum) data->push_back(infolist);
//	}
//	if (ifprintstate) cout << "读取成功 " << path << endl;
//	return data;
//}

string GbkToUtf8(string strValue)
{
	int len = MultiByteToWideChar(CP_ACP, 0, strValue.c_str(), -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len + 1];
	memset(wstr, 0, len + 1);
	MultiByteToWideChar(CP_ACP, 0, strValue.c_str(), -1, wstr, len);
	len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* str = new char[len + 1];
	memset(str, 0, len + 1);
	WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, len, NULL, NULL);
	string strTemp = str;
	if (wstr) delete[] wstr;
	if (str) delete[] str;
	return strTemp;
}

string UtfToGbk(string strValue)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, strValue.c_str(), -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len + 1];
	memset(wstr, 0, len + 1);
	MultiByteToWideChar(CP_UTF8, 0, strValue.c_str(), -1, wstr, len);
	len = WideCharToMultiByte(CP_ACP, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* str = new char[len + 1];
	memset(str, 0, len + 1);
	WideCharToMultiByte(CP_ACP, 0, wstr, -1, str, len, NULL, NULL);
	if (wstr) delete[] wstr;
	return string(str);
}

int LoadInput::calcSuffix() 
{
	string suffix = m_path.substr(m_path.find_last_of('.') + 1, m_path.length() - 1);
	if (!suffix.compare("txt"))
	{
		m_type = Suffix::txt;
	}
	else if (!suffix.compare("ini"))
	{
		m_type = Suffix::ini;
	}
	else
	{
		cout << ("当前输入文件类型为：" + suffix + "，错误的输入文件类型/n");
		return -1;
	}
	return 0;
}

LoadInput::LoadInput(const string & path) :m_path(path)
{
	m_InfoList.clear();
}

int LoadInput::readFile()
{
	int state = calcSuffix();
	if (state < 0) return -1;
	//
	ifstream ifs(m_path);
	if (ifs.fail())
	{
		cout << ("打开失败 " + m_path + "\n");
		return -2;
	}
	m_InfoList.clear();

	string strLine;
	while (getline(ifs, strLine))//读取每一行
	{
		pair<string, string> temp("", "");
		stringstream ss(strLine);
		switch (m_type)
		{
		case LoadInput::Suffix::txt: 
			if (strLine[0] != '#')
			{
				string temp1;
				ss >> temp.first;
				ss >> temp1;
				ss >> temp.second;
			}
			break;
		case LoadInput::Suffix::ini:
			getline(ss, temp.first,'=');
			getline(ss, temp.second, '=');
			break;
		default:
			break;
		}
		if (temp.second.size() != 0)
		{
			//temp.second = UtfToGbk(temp.second);
			m_InfoList.insert(temp);
		}
	}
	return 0;
}
