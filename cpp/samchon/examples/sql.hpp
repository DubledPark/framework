#pragma once

#include <iostream>
#include <samchon/library/TSQLi.hpp>

using namespace std;
using namespace samchon::library;

namespace samchon
{
namespace examples
{
namespace sql
{
	void main()
	{
		using namespace std;
		using namespace samchon::library;

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

			stmt->executeDirectly(u8"SELECT * FROM �ѱ����̺�");

			while (stmt->fetch())
			{
				int uid = stmt->at<int>(0);
				const string &name = stmt->at<string>(1);
				const string &department = stmt->at<string>(2);

				cout << uid << endl;
				cout << name << endl;
				cout << department << endl;
			}
		}
		catch (exception &e)
		{
			cout << e.what() << endl;
		}
	};
}
}
}