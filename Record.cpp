#include "Record.h"

namespace Jereczek
{
	template <typename T>
	Record<T>::Record()
	{
		this->numbers = std::vector<T>(MAX_RECORD_SIZE);
		this->key = -1;
	}

	template <typename T>
	Record<T>::~Record()
	{
	}

	template <typename T>
	void Record<T>::generate()
	{
		MyRand<int> rand(2, MAX_RECORD_SIZE);
		int size = rand();

		for (int i = 0; i < MAX_RECORD_SIZE; ++i)
			this->numbers[i] = (i < size) ? randomNumber() : std::numeric_limits<double>::quiet_NaN();
	}

	template <typename T>
	T Record<T>::randomNumber() const
	{
		MyRand<T> rand(-MAX_GENERATED, MAX_GENERATED);

		return rand();
	}

	template <typename T>
	T& Record<T>::operator[] (const int& i)
	{
		return this->numbers[i];
	}
	
	template <typename T>
	bool Record<T>::operator <= (Record<T>& other) const
	{
		if (this->range() <= other.range())
			return true;
		return false;
	}
	
	template <typename T>
	bool Record<T>::operator >= (Record<T>& other) const
	{
		if (this->range() >= other.range())
			return true;
		return false;
	}

	template <typename T>
	bool Record<T>::operator < (Record<T>& other) const
	{
		if (this->range() < other.range())
			return true;
		return false;
	}


	template <typename T>
	bool Record<T>::operator > (Record<T>& other) const
	{
		if (this->range() > other.range())
			return true;
		return false;
	}


	template <typename T>
	bool Record<T>::operator > (__int64& key) const
	{
		if (this->key > key)
			return true;
		return false;
	}

	template <typename T>
	bool Record<T>::operator == (Record<T>& other) const
	{
		for (int i = 0; i < MAX_RECORD_SIZE; ++i)
		{
			if (this->numbers[i] != other[i])
				return false;
		}
		return true;
	}

	template <typename T>
	bool Record<T>::operator == (__int64& key) const
	{
		if (this->key == key)
				return true;
		return false;
	}

	template <typename T>
	__int64& Record<T>::Key()
	{
		return this->key;
	}

	template <typename T>
	void Record<T>::setKey(__int64 k)
	{
		this->key = k;
	}
	
	template <typename T>
	T Record<T>::range() const
	{
		if (std::isnan(this->numbers[1]))
			return 0.0;

		T min, max;
		min = this->numbers[0];
		max = min;

		for (int i = 1; i < MAX_RECORD_SIZE && !std::isnan(this->numbers[i]); ++i)
		{
			if (min > this->numbers[i])
				min = this->numbers[i];
			else if (max < this->numbers[i])
				max = this->numbers[i];
		}
		return abs(max - min);
	}

	template class Record<double>;
};