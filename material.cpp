#include"material.h"
#include <iomanip>
using namespace std;

void PBARL::printPBARL(std::ofstream& ofs)
{
	//ofs <<  setprecision(3);//设置输出精度为8
	ofs << "$-- Property " << name << " --$" << endl;
	ofs << "PBARL   " << setw(8) << PID << setw(8) << MID << setw(16) << TYPE << endl;
	ofs << "        ";
	for (int i = 0; i < DIM.size(); i++)
	{
		ofs << setw(8) << DIM[i];
	} 
	ofs << endl;
}

void PSHELL::printPSHELL(ofstream& ofs)
{
	//ofs << setprecision(3);//设置输出精度为8
	ofs << "$-- Property " << name << " --$" << endl;
	ofs << "PSHELL  " << setw(8) << PID << setw(8) << MID << setw(8) << T << endl;
		//<< setw(8) << 1 << setw(16) << 1 << endl;
}

void MAT1::printMAT1(ofstream& ofs)
{
	//ofs <<  setprecision(3);//设置输出精度为8
	ofs << "$-- Material " << name << " --$" << endl;
	ofs << "MAT1    " 
		<< setw(8) << MID 
		<< setw(8) << E 
		<< setw(16) << Nu
		<< setw(8) << RHO << endl;
}

void MAT8::printMAT8(std::ofstream& ofs)
{
	ofs << "MAT8    " 
		<< setw(8) << MID 
		<< setw(8) << E1  
		<< setw(8) << E2
		<< setw(8) << NU12
		<< setw(8) << G12 
		<< setw(8) << G1Z
		<< setw(8) << G2Z 
		<< setw(8) << RHO << endl;
}

