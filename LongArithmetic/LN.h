#include "Vector.h"

class LN
{
  private:
	bool negate = false;
	bool isNan = false;
	Vector nums;

  public:
	LN();
	LN(const char *str, long length);
	LN(long long num);
	LN(std::string_view num);
	LN(const LN &second);
	LN(LN &&second);

	LN &operator=(const LN &second);
	LN &operator=(LN &&second);
	friend LN operator+(LN &first, LN &second);
	friend LN operator-(LN &first, LN &second);
	friend LN operator*(LN &first, LN &second);
	friend LN operator/(LN &first, LN &second);

	friend LN operator%(LN &first, LN &second);
	friend LN operator~(LN &first);

	friend void operator_(LN &num);

	friend LN operator-=(LN &first, LN &second);
	friend LN operator+=(LN &first, LN &second);

	friend bool operator<(const LN &first, const LN &second);
	friend bool operator>(const LN &first, const LN &second);
	friend bool operator<=(const LN &first, const LN &second);
	friend bool operator>=(const LN &first, const LN &second);
	friend bool operator==(const LN &first, const LN &second);
	friend bool operator!=(const LN &first, const LN &second);

	friend bool getNan(const LN &num);
	friend int getNum(const LN &num, const long index);
	friend long int getSize(const LN &num);
	friend bool getNegate(const LN &num);

	friend void leading_zeros(LN &result, long length);
	friend int universal_comprasion(const LN &first, const LN &second, bool flag, bool flag2);


};

LN operator""_ln(unsigned long long num);