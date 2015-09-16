#include <samchon/Timestamp.hpp>

#include <string>
#include <samchon/StringUtil.hpp>
#include <samchon/Charset.hpp>

namespace samchon
{
	chrono::system_clock::time_point Timestamp::TP_1970;
	long long Timestamp::SECONDS_1970;

	Timestamp::Timestamp()
	{
		init();

		timePoint = chrono::system_clock::now();
		refreshTM();
	};
	Timestamp::Timestamp(long year, long month, long date, long hour, long min, long sec)
	{
		init();

		try
		{
			setTimestamp(year, month, date, hour, min, sec);
		}
		catch (invalid_argument &error)
		{
			throw error;
		}
	};
	Timestamp::Timestamp(const string &str)
	{
		init();
		string val = StringUtil::trim(str);

		int year, month, date;
		int hour = 0, min = 0, sec = 0;

		string ymd;

		//�ú��ʱ��� ���� ��
		if (val.find(" ") != string::npos)
		{
			ymd = move(StringUtil::between(val, "", " "));
			string &hms = StringUtil::between(val, " ");

			vector<string> &hmsVec = StringUtil::split(hms, ":");

			hour = stoi(hmsVec.at(0));
			min = stoi(hmsVec.at(1));
			sec = stoi(hmsVec.at(2));
		}
		else
			ymd = move(val);

		//����� ����
		vector<string> &ymdVec = StringUtil::split(ymd, "-");
		year = stoi(ymdVec.at(0));
		month = stoi(ymdVec.at(1));
		date = stoi(ymdVec.at(2));

		try
		{
			setTimestamp(year, month, date, hour, min, sec);
		}
		catch (invalid_argument &error)
		{
			throw error;
		}
	};
	Timestamp::Timestamp(const wstring &wstr)
		: Timestamp(string(wstr.begin(), wstr.end())) {}
	Timestamp::Timestamp(const long long linuxTime)
	{
		init();

		setLinuxTime(linuxTime);
	};
	Timestamp::~Timestamp() {};

	//SET METHODS -> SEMI-CONSTRUCTORS
	void Timestamp::setTimestamp(long year, long month, long date, long hours, long minutes, long seconds) //��¥, �ð� ����
	{
		array<int, 12> &monthArray = getLastDateArray(year);

		if (month > 12)
			throw invalid_argument(StringUtil::substitute("month is over 12: {0}", month));
		if (date > monthArray[month - 1])
			throw invalid_argument(StringUtil::substitute("date is over {0}: {1}", monthArray[month - 1], date));

		long long totalSeconds = calcSeconds(year, month, date, hours, minutes, seconds);
		timePoint = chrono::system_clock::from_time_t(totalSeconds - SECONDS_1970);

		refreshTM();
	};
	void Timestamp::setLinuxTime(long long val) //1970-01-01 09:00:00���κ��� �帥 �ð�(��)�� �������� �ð��� ����
	{
		timePoint = chrono::system_clock::from_time_t(val);

		refreshTM();
	}

	//INITS
	void Timestamp::init()
	{
		//�ʱ�ȭ
		static bool initFlag = false;
		if (initFlag == true)
			return;

		TP_1970 = chrono::system_clock::from_time_t(0);
		SECONDS_1970 = calcSeconds(1970, 1, 1, 9);

		initFlag = true;
	};
	array<int, 12> Timestamp::getLastDateArray(int year)
	{
		//�� �� ������ ��¥
		array<int, 12> monthArray = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

		//�����̸� 2���� 29�Ϸ�
		if (year % 4 == 0)
			if (!(year % 100 == 0 && year % 400 != 0))
				monthArray[1]++;

		return move(monthArray);
	}

	void Timestamp::refreshTM()
	{
		//TM ����
		time_t tt = chrono::system_clock::to_time_t(timePoint);
		localtime_s(&tm, &tt);
	};

	//�� �� ���ϱ� -> ����� �Ұ���
	long long Timestamp::calcSeconds(long year, long month, long date, long hours, long minutes, long seconds)
	{
		array<int, 12> &monthArray = getLastDateArray(year);
		long long total;

		//��, ��, ���� �� �ϼ�
		total = (year - 1) * 365 + ((year - 1) / 4) - ((year - 1) / 100) + ((year - 1) / 400);

		//�޼���ŭ ���ڸ� ����
		for (int i = 0; i < month - 1; i++)
			total += monthArray[i];
		total += date;

		//���ڿ� 3,600�� ���ϰ� �ð��� ����
		total = (total * 24 * 60 * 60) + (hours * 60 * 60 + minutes * 60 + seconds);
		return total;
	};

	//SET METHODS
	void Timestamp::setYear(int year)
	{
		int month = getMonth();
		int newDate = getDate();

		//���޿� 29���ε� addYear�� �Ϸ��� ���
		if
			(
			month == 2 && newDate == 29
			//���簡 ���� 29��

			&&
			!(
			year == 4 &&
			!((year % 100 == 0 && year % 400 != 0))
			) //�ٲ� ������ 2���� ������ �ƴ�
			)
			newDate = 28;

		setTimestamp(year, month, newDate, getHour(), getMinute(), getSecond());
	};
	void Timestamp::setMonth(int month)
	{
		if (month > 12)
			throw invalid_argument(StringUtil::substitute("month is over 12: {0}", month));

		//�޷�
		int year = getYear();
		int newDate = getDate();

		array<int, 12> &monthArray = getLastDateArray(year);

		//�ش� ���� ������ ���� �ʰ��� ��, �����Ѵ�
		//EX-> 4���ε� 31���̸� 4�� 30�Ϸ� ����
		if (newDate > monthArray[month - 1])
			newDate = monthArray[month - 1];

		setTimestamp(year, month, newDate, getHour(), getMinute(), getSecond());
	};
};