#include "BTree.h"

//seekg - reading pointer
//seekp - writing pointer

namespace Jereczek
{
	template <typename T, typename K>
	BTree<T, K>::BTree()
	{
		this->loads = 0;
		this->show = false;
		this->saves = 0;
		this->recordBase.open("main.bt", std::fstream::trunc | std::fstream::binary | std::fstream::out | std::fstream::in);
		this->treeFile.open("tree.bt", std::fstream::trunc | std::fstream::binary | std::fstream::out | std::fstream::in);

		this->savePage(this->actualPage);
	}

	template <typename T, typename K>
	BTree<T, K>::~BTree()
	{
		this->recordBase.close();
		this->treeFile.close();
	}

	template <typename T, typename K>
	__int64 BTree<T, K>::saveRecordToBase(T& record, __int64 address = -1)
	{
		__int64 position;
		if (address = -1)
		{
			this->recordBase.seekp(0, this->recordBase.end);
			position = this->recordBase.tellp();
		}
		else
		{
			this->recordBase.seekp(address);
			position = address;
		}
		this->recordBase.write(reinterpret_cast<char*>(&record.Key()), sizeof(record.Key()));
		this->recordBase.write(reinterpret_cast<char*>(&record[0]), MAX_RECORD_SIZE * sizeof(record[0]));
		this->recordBase.flush();

		return position;
	}

	template <typename T, typename K>
	void BTree<T, K>::loadRecord(T& record, __int64& address)
	{
		this->recordBase.seekg(address);
		this->recordBase.read(reinterpret_cast<char*>(&record.Key()), sizeof(record.Key()));
		this->recordBase.read(reinterpret_cast<char*>(&record[0]), MAX_RECORD_SIZE * sizeof(record[0]));
	}

	template <typename T, typename K>
	void BTree<T, K>::setShow(bool s)
	{
		this->show = s;
	}

	template <typename T, typename K>
	__int64 BTree<T, K>::savePage(Page<K>& page, __int64 address = -1)
	{
		__int64 position;

		if (address == -1)
			this->treeFile.seekp(0, this->treeFile.end);
		else
			this->treeFile.seekp(address);
		position = this->treeFile.tellp();

		page.setAddress(position);
		this->treeFile.write(reinterpret_cast<char*>(&page.getAncestor()), sizeof(page.getAncestor()));
		this->treeFile.write(reinterpret_cast<char*>(&page[0]), (PAGE_SIZE + 1) * sizeof(page[0]));
		this->treeFile.flush();

		++this->saves;

		return position;
	}

	template <typename T, typename K>
	void BTree<T, K>::displayState()
	{
		std::cout << "Page saves: " << this->saves << " Page loads: " << this->loads << "\n";
	}

	template <typename T, typename K>
	void BTree<T, K>::update(__int64& key, T& record)
	{
		PageCell update = this->find(key);
		if (update.isEmpty())
			std::cout << "Couldn't find record with key=" << key << "\n";
		else
		{
			this->recordBase.seekp(update.recordAddress + sizeof(key));
			this->recordBase.write(reinterpret_cast<char*>(&record[0]), MAX_RECORD_SIZE * sizeof(record[0]));
			this->recordBase.flush();
			std::cout << "\nRecord with key=" << key << " updated!\n";
			if (this->show)
				this->displayManager(true);
		}
	}

	template <typename T, typename K>
	PageCell BTree<T, K>::find(const __int64& key)
	{
		this->clearState();
		__int64 place = 0;
		this->actualPage.setAddress(0);
		PageCell p = { -1, -1, -1 };

		while (true)
		{
			this->loadPage(this->actualPage, this->actualPage.getAddress());

			//key found
			if ((place = this->findRecordPlace(this->actualPage, key)) < 0)
			{
				for (place = 0; this->actualPage[place].key != key; ++place);
				p = this->actualPage[place];
				break;
			}

			if (!this->actualPage.isLeaf())
				this->actualPage.setAddress(this->actualPage[place].child);
			else
				break;
		}

		this->displayState();
		return p;
	}

	template <typename T, typename K>
	void BTree<T, K>::displayManager(bool inOrder = false)
	{
		this->clearState();
		this->counter = this->wholeSize = 0;
		this->actualPage.setAddress(0);

		if (inOrder)
			this->displayInOrder();
		else
			this->display();

		if (!inOrder)
		{
			double alpha = (this->wholeSize / (this->counter * PAGE_SIZE)) * 100;
			std::cout << "ALPHA=" << alpha << "%\n\n";
		}
		this->counter = this->wholeSize = 0;
	}

	template <typename T, typename K>
	void BTree<T, K>::displayInOrder()
	{
		T record;
		this->loadPage(this->actualPage, this->actualPage.getAddress());
		if (this->actualPage.isRoot() && this->actualPage.isLeaf())
		{
			for (int i = 0; i < this->actualPage.size(); ++i)
			{
				this->loadRecord(record, this->actualPage[i].recordAddress);
				std::cout << "key=" << this->actualPage[i].key << " " << record;
			}
		}
		else if (this->actualPage.isLeaf())
		{
			for (int i = 0; i < this->actualPage.size(); ++i)
			{
				this->loadRecord(record, this->actualPage[i].recordAddress);
				std::cout << "key=" << this->actualPage[i].key << " " << record;
			}
		}
		else if (!this->actualPage.isLeaf())
		{
			for (int i = 0; i <= this->actualPage.size(); ++i)
			{
				this->actualPage.setAddress(this->actualPage[i].child);
				this->displayInOrder();
				if (!this->actualPage[i].isEmpty())
				{
					this->loadRecord(record, this->actualPage[i].recordAddress);
					std::cout << "key=" << this->actualPage[i].key << " " << record;
				}
			}
		}
		//std::cout << 

		if (!this->actualPage.isRoot())
			this->loadPage(this->actualPage, this->actualPage.getAncestor());
	}

	template <typename T, typename K>
	void BTree<T, K>::display(__int64 depth = 0)
	{
		int i;
		this->loadPage(this->actualPage, this->actualPage.getAddress());

		for (i = 0; i < depth; ++i) std::cout << "\t";
		std::cout << "<PAGE address=" << this->actualPage.getAddress();
		this->actualPage.display(depth);
		for (i = 0; i < depth; ++i) std::cout << "\t";
		std::cout << "</PAGE>\n";

		if (!this->actualPage.isRoot())
		{
			wholeSize += this->actualPage.size();
			++counter;
		}

		if (!this->actualPage.isLeaf())
		{
			for (i = 0; i <= this->actualPage.size(); ++i)
			{
				if (this->actualPage[i].child != -1)
				{
					this->actualPage.setAddress(this->actualPage[i].child);
					this->display(depth + 1);
				}
			}
		}

		if (!this->actualPage.isRoot())
			this->loadPage(this->actualPage, this->actualPage.getAncestor());
	}

	template <typename T, typename K>
	void BTree<T, K>::loadPage(Page<K>& page, const __int64& address)
	{
		this->treeFile.seekg(address);
		page.setAddress(address);
		this->treeFile.read(reinterpret_cast<char*>(&page.getAncestor()), sizeof(page.getAncestor()));
		this->treeFile.read(reinterpret_cast<char*>(&page[0]), (PAGE_SIZE + 1) * sizeof(page[0]));
		page.resetLast();
		++this->loads;
	}

	template <typename T, typename K>
	void BTree<T, K>::clearState()
	{
		this->loads = 0;
		this->saves = 0;
	}

	template <typename T, typename K>
	void BTree<T, K>::splitRoot(Page<K>& page)
	{
		Page<K> left, right;
		left.createLeft(page);
		right.createRight(page);
		page.createParent();

		left.setAncestor(page.getAddress()); right.setAncestor(page.getAddress());
		this->savePage(left);
		this->savePage(right);

		__int64 place = this->findRecordPlace(page, left[PAGE_SIZE / 2].key);
		page[place].child = left.getAddress();
		page[place + 1].child = right.getAddress();
		this->savePage(page, page.getAddress());

		if (!left.isLeaf())
		{
			for (int i = 0; i <= left.size(); ++i)
			{
				this->treeFile.seekp(left[i].child);
				this->treeFile.write(reinterpret_cast<char*>(&left.getAddress()), sizeof(left.getAncestor()));
				this->treeFile.seekp(right[i].child);
				this->treeFile.write(reinterpret_cast<char*>(&right.getAddress()), sizeof(left.getAncestor()));
			}
		}
	}

	template <typename T, typename K>
	void BTree<T, K>::split()
	{
		PageCell transfer;
		Page<K> left, parent;
		__int64 place;

		left.createLeft(this->actualPage);
		this->loadPage(parent, this->actualPage.getAncestor());
		transfer = this->actualPage.popMiddle();
		this->actualPage.createRight();

		this->savePage(this->actualPage, this->actualPage.getAddress());
		left.setAncestor(parent.getAddress());
		this->savePage(left);
		transfer.child = left.getAddress();

		place = this->findRecordPlace(parent, transfer.key);
		parent.makePlace(place);
		parent[place] = transfer;

		this->actualPage = parent;
		while (true)
		{
			if (this->actualPage.isOverflowed())
			{
				if (this->actualPage.isRoot())
				{
					this->splitRoot(this->actualPage);
					break;
				}
				else if (!this->compensate(this->actualPage[0].key))
				{
					this->loadPage(parent, this->actualPage.getAncestor());

					left.createLeft(this->actualPage);
					transfer = this->actualPage.popMiddle();
					this->actualPage.createRight();

					this->savePage(this->actualPage, this->actualPage.getAddress());
					left.setAncestor(parent.getAddress());
					this->savePage(left);
					transfer.child = left.getAddress();

					place = this->findRecordPlace(parent, transfer.key);
					parent.makePlace(place);
					parent[place] = transfer;

					if (!left.isLeaf())
					{
						for (int i = 0; i <= left.size(); ++i)
						{
							this->treeFile.seekp(left[i].child);
							this->treeFile.write(reinterpret_cast<char*>(&left.getAddress()), sizeof(left.getAncestor()));
						}
					}
					this->actualPage = parent;
				}
			}
			else
			{
				this->savePage(this->actualPage, this->actualPage.getAddress());
				break;
			}
		}
	}

	template <typename T, typename K>
	bool BTree<T, K>::compensateWithRightSibling(Page<K>& parent, Page<K>& sibling, const __int64& placeInParent)
	{
		if (parent[placeInParent].isEmpty())
			return false;

		this->loadPage(sibling, parent[placeInParent + 1].child);
		if (!sibling.isFull())
		{
			PageCell transfer;
			while (std::abs(sibling.size() - this->actualPage.size()) > 1)
			{
				transfer.copy(parent[placeInParent]);
				sibling.makePlace(0);
				sibling[0] = transfer;
				transfer = this->actualPage.popLast();
				sibling[0].child = this->actualPage[this->actualPage.size()].child;
				this->actualPage[this->actualPage.size()].child = transfer.child;
				parent[placeInParent].copy(transfer);

				if (!sibling.isLeaf())
				{
					this->treeFile.seekp(sibling[0].child);
					this->treeFile.write(reinterpret_cast<char*>(&sibling.getAddress()), sizeof(sibling.getAddress()));
				}
			}

			this->savePage(this->actualPage, this->actualPage.getAddress());
			this->savePage(parent, parent.getAddress());
			this->savePage(sibling, sibling.getAddress());

			return true;
		}
		else
			return false;
	}

	template <typename T, typename K>
	bool BTree<T, K>::compensateWithLeftSibling(Page<K>& parent, Page<K>& sibling, const __int64& placeInParent)
	{
		this->loadPage(sibling, parent[placeInParent - 1].child);
		if (!sibling.isFull())
		{
			PageCell transfer;
			while (std::abs(sibling.size() - this->actualPage.size()) > 1)
			{
				transfer = parent[placeInParent - 1];
				sibling[sibling.size()].copy(transfer);
				transfer = this->actualPage.popFirst();
				parent[placeInParent - 1].copy(transfer);
				sibling[sibling.size()].child = transfer.child;

				if (!sibling.isLeaf())
				{
					this->treeFile.seekp(transfer.child);
					this->treeFile.write(reinterpret_cast<char*>(&sibling.getAddress()), sizeof(sibling.getAddress()));
				}
			}

			this->savePage(this->actualPage, this->actualPage.getAddress());
			this->savePage(parent, parent.getAddress());
			this->savePage(sibling, sibling.getAddress());

			return true;
		}
		else
			return false;
	}

	template <typename T, typename K>
	bool BTree<T, K>::compensate(const __int64& key)
	{
		Page<K> parent, sibling;
		__int64 placeInParent;

		this->loadPage(parent, this->actualPage.getAncestor());
		placeInParent = this->findRecordPlace(parent, key);

		if (placeInParent == 0)
			return this->compensateWithRightSibling(parent, sibling, placeInParent);

		else
		{
			if (this->compensateWithLeftSibling(parent, sibling, placeInParent))
				return true;
			return this->compensateWithRightSibling(parent, sibling, placeInParent);
		}

		return true;
	}

	template <typename T, typename K>
	__int64 BTree<T, K>::findRecordPlace(Page<K>& page, const __int64& key)
	{
		__int64 place = -2;

		if (key != page[0].key)
		for (place = 0; page[place].key != -1 && key > page[place].key; ++place)
		{
			if (key == page[place + 1].key)
			{
				place = -1;
				break;
			}
		}

		return place;
	}

	template <typename T, typename K>
	bool BTree<T, K>::insert(T& record)
	{
		this->clearState();
		this->recordBase.seekp(0, this->recordBase.end);
		__int64 recordAddress = this->recordBase.tellp();

		__int64 placeToInsert = 0;
		this->actualPage.setAddress(0);
		while (true)
		{
			this->loadPage(this->actualPage, this->actualPage.getAddress());

			//key already used
			if ((placeToInsert = this->findRecordPlace(this->actualPage, record.Key())) < 0)
				return false;

			if (this->actualPage.isLeaf())
			{
				this->actualPage.makePlace(placeToInsert);
				this->actualPage[placeToInsert] = { recordAddress, -1, record.Key() };

				if (this->actualPage.isOverflowed())
				{
					if (this->actualPage.isRoot())
						this->splitRoot(this->actualPage);
					else if (!this->compensate(record.Key()))
					{
						this->split();
						break;
					}
				}
				else
					this->savePage(this->actualPage, this->actualPage.getAddress());
				break;
			}
			else
				this->actualPage.setAddress(this->actualPage[placeToInsert].child);

		}

		this->saveRecordToBase(record);
		std::cout << "\nAdded record= " << record;
		this->displayState();
		if (this->show)
			this->displayManager();

		return true;
	}

	template class BTree<Record<double>, double>;
}
