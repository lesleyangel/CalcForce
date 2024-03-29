#include "Aero.h"
#include "Spline.h"
#include <memory>

void CP::CalcThetaList()
{
	//thetalist.resize(2 * (CPmatrix[0].size()-1) - 1);
	//for (int i = 0; i < thetalist.size(); i++)
	//{
	//	thetalist[i] = i * 2 * pi / (thetalist.size()-1);
	//}
	thetalist.resize(CPmatrix[0].size()-1);
	for (int i = 0; i < thetalist.size(); i++)
	{
		thetalist[i] = (double)i * pie / (thetalist.size()-1);
	}
}

void CP::CalcXList()
{
	xlist.resize(CPmatrix.size());
	for (int i = 0; i < xlist.size(); i++)
	{
		xlist[i] = CPmatrix[i][0];
	}
}

int CP::CalcData()
{
	if (CPmatrix.size() == 0)
	{
		return -1;
	}
	int xnum = CPmatrix.size();
	int theta_data_num = CPmatrix[0].size()-1;
	int thetanum = 2 * theta_data_num - 1;
	Datainfo.resize(xnum * theta_data_num);
	for (int i = 0; i < xnum; i++)
	{
		for (int j = 1; j < CPmatrix[i].size(); j++)
		{
			Datainfo[i*theta_data_num+j-1] = CPmatrix[i][j];
		}
	}
	CalcXList();
	CalcThetaList();
	// spline
	m_xlist = new double[xlist.size()];
	m_thetalist = new double[thetalist.size()];
	for (size_t i = 0; i < xlist.size(); i++)
	{
		m_xlist[i] = xlist[i];
	}
	for (size_t i = 0; i < thetalist.size(); i++)
	{
		m_thetalist[i] = thetalist[i];
	}
	m_data.resize(CPmatrix[0].size()-1);
	m_spline_list.resize(CPmatrix[0].size()-1);
	for (size_t i = 0; i < m_data.size(); i++)
	{
		m_data[i] = new double[CPmatrix.size()];
	}
	for (size_t i = 0; i < CPmatrix.size(); i++)
	{
		for (size_t j = 1; j < CPmatrix[i].size(); j++)
		{
			m_data[j-1][i] = CPmatrix[i][j];
		}
	}
	for (size_t i = 0; i < m_spline_list.size(); i++)
	{
		m_spline_list[i] = new Spline(m_xlist, m_data[i], CPmatrix.size());
	}
	

	return 0;
}
double CP::getP(double xsite, double theta)
{
	if (Datainfo.size() == 0)
	{
		if (CalcData()) return 0;
	}
	if (xlist.size() == 0)
	{
		CalcXList();
	}
	if (thetalist.size() == 0)
	{
		CalcThetaList();
	}

	xsite = (xsite < xlist[0]) ? xlist[0] : xsite;
	xsite = (xsite > *(xlist.end()-1)) ? *(xlist.end()-1) : xsite;

	//double res1 = Interpolation2(thetalist,xlist,Datainfo,theta,xsite);

	// Spline
	unique_ptr<double[]> res_theta(new double[thetalist.size()]);
	for (size_t i = 0; i < thetalist.size(); i++)
	{
		if (xsite < m_spline_list[i]->getMinX() || m_spline_list[i]->getMaxX() < xsite)
		{
			res_theta[i] = 0;
		}
		else
		{
			// try
			// {
				m_spline_list[i]->SinglePointInterp(xsite, res_theta[i]);
				// cout << res_theta[i] << endl;
				// }
				// catch (SplineFailure sf)
				// {
				// 	//reslist[i] = 0;
				// 	cout << sf.GetMessage() << endl;
				// }
		}
	}
	std::unique_ptr<SplineInterface> sp(new Spline(m_thetalist, res_theta.get(), thetalist.size())); //使用接口，且使用默认边界条件

	double res=0;
	if (theta < sp->getMinX() || sp->getMaxX() < theta)
	{
		res = 0;
	}
	else
	{
		// try
		// {
			sp->SinglePointInterp(theta, res);
		// }
		// catch (SplineFailure sf)
		// {
		// 	//resForce = 0;
		// 	cout << sf.GetMessage() << endl;
		// }
	}
	return res * q;
}

void ReadCP::readFile(const string _path,double q){

	vector<vector<string>> contents;
	vector<string> delimiter;
	initDelimiter(delimiter);
	this->readFilePerElement(_path, contents, delimiter);

	// double curMACH, curALPHA;
	int count=0, sz;
	for (int i = 1;i<contents.size();++i){
		if (contents[i].size()>1){
			//if (contents[i][0]=="MACH"){
			//	CP_vec.resize(count+1);
			//	CP_vec[count].alpha=atof(contents[i][3].c_str());
			//	CP_vec[count].mach=atof(contents[i][1].c_str());
			//	count+=1;

			//}
			if (contents[i][0] == "q") {
				CP_vec.resize(count + 1);
				//CP_vec[count].alpha = atof(contents[i][3].c_str());
				//CP_vec[count].q = atof(contents[i][1].c_str());
				CP_vec[count].q = q;
				count += 1;

			}
			else{
				sz=CP_vec[count-1].CPmatrix.size();
				for (int j=0;j<8;++j){
					CP_vec[count-1].CPmatrix.resize(sz+1);
					CP_vec[count - 1].CPmatrix[sz].push_back(atof(contents[i][j].c_str())); //考虑动压
				}

			}
		}
	}

	
}


void ReadCP::readFilePerElement(const string _filePath, vector<vector<string>>& _tokens, vector<string>& _delimiter)
{
	ifstream file(_filePath);
	string perLine; // 每一行信息
	vector<string> contents;
	// 一次性将 .dat 文件读取完
	while (std::getline(file, perLine)) {
		contents.push_back(perLine);
	}
	file.close();

	_tokens.resize(contents.size());
	for (int i = 0; i < contents.size(); ++i)
	{
		split(contents[i], _tokens[i], _delimiter);
	}

}


void ReadCP::split(const string& s,vector<string>& tokens,const vector<string>& delimiter){
	
	tokens.resize(0);
	int lastPos=0; // 第1个非分隔符字符位置 
	string::size_type pos=0; // 在lastPos之后的第1个分隔符位置
	bool isneq = true;
	string trist_a;


	for (int i = 0; i < s.size(); ++i){
		isneq = true;
		for (int j = 0; j < delimiter.size(); ++j){

			// 检查是否为中文
			if ((s[i]& 0x80) == false){
				trist_a = s[i];
				// 等同于 s.substr(i,1), 但这样写对并行更友好
			}
			else{
				trist_a = s[i];
				trist_a += s[i+1];
				i++;
			}

			// 确定是否为分隔符
			if (trist_a == delimiter[j]){
				isneq = false;
				break;
			}
		}

		// 如果第i个不为分隔符，跳出
		if (isneq == true){
			lastPos = i;
			break;
		}
	}
	



	vector<string::size_type> trist_Pos;


	
	trist_Pos.resize(delimiter.size());
	for (int i=0; i < delimiter.size();++i){
		trist_Pos[i]=s.find_first_of(delimiter[i],lastPos);
	}
	

	auto m2 = std::minmax_element(trist_Pos.begin(),trist_Pos.end());
	pos = *m2.first;

	// 不存在分隔符
	if (s.size()!=0 && string::npos == pos){
		tokens.push_back(s);
		return;
	}


	
	while (s.size()-1 >= lastPos && string::npos != pos && isneq == true){
		
		tokens.push_back(s.substr(lastPos, pos-lastPos));
		
		for (int i = pos; i < s.size(); ++i){
			isneq = true;
			for (int j = 0; j < delimiter.size(); ++j){

				if ((s[i]& 0x80) == false){
					trist_a = s[i];
				}
				else{
					trist_a = s[i];
					trist_a += s[i+1];
					i++;
				}
				if (trist_a == delimiter[j]){
					isneq = false;
					break;
				}
			}
			if (isneq == true){
				lastPos = i;
				break;
			}
		}
		

		
		for (int i=0; i < delimiter.size();++i){
			trist_Pos[i] = s.find_first_of(delimiter[i],lastPos);
		}
		auto m2 = std::minmax_element(trist_Pos.begin(),trist_Pos.end());
		pos = *m2.first;
		if ( string::npos == pos){
			tokens.push_back(s.substr(lastPos, s.size()));
		}
	}
	
}

void ReadCP::initDelimiter(vector<string>& delimiter){
	delimiter.resize(9);
	delimiter[0]=" ";
	delimiter[1]=",";
	/*delimiter[2]="-";*/
	delimiter[2]="!";
	delimiter[3]="=";
	delimiter[4]="*";
	delimiter[5]="\"";
	delimiter[6]="[";
	delimiter[7]="]";
	delimiter[8]="\t";
}