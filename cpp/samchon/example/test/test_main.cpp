#include <iostream>
#include <fstream>
#include <iterator>
#include <string>

#include <samchon/library/XML.hpp>
#include <Windows.h>

#ifdef _WIN64
#	ifdef _DEBUG
#		pragma comment(lib, "x64/Debug/SamchonFramework.lib")
#	else
#		pragma comment(lib, "x64/Release/SamchonFramework.lib")
#	endif
#else
#	ifdef _DEBUG
#		pragma comment(lib, "Debug/SamchonFramework.lib")
#	else
#		pragma comment(lib, "Release/SamchonFramework.lib")
#	endif
#endif

using namespace std;
using namespace samchon;
using namespace samchon::library;

#include <samchon/library/HTTPLoader.hpp>
#include <samchon/library/Charset.hpp>
#include <samchon/library/StringUtil.hpp>

void main()
{
	string url = Charset::toUTF8("http://www.bomtvbiz.com/dt/order_print/1441261881391/20150903_1441261881391_�����_4��_�� �� ��.pdf");
	//url = StringUtil::replaceAll(url, "�����_4��_�� �� ��", Charset::toUTF8("�����_4��_�� �� ��"));

	cout << HTTPLoader(url, HTTPLoader::GET).load().size() << endl;
	cout << HTTPLoader(u8"http://www.bomtvbiz.com/dt/order_print/1441261881391/20150903_1441261881391_�����_4��_�� �� ��.pdf", HTTPLoader::GET).load().size() << endl;

	system("pause");
}