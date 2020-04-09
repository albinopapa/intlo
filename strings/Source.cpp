#include <conio.h>
#include <fstream>

namespace chili
{
	void print(const char* s)
	{
		for (; *s != 0; s++)
		{
			_putch(*s);
		}
	}
	
	void printStart()
	{
		print("(l)oad, (s)ave, (a)dd, (p)rint or (q)it?");
	}

	void read(char* buf, int maxSize)
	{
		const char* const pEnd = buf + maxSize;
		for (char c = _getch(); c != 13 && (buf + 1 < pEnd); c = _getch(), buf++)
		{
			_putch(c);
			*buf = c;
		}
		*buf = 0;
	}

	int str2int(const char* s)
	{
		// scan to start point
		const char* p = s;
		for (; *p >= '0' && *p <= '9'; p++);
		p--;

		int val = 0;
		int place = 1;
		// convert place values and accumulate
		for (; p >= s; p--)
		{
			val += (*p - '0') * place;
			place *= 10;
		}

		return val;
	}

	int fib(int n)
	{
		if (n < 2)
		{
			return n;
		}
		return fib(n - 1) + fib(n - 2);
	}

	void strrev(char* pl)
	{
		// scan to start point
		char* pr = pl;
		for (; *pr != 0; pr++);
		pr--;

		for (; pl < pr; pl++, pr--)
		{
			const char temp = *pl;
			*pl = *pr;
			*pr = temp;
		}
	}

	void int2str(int val, char* buf, int size)
	{
		char* const pStart = buf;
		char* const pEnd = buf + size;
		for (; val > 0 && (buf + 1 < pEnd); val /= 10, buf++)
		{
			*buf = '0' + val % 10;
		}
		*buf = 0;
		strrev(pStart);
	}
};

class Person
{
public:

	void setString(char* s)
	{
		*buffer = s;

	}
	void setValue(int int_in)
	{
		value = int_in;
	}
	void printString() const
	{
		chili::print(*buffer);
	}
	int getValue() const
	{
		return value; 
	}
	char* getBuffer()
	{
		return *buffer;
	};

private:
	int value = 0;
	char* buffer[10];
};

enum Options
{
	Loading = 108,
	Saving = 115,
	Adding = 97, 
	Printing = 112, 
	Quitting = 113 
};


int main()
{
	static constexpr int maxSize = 20;
	bool IsRunning = true;
	int IndexPersons = 0;
	Person persons[20];
	char NameBufferArray[maxSize][maxSize];
	char switcher = -1;
	while (IsRunning)
	{
		

		chili::printStart();
		switcher = (_getch());
		switch (switcher)
		{
		case Options::Adding:
		{

			chili::print("\nEnter Name:");
			chili::read(NameBufferArray[IndexPersons], maxSize);
			persons[IndexPersons].setString(NameBufferArray[IndexPersons]);
			

			char ValBuffer[maxSize];
			chili::print("\nEnter Value:");
			chili::read(ValBuffer, maxSize);
			persons[IndexPersons].setValue(chili::str2int(ValBuffer));

			++IndexPersons;
			chili::print("\n");

		}
		break; 
		case  Options::Printing:
		{
			for (int i = 0; i < IndexPersons; i++)
			{	
				chili::print("\n");
				persons[i].printString();
				chili::print("\t|");
				for (int j = 0; j < persons[i].getValue(); j++)
				{
					chili::print("=");
				}

			}
			chili::print("\n");
		}
		break;
		 
		case Options::Saving:
		{
			std::ofstream out("names.dat", std::ios::binary);
			
			out.write(reinterpret_cast<char*>(&persons), sizeof(persons));	
			
		}
		break;

		case Options::Loading:
		{
			std::ifstream in("names.dat", std::ios::binary);
			in.read(reinterpret_cast<char*>(persons), sizeof(persons));

			chili::print("loaded File success!");
		}
		break;

		case  Options::Quitting:
		{
			IsRunning = false;
		}
		break;
		}

	}
	return 0;

	
}