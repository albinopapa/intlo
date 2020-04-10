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

	int strlen( char const* str ) 
	{
		// Count the number of chars from beginning to
		// null-terminator
		int count = 0;
		for( auto const* it = str; it != nullptr && *it != '\0'; ++it, ++count );

		// return count - 1 for actual string length without
		// the null-terminator
		return count - 1;
	}
	char* strcopy( const char* source, char* destination )
	{
		// Safety step, make sure source and destination are valid
		if( source == nullptr || destination == nullptr )return nullptr;

		// Make a copy of the destination pointer
		// so that we can just return it and not have to count the 
		// loop iterations
		auto* dest_iterator = destination;

		// Loop until src_iterator reaches a null-terminator
		for( const auto* src_iterator = source; 
			*src_iterator != '\0'; 
			++src_iterator, ++dest_iterator )
		{
			*dest_iterator = *src_iterator;
		}

		// null-terminate the destination string
		dest_iterator = '\0';

		return destination;
	}
};

class Person
{
public:
	// Since we have to allocate a new buffer for each person
	// We should probably have a constructor and destructor
	// to avoid memory leaks
	Person()
		:
		buffer( new char[ string_length_max + 1 ] )
	{}
	~Person() { 
		// delete[] already checks if nullptr, so no need
		// to check it before hand
		delete[] buffer; 
	}
	void setString(char* s)
	{
		// buffer = s just copies the address of ( *s ) to buffer
		// You need to make a deep copy which means you need to copy 
		// each char in 's' to buffer.
		auto* dest_iterator = buffer;

		for( auto const* src_iterator = s;
			dest_iterator != buffer + string_length_max && *src_iterator != '\0';
			++src_iterator, ++dest_iterator )
		{
			*dest_iterator = *src_iterator;
		}

		*dest_iterator = '\0';
	}
	void setValue(int int_in)
	{
		value = int_in;
	}
	void printString() const
	{
		chili::print(buffer);
	}
	int getValue() const
	{
		return value; 
	}
	char const* getBuffer()const
	{
		return buffer;
	}
	char* getBuffer()
	{
		return buffer;
	}
	void Serialize( std::ofstream& file )const
	{
		file.write( reinterpret_cast< char const* >( &value ), sizeof( value ) );
		file.write( buffer, string_length_max );
	}
	void Deserialize( std::ifstream& file )
	{
		file.read( reinterpret_cast< char* >( &value ), sizeof( value ) );
		file.read( buffer, string_length_max );
	}
private:
	static constexpr int string_length_max = 20;
	int value = 0;
	char* buffer = nullptr;
};

class Database
{
public:
	void AddPerson( char* name, int age )
	{
		persons[ num_persons ].setString( name );
		persons[ num_persons ].setValue( age );
		progress_saved = false;
		++num_persons;
	}
	void Serialize()
	{
		std::ofstream out( "names.dat", std::ios::binary );
		// Write how many records you have
		out.write( reinterpret_cast< const char* >( &num_persons ), sizeof( num_persons ) );

		for( int i = 0; i < num_persons; ++i )
		{
			persons[ i ].Serialize( out );
		}

		progress_saved = true;
	}
	bool Deserialize()
	{
		std::ifstream in( "names.dat", std::ios::binary );
		if( in.is_open() )
		{
			// Read in how many records were saved
			num_persons = 0;
			in.read( reinterpret_cast< char* >( &num_persons ), sizeof( num_persons ) );

			for( int i = 0; i < num_persons; ++i )
			{
				persons[ i ].Deserialize( in );
			}

			return true;
		}

		return false;
	}
	void PrintChart()
	{
		for( int i = 0; i < num_persons; i++ )
		{
			const auto& person = persons[ i ];
			chili::print( "\n" );
			person.printString(); chili::print( "\t| " );

			for( int j = 0; j < person.getValue(); j++ )
				chili::print( "=" );
		}
	}
	bool IsSaved()const { return progress_saved; }

private:
	static constexpr int maxSize = 20;
	Person persons[ maxSize ];
	int num_persons = 0;
	bool progress_saved = false;
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
	Database dbase;
	bool IsRunning = true;
	char switcher = -1;
	while (IsRunning)
	{
		chili::printStart();
		switcher = (_getch());
		switch( switcher )
		{
			case Options::Adding:
			{
				// Limit names to arbitrary 19 chars + null-terminator
				char name_buffer[ 20 ]{};
				chili::print( "\nEnter Name:" );
				chili::read( name_buffer, 20 );
				
				// No one lives past 999 years
				char age_buffer[ 4 ]{};
				chili::print( "\nEnter Value:" );
				chili::read( age_buffer, 4 );
				
				dbase.AddPerson( name_buffer, chili::str2int( age_buffer ) );

				chili::print( "\n" );
				break;
			}
			case  Options::Printing:
				dbase.PrintChart();
				chili::print( "\n" );
				break;
			case Options::Saving:
				dbase.Serialize();
				break;
			case Options::Loading:
				if( !dbase.IsSaved() )
				{
					chili::print( "\nYour progress will be lost, do you want to save first? Y/N: " );
					char response[ 2 ]{};
					chili::read( response, 2 );
					if( response[ 0 ] == 'y' || response[ 0 ] == 'Y' )
						dbase.Serialize();
				}

				if( dbase.Deserialize() )
				{
					chili::print( "\nloaded File success!" );
				}
				else
				{
					chili::print( "\nFailed to load file." );
				}

				chili::print( "\n" );

				break;
			case  Options::Quitting:
				IsRunning = false;
				break;
		}

		chili::print( "\n" );
	}

	return 0;
}