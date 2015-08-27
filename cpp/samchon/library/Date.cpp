#include <samchon/library/Date.hpp>

#include <array>

#include <samchon/WeakString.hpp>
#include <samchon/library/StringUtil.hpp>

using namespace std;
using namespace samchon;
using namespace samchon::library;

chrono::system_clock::time_point Date::TP_1970 = chrono::system_clock::from_time_t(0);

/* --------------------------------------------------------------------------
	CONSTRUCTORS
-------------------------------------------------------------------------- */
//CONSTRUCTORS
Date::Date()
	: super(chrono::system_clock::now())
{
}
Date::Date(const Date &date)
	: super(date)
{
}
Date::Date(Date &&date)
	: super(move(date))
{
}

Date::Date(int year, int month, int date)
{
	set(year, month, date);
}
Date::Date(const WeakString &wStr)
{
	set(wStr);
}
Date::Date(long long linuxTime)
	: super(chrono::system_clock::from_time_t(linuxTime))
{
}

//SEMI-CONSTRUCTORS
void Date::set(const WeakString &wStr)
{
	WeakString val = wStr.trim();

	int year, month, date;

	//�ú��ʱ��� ���� ��
	if (val.find(_T(" ")) != String::npos)
		val = val.between(String(), _T(" "));

	//����� ����
	vector<WeakString> &ymdVec = val.split(_T("-"));
	year = stoi(ymdVec[0].str());
	month = stoi(ymdVec[1].str());
	date = stoi(ymdVec[2].str());

	set(year, month, date);
}
void Date::set(int year, int month, int date) //��¥, �ð� ����
{
	array<int, 12> &monthArray = calcLastDates(year);

	if (month > 12)
		throw invalid_argument("month is over 12");
	if (date > monthArray[month - 1])
		throw invalid_argument("date is over expiration date");

	long long linuxTime = calcSeconds(year, month, date);
	set(linuxTime);
};
void Date::set(long long linuxTime)
{
	this->operator-=(chrono::seconds(toLinuxTime()));
	this->operator+=(chrono::seconds(linuxTime));
}

/* --------------------------------------------------------------------------
	HIDDEN METHODS
-------------------------------------------------------------------------- */
auto Date::calcLastDates(int year) -> array<int, 12>
{
	//Last dates of each month
	static array<int, 12> monthArray = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

	//When the year contains leaf month, then make expiration date of Feburary to 29
	if (year % 4 == 0)
		if (!(year % 100 == 0 && year % 400 != 0))
			monthArray[1] = 29;

	return move(monthArray);
}
long long Date::calcSeconds(int year, int month, int date)
{
	array<int, 12> &monthArray = calcLastDates(year);
	long long total;

	//��, ��, ���� �� �ϼ�
	total = (year - 1) * 365 + ((year - 1) / 4) - ((year - 1) / 100) + ((year - 1) / 400);

	//�޼���ŭ ���ڸ� ����
	for (int i = 0; i < month - 1; i++)
		total += monthArray[i];
	total += date;

	//���ڿ� 3,600�� ���Ѵ�
	total = (total * 24 * 60 * 60);
	return total;
}

/* --------------------------------------------------------------------------
	SETTERS OF EACH COMPONENT
-------------------------------------------------------------------------- */
//SET METHODS
void Date::setYear(int year)
{
	int month = getMonth();
	int newDate = getDate();

	//���޿� 29���ε� addYear�� �Ϸ��� ���
	if
		(
			month == 2 && newDate == 29 //���簡 ���� 29��
			&&
			!(
				year == 4 &&
				!((year % 100 == 0 && year % 400 != 0))
			) //�ٲ� ������ 2���� ������ �ƴ�
		)
		newDate = 28;

	set(year, month, newDate);
}
void Date::setMonth(int month)
{
	if (month > 12)
		throw invalid_argument("month is over 12");

	//�޷�
	int year = getYear();
	int newDate = getDate();

	array<int, 12> &monthArray = calcLastDates(year);

	//�ش� ���� ������ ���� �ʰ��� ��, �����Ѵ�
	//EX-> 4���ε� 31���̸� 4�� 30�Ϸ� ����
	if (newDate > monthArray[month - 1])
		newDate = monthArray[month - 1];

	set(year, month, newDate);
}
void Date::setDate(int val)
{
	set(getYear(), getMonth(), val); 
}

//ADD METHODS
void Date::addYear(int val)
{
	//���簡 2012�� 2�� 29��(����)�ε� 1���� �߰��ϸ� 2013�� 2�� 28�Ϸ� ����
	setYear(getYear() + val);
}
void Date::addMonth(int val)
{
	//���簡 3�� 31���ε� 1���� �߰��Ǹ� ��¥�� 4�� 30�Ϸ� ���Ѵ�
	int newYear = getYear();
	int newMonth = getMonth() + val;
	int newDate = getDate();

	if (newMonth > 12)
	{
		newYear = newYear + (newMonth - 1) / 12;
		newMonth = newMonth % 12;
	}

	//�޷�
	std::array<int, 12> &monthArray = calcLastDates(newYear);

	//�ش� ���� ������ ���� �ʰ��� ��, �����Ѵ�
	//EX-> 4���ε� 31���̸� 4�� 30�Ϸ� ����
	if (newDate > monthArray[newMonth - 1])
		newDate = monthArray[newMonth - 1];

	set(newYear, newMonth, newDate);
}
void Date::addWeek(int val)
{
	addDate(7 * val);
}
void Date::addDate(int val)
{
	operator+=(chrono::hours(24 * val));
}

/* --------------------------------------------------------------------------
	GETTERS AND TO_STRING
-------------------------------------------------------------------------- */
auto Date::toLinuxTime() const -> long long
{
	std::chrono::system_clock::duration duration = *this - TP_1970;
	std::chrono::seconds seconds = std::chrono::duration_cast<std::chrono::seconds>(duration);

	return seconds.count();
}
int Date::getYear() const
{
	auto &tm = toTM();
	return tm.tm_year + 1900;
}
int Date::getMonth() const		
{
	auto &tm = toTM();
	return tm.tm_mon + 1; 
}
int Date::getDay() const		
{ 
	auto &tm = toTM();
	return tm.tm_wday; 
}
int Date::getDate() const		
{ 
	auto &tm = toTM();
	return tm.tm_mday; 
}

auto Date::toTM() const -> struct tm
{
	struct tm tm;
	time_t tt = chrono::system_clock::to_time_t(*this);
	localtime_s(&tm, &tt);

	return tm;
}
auto Date::toString() const -> String
{
	return StringUtil::substitute
		(
			_T("{1}-{2}-{3}"),
			getYear(), getMonth(), getDate()
		);
}