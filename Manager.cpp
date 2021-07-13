#include "Manager.h"


namespace Jereczek
{
	template <typename T>
	Manager<T>::Manager()
	{
	}

	template <typename T>
	Manager<T>::~Manager()
	{
	}

	template <typename T>
	void Manager<T>::loadInitialFromFile()
	{
		std::fstream file;
		T record;
		__int64 key;
		char sign;

		file.open("testFile.test", std::fstream::binary | std::fstream::in);

		if (file.good())
		{
			while (file.peek() != EOF)
			{
				file.read(reinterpret_cast<char*>(&sign), sizeof(sign));
				if (sign == 'I')
				{
					file.read(reinterpret_cast<char*>(&record.Key()), sizeof(record.Key()));
					file.read(reinterpret_cast<char*>(&record[0]), MAX_RECORD_SIZE * sizeof(record[0]));
					this->bTree.insert(record);
				}
				else if (sign == 'U')
				{
					file.read(reinterpret_cast<char*>(&key), sizeof(key));
					file.read(reinterpret_cast<char*>(&record[0]), MAX_RECORD_SIZE * sizeof(record[0]));
					this->bTree.update(key, record);
				}
			}

			file.close();
		}
		else
			std::cout << "Sth wrong!\n";
	}

	template <typename T>
	void Manager<T>::findRecord(__int64 key)
	{
		PageCell cell = this->bTree.find(key);

		if (cell.isEmpty())
			std::cout << "Couldn't find record!\n";
		else
		{
			T r;
			this->bTree.loadRecord(r, cell.recordAddress);
			std::cout << "Record found key=" << r;
		}
	}

	template <typename T>
	void Manager<T>::generateInitial(const __int64& n)
	{
		MyRand<__int64> rand(0, n * 50);
		T r;
		for (int i = 0; i < n; ++i)
		{
			r.generate();
			for (int j = 0; j < 5; ++j)
			{
				r.setKey(rand());
				if (this->bTree.insert(r))
					break;
			}
		}
		//this->bTree.displayManager();
	}

	template <typename T>
	void Manager<T>::generateInitial()
	{
		std::fstream testFile;
		MyRand<__int64> rand(0, RECORDS_TO_GENERATE * 50);
		testFile.open("testFile.test", std::fstream::binary | std::fstream::out);
		T record;
		std::deque<__int64> stack;

		if (testFile.good())
		{
			for (int i = 0; i < RECORDS_TO_GENERATE; ++i)
			{
				record.generate();
				record.setKey(rand());
				testFile.write(reinterpret_cast<char*>("I"), sizeof(char));
				testFile.write(reinterpret_cast<char*>(&record.Key()), sizeof(record.Key()));
				testFile.write(reinterpret_cast<char*>(&record[0]), MAX_RECORD_SIZE * sizeof(record[0]));

				if (stack.size() < (RECORDS_TO_GENERATE * 0.1) && record.Key() < (RECORDS_TO_GENERATE * 25))
					stack.push_back(record.Key());
			}
			while (stack.size() != 0)
			{
				testFile.write(reinterpret_cast<char*>("U"), sizeof(char));
				testFile.write(reinterpret_cast<char*>(&stack.front()), sizeof(record.Key()));
				stack.pop_front();
				record.generate();
				testFile.write(reinterpret_cast<char*>(&record[0]), MAX_RECORD_SIZE * sizeof(record[0]));
			}
			testFile.close();
		}
	}

	template <typename T>
	void Manager<T>::selectInitActivity()
	{
		std::cout << "Do you want to show content after every change? (Y/N)";

		char sign = 0;
		while (true)
		{
			std::cin >> sign;
			if (tolower(sign) == 'y')
			{
				this->bTree.setShow(true);
				break;
			}
			else if (tolower(sign) == 'n')
				break;
		}
		std::cout << "What do you want to do: \n";
		std::cout << "1. Load initial actions from file.\n";
		std::cout << "2. Generate initial tree values.\n";
		std::cout << "3. Continue.\n";
		std::cout << "4. Generate test file.\n";

		__int64 choose = 0;
		while (true)
		{
			std::cin >> choose;
			if (choose == 1)
			{
				this->loadInitialFromFile();
				break;
			}
			else if (choose == 2)
			{
				std::cout << "How many records to generate: ";
				choose = 0;
				while (true)
				{
					std::cin >> choose;
					if (choose > 0)
						break;
				}
				this->generateInitial(choose);
				break;
			}
			else if (choose == 3)
				break;
			else if (choose == 4)
				this->generateInitial();
		}
	}

	template <typename T>
	void Manager<T>::selectActivity()
	{
		std::cout << "What do you want to do: \n";
		std::cout << "1. Insert record.\n";
		std::cout << "2. Delete record.\n";
		std::cout << "3. Update record.\n";
		std::cout << "4. Find record.\n";
		std::cout << "5. Show tree file.\n";
		std::cout << "6. Show tree structure.\n";
		std::cout << "7. Exit.\n";

		__int64 choose = 0;
		T record;
		while (true)
		{
			std::cin >> choose;
			if (choose == 1)
			{
				std::cout << "Insert new record:";
				std::cin >> choose;
				std::cin >> record;
				record.setKey(choose);
				this->bTree.insert(record);
			}
			else if (choose == 2)
				std::cout << "TODO :(\n";
			else if (choose == 3)
			{
				std::cout << "Update record:";
				std::cin >> choose;
				std::cin >> record;
				this->bTree.update(choose, record);
			}
			else if (choose == 4)
			{
				std::cout << "Type key to find ";
				choose = 0;
				while (true)
				{
					std::cin >> choose;
					if (choose > 0)
						break;
				}
				this->findRecord(choose);
			}
			else if (choose == 5)
				this->bTree.displayManager();
			else if (choose == 6)
				this->bTree.displayManager(true);
			else if (choose == 7)
				break;
		}
	}


	template class Manager<Record<double>>;
}
