#include "Page.h"


namespace Jereczek
{
	template <typename T>
	Page<T>::Page()
	{
		this->ancestor = -1;
		this->last = -1;
		this->address = -1;
		this->pageData = std::vector<PageCell>(PAGE_SIZE + 1, { -1, -1, -1 });
	}

	template <typename T>
	Page<T>::~Page()
	{
	}

	template <typename T>
	void Page<T>::makePlace(const __int64 index)
	{
		this->last = this->pageData[PAGE_SIZE].child;

		for (int i = PAGE_SIZE; i > index; --i)
			this->pageData[i] = this->pageData[i - 1];
	}

	template <typename T>
	bool Page<T>::isRoot()
	{
		if (this->ancestor == -1)
			return true;
		return false;
	}

	template <typename T>
	bool Page<T>::isLeaf()
	{
		if (this->pageData[0].child == -1)
			return true;
		return false;
	}

	template <typename T>
	bool Page<T>::isEmpty()
	{
		if (this->pageData[0].key == -1)
			return true;
		return false;
	}

	template <typename T>
	bool Page<T>::isOverflowed()
	{
		if (this->pageData[PAGE_SIZE].key != -1)
			return true;
		return false;
	}

	template <typename T>
	bool Page<T>::isFull()
	{
		if (this->pageData[PAGE_SIZE - 1].key != -1)
			return true;
		return false;
	}

	template <typename T>
	void Page<T>::display(__int64 depth = 0)
	{
		__int64 size = this->size();
		__int64 j;
		std::cout << " size=" << size << " ancestor=" << this->ancestor << " >\n";

		for (int i = 0; i < size; ++i)
		{
			for (j = 0; j < depth; ++j) std::cout << "\t";
			std::cout << "\t<CELL child=" << this->pageData[i].child << " record=" << this->pageData[i].recordAddress << " key=" << this->pageData[i].key << " />\n";
		}

		if (!this->isLeaf())
		{
			for (j = 0; j < depth; ++j) std::cout << "\t";
			std::cout << "\t<LAST child=" << this->pageData[size].child << " />\n";
		}
	}

	template <typename T>
	__int64& Page<T>::getAncestor()
	{
		return this->ancestor;
	}

	template <typename T>
	void Page<T>::setAncestor(const __int64 a)
	{
		this->ancestor = a;
	}

	template <typename T>
	__int64& Page<T>::getAddress()
	{
		return this->address;
	}

	template <typename T>
	void Page<T>::setAddress(const __int64 a)
	{
		this->address = a;
	}

	template <typename T>
	__int64 Page<T>::size()
	{
		__int64 size = 0;

		for (; size <= PAGE_SIZE && this->pageData[size].key != -1; ++size);

		return size;
	}

	template <typename T>
	__int64 Page<T>::binarySize()
	{
		return ((PAGE_SIZE + 1) * sizeof(this->pageData[0]) + sizeof(this->ancestor));
	}

	template <typename T>
	PageCell& Page<T>::operator[] (const __int64 i)
	{
		return this->pageData[i];
	}

	template <typename T>
	PageCell Page<T>::popFirst()
	{
		PageCell first = this->pageData[0];

		for (int i = 0; i < PAGE_SIZE; ++i)
			this->pageData[i] = this->pageData[i + 1];

		this->pageData[PAGE_SIZE].key = -1;
		this->pageData[PAGE_SIZE].child = this->last;
		this->pageData[PAGE_SIZE].recordAddress = -1;
		this->last = -1;

		return first;
	}

	template <typename T>
	PageCell Page<T>::popLast()
	{
		__int64 size = this->size();
		PageCell last = this->pageData[size - 1];

		if (size > PAGE_SIZE)
		{
			this->pageData[size - 1] = { -1, this->last, -1 };
			this->last = -1;
		}
		else
			this->pageData[size - 1] = { -1, this->pageData[size].child, -1 };

		return last;
	}

	template <typename T>
	PageCell Page<T>::popOverflow()
	{
		PageCell overflow = this->pageData[PAGE_SIZE];

		this->pageData[PAGE_SIZE].key = -1;
		this->pageData[PAGE_SIZE].child = this->last;
		this->pageData[PAGE_SIZE].recordAddress = -1;
		this->last = -1;

		return overflow;
	}

	template <typename T>
	PageCell Page<T>::popMiddle()
	{
		PageCell middle = this->pageData[PAGE_SIZE / 2];

		int i;
		for (i = PAGE_SIZE / 2; i < PAGE_SIZE; ++i)
			this->pageData[i] = this->pageData[i + 1];

		this->pageData[PAGE_SIZE].key = -1;
		this->pageData[PAGE_SIZE].child = this->last;
		this->pageData[PAGE_SIZE].recordAddress = -1;
		this->last = -1;

		return middle;
	}

	template <typename T>
	void Page<T>::createLeft(Page<T>& parent)
	{
		for (int i = 0; i < PAGE_SIZE / 2; ++i)
			this->pageData[i] = parent[i];

		this->pageData[PAGE_SIZE / 2].child = parent[PAGE_SIZE / 2].child;
	}

	template <typename T>
	void Page<T>::resetLast()
	{
		this->last = -1;
	}

	template <typename T>
	void Page<T>::setLast()
	{
		this->last = this->pageData[PAGE_SIZE].child;
	}

	template <typename T>
	void Page<T>::createRight(Page<T>& parent)
	{
		for (int i = PAGE_SIZE / 2 + 1, j = 0; i <= PAGE_SIZE; ++i, ++j)
			this->pageData[j] = parent[i];

		this->pageData[PAGE_SIZE / 2].child = parent.last;
		parent.resetLast();
	}

	template <typename T>
	void Page<T>::createRight()
	{
		for (int i = PAGE_SIZE / 2, j = 0; i <= PAGE_SIZE; ++i, ++j)
		{
			this->pageData[j] = this->pageData[i];
			this->pageData[i] = { -1, -1, -1 };
		}
	}

	template <typename T>
	void Page<T>::createParent()
	{
		this->pageData[0] = this->pageData[PAGE_SIZE / 2];
		for (int i = 1; i <= PAGE_SIZE; ++i)
			this->pageData[i] = { -1, -1, -1 };
	}

	template class Page<double>;
}
