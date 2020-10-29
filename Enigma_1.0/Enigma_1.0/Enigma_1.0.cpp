#include "stdafx.h"
#include <fstream>
#include "resource.h"
#include <Windows.h>
#include <objidl.h>
#include <gdiplus.h>
#include <string.h>
#include <mmsystem.h>

using namespace Gdiplus;

#pragma comment (lib,"Gdiplus.lib")

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK ROTOR(HWND hWnd, UINT Message, UINT wParam, LONG lParam);
BOOL CALLBACK KOM(HWND hWnd, UINT Message, UINT wParam, LONG lParam);
BOOL CALLBACK INSTR(HWND hWnd, UINT Message, UINT wParam, LONG lParam);

// алфавиты
wchar_t alf[27] = L"ABCDEFGHIJKLMNOPQRSTUVWXYZ";
wchar_t komPanel[22] = L"123456789012345678901";
//роторы установленные
wchar_t rotorI[27];
wchar_t rotorII[27];
wchar_t rotorIII[27];
//роторы 
wchar_t I[27] = L"EKMFLGDQVZNTOWYHXUSPAIBRCJ";
wchar_t II[27] = L"AJDKSIRUXBLHWTMCQGZNPYFVOE";
wchar_t III[27] = L"BDFHJLCPRTXVZNYEIWGAKMUSQO";
wchar_t IV[27] = L"ESOVPZJAYQUIRHXLNFTGKDCMWB";
wchar_t V[27] = L"VZBRGITYUPSDNHLXAWMJQOFECK";
//рефлекторы
wchar_t ref1[27] = L"YRUHQSLDPXNGOKMIEBFZCWVJAT";
wchar_t ref2[27] = L"FVPJIAOYEDRZXWGCTKUQSBNMHL";
wchar_t ref3[27] = L"ENKQAUYWJICOPBLMDXZVFTHRGS";
wchar_t ref4[27] = L"RDOBJNTKVEHMLFCWZAXGYIPSUQ";
wchar_t refStart[27] = L"YRUHQSLDPXNGOKMIEBFZCWVJAT";

int check = 0;
int buk = 0;
int buk2 = 0;
wchar_t Bu;
int r1 = 0, r2 = 0, r3 = 0, ref = 1;
int checkR2 = -1, checkR3 = -1;
int n = 0;
wchar_t txt[1000];
int izmr1 = 0, izmr2, izmr3;
bool nastr = false;
bool power = false;
bool zap = false;


void nastrI(wchar_t ri[]) {
	for (int i = 0; i < 26; i++) {
		rotorI[i] = ri[i];
	}
}

void nastrII(wchar_t ri[]) {
	for (int i = 0; i < 26; i++) {
		rotorII[i] = ri[i];
	}
}

void nastrIII(wchar_t ri[]) {
	for (int i = 0; i < 26; i++) {
		rotorIII[i] = ri[i];
	}
}

void rotorplus() {
	if (r1 == 25) {
		r1 = 0;
	}
	else if (r1 == 5 || r1 == 17) { r2++; r1++; }
	else { r1++; }

	if (r2 == 25) { r2 = 0; }
	else if (r2 == 4 || r2 == 16) { r3++; r2++; }
	txt[n] = alf[buk2];
	n++;
}

int cikl(wchar_t Bu, wchar_t alf[]) {
	int k = 0;
	for (int i = 0; i < 26; i++) {
		if (Bu == alf[i]) {
			k = i;
		}
	}
	return k;
}

int mi(int ch1, int ch2) {
	int h;
	int tmp;
	if (ch2 - ch1 < 0) {
		h = ch2;
		int k2 = 0;
		while (k2 < ch1) {
			if (h == 0) { h = 25; }
			else { h--; }
			k2++;
		}
		tmp = h;
	}
	else {
		tmp = ch2 - ch1;
	}
	return tmp;
}

int pl(int ch1, int ch2) {
	int tmp;
	if (ch1 + ch2 >= 26) {
		tmp = ch1;
		int k2 = 0;
		while (k2 < ch2) {
			if (tmp == 25) { tmp = 0; }
			else { tmp++; }
			k2++;
		}
	}
	else {
		tmp = ch1 + ch2;
	}
	return tmp;
}

int perevod(int buk, int r1, int r2, int r3, wchar_t rotorI[], wchar_t rotorII[], wchar_t rotorIII[]) {

	int k = 0;
	int raz1;
	int raz2;
	int tmp = 0;
	wchar_t izm='.';
	wchar_t izm2 = '.';

		// razs
		raz1 = mi(r1, r2);
		raz2 = mi(r2, r3);

		// вход
		k = buk;
		for (int i = 1; i < 22; i++) {
			if (alf[buk] == komPanel[i]) {
				if (i % 2 == 1) {
					izm = komPanel[i + 1];
				}
				else if (i % 2 == 0) {
					izm = komPanel[i - 1];
				}
			}
		}
		for (int i = 0; i < 26; i++) {
			if (alf[i] == izm) {
				k = i;
			}
		}
		
		tmp = pl(r1, k);
		Bu = rotorI[tmp];
		// на второй
		k = cikl(Bu, alf);
		tmp = pl(raz1, k);
		Bu = rotorII[tmp];
		// на третий
		k = cikl(Bu, alf);
		tmp = pl(raz2, k);
		Bu = rotorIII[tmp];
		// развoрот 
		k = cikl(Bu, alf);
		tmp = mi(r3, k);
		Bu = refStart[tmp];
		// на третий обр 
		k = cikl(Bu, alf);
		tmp = pl(r3, k);
		Bu = alf[tmp];
		k = cikl(Bu, rotorIII);
		// на второй обр
		tmp = mi(raz2, k);
		Bu = alf[tmp];
		k = cikl(Bu, rotorII);
		// на первый обр
		tmp = mi(raz1, k);
		Bu = alf[tmp];
		k = cikl(Bu, rotorI);
		// на выход 
		tmp = mi(r1, k);

		for (int i = 1; i < 22; i++) {
			if (alf[tmp] == komPanel[i]) {
				if (i % 2 == 1) {
					izm2 = komPanel[i + 1];
				}
				else if (i % 2 == 0) {
					izm2 = komPanel[i - 1];
				}
			}
		}
		wchar_t izm3;
		izm3 = izm2;
		for (int i = 0; i < 26; i++) {
			if (alf[i] == izm2) {
				tmp = i;
			}
		}
		
		return tmp;	
}

void sozdcombobox(HWND hWnd, int a) {
	for (int i = 0; i < 26; i++) {
		wchar_t txt[2] = L" ";
		txt[1] = '\0';
		txt[0] = alf[i];
		SendDlgItemMessage(hWnd, a, CB_ADDSTRING, NULL, (LPARAM)txt);
	}
}

void vivodcombobox(HWND hWnd, int a, int i) {
	wchar_t txt[5];
	GetDlgItemText(hWnd, a, txt, 10);
	komPanel[i] = txt[0];
}

VOID golova(HDC hdc) {
	Graphics graphics(hdc);
	Image image(L"images\\fon2.jpg");
	graphics.DrawImage(&image, 0, 0, 1546, 1030);
}
// буквы 
VOID buttonApressed(HDC hdc){
	Graphics graphics(hdc);
	Image image(L"images\\button_A_pressed.png");
	graphics.DrawImage(&image, 553, 615);
}

VOID buttonSpressed(HDC hdc) {
	Graphics graphics(hdc);
	Image image(L"images\\button_S_pressed.png");
	graphics.DrawImage(&image, 613, 615);
}

VOID button_D_pressed(HDC hdc) {
	Graphics graphics(hdc);
	Image image(L"images\\button_D_pressed.png");
	graphics.DrawImage(&image, 672, 615);
}

VOID button_F_pressed(HDC hdc) {
	Graphics graphics(hdc);
	Image image(L"images\\button_F_pressed.png");
	graphics.DrawImage(&image, 731, 615);
}

VOID button_G_pressed(HDC hdc) {
	Graphics graphics(hdc);
	Image image(L"images\\button_G_pressed.png");
	graphics.DrawImage(&image, 791, 615);
}

VOID button_H_pressed(HDC hdc) {
	Graphics graphics(hdc);
	Image image(L"images\\button_H_pressed.png");
	graphics.DrawImage(&image, 850, 615);
}

VOID button_J_pressed(HDC hdc) {
	Graphics graphics(hdc);
	Image image(L"images\\button_J_pressed.png");
	graphics.DrawImage(&image, 910, 615);
}

VOID button_K_pressed(HDC hdc) {
	Graphics graphics(hdc);
	Image image(L"images\\button_K_pressed.png");
	graphics.DrawImage(&image, 969, 615);
}

VOID button_Q_pressed(HDC hdc) {
	Graphics graphics(hdc);
	Image image(L"images\\button_Q_pressed.png");
	graphics.DrawImage(&image, 523, 563);
}

VOID button_W_pressed(HDC hdc) {
	Graphics graphics(hdc);
	Image image(L"images\\button_W_pressed.png");
	graphics.DrawImage(&image, 583, 563);
}

VOID button_E_pressed(HDC hdc) {
	Graphics graphics(hdc);
	Image image(L"images\\button_E_pressed.png");
	graphics.DrawImage(&image, 642, 563);
}

VOID button_R_pressed(HDC hdc) {
	Graphics graphics(hdc);
	Image image(L"images\\button_R_pressed.png");
	graphics.DrawImage(&image, 701, 563);
}

VOID button_T_pressed(HDC hdc) {
	Graphics graphics(hdc);
	Image image(L"images\\button_T_pressed.png");
	graphics.DrawImage(&image, 761, 563);
}

VOID button_Z_pressed(HDC hdc) {
	Graphics graphics(hdc);
	Image image(L"images\\button_Z_pressed.png");
	graphics.DrawImage(&image, 821, 563);
}

VOID button_U_pressed(HDC hdc) {
	Graphics graphics(hdc);
	Image image(L"images\\button_U_pressed.png");
	graphics.DrawImage(&image, 880, 563);
}

VOID button_I_pressed(HDC hdc) {
	Graphics graphics(hdc);
	Image image(L"images\\button_I_pressed.png");
	graphics.DrawImage(&image, 939, 563);
}

VOID button_O_pressed(HDC hdc) {
	Graphics graphics(hdc);
	Image image(L"images\\button_O_pressed.png");
	graphics.DrawImage(&image, 999, 563);
}

VOID button_P_pressed(HDC hdc) {
	Graphics graphics(hdc);
	Image image(L"images\\button_P_pressed.png");
	graphics.DrawImage(&image, 523, 667);
}

VOID button_Y_pressed(HDC hdc) {
	Graphics graphics(hdc);
	Image image(L"images\\button_Y_pressed.png");
	graphics.DrawImage(&image, 583, 667);
}

VOID button_X_pressed(HDC hdc) {
	Graphics graphics(hdc);
	Image image(L"images\\button_X_pressed.png");
	graphics.DrawImage(&image, 642, 667);
}

VOID button_C_pressed(HDC hdc) {
	Graphics graphics(hdc);
	Image image(L"images\\button_C_pressed.png");
	graphics.DrawImage(&image, 701, 667);
}

VOID button_V_pressed(HDC hdc) {
	Graphics graphics(hdc);
	Image image(L"images\\button_V_pressed.png");
	graphics.DrawImage(&image, 761, 667);
}

VOID button_B_pressed(HDC hdc) {
	Graphics graphics(hdc);
	Image image(L"images\\button_B_pressed.png");
	graphics.DrawImage(&image, 821, 667);
}

VOID button_N_pressed(HDC hdc) {
	Graphics graphics(hdc);
	Image image(L"images\\button_N_pressed.png");
	graphics.DrawImage(&image, 880, 667);
}

VOID button_M_pressed(HDC hdc) {
	Graphics graphics(hdc);
	Image image(L"images\\button_M_pressed.png");
	graphics.DrawImage(&image, 939, 667);
}

VOID button_L_pressed(HDC hdc) {
	Graphics graphics(hdc);
	Image image(L"images\\button_L_pressed.png");
	graphics.DrawImage(&image, 999, 667);
}
// lampi 
VOID lamp_A_pressed(HDC hdc) {
	Graphics graphics(hdc);
	Image image(L"images\\lamp_A_pressed.png");
	graphics.DrawImage(&image, 542, 456);
}

VOID lamp_S_pressed(HDC hdc) {
	Graphics graphics(hdc);
	Image image(L"images\\lamp_S_pressed.png");
	graphics.DrawImage(&image, 606, 456);
}

VOID lamp_D_pressed(HDC hdc) {
	Graphics graphics(hdc);
	Image image(L"images\\lamp_D_pressed.png");
	graphics.DrawImage(&image, 669, 456);
}

VOID lamp_F_pressed(HDC hdc) {
	Graphics graphics(hdc);
	Image image(L"images\\lamp_F_pressed.png");
	graphics.DrawImage(&image, 732, 456);
}

VOID lamp_G_pressed(HDC hdc) {
	Graphics graphics(hdc);
	Image image(L"images\\lamp_G_pressed.png");
	graphics.DrawImage(&image, 796, 456);
}

VOID lamp_H_pressed(HDC hdc) {
	Graphics graphics(hdc);
	Image image(L"images\\lamp_H_pressed.png");
	graphics.DrawImage(&image, 860, 456);
}

VOID lamp_J_pressed(HDC hdc) {
	Graphics graphics(hdc);
	Image image(L"images\\lamp_J_pressed.png");
	graphics.DrawImage(&image, 923, 456);
}

VOID lamp_K_pressed(HDC hdc) {
	Graphics graphics(hdc);
	Image image(L"images\\lamp_K_pressed.png");
	graphics.DrawImage(&image, 986, 456);
}

VOID lamp_Q_pressed(HDC hdc) {
	Graphics graphics(hdc);
	Image image(L"images\\lamp_Q_pressed.png");
	graphics.DrawImage(&image, 510, 406);
}

VOID lamp_W_pressed(HDC hdc) {
	Graphics graphics(hdc);
	Image image(L"images\\lamp_W_pressed.png");
	graphics.DrawImage(&image, 573, 406);
}

VOID lamp_E_pressed(HDC hdc) {
	Graphics graphics(hdc);
	Image image(L"images\\lamp_E_pressed.png");
	graphics.DrawImage(&image, 637, 406);
}

VOID lamp_R_pressed(HDC hdc) {
	Graphics graphics(hdc);
	Image image(L"images\\lamp_R_pressed.png");
	graphics.DrawImage(&image, 701, 406);
}

VOID lamp_T_pressed(HDC hdc) {
	Graphics graphics(hdc);
	Image image(L"images\\lamp_T_pressed.png");
	graphics.DrawImage(&image, 764, 406);
}

VOID lamp_Z_pressed(HDC hdc) {
	Graphics graphics(hdc);
	Image image(L"images\\lamp_Z_pressed.png");
	graphics.DrawImage(&image, 827, 406);
}

VOID lamp_U_pressed(HDC hdc) {
	Graphics graphics(hdc);
	Image image(L"images\\lamp_U_pressed.png");
	graphics.DrawImage(&image, 891, 406);
}

VOID lamp_I_pressed(HDC hdc) {
	Graphics graphics(hdc);
	Image image(L"images\\lamp_I_pressed.png");
	graphics.DrawImage(&image, 955, 406);
}

VOID lamp_O_pressed(HDC hdc) {
	Graphics graphics(hdc);
	Image image(L"images\\lamp_O_pressed.png");
	graphics.DrawImage(&image, 1018, 406);
}

VOID lamp_P_pressed(HDC hdc) {
	Graphics graphics(hdc);
	Image image(L"images\\lamp_P_pressed.png");
	graphics.DrawImage(&image, 510, 507);
}

VOID lamp_Y_pressed(HDC hdc) {
	Graphics graphics(hdc);
	Image image(L"images\\lamp_Y_pressed.png");
	graphics.DrawImage(&image, 573, 507);
}

VOID lamp_X_pressed(HDC hdc) {
	Graphics graphics(hdc);
	Image image(L"images\\lamp_X_pressed.png");
	graphics.DrawImage(&image, 637, 507);
}

VOID lamp_C_pressed(HDC hdc) {
	Graphics graphics(hdc);
	Image image(L"images\\lamp_C_pressed.png");
	graphics.DrawImage(&image, 701, 507);
}

VOID lamp_V_pressed(HDC hdc) {
	Graphics graphics(hdc);
	Image image(L"images\\lamp_V_pressed.png");
	graphics.DrawImage(&image, 764, 507);
}

VOID lamp_B_pressed(HDC hdc) {
	Graphics graphics(hdc);
	Image image(L"images\\lamp_B_pressed.png");
	graphics.DrawImage(&image, 827, 507);
}

VOID lamp_N_pressed(HDC hdc) {
	Graphics graphics(hdc);
	Image image(L"images\\lamp_N_pressed.png");
	graphics.DrawImage(&image, 891, 507);
}

VOID lamp_M_pressed(HDC hdc) {
	Graphics graphics(hdc);
	Image image(L"images\\lamp_M_pressed.png");
	graphics.DrawImage(&image, 955, 507);
}

VOID lamp_L_pressed(HDC hdc) {
	Graphics graphics(hdc);
	Image image(L"images\\lamp_L_pressed.png");
	graphics.DrawImage(&image, 1018, 507);
}
//reflektor
VOID reflector_A(HDC hdc) {
	Graphics graphics(hdc);
	Image image(L"images\\reflector_A.png");
	graphics.DrawImage(&image, 512, 220);
}

VOID reflector_B(HDC hdc) {
	Graphics graphics(hdc);
	Image image(L"images\\reflector_B.png");
	graphics.DrawImage(&image, 512, 220);
}

VOID reflector_C(HDC hdc) {
	Graphics graphics(hdc);
	Image image(L"images\\reflector_C.png");
	graphics.DrawImage(&image, 512, 220);
}

VOID reflector_D(HDC hdc) {
	Graphics graphics(hdc);
	Image image(L"images\\reflector_D.png");
	graphics.DrawImage(&image, 512, 220);
}
// power
VOID Power(HDC hdc){
	Graphics graphics(hdc);
	Image image(L"images\\power_on.png");
	graphics.DrawImage(&image, 857, 175);
}
// nastroiki
VOID rotor_1(HDC hdc, int r1) {
	wchar_t name[40];
	Graphics graphics(hdc);
	switch (r1) {
	case 0: {
		wchar_t name1[] = L"images\\letter_A.png";
		wcscpy_s(name, name1);
		break; }
	case 1: {
		wchar_t name1[] = L"images\\letter_B.png";
		wcscpy_s(name, name1);
		break; }
	case 2: {
		wchar_t name1[] = L"images\\letter_C.png";
		wcscpy_s(name, name1);
		break; }
	case 3: {
		wchar_t name1[] = L"images\\letter_D.png";
		wcscpy_s(name, name1);
		break; }
	case 4: {
		wchar_t name1[] = L"images\\letter_E.png";
		wcscpy_s(name, name1);
		break; }
	case 5: {
		wchar_t name1[] = L"images\\letter_F.png";
		wcscpy_s(name, name1);
		break; }
	case 6: {
		wchar_t name1[] = L"images\\letter_G.png";
		wcscpy_s(name, name1);
		break; }
	case 7: {
		wchar_t name1[] = L"images\\letter_H.png";
		wcscpy_s(name, name1);
		break; }
	case 8: {
		wchar_t name1[] = L"images\\letter_I.png";
		wcscpy_s(name, name1);
		break; }
	case 9: {
		wchar_t name1[] = L"images\\letter_J.png";
		wcscpy_s(name, name1);
		break; }
	case 10: {
		wchar_t name1[] = L"images\\letter_K.png";
		wcscpy_s(name, name1);
		break; }
	case 11: {
		wchar_t name1[] = L"images\\letter_L.png";
		wcscpy_s(name, name1);
		break; }
	case 12: {
		wchar_t name1[] = L"images\\letter_M.png";
		wcscpy_s(name, name1);
		break; }
	case 13: {
		wchar_t name1[] = L"images\\letter_N.png";
		wcscpy_s(name, name1);
		break; }
	case 14: {
		wchar_t name1[] = L"images\\letter_O.png";
		wcscpy_s(name, name1);
		break; }
	case 15: {
		wchar_t name1[] = L"images\\letter_P.png";
		wcscpy_s(name, name1);
		break; }
	case 16: {
		wchar_t name1[] = L"images\\letter_Q.png";
		wcscpy_s(name, name1);
		break; }
	case 17: {
		wchar_t name1[] = L"images\\letter_R.png";
		wcscpy_s(name, name1);
		break; }
	case 18: {
		wchar_t name1[] = L"images\\letter_S.png";
		wcscpy_s(name, name1);
		break; }
	case 19: {
		wchar_t name1[] = L"images\\letter_T.png";
		wcscpy_s(name, name1);
		break; }
	case 20: {
		wchar_t name1[] = L"images\\letter_U.png";
		wcscpy_s(name, name1);
		break; }
	case 21: {
		wchar_t name1[] = L"images\\letter_V.png";
		wcscpy_s(name, name1);
		break; }
	case 22: {
		wchar_t name1[] = L"images\\letter_W.png";
		wcscpy_s(name, name1);
		break; }
	case 23: {
		wchar_t name1[] = L"images\\letter_X.png";
		wcscpy_s(name, name1);
		break; }
	case 24: {
		wchar_t name1[] = L"images\\letter_Y.png";
		wcscpy_s(name, name1);
		break; }
	case 25: {
		wchar_t name1[] = L"images\\letter_Z.png";
		wcscpy_s(name, name1);
		break; }

	}
	
	Image image(name);
		graphics.DrawImage(&image, 780, 230);
	
}

VOID rotor_2(HDC hdc, int r2, int checkR2) {
	if (checkR2 != r2) {
		wchar_t name[40];
		Graphics graphics(hdc);
		switch (r2) {
		case 0: {
			wchar_t name1[] = L"images\\letter_A.png";
			wcscpy_s(name, name1);
			break; }
		case 1: {
			wchar_t name1[] = L"images\\letter_B.png";
			wcscpy_s(name, name1);
			break; }
		case 2: {
			wchar_t name1[] = L"images\\letter_C.png";
			wcscpy_s(name, name1);
			break; }
		case 3: {
			wchar_t name1[] = L"images\\letter_D.png";
			wcscpy_s(name, name1);
			break; }
		case 4: {
			wchar_t name1[] = L"images\\letter_E.png";
			wcscpy_s(name, name1);
			break; }
		case 5: {
			wchar_t name1[] = L"images\\letter_F.png";
			wcscpy_s(name, name1);
			break; }
		case 6: {
			wchar_t name1[] = L"images\\letter_G.png";
			wcscpy_s(name, name1);
			break; }
		case 7: {
			wchar_t name1[] = L"images\\letter_H.png";
			wcscpy_s(name, name1);
			break; }
		case 8: {
			wchar_t name1[] = L"images\\letter_I.png";
			wcscpy_s(name, name1);
			break; }
		case 9: {
			wchar_t name1[] = L"images\\letter_J.png";
			wcscpy_s(name, name1);
			break; }
		case 10: {
			wchar_t name1[] = L"images\\letter_K.png";
			wcscpy_s(name, name1);
			break; }
		case 11: {
			wchar_t name1[] = L"images\\letter_L.png";
			wcscpy_s(name, name1);
			break; }
		case 12: {
			wchar_t name1[] = L"images\\letter_M.png";
			wcscpy_s(name, name1);
			break; }
		case 13: {
			wchar_t name1[] = L"images\\letter_N.png";
			wcscpy_s(name, name1);
			break; }
		case 14: {
			wchar_t name1[] = L"images\\letter_O.png";
			wcscpy_s(name, name1);
			break; }
		case 15: {
			wchar_t name1[] = L"images\\letter_P.png";
			wcscpy_s(name, name1);
			break; }
		case 16: {
			wchar_t name1[] = L"images\\letter_Q.png";
			wcscpy_s(name, name1);
			break; }
		case 17: {
			wchar_t name1[] = L"images\\letter_R.png";
			wcscpy_s(name, name1);
			break; }
		case 18: {
			wchar_t name1[] = L"images\\letter_S.png";
			wcscpy_s(name, name1);
			break; }
		case 19: {
			wchar_t name1[] = L"images\\letter_T.png";
			wcscpy_s(name, name1);
			break; }
		case 20: {
			wchar_t name1[] = L"images\\letter_U.png";
			wcscpy_s(name, name1);
			break; }
		case 21: {
			wchar_t name1[] = L"images\\letter_V.png";
			wcscpy_s(name, name1);
			break; }
		case 22: {
			wchar_t name1[] = L"images\\letter_W.png";
			wcscpy_s(name, name1);
			break; }
		case 23: {
			wchar_t name1[] = L"images\\letter_X.png";
			wcscpy_s(name, name1);
			break; }
		case 24: {
			wchar_t name1[] = L"images\\letter_Y.png";
			wcscpy_s(name, name1);
			break; }
		case 25: {
			wchar_t name1[] = L"images\\letter_Z.png";
			wcscpy_s(name, name1);
			break; }

		}

		Image image(name);
		graphics.DrawImage(&image, 702, 230);
	}
}

VOID rotor_3(HDC hdc, int r3, int checkR3) {
	if (checkR3 != r3) {
		wchar_t name[40];
		Graphics graphics(hdc);
		switch (r3) {
		case 0: {
			wchar_t name1[] = L"images\\letter_A.png";
			wcscpy_s(name, name1);
			break; }
		case 1: {
			wchar_t name1[] = L"images\\letter_B.png";
			wcscpy_s(name, name1);
			break; }
		case 2: {
			wchar_t name1[] = L"images\\letter_C.png";
			wcscpy_s(name, name1);
			break; }
		case 3: {
			wchar_t name1[] = L"images\\letter_D.png";
			wcscpy_s(name, name1);
			break; }
		case 4: {
			wchar_t name1[] = L"images\\letter_E.png";
			wcscpy_s(name, name1);
			break; }
		case 5: {
			wchar_t name1[] = L"images\\letter_F.png";
			wcscpy_s(name, name1);
			break; }
		case 6: {
			wchar_t name1[] = L"images\\letter_G.png";
			wcscpy_s(name, name1);
			break; }
		case 7: {
			wchar_t name1[] = L"images\\letter_H.png";
			wcscpy_s(name, name1);
			break; }
		case 8: {
			wchar_t name1[] = L"images\\letter_I.png";
			wcscpy_s(name, name1);
			break; }
		case 9: {
			wchar_t name1[] = L"images\\letter_J.png";
			wcscpy_s(name, name1);
			break; }
		case 10: {
			wchar_t name1[] = L"images\\letter_K.png";
			wcscpy_s(name, name1);
			break; }
		case 11: {
			wchar_t name1[] = L"images\\letter_L.png";
			wcscpy_s(name, name1);
			break; }
		case 12: {
			wchar_t name1[] = L"images\\letter_M.png";
			wcscpy_s(name, name1);
			break; }
		case 13: {
			wchar_t name1[] = L"images\\letter_N.png";
			wcscpy_s(name, name1);
			break; }
		case 14: {
			wchar_t name1[] = L"images\\letter_O.png";
			wcscpy_s(name, name1);
			break; }
		case 15: {
			wchar_t name1[] = L"images\\letter_P.png";
			wcscpy_s(name, name1);
			break; }
		case 16: {
			wchar_t name1[] = L"images\\letter_Q.png";
			wcscpy_s(name, name1);
			break; }
		case 17: {
			wchar_t name1[] = L"images\\letter_R.png";
			wcscpy_s(name, name1);
			break; }
		case 18: {
			wchar_t name1[] = L"images\\letter_S.png";
			wcscpy_s(name, name1);
			break; }
		case 19: {
			wchar_t name1[] = L"images\\letter_T.png";
			wcscpy_s(name, name1);
			break; }
		case 20: {
			wchar_t name1[] = L"images\\letter_U.png";
			wcscpy_s(name, name1);
			break; }
		case 21: {
			wchar_t name1[] = L"images\\letter_V.png";
			wcscpy_s(name, name1);
			break; }
		case 22: {
			wchar_t name1[] = L"images\\letter_W.png";
			wcscpy_s(name, name1);
			break; }
		case 23: {
			wchar_t name1[] = L"images\\letter_X.png";
			wcscpy_s(name, name1);
			break; }
		case 24: {
			wchar_t name1[] = L"images\\letter_Y.png";
			wcscpy_s(name, name1);
			break; }
		case 25: {
			wchar_t name1[] = L"images\\letter_Z.png";
			wcscpy_s(name, name1);
			break; }

		}

		Image image(name);
		graphics.DrawImage(&image, 624, 230);
	}
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	//SetProcessDPIAware();

	HWND hMainWnd = FindWindow(L"proga", L"Enigma");

	MSG msg;
	WNDCLASSEX wc;
	LPCWSTR szClassName = L"proga";
	wc.cbSize = sizeof(wc);
	wc.style = CS_HREDRAW | CS_VREDRAW;							//стиль класса окна (перерисовывает окно, если изменён размер
																//по горизонтали или по вертикали)
	wc.lpfnWndProc = WndProc;									//указатель на оконную процедуру
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;									//дескриптор экземпляра приложения, в котором находится оконная
																//процедура для этого класса
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND);		//дескриптор кисти, используемый для закраски фона окна
														//(функция GetStockObject возвратила дескриптор белой кисти
														//типа HGDIOBJ, поэтому требуется приведение к типу HBRUSH)
	wc.lpszMenuName = NULL;										//указатель на строку, содержащую имя меню, применяемого по
																//умолчанию для этого класса
	wc.lpszClassName = szClassName;								//указатель на строку, содержащую имя класса окна
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);				//дескриптор малой пиктограммы (загружена пиктограмма по умолчанию)
																/*****************************************************************************************************************************/
	if (!RegisterClassEx(&wc)) {
		MessageBox(NULL, L"Не удалось зарегистрировать класс окна", L"Ошибка", MB_OK);
		return 0;
	}

	hMainWnd = CreateWindow(szClassName, L"Enigma", WS_SYSMENU, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);
	HWND text = CreateWindow(L"edit", L"", WS_CHILD| WS_DISABLED | WS_VISIBLE | ES_LEFT | ES_AUTOHSCROLL,  473, 367, 618, 19, hMainWnd, (HMENU)3001, hInstance, NULL);
	HFONT hfn = CreateFont(20, 10, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, VARIABLE_PITCH, L"Cooper Black");

	SendMessage(text, WM_SETFONT, WPARAM(hfn), TRUE);
	if (!hMainWnd) {
		MessageBox(NULL, L"Не удалось создать главное окно программы", L"Ошибка", MB_OK);
		return 0;
	}
	ShowWindow(hMainWnd, SW_SHOWMAXIMIZED);
	ShowWindow(text, nCmdShow);
	HMENU hMainMenu = CreateMenu();

	AppendMenu(hMainMenu, MF_STRING , 1, L"Инструкция");
	AppendMenu(hMainMenu, MF_STRING, 2, L"Сохранить");
	SetMenu(hMainWnd, hMainMenu);

	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);

		DispatchMessage(&msg);
	}
	GdiplusShutdown(gdiplusToken);
	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT Message, UINT wParam, LONG lParam) {
	
	if (izmr1 == 0) {
		nastrI(I);
		nastrII(II);
		nastrIII(III);
	}

	switch (Message) {

	case WM_COMMAND: {
		switch (LOWORD(wParam)) {
		case 1111: {
			DialogBox(NULL, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, ROTOR);
			if (izmr1 != 0) {
				switch (izmr1) {
				case 1: {
					nastrI(I); break; }
				case 2: {
					nastrI(II); break; }
				case 3: {
					nastrI(III); break; }
				case 4: {
					nastrI(IV); break; }
				case 5: {
					nastrI(V); break; }
				}
				switch (izmr2) {
				case 1: {
					nastrII(I); break; }
				case 2: {
					nastrII(II); break; }
				case 3: {
					nastrII(III); break; }
				case 4: {
					nastrII(IV); break; }
				case 5: {
					nastrII(V); break; }
				}
				switch (izmr3) {
				case 1: {
					nastrIII(I); break; }
				case 2: {
					nastrIII(II); break; }
				case 3: {
					nastrIII(III); break; }
				case 4: {
					nastrIII(IV); break; }
				case 5: {
					nastrIII(V); break; }
				}
			}
					
			break;
		}

		case 2222: {
			DialogBox(NULL, MAKEINTRESOURCE(IDD_DIALOG2), hWnd, KOM);
			break; }

		case 1: {
			DialogBox(NULL, MAKEINTRESOURCE(IDD_DIALOG3), hWnd, INSTR);
			break; }
				
		case 2: {
			std::wfstream save;
			save.open("C:\\Users\\Никита\\Desktop\\saveEnigma.txt", std::ios::out);
			save << txt;
			save.close();
			MessageBox(hWnd, L"Сообщение сохранено", L"Сохранение", NULL);
		}
		}
		break; }

	case WM_LBUTTONDOWN: {
		int x1, y1;
		x1 = LOWORD(lParam);
		y1 = HIWORD(lParam);

		if ((x1 > 800 && x1 <830) && (y1 >170 && y1 < 230)) {
			PlaySound(TEXT("rotor.wav"), NULL, SND_ASYNC);
			nastr = false;
			if (r1 == 25) { r1 = 0; }
			else {
				r1++;
			}
			if (!nastr) {
				nastr = true;
				const RECT ab = { 780, 230, 810, 270 };
				InvalidateRect(hWnd, &ab, FALSE);
			}
		}

		if ((x1 > 800 && x1 <830) && (y1 >250 && y1 < 320)) {
			PlaySound(TEXT("rotor.wav"), NULL, SND_ASYNC);
			nastr = false;
			if (r1 == 0) { r1 = 25; }
			else {
				r1--;
			}
			if (!nastr) {
				nastr = true;
				const RECT ab = { 780, 230, 810, 270 };
				InvalidateRect(hWnd, &ab, FALSE);
			}
		}

		if ((x1 > 730 && x1 <760) && (y1 >170 && y1 < 230)) {
			PlaySound(TEXT("rotor.wav"), NULL, SND_ASYNC);
			nastr = false;
			if (r2 == 25) { r2 = 0; }
			else {
				r2++;
			}
			if (!nastr) {
				nastr = true;
				checkR2 = -1;
				const RECT ab = { 702, 230, 732, 270 };
				InvalidateRect(hWnd, &ab, FALSE);
			}
		}

		if ((x1 > 730 && x1 <760) && (y1 >250 && y1 < 320)) {
			PlaySound(TEXT("rotor.wav"), NULL, SND_ASYNC);
			nastr = false;
			if (r2 == 0) { r2 = 25; }
			else {
				r2--;
			}
			if (!nastr) {
				nastr = true;
				checkR2 = -1;
				const RECT ab = { 702, 230, 732, 270 };
				InvalidateRect(hWnd, &ab, FALSE);
			}
		}

		if ((x1 > 650 && x1 <680) && (y1 >170 && y1 < 230)) {
			PlaySound(TEXT("rotor.wav"), NULL, SND_ASYNC);
			nastr = false;
			if (r3 == 25) { r3 = 0; }
			else {
				r3++;
			}
			if (!nastr) {
				nastr = true;
				checkR3 = -1;
				const RECT ab = { 624, 230, 660, 270 };
				InvalidateRect(hWnd, &ab, FALSE);
			}
		}

		if ((x1 > 650 && x1 <680) && (y1 >250 && y1 < 320)) {
			PlaySound(TEXT("rotor.wav"), NULL, SND_ASYNC);
			nastr = false;
			if (r3 == 0) { r3 = 25; }
			else {
				r3--;
			}
			if (!nastr) {
				nastr = true;
				checkR3 = -1;
				const RECT ab = { 624, 230, 660, 270 };
				InvalidateRect(hWnd, &ab, FALSE);
			}
		}
		
		if ((x1 > 512 && x1 < 550) && (y1 > 220 && y1 < 270)) {
			PlaySound(TEXT("krugok.wav"), NULL, SND_ASYNC);
			if (ref == 4) { 
				ref = 1; }
			else { ref++; }
			switch (ref) {

			case 1: {
				for (int i = 0; i < 26; i++) {
					refStart[i] = ref1[i];
				}
				break; }

			case 2: {
				for (int i = 0; i < 26; i++) {
					refStart[i] = ref2[i];
				}
				break; }

			case 3: {
				for (int i = 0; i < 26; i++) {
					refStart[i] = ref3[i];
				}
				break; }

			case 4: {
				for (int i = 0; i < 26; i++) {
					refStart[i] = ref4[i];
				}
				break; }
			}
			const RECT aw = { 512, 220, 550, 280 };
					InvalidateRect(hWnd, &aw, FALSE); 		
		
		}
		if ((x1 > 860 && x1 < 920) && (y1 > 180 && y1 < 300)){
			PlaySound(TEXT("glkn.wav"), NULL, SND_ASYNC);
			if (!power) { power = true; }
			else if (power) { power = false; }
			const RECT aw = { 860, 180,920, 300 };
			InvalidateRect(hWnd, &aw, FALSE);
			wchar_t txt2[2] = L"";
			for(int i=0; i<n+1; i++){
				txt[i] = '\0';
			}
			n = 0;
			SetDlgItemText(hWnd, 3001, txt);
		}
		break; }
					
	case WM_PAINT: {
		
		HDC hDC;
		PAINTSTRUCT PaintStruct;
		hDC = BeginPaint(hWnd, &PaintStruct);
		golova(hDC);
		if(!zap){
			rotor_1(hDC, r1);
			rotor_2(hDC, r2, checkR2);
			rotor_3(hDC, r3, checkR3);
			reflector_A(hDC);
			zap = true;
		}

		switch (ref) {

		case 1: {
			reflector_A(hDC);
			break; }

		case 2: {
			reflector_B(hDC);
			break; }

		case 3: {
			reflector_C(hDC);
			break; }

		case 4: {
			reflector_D(hDC);
			break; }
		}

		if(power){
			Power(hDC);
			rotor_1(hDC, r1);
			rotor_2(hDC, r2, checkR2);
			rotor_3(hDC, r3, checkR3);
		}
		
		if (check == 1) {

			switch (buk) {

			case 0: {
				buttonApressed(hDC);
				break; }

			case 18: {
				buttonSpressed(hDC);
				break; }

			case 3: {
				button_D_pressed(hDC);
				break; }

			case 5: {
				button_F_pressed(hDC);
				break; }

			case 6: {
				button_G_pressed(hDC);
				break; }

			case 7: {
				button_H_pressed(hDC);
				break; }

			case 9: {
				button_J_pressed(hDC);
				break; }

			case 10: {
				button_K_pressed(hDC);
				break; }

			case 16: {
				button_Q_pressed(hDC);
				break; }

			case 22: {
				button_W_pressed(hDC);
				break; }

			case 4: {
				button_E_pressed(hDC);
				break; }

			case 17: {
				button_R_pressed(hDC);
				break; }

			case 19: {
				button_T_pressed(hDC);
				break; }

			case 25: {
				button_Z_pressed(hDC);
				break; }

			case 20: {
				button_U_pressed(hDC);
				break; }

			case 8: {
				button_I_pressed(hDC);
				break; }

			case 14: {
				button_O_pressed(hDC);
				break; }

			case 15: {
				button_P_pressed(hDC);
				break; }

			case 24: {
				button_Y_pressed(hDC);
				break; }

			case 23: {
				button_X_pressed(hDC);
				break; }

			case 2: {
				button_C_pressed(hDC);
				break; }

			case 21: {
				button_V_pressed(hDC);
				break; }

			case 1: {
				button_B_pressed(hDC);
				break; }

			case 13: {
				button_N_pressed(hDC);
				break; }

			case 11: {
				button_L_pressed(hDC);
				break; }

			case 12: {
				button_M_pressed(hDC);
				break; }

			}

			switch (buk2) {

			case 0: {
				lamp_A_pressed(hDC);
				break; }

			case 18: {
				lamp_S_pressed(hDC);
				break; }

			case 3: {
				lamp_D_pressed(hDC);
				break; }

			case 5: {
				lamp_F_pressed(hDC);
				break; }

			case 6: {
				lamp_G_pressed(hDC);
				break; }

			case 7: {
				lamp_H_pressed(hDC);
				break; }

			case 9: {
				lamp_J_pressed(hDC);
				break; }

			case 10: {
				lamp_K_pressed(hDC);
				break; }

			case 16: {
				lamp_Q_pressed(hDC);
				break; }

			case 22: {
				lamp_W_pressed(hDC);
				break; }

			case 4: {
				lamp_E_pressed(hDC);
				break; }

			case 17: {
				lamp_R_pressed(hDC);
				break; }

			case 19: {
				lamp_T_pressed(hDC);
				break; }

			case 25: {
				lamp_Z_pressed(hDC);
				break; }

			case 20: {
				lamp_U_pressed(hDC);
				break; }

			case 8: {
				lamp_I_pressed(hDC);
				break; }

			case 14: {
				lamp_O_pressed(hDC);
				break; }

			case 15: {
				lamp_P_pressed(hDC);
				break; }

			case 24: {
				lamp_Y_pressed(hDC);
				break; }

			case 23: {
				lamp_X_pressed(hDC);
				break; }

			case 2: {
				lamp_C_pressed(hDC);
				break; }

			case 21: {
				lamp_V_pressed(hDC);
				break; }

			case 1: {
				lamp_B_pressed(hDC);
				break; }

			case 13: {
				lamp_N_pressed(hDC);
				break; }

			case 11: {
				lamp_L_pressed(hDC);
				break; }

			case 12: {
				lamp_M_pressed(hDC);
				break; }

			}


		}
		
		EndPaint(hWnd, &PaintStruct);
		break;
	}

	case WM_KEYDOWN: {
		if (power) {
		
			checkR2 = r2;
			checkR3 = r3;
			switch (wParam)
			{
			case 0x41: {
				
				if (check == 0) {
					PlaySound(TEXT("klavishi.wav"), NULL, SND_ASYNC);
					check = 1;
					buk = 0;
					const RECT aw = { 553, 615, 598, 660 };
					InvalidateRect(hWnd, &aw, FALSE);
					buk2 = perevod(buk, r1, r2, r3, rotorI, rotorII, rotorIII);
					rotorplus();
					wchar_t strtmp[45];
					if (n > 40) {
						wcsncpy_s(strtmp, txt+(n-40), 40);
					}
					else { wcscpy_s(strtmp, txt); }
					SetDlgItemText(hWnd, 3001, strtmp);
					const RECT ab = { 500, 400, 1055, 560 };
					InvalidateRect(hWnd, &ab, FALSE);

				}
				break; }

			case 0x53: {
				
				if (check == 0) {
					PlaySound(TEXT("klavishi.wav"), NULL, SND_ASYNC);
					check = 1;
					buk = 18;
					const RECT aw = { 613, 615, 658, 660 };
					InvalidateRect(hWnd, &aw, FALSE);
					buk2 = perevod(buk, r1, r2, r3, rotorI, rotorII, rotorIII);
					txt[n] = alf[buk2];
					rotorplus();
					wchar_t strtmp[45];
					if (n > 40) {

						wcsncpy_s(strtmp, txt + (n - 40), 40);

					}
					else { wcscpy_s(strtmp, txt); }
					SetDlgItemText(hWnd, 3001, strtmp);
					const RECT ab = { 500, 400, 1055, 560 };
					InvalidateRect(hWnd, &ab, FALSE);
				}
				break; }

			case 0x44: {
				if (check == 0) {
					PlaySound(TEXT("klavishi.wav"), NULL, SND_ASYNC);
					check = 1;
					buk = 3;
					const RECT aw = { 672, 615, 717, 660 };
					InvalidateRect(hWnd, &aw, FALSE);
					buk2 = perevod(buk, r1, r2, r3, rotorI, rotorII, rotorIII);
					rotorplus();
					wchar_t strtmp[45];
					if (n > 40) {

						wcsncpy_s(strtmp, txt + (n - 40), 40);

					}
					else { wcscpy_s(strtmp, txt); }
					SetDlgItemText(hWnd, 3001, strtmp);
					const RECT ab = { 500, 400, 1055, 560 };
					InvalidateRect(hWnd, &ab, FALSE);
				}
				break; }

			case 0x46: {
				if (check == 0) {
					PlaySound(TEXT("klavishi.wav"), NULL, SND_ASYNC);
					check = 1;
					buk = 5;
					const RECT aw = { 731, 615, 776, 660 };
					InvalidateRect(hWnd, &aw, FALSE);
					buk2 = perevod(buk, r1, r2, r3, rotorI, rotorII, rotorIII);
					rotorplus();
					wchar_t strtmp[45];
					if (n > 40) {

						wcsncpy_s(strtmp, txt + (n - 40), 40);

					}
					else { wcscpy_s(strtmp, txt); }
					SetDlgItemText(hWnd, 3001, strtmp);
					const RECT ab = { 500, 400, 1055, 560 };
					InvalidateRect(hWnd, &ab, FALSE);
				}
				break; }

			case 0x47: {
				if (check == 0) {
					PlaySound(TEXT("klavishi.wav"), NULL, SND_ASYNC);
					check = 1;
					buk = 6;
					const RECT aw = { 791, 615, 836, 660 };
					InvalidateRect(hWnd, &aw, FALSE);
					buk2 = perevod(buk, r1, r2, r3, rotorI, rotorII, rotorIII);
					rotorplus();
					wchar_t strtmp[45];
					if (n > 40) {

						wcsncpy_s(strtmp, txt + (n - 40), 40);

					}
					else { wcscpy_s(strtmp, txt); }
					SetDlgItemText(hWnd, 3001, strtmp);
					const RECT ab = { 500, 400, 1055, 560 };
					InvalidateRect(hWnd, &ab, FALSE);
				}
				break; }

			case 0x48: {
				if (check == 0) {
					PlaySound(TEXT("klavishi.wav"), NULL, SND_ASYNC);
					check = 1;
					buk = 7;
					const RECT aw = { 850, 615, 895, 660 };
					InvalidateRect(hWnd, &aw, FALSE);
					buk2 = perevod(buk, r1, r2, r3, rotorI, rotorII, rotorIII);
					rotorplus();
					wchar_t strtmp[45];
					if (n > 40) {

						wcsncpy_s(strtmp, txt + (n - 40), 40);

					}
					else { wcscpy_s(strtmp, txt); }
					SetDlgItemText(hWnd, 3001, strtmp);
					const RECT ab = { 500, 400, 1055, 560 };
					InvalidateRect(hWnd, &ab, FALSE);
				}
				break; }

			case 0x4A: {
				if (check == 0) {
					PlaySound(TEXT("klavishi.wav"), NULL, SND_ASYNC);
					check = 1;
					buk = 9;
					const RECT aw = { 910, 615, 955, 660 };
					InvalidateRect(hWnd, &aw, FALSE);
					buk2 = perevod(buk, r1, r2, r3, rotorI, rotorII, rotorIII);
					rotorplus();
					wchar_t strtmp[45];
					if (n > 40) {
						wcsncpy_s(strtmp, txt + (n - 40), 40);
					}
					else { wcscpy_s(strtmp, txt); }
					SetDlgItemText(hWnd, 3001, strtmp);
					const RECT ab = { 500, 400, 1055, 560 };
					InvalidateRect(hWnd, &ab, FALSE);
				}
				break; }

			case 0x4B: {
				if (check == 0) {
					PlaySound(TEXT("klavishi.wav"), NULL, SND_ASYNC);
					check = 1;
					buk = 10;
					const RECT aw = { 969, 615, 1014, 660 };
					InvalidateRect(hWnd, &aw, FALSE);
					buk2 = perevod(buk, r1, r2, r3, rotorI, rotorII, rotorIII);
					rotorplus();
					wchar_t strtmp[45];
					if (n > 40) {
						wcsncpy_s(strtmp, txt + (n - 40), 40);
					}
					else { wcscpy_s(strtmp, txt); }
					SetDlgItemText(hWnd, 3001, strtmp);
					const RECT ab = { 500, 400, 1055, 560 };
					InvalidateRect(hWnd, &ab, FALSE);
				}
				break; }

			case 0x51: {
				if (check == 0) {
					PlaySound(TEXT("klavishi.wav"), NULL, SND_ASYNC);
					check = 1;
					buk = 16;
					const RECT aw = { 523, 563, 568, 608 };
					InvalidateRect(hWnd, &aw, FALSE);
					buk2 = perevod(buk, r1, r2, r3, rotorI, rotorII, rotorIII);
					rotorplus();
					wchar_t strtmp[45];
					if (n > 40) {
						wcsncpy_s(strtmp, txt + (n - 40), 40);
					}
					else { wcscpy_s(strtmp, txt); }
					SetDlgItemText(hWnd, 3001, strtmp);
					const RECT ab = { 500, 400, 1055, 560 };
					InvalidateRect(hWnd, &ab, FALSE);
				}
				break; }

			case 0x57: {
				if (check == 0) {
					PlaySound(TEXT("klavishi.wav"), NULL, SND_ASYNC);
					check = 1;
					buk = 22;
					const RECT aw = { 583, 563, 628, 608 };
					InvalidateRect(hWnd, &aw, FALSE);
					buk2 = perevod(buk, r1, r2, r3, rotorI, rotorII, rotorIII);
					rotorplus();
					wchar_t strtmp[45];
					if (n > 40) {
						wcsncpy_s(strtmp, txt + (n - 40), 40);
					}
					else { wcscpy_s(strtmp, txt); }
					SetDlgItemText(hWnd, 3001, strtmp);
					const RECT ab = { 500, 400, 1055, 560 };
					InvalidateRect(hWnd, &ab, FALSE);
				}
				break; }

			case 0x59: {
				if (check == 0) {
					PlaySound(TEXT("klavishi.wav"), NULL, SND_ASYNC);
					check = 1;
					buk = 24;
					const RECT aw = { 583, 667, 628, 712 };
					InvalidateRect(hWnd, &aw, FALSE);
					buk2 = perevod(buk, r1, r2, r3, rotorI, rotorII, rotorIII);
					rotorplus();
					wchar_t strtmp[45];
					if (n > 40) {
						wcsncpy_s(strtmp, txt + (n - 40), 40);
					}
					else { wcscpy_s(strtmp, txt); }
					SetDlgItemText(hWnd, 3001, strtmp);
					const RECT ab = { 500, 400, 1055, 560 };
					InvalidateRect(hWnd, &ab, FALSE);
				}
				break; }

			case 0x45: {
				if (check == 0) {
					PlaySound(TEXT("klavishi.wav"), NULL, SND_ASYNC);
					check = 1;
					buk = 4;
					const RECT aw = { 642, 563, 687, 608 };
					InvalidateRect(hWnd, &aw, FALSE);
					buk2 = perevod(buk, r1, r2, r3, rotorI, rotorII, rotorIII);
					rotorplus();
					wchar_t strtmp[45];
					if (n > 40) {
						wcsncpy_s(strtmp, txt + (n - 40), 40);
					}
					else { wcscpy_s(strtmp, txt); }
					SetDlgItemText(hWnd, 3001, strtmp);
					const RECT ab = { 500, 400, 1055, 560 };
					InvalidateRect(hWnd, &ab, FALSE);
				}
				break; }

			case 0x58: {
				if (check == 0) {
					PlaySound(TEXT("klavishi.wav"), NULL, SND_ASYNC);
					check = 1;
					buk = 23;
					const RECT aw = { 642, 667, 687, 712 };
					InvalidateRect(hWnd, &aw, FALSE);
					buk2 = perevod(buk, r1, r2, r3, rotorI, rotorII, rotorIII);
					rotorplus();
					wchar_t strtmp[45];
					if (n > 40) {
						wcsncpy_s(strtmp, txt + (n - 40), 40);
					}
					else { wcscpy_s(strtmp, txt); }
					SetDlgItemText(hWnd, 3001, strtmp);
					const RECT ab = { 500, 400, 1055, 560 };
					InvalidateRect(hWnd, &ab, FALSE);
				}
				break; }

			case 0x52: {
				if (check == 0) {
					PlaySound(TEXT("klavishi.wav"), NULL, SND_ASYNC);
					check = 1;
					buk = 17;
					const RECT aw = { 701, 563, 746, 608 };
					InvalidateRect(hWnd, &aw, FALSE);
					buk2 = perevod(buk, r1, r2, r3, rotorI, rotorII, rotorIII);
					rotorplus();
					wchar_t strtmp[45];
					if (n > 40) {
						wcsncpy_s(strtmp, txt + (n - 40), 40);
					}
					else { wcscpy_s(strtmp, txt); }
					SetDlgItemText(hWnd, 3001, strtmp);
					const RECT ab = { 500, 400, 1055, 560 };
					InvalidateRect(hWnd, &ab, FALSE);
				}
				break; }

			case 0x43: {
				if (check == 0) {
					PlaySound(TEXT("klavishi.wav"), NULL, SND_ASYNC);
					check = 1;
					buk = 2;
					const RECT aw = { 701, 667, 746, 712 };
					InvalidateRect(hWnd, &aw, FALSE);
					buk2 = perevod(buk, r1, r2, r3, rotorI, rotorII, rotorIII);
					rotorplus();
					wchar_t strtmp[45];
					if (n > 40) {
						wcsncpy_s(strtmp, txt + (n - 40), 40);
					}
					else { wcscpy_s(strtmp, txt); }
					SetDlgItemText(hWnd, 3001, strtmp);
					const RECT ab = { 500, 400, 1055, 560 };
					InvalidateRect(hWnd, &ab, FALSE);
				}
				break; }

			case 0x54: {
				if (check == 0) {
					PlaySound(TEXT("klavishi.wav"), NULL, SND_ASYNC);
					check = 1;
					buk = 19;
					const RECT aw = { 761, 563, 806, 608 };
					InvalidateRect(hWnd, &aw, FALSE);
					buk2 = perevod(buk, r1, r2, r3, rotorI, rotorII, rotorIII);
					rotorplus();
					wchar_t strtmp[45];
					if (n > 40) {
						wcsncpy_s(strtmp, txt + (n - 40), 40);
					}
					else { wcscpy_s(strtmp, txt); }
					SetDlgItemText(hWnd, 3001, strtmp);
					const RECT ab = { 500, 400, 1055, 560 };
					InvalidateRect(hWnd, &ab, FALSE);
				}
				break; }

			case 0x56: {
				if (check == 0) {
					PlaySound(TEXT("klavishi.wav"), NULL, SND_ASYNC);
					check = 1;
					buk = 21;
					const RECT aw = { 761, 667, 806, 712 };
					InvalidateRect(hWnd, &aw, FALSE);
					buk2 = perevod(buk, r1, r2, r3, rotorI, rotorII, rotorIII);
					rotorplus();
					wchar_t strtmp[45];
					if (n > 40) {
						wcsncpy_s(strtmp, txt + (n - 40), 40);
					}
					else { wcscpy_s(strtmp, txt); }
					SetDlgItemText(hWnd, 3001, strtmp);
					const RECT ab = { 500, 400, 1055, 560 };
					InvalidateRect(hWnd, &ab, FALSE);
				}
				break; }

			case 0x5A: {
				if (check == 0) {
					PlaySound(TEXT("klavishi.wav"), NULL, SND_ASYNC);
					check = 1;
					buk = 25;
					const RECT aw = { 821, 563, 866, 608 };
					InvalidateRect(hWnd, &aw, FALSE);
					buk2 = perevod(buk, r1, r2, r3, rotorI, rotorII, rotorIII);
					rotorplus();
					wchar_t strtmp[45];
					if (n > 40) {
						wcsncpy_s(strtmp, txt + (n - 40), 40);
					}
					else { wcscpy_s(strtmp, txt); }
					SetDlgItemText(hWnd, 3001, strtmp);
					const RECT ab = { 500, 400, 1055, 560 };
					InvalidateRect(hWnd, &ab, FALSE);
				}
				break; }

			case 0x42: {
				if (check == 0) {
					PlaySound(TEXT("klavishi.wav"), NULL, SND_ASYNC);
					check = 1;
					buk = 1;
					const RECT aw = { 821, 667, 866, 712 };
					InvalidateRect(hWnd, &aw, FALSE);
					buk2 = perevod(buk, r1, r2, r3, rotorI, rotorII, rotorIII);
					rotorplus();
					wchar_t strtmp[45];
					if (n > 40) {
						wcsncpy_s(strtmp, txt + (n - 40), 40);
					}
					else { wcscpy_s(strtmp, txt); }
					SetDlgItemText(hWnd, 3001, strtmp);
					const RECT ab = { 500, 400, 1055, 560 };
					InvalidateRect(hWnd, &ab, FALSE);
				}
				break; }

			case 0x55: {
				if (check == 0) {
					PlaySound(TEXT("klavishi.wav"), NULL, SND_ASYNC);
					check = 1;
					buk = 20;
					const RECT aw = { 880, 563, 925, 608 };
					InvalidateRect(hWnd, &aw, FALSE);
					buk2 = perevod(buk, r1, r2, r3, rotorI, rotorII, rotorIII);
					rotorplus();
					wchar_t strtmp[45];
					if (n > 40) {
						wcsncpy_s(strtmp, txt + (n - 40), 40);
					}
					else { wcscpy_s(strtmp, txt); }
					SetDlgItemText(hWnd, 3001, strtmp);
					const RECT ab = { 500, 400, 1055, 560 };
					InvalidateRect(hWnd, &ab, FALSE);
				}
				break; }

			case 0x4E: {
				if (check == 0) {
					PlaySound(TEXT("klavishi.wav"), NULL, SND_ASYNC);
					check = 1;
					buk = 13;
					const RECT aw = { 880, 667, 925, 712 };
					InvalidateRect(hWnd, &aw, FALSE);
					buk2 = perevod(buk, r1, r2, r3, rotorI, rotorII, rotorIII);
					rotorplus();
					wchar_t strtmp[45];
					if (n > 40) {
						wcsncpy_s(strtmp, txt + (n - 40), 40);
					}
					else { wcscpy_s(strtmp, txt); }
					SetDlgItemText(hWnd, 3001, strtmp);
					const RECT ab = { 500, 400, 1055, 560 };
					InvalidateRect(hWnd, &ab, FALSE);
				}
				break; }

			case 0x49: {
				if (check == 0) {
					PlaySound(TEXT("klavishi.wav"), NULL, SND_ASYNC);
					check = 1;
					buk = 8;
					const RECT aw = { 939, 563, 984, 608 };
					InvalidateRect(hWnd, &aw, FALSE);
					buk2 = perevod(buk, r1, r2, r3, rotorI, rotorII, rotorIII);
					rotorplus();
					wchar_t strtmp[45];
					if (n > 40) {
						wcsncpy_s(strtmp, txt + (n - 40), 40);
					}
					else { wcscpy_s(strtmp, txt); }
					SetDlgItemText(hWnd, 3001, strtmp);
					const RECT ab = { 500, 400, 1055, 560 };
					InvalidateRect(hWnd, &ab, FALSE);
				}
				break; }

			case 0x4D: {
				if (check == 0) {
					PlaySound(TEXT("klavishi.wav"), NULL, SND_ASYNC);
					check = 1;
					buk = 12;
					const RECT aw = { 939, 667, 984, 712 };
					InvalidateRect(hWnd, &aw, FALSE);
					buk2 = perevod(buk, r1, r2, r3, rotorI, rotorII, rotorIII);
					rotorplus();
					wchar_t strtmp[45];
					if (n > 40) {
						wcsncpy_s(strtmp, txt + (n - 40), 40);
					}
					else { wcscpy_s(strtmp, txt); }
					SetDlgItemText(hWnd, 3001, strtmp);
					const RECT ab = { 500, 400, 1055, 560 };
					InvalidateRect(hWnd, &ab, FALSE);
				}
				break; }

			case 0x4F: {
				if (check == 0) {
					PlaySound(TEXT("klavishi.wav"), NULL, SND_ASYNC);
					check = 1;
					buk = 14;
					const RECT aw = { 999, 563, 1044, 608 };
					InvalidateRect(hWnd, &aw, FALSE);
					buk2 = perevod(buk, r1, r2, r3, rotorI, rotorII, rotorIII);
					rotorplus();
					wchar_t strtmp[45];
					if (n > 40) {
						wcsncpy_s(strtmp, txt + (n - 40), 40);
					}
					else { wcscpy_s(strtmp, txt); }
					SetDlgItemText(hWnd, 3001, strtmp);
					const RECT ab = { 500, 400, 1055, 560 };
					InvalidateRect(hWnd, &ab, FALSE);
				}
				break; }

			case 0x4C: {
				if (check == 0) {
					PlaySound(TEXT("klavishi.wav"), NULL, SND_ASYNC);
					check = 1;
					buk = 11;
					const RECT aw = { 999, 667, 1044, 712 };
					InvalidateRect(hWnd, &aw, FALSE);
					buk2 = perevod(buk, r1, r2, r3, rotorI, rotorII, rotorIII);
					rotorplus();
					wchar_t strtmp[45];
					if (n > 40) {
						wcsncpy_s(strtmp, txt + (n - 40), 40);
					}
					else { wcscpy_s(strtmp, txt); }
					SetDlgItemText(hWnd, 3001, strtmp);
					const RECT ab = { 500, 400, 1055, 560 };
					InvalidateRect(hWnd, &ab, FALSE);
				}
				break; }

			case 0x50: {
				if (check == 0) {
					PlaySound(TEXT("klavishi.wav"), NULL, SND_ASYNC);
					check = 1;
					buk = 15;
					const RECT aw = { 523, 670,  568, 715 };
					InvalidateRect(hWnd, &aw, FALSE);
					buk2 = perevod(buk, r1, r2, r3, rotorI, rotorII, rotorIII);
					rotorplus();
					wchar_t strtmp[45];
					if (n > 40) {
						wcsncpy_s(strtmp, txt + (n - 40), 40);
					}
					else { wcscpy_s(strtmp, txt); }
					SetDlgItemText(hWnd, 3001, strtmp);
					const RECT ab = { 500, 400, 1055, 560 };
					InvalidateRect(hWnd, &ab, FALSE);
				}
				break; }

			case 8: {

				if (n != 0) {
					txt[n - 1] = '\0';
					n--;
					if (r1 == 0) {
						r1 = 25;
					}
					else if (r1 == 6 || r1 == 18) { r2--; r1--; }
					else { r1--; }

					if (r2 == -1) { r2 = 25; }
					else if (r2 == 5 || r2 == 17) { r3--; r2--; }
					wchar_t strtmp[45];
					if (n > 40) {
						wcsncpy_s(strtmp, txt + (n - 40), 40);
					}
					else { wcscpy_s(strtmp, txt); }
					SetDlgItemText(hWnd, 3001, strtmp);

				}

				if (!nastr) {
					nastr = true;
					if (checkR3 != r3) {
						const RECT ab = { 624, 230, 660, 270 };
						InvalidateRect(hWnd, &ab, FALSE);
					}
					if (checkR2 != r2) {
						const RECT ab = { 702, 230, 732, 270 };
						InvalidateRect(hWnd, &ab, FALSE);
					}

					const RECT ab = { 780, 230, 810, 270 };
					InvalidateRect(hWnd, &ab, FALSE);

				}

				break; }
			}

			if (!nastr) {
				nastr = true;
				if (checkR3 != r3) {
					const RECT ab = { 624, 230, 660, 270 };
					InvalidateRect(hWnd, &ab, FALSE);
				}
				if (checkR2 != r2) {
					const RECT ab = { 702, 230, 732, 270 };
					InvalidateRect(hWnd, &ab, FALSE);
				}

				const RECT ab = { 780, 230, 810, 270 };
				InvalidateRect(hWnd, &ab, FALSE);

			}
		}
			break;	
	}
	
	 case WM_KEYUP: {
		 if (power) {
			 switch (wParam)
			 {
			 case 0x41: {
				 check = 0;
				 const RECT aw = { 553, 615, 598, 660 };
				 InvalidateRect(hWnd, &aw, FALSE);
				 const RECT ab = { 500, 400, 1055, 560 };
				 InvalidateRect(hWnd, &ab, FALSE); break; }

			 case 0x53: {
				 check = 0;
				 const RECT aw = { 613, 615, 658, 660 };
				 InvalidateRect(hWnd, &aw, FALSE);
				 const RECT ab = { 500, 400, 1055, 560 };
				 InvalidateRect(hWnd, &ab, FALSE); break; }

			 case 0x44: {
				 check = 0;
				 const RECT aw = { 672, 615, 717, 660 };
				 InvalidateRect(hWnd, &aw, FALSE);
				 const RECT ab = { 500, 400, 1055, 560 };
				 InvalidateRect(hWnd, &ab, FALSE); break; }

			 case 0x46: {
				 check = 0;
				 const RECT aw = { 731, 615, 776, 660 };
				 InvalidateRect(hWnd, &aw, FALSE);
				 const RECT ab = { 500, 400, 1055, 560 };
				 InvalidateRect(hWnd, &ab, FALSE); break; }

			 case 0x47: {
				 check = 0;
				 const RECT aw = { 791, 615, 836, 660 };
				 InvalidateRect(hWnd, &aw, FALSE);
				 const RECT ab = { 500, 400, 1055, 560 };
				 InvalidateRect(hWnd, &ab, FALSE); break; }

			 case 0x48: {
				 check = 0;
				 const RECT aw = { 850, 615, 895, 660 };
				 InvalidateRect(hWnd, &aw, FALSE);
				 const RECT ab = { 500, 400, 1055, 560 };
				 InvalidateRect(hWnd, &ab, FALSE); break; }

			 case 0x4A: {
				 check = 0;
				 const RECT aw = { 910, 615, 955, 660 };
				 InvalidateRect(hWnd, &aw, FALSE);
				 const RECT ab = { 500, 400, 1055, 560 };
				 InvalidateRect(hWnd, &ab, FALSE); break; }

			 case 0x4B: {
				 check = 0;
				 const RECT aw = { 969, 615, 1014, 660 };
				 InvalidateRect(hWnd, &aw, FALSE);
				 const RECT ab = { 500, 400, 1055, 560 };
				 InvalidateRect(hWnd, &ab, FALSE); break; }

			 case 0x51: {
				 check = 0;
				 const RECT aw = { 523, 563, 568, 608 };
				 InvalidateRect(hWnd, &aw, FALSE);
				 const RECT ab = { 500, 400, 1055, 560 };
				 InvalidateRect(hWnd, &ab, FALSE); break; }

			 case 0x57: {
				 check = 0;
				 const RECT aw = { 583, 563, 628, 608 };
				 InvalidateRect(hWnd, &aw, FALSE);
				 const RECT ab = { 500, 400, 1055, 560 };
				 InvalidateRect(hWnd, &ab, FALSE); break; }

			 case 0x45: {
				 check = 0;
				 const RECT aw = { 642, 563, 687, 608 };
				 InvalidateRect(hWnd, &aw, FALSE);
				 const RECT ab = { 500, 400, 1055, 560 };
				 InvalidateRect(hWnd, &ab, FALSE); break; }

			 case 0x52: {
				 check = 0;
				 const RECT aw = { 701, 563, 746, 608 };
				 InvalidateRect(hWnd, &aw, FALSE);
				 const RECT ab = { 500, 400, 1055, 560 };
				 InvalidateRect(hWnd, &ab, FALSE); break; }

			 case 0x54: {
				 check = 0;
				 const RECT aw = { 761, 563, 806, 608 };
				 InvalidateRect(hWnd, &aw, FALSE);
				 const RECT ab = { 500, 400, 1055, 560 };
				 InvalidateRect(hWnd, &ab, FALSE); break; }

			 case 0x5A: {
				 check = 0;
				 const RECT aw = { 821, 563, 866, 608 };
				 InvalidateRect(hWnd, &aw, FALSE);
				 const RECT ab = { 500, 400, 1055, 560 };
				 InvalidateRect(hWnd, &ab, FALSE); break; }

			 case 0x55: {
				 check = 0;
				 const RECT aw = { 880, 563, 925, 608 };
				 InvalidateRect(hWnd, &aw, FALSE);
				 const RECT ab = { 500, 400, 1055, 560 };
				 InvalidateRect(hWnd, &ab, FALSE); break; }

			 case 0x49: {
				 check = 0;
				 const RECT aw = { 939, 563, 984, 608 };
				 InvalidateRect(hWnd, &aw, FALSE);
				 const RECT ab = { 500, 400, 1055, 560 };
				 InvalidateRect(hWnd, &ab, FALSE); break; }

			 case 0x4f: {
				 check = 0;
				 const RECT aw = { 999, 563, 1044, 608 };
				 InvalidateRect(hWnd, &aw, FALSE);
				 const RECT ab = { 500, 400, 1055, 560 };
				 InvalidateRect(hWnd, &ab, FALSE); break; }

			 case 0x50: {
				 check = 0;
				 const RECT aw = { 523, 670,  568, 715 };
				 InvalidateRect(hWnd, &aw, FALSE);
				 const RECT ab = { 500, 400, 1055, 560 };
				 InvalidateRect(hWnd, &ab, FALSE); break; }

			 case 0x4C: {
				 check = 0;
				 const RECT aw = { 999, 667, 1044, 712 };
				 InvalidateRect(hWnd, &aw, FALSE);
				 const RECT ab = { 500, 400, 1055, 560 };
				 InvalidateRect(hWnd, &ab, FALSE); break; }

			 case 0x4D: {
				 check = 0;
				 const RECT aw = { 939, 667, 984, 712 };
				 InvalidateRect(hWnd, &aw, FALSE);
				 const RECT ab = { 500, 400, 1055, 560 };
				 InvalidateRect(hWnd, &ab, FALSE); break; }

			 case 0x4E: {
				 check = 0;
				 const RECT aw = { 880, 667, 925, 712 };
				 InvalidateRect(hWnd, &aw, FALSE);
				 const RECT ab = { 500, 400, 1055, 560 };
				 InvalidateRect(hWnd, &ab, FALSE); break; }

			 case 0x42: {
				 check = 0;
				 const RECT aw = { 821, 667, 866, 712 };
				 InvalidateRect(hWnd, &aw, FALSE);
				 const RECT ab = { 500, 400, 1055, 560 };
				 InvalidateRect(hWnd, &ab, FALSE); break; }

			 case 0x56: {
				 check = 0;
				 const RECT aw = { 761, 667, 806, 712 };
				 InvalidateRect(hWnd, &aw, FALSE);
				 const RECT ab = { 500, 400, 1055, 560 };
				 InvalidateRect(hWnd, &ab, FALSE); break; }

			 case 0x43: {
				 check = 0;
				 const RECT aw = { 701, 667, 746, 712 };
				 InvalidateRect(hWnd, &aw, FALSE);
				 const RECT ab = { 500, 400, 1055, 560 };
				 InvalidateRect(hWnd, &ab, FALSE); break; }

			 case 0x58: {
				 check = 0;
				 const RECT aw = { 642, 667, 687, 712 };
				 InvalidateRect(hWnd, &aw, FALSE);
				 const RECT ab = { 500, 400, 1055, 560 };
				 InvalidateRect(hWnd, &ab, FALSE); break; }

			 case 0x59: {
				 check = 0;
				 const RECT aw = { 583, 667, 628, 712 };
				 InvalidateRect(hWnd, &aw, FALSE); const RECT ab = { 500, 400, 1055, 560 };
				 InvalidateRect(hWnd, &ab, FALSE);
				 break; }

			 }
			 nastr = false;
		 }
		 break;
	}

	case WM_CONTEXTMENU:{
		HMENU hMenu = CreatePopupMenu();
	
		AppendMenu(hMenu, MFT_STRING, 1111, L"Настройка роторов");
		AppendMenu(hMenu, MFT_STRING, 2222, L"Настройка коммутационной панели");
	
		TrackPopupMenu(hMenu, TPM_RIGHTBUTTON | TPM_TOPALIGN | TPM_LEFTALIGN, LOWORD(lParam), HIWORD(lParam), 0, hWnd, NULL);
		DestroyMenu(hMenu);
		break;
	}

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, Message, wParam, lParam);
}

BOOL CALLBACK ROTOR(HWND hWnd, UINT Message, UINT wParam, LONG lParam) {

	switch (Message) {
	
	case WM_INITDIALOG: {
	
		SendDlgItemMessage(hWnd, IDC_R4, CB_ADDSTRING, NULL, (LPARAM)"1");
		SendDlgItemMessage(hWnd, IDC_R4, CB_ADDSTRING, NULL, (LPARAM)"2");
		SendDlgItemMessage(hWnd, IDC_R4, CB_ADDSTRING, NULL, (LPARAM)"3");
		SendDlgItemMessage(hWnd, IDC_R4, CB_ADDSTRING, NULL, (LPARAM)"4");
		SendDlgItemMessage(hWnd, IDC_R4, CB_ADDSTRING, NULL, (LPARAM)"5");
	
		SendDlgItemMessage(hWnd, IDC_R2, CB_ADDSTRING, NULL, (LPARAM)"1");
		SendDlgItemMessage(hWnd, IDC_R2, CB_ADDSTRING, NULL, (LPARAM)"2");
		SendDlgItemMessage(hWnd, IDC_R2, CB_ADDSTRING, NULL, (LPARAM)"3");
		SendDlgItemMessage(hWnd, IDC_R2, CB_ADDSTRING, NULL, (LPARAM)"4");
		SendDlgItemMessage(hWnd, IDC_R2, CB_ADDSTRING, NULL, (LPARAM)"5");

		SendDlgItemMessage(hWnd, IDC_R3, CB_ADDSTRING, NULL, (LPARAM)"1");
		SendDlgItemMessage(hWnd, IDC_R3, CB_ADDSTRING, NULL, (LPARAM)"2");
		SendDlgItemMessage(hWnd, IDC_R3, CB_ADDSTRING, NULL, (LPARAM)"3");
		SendDlgItemMessage(hWnd, IDC_R3, CB_ADDSTRING, NULL, (LPARAM)"4");
		SendDlgItemMessage(hWnd, IDC_R3, CB_ADDSTRING, NULL, (LPARAM)"5");

		break; }

	case WM_COMMAND: {
		if (LOWORD(wParam) == IDC_OK ) {

			wchar_t txt[10];
			GetDlgItemText(hWnd, IDC_R4, txt, 10);
			swscanf_s(txt, L"%d", &izmr1);
			GetDlgItemText(hWnd, IDC_R2, txt, 10);
			swscanf_s(txt, L"%d", &izmr2);
			GetDlgItemText(hWnd, IDC_R3, txt, 10);
			swscanf_s(txt, L"%d", &izmr3);
			if ((izmr1 == izmr2) || (izmr1 == izmr3) || (izmr2 == izmr3)) {
				MessageBox(hWnd, L"Один ротор использован два раза!", L"Ошибка", NULL);
			}
			else if((izmr1 != izmr2) && (izmr1 != izmr3) && (izmr2 != izmr3)){
				EndDialog(hWnd, 0);
			}

		}

		if (LOWORD(wParam) == IDCANCEL) { izmr1 = 0;
		EndDialog(hWnd, 0);
		}
			break;
	}
	
	case WM_CLOSE: {
		EndDialog(hWnd, 0);
		break;
	}

	}
	return 0;
}

BOOL CALLBACK KOM(HWND hWnd, UINT Message, UINT wParam, LONG lParam) {
	wchar_t txt[2] = L" ";
	txt[1] = '\0';

	switch (Message) {
		
	case WM_INITDIALOG: {
		sozdcombobox(hWnd, IDC_COMBO1);
		sozdcombobox(hWnd, IDC_COMBO2);
		sozdcombobox(hWnd, IDC_COMBO3);
		sozdcombobox(hWnd, IDC_COMBO4);
		sozdcombobox(hWnd, IDC_COMBO5);
		sozdcombobox(hWnd, IDC_COMBO6);
		sozdcombobox(hWnd, IDC_COMBO7);
		sozdcombobox(hWnd, IDC_COMBO8);
		sozdcombobox(hWnd, IDC_COMBO9);
		sozdcombobox(hWnd, IDC_COMBO10);
		sozdcombobox(hWnd, IDC_COMBO11);
		sozdcombobox(hWnd, IDC_COMBO12);
		sozdcombobox(hWnd, IDC_COMBO13);
		sozdcombobox(hWnd, IDC_COMBO14);
		sozdcombobox(hWnd, IDC_COMBO15);
		sozdcombobox(hWnd, IDC_COMBO16);
		sozdcombobox(hWnd, IDC_COMBO17);
		sozdcombobox(hWnd, IDC_COMBO18);
		sozdcombobox(hWnd, IDC_COMBO19);
		sozdcombobox(hWnd, IDC_COMBO20);
		break; }
	case WM_COMMAND: {
		if (LOWORD(wParam) == IDOK) {
			
			vivodcombobox(hWnd, IDC_COMBO1, 1);
			vivodcombobox(hWnd, IDC_COMBO2, 2);
			vivodcombobox(hWnd, IDC_COMBO3,3);
			vivodcombobox(hWnd, IDC_COMBO4, 4);
			vivodcombobox(hWnd, IDC_COMBO5, 5);
			vivodcombobox(hWnd, IDC_COMBO6, 6);
			vivodcombobox(hWnd, IDC_COMBO7, 7);
			vivodcombobox(hWnd, IDC_COMBO8, 8);
			vivodcombobox(hWnd, IDC_COMBO9, 9);
			vivodcombobox(hWnd, IDC_COMBO10, 10);
			vivodcombobox(hWnd, IDC_COMBO11, 11);
			vivodcombobox(hWnd, IDC_COMBO12, 12);
			vivodcombobox(hWnd, IDC_COMBO13, 13);
			vivodcombobox(hWnd, IDC_COMBO14, 14);
			vivodcombobox(hWnd, IDC_COMBO15, 15);
			vivodcombobox(hWnd, IDC_COMBO16, 16);
			vivodcombobox(hWnd, IDC_COMBO17, 17);
			vivodcombobox(hWnd, IDC_COMBO18, 18);
			vivodcombobox(hWnd, IDC_COMBO19, 19);
			vivodcombobox(hWnd, IDC_COMBO20, 20);
			
			if ((komPanel[1] == komPanel[2]) || (komPanel[1] == komPanel[3]) || (komPanel[1] == komPanel[4]) || (komPanel[1] == komPanel[5]) || (komPanel[1] == komPanel[6]) || (komPanel[1] == komPanel[7]) || (komPanel[1] == komPanel[8]) || (komPanel[1] == komPanel[9]) || (komPanel[1] == komPanel[10]) || (komPanel[1] == komPanel[11]) || (komPanel[1] == komPanel[12]) || (komPanel[1] == komPanel[13]) || (komPanel[1] == komPanel[14]) || (komPanel[1] == komPanel[15]) || (komPanel[1] == komPanel[16]) || (komPanel[1] == komPanel[17]) || (komPanel[1] == komPanel[18]) || (komPanel[1] == komPanel[19]) || (komPanel[1] == komPanel[20])) {
				MessageBox(hWnd, L"Одинa буква использованa два или более раз!", L"Ошибка", NULL); 
			}
			else  {
				if ((komPanel[2] == komPanel[3]) || (komPanel[2] == komPanel[4]) || (komPanel[2] == komPanel[5]) || (komPanel[2] == komPanel[6]) || (komPanel[2] == komPanel[7]) || (komPanel[2] == komPanel[8]) || (komPanel[2] == komPanel[9]) || (komPanel[2] == komPanel[10]) || (komPanel[2] == komPanel[11]) || (komPanel[2] == komPanel[12]) || (komPanel[2] == komPanel[13]) || (komPanel[2] == komPanel[14]) || (komPanel[2] == komPanel[15]) || (komPanel[2] == komPanel[16]) || (komPanel[2] == komPanel[17]) || (komPanel[2] == komPanel[18]) || (komPanel[2] == komPanel[19]) || (komPanel[2] == komPanel[20])) {
					MessageBox(hWnd, L"Одинa буква использованa два или более раз!", L"Ошибка", NULL); break;
				}
				else {
					if ( (komPanel[3] == komPanel[4]) || (komPanel[3] == komPanel[5]) || (komPanel[3] == komPanel[6]) || (komPanel[3] == komPanel[7]) || (komPanel[3] == komPanel[8]) || (komPanel[3] == komPanel[9]) || (komPanel[3] == komPanel[10]) || (komPanel[3] == komPanel[11]) || (komPanel[3] == komPanel[12]) || (komPanel[3] == komPanel[13]) || (komPanel[3] == komPanel[14]) || (komPanel[3] == komPanel[15]) || (komPanel[3] == komPanel[16]) || (komPanel[3] == komPanel[17]) || (komPanel[3] == komPanel[18]) || (komPanel[3] == komPanel[19]) || (komPanel[3] == komPanel[20])) {
						MessageBox(hWnd, L"Одинa буква использованa два или более раз!", L"Ошибка", NULL); break;
					}
					else {
						if ( (komPanel[4] == komPanel[5]) || (komPanel[4] == komPanel[6]) || (komPanel[4] == komPanel[7]) || (komPanel[4] == komPanel[8]) || (komPanel[4] == komPanel[9]) || (komPanel[4] == komPanel[10]) || (komPanel[4] == komPanel[11]) || (komPanel[4] == komPanel[12]) || (komPanel[4] == komPanel[13]) || (komPanel[4] == komPanel[14]) || (komPanel[4] == komPanel[15]) || (komPanel[4] == komPanel[16]) || (komPanel[4] == komPanel[17]) || (komPanel[4] == komPanel[18]) || (komPanel[4] == komPanel[19]) || (komPanel[4] == komPanel[20])) {
							MessageBox(hWnd, L"Одинa буква использованa два или более раз!", L"Ошибка", NULL); break;
						}
						else {
							if ((komPanel[5] == komPanel[6]) || (komPanel[5] == komPanel[7]) || (komPanel[5] == komPanel[8]) || (komPanel[5] == komPanel[9]) || (komPanel[5] == komPanel[10]) || (komPanel[5] == komPanel[11]) || (komPanel[5] == komPanel[12]) || (komPanel[5] == komPanel[13]) || (komPanel[5] == komPanel[14]) || (komPanel[5] == komPanel[15]) || (komPanel[5] == komPanel[16]) || (komPanel[5] == komPanel[17]) || (komPanel[5] == komPanel[18]) || (komPanel[5] == komPanel[19]) || (komPanel[5] == komPanel[20])) {
								MessageBox(hWnd, L"Одинa буква использованa два или более раз!", L"Ошибка", NULL); break;
							}
							else {
								if ((komPanel[6] == komPanel[7]) || (komPanel[6] == komPanel[8]) || (komPanel[6] == komPanel[9]) || (komPanel[6] == komPanel[10]) || (komPanel[6] == komPanel[11]) || (komPanel[6] == komPanel[12]) || (komPanel[6] == komPanel[13]) || (komPanel[6] == komPanel[14]) || (komPanel[6] == komPanel[15]) || (komPanel[6] == komPanel[16]) || (komPanel[5] == komPanel[17]) || (komPanel[6] == komPanel[18]) || (komPanel[6] == komPanel[19]) || (komPanel[6] == komPanel[20])) {
									MessageBox(hWnd, L"Одинa буква использованa два или более раз!", L"Ошибка", NULL); break;
								}
								else {
									if ((komPanel[7] == komPanel[8]) || (komPanel[7] == komPanel[9]) || (komPanel[7] == komPanel[10]) || (komPanel[7] == komPanel[11]) || (komPanel[7] == komPanel[12]) || (komPanel[7] == komPanel[13]) || (komPanel[7] == komPanel[14]) || (komPanel[7] == komPanel[15]) || (komPanel[7] == komPanel[16]) || (komPanel[7] == komPanel[17]) || (komPanel[7] == komPanel[18]) || (komPanel[7] == komPanel[19]) || (komPanel[7] == komPanel[20])) {
										MessageBox(hWnd, L"Одинa буква использованa два или более раз!", L"Ошибка", NULL); break;
									}
									else {
										if ((komPanel[8] == komPanel[9]) || (komPanel[8] == komPanel[10]) || (komPanel[8] == komPanel[11]) || (komPanel[8] == komPanel[12]) || (komPanel[8] == komPanel[13]) || (komPanel[8] == komPanel[14]) || (komPanel[8] == komPanel[15]) || (komPanel[8] == komPanel[16]) || (komPanel[8] == komPanel[17]) || (komPanel[8] == komPanel[18]) || (komPanel[8] == komPanel[19]) || (komPanel[8] == komPanel[20])) {
											MessageBox(hWnd, L"Одинa буква использованa два или более раз!", L"Ошибка", NULL); break;
										}
										else {
											if ((komPanel[9] == komPanel[10]) || (komPanel[9] == komPanel[11]) || (komPanel[9] == komPanel[12]) || (komPanel[9] == komPanel[13]) || (komPanel[9] == komPanel[14]) || (komPanel[9] == komPanel[15]) || (komPanel[9] == komPanel[16]) || (komPanel[9] == komPanel[17]) || (komPanel[9] == komPanel[18]) || (komPanel[9] == komPanel[19]) || (komPanel[9] == komPanel[20])) {
												MessageBox(hWnd, L"Одинa буква использованa два или более раз!", L"Ошибка", NULL); break;
											}
											else {
												if ((komPanel[10] == komPanel[11]) || (komPanel[10] == komPanel[12]) || (komPanel[10] == komPanel[13]) || (komPanel[10] == komPanel[14]) || (komPanel[10] == komPanel[15]) || (komPanel[10] == komPanel[16]) || (komPanel[10] == komPanel[17]) || (komPanel[10] == komPanel[18]) || (komPanel[10] == komPanel[19]) || (komPanel[10] == komPanel[20])) {
													MessageBox(hWnd, L"Одинa буква использованa два или более раз!", L"Ошибка", NULL); break;
												}
												else {
													if ( (komPanel[11] == komPanel[12]) || (komPanel[11] == komPanel[13]) || (komPanel[11] == komPanel[14]) || (komPanel[11] == komPanel[15]) || (komPanel[11] == komPanel[16]) || (komPanel[11] == komPanel[17]) || (komPanel[11] == komPanel[18]) || (komPanel[11] == komPanel[19]) || (komPanel[11] == komPanel[20])) {
														MessageBox(hWnd, L"Одинa буква использованa два или более раз!", L"Ошибка", NULL); break;
													}
													else {
														if ((komPanel[12] == komPanel[13]) || (komPanel[12] == komPanel[14]) || (komPanel[12] == komPanel[15]) || (komPanel[12] == komPanel[16]) || (komPanel[12] == komPanel[17]) || (komPanel[12] == komPanel[18]) || (komPanel[12] == komPanel[19]) || (komPanel[12] == komPanel[20])) {
															MessageBox(hWnd, L"Одинa буква использованa два или более раз!", L"Ошибка", NULL); break;
														}
														else {
															if ((komPanel[13] == komPanel[14]) || (komPanel[13] == komPanel[15]) || (komPanel[13] == komPanel[16]) || (komPanel[13] == komPanel[17]) || (komPanel[13] == komPanel[18]) || (komPanel[13] == komPanel[19]) || (komPanel[13] == komPanel[20])) {
																MessageBox(hWnd, L"Одинa буква использованa два или более раз!", L"Ошибка", NULL); break;
															}
															else {
																if ((komPanel[14] == komPanel[15]) || (komPanel[14] == komPanel[16]) || (komPanel[14] == komPanel[17]) || (komPanel[14] == komPanel[18]) || (komPanel[14] == komPanel[19]) || (komPanel[14] == komPanel[20])) {
																	MessageBox(hWnd, L"Одинa буква использованa два или более раз!", L"Ошибка", NULL); break;
																}
																else {
																	if ((komPanel[15] == komPanel[16]) || (komPanel[15] == komPanel[17]) || (komPanel[15] == komPanel[18]) || (komPanel[15] == komPanel[19]) || (komPanel[15] == komPanel[20])) {
																		MessageBox(hWnd, L"Одинa буква использованa два или более раз!", L"Ошибка", NULL); break;
																	}
																	else {
																		if ((komPanel[16] == komPanel[17]) || (komPanel[16] == komPanel[18]) || (komPanel[16] == komPanel[19]) || (komPanel[16] == komPanel[20])) {
																			MessageBox(hWnd, L"Одинa буква использованa два или более раз!", L"Ошибка", NULL); break;
																		}
																		else {
																			if ((komPanel[17] == komPanel[18]) || (komPanel[17] == komPanel[19]) || (komPanel[17] == komPanel[20])) {
																				MessageBox(hWnd, L"Одинa буква использованa два или более раз!", L"Ошибка", NULL); break;
																			}
																			else {
																				if ((komPanel[18] == komPanel[19]) || (komPanel[18] == komPanel[20])) {
																					MessageBox(hWnd, L"Одинa буква использованa два или более раз!", L"Ошибка", NULL); break;
																				}
																				else {
																					if ((komPanel[19] == komPanel[20])) {
																						MessageBox(hWnd, L"Одинa буква использованa два или более раз!", L"Ошибка", NULL); break;

																					}
																					else { EndDialog(hWnd, 0); }
																				}
																			}
																		}
																	}
																}
															}
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
					}
				}
			}
			if (LOWORD(wParam) == IDCANCEL) {
				
				EndDialog(hWnd, 0);
			}
		break;
		}
		
	case WM_CLOSE: {
		EndDialog(hWnd, 0);
		break;
	}

	}
	return 0;

}

BOOL CALLBACK INSTR(HWND hWnd, UINT Message, UINT wParam, LONG lParam) {
	switch (Message) {
	case WM_COMMAND: {
		if (LOWORD(wParam) == IDOK) { EndDialog(hWnd, 0); }
		break; }

	case WM_CLOSE: {
		EndDialog(hWnd, 0);
		break;
	}
	}

	return 0;
}
