#pragma once
#include<iostream>
#include<vector>
#include"MyRand.h"


#define MAX_RECORD_SIZE 15
#define MAX_GENERATED 1000


namespace Jereczek
{
	template<class T>
	class Record
	{
	public:
		Record();
		~Record();

		void generate();
		__int64& Key();
		void setKey(__int64);
		T& operator[](const int &);
		bool operator <= (Record<T> &) const;
		bool operator >= (Record<T> &) const;
		bool operator < (Record<T> &) const;
		bool operator > (Record<T> &) const;
		bool operator > (__int64 &) const;
		bool operator == (Record<T> &) const;
		bool operator == (__int64 &) const;

		friend std::ostream& operator<< (std::ostream& out, Record<T>& r)
		{
			out << r.key << ":: ";
			for (int i = 0; i < MAX_RECORD_SIZE; ++i)
				out << r[i] << ((i != (MAX_RECORD_SIZE - 1)) ? " " : "\n");

			return out;
		};

		friend std::istream& operator>> (std::istream& in, Record<T>& r)
		{
			char c;
			int i;
			for (i = 0; i < MAX_RECORD_SIZE; ++i)
			{
				c = std::cin.peek();
				if (c == '\n')
					break;
				in >> r[i];
			}
			for (; i < MAX_RECORD_SIZE; ++i)
				r[i] = std::numeric_limits<double>::quiet_NaN();

			return in;
		};


	private:
		T randomNumber() const;
		T range() const;

		std::vector<T> numbers;
		__int64 key;
	};
};