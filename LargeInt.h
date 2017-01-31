#if !defined(LARGEINT_HEADER)
#define LARGEINT_HEADER

#include <iostream>
#include <iomanip>

using namespace std;

namespace rsa {
	typedef uint32_t T_DWORD;

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
		enum LIMIT{ LENGTH = 0x400 };
		T_DWORD _len;
		mutable T_DWORD _data[LENGTH] = { 0 };

	public:
		void Print() const {
			ios state(nullptr);

			cout << "0x";
			state.copyfmt(cout);

			cout << hex << uppercase;
			cout << _data[_len - 1];
			cout << setw(sizeof(T_DWORD) * 2) << setfill('0');
			for (int i = _len - 2; i >= 0; i--) {
				cout << _data[i];
			}

			cout.copyfmt(state);
			cout << endl;
		}
		void PrintString() const {
			for (int i = _len-1 ;  i >= 0; i--) {
				char str[] = "    ";
				int c = _data[i], d = 0;
				for (int j = 3; j >= 0; j--) {
					d = c & 0xFF;
					str[j] = d;
					c >>= 8;
				}
				d = 0;
				while (str[d] == 0 && d < 4)
					d++;
				cout << (str + d);
			}
			cout << endl;
		}
	};
}


#endif // LARGEINT_HEADER
