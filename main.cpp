
#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <map>
#include <algorithm>
#include <numeric>

using namespace std;

// ���������� ����������
//------------------------
ofstream VseSdelki;	// ��� ������ ���� ������
ofstream PS; // ��� ������ ������ �� ���������
string AdresLenta; // ����� �����
float DopProsadka = 0; // ���������� ���������� ��������, %
float DopUbitSdel = 0; // ���������� ������������� ������ �� ������, %
float GarantObesp = 0; // ����������� ����������� �� ��������, ���
float ShagCeni = 0; // ��� ����
float StoimShagCeni = 0; // ��������� ���� ����
int KolMesyacev = 0; // ���������� ������� ��� ����������� ������������� ������
int KolDnei = 0; // ���������� ���� (�������������� � �-�� ContTochki)
char Strategi = '0'; // �������� ���������� ��������� ��� ���, Y/N
// ���� ��������� ���������� ���������
int Polz_Sniat = 0; // ���������� ������
int Polz_otkr = 0; // ���������� ��������
int Polz_ubit = 0; // ���������� �������� � �������
int Polz_kolVol = 0; // ���������� �������
//------------------------

class Svecha	// ����� ��� ������ �����
{
public:
	char Period; // ������ �����
	string Tiker; // �����
	int Date = 0; // ����
	short int Time = 0; // �����
	float Open = 0.0f; // ��������
	float High = 0.0f; // ��������
	float Low = 0.0f; // �������
	float Close = 0.0f; // ��������
	int Vol = 0; // �����
	unsigned int God = 0; // ���
	unsigned int Mes = 0; // �����
	unsigned int Den = 0;			// ����
	unsigned int Chas = 0;			// ���
	unsigned int Min = 0;	// ������
	float KoefPer = 0.0f;	// ����������� ��������� ��� ����� ������
	list<Svecha> next;
	void ZapPokaz(char period)	// ����������� ����������� � ����������� �� ������� �����
	{
		if (period == '�')	// ���
		{
			this->Tiker = this->next.back().Tiker; // 
			this->Chas = this->next.back().Chas; // 
			this->Den = this->next.back().Den; // 
			this->Mes = this->next.back().Mes; // 
		}
		else if (period == '�')	// ����
		{
			this->Tiker = this->next.back().Tiker; // 
			this->Den = this->next.back().Den; // 
			this->Mes = this->next.back().Mes; // 
		}
		else if (period == '�')	// �����
		{
			this->Mes = this->next.back().Mes; // 
		}
		this->Period = period; // ���
		this->God = this->next.back().God; // 

		this->Open = this->next.front().Open; // ��������
		this->Close = this->next.back().Close; // ��������
		this->High = (*max_element(this->next.begin(), this->next.end(), [](Svecha& el1, Svecha& el2)
			{
				return el1.High < el2.High;
			})).High; // ��������
		this->Low = (*max_element(this->next.begin(), this->next.end(), [](Svecha& el1, Svecha& el2)
			{
				return el1.Low > el2.Low;
			})).Low; // �������
		this->Vol = accumulate(this->next.begin(), this->next.end(), 0, [](int el1, Svecha& el2)
			{
				return el1 + el2.Vol;
			}); // �����
	}
};
class Svecha_30	// ����� 30 ����� ��� ����������� ����� �����
{
public:
	string Tiker;
	int Date;
	int TimeN; // ����� �������� �����
	int TimeK; // ����� �������� �����
	float Open; // 
	float Close; // 
	int Vol; // 
	char Oper; // �������� ����� �����
	short int Mes; // �����
	float izm_5; // ��������� ������ �����������
	float KoefPer = 0.0f; // ����������� ���������
};
class Tochka // ����� �����
{
public:
	string Tiker = "0"; // �����
	int Date = 0; // ����
	int TimeN = 0; // ����� �������� �����
	int TimeK = 0; // ����� �������� �����
	float Open = 0.0f; // ��������
	float Close = 0.0f; // ��������
	float KoefPerS = 0.0f; // ����������� ��������� ��� ����� ������
	char Oper; // �������� �����
	int Vol = 0; // �����
	float izm_5 = 0; // ��������� ������ �����������
	float PokKill = 0.0f; // ���������� ������ ������
	float PokOpen = 0.0f; // ���������� ��������
	float PokClose = 0.0f; // ���������� �������� � �������
	string TikerOpen = "0"; // ����� ��������
	int DateOpen = 0; // ���� ��������
	int TimeOpen = 0; // ����� ��������
	float PriceOpen = 0.0f; // ���� ��������
	float KoefPerOpen = 0.0f; // ����������� ��������� ���� ��������
	string TikerKill = "0"; // ����� ������
	int DateKill = 0; // ���� ������
	int TimeKill = 0; // ����� ������
	float PriceKill = 0.0f; // ���� ������
	float KoefPerKill = 0.0f; // ����������� ��������� ���� ������
	string TikerCloseU = "0"; // ����� �������� � �������
	int DateCloseU = 0; // ���� �������� � �������
	int TimeCloseU = 0; // ����� �������� � �������
	float PriceCloseU = 0.0f; // ���� �������� � �������
	float KoefPerCloseU = 0.0f; // ����������� ��������� ���� �������� � �������
	string TikerCloseP = "0"; // ����� �������� � ��������
	int DateCloseP = 0; // ���� �������� � ��������
	int TimeCloseP = 0; // ����� �������� � ��������
	float PriceCloseP = 0.0f; // ���� �������� � ��������
	float KoefPerCloseP = 0.0f; // ����������� ��������� ���� �������� � ��������
	char Sostoyanie; // ��������� (�����/�������)
	float Rezult = 0.0f; // ��������� (�������/������)
	float PoprPriceOpen = 0.0f; // �������� ���� �������� �� ������� ����� ����� ������ � ����/��� ���� ������ (������������� ������ ���� ������ �� ������)
	float PoprPriceCloseU = 0.0f; // �������� ���� �������� � ������� �� ������� ����� ����� ������ � ����/��� ���� ������ (������������� ������ ���� ������ �� ������)
	float PoprPriceCloseP = 0.0f; // �������� ���� �������� � �������� �� ������� ����� ����� ������ � ����/��� ���� ������ (������������� ������ ���� ������ �� ������)
	string Koment; // ����������

	Tochka()
	{
	}
	Tochka(Svecha_30& s) // ������ ����������� ����� �� ����� 30 ���.
	{
		this->Tiker = s.Tiker;
		this->Date = s.Date;
		this->TimeN = s.TimeN;
		this->TimeK = s.TimeK;
		this->Open = s.Open;
		this->Close = s.Close;
		this->Vol = s.Vol;
		this->Oper = s.Oper;
		this->izm_5 = s.izm_5;
		this->KoefPerS = s.KoefPer;
	}
	unsigned int GetVremia(char period, char deistvie) // ���������� ����� � ����������� �� �������
	{						// deistvie - ����� �����, periol - ������		
		switch (deistvie)
		{
		case 'S': // ����� �����
			if (period == '�') // ������ ������� �� ������
			{
				return this->Date * 10000 + this->TimeK;
			}
			if (period == '�') // �� ����
			{
				return this->Date * 100 + this->TimeK / 100;
			}
			if (period == '�') // �� ���
			{
				return this->Date;
			}
			if (period == '�') // �� ������
			{
				return this->Date / 100;
			}
			if (period == '�') // �� ����
			{
				return this->Date / 10000;
			}
			return -1;
		case 'O': // ����� ��������
			if (period == '�')
			{
				return this->DateOpen * 10000 + this->TimeOpen;
			}
			if (period == '�')
			{
				return this->DateOpen * 100 + this->TimeOpen / 100;
			}
			if (period == '�')
			{
				return this->DateOpen;
			}
			if (period == '�')
			{
				return this->DateOpen / 100;
			}
			if (period == '�')
			{
				return this->DateOpen / 10000;
			}
			return -1;
		case 'U': // ����� �������� � �������
			if (period == '�')
			{
				return this->DateCloseU * 10000 + this->TimeCloseU;
			}
			return -1;
		}
		return -1;
	}
	void OprSostoyanie()	// ���������� ��������� ����� �����, ��������� ��� �����
	{			// �� ���� ������ � �������� ������
		if (this->DateOpen)
		{
			if (this->DateKill)
			{
				if ((unsigned int)(this->DateKill) * 10000 + this->TimeKill < (unsigned int)(this->DateOpen) * 10000 + this->TimeOpen)
				{
					this->Sostoyanie = 'K'; // 
				}
				else
				{
					this->Sostoyanie = 'I'; // 
				}
			}
			else
			{
				this->Sostoyanie = 'I'; // 
			}
		}
		else
		{
			this->Sostoyanie = 'K'; // 
		}
	}
	void CalcRezult()	// ������� ��������� ������
	{			// � ����������� �� ����������� � ������ �������� �� ������ ���� ������
		switch (Oper)
		{
		case 'B':
			if (DateCloseP)
			{
				Rezult = (PriceCloseP - PriceOpen + PoprPriceOpen + PoprPriceCloseP) * StoimShagCeni / ShagCeni; // 
			}
			else
			{
				Rezult = (PriceCloseU - PriceOpen + PoprPriceOpen + PoprPriceCloseU) * StoimShagCeni / ShagCeni; // 
			}
			break; // 
		case 'S':
			if (DateCloseP)
			{
				Rezult = (PriceOpen - PriceCloseP + PoprPriceOpen + PoprPriceCloseP) * StoimShagCeni / ShagCeni; // 
			}
			else
			{
				Rezult = (PriceOpen - PriceCloseU + PoprPriceOpen + PoprPriceCloseU) * StoimShagCeni / ShagCeni; // 
			}
			break; // 
		}
	}
};
void Nastroika()	// ��������� ������ ��������� �� �����
{
	ifstream FileNastr("���������.txt"); // ���� ����� ���������
	FileNastr >> AdresLenta; // ����� ������ ������
	FileNastr >> DopProsadka; // ���������� ��������
	FileNastr >> DopUbitSdel; // ���������� ������ �� ������
	FileNastr >> GarantObesp; // ����������� ����������� �� ��������
	FileNastr >> ShagCeni; // ��� ���� ���������
	FileNastr >> StoimShagCeni; // ��������� ���� ����
	FileNastr >> KolMesyacev; // ���������� ������� ��� ����������� ������������� ������
	FileNastr.close();
}
void ContTorgi(list<Svecha>& Torg)	// �������� ���������� ����������� ������
					// �������� ����������� ������ ������������ ���
					// ������ ������, ������ ������� ������ � ����
					// ������� �������� ������ ������ ������� �������
{
	ifstream FList(AdresLenta + "filelist.txt"); // ������� ���� �� ������� �������� ������ ������
	list<string> NameLenta; // ������ �������� ������
	string str;

	while (FList >> str)	NameLenta.push_back(str); // ��������� ������ �������� ����� ������
	FList.close(); // 

	ifstream FLenta; // 
	string Tiker; // 
	float PrCl = 0.0f; // 
	char ch; // 
	int n = 0; // 
	float Koef = 0.0f; // 
	Svecha Sv; // 

	int G = 0, M = 0, D = 0, Ch = 0; // 
	Torg.push_back(Svecha()); // ������ �������� ������
	Torg.back().next.push_back(Svecha()); // ��������
	Torg.back().next.back().next.push_back(Svecha()); // �������
	Torg.back().next.back().next.back().next.push_back(Svecha()); // �������

	for (string Lenta : NameLenta)
	{
		cout << Lenta << endl; // 
		FLenta.open(AdresLenta + Lenta, ios_base::in); // ������� ���� ������
		getline(FLenta, str); // 
		str = ""; // 
		while (FLenta.get(ch))	// ������ ������ ����� ����������� �� ���������� ����� �������
					// � ������������ � ���������� ������ Svecha (��������)
		{
			if (ch != ',' && ch != '\n')
			{
				str += ch; // 
			}
			else
			{
				switch (n)
				{
				case 0:
					Sv.Tiker = str; // �����
					Sv.Period = '�'; // ������ ������
					break; // 
				case 2:
					Sv.Date = stoi(str) % 1000000; // ����
					Sv.God = stoi(str.substr(2, 2)); // ���� ������� �� ���
					Sv.Mes = Sv.God * 100 + stoi(str.substr(4, 2)); // �����
					Sv.Den = Sv.Mes * 100 + stoi(str.substr(6, 2)); // ����
					break; // 
				case 3:
					Sv.Time = stoi(str) / 100; // �����
					Sv.Chas = Sv.Den * 100 + stoi(str)/10000; // ����
					Sv.Min = Sv.Den * 10000 + Sv.Time; // ������
					break; // 
				case 4:
					Sv.Open = stof(str); // ���� ��������
					break; // 
				case 5:
					Sv.High = stof(str); // ������������ ����
					break; // 
				case 6:
					Sv.Low = stof(str); // �����������
					break; // 
				case 7:
					Sv.Close = stof(str); // ���� ��������
					break; // 
				case 8:
					Sv.Vol = stoi(str); // ����� �����
					if (Tiker != "")	// ���� �������� ����� (����� ��������)
					{
						if (Tiker != Sv.Tiker)
						{
							Tiker = Sv.Tiker; // ����� �����
							Koef = PrCl - Sv.Open; // ��������� ����������� ��������� ��� 
						}	// ��� ����� ������ ���� ����������
					}
					else
					{
						Tiker = Sv.Tiker; // 
						G = Sv.God; // 
						M = Sv.Mes; // 
						D = Sv.Den; // 
						Ch = Sv.Chas; // 
					}
					Sv.KoefPer = Koef; // �������� ����������� ��������� ������ ������
					Sv.Open += Koef; // ����������� ���� ��������
					Sv.High += Koef; // ������������
					Sv.Low += Koef; // �����������
					Sv.Close += Koef; // ��������
					PrCl = Sv.Close; // ��������� � ���������� ���� ��������

					// �������� �������� ������ (���, �����, ����, ���, ������)
					if (Sv.Chas != Ch) // ���� �������� ���
					{
						Ch = Sv.Chas; // ��������� ������� ���
						Torg.back().next.back().next.back().next.back().ZapPokaz('�'); // �������� ���������� ���������� ����
						if (Sv.Den != D)	// �������� ����
						{
							D = Sv.Den; // ��������� ������� ����
							Torg.back().next.back().next.back().ZapPokaz('�'); // �������� ���������� ���������� ���
							if (Sv.Mes != M)	// �������� �����
							{
								M = Sv.Mes; // ��������� ������� �����
								Torg.back().next.back().ZapPokaz('�'); // �������� ���������� ���������� ������
								if (Sv.God != G)	// �������� ���
								{
									G = Sv.God; // ��������� ������� ���
									Torg.back().ZapPokaz('�'); // �������� ���������� ���������� ����
									Torg.push_back(Svecha()); // �������� ����� ������� �����
								}
								Torg.back().next.push_back(Svecha()); // �������� ����� �������� �����
							}
							Torg.back().next.back().next.push_back(Svecha()); // �������� ����� ������� �����
						}
						Torg.back().next.back().next.back().next.push_back(Svecha()); // �������� ����� ������� �����
					}
					Torg.back().next.back().next.back().next.back().next.push_back(Sv); // �������� ����� �������� �����
					n = -1; // 
				}
				str = ""; // 
				n++; // 
			}
		}
		FLenta.close(); // 
	}
	// ����� ��������� ���� ������ ������� ��������� ������
	Torg.back().next.back().next.back().next.back().ZapPokaz('�'); // �������
	Torg.back().next.back().next.back().ZapPokaz('�'); // �������
	Torg.back().next.back().ZapPokaz('�'); // ��������
	Torg.back().ZapPokaz('�'); // �������
}
void ContSv_30(list<Svecha>& TorgList, list<Svecha_30>& svList)	// ��������� 30-�� �������� ������, ��������� �� ���������� ������
{
	int i; // 
	Svecha_30 sv; // 
	bool Sv_5 = true; // 
	int Time_5; // 
	// ���������� ����� �� ����������� ������ ������
	sv.Tiker = TorgList.front().next.front().next.front().next.front().next.front().Tiker; // �����
	sv.Date = TorgList.front().next.front().next.front().next.front().next.front().Date; // ����
	sv.TimeN = TorgList.front().next.front().next.front().next.front().next.front().Time; // ����� ������
	sv.Open = TorgList.front().next.front().next.front().next.front().next.front().Open; // ���� ��������
	Time_5 = TorgList.front().next.front().next.front().next.front().next.front().Time; // ����� ������ ������ �����������
	sv.Vol = 0; // ����� �������
	for (Svecha God : TorgList)	// ���������� �������� ������
	{
		for (Svecha Mes : God.next)	// ���������� �������� ������
		{
			for (Svecha Den : Mes.next)	// ���������� ������� ������
			{
				for (Svecha Chas : Den.next)	// ���������� ������� ������
				{
					for (Svecha Minuta : Chas.next)	// ���������� �������� ������
					{
						if (Sv_5)	// ���� ���� ������ �����������
						{
							if (Minuta.Time - Time_5 >= 5)	// ���� ������ ����������� �����������
							{
								Sv_5 = false; // ������������� ������ ����������� false
								sv.izm_5 = sv.Close - sv.Open; // ���������� ���������
								if (sv.Close - sv.Open >= 0)	// ���� �������������
								{
									sv.Oper = 'S'; // �������� �������
								}
								else
								{
									sv.Oper = 'B'; // �������� �������
								}
							}
						}
						if (sv.Date != Minuta.Date || Minuta.Time - sv.TimeN >= 30)	// ���� 30-���. ����� ���������
						{
							Time_5 = Minuta.Time; // �������� ������� ������
							Sv_5 = true; // ������ ����������� � true
							i = Time_5 % 10; // ��������� ����� ������ ����������� �� ������������
							if (!(i == 0 || i == 5))
								if (i < 5)		Time_5 = Time_5 - i;	// �������������� ����� �� 0 �� �����
								else			Time_5 = Time_5 - i + 5;	// �������������� ����� �� 5 �� �����

							svList.push_back(sv); // �������� 30-���. �����
							sv.Tiker = Minuta.Tiker; // �����
							sv.Date = Minuta.Date; // ����
							sv.TimeN = Minuta.Time; // ����� ������
							sv.Open = Minuta.Open; // ��������
							sv.Mes = Minuta.Mes; // �����
							sv.KoefPer = Minuta.KoefPer; // ����. ���������
							sv.Vol = 0; // �����

							i = sv.TimeN % 100; //  ��������� ����� ������ ����� �� ������������
							if (!(i == 0 || i == 30))
								if (i < 30)		sv.TimeN = sv.TimeN - i;	// �������������� ����� �� 0 �� �����
								else			sv.TimeN = sv.TimeN - i + 30;	// �������������� ����� �� 30 �� �����
						}
						// ���� ����� �� ���������
						sv.TimeK = Minuta.Time; // ��������� ����� ���������
						sv.Close = Minuta.Close; // ���� ��������
						sv.Vol += Minuta.Vol; // ����������� �����
					}
				}
			}
		}
	}
}
void SdvigIter(list<Svecha_30>::iterator& it) // ����� ��������� �� ����� ������
{
	int m = (*it).Date / 100; // ����� �����
	it++; // �������� �������� �� ��������� ������
	while ((*it).Date / 100 == m)	// ���������� �� ���������� ������
		it++;
}
void ContTochki(list<Svecha>& Torg, map<const int, list<Tochka>>& tochList)	// �������� ���������� ����� �����
{	//����� ������������ �� ������ 30-�������� ����� (���� ����� ���������� �������)
	list<Svecha_30> svList; // 
	ContSv_30(Torg, svList); // ������� ��������� 30-�������� ������

	int mes = svList.front().Mes; // ����� ����� ������ 30-�������� �����
	int SchMes = 0; // ������� �������
	int Date = 0; // ����
	map<int, int> Volume; // ��������� ������� (���������� �� ���, �����)
	list<Tochka> TochkaList; // 
	list<Svecha_30>::iterator IterStart = svList.begin(); // �������� �� ������ ��������� ������� ������
	list<Svecha_30>::iterator IterFinish = svList.begin(); // �������� �� ����� ��������� ������� ������
	list<Svecha_30>::iterator IterSv_12m; // 
	list<Svecha_30> Sv_12m; // 

	for (Svecha_30 sv : svList)	// ���������� ������ 30-���. ������
	{
		if (sv.Mes != mes)	// ���� �������� �����
		{
			SchMes++; // ��������� ���������� �������
			mes = sv.Mes; // ��������� �����
			if (SchMes > KolMesyacev)	// ���� ���������� ������ ������� ��� KolMesyacev (���������� ��� ���. ������������ ������ ��������� �������)
			{
				copy(IterStart, IterFinish, back_inserter(Sv_12m)); // ����������� 30-���. ������ �� ��������������� ���
				Sv_12m.sort([](Svecha_30& s1, Svecha_30& s2) {return s1.Vol > s2.Vol; }); // ������������� �� ������
				Sv_12m.unique([](Svecha_30& s1, Svecha_30& s2) { return s1.Vol == s2.Vol; }); // ������� ������������� �����
				Volume.clear(); // �������� ���������� ��������� �������

				for (int el : { 1, 2, 3, 4, 5, 10, 15, 20 })	// ���������� ������������ ������� �� ������ KolMesyacev
				{
					IterSv_12m = Sv_12m.begin(); // ���������� �������� �� ������ ������ 30-���. ������
					advance(IterSv_12m, el - 1); //  // ����� ��������� �� ������������ ���������� ������� (el - 1)
					Volume.emplace(el, (*IterSv_12m).Vol); // �������� ����������� ����� � ���������� Volume, ����� - ���������� �������
				}
				Sv_12m.clear(); // �������� ������ 30-���. ������
				SdvigIter(IterStart); // �������� �������� ������ �� ���� ����� ������
			}
		}
		if (!Volume.empty())
		{
			for (auto KolVol : Volume)	// ���������� ������ ���������� �������
			{
				if (sv.Vol >= KolVol.second)	// ���� ����� 30-���. ������ ������ ���������� ������������
				{
					(*tochList.emplace(KolVol.first, TochkaList).first).second.push_back(sv); // �������� 30-���. ������ � ������ ����� ����� � ��������������� ���������� �������
				}
			}
		}
		if (Date != sv.Date && SchMes > KolMesyacev)
		{
			KolDnei++; // ���������� ��� ���� ��������
			Date = sv.Date; // 
		}
		IterFinish++; // �������� �������� ����� ��������� �������
	}
}
void Sort_Po_Otkr(pair<int, list<Tochka>>& Para)	// ���������� ������ �� ���� ��������
{
	Para.second.sort([](const Tochka& T1, const Tochka& T2)
		{
			return ((unsigned int)(T1.DateOpen) * 10000 + T1.TimeOpen < (unsigned int)(T2.DateOpen) * 10000 + T2.TimeOpen); // 
		}); // 
}
void Print(pair<int, list<Tochka>>& SpisokSdelok) // ���������� � ���� ��� ������
{
	if (!VseSdelki.is_open())
	{
		string FileName; // 
		FileName = "������ (O-"; // 
		FileName += to_string(Polz_otkr); // 
		FileName += ",C-"; // 
		FileName += to_string(Polz_ubit); // 
		FileName += ",K-"; // 
		FileName += to_string(Polz_Sniat); // 
		FileName += ",V-"; // 
		FileName += to_string(Polz_kolVol); // 
		FileName += ").txt"; // 
		VseSdelki.open(FileName); // 
		VseSdelki << "Para.first" << '\t' << "Tiker" << '\t' << "Date" << '\t' << "TimeN" << '\t' << "TimeK" << '\t' << "Open" << '\t' << "Close" << '\t' << "Oper" << '\t'; // 
		VseSdelki << "Vol" << '\t' << "izm_5" << '\t' << "PokKill" << '\t' << "PokOpen" << '\t' << "PokClose" << '\t' << "TikerOpen" << '\t'; // 
		VseSdelki << "DateOpen" << '\t' << "TimeOpen" << '\t' << "PriceOpen" << '\t' << "TikerKill" << '\t' << "DateKill" << '\t' << "TimeKill" << '\t'; // 
		VseSdelki << "PriceKill" << '\t' << "TikerCloseU" << '\t' << "DateCloseU" << '\t' << "TimeCloseU" << '\t' << "PriceCloseU" << '\t' << "Sostoyanie" << '\t'; // 
		VseSdelki << "TikerCloseP" << '\t' << "DateCloseP" << '\t' << "TimeCloseP" << '\t' << "PriceCloseP" << '\t' << "Rezult" << '\t' << "Koment" << endl; // 
	}
	for (Tochka& Sdelka : SpisokSdelok.second)
	{
		Sdelka.Open -= Sdelka.KoefPerS; // 
		Sdelka.Close -= Sdelka.KoefPerS; // 
		Sdelka.PriceOpen -= Sdelka.KoefPerOpen; // 
		Sdelka.PriceKill -= Sdelka.KoefPerKill; // 
		Sdelka.PriceCloseU -= Sdelka.KoefPerCloseU; // 
		Sdelka.PriceCloseP -= Sdelka.KoefPerCloseP; // 

		VseSdelki << SpisokSdelok.first << '\t' << Sdelka.Tiker << '\t' << Sdelka.Date << '\t' << Sdelka.TimeN << '\t' << Sdelka.TimeK << '\t' << Sdelka.Open << '\t' << Sdelka.Close << '\t' << Sdelka.Oper << '\t'; // 
		VseSdelki << Sdelka.Vol << '\t' << Sdelka.izm_5 << '\t' << Sdelka.PokKill << '\t' << Sdelka.PokOpen << '\t' << Sdelka.PokClose << '\t' << Sdelka.TikerOpen << '\t'; // 
		VseSdelki << Sdelka.DateOpen << '\t' << Sdelka.TimeOpen << '\t' << Sdelka.PriceOpen << '\t' << Sdelka.TikerKill << '\t' << Sdelka.DateKill << '\t' << Sdelka.TimeKill << '\t'; // 
		VseSdelki << Sdelka.PriceKill << '\t' << Sdelka.TikerCloseU << '\t' << Sdelka.DateCloseU << '\t' << Sdelka.TimeCloseU << '\t' << Sdelka.PriceCloseU << '\t' << Sdelka.Sostoyanie << '\t'; // 
		VseSdelki << Sdelka.TikerCloseP << '\t' << Sdelka.DateCloseP << '\t' << Sdelka.TimeCloseP << '\t' << Sdelka.PriceCloseP << '\t' << Sdelka.Rezult << '\t' << Sdelka.Koment << endl; // 
	}
}
int FindOpen(list<Svecha>& TorgilList, Tochka& Tochka) // ����� �������� ������
{
	if (Tochka.Oper == 'B')	// ���� �������� �������
	{
		for (Svecha& God : TorgilList) // ������������ �������� ������ ������
		{
			if (God.God >= Tochka.GetVremia('�', 'S') && (abs(God.High - Tochka.PriceOpen) < 0.000001 || God.High > Tochka.PriceOpen))	// ��������� ����� � ������������ ���� ������
			{
				for (Svecha& Mes : God.next)	// �������� ������
				{
					if (Mes.Mes >= Tochka.GetVremia('�', 'S') && (abs(Mes.High - Tochka.PriceOpen) < 0.000001 || Mes.High > Tochka.PriceOpen))	// ��������� ����� � ������������ ���� ������
					{
						for (Svecha& Den : Mes.next)	// �������
						{
							if (Den.Den >= Tochka.GetVremia('�', 'S') && (abs(Den.High - Tochka.PriceOpen) < 0.000001 || Den.High > Tochka.PriceOpen))	// ��������� ����� � ������������ ���� ������
							{
								for (Svecha& Chas : Den.next)	// �������
								{
									if (Chas.Chas >= Tochka.GetVremia('�', 'S') && (abs(Chas.High - Tochka.PriceOpen) < 0.000001 || Chas.High > Tochka.PriceOpen))	// ��������� ����� � ������������ ���� ������
									{
										for (Svecha& Minut : Chas.next)	// ��������
										{
											if (Minut.Min > Tochka.GetVremia('�', 'S') && (abs(Minut.High - Tochka.PriceOpen) < 0.000001 || Minut.High > Tochka.PriceOpen))	// ��������� ����� � ������������ ���� ������
											{
												Tochka.TikerOpen = Minut.Tiker; // ���� ������� �������� ������ ���� ������� ������
												Tochka.DateOpen = Minut.Date; // ���� ����� ����� � ������������ ���� ������ ���� 
												Tochka.TimeOpen = Minut.Time; // �������� ������, �� �������� �����, ����, �����, �������.
												Tochka.KoefPerOpen = Minut.KoefPer; // ��������� �������� ������
												Tochka.PoprPriceOpen = Tochka.PriceOpen - Minut.High;
												return 0; // 
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
	else if (Tochka.Oper == 'S')	// ���� �������� �������, �� ��������� ����������� ���� �����
	{
		for (Svecha& God : TorgilList)
		{
			if (God.God >= Tochka.GetVremia('�', 'S') && (abs(God.Low - Tochka.PriceOpen) < 0.000001 || God.Low < Tochka.PriceOpen))
			{
				for (Svecha& Mes : God.next)
				{
					if (Mes.Mes >= Tochka.GetVremia('�', 'S') && (abs(Mes.Low - Tochka.PriceOpen) < 0.000001 || Mes.Low < Tochka.PriceOpen))
					{
						for (Svecha& Den : Mes.next)
						{
							if (Den.Den >= Tochka.GetVremia('�', 'S') && (abs(Den.Low - Tochka.PriceOpen) < 0.000001 || Den.Low < Tochka.PriceOpen))
							{
								for (Svecha& Chas : Den.next)
								{
									if (Chas.Chas >= Tochka.GetVremia('�', 'S') && (abs(Chas.Low - Tochka.PriceOpen) < 0.000001 || Chas.Low < Tochka.PriceOpen))
									{
										for (Svecha& Minut : Chas.next)
										{
											if (Minut.Min > Tochka.GetVremia('�', 'S') && (abs(Minut.Low - Tochka.PriceOpen) < 0.000001 || Minut.Low < Tochka.PriceOpen))
											{
												Tochka.TikerOpen = Minut.Tiker; // 
												Tochka.DateOpen = Minut.Date; // 
												Tochka.TimeOpen = Minut.Time; // 
												Tochka.KoefPerOpen = Minut.KoefPer; // 
												Tochka.PoprPriceOpen = Minut.Low - Tochka.PriceOpen;
												return 0; // 
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
	return 0; // 
}
int FindKill(list<Svecha>& TorgilList, Tochka& Tochka) // ����� ������ ������
{
	if (Tochka.Oper == 'B')	// ���� �������� �������
	{
		for (Svecha& God : TorgilList) // ������������ �������� ������ ������
		{
			if (God.God >= Tochka.GetVremia('�', 'S') && (abs(God.Low - Tochka.PriceKill) < 0.000001 || God.Low < Tochka.PriceKill))	// ��������� ����� � ����������� ���� ������
			{
				for (Svecha& Mes : God.next) // ��������
				{
					if (Mes.Mes >= Tochka.GetVremia('�', 'S') && (abs(Mes.Low - Tochka.PriceKill) < 0.000001 || Mes.Low < Tochka.PriceKill))	// ��������� ����� � ����������� ���� ������
					{
						for (Svecha& Den : Mes.next) // �������
						{
							if (Den.Den >= Tochka.GetVremia('�', 'S') && (abs(Den.Low - Tochka.PriceKill) < 0.000001 || Den.Low < Tochka.PriceKill))	// ��������� ����� � ����������� ���� ������
							{
								for (Svecha& Chas : Den.next) // �������
								{
									if (Chas.Chas >= Tochka.GetVremia('�', 'S') && (abs(Chas.Low - Tochka.PriceKill) < 0.000001 || Chas.Low < Tochka.PriceKill))	// ��������� ����� � ����������� ���� ������
									{
										for (Svecha& Minut : Chas.next) // ��������
										{
											if (Minut.Min > Tochka.GetVremia('�', 'S') && (abs(Minut.Low - Tochka.PriceKill) < 0.000001 || Minut.Low < Tochka.PriceKill))	// ��������� ����� � ����������� ���� ������
											{
												Tochka.TikerKill = Minut.Tiker; // ���� ������� �������� ������ ���� ������� ������
												Tochka.DateKill = Minut.Date; // ���� ����� ����� � ����������� ���� ������ ���� 
												Tochka.TimeKill = Minut.Time; // ������ ������, �� �������� �����, ����, �����, �������.
												Tochka.KoefPerKill = Minut.KoefPer; // ��������� ������ ������
												return 0; //
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
	else if (Tochka.Oper == 'S')	// ���� �������� �������, �� ��������� ������������ ���� �����
	{
		for (Svecha& God : TorgilList)
		{
			if (God.God >= Tochka.GetVremia('�', 'S') && (abs(God.High - Tochka.PriceKill) < 0.000001 || God.High > Tochka.PriceKill))
			{
				for (Svecha& Mes : God.next)
				{
					if (Mes.Mes >= Tochka.GetVremia('�', 'S') && (abs(Mes.High - Tochka.PriceKill) < 0.000001 || Mes.High > Tochka.PriceKill))
					{
						for (Svecha& Den : Mes.next)
						{
							if (Den.Den >= Tochka.GetVremia('�', 'S') && (abs(Den.High - Tochka.PriceKill) < 0.000001 || Den.High > Tochka.PriceKill))
							{
								for (Svecha& Chas : Den.next)
								{
									if (Chas.Chas >= Tochka.GetVremia('�', 'S') && (abs(Chas.High - Tochka.PriceKill) < 0.000001 || Chas.High > Tochka.PriceKill))
									{
										for (Svecha& Minut : Chas.next)
										{
											if (Minut.Min > Tochka.GetVremia('�', 'S') && (abs(Minut.High - Tochka.PriceKill) < 0.000001 || Minut.High > Tochka.PriceKill))
											{
												Tochka.TikerKill = Minut.Tiker; // 
												Tochka.DateKill = Minut.Date; // 
												Tochka.TimeKill = Minut.Time; // 
												Tochka.KoefPerKill = Minut.KoefPer; // 
												return 0; // 
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
	return 0; // 
}
int FindClose(list<Svecha>& TorgilList, Tochka& Tochka) // ����� �������� ������ � �������
{
	if (Tochka.Oper == 'B')	// ���� �������� �������
	{
		for (Svecha& God : TorgilList) // ������������ �������� ������ ������
		{
			if (God.God >= Tochka.GetVremia('�', 'O') && (abs(God.Low - Tochka.PriceCloseU) < 0.000001 || God.Low < Tochka.PriceCloseU))	// ��������� ����� � ����������� ���� ������
			{
				for (Svecha& Mes : God.next) // ��������
				{
					if (Mes.Mes >= Tochka.GetVremia('�', 'O') && (abs(Mes.Low - Tochka.PriceCloseU) < 0.000001 || Mes.Low < Tochka.PriceCloseU))	// ��������� ����� � ����������� ���� ������
					{
						for (Svecha& Den : Mes.next) // �������
						{
							if (Den.Den >= Tochka.GetVremia('�', 'O') && (abs(Den.Low - Tochka.PriceCloseU) < 0.000001 || Den.Low < Tochka.PriceCloseU))	// ��������� ����� � ����������� ���� ������
							{
								for (Svecha& Chas : Den.next) // �������
								{
									if (Chas.Chas >= Tochka.GetVremia('�', 'O') && (abs(Chas.Low - Tochka.PriceCloseU) < 0.000001 || Chas.Low < Tochka.PriceCloseU))	// ��������� ����� � ����������� ���� ������
									{
										for (Svecha& Minut : Chas.next) // ��������
										{
											if (Minut.Min > Tochka.GetVremia('�', 'O') && (abs(Minut.Low - Tochka.PriceCloseU) < 0.000001 || Minut.Low < Tochka.PriceCloseU))	// ��������� ����� � ����������� ���� ������
											{
												Tochka.TikerCloseU = Minut.Tiker; // ���� ������� �������� ������ ���� ������� ������
												Tochka.DateCloseU = Minut.Date; // ���� �������� ������ � ����������� ���� ������ ���� 
												Tochka.TimeCloseU = Minut.Time; // �������� ������ � �������, �� �������� �����, ����,
												Tochka.KoefPerCloseU = Minut.KoefPer; //  �����, �������. ��������� �������� ������ � �������
												Tochka.PoprPriceCloseU = Minut.Low - Tochka.PriceCloseU;
												return 0; //
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
	else if (Tochka.Oper == 'S')	// ���� �������� �������, �� ��������� ������������ ���� �����
	{
		for (Svecha& God : TorgilList)
		{
			if (God.God >= Tochka.GetVremia('�', 'O') && (abs(God.High - Tochka.PriceCloseU) < 0.000001 || God.High > Tochka.PriceCloseU))
			{
				for (Svecha& Mes : God.next)
				{
					if (Mes.Mes >= Tochka.GetVremia('�', 'O') && (abs(Mes.High - Tochka.PriceCloseU) < 0.000001 || Mes.High > Tochka.PriceCloseU))
					{
						for (Svecha& Den : Mes.next)
						{
							if (Den.Den >= Tochka.GetVremia('�', 'O') && (abs(Den.High - Tochka.PriceCloseU) < 0.000001 || Den.High > Tochka.PriceCloseU))
							{
								for (Svecha& Chas : Den.next)
								{
									if (Chas.Chas >= Tochka.GetVremia('�', 'O') && (abs(Chas.High - Tochka.PriceCloseU) < 0.000001 || Chas.High > Tochka.PriceCloseU))
									{
										for (Svecha& Minut : Chas.next)
										{
											if (Minut.Min > Tochka.GetVremia('�', 'O') && (abs(Minut.High - Tochka.PriceCloseU) < 0.000001 || Minut.High > Tochka.PriceCloseU))
											{
												Tochka.TikerCloseU = Minut.Tiker; // 
												Tochka.DateCloseU = Minut.Date; // 
												Tochka.TimeCloseU = Minut.Time; // 
												Tochka.KoefPerCloseU = Minut.KoefPer; // 
												Tochka.PoprPriceCloseU = Tochka.PriceCloseU - Minut.High;
												return 0; // 
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
	return 0; // 
}
void PokStrat(pair<int, list<Tochka>>& SpisokSdelok) // ���������� � ���� ����� ���������� �������� �� ���������
{
	float D_max = 0.0f;		// ������������ �����
	float Prosadka = 0.0f; 		// ������������ ��������
	float Dohod = 0.0f;		// �������� �����
	int KolSdel = 0;		// ���������� ������
	float SredSdel = 0.0f; 		// ������� ���������� ����� ������
	float MaxUbitSdel = 0.0f;	// ������������ ������ �� ������
	float DohodMes = 0.0f; 		// ����� �� �����
	float Limit = 0.0f;		// ����� ��� ��������
	float DohodMesProc = 0.0f;	// ���������� � ��������� �� �����
	float MaxUbitSdelP = 0.0f;	// ������������ ������ �� ������ � ���������
	float MaxUbitProsP = 0.0f;	// ������������ ������ �� �������� � ���������
	float KoefDohod = 0.0f;		// ����������� ����������
	float KoefPereschRez = 1.0;	// ����������� ��������� ����������

	float X = 0.0; // ���������� ��� ������ ������������ �������
	float Y = 0.0; // 
	float SumX = 0.0; // 
	float SumY = 0.0; // 
	float MX = 0.0; // 
	float MY = 0.0; // 
	float Sum_dXdY = 0.0; // 
	float Sum_dX2 = 0.0; // 
	float Sum_dY2 = 0.0; // 
	float KoefPirson = 0.0; // 

	if (!PS.is_open())	// ���� ���� ��� ������ ����������� �� ������
	{
		string FileName; // 
		if (Strategi == 'y')	// ���� ����� ������� ��������� ���������
		{
			FileName = "��������� (O-"; // ������� ��������������� ��� �����
			FileName += to_string(Polz_otkr); // 
			FileName += ",C-"; // 
			FileName += to_string(Polz_ubit); // 
			FileName += ",K-"; // 
			FileName += to_string(Polz_Sniat); // 
			FileName += ",V-"; // 
			FileName += to_string(Polz_kolVol); // 
			FileName += ").txt"; // 
			PS.open(FileName); // 
		}
		else
		{
			PS.open("��� ���������.txt"); // ���� ����� ������� ��� ���������
		}
		PS << "KolVol" << '\t'
			<< "PokOpen" << '\t'
			<< "PokClose" << '\t'
			<< "PokKill" << '\t'
			<< "Limit" << '\t'
			<< "KolSdel" << '\t'
			<< "D_max" << '\t'
			<< "Dohod" << '\t'
			<< "SredSdel" << '\t'
			<< "MaxUbitSdel" << '\t'
			<< "MaxUbitSdelP" << '\t'
			<< "Prosadka" << '\t'
			<< "MaxUbitProsP" << '\t'
			<< "DohodMes" << '\t'
			<< "DohodMesProc" << '\t'
			<< "KoefPirson" << '\t'
			<< "KoefDohod" << endl; // ������� ��������� ���� ���
	}

	for (Tochka& Sdelka : SpisokSdelok.second)	// ���������� ��� ������ �� ���������
	{
		if (Sdelka.Sostoyanie == 'I')	// ���� ������ ���������
		{
			Dohod += Sdelka.Rezult; // ������� �����
			KolSdel++; // ��������� ���������� ������ �� ���������
			SumX = SumX + Dohod; // ����� � ��� ����. �������
			SumY = SumY + KolSdel; // ����� Y ��� ����. �������
			if (Dohod > D_max)
			{
				D_max = Dohod; // ����� ������������� ������
			}
			if (Sdelka.Rezult < MaxUbitSdel)
			{
				MaxUbitSdel = Sdelka.Rezult; // ����� ������������� ������ �� ������
			}
			if (D_max - Dohod > Prosadka)
			{
				Prosadka = D_max - Dohod; // ����� ������������ ��������
			}
		}
	}

	// ������ ������������ ���������� �������
	//---------------------------------------
	MX = SumX / KolSdel; // 
	MY = SumY / KolSdel; // 
	for (Tochka& Sdelka : SpisokSdelok.second)
	{
		if (Sdelka.Sostoyanie == 'I')	// 
		{
			X = X + Sdelka.Rezult; // 
			Y = Y + 1.0f; // 
			Sum_dXdY = Sum_dXdY + (X - MX) * (Y - MY); // 
			Sum_dX2 = Sum_dX2 + pow((X - MX), 2); // 
			Sum_dY2 = Sum_dY2 + pow((Y - MY), 2); // 
		}
	}
	KoefPirson = pow(Sum_dXdY / sqrt(Sum_dX2 * Sum_dY2), 2);
	//---------------------------------------

	MaxUbitSdel = abs(MaxUbitSdel); // 
	if ((Prosadka * 100 / DopProsadka) < (MaxUbitSdel * 100 / DopUbitSdel))	// ���� ����������� �� ������������ �������� ����� ������ ������������ �� ������������� ������ �� ������
		Limit = MaxUbitSdel * 100 / DopUbitSdel;			// � ������ ������� �����, ����������� �� ������������ ������ �� ������
	else
		Limit = Prosadka * 100 / DopProsadka;				// ����� ������� �����, ����������� �� ������������ ��������
	if ((Prosadka + GarantObesp) > Limit)					// ���� �������� + ����������� ����������� ������ ������������ ������
		Limit = Prosadka + GarantObesp;					// ����� ���������� ����������� ����� �� ������������ �����������
	if (KolSdel)	// ���� ���� ���� ���� ������ ��������� ����������� ����������
	{
		SredSdel = Dohod / KolSdel;			// ������� ���������� �� ������
		DohodMes = Dohod / KolDnei * 21;		// ������� ����� �� �����
		DohodMesProc = DohodMes / Limit * 100;		// ���������� � ��������� �� �����
		MaxUbitSdelP = MaxUbitSdel / Limit * 100;	// ������������ ������ �� ������ � ���������
		MaxUbitProsP = Prosadka / Limit * 100;		// ������������ ������ �� �������� � ���������
		if (DohodMes > 0)
		{
			KoefDohod = DohodMes * DohodMesProc; // ����������� ����������
		}
		else
		{
			KoefDohod = 0; // ���� ��������� ��������
		}
	}
	// ������� ���������� ��������� �� ���� �������� � ����
	PS << SpisokSdelok.first << '\t'
		<< SpisokSdelok.second.front().PokOpen << '\t'
		<< SpisokSdelok.second.front().PokClose << '\t'
		<< SpisokSdelok.second.front().PokKill << '\t'
		<< Limit << '\t'
		<< KolSdel << '\t'
		<< D_max << '\t'
		<< Dohod << '\t'
		<< SredSdel << '\t'
		<< MaxUbitSdel << '\t'
		<< MaxUbitSdelP << '\t'
		<< Prosadka << '\t'
		<< MaxUbitProsP << '\t'
		<< DohodMes << '\t'
		<< DohodMesProc << '\t'
		<< KoefPirson << '\t'
		<< KoefDohod << endl; // 

	if (Strategi == 'y')	// ���� ����� ������� ���������� ���������
	{			// ����������� ���������� ��������� �� ����� 100000
		KoefPereschRez = 100000 / Limit; // 
		Limit = 100000; // 
		Dohod *= KoefPereschRez; // 
		Prosadka *= KoefPereschRez; // 
		D_max *= KoefPereschRez; // 
		MaxUbitSdel *= KoefPereschRez; // 
		if (KolSdel)
		{
			SredSdel = Dohod / KolSdel; //
			DohodMes = Dohod / KolDnei * 21; //
			DohodMesProc = DohodMes / Limit * 100; //
			MaxUbitSdelP = MaxUbitSdel / Limit * 100; //
			MaxUbitProsP = Prosadka / Limit * 100; //
			if (DohodMes > 0)
			{
				KoefDohod = DohodMes * DohodMesProc; // 
			}
			else
			{
				KoefDohod = 0; // 
			}
		}
		// ������� ���������� ��������� � ��������� �� 100000
		PS << SpisokSdelok.first << '\t'
			<< SpisokSdelok.second.front().PokOpen << '\t'
			<< SpisokSdelok.second.front().PokClose << '\t'
			<< SpisokSdelok.second.front().PokKill << '\t'
			<< Limit << '\t'
			<< KolSdel << '\t'
			<< D_max << '\t'
			<< Dohod << '\t'
			<< SredSdel << '\t'
			<< MaxUbitSdel << '\t'
			<< MaxUbitSdelP << '\t'
			<< Prosadka << '\t'
			<< MaxUbitProsP << '\t'
			<< DohodMes << '\t'
			<< DohodMesProc << '\t'
			<< KoefPirson << '\t'
			<< KoefDohod << endl; // 
	}
}
void Strategiya(map<const int, list<Tochka>>& TMap, list<Svecha>& TorgilList)
{
	float sn = 0.0f, ot = 0.0f, ub = 0.0f; // 
	Tochka* TekSdelka; // ���������� ��� �������� ���������� ������
	pair<int, list<Tochka>> SpisokSdelok; // ���������� ��� �������� �������� ������ ������
	for (pair<const int, list<Tochka>>& element : TMap)
	{
		for (int sniat(1); sniat <= 40; sniat += 1)			// ������� ������ ������ (0.1 - 4.0%)
		{
			for (int otkr(1); otkr <= 40; otkr += 1)		// ������� �������� ������ (0.1 - 4.0%)
			{
				for (int ubit(1); ubit <= 40; ubit += 1)	// ������� �������� ������ � ������� (0.1 - 4.0%)
				{
					if (Strategi == 'y')
					{	// ��������� ���������� ���������
						SpisokSdelok.first = (*TMap.find(Polz_kolVol)).first; // ����������� ������ ����� ����� �� ���������� 
						SpisokSdelok.second = (*TMap.find(Polz_kolVol)).second; // � Polz_kolVol ���������� ������� �� ���
						sniat = Polz_Sniat; // ������������ ���������� ��������� ��������� �������������
						otkr = Polz_otkr; // 
						ubit = Polz_ubit; // 
					}
					else
					{	// ��������� ��� ���������
						SpisokSdelok.first = element.first; // ����������� ������ ����� �����
						SpisokSdelok.second = element.second; // 
					}
					for (Tochka& Sdelka : SpisokSdelok.second)	// ����� �� �������� ������ ��������� ������
					{
						Sdelka.PokKill = (float)sniat / 10;	// �������� � ������� ������ ���������� ������
						Sdelka.PokOpen = (float)otkr / 10;	// ��������
						Sdelka.PokClose = (float)ubit / 10;	// �������� � �������
						sn = floor(sniat * (Sdelka.Close - Sdelka.KoefPerS) / 1000 / ShagCeni) * ShagCeni; // ��������� �������� �������� ����� ��� ������ ������
						ot = floor(otkr * (Sdelka.Close - Sdelka.KoefPerS) / 1000 / ShagCeni) * ShagCeni; // ��� �������� ������
						ub = floor(ubit * (Sdelka.Close - Sdelka.KoefPerS) / 1000 / ShagCeni) * ShagCeni; // ��� �������� � �������
						if (Sdelka.Oper == 'B')	// ���� �������� �������
						{
							Sdelka.PriceOpen = Sdelka.Close + ot + ShagCeni; // ��������� ���� ��������
							Sdelka.PriceKill = Sdelka.Close - sn - ShagCeni; // ������
							Sdelka.PriceCloseU = Sdelka.Close - ub - ShagCeni; // �������� � �������
						}
						else if (Sdelka.Oper == 'S')	// ���� �������� �������
						{
							Sdelka.PriceOpen = Sdelka.Close - ot - ShagCeni; // 
							Sdelka.PriceKill = Sdelka.Close + sn + ShagCeni; // 
							Sdelka.PriceCloseU = Sdelka.Close + ub + ShagCeni; // 
						}
						FindOpen(TorgilList, Sdelka); // ����� ����� ��������
						FindKill(TorgilList, Sdelka); // ����� ����� ������
						Sdelka.OprSostoyanie(); // ���������� ��������� ������ (�������, ���� ����� ����� ��������)
						if (Sdelka.Sostoyanie == 'I')
						{
							FindClose(TorgilList, Sdelka); // ���� ������ �������, ����� ����� �������� � �������
						}
					}

					Sort_Po_Otkr(SpisokSdelok); // ������������� �� ���� ��������
					TekSdelka = NULL; // ������� ������ - �����

					for (Tochka& Sdelka : SpisokSdelok.second)	// ����� ���������� ������ ������
					{
						if (Sdelka.Sostoyanie == 'I')	// ��������� ������ �����������
						{
							if (TekSdelka)	// ���� ���� ������� ������
							{
								if (TekSdelka->Oper == Sdelka.Oper)	// ���� �������� ������� ������ � ����� ���������
								{	// ��������� ���������� �������
									if (TekSdelka->DateCloseU != 0 && TekSdelka->GetVremia('�', 'U') <= Sdelka.GetVremia('�', 'O')) // ������� ������� � ������� ������ �������� �����
									{
										TekSdelka->CalcRezult();	// ������� ��������� � �������
										TekSdelka->Koment = "������";	// 
										TekSdelka = &Sdelka;	// ������� ����� ������ ��� �������
									}
									else	// ������� ������� � ������� ����� �������� �����, ���������� �����, ��������� �������
									{
										Sdelka.Sostoyanie = 'K'; // 
										Sdelka.Koment = "��������� ������� D-"; // 
										Sdelka.Koment += to_string(TekSdelka->DateOpen); // 
										Sdelka.Koment += " T-"; // 
										Sdelka.Koment += to_string(TekSdelka->TimeOpen); // 
									}
								}
								else	// ���� �������� ������� ������ � ����� �� ���������
								{	// ��������� �������� ������� � ��������
									switch (TekSdelka->Oper)
									{
									case 'B':	// ���� ������� ������ �� �������
										if (TekSdelka->DateCloseU != 0 && TekSdelka->GetVremia('�', 'U') < Sdelka.GetVremia('�', 'O')) // ������� ������� � ������� ������ �������� �����
										{
											TekSdelka->CalcRezult();	// ������� ��������� � �������
											TekSdelka->Koment = "������"; // 
											TekSdelka = &Sdelka;	// ������� ����� ������ ��� �������
										}
										else	// ������� ������� � ������� ����� �������� �����
										{
											if (TekSdelka->GetVremia('�', 'U') == Sdelka.GetVremia('�', 'O'))	// ���� ������� ������� � ������� ������������ � ��������� �����
											{	// ��������� �������� �� ����
												if (abs(TekSdelka->PriceCloseU - Sdelka.PriceOpen) > 0.000001 && TekSdelka->PriceCloseU > Sdelka.PriceOpen) // ������� ������� ������ �����
												{
													TekSdelka->CalcRezult();	// ������� ��������� � �������
													TekSdelka->Koment = "����� ������ ���������, ������"; // 
													TekSdelka = &Sdelka;	// ������� ����� ������ ��� �������
													break; // 
												}
											}
											if (abs(TekSdelka->PriceOpen - Sdelka.PriceOpen) < 0.000001 || TekSdelka->PriceOpen < Sdelka.PriceOpen)
											{	// ������� ������� ������� ���� �������� ����� �������, ������� ������� � ��������, �������� ����������
												// ����� ������ ������������, ���� ��� ������� � �������� ��� �������� � �������� �������
												TekSdelka->TikerCloseP = Sdelka.TikerOpen; // 
												TekSdelka->DateCloseP = Sdelka.DateOpen; // 
												TekSdelka->TimeCloseP = Sdelka.TimeOpen; // 
												TekSdelka->PriceCloseP = Sdelka.PriceOpen; // 
												TekSdelka->KoefPerCloseP = Sdelka.KoefPerOpen; // 
												TekSdelka->PoprPriceCloseP = Sdelka.PoprPriceOpen;
												TekSdelka->CalcRezult(); // 
												TekSdelka->Koment = "�������"; // 
												TekSdelka = &Sdelka;	// ������� ����� ������ ��� �������
											}
											else	// ������� ������� ������� ���� �������� ����� ������� (� �������� ��������/�������� �����), ���������� �����
											{
												Sdelka.Sostoyanie = 'K'; // 
												Sdelka.Koment = "����� �������� �������� D-"; // 
												Sdelka.Koment += to_string(TekSdelka->DateOpen); // 
												Sdelka.Koment += " T-"; // 
												Sdelka.Koment += to_string(TekSdelka->TimeOpen); // 
											}
										}
										break; // 
									case 'S':	// ���� ������� ������ �� ������� (��� ��������)
										if (TekSdelka->DateCloseU != 0 && TekSdelka->GetVremia('�', 'U') < Sdelka.GetVremia('�', 'O')) // ������� ������� ������ �����
										{
											TekSdelka->CalcRezult();	// ������� ��������� � �������
											TekSdelka->Koment = "������"; // 
											TekSdelka = &Sdelka;	// ������� ����� ������ ��� �������
										}
										else	// ������� ������� ����� �������� �����
										{
											if (TekSdelka->GetVremia('�', 'U') == Sdelka.GetVremia('�', 'O'))
											{
												if (abs(TekSdelka->PriceCloseU - Sdelka.PriceOpen) > 0.000001 && TekSdelka->PriceCloseU < Sdelka.PriceOpen) // ������� ������� ������ �����
												{
													TekSdelka->CalcRezult();	// ������� ��������� � �������
													TekSdelka->Koment = "����� ������ ���������, ������"; // 
													TekSdelka = &Sdelka;	// ������� ����� ������ ��� �������
													break; // 
												}
											}
											if (abs(TekSdelka->PriceOpen - Sdelka.PriceOpen) < 0.000001 || TekSdelka->PriceOpen > Sdelka.PriceOpen)
											{	// ������� ������� ������� ���� �������� ����� �������, ������� ������� � ��������, �������� ����������
												TekSdelka->TikerCloseP = Sdelka.TikerOpen; // 
												TekSdelka->DateCloseP = Sdelka.DateOpen; // 
												TekSdelka->TimeCloseP = Sdelka.TimeOpen; // 
												TekSdelka->PriceCloseP = Sdelka.PriceOpen; // 
												TekSdelka->KoefPerCloseP = Sdelka.KoefPerOpen; // 
												TekSdelka->PoprPriceCloseP = Sdelka.PoprPriceOpen;
												TekSdelka->CalcRezult(); // 
												TekSdelka->Koment = "�������"; // 
												TekSdelka = &Sdelka;	// ������� ����� ������ ��� �������
											}
											else	// ������� ������� ������� ���� �������� ����� ������� (� �������� ��������/�������� �����), ���������� �����
											{
												Sdelka.Sostoyanie = 'K'; // 
												Sdelka.Koment = "����� �������� �������� D-"; // 
												Sdelka.Koment += to_string(TekSdelka->DateOpen); // 
												Sdelka.Koment += " T-"; // 
												Sdelka.Koment += to_string(TekSdelka->TimeOpen); // 
											}
										}
									}
								}
							}
							else
							{
								TekSdelka = &Sdelka;	// ������� ����� ������ ��� �������
							}
						}
					}
					if (TekSdelka)	// ����� ���������� ��� ������, ���� ���� ������� ������
					{
						if (TekSdelka->DateCloseU == 0)	// ���� � ������� �� ������� ���� �������� � �������
						{	// ������� �������� �� ��������� ������ ������ ������
							TekSdelka->TikerCloseP = TorgilList.back().next.back().next.back().next.back().next.back().Tiker; // 
							TekSdelka->DateCloseP = TorgilList.back().next.back().next.back().next.back().next.back().Date; // 
							TekSdelka->TimeCloseP = TorgilList.back().next.back().next.back().next.back().next.back().Time; // 
							TekSdelka->PriceCloseP = TorgilList.back().next.back().next.back().next.back().next.back().Close; // 
							TekSdelka->KoefPerCloseP = TorgilList.back().next.back().next.back().next.back().next.back().KoefPer; // 
							TekSdelka->Koment = "������ �� �������"; // 
						}
						else	// ���� �������� � ������� ����������, �� ������� � �������
						{
							TekSdelka->Koment = "������"; // 
						}
						TekSdelka->CalcRezult(); // ������� ���������
						if (Strategi == 'y')	// ���� ����� ������� ���������� ���������
						{
							Print(SpisokSdelok); // ������� ��� ������ �� ���������
							PokStrat(SpisokSdelok); // ������� ����� ���������� ���������
							return; // 
						}
						else
						{
							PokStrat(SpisokSdelok);	// ������� ����� ���������� �� ���� ����������
						}
					}
				}
			}
		}
	}
}

int main()
{
	setlocale(LC_ALL, "Russian");	// ����� � ��������� ������ stof ������������ �����������
	// ������� ��������� ������ ���������
	cout << "������� ������ �� ���������? "; // 
	do
	{
		cin >> Strategi; // 
		if (Strategi != 'n' && Strategi != 'y')
		{
			cout << "������������ ������! "; // 
		}
		else
		{
			if (Strategi == 'y')
			{
				cout << "������� ���������� ���������� �������: "; // 
				cin >> Polz_kolVol; // 
				cout << "������� ���������� ������: "; // 
				cin >> Polz_Sniat; // 
				cout << "������� ���������� ��������: "; // 
				cin >> Polz_otkr; // 
				cout << "������� ���������� ������: "; // 
				cin >> Polz_ubit; // 
			}
			break; // 
		}
	} while (true); // 


	//Strategi = 'y'; // 
	//Polz_Sniat = 8; // 
	//Polz_otkr = 11; // 
	//Polz_ubit = 4; // 
	//Polz_kolVol = 20; // 

	setlocale(LC_ALL, "C"); // ��� ���������� ������ � ����� float
	list<Svecha> Torgi; // 
	map<const int, list<Tochka>> Tochki; // 
	Nastroika(); // ��������� ������ ��������� �� �����
	ContTorgi(Torgi); // ������� ������ ������
	ContTochki(Torgi, Tochki); // 
	Strategiya(Tochki, Torgi); // 

	PS.close(); // 
	VseSdelki.close(); // 
	return 0; // 
}