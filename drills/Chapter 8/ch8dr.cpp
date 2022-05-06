#include "std_lib_facilities.h"

void swap_v(int a, int b)
{
	int temp{};
	temp = a;
	a = b;
	b = temp;
	std::cout << "*** In function swap_v() a: " << a << " b: " << b << "\n";
}

void swap_r(int& a, int& b)
{
	int temp{};
	temp = a;
	a = b;
	b = temp;
	std::cout << "*** In function swap_r() a: " << a << " b: " << b << "\n";
}

// cannot assign to a variable that is const
//void swap_cr(const int &a, const int &b)
//{
//    int temp{};
//    temp = a;
//    a = b;
//    b = temp;
//}

int main()
{
	int x{7};
	int y{9};
	std::cout << "x: " << x << " y: " << y << "\n";
	swap_v(x, y);
	std::cout << "after swap_v() x: " << x << " y: " << y << "\n";
	swap_r(x, y);
	std::cout << "after swap_r() x: " << x << " y: " << y << "\n";
	//swap_cr(x, y);    
	std::cout << "-------------------------------------------------\n";

	swap_v(7, 9);       
	//swap_r(7, 9);     
	//swap_cr(7, 9);    
	std::cout << "-------------------------------------------------\n";

	const int cx{7};
	const int cy{9};
	std::cout << "cx: " << cx << " cy: " << cy << "\n";
	swap_v(cx, cy);
	std::cout << "after swap_v() cx: " << cx << " cy: " << cy << "\n";
	//swap_r(cx, cy);   
	//swap_cr(cx, cy);  
	std::cout << "-------------------------------------------------\n";

	double dx{7.7};
	double dy{9.9};
	std::cout << "dx: " << dx << " dy: " << dy << "\n";
	swap_v(dx, dy);     
	std::cout << "after swap_v() dx: " << dx << " dy: " << dy << "\n";
	//swap_r(dx, dy);   
	//swap_cr(dx, dy);  
	std::cout << "-------------------------------------------------\n";

	swap_v(7.7, 9.9);   
	//swap_r(7.7, 9.9); 
	//swap_cr(7.7, 9.9);
	std::cout << "-------------------------------------------------\n";

	keep_window_open();
	return 0;
}