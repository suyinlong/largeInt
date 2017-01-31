#include "LargeInt.h"
#include <iostream>
#include <string>

using namespace std;
using namespace rsa;

int main(int argc, char* argv[]) {
	int a;
	CLargeInt N, D, E, M, M_Source, C;

	E.Create("0x03");
	N.Create("0x84EDCB7BA8BD2554D1965E64C105192DB68528C7531570AD3A93EE68B4256F64684A1D75CC4107D2BC50827E47C106FC3712C68E07A8D96D1B3356958307B101E1EDEBBF2CC0D202BE59E17C7FFA6E3F81D137A60B7C66C5D95AAEB2D4C2AE5F1F36C720734FF7575BE96F8FFD537EC425EBA34DA3C35F0D6E81979380962E780FE65CA51D98EAF25A98D0176A433E5D");
	D.Create("0x589E87A7C5D36E388BB994432B58BB73CF037084E20E4B1E270D499B22C39F984586BE4E882B5A8C7D8B01A9852B59FD7A0C845EAFC5E648BCCCE463ACAFCB56969E9D2A1DD5E1563346B3633F66738CEF07F0475097DC02FB73D86555600BEB595659D2364A5F65F1876F9FE147BEDFE494DDDD133FB6B36C8C648A2FB4051CACAC48A81AB9675AA1E2E87C30168C2B");
	cout << "Enter your string: ";
	string m_source_string;
	getline(cin, m_source_string);
	string hex_string = "0x";
	for (unsigned int i = 0; i < m_source_string.size(); i++) {
		int c = m_source_string[i] / 16;
		int d = m_source_string[i] % 16;
		char cc = c <= 9 ? '0' + c : 'A' + (c - 10);
		char dd = d <= 9 ? '0' + d : 'A' + (d - 10);
		hex_string += cc;
		hex_string += dd;
	}
	M_Source.Create(hex_string.c_str());


	cout << "Your string HEX: ";
	M_Source.Print();
	cout << endl;

	CLargeInt::ExpMod(M_Source, D, N, C);

	cout << "HEX after encryption: ";
	C.Print();
	cout << endl;

	CLargeInt::ExpMod(C, E, N, M);

	cout << "HEX after decryption: ";
	M.Print();
	cout << endl;

	cout << "String after decryption: ";
	M.PrintString();
	cout << endl;

	
	cin >> a;
	cin >> a;
	cin >> a;
	return 0;
}

