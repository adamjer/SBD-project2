#pragma once
#include <iostream>
#include <vector>

#define PAGE_HALF_SIZE 2
#define PAGE_SIZE (2 * PAGE_HALF_SIZE)

namespace Jereczek
{
	struct PageCell
	{
		__int64 recordAddress;
		__int64 child;
		__int64 key;

		bool isEmpty()
		{
			if (this->key == -1)
				return true;
			return false;
		}

		void copy(PageCell& p)
		{
			this->key = p.key;
			this->recordAddress = p.recordAddress;
		}
	};

	template <class T>
	class Page
	{
	public:
		Page();
		~Page();

		void makePlace(const __int64);
		__int64& getAncestor();
		__int64& getAddress();
		__int64 size();
		__int64 binarySize();
		PageCell popFirst();
		PageCell popOverflow();
		PageCell popLast();
		PageCell popMiddle();
		void setAncestor(const __int64);
		void setAddress(const __int64);
		void setLast();
		void createLeft(Page<T>&);
		void createRight(Page<T>&);
		void createRight();
		void createParent();
		void resetLast();
		void display(__int64 depth = 0);
		bool isRoot();
		bool isLeaf();
		bool isOverflowed();
		bool isEmpty();
		bool isFull();
		PageCell& operator[] (const __int64);

	private:
		__int64 ancestor, last, address;
		std::vector<PageCell> pageData;
	};
}

