#include <iostream>
#include <Windows.h>
#include <string>
#include <new>
#include <map>
#include <assert.h>
#include <algorithm>
#include <fstream>
#include <functional>
#include <sstream>
#include <vector>
#include <iterator>
typedef unsigned int uint;
using namespace std;


//ассоциативный массив(используется двоичный поиск) 
template<typename KEY, typename T>
class marray {

	struct info {
		KEY key;
		T   data;
	};

private:
	info* arr;
	uint  cnt;
	uint  mem;
public:
	marray(void) :arr(NULL),
		cnt(0),
		mem(16) {}
	~marray() {
		this->clear();
	}

	marray(const marray&);
	marray& operator = (const marray&);
public:

	//добавление ключ=значение
	T* add(const KEY& key, const T& data) {
		uint i = _bin_find(key);
		if (!_heap_alloc_())
			return NULL;

		//дубликат
		if (key == arr[i].key)
			return NULL;

		if (i < cnt) {
			info* pb = arr + cnt;
			info* pa = arr + i;
			for (; pb > pa; --pb)
				*pb = *(pb - 1);
		}
		arr[i].key = key;
		arr[i].data = data;

		++cnt;
		return &arr[i].data;
	}

	//удаление данных по-ключу
	 void remove(const KEY& key) {
		uint i = _bfind_info(key);
		if (i != (uint)-1) {
			--cnt;
			info* pa = arr + i;
			info* pb = arr + cnt;
			for (; pa < pb; ++pa)
				*pa = *(pa + 1);

			if (!cnt)
				this->clear();
		}
	}

	//поиск данных по-ключу O(log(n))
	T* find(const KEY& key) {
		uint i = _bfind_info(key);
		return (i != (uint)-1) ? &arr[i].data : NULL;
	}
	const T* find(const KEY& key) const {
		const uint i = _bfind_info(key);
		return (i != (uint)-1) ? &arr[i].data : NULL;
	}

	uint empty(void) const {
		if (arr == NULL)
		{
			cout << "Массив пуст" << endl;
			return 0;
		}
		else
		{
			cout << "Массив имеет элементы" << endl;
			return 1;
		}
	}

	//кол-во элементов
	uint size(void) const {
		return cnt;
	}

	//удаление всего массива
	void clear(void) {
		if (arr != NULL)
			delete[] arr;
		arr = NULL;
		cnt = 0;
		mem = 16;
	}

	//получение ключа по-индексу только для чтения
	const KEY& keyAt(uint i) const {
		return arr[i].key;
	}

	//доступ к значению по-индексу
	T& valueAt(uint i) const {
		return arr[i].data;
	}

	//получение доступа как к обычному массиву
	const T& operator [] (const KEY& key) const {
		const uint i = _bfind_info(key);

		assert(i != (uint)-1);
		return arr[i].data;
	}

	T& operator [] (const KEY& key) {
		uint i = _bin_find(key);
		if ((i < cnt) && (key == arr[i].key))
			return arr[i].data;

		if (_heap_alloc_()) {
			if (i < cnt) {
				info* pb = arr + cnt;
				info* pa = arr + i;
				for (; pb > pa; --pb)
					*pb = *(pb - 1);
			}
			arr[i].key = key;
			arr[i].data = T();
			++cnt;
		}
		else
			assert(false);

		return arr[i].data;
	}

private:

	//выделение динамической памяти для массива
	bool _heap_alloc_(void) {
		if (arr == NULL) {
			arr = new (std::nothrow) info[mem];
			if (arr == NULL)
				return false;
		}
		else if ((cnt + 1) >= mem) {
			uint  tmem = cnt + mem / 2;
			info* tmp = new (std::nothrow) info[tmem];
			if (tmp == NULL)
				return false;

			const info* p = arr;
			const info* e = arr + cnt;
			info* d = tmp;
			while (p != e)
				*d++ = *p++;

			delete[] arr;
			arr = tmp;
			mem = tmem;
		}
		return true;
	}

	//двоичный поиск вхождение ключа для вставки
	uint _bin_find(const KEY& key) {
		if (!cnt || (key < arr[0].key))
			return 0;
		else if (arr[cnt - 1].key < key)
			return cnt;

		uint m = 0, f = 0, l = cnt;

		while (f < l) {
			m = (f + l) >> 1;
			if (key < arr[m].key)
				l = m;
			else {
				if (key == arr[m].key)
					return m;
				f = m + 1;
			}
		}
		return f;
	}

	//двоичный поиск вхождения ключа
	uint _bfind_info(const KEY& key) const {
		if (!cnt || (key < arr[0].key))
			return (uint)-1;
		else if (arr[cnt - 1].key < key)
			return (uint)-1;

		uint m = 0, f = 0, l = cnt;

		while (f < l) {
			m = (f + l) >> 1;
			if (key < arr[m].key)
				l = m;
			else {
				if (key == arr[m].key)
					return m;
				f = m + 1;
			}
		}
		return (uint)-1;
	}
};
class Functor
{
public:
	Functor(const string& fileName)
	{
		mCorrectFile = true;
		mFile.open(fileName.c_str());
		if (mFile.fail())
		{
			mCorrectFile = false;
		}
	}
	Functor()
	{
		mFile.close();
	}
	void operator() (const pair<string, string>& p)
	{
		mFile << "\t['" << p.first << "'] = '" << p.second << '\'' << endl;
	}

private:
	ofstream mFile;
	bool mCorrectFile;
};

int main() {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	marray<string, int> arr;
	string a;
	int com, b;
	cout << "Выбор пункта меню " << endl;

	do {
		system("cls");
		system("pause");
		cout << " " << endl;
		cout << "| Меню:|" << endl;
		cout << "+----------------------------------------------+" << endl;
		cout << "1.Добавление елементов " << endl;
		cout << "2.Поиск элементов по ключу " << endl;
		cout << "3.Количество элементов" << endl;
		cout << "4.Удаление по ключу" << endl;
		cout << "5.Вывод элементов на экран" << endl;
		cout << "6.Сохранить в файл" << endl;
		cout << "7.Очистить контейнер" << endl;
		cout << "8.Проверка на наличие элементов в контейнере" << endl;
		cout << "9.Поиск по индексу" << endl;
		cout << "+----------------------------------------------+" << endl;
		cin >> com;
		switch (com)
		{
		case 1: {
			
			cout << "Введите ключ " << endl;
			cin >> a;
			cout << "Введите введите значение " << endl;
			cin >> b;
			arr.add(a, b);

			break;
		}
		case 2: {
			cout << "Введите ключ для поиска" << endl;
			cin >> a;
			cout << arr[a] << endl;
			break;
		}
		case 3: {
			cout << "Количество элементов " << endl;
			cout << arr.size() << endl;
			system("pause");
			break;
		}
		case 4: {
			cout << "Введите ключ элемента, который желаете удалить" << endl;
			cin >> a;
			arr.remove (a);
			if (arr.find(a) == NULL)
				cout << "Элемент удалён" << endl;
			system("pause");
			break;
		}
		case 5: {
			for (uint i = 0; i < arr.size(); ++i) {
				cout << arr.keyAt(i) << " = " << arr.valueAt(i) << endl;
	}
			system("pause");
			break;
		}
		case 6: {
			ofstream fout;
			fout.open("file.txt");
			for (uint i = 0; i < arr.size(); ++i)
			{
				fout << arr.keyAt(i) << " = " << arr.valueAt(i) << endl;
			}
			fout.close();

		}
		case 7: {
			arr.clear();
			break;
		}
		case 8: {
			cout << "Проверка на наличие элементов " << endl;
			cout <<arr.empty() << endl;
			system("pause");
			break;
		}
		case 9: {
			cout << "Введите индекс" << endl;
			cin >> b;
			cout << arr.keyAt(b) << endl;;
			system("pause");
		}
				
		}
	} while (true);
	//system("pause");
	return 0;
}
