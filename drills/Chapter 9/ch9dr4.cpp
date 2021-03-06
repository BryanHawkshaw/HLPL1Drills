#include "std_lib_facilities.h"

enum class Month
{
	jan = 1, feb, mar, apr, may, jun, jul, aug, sep, oct, nov, dec
};

Month operator++(Month& m)
{
	m = (m == Month::dec) ? Month::jan : Month(int(m) + 1);
	return m;
}

class Year
{
public:
	static constexpr int year_min{1800};
	static constexpr int year_max{2200};

	class Invalid {};
	Year(int x) : y(x) { if(x < year_min || year_max <= x) throw Invalid(); }
	int year() { return y; }
private:
	int y;
};

Year operator++(Year& y)
{
	y = (y.year() == Year::year_max) ? y.year() + 1 : throw Year::Invalid();
	return y;
}

class Date
{
public:
	class Invalid {};                        // to be used as exception
	Date(Year y, Month m, int d);	         // check for valid date and initialize
	void add_day(const int n);
	Month month() const { return m_month; }
	int day() const { return m_day; }
	Year year() const { return m_year; }

private:
	Year m_year;
	Month m_month;
	int m_day;
	bool is_valid();

	static constexpr array<int, 12> month_day{31,28,31,30,31,30,31,31,30,31,30,31};
};

Date::Date(Year y, Month m, int d)
	: m_year{y}, m_month{m}, m_day{d}
{
	if(!is_valid()) throw Invalid{};
}

void Date::add_day(const int n)
{
	// increase dd by n days.
	if(m_day + n <= month_day.at((int)m_month - 1)){
		m_day += n;
	} else{
		int new_day{m_day + n - month_day.at((int)m_month - 1)};
		while(true){
			++m_month;
			if(m_month == Month::jan){
				++m_year;
			}
			if(new_day <= month_day.at((int)m_month - 1)){
				m_day = new_day;
				break;
			}
			new_day -= month_day.at((int)m_month - 1);
		}
	}
}

bool Date::is_valid()
{
	if(m_month < Month::jan || m_month > Month::dec){
		return false;
	}
	if(m_day < 1 || m_day > month_day.at((int)m_month - 1)){
		return false;
	}
	return true;
}

std::ostream& operator<<(std::ostream& os, const Date& d)
{
	return os << '(' << d.year().year() << ',' << (int)d.month() << ',' << d.day() << ')';
}

int main()
try{
	try{
		Date today{1978, Month::jun, 25};
		//Date today{2004, 13, -5};
		Date tomorrow{today};
		tomorrow.add_day(1);

		std::cout << "today" << today << '\n';
		std::cout << "tomorrow" << tomorrow << '\n';
	}
	catch(Date::Invalid){
		error("invalid date");
	}

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