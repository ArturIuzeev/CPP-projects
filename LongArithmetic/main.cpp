#include "LN.h"
#include "return_codes.h"

#include <fstream>
#include <iostream>
#include <stack>

using namespace std;

bool isSymbol(const string& str)
{
	return str == "-" || str == "+" || str == "/" || str == "*" || str == "_" || str == "~" || str == "%" || str == "<" ||
		   str == ">" || str == "<=" || str == ">=" || str == "==" || str == "!=" || str == "-=" || str == "+=";
}

int main(int argc, char** argv)
{
	if (argc != 3)
	{
		return ERROR_INVALID_DATA;
	}
	ifstream in(argv[1]);
	ofstream out(argv[2]);
	string line;
	try
	{
		stack< LN > list;
		while (in >> line)
		{
			if (!isSymbol(line))
			{
				char* q = (char*)malloc(sizeof(char) * line.length());
				if (q == nullptr)
				{
					cerr << "Not enough memory";
					return ERROR_MEMORY;
				}
				for (long i = 0; i < line.length(); i++)
				{
					q[i] = line[i];
				}
				LN num = LN(q, line.length());
				list.push(num);
				free(q);
			}
			else
			{
				if (line != "_" && line != "~" && list.size() < 2)
				{
					cerr << "Not enough arguments for binary or compare operation";
					in.close();
					out.close();
					return ERROR_INVALID_DATA;
				}
				if (line == "_" && line == "~" && list.size() < 1)
				{
					cerr << "Not enough arguments for unary operation";
					in.close();
					out.close();
					return ERROR_INVALID_DATA;
				}
				if (line == "+")
				{
					LN second = list.top();
					list.pop();
					LN first = list.top();
					list.pop();
					if (getNan(first))
					{
						list.push(first);
					}
					else if (getNan(second))
					{
						list.push(second);
					}
					else
					{
						first = first + second;
						list.push(first);
					}
				}
				else if (line == "-")
				{
					LN second = list.top();
					list.pop();
					LN first = list.top();
					list.pop();
					if (getNan(first))
					{
						list.push(first);
					}
					else if (getNan(second))
					{
						list.push(second);
					}
					else
					{
						first = first - second;
						list.push(first);
					}
				}
				else if (line == "*")
				{
					LN second = list.top();
					list.pop();
					LN first = list.top();
					list.pop();
					if (getNan(first))
					{
						list.push(first);
					}
					else if (getNan(second))
					{
						list.push(second);
					}
					else
					{
						first = first * second;
						list.push(first);
					}
				}
				else if (line == "/")
				{
					LN second = list.top();
					list.pop();
					LN first = list.top();
					list.pop();
					if (getNan(first))
					{
						list.push(first);
					}
					else if (getNan(second))
					{
						list.push(second);
					}
					else
					{
						first = first / second;
						list.push(first);
					}
				}
				else if (line == "%")
				{
					LN second = list.top();
					list.pop();
					LN first = list.top();
					list.pop();
					if (getNan(first))
					{
						list.push(first);
					}
					else if (getNan(second))
					{
						list.push(second);
					}
					else
					{
						first = first % second;
						list.push(first);
					}
				}
				else if (line == "-=")
				{
					LN second = list.top();
					list.pop();
					LN first = list.top();
					list.pop();
					if (getNan(first))
					{
						list.push(first);
					}
					else if (getNan(second))
					{
						list.push(second);
					}
					else
					{
						first -= second;
						list.push(first);
					}
				}
				else if (line == "+=")
				{
					LN second = list.top();
					list.pop();
					LN first = list.top();
					list.pop();
					if (getNan(first))
					{
						list.push(first);
					}
					else if (getNan(second))
					{
						list.push(second);
					}
					else
					{
						first += second;
						list.push(first);
					}
				}
				else if (line == "_")
				{
					LN first = list.top();
					list.pop();
					if (getNan(first))
					{
						list.push(first);
					}
					else
					{
						operator_(first);
						list.push(first);
					}
				}
				else if (line == "~")
				{
					LN first = list.top();
					list.pop();
					if (getNan(first))
					{
						list.push(first);
					}
					else
					{
						first = ~first;
						list.push(first);
					}
				}
				else if (line == ">")
				{
					LN second = list.top();
					list.pop();
					LN first = list.top();
					list.pop();
					if (getNan(first) || getNan(second))
					{
						LN result(0);
						list.push(result);
					}
					else
					{
						bool res = first > second;
						if (res)
						{
							LN result(1);
							list.push(result);
						}
						else
						{
							LN result(0);
							list.push(result);
						}
					}
				}
				else if (line == "<")
				{
					LN second = list.top();
					list.pop();
					LN first = list.top();
					list.pop();
					if (getNan(first) || getNan(second))
					{
						LN result(0);
						list.push(result);
					}
					else
					{
						bool res = first < second;
						if (res)
						{
							LN result(1);
							list.push(result);
						}
						else
						{
							LN result(0);
							list.push(result);
						}
					}
				}
				else if (line == ">=")
				{
					LN second = list.top();
					list.pop();
					LN first = list.top();
					list.pop();
					if (getNan(first) || getNan(second))
					{
						LN result(0);
						list.push(result);
					}
					else
					{
						bool res = first >= second;
						if (res)
						{
							LN result(1);
							list.push(result);
						}
						else
						{
							LN result(0);
							list.push(result);
						}
					}
				}
				else if (line == "<=")
				{
					LN second = list.top();
					list.pop();
					LN first = list.top();
					list.pop();
					if (getNan(first) || getNan(second))
					{
						LN result(0);
						list.push(result);
					}
					else
					{
						bool res = first <= second;
						if (res)
						{
							LN result(1);
							list.push(result);
						}
						else
						{
							LN result(0);
							list.push(result);
						}
					}
				}
				else if (line == "==")
				{
					LN second = list.top();
					list.pop();
					LN first = list.top();
					list.pop();
					if (getNan(first) || getNan(second))
					{
						LN result(0);
						list.push(result);
					}
					else
					{
						bool res = first == second;
						if (res)
						{
							LN result(1);
							list.push(result);
						}
						else
						{
							LN result(0);
							list.push(result);
						}
					}
				}
				else if (line == "!=")
				{
					LN second = list.top();
					list.pop();
					LN first = list.top();
					list.pop();
					if (getNan(first) && getNan(second))
					{
						LN result(1);
						list.push(result);
					}
					else if (getNan(first) || getNan(second))
					{
						LN result(0);
						list.push(result);
					}
					else
					{
						bool res = first != second;
						if (res)
						{
							LN result(1);
							list.push(result);
						}
						else
						{
							LN result(0);
							list.push(result);
						}
					}
				}
				else
				{
					cerr << "Unknown operation";
					in.close();
					out.close();
					return ERROR_INVALID_DATA;
				}
			}
		}
		while (!list.empty())
		{
			bool flag = false;
			LN helper = list.top();
			list.pop();
			if (getNan(helper))
			{
				out << "NaN\n";
			}
			else
			{
				for (long j = getSize(helper) - 1; j >= 0; j--)
				{
					if (getNegate(helper) && !flag)
					{
						out << "-";
						flag = true;
					}
					out << getNum(helper, j);
				}
				out << "\n";
			}
		}
		in.close();
		out.close();
		return ERROR_SUCCESS;
	} catch (...)
	{
		in.close();
		out.close();
		cerr << "Not enough memory";
		return ERROR_MEMORY;
	}
}
