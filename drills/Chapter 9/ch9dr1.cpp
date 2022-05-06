#include "std_lib_facilities.h"

constexpr std::array<int, 12> month_day{31,28,31,30,31,30,31,31,30,31,30,31};

struct Date
{
	int year;
	int month;
	int day;
};

void init_day(Date& dd, const int y, const int m, const int d)
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
	dd.month = m;
	dd.day = d;
	dd.year = y;
}

void add_day(Date& dd, const int n)
{
	// increase dd by n days.
	if(dd.day + n <= month_day.at(dd.month - 1)){
		dd.day += n;
	} else{
		int new_day{dd.day + n - month_day.at(dd.month - 1)};
		while(true){
			++dd.month;
			if(dd.month > 12){
				dd.month = 1;
				++dd.year;
			}
			if(new_day <= month_day.at(dd.month - 1)){
				dd.day = new_day;
				break;
			}
			new_day -= month_day.at(dd.month - 1);
		}
	}
}

std::ostream& operator<<(std::ostream& os, const Date& d)
{
	return os << '(' << d.year << ',' << d.month << ',' << d.day << ')';
}

int main()
try{
	Date today;
	init_day(today, 1978, 6, 25);
	//init_day(today, 2004, 13, -5);
	Date tomorrow{today};
	add_day(tomorrow, 1);

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