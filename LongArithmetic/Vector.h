#include <iostream>

class Vector
{
  private:
	int* array;
	unsigned long capacity;
	long size;

  public:
	Vector()
	{
		try
		{
			array = (int*)malloc(sizeof(int));
			size = 0;
			capacity = 1;
		} catch (...)
		{
			throw;
		}
	}

	Vector(long count, int num)
	{
		array = (int*)malloc(sizeof(int) * count);
		if (array == nullptr)
		{
			throw "wqe";
		}
		for (int i = 0; i < count; i++)
		{
			array[i] = num;
		}
		size = count;
		capacity = count;
	}

	long get_size() const { return size; }

	int& operator[](unsigned long index) const { return array[index]; }

	void push_back(const int num)
	{
		try
		{
			if (capacity < size + 1)
			{
				capacity *= 2;
				array = (int*)realloc(array, sizeof(int) * capacity);
			}
			array[size++] = num;
		} catch (...)
		{
			throw;
		}
	}

	void pop_back() { size--; }

	Vector& operator=(const Vector& vec)
	{
		try
		{
			if (this == &vec)
			{
				return *this;
			}
			free(array);
			array = (int*)malloc(sizeof(int) * vec.size);
			for (int i = 0; i < vec.size; i++)
			{
				array[i] = vec.array[i];
			}
			capacity = vec.capacity;
			size = vec.size;
			return *this;
		} catch (...)
		{
			throw;
		}
	}

	Vector& operator=(Vector&& vec)
	{
		try
		{
			if (this == &vec)
			{
				return *this;
			}
			free(array);
			array = vec.array;
			capacity = vec.capacity;
			size = vec.size;
			vec.capacity = 0;
			vec.size = 0;
			return *this;
		} catch (...)
		{
			throw;
		}
	}

	~Vector() { free(array); }
};
