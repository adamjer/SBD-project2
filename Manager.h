#pragma once
#include "BTree.h"
#include <deque>

namespace Jereczek
{
#define RECORDS_TO_GENERATE 50

	template <class T>
	class Manager
	{
	public:
		Manager();
		~Manager();

		void selectInitActivity();
		void selectActivity();

	private:
		void insert();
		void deleteRecord();
		void updateRecord();
		void generateInitial();
		void findRecord(__int64);
		void loadInitialFromFile();
		void generateInitial(const __int64&);

		BTree<T, double> bTree;
	};
}

