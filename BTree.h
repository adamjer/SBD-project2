#pragma once
#include "Record.h"
#include "Page.h"
#include <fstream>

namespace Jereczek
{
	template <class T, typename K>
	class BTree
	{
	public:
		BTree();
		~BTree();

		bool insert(T&);
		void setShow(bool);
		void displayManager(bool inOrder = false);	
		void loadRecord(T&, __int64&);
		void update(__int64&, T&);
		PageCell find(const __int64&);

	private:
		__int64 saveRecordToBase(T&, __int64 address = -1);
		__int64 savePage(Page<K>&, __int64 address = -1);
		void loadPage(Page<K>&, const __int64&);		
		void clearState();
		void display(__int64 depth = 0);
		void displayInOrder();
		void displayState();
		void splitRoot(Page<K>&);
		void splitNode(Page<K>&);
		void split();
		__int64 findRecordPlace(Page<K>&, const __int64&);
		bool compensate(const __int64&);
		bool compensateWithLeftSibling(Page<K>&, Page<K>&, const __int64&);
		bool compensateWithRightSibling(Page<K>&, Page<K>&, const __int64&);

		bool show;
		__int64 loads, saves;
		double counter, wholeSize;
		std::fstream recordBase, treeFile;
		Page<K> actualPage;
	};
}