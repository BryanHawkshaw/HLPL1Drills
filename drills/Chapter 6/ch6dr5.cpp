#include "std_lib_facilities.h"

constexpr char quit = 'x';
constexpr char print = '=';

class Token
{
public:
  char kind;			
  double value;		

  
  Token(char ch) : kind(ch), value(0) {}
  Token(char ch, double val) : kind(ch), value(val) {}
};


class Token_stream
{
public:

  Token_stream() : full(false), buffer('\0') {}
  Token get();					  
  void putback(Token t);  
private:
  bool full;      				
  Token buffer;					  
};


void Token_stream::putback(Token t)
{
  if (full)
  {
    error("putback() into a full buffer");
  }
  buffer = t;      
  full = true;      
}



Token Token_stream::get()
{
  Token temp{'\0'};

  
  if (full)
  {
    full = false;
    temp = buffer;
  }
  else
  {
    char ch;
    std::cin >> ch;
    switch (ch)
    {
    case print:
    case quit:
    case '(': case ')': case '*': case '/': case '+': case '-':
      temp.kind = ch;
      break;
    case '.':
    case '0': case '1': case '2': case '3': case '4':
    case '5': case '6': case '7': case '8': case '9':
    {
      std::cin.putback(ch); 
      double val;
      std::cin >> val;		  
      temp.kind = '8';
      temp.value = val;
      break;
    }
    default:
      error("Bad token");
    }
  }
  return temp;
}



Token_stream ts;        



double expression();    


double primary()
{
  double temp{};    

  Token t = ts.get();
  switch (t.kind)
  {
  case '(':			    
  {
    double d = expression();
    t = ts.get();
    if (t.kind != ')')
    {
      error("')' expected");
    }
    temp = d;
    break;
  }
  case '-':			    
    temp = -1 * primary();
    break;
  case '+':			    
    temp = primary();
    break;
  case '8':			    
    temp = t.value;	
    break;
  default:
    error("primary expected");
  }
  return temp;
}


double term()
{
  double left = primary();
  Token t = ts.get(); 
  while (true)
  {
    switch (t.kind)
    {
    case '*':
      left *= primary();
      t = ts.get();
      break;
    case '/':
    {
      double d = primary();
      if (d == 0)
      {
        error("divide by zero");
      }
      left /= d;
      t = ts.get();
      break;
    }
    default:
      ts.putback(t);	
      return left;
    }
  }
}


double expression()
{
  double left = term(); 
  Token t = ts.get();		
  while (true)
  {
    switch (t.kind)
    {
    case '+':
      left += term();		
      t = ts.get();
      break;
    case '-':
      left -= term();		
      t = ts.get();
      break;
    default:
      ts.putback(t);		
      return left;		  
    }
  }
}



int main()
{
  try
  {
    std::cout << "\"Welcome to our simple calculator.\"\n\n"
      << "Please enter expressions using floating-point numbers.\n"
      << "Operations available are '+', '-', '*' and '/'.\n"
      << "Can change order of operations using ( ).\n"
      << "Use the '" << print << "' to show results and '" << quit << "' to exit.\n\n";

    double val{};
    while (std::cin)
    {
      Token t = ts.get();
      if (t.kind == quit)
      {
        break;
      }
      if (t.kind == print)
      {
        std::cout << "= " << val << '\n';
      }
      else
      {
        ts.putback(t);
        val = expression();
      }
    }
  }
  catch (std::exception& e)
  {
    std::cerr << "error: " << e.what() << '\n';
    keep_window_open();
    return 1;
  }
  catch (...)
  {
    std::cerr << "Oops: unknown exception!\n";
    keep_window_open();
    return 2;
  }
  keep_window_open();
  return 0;
}