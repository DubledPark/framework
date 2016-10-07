#include <iostream>

#include <samchon/library/TSQLi.hpp>
#include <samchon/library/SQLStatement.hpp>

#include <samchon/library/XML.hpp>

using namespace std;
using namespace samchon::library;

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

void main()
{
	shared_ptr<SQLi> sqli(new TSQLi());

	try
	{
		sqli->connect("211.239.161.211", "REDPRINTING_TEST", "redprinting_test", "redprinting");

		auto stmt = sqli->createStatement();
		size_t uid = 5;
		wstring name = L"��浿";
		wstring department = L"ȫ����";

		stmt->prepare(L"INSERT INTO �ѱ����̺� (��ȣ, �̸�, �μ�) VALUES (?, ?, ?)", uid, name, department);
		stmt->execute();

		/*stmt->executeDirectly(u8"SELECT * FROM �ѱ����̺�");

		while (stmt->fetch())
		{
			int uid = stmt->at<int>(0);
			const string &name = stmt->at<string>(1);
			const string &department = stmt->at<string>(2);

			cout << uid << endl;
			cout << name << endl;
			cout << department << endl;
		}*/
	}
	catch (exception &e)
	{
		cout << e.what() << endl;
	}

	system("pause");
}