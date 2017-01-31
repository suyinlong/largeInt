#include "LargeInt.h"

namespace rsa {
	
	CLargeInt::CLargeInt(T_DWORD value) : _len(1) {
		_data[0] = value;
	}
	
	CLargeInt::CLargeInt(const CLargeInt& other) {
		Copy(other,*this);
	}
	
	CLargeInt::CLargeInt(const char	* str):	_len(1) {
		Create(str);
	}

	void CLargeInt::Create(const char* str) {
		_data[0] = 0;
		if (str && strlen(str) > 2 && (strncmp(str, "0x", 2) == 0 || strncmp(str, "0X", 2) == 0)) {
			_len = (strlen(str) - 3) / 8 + 1;
			
			for (unsigned int i = 0; i < strlen(str) - 2; i++) {
				char ch = str[strlen(str) - 1 - i];
				T_DWORD n = 0;
				switch (ch) {
				case '0':case '1':case '2':	case '3':case '4':case '5':case	'6':case '7':case '8':case '9':
					n = (ch - '0');
					break;
				case 'A':case 'B':case 'C':case	'D':case 'E':case 'F':
					n = (ch - 'A' + 10);
					break;
				case 'a':case 'b':case 'c':case	'd':case 'e':case 'f':
					n = (ch - 'a' + 10);
					break;
				}
				_data[i / 8] |= (n << (i % 8 * 4));
			}
		}
		else { //need to handle hex string without '0x' as prefix
			__asm int 3;
		}
	}
	
	CLargeInt::~CLargeInt() {
		
	}
	
	
	void CLargeInt::Copy(const CLargeInt& source,CLargeInt&	target) {
		// avoid self-copy
		if( &source != &target ) {
			target._len = source._len;
			memcpy(target._data,source._data,source._len*sizeof(T_DWORD));
		}
	}
	
	bool CLargeInt::Equal(const CLargeInt& source,const CLargeInt& target) {
		if( source._len	== target._len ) {
			return !(memcmp(source._data,target._data,source._len*4));
		}
		else return false;
	}
	
	bool CLargeInt::LargerEqual(const CLargeInt& source,const CLargeInt& target,T_DWORD offset ) {
		if( source._len	> (target._len + offset) ) return true;
		else if( source._len < (target._len + offset) )	return false;
		else {
			int end	= offset;
			for( int i = source._len - 1; i	>= end ; i--) {
				T_DWORD	ii = i - offset;
				if( source._data[i] > target._data[ii] ) return	true;
				else if( source._data[i] < target._data[ii] )	return false;
			}
			return true; // =
		}
		
	}
	
	void CLargeInt::Add(const CLargeInt &augend, const CLargeInt &addend, CLargeInt	&sum,T_DWORD offset) {
		T_DWORD	len,len1,len2; // len cannot be less than 1
		T_DWORD	*p1 = 0;
		T_DWORD	*p2 = 0;
		T_DWORD	*p3 = 0;
		
		if( augend._len	>= (addend._len+offset)	) {
			len = augend._len - offset;
			len1 = addend._len;
			len2 =	len - len1;
			p1 = augend._data+offset;
			p2 = addend._data;
			p3 = sum._data+offset;
		}
		else {
			len = addend._len;
			if( augend._len	<= offset ) {
				augend._data[offset] = 0;
				len1 = 1;
				len2 = addend._len - len1;
			}
			else {
				len1 = augend._len - offset;
				len2 = addend._len - len1;
			}		
			p1 = addend._data;
			p2 = augend._data + offset;
			p3 = sum._data + offset;
		}
		
		__asm
		{
			pushad;
			// load addresses of the numbers
			mov esi,p1;
			mov ebx,p2;
			mov edi,p3;
			
			mov eax,len1;
			sal eax,2;
			
			// calc ending addresses
			add esi,eax;
			add ebx,eax;
			add edi,eax;
			
			mov ecx,eax;
			add ecx,4*7;
			and ecx,0xFFFFFFE0; //ecx - ecx%32, ecx becomes offset and accumulator
			neg ecx; // neg ecx for accumulating
			
			sar eax,2; // return to origin length
			
			neg eax; // neg eax
			and eax,dword ptr 7;
			jz labeladd0; // if the low bit is 0, then length >= 8
			
			mov edx,dword ptr 0xC;
			mul edx; // eax become offset
			
			lea edx,labeladd1; // load starting address
			sub edx,dword ptr 0xC;
			add eax,edx; // calc jumping address
			
			clc; // clear flag
			jmp eax; // jump
labeladdloop:
			popf;
labeladd0:
			mov eax,[esi+ecx+0*4];
			adc eax,[ebx+ecx+0*4];
			mov [edi+ecx+0*4],eax;
labeladd1:
			mov eax,[esi+ecx+1*4];
			adc eax,[ebx+ecx+1*4];
			mov [edi+ecx+1*4],eax;
			//labeladd2:
			mov eax,[esi+ecx+2*4];
			adc eax,[ebx+ecx+2*4];
			mov [edi+ecx+2*4],eax;
			//labeladd3:
			mov eax,[esi+ecx+3*4];
			adc eax,[ebx+ecx+3*4];
			mov [edi+ecx+3*4],eax;
			//labeladd4:
			mov eax,[esi+ecx+4*4];
			adc eax,[ebx+ecx+4*4];
			mov [edi+ecx+4*4],eax;
			//labeladd5:
			mov eax,[esi+ecx+5*4];
			adc eax,[ebx+ecx+5*4];
			mov [edi+ecx+5*4],eax;
			//labeladd6:
			mov eax,[esi+ecx+6*4];
			adc eax,[ebx+ecx+6*4];
			mov [edi+ecx+6*4],eax;
			//labeladd7:
			mov eax,[esi+ecx+7*4];
			adc eax,[ebx+ecx+7*4];
			mov [edi+ecx+7*4],eax;
			//labeladd8:
			pushf;
			add ecx,32;
			jnz labeladdloop;
			
			// finish overlapped part, start to process remaining part
			
			mov ecx,len2; // load length
			
			xor ebx,ebx; // set ebx=0
			cmp ebx,ecx; // compare ecx with 0
			jz labelcarry; // if equal, jump to handle last carry
			
labelfix:
			popf; // popup flag
			
			mov eax,[esi+ebx*4];
			adc eax,0;	
			mov [edi+ebx*4],eax;
			
			// if no carry, copy the remaining part with length of ecx-1
			jnc labelcopy;
			
			pushf; // save flag
			
			inc ebx; // step
			dec ecx;
			jnz labelfix; // loop
			
labelcarry:
			popf; // popup flag
			jnc labelend; // if no carry, finish
			// handle carry:
			mov dword ptr [edi+ebx*4],dword	ptr 0x00000001;	// set 1
			lea eax,len;
			inc [eax]; // length+1
			inc ecx; // do one more loop
labelcopy:
			dec ecx;
			sal ecx,2;
			cmp edi,esi; // compare des and src address, skip if equal
			jz labelend;
			
			sal ebx,2;
			add ebx,4;
			
			add edi,ebx;
			add esi,ebx;
			
			rep movs dword ptr [edi],dword ptr [esi];
			
labelend:
			popad;
		}
		sum._len = len + offset;	
	}
	
	void CLargeInt::Sub(const CLargeInt& minuend,const CLargeInt& subtrahend,CLargeInt& difference,T_DWORD offset) {
		T_DWORD	len,len1,len2; // len cannot be less than 1
		T_DWORD	*p1 = 0;
		T_DWORD	*p2 = 0;
		T_DWORD	*p3 = 0;
		
		if( minuend._len >= (subtrahend._len+offset) ) {
			len = minuend._len- offset;
			len1 = subtrahend._len;
			len2 =	len - len1;
			p1 = minuend._data + offset;
			p2 = subtrahend._data;
			p3 = difference._data +	offset;
		}
		else {
			__asm int 3; // error
		}
		
		__asm
		{
			pushad;
			// load addresses of the numbers
			mov esi,p1;
			mov ebx,p2;
			mov edi,p3;
			
			mov eax,len1;
			sal eax,2;
			
			// calc ending addresses
			add esi,eax;
			add ebx,eax;
			add edi,eax;
			
			mov ecx,eax;
			add ecx,4*7;
			and ecx,0xFFFFFFE0; //ecx - ecx%32, ecx becomes offset and accumulator
			neg ecx; // neg ecx for accumulating
			
			sar eax,2; // return to origin length
			
			neg eax; // neg eax
			and eax,dword ptr 7;
			jz labeladd0; // if the low bit is 0, then length >= 8
			
			mov edx,dword ptr 0xC;
			mul edx; // eax become offset
			
			lea edx,labeladd1; // load starting address
			sub edx,dword ptr 0xC;
			add eax,edx; // calc jumping address
			
			clc; // clear flag
			jmp eax; // jump
	labeladdloop:
			popf;
	labeladd0:
			mov eax,[esi+ecx+0*4];
			sbb eax,[ebx+ecx+0*4];
			mov [edi+ecx+0*4],eax;
	labeladd1:
			mov eax,[esi+ecx+1*4];
			sbb eax,[ebx+ecx+1*4];
			mov [edi+ecx+1*4],eax;
			//labeladd2:
			mov eax,[esi+ecx+2*4];
			sbb eax,[ebx+ecx+2*4];
			mov [edi+ecx+2*4],eax;
			//labeladd3:
			mov eax,[esi+ecx+3*4];
			sbb eax,[ebx+ecx+3*4];
			mov [edi+ecx+3*4],eax;
			//labeladd4:
			mov eax,[esi+ecx+4*4];
			sbb eax,[ebx+ecx+4*4];
			mov [edi+ecx+4*4],eax;
			//labeladd5:
			mov eax,[esi+ecx+5*4];
			sbb eax,[ebx+ecx+5*4];
			mov [edi+ecx+5*4],eax;
			//labeladd6:
			mov eax,[esi+ecx+6*4];
			sbb eax,[ebx+ecx+6*4];
			mov [edi+ecx+6*4],eax;
			//labeladd7:
			mov eax,[esi+ecx+7*4];
			sbb eax,[ebx+ecx+7*4];
			mov [edi+ecx+7*4],eax;
			//labeladd8:
			pushf;
			add ecx,32;
			jnz labeladdloop;
			
			// finish overlapped part, start to process remaining part
			
			mov ecx,len2; // load length
			// handle p1's buf
			
			xor ebx,ebx; // set ebx=0
			cmp ebx,ecx; // compare ecx with 0
			jz labelcarry; // if equal, jump to handle last carry
			
	labelfix:
			popf; // popup flag
			
			mov eax,[esi+ebx*4];
			sbb eax,0;	
			mov [edi+ebx*4],eax;
			
			// if no carry, copy the remaining part with length of ecx-1
			jnc labelcopy;
			
			pushf; // save flag
			
			inc ebx; // step
			dec ecx;
			jnz labelfix; // loop
			
	labelcarry:
			popf; // popup flag
			jnc labelend; // if no carry, finish
			// handle carry:
			int 3; // result is negative
			mov dword ptr [edi+ebx*4],dword	ptr 0x00000001;	// set 1
			lea eax,len;
			dec [eax]; // length+1
			inc ecx; // do one more loop
	labelcopy:
			dec ecx;
			sal ecx,2;
			cmp edi,esi; // compare des and src address, skip if equal
			jz labelend;
			
			sal ebx,2;
			add ebx,4;
			
			add edi,ebx;
			add esi,ebx;
			
			rep movs dword ptr [edi],dword ptr [esi];
			
	labelend:
			popad;
		}
		difference._len	= len+offset;
		for( T_DWORD i = difference._len-1; i >	0; i--) {
			// tail = 0
			if( difference._data[i]	== 0 ) {
				difference._len--;
			}
			else break;
		}
	}
	
	
	void CLargeInt::Mul(const CLargeInt& faciend,T_DWORD multiplier,CLargeInt& product)
	{
		T_DWORD	len; // len cannot be less than 1
		T_DWORD	*p1 = 0;
		T_DWORD	*p2 = 0;
		T_DWORD	*p3 = 0;
		
		len = faciend._len;
		p1 = faciend._data;
		p2 = 0;
		p3 = product._data;
		
		memset(p3,0,(len+1)*4);
		__asm
		{
			pushad;
			// load addresses of the numbers
			mov esi,p1;
			mov edi,p3;
			mov ebx,multiplier
				
			mov eax,len;
			sal eax,2;
			
			// calc ending addresses
			add esi,eax;
			add edi,eax;
			
			mov ecx,eax;
			add ecx,4*7;
			and ecx,0xFFFFFFE0; //ecx - ecx%32, ecx becomes offset and accumulator
			neg ecx; // neg ecx for accumulating
			
			sar eax,2; // return to origin length
			
			neg eax; // neg eax
			and eax,dword ptr 7;
			jz labeladd0; // if the low bit is 0, then length >= 8
			
			mov edx,dword ptr 0xe; // 0xe is the factor of jumping address
			mul edx; // eax becomes offset
			
			lea edx,labeladd1; // load starting address
			sub edx,dword ptr 0xe;
			add eax,edx; // calc jumping address
			
			clc; // clear flag
			jmp eax; // jump
	labeladdloop:
	labeladd0:
			mov eax,[esi+ecx+0*4];
			mul ebx;
			add [edi+ecx+0*4],eax;
			adc [edi+ecx+1*4],edx;
	labeladd1:
			mov eax,[esi+ecx+1*4];
			mul ebx;
			add [edi+ecx+1*4],eax;
			adc [edi+ecx+2*4],edx;
			//labeladd2:
			mov eax,[esi+ecx+2*4];
			mul ebx;
			add [edi+ecx+2*4],eax;
			adc [edi+ecx+3*4],edx;
			//labeladd3:
			mov eax,[esi+ecx+3*4];
			mul ebx;
			add [edi+ecx+3*4],eax;
			adc [edi+ecx+4*4],edx;
			//labeladd4:
			mov eax,[esi+ecx+4*4];
			mul ebx;
			add [edi+ecx+4*4],eax;
			adc [edi+ecx+5*4],edx;
			//labeladd5:
			mov eax,[esi+ecx+5*4];
			mul ebx;
			add [edi+ecx+5*4],eax;
			adc [edi+ecx+6*4],edx;
			//labeladd6:
			mov eax,[esi+ecx+6*4];
			mul ebx;
			add [edi+ecx+6*4],eax;
			adc [edi+ecx+7*4],edx;
			//labeladd7:
			mov eax,[esi+ecx+7*4];
			mul ebx;
			add [edi+ecx+7*4],eax;
			adc [edi+ecx+8*4],edx;
			//labeladd8:
			add ecx,32;
			jnz labeladdloop;
			
			//labelend:
			popad;
		}
		product._len = len+1;
		
		for( T_DWORD i = product._len-1; i > 0;	i--) {
			// tail == 0
			if( product._data[i] ==	0 ) {
				product._len--;
			}
			else break;
		}
		
	}
	
	void CLargeInt::RCR(CLargeInt& n) {
		T_DWORD	len; // len cannot be less than 1
		T_DWORD	*p1 = 0;	
		len = n._len;
		p1 = n._data;
		
		__asm
		{
			pushad;		
			
			// load addresses of the numbers
			mov esi,p1;
			
			mov eax,len;
			mov ecx,eax;
			
			add ecx,dword ptr 15;
			and ecx,dword ptr 0xFFFFFFF0;
			
			mov eax,ecx;
			sal ecx,dword ptr 2;
			
			sub eax,len; // get origin offset
			
			mov ebx,dword ptr 4; // offset multiplier
			mul ebx;		
			
			lea ebx,label0;
			add eax,ebx;
			clc;
			jmp eax;
	labelloop:
			popf;
	label0:
			rcr dword ptr [esi + ecx - 1*4],1;
			//label1:
			rcr dword ptr [esi + ecx - 2*4],1;
			//label2:
			rcr dword ptr [esi + ecx - 3*4],1;
			//label3:
			rcr dword ptr [esi + ecx - 4*4],1;
			//label4:
			rcr dword ptr [esi + ecx - 5*4],1;
			//label5:
			rcr dword ptr [esi + ecx - 6*4],1;
			//label6:
			rcr dword ptr [esi + ecx - 7*4],1;
			//label7:
			rcr dword ptr [esi + ecx - 8*4],1;
			//label8:
			rcr dword ptr [esi + ecx - 9*4],1;
			//label9:
			rcr dword ptr [esi + ecx - 10*4],1;
			//label10:
			rcr dword ptr [esi + ecx - 11*4],1;
			//label11:
			rcr dword ptr [esi + ecx - 12*4],1;
			//label12:
			rcr dword ptr [esi + ecx - 13*4],1;
			//label13:
			rcr dword ptr [esi + ecx - 14*4],1;
			//label14:
			rcr dword ptr [esi + ecx - 15*4],1;
			//label15:
			rcr dword ptr [esi + ecx - 16*4],1;
			//label16:
			pushf;
			sub ecx,dword ptr 64;
			jnz labelloop;
			popf;
			popad;
		}
		for( T_DWORD i = n._len-1; i > 0; i--) {
			// tail == 0
			if( n._data[i] == 0 ) {
				n._len--;
			}
			else break;
		}
	}
	
	void CLargeInt::RCL(CLargeInt& n)
	{
		T_DWORD	len = n._len; // len cannot be less than 1
		T_DWORD	*p1 = 0;
		
		n._data[len] = 0;
		n._len++;
		
		len++;
		p1 = n._data;
		
		__asm
		{
			pushad;		
			
			// load addresses of the numbers
			mov esi,p1;
			
			mov eax,len;
			mov ecx,eax;
			
			sal eax,dword ptr 2;
			add esi,eax;
			
			add ecx,dword ptr 15;
			and ecx,dword ptr 0xFFFFFFF0;
			
			mov eax,ecx;
			sal ecx,dword ptr 2;
			
			neg ecx;
			
			sub eax,len; // get origin offset
			jz label0;
			
			mov ebx,dword ptr 4; // offset multiplier
			mul ebx;		
			
			lea ebx,label1;
			sub ebx,4;
			add eax,ebx;
			clc;
			jmp eax;
	labelloop:
			popf;
	label0:
			rcl dword ptr [esi + ecx + 0*4],1;
	label1:
			rcl dword ptr [esi + ecx + 1*4],1;
			//label2:
			rcl dword ptr [esi + ecx + 2*4],1;
			//label3:
			rcl dword ptr [esi + ecx + 3*4],1;
			//label4:
			rcl dword ptr [esi + ecx + 4*4],1;
			//label5:
			rcl dword ptr [esi + ecx + 5*4],1;
			//label6:
			rcl dword ptr [esi + ecx + 6*4],1;
			//label7:
			rcl dword ptr [esi + ecx + 7*4],1;
			//label8:
			rcl dword ptr [esi + ecx + 8*4],1;
			//label9:
			rcl dword ptr [esi + ecx + 9*4],1;
			//label10:
			rcl dword ptr [esi + ecx + 10*4],1;
			//label11:
			rcl dword ptr [esi + ecx + 11*4],1;
			//label12:
			rcl dword ptr [esi + ecx + 12*4],1;
			//label13:
			rcl dword ptr [esi + ecx + 13*4],1;
			//label14:
			rcl dword ptr [esi + ecx + 14*4],1;
			//label15:
			rcl dword ptr [esi + ecx + 15*4],1;
			//label16:
			pushf;
			add ecx,dword ptr 64;
			jnz labelloop;
			popf;
			popad;
		}
		for( T_DWORD i = n._len-1; i > 0; i--) {
			// tail == 0
			if( n._data[i] == 0 ) {
				n._len--;
			}
			else break;
		}
	}
	
	void CLargeInt::Mul(const CLargeInt& faciend,const CLargeInt& multiplier,CLargeInt& product) {
		CLargeInt temp;
		product._len = (faciend._len + multiplier._len);
		memset(product._data,0,product._len * 4	);
		
		for( T_DWORD i = 0; i <	multiplier._len; i++) {
			Mul(faciend,multiplier._data[i],temp);
			Add(product,temp,product,i);
		}
		for( T_DWORD i = product._len-1; i > 0;	i--) {
			// tail == 0
			if( product._data[i] ==	0 ) {
				product._len--;
			}
			else break;
		}
	}
	
	void CLargeInt::Div(const CLargeInt& dividend,T_DWORD divisor,CLargeInt& quotient,T_DWORD& residual) {
		T_DWORD	len; // len cannot be less than 1
		T_DWORD	*p1 = 0;
		T_DWORD	*p2 = 0;
		
		T_DWORD	*pr = &residual;
		
		len = dividend._len;
		quotient._len =	dividend._len;
		p1 = dividend._data;
		p2 = quotient._data;
		
		__asm
		{
			pushad;		
			
			// load addresses of the numbers
			mov esi,p1;
			mov edi,p2;
			
			mov eax,len;
			mov ecx,eax;
			
			add ecx,dword ptr 7;
			and ecx,dword ptr 0xFFFFFFF8;
			
			mov eax,ecx;
			sal ecx,dword ptr 2;
			
			sub eax,len; // get origin offset
			
			mov ebx,dword ptr 0x0A;	// offset multiplier
			mul ebx;		
			
			lea edx,label0;
			add eax,edx;
			
			xor edx,edx;
			
			mov ebx,divisor;
			
			clc;
			jmp eax;
	labelloop:
			
	label0:
			mov eax,[esi + ecx - 1*4];
			div ebx;
			mov [edi + ecx - 1*4],eax;
			//label1:
			mov eax,[esi + ecx - 2*4];
			div ebx;
			mov [edi + ecx - 2*4],eax;
			//label2:
			mov eax,[esi + ecx - 3*4];
			div ebx;
			mov [edi + ecx - 3*4],eax;
			//label3:
			mov eax,[esi + ecx - 4*4];
			div ebx;
			mov [edi + ecx - 4*4],eax;
			//label4:
			mov eax,[esi + ecx - 5*4];
			div ebx;
			mov [edi + ecx - 5*4],eax;
			//label5:
			mov eax,[esi + ecx - 6*4];
			div ebx;
			mov [edi + ecx - 6*4],eax;
			//label6:
			mov eax,[esi + ecx - 7*4];
			div ebx;
			mov [edi + ecx - 7*4],eax;
			//label7:
			mov eax,[esi + ecx - 8*4];
			div ebx;
			mov [edi + ecx - 8*4],eax;
			//label8:
			sub ecx,dword ptr 32;
			jnz labelloop;
			
			// finish, save remainder
			mov ecx,pr;
			mov [ecx],edx;
			popad;
		}
		
		for( T_DWORD i = quotient._len-1; i > 0; i--) {
			// tail == 0
			if( quotient._data[i] == 0 ) {
				quotient._len--;
			}
			else break;
		}
	}
	
	void CLargeInt::Div(const CLargeInt& dividend,const CLargeInt& divisor,CLargeInt& quotient,CLargeInt& residual) {
		if( dividend._len < divisor._len ) {
			quotient._len =	1;
			quotient._data[0] = 0;
			Copy(dividend,residual);
		}
		else {
			if( divisor._len == 1) {			
				Div(dividend,divisor._data[0],quotient,residual._data[0]);
				residual._len =	1;
			}
			else {
				CLargeInt d,r;
				
				T_DWORD	head = divisor._data[divisor._len-1];
				__asm
				{
					pushad;
					mov eax,head;
					bsr ecx,eax;
					mov edx,dword ptr 31;
					sub edx,ecx;
					mov ecx,edx;
					mov eax,dword ptr 0x01;
					sal eax,cl;
					mov head,eax;
					popad;
				}
				
				Mul(dividend,head,d);
				Mul(divisor,head,r);
				
				quotient._len =	d._len - r._len	+ 1;
				quotient._data[quotient._len - 1] = 0;
				
				T_DWORD	newhead	= r._data[r._len - 1];
				// move the MSB 1 of dividend to MSB of DWORD
				// can significantly reduce the difference between the upper bound and lower bound of quotient
				
				T_DWORD	highpart = 0;
				T_DWORD	lowpart	 = 0;
				T_DWORD	max = 0,min = 0;
				T_DWORD	offset = 0;
				CLargeInt temp;
				for( T_DWORD i = d._len-1; i >=	r._len-1; i-- )
				{				
					offset = i - (r._len - 1);	
					lowpart	= d._data[i];
					
					__asm
					{ // get the upper bound and lower bound of quotient
						pushad;
						mov edx,highpart;
						mov eax,lowpart;
						mov ecx,newhead;
						div ecx;
						mov max,eax;
						
						inc ecx;
						jz _mov;
						mov edx,highpart;
						mov eax,lowpart;
						div ecx;
						mov edx,eax;
				_mov:
						mov min,edx;
						popad;
					}
					
					if( max	== 0 ) {
						highpart = d._data[i];
						quotient._data[offset] = 0;
					}
					else {
						Mul(r,max,temp);
						if( max	!= min )
						{
							while( !LargerEqual(d,temp,offset) ) { // Trial division
								// the difference between the upper bound and lower bound of quotient is small
								// so we dont use binary search
								max--;
								Sub(temp,r,temp);
							}
						}	
						
						quotient._data[offset] = max; // save quotient
						Sub(d,temp,d,offset); // get remainder
						highpart = d._data[i]; // load remainder
					}
				}
				
				// clear prefix 0
				for( T_DWORD i = quotient._len-1; i > 0; i--) {
					// tail == 0
					if( quotient._data[i] == 0 ) {
						quotient._len--;
					}
					else break;
				}
				// finish div, get remainder
				Div(d,head,residual,newhead);
				
				if( newhead != 0 ) {
					// remainder < 0, error
					__asm int 3;
				}
			}
			
		}
	}
	
	void CLargeInt::ExpMod(const CLargeInt&	source,const CLargeInt&	exponent,const CLargeInt& modulo,CLargeInt& result) {
		CLargeInt n,e,r(1),temp,temp1;
		Copy(source,n);
		Copy(exponent,e);
		
		while( e._len >	1 || e._data[e._len - 1] > 1 ) {
			if( e._data[0] &1 ) {
				Mul(r,n,temp);
				Div(temp,modulo,temp1,r);
			}
			RCR(e);
			Mul(n,n,temp);
			Div(temp,modulo,temp1,n);
		}
		Mul(r,n,temp);
		Div(temp,modulo,temp1,result);
	}
}
