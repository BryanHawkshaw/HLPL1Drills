#include "std_lib_facilities.h"


constexpr char number = '8';    // t.kind == number means that t is a number Token.
constexpr char quit = 'q';      // t.kind == quit means that t is a quit Token.
constexpr char* declexit = "exit";
constexpr char print = ';';     // t.kind == print means that t is a print Token.

constexpr char name = 'a';      // name token
constexpr char let = 'L';       // declaration token
constexpr char* declkey = "#"; // declaration keyword
constexpr char func = 'F';  // function Token

struct Token
{
	char kind;
	double value;
	std::string name;
	Token(char ch, double val = 0.0) :kind(ch), value(val) {}
	Token(char ch, std::string s) :kind(ch), value(0.0), name(s) {}
};

struct Variable
{
	std::string name;
	double value{};
};

bool operator==(const Variable& v, const std::string s) { return v.name == s; }


vector<Variable> names;


double get_value(const std::string s)
{
	auto vt_itr = std::find(names.cbegin(), names.cend(), s);
	if(vt_itr == names.cend()){
		error("get: undefined name ", s);
	}
	return vt_itr->value;
}

void set_value(const std::string s, const double d)
{
	auto vt_itr = std::find(names.begin(), names.end(), s);
	if(vt_itr == names.cend()){
		error("set: undefined name ", s);
	}
	vt_itr->value = d;
}

bool is_declared(const std::string s)
{
	return names.cend() != std::find(names.cbegin(), names.cend(), s);
}

double define_name(const std::string s, const double d)
{
	if(is_declared(s)) error(s, " declared twice");
	names.push_back(Variable{s, d});
	return d;
}

class Token_stream
{
public:
	Token_stream() :full(false), buffer('\0') {}

	
	Token get();

	
	void putback(const Token t);

	
	void ignore(const char c);

private:
	bool full;      
	Token buffer;   
};

Token Token_stream::get()
{
	Token t{'\0'};
	if(full){
		full = false;
		t = buffer;
	} else{
		char ch;
		std::cin >> ch;
		switch(ch){
			case print:
			case quit:
			case '(':
			case ')':
			case '+':
			case '-':
			case '*':
			case '/':
			case ',':
				//case '%':
				t.kind = ch;
				break;
			case '=':
				if(this->buffer.kind != let){
					const std::string s = (this->buffer.kind == name ? this->buffer.name : std::to_string(this->buffer.value));
					error(s, " can not be re-assigned");
				}
				t.kind = ch;
				break;
			case '.':
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				{
					std::cin.putback(ch);
					double val;
					std::cin >> val;
					t.kind = number;
					t.value = val;
					break;
				}
			default:
				if(isalpha(ch) || (strlen(declkey) == 1)){
					std::string s;
					s += ch;
					if(ch != declkey[0]){
						while(std::cin.get(ch) && (isalpha(ch) || isdigit(ch))) s += ch;
						std::cin.putback(ch);
					}
					if(s == declkey){
						t.kind = let;
					} else if(ch == '('){
						t.kind = func;
						t.name = s;
					} else if(s == declexit){
						t.kind = quit;
					} else{
						t.kind = name;
						t.name = s;
					}
				} else{
					error("Bad token");
				}
		}
	}
	return t;
}

void Token_stream::putback(const Token t)
{
	if(full){
		error("putback() into a full buffer");
	}
	buffer = t;
	full = true;
}

void Token_stream::ignore(const char c)
{
	if(full && c == buffer.kind){
		full = false;
	} else{
		full = false;
		std::cin.clear();
		std::cin.ignore(std::cin.rdbuf()->in_avail(), c);
	}
}

Token_stream ts;


double expression();


double primary()
{
	Token t = ts.get();
	double d{};
	switch(t.kind){
		case '(':
			{
				d = expression();
				t = ts.get();
				if(t.kind != ')'){
					error("')' expected");
				}
				return d;
			}
		case '-':
			return -1 * primary();
		case '+':
			return primary();
		case number:
			return t.value;
		case name:
			return get_value(t.name);
		default:
			error("primary expected");
	}
}


double term()
{
	double left = primary();
	while(true){
		Token t = ts.get();
		switch(t.kind){
			case '*':
				left *= primary();
				break;
			case '/':
				{
					double d = primary();
					if(d == 0) error("divide by zero");
					left /= d;
					break;
				}
				//case '%':
					// not implemented yet
			default:
				ts.putback(t);
				return left;
		}
	}
}


double expression()
{
	double left = term();
	while(true){
		Token t = ts.get();
		switch(t.kind){
			case '+':
				left += term();
				break;
			case '-':
				left -= term();
				break;
			default:
				ts.putback(t);
				return left;
		}
	}
}

double declaration()
{
	Token t = ts.get();
	if(t.kind != name){
		error("name expected in declaration");
	}
	if(is_declared(t.name)){
		error(t.name, " declared twice");
	}
	Token t2 = ts.get();
	if(t2.kind != '='){
		error("= missing in declaration of ", t.name);
	}
	double d = expression();
	define_name(t.name, d);
	return d;
}


double function(const std::string& s)
{
	Token t = ts.get();
	double d{};
	vector<double> func_args;
	if(t.kind != '('){
		error("expected '(', malformed function call");
	} else{
		do{
			t = ts.get();
			// see if any argument is a function call
			if(t.kind == func){
				t.kind = number;
				// recursive call
				//t.value = function(t.name);
				ts.putback(t);
			}
			// check if empty arguments
			if(t.kind == ')'){
				break;
			} else{
				ts.putback(t);
			}
			// push valid function argument
			func_args.push_back(expression());
			t = ts.get();
			if(t.kind == ')') break;
			if(t.kind != ',') error("expected ')', malformed function call");
		} while(t.kind == ',');
	}

	if(s == "sqrt"){
		if(func_args.size() != 1) error("sqrt() expects 1 argument");
		if(func_args[0] < 0) error("sqrt() expects argument value >= 0");
		d = sqrt(func_args[0]);
	} else if(s == "pow"){
		if(func_args.size() != 2) error("pow() expects 2 arguments");
		d = func_args[0];
		auto multiplier = func_args[0];
		int p = narrow_cast<int>(func_args[1]);
		for(; p > 1; --p){
			d *= multiplier;
		}
	} else{
		error("unknown function");
	}
	return d;
}


double statement()
{
	Token t = ts.get();
	switch(t.kind){
		case let:
			return declaration();
		case func:
			//return function(t.name);
		default:
			ts.putback(t);
			return expression();
	}
}

//------------------------------------------------------------------------------

void clean_up_mess()
{
	ts.ignore(print);
}


void calculate()
{
	constexpr char* prompt = "> ";  // indicate a prompt
	constexpr char* result = "= ";  // indicate a result

	while(true) try{
		std::cout << prompt;
		Token t = ts.get();
		while(t.kind == print) t = ts.get();
		if(t.kind == quit) return;
		ts.putback(t);
		std::cout << result << statement() << std::endl;
	}
	catch(std::runtime_error& e){
		std::cerr << e.what() << std::endl;
		clean_up_mess();
	}
}

int main()
try{
	std::cin.sync_with_stdio(false);
	calculate();
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