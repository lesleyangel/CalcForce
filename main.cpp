#include <memory>
#include "Missile.h"
#include "NasPrinter.h"




int main(int argc, char *argv[])
{
	clock_t brginTime = clock();
	string datapath;
	switch (argc)
	{
	case 1: {
		datapath = argv[0];
		string::size_type iPos = (datapath.find_last_of('\\') + 1) == 0 ? datapath.find_last_of('/') + 1 : datapath.find_last_of('\\') + 1;
		datapath = datapath.substr(0, iPos) + "input.txt";//获取文件路径
	}	break;
	case 2:
		datapath = argv[1];
		break;
	default:
		cout << "输入的参数个数错误！" << endl;
		break;
	}
	unique_ptr<ShapeMash> mt3(new Minuteman3);//TestTube
	cout << "$---------------------------------------------------$" << endl;
	cout << "$                    CalcForce                      $" << endl;
	cout << "$                      v2.2.1                       $" << endl;
	cout << "$---------------------------------------------------$" << endl;
	cout << endl;

	//datapath = "C:\\Users\\yycab\\Desktop\\database\\1Y1B\\CalcForce\\Minuteman3struct_input_fortest.txt";
	int type = mt3->ReadFromFile(datapath);
	cout << "用时：" << (clock() - brginTime)/1000.0  << "s" << endl;
	system("pause");
	return 0;
}


