#include "LN.h"

using namespace std;

void leading_zeros(LN& first, long length)
{
	int q = first.nums[length - 1];

	if (q == 0 && length != 1)
	{
		for (long i = length - 1; i >= 0; i--)
		{
			if (first.nums[i] == 0 && first.nums.get_size() != 1)
			{
				first.nums.pop_back();
			}
			else
			{
				break;
			}
		}
	}
}

int universal_comprasion(const LN& first, const LN& second, bool flag1, bool flag2)
{
	if (!flag1)
	{
		if (first.nums[0] == 0 && second.nums[0] == 0 && first.nums.get_size() == 1 && second.nums.get_size() == 1)
		{
			return flag2 ? 0 : 1;
		}
		if (first.negate && !second.negate)
		{
			return 0;
		}
		if (!first.negate && second.negate)
		{
			return 1;
		}
		if (first.nums.get_size() > second.nums.get_size() && first.negate && second.negate)
		{
			return 0;
		}
		else if (first.nums.get_size() < second.nums.get_size() && first.negate && second.negate)
		{
			return 1;
		}
		else if (first.nums.get_size() > second.nums.get_size())
		{
			return 1;
		}
		else if (first.nums.get_size() < second.nums.get_size())
		{
			return 0;
		}
		else if (!(first.negate && second.negate))
		{
			for (long i = first.nums.get_size() - 1; i >= 0; i--)
			{
				if (first.nums[i] > second.nums[i])
				{
					return 1;
				}
				else if (first.nums[i] < second.nums[i])
				{
					return 0;
				}
			}
		}
		else
		{
			for (long i = first.nums.get_size() - 1; i >= 0; i--)
			{
				if (first.nums[i] < second.nums[i])
				{
					return 1;
				}
				else if (first.nums[i] > second.nums[i])
				{
					return 0;
				}
			}
		}
		return 0;
	}
	else
	{
		if (first.negate && !second.negate)
		{
			return 0;
		}
		if (!first.negate && second.negate)
		{
			return 1;
		}
		if (first.nums.get_size() > second.nums.get_size() && first.negate && second.negate)
		{
			return 0;
		}
		else if (first.nums.get_size() < second.nums.get_size() && first.negate && second.negate)
		{
			return 1;
		}
		else if (first.nums.get_size() > second.nums.get_size())
		{
			return 1;
		}
		else if (first.nums.get_size() < second.nums.get_size())
		{
			return 0;
		}
		else if (!(first.negate && second.negate))
		{
			for (long i = first.nums.get_size() - 1; i >= 0; i--)
			{
				if (first.nums[i] < second.nums[i])
				{
					return 0;
				}
				if (first.nums[i] > second.nums[i])
				{
					return 1;
				}
			}
		}
		else
		{
			for (long i = first.nums.get_size() - 1; i >= 0; i--)
			{
				if (first.nums[i] < second.nums[i])
				{
					return 1;
				}
				if (first.nums[i] > second.nums[i])
				{
					return 0;
				}
			}
		}
		if (flag2)
		{
			return 0;
		}
		return 1;
	}
}

bool operator<(const LN& first, const LN& second)
{
	return universal_comprasion(first, second, false, false) != 1;
}

bool operator>(const LN& first, const LN& second)
{
	return universal_comprasion(first, second, false, false);
}

bool operator<=(const LN& first, const LN& second)
{
	return universal_comprasion(first, second, false, true) != 1;
}

bool operator>=(const LN& first, const LN& second)
{
	return universal_comprasion(first, second, true, false);
}

bool operator==(const LN& first, const LN& second)
{
	return (first >= second) == (second >= first);
}

bool operator!=(const LN& first, const LN& second)
{
	return (first >= second) != (second >= first);
}

LN& LN::operator=(const LN& second)
{
	this->nums = second.nums;
	this->negate = second.negate;
	this->isNan = second.isNan;
	return *this;
}

LN& LN::operator=(LN&& second)
{
	this->nums = second.nums;
	this->negate = second.negate;
	this->isNan = second.isNan;
	second.negate = false;
	second.isNan = false;
	return *this;
}

LN operator-(LN& first, LN& second)
{
	try
	{
		LN result;
		if (!first.negate && second.negate)
		{
			operator_(second);
			result = first + second;
			return result;
		}
		if (first.negate && !second.negate)
		{
			operator_(second);
			result = first + second;
			result.negate = true;
			return result;
		}
		if (first.negate && second.negate)
		{
			operator_(second);
			result = first + second;
			return result;
		}
		int cur = 0;
		int helper;
		if (first < second)
		{
			LN q = second;
			result.negate = true;
			second = first;
			first = q;
		}
		for (long i = 0; i < max(first.nums.get_size(), second.nums.get_size()); i++)
		{
			if (second.nums.get_size() > i && first.nums.get_size() > i)
			{
				helper = first.nums[i] - second.nums[i] + cur;
			}
			else if (second.nums.get_size() > i)
			{
				helper = second.nums[i] + cur;
			}
			else
			{
				helper = first.nums[i] + cur;
			}
			if (helper < 0)
			{
				helper += 10;
				cur = -1;
			}
			else
			{
				cur = 0;
			}
			result.nums.push_back(helper);
		}
		leading_zeros(result, result.nums.get_size());
		if (result.nums.get_size() == 1 && result.nums[0] == 0 && result.negate)
		{
			operator_(result);
		}
		return result;
	} catch (...)
	{
		throw;
	}
}

LN operator+(LN& first, LN& second)
{
	try
	{
		LN result;
		if (first.negate && !second.negate)
		{
			operator_(first);
			result = second - first;
			return result;
		}
		if (!first.negate && second.negate)
		{
			operator_(second);
			result = first - second;
			return result;
		}
		int cur = 0;
		int helper;
		if (first.negate && second.negate)
		{
			result.negate = true;
		}
		for (long i = 0; i < max(first.nums.get_size(), second.nums.get_size()) + 1; i++)
		{
			if (i != max(first.nums.get_size(), second.nums.get_size()))
			{
				if (second.nums.get_size() > i && first.nums.get_size() > i)
				{
					helper = first.nums[i] + second.nums[i] + cur;
				}
				else if (second.nums.get_size() > i)
				{
					helper = second.nums[i] + cur;
				}
				else
				{
					helper = first.nums[i] + cur;
				}
				if (helper > 9)
				{
					cur = helper / 10;
					helper = helper % 10;
				}
				else
				{
					cur = 0;
				}
				result.nums.push_back(helper);
			}
			else
			{
				if (cur != 0)
				{
					result.nums.push_back(cur);
				}
			}
		}
		leading_zeros(result, result.nums.get_size());
		if (result.nums.get_size() == 1 && result.nums[0] == 0 && result.negate)
		{
			operator_(result);
		}
		return result;
	} catch (...)
	{
		throw;
	}
}

void operator_(LN& num)
{
	if (num.negate)
	{
		num.negate = false;
	}
	else
	{
		num.negate = true;
	}
}

LN operator-=(LN& first, LN& second)
{
	try
	{
		first = first - second;
		return first;
	} catch (...)
	{
		throw;
	}
}

LN operator+=(LN& first, LN& second)
{
	try
	{
		first = first + second;
		return first;
	} catch (...)
	{
		throw;
	}
}

LN operator*(LN& first, LN& second)
{
	try
	{
		LN res;
		if (!first.negate && second.negate || first.negate && !second.negate)
		{
			res.negate = true;
		}
		Vector result(first.nums.get_size() * second.nums.get_size(), 0);
		for (long i = 0; i < first.nums.get_size(); i++)
		{
			int cur = 0;
			for (long j = 0; j < second.nums.get_size(); j++)
			{
				int sum = result[i + j] + first.nums[i] * second.nums[j] + cur;
				cur = sum / 10;
				result[i + j] = sum % 10;
			}
			long sum = second.nums.get_size();
			while (cur)
			{
				result[i + sum] = cur % 10;
				sum++;
				cur /= 10;
			}
		}
		res.nums = result;
		leading_zeros(res, res.nums.get_size());
		return res;
	} catch (...)
	{
		throw;
	}
}

LN operator/(LN& first, LN& second)
{
	try
	{
		if (second.nums[0] == 0 && second.nums.get_size() == 1)
		{
			second.isNan = true;
			return second;
		}
		LN res;
		bool s = false;
		if (!first.negate && second.negate || first.negate && !second.negate)
		{
			s = true;
		}

		first.negate = false;
		second.negate = false;

		Vector result(abs(first.nums.get_size() - second.nums.get_size()) + 1, 0);
		res.nums = result;
		for (long i = res.nums.get_size() - 1; i >= 0; i--)
		{
			while (second * res <= first)
			{
				res.nums[i]++;
			}
			res.nums[i]--;
		}
		leading_zeros(res, res.nums.get_size());
		res.negate = s;
		if (res.nums.get_size() == 1 && res.nums[0] == 0 && res.negate)
		{
			operator_(res);
			return res;
		}
		return res;
	} catch (...)
	{
		throw;
	}
}

LN operator%(LN& first, LN& second)
{
	try
	{
		LN helper = first / second;
		if (helper.isNan)
		{
			return helper;
		}
		helper = helper * second;
		LN res = first - helper;
		if (res.negate)
		{
			res += second;
		}
		leading_zeros(res, res.nums.get_size());
		return res;
	} catch (...)
	{
		throw;
	}
}

LN operator~(LN& first)
{
	try
	{
		if (first.negate)
		{
			first.isNan = true;
			return first;
		}

		LN res;
		Vector result(first.nums.get_size() + 1, 0);
		res.nums = result;

		for (long i = res.nums.get_size() - 1; i >= 0; i--)
		{
			while (res * res <= first)
			{
				res.nums[i]++;
			}
			res.nums[i]--;
		}
		leading_zeros(res, res.nums.get_size());
		return res;
	} catch (...)
	{
		throw;
	}
}

LN::LN() {}

LN::LN(const char* str, long length)
{
	try
	{
		if (str[0] == 'N')
		{
			isNan = true;
		}
		else
		{
			if (!(str[0] == '-' && str[1] == '0' && length == 2))
			{
				if (str[0] == '-')
				{
					negate = true;
				}
				for (long i = length - 1; i >= 0; i--)
				{
					if (negate && i == 0)
					{
						break;
					}
					nums.push_back(str[i] - '0');
				}

				int q;
				if (negate)
				{
					q = nums[length - 2];
				}
				else
				{
					q = nums[length - 1];
				}

				if (q == 0 && length != 1)
				{
					for (long i = length - 1; i >= 0; i--)
					{
						if (nums[i] == 0)
						{
							nums.pop_back();
						}
						else
						{
							break;
						}
					}
				}
			}
			else
			{
				nums.push_back(0);
			}
		}
	} catch (...)
	{
		throw;
	}
}

LN::LN(const LN& second)
{
	this->nums = second.nums;
	this->negate = second.negate;
	this->isNan = second.isNan;
}

LN::LN(LN&& second)
{
	this->nums = second.nums;
	this->negate = second.negate;
	this->isNan = second.isNan;
	second.negate = false;
	second.isNan = false;
}

LN::LN(long long num)
{
	try
	{
		if (num < 0)
		{
			negate = true;
		}
		int i = 0;
		Vector numbers(25, 0);
		nums = numbers;
		while (num != 0)
		{
			nums[i] = num % 10;
			num = num / 10;
			i++;
		}
		leading_zeros(*this, nums.get_size());
	} catch (...)
	{
		throw;
	}
}

bool getNan(const LN& num)
{
	return num.isNan;
}

bool getNegate(const LN& num)
{
	return num.negate;
}

long int getSize(const LN& num)
{
	return num.nums.get_size();
}

int getNum(const LN& num, const long index)
{
	return num.nums[index];
}

LN::LN(string_view str)
{
	try
	{
		if (!(str[0] == '-' && str[1] == '0' && str.length() == 2))
		{
			if (str[0] == '-')
			{
				negate = true;
			}
			for (long i = str.length() - 1; i >= 0; i--)
			{
				if (negate && i == 0)
				{
					break;
				}
				nums.push_back(str[i] - '0');
			}

			int q;
			if (negate)
			{
				q = nums[str.length() - 2];
			}
			else
			{
				q = nums[str.length() - 1];
			}

			if (q == 0 && str.length() != 1)
			{
				for (long i = str.length() - 1; i >= 0; i--)
				{
					if (nums[i] == 0)
					{
						nums.pop_back();
					}
					else
					{
						break;
					}
				}
			}
		}
		else
		{
			nums.push_back(0);
		}
	} catch (...)
	{
		throw;
	}
}


//LN operator""_ln(unsigned long long int num)
//{
//	return LN();
//}