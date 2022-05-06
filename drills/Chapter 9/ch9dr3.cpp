#include "std_lib_facilities.h"

class Date
{
public:
	Date(int y, int m, int d);
	void add_day(const int n);
	int month() const { return m_month; }
	int day() const { return m_day; }
	int year() const { return m_year; }

private:
	int m_year;
	int m_month;
	int m_day;

	static constexpr std::array<int, 12> month_day{31,28,31,30,31,30,31,31,30,31,30,31};
};

Date::Date(int y, int m, int d)
{
	// check that (y,m,d) is a valid date.
	if(m < 1 || m > 12){
		error("Invalid month!");
	}
	if(d < 1 || d > month_day.at(m - 1)){
		error("Invalid day!");
	}
	if(y < 0){
		error("Invalid year!");
	}
	m_month = m;
	m_day = d;
	m_year = y;
}

void Date::add_day(const int n)
{
	// increase dd by n days.
	if(m_day + n <= month_day.at(m_month - 1)){
		m_day += n;
	} else{
		int new_day{m_day + n - month_day.at(m_month - 1)};
		while(true){
			++m_month;
			if(m_month > 12){
				m_month = 1;
				++m_year;
			}
			if(new_day <= month_day.at(m_month - 1)){
				m_day = new_day;
				break;
			}
			new_day -= month_day.at(m_month - 1);
		}
	}
}

std::ostream& operator<<(std::ostream& os, const Date& d)
{
	return os << '(' << d.year() << ',' << d.month() << ',' << d.day() << ')';
}

int main()
try{
	Date today{1978, 6, 25};
	//Date today{2004, 13, -5};
	Date tomorrow{today};
	tomorrow.add_day(1);

	std::cout << "today" << today << '\n';
	std::cout << "tomorrow" << tomorrow << '\n';

	keep_window_open();
	return 0;
}
catch(std::exception& e){
	std::cerr << "exception: " << e.what() << std::endl;
	keep_window_open();
	return 1;
}
catch(...){
	std::cerr << "exception\n";
	keep_window_open();
	return 2;
}