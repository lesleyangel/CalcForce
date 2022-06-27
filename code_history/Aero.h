#pragma once
#include "stdafx.h"
#include "Interpolation.h"
#ifndef TOYYC_H
#define TOYYC_H
class CP
{
public:
	CP(){xlist.clear();thetalist.clear();Datainfo.clear();}
	double mach;
	double alpha;
	vector<vector<double>> CPmatrix;

	double getP(double xsite, double theta);
private:
	vector<double> xlist;
	vector<double> thetalist;
	vector<double> Datainfo;
	void CalcXList();
	void CalcThetaList();
	int CalcData();
};

class ReadCP
{
public:
	
	// CP
	vector<CP> CP_vec;
	void readFile(const string _path);
protected:

	void readFilePerElement(const string _filePath, vector<vector<string>>& _tokens, vector<string>& _delimiter);
	void split(const string& s,vector<string>& tokens,const vector<string>& delimiter);
	void initDelimiter(vector<string>& delimiter);
};

#endif