#if !defined(LARGEINT_HEADER)
#define LARGEINT_HEADER

#include <vector>

namespace rsa {
	typedef unsigned long T_DWORD;

	class CLargeInt	{
	public:
		static void Copy(const CLargeInt& source,CLargeInt& target);

		static bool LargerEqual(const CLargeInt& source,const CLargeInt& target,T_DWORD offset = 0);
		static bool Equal(const CLargeInt& source,const CLargeInt& target);

		static void Mul(const CLargeInt& faciend,T_DWORD multiplier,CLargeInt& product);

		static void Div(const CLargeInt& dividend,T_DWORD divisor,CLargeInt& quotient,T_DWORD& residual);

		static void RCR(CLargeInt& n);
		static void RCL(CLargeInt& n);

		static void Add(const CLargeInt& augend,const CLargeInt& addend,CLargeInt& sum,T_DWORD offset = 0);
		static void Sub(const CLargeInt& minuend,const CLargeInt& subtrahend,CLargeInt& difference,T_DWORD offset = 0);
		static void Mul(const CLargeInt& faciend,const CLargeInt& multiplier,CLargeInt& product);
		static void Div(const CLargeInt& dividend,const CLargeInt& divisor,CLargeInt& quotient,CLargeInt& residual);
		static void ExpMod(const CLargeInt& source,const CLargeInt& exponent,const CLargeInt& modulo,CLargeInt& result);


	public:
		CLargeInt(T_DWORD value = 0);
		CLargeInt(const char * str);
		CLargeInt(const CLargeInt& other);
		void Create(const char* str);

		~CLargeInt();
	private:
		enum LIMIT{ LENGTH = 0x3FF};
		T_DWORD _len;
		mutable T_DWORD _data[LENGTH];


	public:
		void Print() const {
			printf("0x");
			for( int i = _len-1 ; i >= 0; i--) {
				char str[] = "%08X";
				str[2] = '0' + sizeof(T_DWORD) * 2;
				if( i != (_len-1) )printf(str,_data[i]);
				else printf("%X",_data[i]);
			}
			printf("\n");
		}
		void PrintString() const {
			for ( int i = _len-1 ;  i >= 0; i--) {
				char str[] = "    ";
				int c = _data[i];
				int d = c & 0xFF;
				if (d != 0)
				{
					str[3] = c & 0xFF;
				}
				c = c >> 8;
				d = c & 0xFF;
				if (d != 0)
					str[2] = c & 0xFF;
				c = c >> 8;
				d = c & 0xFF;
				if (d != 0)
					str[1] = c & 0xFF;
				c = c >> 8;
				d = c & 0xFF;
				if (d != 0)
					str[0] = c & 0xFF;
				printf("%s",str);

			}
			printf("\n");
		}
	};
}


#endif // LARGEINT_HEADER
