
#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <map>
#include <algorithm>
#include <numeric>

using namespace std;

// ГЛОБАЛЬНЫЕ ПЕРЕМЕННЫЕ
//------------------------
ofstream VseSdelki;	// ДЛЯ ЗАПИСИ ВСЕХ СДЕЛОК
ofstream PS; // ДЛЯ ЗАПИСИ ИТОГОВ ПО СТРАТЕГИИ
string AdresLenta; // АДРЕС ЛЕНТЫ
float DopProsadka = 0; // ПОКАЗАТЕЛЬ ДОПУСТИМОЙ ПРОСАДКИ, %
float DopUbitSdel = 0; // ПОКАЗАТЕЛЬ МАКСИМАЛЬНОГО УБЫТКА ПО СДЕЛКЕ, %
float GarantObesp = 0; // ГАРАНТИЙНОЕ ОБЕСПЕЧЕНИЕ ЗА КОНТРАКТ, РУБ
float ShagCeni = 0; // ШАГ ЦЕНЫ
float StoimShagCeni = 0; // СТОИМОСТЬ ШАГА ЦЕНЫ
int KolMesyacev = 0; // КОЛИЧЕСТВО МЕСЯЦЕВ ДЛЯ ОПРЕДЕЛЕНИЯ МАКСИМАЛЬНОГО ОБЪЕМА
int KolDnei = 0; // КОЛИЧЕСТВО ДНЕЙ (Подсчитывается в ф-ии ContTochki)
char Strategi = '0'; // ВЫВОДИТЬ КОНКРЕТНУЮ СТРАТЕГИЮ ИЛИ ВСЕ, Y/N
// ЕСЛИ ВЫВОДИТСЯ КОНКРЕТНАЯ СТРАТЕГИЯ
int Polz_Sniat = 0; // ПОКАЗАТЕЛЬ СНЯТИЯ
int Polz_otkr = 0; // ПОКАЗАТЕЛЬ ОТКРЫТИЯ
int Polz_ubit = 0; // ПОКАЗАТЕЛЬ ЗАКРЫТИЯ С УБЫТКОМ
int Polz_kolVol = 0; // КОЛИЧЕСТВО ОБЪЕМОВ
//------------------------

class Svecha	// КЛАСС ДЛЯ ЗАПИСИ СВЕЧИ
{
public:
	char Period; // ПЕРИОД СВЕЧИ
	string Tiker; // ТИКЕР
	int Date = 0; // ДАТА
	short int Time = 0; // ВРЕМЯ
	float Open = 0.0f; // ОТКРЫТИЕ
	float High = 0.0f; // МАКСИМУМ
	float Low = 0.0f; // МИНИМУМ
	float Close = 0.0f; // ЗАКРЫТИЕ
	int Vol = 0; // ОБЪЕМ
	unsigned int God = 0; // ГОД
	unsigned int Mes = 0; // МЕСЯЦ
	unsigned int Den = 0;			// ДЕНЬ
	unsigned int Chas = 0;			// ЧАС
	unsigned int Min = 0;	// МИНУТА
	float KoefPer = 0.0f;	// КОЭФФИЦИЕНТ ПЕРЕСЧЕТА ПРИ СМЕНЕ ТИКЕРА
	list<Svecha> next;
	void ZapPokaz(char period)	// ОПРЕДЕЛЕНИЕ ПОКАЗАТЕЛЕЙ В ЗАВИСИМОСТИ ОТ ПЕРИОДА СВЕЧИ
	{
		if (period == 'Ч')	// ЧАС
		{
			this->Tiker = this->next.back().Tiker; // 
			this->Chas = this->next.back().Chas; // 
			this->Den = this->next.back().Den; // 
			this->Mes = this->next.back().Mes; // 
		}
		else if (period == 'Д')	// ДЕНЬ
		{
			this->Tiker = this->next.back().Tiker; // 
			this->Den = this->next.back().Den; // 
			this->Mes = this->next.back().Mes; // 
		}
		else if (period == 'М')	// МЕСЯЦ
		{
			this->Mes = this->next.back().Mes; // 
		}
		this->Period = period; // ГОД
		this->God = this->next.back().God; // 

		this->Open = this->next.front().Open; // ОТКРЫТИЕ
		this->Close = this->next.back().Close; // ЗАКРЫТИЕ
		this->High = (*max_element(this->next.begin(), this->next.end(), [](Svecha& el1, Svecha& el2)
			{
				return el1.High < el2.High;
			})).High; // МАКСИМУМ
		this->Low = (*max_element(this->next.begin(), this->next.end(), [](Svecha& el1, Svecha& el2)
			{
				return el1.Low > el2.Low;
			})).Low; // МИНИМУМ
		this->Vol = accumulate(this->next.begin(), this->next.end(), 0, [](int el1, Svecha& el2)
			{
				return el1 + el2.Vol;
			}); // ОБЪЕМ
	}
};
class Svecha_30	// СВЕЧА 30 МИНУТ ДЛЯ ОПРЕДЕЛЕНИЯ ТОЧКИ ВХОДА
{
public:
	string Tiker;
	int Date;
	int TimeN; // ВРЕМЯ ОТКРЫТИЯ СВЕЧИ
	int TimeK; // ВРЕМЯ ЗАКРЫТИЯ СВЕЧИ
	float Open; // 
	float Close; // 
	int Vol; // 
	char Oper; // ОПЕРАЦИЯ ТОЧКИ ВХОДА
	short int Mes; // МЕСЯЦ
	float izm_5; // ИЗМЕНЕНИЕ ПЕРВОЙ ПЯТИМИНУТКИ
	float KoefPer = 0.0f; // КОЭФФИЦИЕНТ ПЕРЕСЧЕТА
};
class Tochka // ТОЧКА ВХОДА
{
public:
	string Tiker = "0"; // ТИКЕР
	int Date = 0; // ДАТА
	int TimeN = 0; // ВРЕМЯ ОТКРЫТИЯ СВЕЧИ
	int TimeK = 0; // ВРЕМЯ ЗАКРЫТИЯ СВЕЧИ
	float Open = 0.0f; // ОТКРЫТИЕ
	float Close = 0.0f; // ЗАКРЫТИЕ
	float KoefPerS = 0.0f; // КОЭФФИЦИЕНТ ПЕРЕСЧЕТА ПРИ СМЕНЕ ТИКЕРА
	char Oper; // ОПЕРАЦИЯ ВХОДА
	int Vol = 0; // ОБЪЕМ
	float izm_5 = 0; // ИЗМЕНЕНИЕ ПЕРВОЙ ПЯТИМИНУТКИ
	float PokKill = 0.0f; // ПОКАЗАТЕЛЬ СНЯТИЯ ЗАЯВКИ
	float PokOpen = 0.0f; // ПОКАЗАТЕЛЬ ОТКРЫТИЯ
	float PokClose = 0.0f; // ПОКАЗАТЕЛЬ ЗАКРЫТИЯ С УБЫТКОМ
	string TikerOpen = "0"; // ТИКЕР ОТКРЫТИЯ
	int DateOpen = 0; // ДАТА ОТКРЫТИЯ
	int TimeOpen = 0; // ВРЕМЯ ОТКРЫТИЯ
	float PriceOpen = 0.0f; // ЦЕНА ОТКРЫТИЯ
	float KoefPerOpen = 0.0f; // КОЭФФИЦИЕНТ ПЕРЕСЧЕТА ЦЕНЫ ОТКРЫТИЯ
	string TikerKill = "0"; // ТИКЕР СНЯТИЯ
	int DateKill = 0; // ДАТА СНЯТИЯ
	int TimeKill = 0; // ВРЕМЯ СНЯТИЯ
	float PriceKill = 0.0f; // ЦЕНА СНЯТИЯ
	float KoefPerKill = 0.0f; // КОЭФФИЦИЕНТ ПЕРЕСЧЕТА ЦЕНЫ СНЯТИЯ
	string TikerCloseU = "0"; // ТИКЕР ЗАКРЫТИЯ С УБЫТКОМ
	int DateCloseU = 0; // ДАТА ЗАКРЫТИЯ С УБЫТКОМ
	int TimeCloseU = 0; // ВРЕМЯ ЗАКРЫТИЯ С УБЫТКОМ
	float PriceCloseU = 0.0f; // ЦЕНА ЗАКРЫТИЯ С УБЫТКОМ
	float KoefPerCloseU = 0.0f; // КОЭФФИЦИЕНТ ПЕРЕСЧЕТА ЦЕНЫ ЗАКРЫТИЯ С УБЫТКОМ
	string TikerCloseP = "0"; // ТИКЕР ЗАКРЫТИЯ С ПРИБЫЛЬЮ
	int DateCloseP = 0; // ДАТА ЗАКРЫТИЯ С ПРИБЫЛЬЮ
	int TimeCloseP = 0; // ВРЕМЯ ЗАКРЫТИЯ С ПРИБЫЛЬЮ
	float PriceCloseP = 0.0f; // ЦЕНА ЗАКРЫТИЯ С ПРИБЫЛЬЮ
	float KoefPerCloseP = 0.0f; // КОЭФФИЦИЕНТ ПЕРЕСЧЕТА ЦЕНЫ ЗАКРЫТИЯ С ПРИБЫЛЬЮ
	char Sostoyanie; // СОСТОЯНИЕ (СНЯТА/ОТКРЫТА)
	float Rezult = 0.0f; // РЕЗУЛЬТАТ (ПРИБЫЛЬ/УБЫТОК)
	float PoprPriceOpen = 0.0f; // ПОПРАВКА ЦЕНЫ ОТКРЫТИЯ НА РАЗНИЦУ МЕЖДУ ЦЕНОЙ СДЕЛКИ И МАКС/МИН ЦЕНЫ СВЕЧКИ (ЗАКЛАТЫВАЕТСЯ ХУДШАЯ ЦЕНА СДЕЛКИ ЗА МИНУТУ)
	float PoprPriceCloseU = 0.0f; // ПОПРАВКА ЦЕНЫ ЗАКРЫТИЯ С УБЫТКОМ НА РАЗНИЦУ МЕЖДУ ЦЕНОЙ СДЕЛКИ И МАКС/МИН ЦЕНЫ СВЕЧКИ (ЗАКЛАТЫВАЕТСЯ ХУДШАЯ ЦЕНА СДЕЛКИ ЗА МИНУТУ)
	float PoprPriceCloseP = 0.0f; // ПОПРАВКА ЦЕНЫ ЗАКРЫТИЯ С ПРИБЫЛЬЮ НА РАЗНИЦУ МЕЖДУ ЦЕНОЙ СДЕЛКИ И МАКС/МИН ЦЕНЫ СВЕЧКИ (ЗАКЛАТЫВАЕТСЯ ХУДШАЯ ЦЕНА СДЕЛКИ ЗА МИНУТУ)
	string Koment; // КОМЕНТАРИЙ

	Tochka()
	{
	}
	Tochka(Svecha_30& s) // ЗАПИСЬ ПОКАЗАТЕЛЕЙ ТОЧКИ ИЗ СВЕЧИ 30 МИН.
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
	unsigned int GetVremia(char period, char deistvie) // ВОЗВРАЩАЕТ ВРЕМЯ В ЗАВИСИМОСТИ ОТ ПЕРИОДА
	{						// deistvie - ВРЕМЯ СВЕЧИ, periol - ПЕРИОД		
		switch (deistvie)
		{
		case 'S': // ВРЕМЯ СВЕЧИ
			if (period == 'Т') // ФОРМАТ ВРЕМЕНИ ДО МИНУТЫ
			{
				return this->Date * 10000 + this->TimeK;
			}
			if (period == 'Ч') // ДО ЧАСА
			{
				return this->Date * 100 + this->TimeK / 100;
			}
			if (period == 'Д') // ДО ДНЯ
			{
				return this->Date;
			}
			if (period == 'М') // ДО МЕСЯЦА
			{
				return this->Date / 100;
			}
			if (period == 'Г') // ДО ГОДА
			{
				return this->Date / 10000;
			}
			return -1;
		case 'O': // ВРЕМЯ ОТКРЫТИЯ
			if (period == 'Т')
			{
				return this->DateOpen * 10000 + this->TimeOpen;
			}
			if (period == 'Ч')
			{
				return this->DateOpen * 100 + this->TimeOpen / 100;
			}
			if (period == 'Д')
			{
				return this->DateOpen;
			}
			if (period == 'М')
			{
				return this->DateOpen / 100;
			}
			if (period == 'Г')
			{
				return this->DateOpen / 10000;
			}
			return -1;
		case 'U': // ВРЕМЯ ЗАКРЫТИЯ С УБЫТКОМ
			if (period == 'Т')
			{
				return this->DateCloseU * 10000 + this->TimeCloseU;
			}
			return -1;
		}
		return -1;
	}
	void OprSostoyanie()	// ОПРЕДЕЛИТЬ СОСТОЯНИЕ ТОЧКИ ВХОДА, ИСПОЛНЕНА ИЛИ СНЯТА
	{			// ПО ДАТЕ СНЯТИЯ И ОТКРЫТИЯ СДЕЛКИ
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
	void CalcRezult()	// ПОДБИТЬ РЕЗУЛЬТАТ СДЕЛКИ
	{			// В ЗАВИСИМОСТИ ОТ НАПРАВЛЕНИЯ С УЧЕТОМ ПОПРАВКИ НА ХУДШУЮ ЦЕНУ СДЕЛКИ
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
void Nastroika()	// НАСТРОЙКА РАБОТЫ ПРОГРАММЫ ИЗ ФАЙЛА
{
	ifstream FileNastr("Настройка.txt"); // АРЕС ФАЙЛА НАСТРОЙКИ
	FileNastr >> AdresLenta; // АДРЕС ФАЙЛОВ ТОРГОВ
	FileNastr >> DopProsadka; // ДОПУСТИМАЯ ПРОСАДКА
	FileNastr >> DopUbitSdel; // ДОПУСТИМЫЙ УБЫТОК ЗА СДЕЛКУ
	FileNastr >> GarantObesp; // ГАРАНТИЙНОЕ ОБЕСПЕЧЕНИЕ ЗА КОНТРАКТ
	FileNastr >> ShagCeni; // ШАГ ЦЕНЫ КОНТРАКТА
	FileNastr >> StoimShagCeni; // СТОИМОСТЬ ШАГА ЦЕНЫ
	FileNastr >> KolMesyacev; // КОЛИЧЕСТВО МЕСЯЦЕВ ДЛЯ ОПРЕДЕЛЕНИЯ МАКСИМАЛЬНОГО ОБЪЕМА
	FileNastr.close();
}
void ContTorgi(list<Svecha>& Torg)	// СОЗДАНИЕ КОНТЕЙНЕРА ПОКАЗАТЕЛЕЙ ТОРГОВ
					// ХРАНЕНИЕ ПОКАЗАТЕЛЕЙ ТОРГОВ ОРГАНИЗОВАНО КАК
					// СПИСОК СВЕЧЕЙ, КАЖДЫЙ ЭЛЕМЕНТ СПИСКА В СВОЮ
					// ОЧЕРЕДЬ СОДЕРЖИТ СПИСОК СВЕЧЕЙ МЕНШЕГО ПЕРИОДА
{
	ifstream FList(AdresLenta + "filelist.txt"); // ОТКРЫТЬ ФАЙЛ СО СПИСКОМ НАЗВАНИЙ ФАЙЛОВ ТОРГОВ
	list<string> NameLenta; // СПИСОК НАЗВАНИЙ ФАЙЛОВ
	string str;

	while (FList >> str)	NameLenta.push_back(str); // ЗАПОЛНИТЬ СПИСОК НАЗВАНИЙ ФАЛОВ ТОРГОВ
	FList.close(); // 

	ifstream FLenta; // 
	string Tiker; // 
	float PrCl = 0.0f; // 
	char ch; // 
	int n = 0; // 
	float Koef = 0.0f; // 
	Svecha Sv; // 

	int G = 0, M = 0, D = 0, Ch = 0; // 
	Torg.push_back(Svecha()); // СПИСОК ГОДИЧНЫХ СВЕЧЕЙ
	Torg.back().next.push_back(Svecha()); // МЕСЯЧНЫХ
	Torg.back().next.back().next.push_back(Svecha()); // ДНЕВНЫХ
	Torg.back().next.back().next.back().next.push_back(Svecha()); // ЧАСОВЫХ

	for (string Lenta : NameLenta)
	{
		cout << Lenta << endl; // 
		FLenta.open(AdresLenta + Lenta, ios_base::in); // ОТКРЫТЬ ФАЙЛ ТОРГОВ
		getline(FLenta, str); // 
		str = ""; // 
		while (FLenta.get(ch))	// КАЖДАЯ СТРОКА ФАЙЛА РАЗДЕЛЯЕТСЯ НА ПОКАЗАТЕЛИ ЧЕРЕЗ ЗАПЯТУЮ
					// И ЗАПИСЫВАЕТСЯ В ПЕРЕМЕННУЮ КЛАССА Svecha (МИНУТНАЯ)
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
					Sv.Tiker = str; // ТИКЕР
					Sv.Period = 'М'; // ПЕРИОД МИНУТА
					break; // 
				case 2:
					Sv.Date = stoi(str) % 1000000; // ДАТА
					Sv.God = stoi(str.substr(2, 2)); // ДАТА ДЕЛИТСЯ НА ГОД
					Sv.Mes = Sv.God * 100 + stoi(str.substr(4, 2)); // МЕСЯЦ
					Sv.Den = Sv.Mes * 100 + stoi(str.substr(6, 2)); // ДЕНЬ
					break; // 
				case 3:
					Sv.Time = stoi(str) / 100; // ВРЕМЯ
					Sv.Chas = Sv.Den * 100 + stoi(str)/10000; // ЧАСЫ
					Sv.Min = Sv.Den * 10000 + Sv.Time; // МИНУТЫ
					break; // 
				case 4:
					Sv.Open = stof(str); // ЦЕНА ОТКРЫТИЯ
					break; // 
				case 5:
					Sv.High = stof(str); // МАКСИМАЛЬНАЯ ЦЕНА
					break; // 
				case 6:
					Sv.Low = stof(str); // МИНИМАЛЬНАЯ
					break; // 
				case 7:
					Sv.Close = stof(str); // ЦЕНА ЗАКРЫТИЯ
					break; // 
				case 8:
					Sv.Vol = stoi(str); // ОБЪЕМ СВЕЧИ
					if (Tiker != "")	// ЕСЛИ СМЕНИЛСЯ ТИКЕР (НОВЫЙ КОНТРАКТ)
					{
						if (Tiker != Sv.Tiker)
						{
							Tiker = Sv.Tiker; // НОВЫЙ ТИКЕР
							Koef = PrCl - Sv.Open; // ПОСЧИТАТЬ КОЭФФИЦИЕНТ ПЕРЕСЧЕТА ЦЕН 
						}	// ПРИ СМЕНЕ ТИКЕРА ЦЕНА СДВИГАЕТСЯ
					}
					else
					{
						Tiker = Sv.Tiker; // 
						G = Sv.God; // 
						M = Sv.Mes; // 
						D = Sv.Den; // 
						Ch = Sv.Chas; // 
					}
					Sv.KoefPer = Koef; // ЗАПИСАТЬ КОЭФФИЦИЕНТ ПЕРЕСЧЕТА КАЖДОЙ МИНУТЫ
					Sv.Open += Koef; // ПЕРЕСЧИТАТЬ ЦЕНУ ОТКРЫТИЯ
					Sv.High += Koef; // МАКСИМАЛЬНУЮ
					Sv.Low += Koef; // МИНИМАЛЬНУЮ
					Sv.Close += Koef; // ЗАКРЫТИЯ
					PrCl = Sv.Close; // СОХРАНИТЬ В ПЕРЕМЕННОЙ ЦЕНУ ЗАКРЫТИЯ

					// ВСТАВИТЬ ЭЛЕМЕНТЫ СПИСКА (ГОД, МЕСЯЦ, ДЕНЬ, ЧАС, МИНУТА)
					if (Sv.Chas != Ch) // ЕСЛИ СМЕНИЛСЯ ЧАС
					{
						Ch = Sv.Chas; // СОХРАНИТЬ ТЕКУЩИЙ ЧАС
						Torg.back().next.back().next.back().next.back().ZapPokaz('Ч'); // ЗАПИСАТЬ ПОКАЗАТЕЛИ ПОСЛЕДНЕГО ЧАСА
						if (Sv.Den != D)	// СМЕНИЛСЯ ДЕНЬ
						{
							D = Sv.Den; // СОХРАНИТЬ ТЕКУЩИЙ ДЕНЬ
							Torg.back().next.back().next.back().ZapPokaz('Д'); // ЗАПИСАТЬ ПОКАЗАТЕЛИ ПОСЛЕДНЕГО ДНЯ
							if (Sv.Mes != M)	// СМЕНИЛСЯ МЕСЯЦ
							{
								M = Sv.Mes; // СОХРАНИТЬ ТЕКУЩИЙ МЕСЯЦ
								Torg.back().next.back().ZapPokaz('М'); // ЗАПИСАТЬ ПОКАЗАТЕЛИ ПОСЛЕДНЕГО МЕСЯЦА
								if (Sv.God != G)	// СМЕНИЛСЯ ГОД
								{
									G = Sv.God; // СОХРАНИТЬ ТЕКУЩИЙ ГОД
									Torg.back().ZapPokaz('Г'); // ЗАПИСАТЬ ПОКАЗАТЕЛИ ПОСЛЕДНЕГО ГОДА
									Torg.push_back(Svecha()); // ВСТАВИТЬ НОВУЮ ГОДОВУЮ СВЕЧУ
								}
								Torg.back().next.push_back(Svecha()); // ВСТАВИТЬ НОВУЮ МЕСЯЮНУЮ СВЕЧУ
							}
							Torg.back().next.back().next.push_back(Svecha()); // ВСТАВИТЬ НОВУЮ ДНЕВНУЮ СВЕЧУ
						}
						Torg.back().next.back().next.back().next.push_back(Svecha()); // ВСТАВИТЬ НОВУЮ ЧАСОВУЮ СВЕЧУ
					}
					Torg.back().next.back().next.back().next.back().next.push_back(Sv); // ВСТАВИТЬ НОВУЮ МИНУТНУЮ СВЕЧУ
					n = -1; // 
				}
				str = ""; // 
				n++; // 
			}
		}
		FLenta.close(); // 
	}
	// ПОСЛЕ ОБРАБОТКИ ВСЕХ ФАЙЛОВ ПОДБИТЬ ПОСЛЕДНИЕ СВЕЧКИ
	Torg.back().next.back().next.back().next.back().ZapPokaz('Ч'); // ЧАСОВАЯ
	Torg.back().next.back().next.back().ZapPokaz('Д'); // ДНЕВНАЯ
	Torg.back().next.back().ZapPokaz('М'); // МЕСЯЧНАЯ
	Torg.back().ZapPokaz('Г'); // ГОДОВАЯ
}
void ContSv_30(list<Svecha>& TorgList, list<Svecha_30>& svList)	// КОНТЕЙНЕР 30-ТИ МИНУТНЫХ СВЕЧЕЙ, СОЗДАЕТСЯ ИЗ КОНТЕЙНЕРА ТОРГОВ
{
	int i; // 
	Svecha_30 sv; // 
	bool Sv_5 = true; // 
	int Time_5; // 
	// ПОКАЗАТЕЛИ СВЕЧИ ИЗ ПОКАЗАТЕЛЕЙ ПЕРВОЙ МИНУТЫ
	sv.Tiker = TorgList.front().next.front().next.front().next.front().next.front().Tiker; // ТИКЕР
	sv.Date = TorgList.front().next.front().next.front().next.front().next.front().Date; // ДАТА
	sv.TimeN = TorgList.front().next.front().next.front().next.front().next.front().Time; // ВРЕМЯ НАЧАЛА
	sv.Open = TorgList.front().next.front().next.front().next.front().next.front().Open; // ЦЕНА ОТКРЫТИЯ
	Time_5 = TorgList.front().next.front().next.front().next.front().next.front().Time; // ВРЕМЯ НАЧАЛА ПЕРВОЙ ПЯТИМИНУТКИ
	sv.Vol = 0; // ОБЪЕМ НУЛЕВОЙ
	for (Svecha God : TorgList)	// ПРОЛИСТАТЬ ГОДИЧНЫЙ СПИСОК
	{
		for (Svecha Mes : God.next)	// ПРОЛИСТАТЬ МЕСЯЧНЫЙ СПИСОК
		{
			for (Svecha Den : Mes.next)	// ПРОЛИСТАТЬ ДНЕВНОЙ СПИСОК
			{
				for (Svecha Chas : Den.next)	// ПРОЛИСТАТЬ ЧАСОВОЙ СПИСОК
				{
					for (Svecha Minuta : Chas.next)	// ПРОЛИСТАТЬ МИНУТЫНЙ СПИСОК
					{
						if (Sv_5)	// ЕСЛИ ИДЕТ ПЕРВАЯ ПЯТИМИНУТКА
						{
							if (Minuta.Time - Time_5 >= 5)	// ЕСЛИ ПЕРВАЯ ПЯТИМИНУТКА ЗАКОНЧИЛАСЬ
							{
								Sv_5 = false; // ПЕРЕКЛЮЧАТЕЛЬ ПЕРВОЙ ПЯТИМИНУТКИ false
								sv.izm_5 = sv.Close - sv.Open; // ОПРЕДЕЛИТЬ ИЗМЕНЕНИЕ
								if (sv.Close - sv.Open >= 0)	// ЕСЛИ ПОЛОЖИТЕЛЬНОЕ
								{
									sv.Oper = 'S'; // ОПЕРАЦИЯ ПРОДАЖА
								}
								else
								{
									sv.Oper = 'B'; // ОПЕРАЦИЯ ПОКУПКА
								}
							}
						}
						if (sv.Date != Minuta.Date || Minuta.Time - sv.TimeN >= 30)	// ЕСЛИ 30-МИН. СВЕЧА СМЕНИЛАСЬ
						{
							Time_5 = Minuta.Time; // ЗАПИСАТЬ ТЕКУЩУЮ МИНУТУ
							Sv_5 = true; // ПЕРВАЯ ПЯТИМИНУТКА В true
							i = Time_5 % 10; // ПРОВЕРИТЬ ВРЕМЯ ПЕРВОЙ ПЯТИМИНУТКИ НА КОРРЕКТНОСТЬ
							if (!(i == 0 || i == 5))
								if (i < 5)		Time_5 = Time_5 - i;	// скоректировать время до 0 на конце
								else			Time_5 = Time_5 - i + 5;	// скоректировать время до 5 на конце

							svList.push_back(sv); // ДОБАВИТЬ 30-МИН. СВЕЧУ
							sv.Tiker = Minuta.Tiker; // ТИКЕР
							sv.Date = Minuta.Date; // ДАТА
							sv.TimeN = Minuta.Time; // ВРЕМЯ НАЧАЛА
							sv.Open = Minuta.Open; // ОТКРЫТИЕ
							sv.Mes = Minuta.Mes; // МЕСЯЦ
							sv.KoefPer = Minuta.KoefPer; // КОЕФ. ПЕРЕСЧЕТА
							sv.Vol = 0; // ОБЪЕМ

							i = sv.TimeN % 100; //  ПРОВЕРИТЬ ВРЕМЯ НАЧАЛА СВЕЧИ НА КОРРЕКТНОСТЬ
							if (!(i == 0 || i == 30))
								if (i < 30)		sv.TimeN = sv.TimeN - i;	// скоректировать время до 0 на конце
								else			sv.TimeN = sv.TimeN - i + 30;	// скоректировать время до 30 на конце
						}
						// ЕСЛИ СВЕЧА НЕ СМЕНИЛАСЬ
						sv.TimeK = Minuta.Time; // СОХРАНЯТЬ ВРЕМЯ ОКОНЧАНИЯ
						sv.Close = Minuta.Close; // ЦЕНУ ЗАКРЫТИЯ
						sv.Vol += Minuta.Vol; // НАКАПЛИВАТЬ ОБЪЕМ
					}
				}
			}
		}
	}
}
void SdvigIter(list<Svecha_30>::iterator& it) // сдвиг итератора на месяц вперед
{
	int m = (*it).Date / 100; // ВЗЯТЬ МЕСЯЦ
	it++; // СДВИНУТЬ ИТЕРАТОР НА СЛЕДУЮЩУЮ МИНУТУ
	while ((*it).Date / 100 == m)	// ПРОЛИСТАТЬ ДО СЛЕДУЮЩЕГО МЕСЯЦА
		it++;
}
void ContTochki(list<Svecha>& Torg, map<const int, list<Tochka>>& tochList)	// СОЗДАНИЕ КОНТЕЙНЕРА ТОЧЕК ВХОДА
{	//ТОЧКА ОПРЕДЕЛЯЕТСЯ ПО ОБЪЕМУ 30-МИНУТНОЙ СВЕЧИ (ЕСЛИ ОБЪЕМ ДОСТАТОЧНО БОЛЬШОЙ)
	list<Svecha_30> svList; // 
	ContSv_30(Torg, svList); // СОЗДАТЬ КОНТЕЙНЕР 30-МИНУТНЫХ СВЕЧЕК

	int mes = svList.front().Mes; // ВЗЯТЬ МЕСЯЦ ПЕРВОЙ 30-МИНУТНОЙ СВЕЧИ
	int SchMes = 0; // СЧЕТЧИК МЕСЯЦЕВ
	int Date = 0; // ДАТА
	map<int, int> Volume; // КОНТЕЙНЕР ОБЪЕМОВ (КОЛИЧЕСТВО ЗА ГОД, ОБЪЕМ)
	list<Tochka> TochkaList; // 
	list<Svecha_30>::iterator IterStart = svList.begin(); // ИТЕРАТОР НА НАЧАЛО ГОДИЧНОГО ПЕРИОДА ПОИСКА
	list<Svecha_30>::iterator IterFinish = svList.begin(); // ИТЕРАТОР НА КОНЕЦ ГОДИЧНОГО ПЕРИОДА ПОИСКА
	list<Svecha_30>::iterator IterSv_12m; // 
	list<Svecha_30> Sv_12m; // 

	for (Svecha_30 sv : svList)	// ПРОЛИСТАТЬ СПИСОК 30-МИН. СВЕЧЕЙ
	{
		if (sv.Mes != mes)	// ЕСЛИ СМЕНИЛСЯ МЕСЯЦ
		{
			SchMes++; // ПРИБАВИТЬ КОЛИЧЕСТВО МЕСЯЦЕВ
			mes = sv.Mes; // СОХРАНИТЬ МЕСЯЦ
			if (SchMes > KolMesyacev)	// ЕСЛИ ПРОЛИСТАНО БОЛЬШЕ МЕСЯЦЕВ ЧЕМ KolMesyacev (ДОСТАТОЧНО ДЛЯ ОПР. МИНИМАЛЬНОГО ОБЪЕМА ГОДИЧНОГО ПЕРИОДА)
			{
				copy(IterStart, IterFinish, back_inserter(Sv_12m)); // СКОПИРОВАТЬ 30-МИН. СВЕЧКИ ЗА РАССМАТРИВАЕМЫЙ ГОД
				Sv_12m.sort([](Svecha_30& s1, Svecha_30& s2) {return s1.Vol > s2.Vol; }); // ОТСОРТИРОВАТЬ ПО ОБЪЕМУ
				Sv_12m.unique([](Svecha_30& s1, Svecha_30& s2) { return s1.Vol == s2.Vol; }); // УДАЛИТЬ ПОВТОРЯЮЩИЕСЯ ОБЕМЫ
				Volume.clear(); // ОЧИСТИТЬ ПРЕДЫДУЩИЙ КОНТЕЙНЕР ОБЪЕМОВ

				for (int el : { 1, 2, 3, 4, 5, 10, 15, 20 })	// КОЛИЧЕСТВО МАКСИМАЛЬНЫХ ОБЪЕМОВ ЗА ПЕРИОД KolMesyacev
				{
					IterSv_12m = Sv_12m.begin(); // УСТАНОВИТЬ ИТЕРАТОР НА НАЧАЛО СПИСКА 30-МИН. СВЕЧЕК
					advance(IterSv_12m, el - 1); //  // СДВИГ ИТЕРАТОРА НА ОПРЕДЕЛЕННОЕ КОЛИЧЕСТВО ОБЪЕМОВ (el - 1)
					Volume.emplace(el, (*IterSv_12m).Vol); // ВСТАВИТЬ МИНИМАЛЬНЫЙ ОБЪЕМ В ПЕРЕМЕННУЮ Volume, КЛЮЧЬ - КОЛИЧЕСТВО ОБЪЕМОВ
				}
				Sv_12m.clear(); // ОЧИСТИТЬ СПИСОК 30-МИН. СВЕЧЕК
				SdvigIter(IterStart); // СДВИНУТЬ ГОДИЧНЫЙ ПЕРИОД НА ОДИН МЕСЯЦ ВПЕРЕД
			}
		}
		if (!Volume.empty())
		{
			for (auto KolVol : Volume)	// ПРОЛИСТАТЬ СПИСОК КОЛИЧЕСТВА ОБЪЕМОВ
			{
				if (sv.Vol >= KolVol.second)	// ЕСЛИ ОБЪЕМ 30-МИН. СВЕЧКИ БОЛЬШЕ МИНИМАЛЬНО НЕОБХОДИМОГО
				{
					(*tochList.emplace(KolVol.first, TochkaList).first).second.push_back(sv); // ДОБАВИТЬ 30-МИН. СВЕЧКУ В СПИСОК ТОЧЕК ВХОДА В СООТВЕТСТВУЮЩЕЕ КОЛИЧЕСТВО ОБЪЕМОВ
				}
			}
		}
		if (Date != sv.Date && SchMes > KolMesyacev)
		{
			KolDnei++; // ПОДСЧИТАТЬ КОЛ ДНЕЙ ТОРГОВЛИ
			Date = sv.Date; // 
		}
		IterFinish++; // СДВИГАТЬ ИТЕРАТОР КОНЦА ГОДИЧНОГО ПЕРИОДА
	}
}
void Sort_Po_Otkr(pair<int, list<Tochka>>& Para)	// СОРТИРОВКА СДЕЛОК ПО ДАТЕ ОТКРЫТИЯ
{
	Para.second.sort([](const Tochka& T1, const Tochka& T2)
		{
			return ((unsigned int)(T1.DateOpen) * 10000 + T1.TimeOpen < (unsigned int)(T2.DateOpen) * 10000 + T2.TimeOpen); // 
		}); // 
}
void Print(pair<int, list<Tochka>>& SpisokSdelok) // Записывает в файл все сделки
{
	if (!VseSdelki.is_open())
	{
		string FileName; // 
		FileName = "Сделки (O-"; // 
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
int FindOpen(list<Svecha>& TorgilList, Tochka& Tochka) // ПОИСК ОТКРЫТИЯ СДЕЛКИ
{
	if (Tochka.Oper == 'B')	// ЕСЛИ ОПЕРАЦИЯ ПОКУПКА
	{
		for (Svecha& God : TorgilList) // ПРОЛИСТЫВАТЬ ГОДИЧНЫЕ СВЕЧКИ ТОРГОВ
		{
			if (God.God >= Tochka.GetVremia('Г', 'S') && (abs(God.High - Tochka.PriceOpen) < 0.000001 || God.High > Tochka.PriceOpen))	// ПРОВЕРИТЬ ВРЕМЯ И МАКСИМАЛЬНУЮ ЦЕНУ СВЕЧКИ
			{
				for (Svecha& Mes : God.next)	// МЕСЯЧНЫЕ СВЕЧКИ
				{
					if (Mes.Mes >= Tochka.GetVremia('М', 'S') && (abs(Mes.High - Tochka.PriceOpen) < 0.000001 || Mes.High > Tochka.PriceOpen))	// ПРОВЕРИТЬ ВРЕМЯ И МАКСИМАЛЬНУЮ ЦЕНУ СВЕЧКИ
					{
						for (Svecha& Den : Mes.next)	// ДНЕВНЫЕ
						{
							if (Den.Den >= Tochka.GetVremia('Д', 'S') && (abs(Den.High - Tochka.PriceOpen) < 0.000001 || Den.High > Tochka.PriceOpen))	// ПРОВЕРИТЬ ВРЕМЯ И МАКСИМАЛЬНУЮ ЦЕНУ СВЕЧКИ
							{
								for (Svecha& Chas : Den.next)	// ЧАСОВЫЕ
								{
									if (Chas.Chas >= Tochka.GetVremia('Ч', 'S') && (abs(Chas.High - Tochka.PriceOpen) < 0.000001 || Chas.High > Tochka.PriceOpen))	// ПРОВЕРИТЬ ВРЕМЯ И МАКСИМАЛЬНУЮ ЦЕНУ СВЕЧКИ
									{
										for (Svecha& Minut : Chas.next)	// МИНУТНЫЕ
										{
											if (Minut.Min > Tochka.GetVremia('Т', 'S') && (abs(Minut.High - Tochka.PriceOpen) < 0.000001 || Minut.High > Tochka.PriceOpen))	// ПРОВЕРИТЬ ВРЕМЯ И МАКСИМАЛЬНУЮ ЦЕНУ СВЕЧКИ
											{
												Tochka.TikerOpen = Minut.Tiker; // ЕСЛИ НАЙДЕНА МИНУТНАЯ СВЕЧКА ДАТА КОТОРОЙ БОЛЬШЕ
												Tochka.DateOpen = Minut.Date; // ДАТЫ ТОЧКИ ВХОДА И МАКСИМАЛЬНАЯ ЦЕНА БОЛЬШЕ ЦЕНЫ 
												Tochka.TimeOpen = Minut.Time; // ОТКРЫТИЯ СДЕЛКИ, ТО ЗАПИСАТЬ ТИКЕР, ДАТУ, ВРЕМЯ, КОЭФФИЦ.
												Tochka.KoefPerOpen = Minut.KoefPer; // ПЕРЕСЧЕТА ОТКРЫТИЯ СДЕЛКИ
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
	else if (Tochka.Oper == 'S')	// ЕСЛИ ОПЕРАЦИЯ ПРОДАЖА, ТО ПРОВЕРЯТЬ МИНИМАЛЬНУЮ ЦЕНУ СВЕЧИ
	{
		for (Svecha& God : TorgilList)
		{
			if (God.God >= Tochka.GetVremia('Г', 'S') && (abs(God.Low - Tochka.PriceOpen) < 0.000001 || God.Low < Tochka.PriceOpen))
			{
				for (Svecha& Mes : God.next)
				{
					if (Mes.Mes >= Tochka.GetVremia('М', 'S') && (abs(Mes.Low - Tochka.PriceOpen) < 0.000001 || Mes.Low < Tochka.PriceOpen))
					{
						for (Svecha& Den : Mes.next)
						{
							if (Den.Den >= Tochka.GetVremia('Д', 'S') && (abs(Den.Low - Tochka.PriceOpen) < 0.000001 || Den.Low < Tochka.PriceOpen))
							{
								for (Svecha& Chas : Den.next)
								{
									if (Chas.Chas >= Tochka.GetVremia('Ч', 'S') && (abs(Chas.Low - Tochka.PriceOpen) < 0.000001 || Chas.Low < Tochka.PriceOpen))
									{
										for (Svecha& Minut : Chas.next)
										{
											if (Minut.Min > Tochka.GetVremia('Т', 'S') && (abs(Minut.Low - Tochka.PriceOpen) < 0.000001 || Minut.Low < Tochka.PriceOpen))
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
int FindKill(list<Svecha>& TorgilList, Tochka& Tochka) // ПОИСК СНЯТИЯ СДЕЛКИ
{
	if (Tochka.Oper == 'B')	// ЕСЛИ ОПЕРАЦИЯ ПОКУПКА
	{
		for (Svecha& God : TorgilList) // ПРОЛИСТЫВАТЬ ГОДИЧНЫЕ СВЕЧКИ ТОРГОВ
		{
			if (God.God >= Tochka.GetVremia('Г', 'S') && (abs(God.Low - Tochka.PriceKill) < 0.000001 || God.Low < Tochka.PriceKill))	// ПРОВЕРИТЬ ВРЕМЯ И МИНИМАЛЬНУЮ ЦЕНУ СВЕЧКИ
			{
				for (Svecha& Mes : God.next) // МЕСЯЧНЫЕ
				{
					if (Mes.Mes >= Tochka.GetVremia('М', 'S') && (abs(Mes.Low - Tochka.PriceKill) < 0.000001 || Mes.Low < Tochka.PriceKill))	// ПРОВЕРИТЬ ВРЕМЯ И МИНИМАЛЬНУЮ ЦЕНУ СВЕЧКИ
					{
						for (Svecha& Den : Mes.next) // ДНЕВНЫЕ
						{
							if (Den.Den >= Tochka.GetVremia('Д', 'S') && (abs(Den.Low - Tochka.PriceKill) < 0.000001 || Den.Low < Tochka.PriceKill))	// ПРОВЕРИТЬ ВРЕМЯ И МИНИМАЛЬНУЮ ЦЕНУ СВЕЧКИ
							{
								for (Svecha& Chas : Den.next) // ЧАСОВЫЕ
								{
									if (Chas.Chas >= Tochka.GetVremia('Ч', 'S') && (abs(Chas.Low - Tochka.PriceKill) < 0.000001 || Chas.Low < Tochka.PriceKill))	// ПРОВЕРИТЬ ВРЕМЯ И МИНИМАЛЬНУЮ ЦЕНУ СВЕЧКИ
									{
										for (Svecha& Minut : Chas.next) // МИНУТНЫЕ
										{
											if (Minut.Min > Tochka.GetVremia('Т', 'S') && (abs(Minut.Low - Tochka.PriceKill) < 0.000001 || Minut.Low < Tochka.PriceKill))	// ПРОВЕРИТЬ ВРЕМЯ И МИНИМАЛЬНУЮ ЦЕНУ СВЕЧКИ
											{
												Tochka.TikerKill = Minut.Tiker; // ЕСЛИ НАЙДЕНА МИНУТНАЯ СВЕЧКА ДАТА КОТОРОЙ БОЛЬШЕ
												Tochka.DateKill = Minut.Date; // ДАТЫ ТОЧКИ ВХОДА И МИНИМАЛЬНАЯ ЦЕНА МЕНЬШЕ ЦЕНЫ 
												Tochka.TimeKill = Minut.Time; // СНЯТИЯ СДЕЛКИ, ТО ЗАПИСАТЬ ТИКЕР, ДАТУ, ВРЕМЯ, КОЭФФИЦ.
												Tochka.KoefPerKill = Minut.KoefPer; // ПЕРЕСЧЕТА СНЯТИЯ СДЕЛКИ
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
	else if (Tochka.Oper == 'S')	// ЕСЛИ ОПЕРАЦИЯ ПРОДАЖА, ТО ПРОВЕРЯТЬ МАКСИМАЛЬНУЮ ЦЕНУ СВЕЧИ
	{
		for (Svecha& God : TorgilList)
		{
			if (God.God >= Tochka.GetVremia('Г', 'S') && (abs(God.High - Tochka.PriceKill) < 0.000001 || God.High > Tochka.PriceKill))
			{
				for (Svecha& Mes : God.next)
				{
					if (Mes.Mes >= Tochka.GetVremia('М', 'S') && (abs(Mes.High - Tochka.PriceKill) < 0.000001 || Mes.High > Tochka.PriceKill))
					{
						for (Svecha& Den : Mes.next)
						{
							if (Den.Den >= Tochka.GetVremia('Д', 'S') && (abs(Den.High - Tochka.PriceKill) < 0.000001 || Den.High > Tochka.PriceKill))
							{
								for (Svecha& Chas : Den.next)
								{
									if (Chas.Chas >= Tochka.GetVremia('Ч', 'S') && (abs(Chas.High - Tochka.PriceKill) < 0.000001 || Chas.High > Tochka.PriceKill))
									{
										for (Svecha& Minut : Chas.next)
										{
											if (Minut.Min > Tochka.GetVremia('Т', 'S') && (abs(Minut.High - Tochka.PriceKill) < 0.000001 || Minut.High > Tochka.PriceKill))
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
int FindClose(list<Svecha>& TorgilList, Tochka& Tochka) // ПОИСК ЗАКРЫТИЯ СДЕЛКИ С УБЫТКОМ
{
	if (Tochka.Oper == 'B')	// ЕСЛИ ОПЕРАЦИЯ ПОКУПКА
	{
		for (Svecha& God : TorgilList) // ПРОЛИСТЫВАТЬ ГОДИЧНЫЕ СВЕЧКИ ТОРГОВ
		{
			if (God.God >= Tochka.GetVremia('Г', 'O') && (abs(God.Low - Tochka.PriceCloseU) < 0.000001 || God.Low < Tochka.PriceCloseU))	// ПРОВЕРИТЬ ВРЕМЯ И МИНИМАЛЬНУЮ ЦЕНУ СВЕЧКИ
			{
				for (Svecha& Mes : God.next) // МЕСЯЧНЫЕ
				{
					if (Mes.Mes >= Tochka.GetVremia('М', 'O') && (abs(Mes.Low - Tochka.PriceCloseU) < 0.000001 || Mes.Low < Tochka.PriceCloseU))	// ПРОВЕРИТЬ ВРЕМЯ И МИНИМАЛЬНУЮ ЦЕНУ СВЕЧКИ
					{
						for (Svecha& Den : Mes.next) // ДНЕВНЫЕ
						{
							if (Den.Den >= Tochka.GetVremia('Д', 'O') && (abs(Den.Low - Tochka.PriceCloseU) < 0.000001 || Den.Low < Tochka.PriceCloseU))	// ПРОВЕРИТЬ ВРЕМЯ И МИНИМАЛЬНУЮ ЦЕНУ СВЕЧКИ
							{
								for (Svecha& Chas : Den.next) // ЧАСОВЫЕ
								{
									if (Chas.Chas >= Tochka.GetVremia('Ч', 'O') && (abs(Chas.Low - Tochka.PriceCloseU) < 0.000001 || Chas.Low < Tochka.PriceCloseU))	// ПРОВЕРИТЬ ВРЕМЯ И МИНИМАЛЬНУЮ ЦЕНУ СВЕЧКИ
									{
										for (Svecha& Minut : Chas.next) // МИНУТНЫЕ
										{
											if (Minut.Min > Tochka.GetVremia('Т', 'O') && (abs(Minut.Low - Tochka.PriceCloseU) < 0.000001 || Minut.Low < Tochka.PriceCloseU))	// ПРОВЕРИТЬ ВРЕМЯ И МИНИМАЛЬНУЮ ЦЕНУ СВЕЧКИ
											{
												Tochka.TikerCloseU = Minut.Tiker; // ЕСЛИ НАЙДЕНА МИНУТНАЯ СВЕЧКА ДАТА КОТОРОЙ БОЛЬШЕ
												Tochka.DateCloseU = Minut.Date; // ДАТЫ ОТКРЫТИЯ СДЕЛКИ И МИНИМАЛЬНАЯ ЦЕНА МЕНЬШЕ ЦЕНЫ 
												Tochka.TimeCloseU = Minut.Time; // ЗАКРЫТИЯ СДЕЛКИ С УБЫТКОМ, ТО ЗАПИСАТЬ ТИКЕР, ДАТУ,
												Tochka.KoefPerCloseU = Minut.KoefPer; //  ВРЕМЯ, КОЭФФИЦ. ПЕРЕСЧЕТА ЗАКРЫТИЯ СДЕЛКИ С УБЫТКОМ
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
	else if (Tochka.Oper == 'S')	// ЕСЛИ ОПЕРАЦИЯ ПРОДАЖА, ТО ПРОВЕРЯТЬ МАКСИМАЛЬНУЮ ЦЕНУ СВЕЧИ
	{
		for (Svecha& God : TorgilList)
		{
			if (God.God >= Tochka.GetVremia('Г', 'O') && (abs(God.High - Tochka.PriceCloseU) < 0.000001 || God.High > Tochka.PriceCloseU))
			{
				for (Svecha& Mes : God.next)
				{
					if (Mes.Mes >= Tochka.GetVremia('М', 'O') && (abs(Mes.High - Tochka.PriceCloseU) < 0.000001 || Mes.High > Tochka.PriceCloseU))
					{
						for (Svecha& Den : Mes.next)
						{
							if (Den.Den >= Tochka.GetVremia('Д', 'O') && (abs(Den.High - Tochka.PriceCloseU) < 0.000001 || Den.High > Tochka.PriceCloseU))
							{
								for (Svecha& Chas : Den.next)
								{
									if (Chas.Chas >= Tochka.GetVremia('Ч', 'O') && (abs(Chas.High - Tochka.PriceCloseU) < 0.000001 || Chas.High > Tochka.PriceCloseU))
									{
										for (Svecha& Minut : Chas.next)
										{
											if (Minut.Min > Tochka.GetVremia('Т', 'O') && (abs(Minut.High - Tochka.PriceCloseU) < 0.000001 || Minut.High > Tochka.PriceCloseU))
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
void PokStrat(pair<int, list<Tochka>>& SpisokSdelok) // Записывает в файл общие показатели торговли по стратегии
{
	float D_max = 0.0f;		// максимальный доход
	float Prosadka = 0.0f; 		// максимальная просадка
	float Dohod = 0.0f;		// конечный доход
	int KolSdel = 0;		// количество сделок
	float SredSdel = 0.0f; 		// средняя доходность одной сделки
	float MaxUbitSdel = 0.0f;	// максимальный убыток по сделке
	float DohodMes = 0.0f; 		// доход за месяц
	float Limit = 0.0f;		// лимит для торговли
	float DohodMesProc = 0.0f;	// доходность в процентах за месяц
	float MaxUbitSdelP = 0.0f;	// максимальный убыток по сделке в процентах
	float MaxUbitProsP = 0.0f;	// максимальный убыток по просадке в процентах
	float KoefDohod = 0.0f;		// коэффициент доходности
	float KoefPereschRez = 1.0;	// коэффициент пересчета результата

	float X = 0.0; // переменные для поиска коэффициента Пирсона
	float Y = 0.0; // 
	float SumX = 0.0; // 
	float SumY = 0.0; // 
	float MX = 0.0; // 
	float MY = 0.0; // 
	float Sum_dXdY = 0.0; // 
	float Sum_dX2 = 0.0; // 
	float Sum_dY2 = 0.0; // 
	float KoefPirson = 0.0; // 

	if (!PS.is_open())	// если файл для записи показателей не открыт
	{
		string FileName; // 
		if (Strategi == 'y')	// если нужно вывести отдельную стратегию
		{
			FileName = "Стратегия (O-"; // создать соодветствующее имя файла
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
			PS.open("Все стратегии.txt"); // если нужно вывести все стратегии
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
			<< "KoefDohod" << endl; // вывести заголовок один раз
	}

	for (Tochka& Sdelka : SpisokSdelok.second)	// пролистать все сделки по стратегии
	{
		if (Sdelka.Sostoyanie == 'I')	// если сделка исполнена
		{
			Dohod += Sdelka.Rezult; // подбить доход
			KolSdel++; // посчитать количество сделок по стратегии
			SumX = SumX + Dohod; // сумма Х для коэф. Пирсона
			SumY = SumY + KolSdel; // сумма Y для коэф. Пирсона
			if (Dohod > D_max)
			{
				D_max = Dohod; // поиск максимального дохода
			}
			if (Sdelka.Rezult < MaxUbitSdel)
			{
				MaxUbitSdel = Sdelka.Rezult; // поиск максимального убытка за сделку
			}
			if (D_max - Dohod > Prosadka)
			{
				Prosadka = D_max - Dohod; // поиск максимальной просадки
			}
		}
	}

	// РАСЧЕТ КОЭФФИЦИЕНТА КОРРЕЛЯЦИИ ПИРСОНА
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
	if ((Prosadka * 100 / DopProsadka) < (MaxUbitSdel * 100 / DopUbitSdel))	// если расчитанный от максимальной просадки лимит меньше расчитанного от максимального убытка за сделку
		Limit = MaxUbitSdel * 100 / DopUbitSdel;			// в расчет берется лимит, расчитанный от минимального убытка за сделку
	else
		Limit = Prosadka * 100 / DopProsadka;				// иначе берется лимит, расчитанный от максимальной просадки
	if ((Prosadka + GarantObesp) > Limit)					// если просадка + гарантийное обеспечение больше расчитанного лимита
		Limit = Prosadka + GarantObesp;					// взять минимально необходимый лимит от гарантийного обеспечения
	if (KolSdel)	// если есть хоть одна сделка расчитать необходимые показатели
	{
		SredSdel = Dohod / KolSdel;			// средняя доходность за сделку
		DohodMes = Dohod / KolDnei * 21;		// средний доход за месяц
		DohodMesProc = DohodMes / Limit * 100;		// доходность в процентах за месяц
		MaxUbitSdelP = MaxUbitSdel / Limit * 100;	// максимальный убыток по сделке в процентах
		MaxUbitProsP = Prosadka / Limit * 100;		// максимальный убыток по просадке в процентах
		if (DohodMes > 0)
		{
			KoefDohod = DohodMes * DohodMesProc; // коэффициент доходности
		}
		else
		{
			KoefDohod = 0; // если стратегия убыточна
		}
	}
	// вывести показатели стратегии на один контракт в файл
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

	if (Strategi == 'y')	// если нужно вывести конкретную стратегию
	{			// пересчитать показатели стратегии на лимит 100000
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
		// Вывести показатели стратегии в пересчете на 100000
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
	Tochka* TekSdelka; // переменная для хранения конкретной сделки
	pair<int, list<Tochka>> SpisokSdelok; // переменная для хранения текущего списка сделок
	for (pair<const int, list<Tochka>>& element : TMap)
	{
		for (int sniat(1); sniat <= 40; sniat += 1)			// условие снятия заявки (0.1 - 4.0%)
		{
			for (int otkr(1); otkr <= 40; otkr += 1)		// условие открытия сделки (0.1 - 4.0%)
			{
				for (int ubit(1); ubit <= 40; ubit += 1)	// условие закрытия сделки с убытком (0.1 - 4.0%)
				{
					if (Strategi == 'y')
					{	// посчитать конкретную стратегию
						SpisokSdelok.first = (*TMap.find(Polz_kolVol)).first; // скопировать список точек входа по указанному 
						SpisokSdelok.second = (*TMap.find(Polz_kolVol)).second; // в Polz_kolVol количеству объемов за год
						sniat = Polz_Sniat; // использавать показатели стратегии указанные пользователем
						otkr = Polz_otkr; // 
						ubit = Polz_ubit; // 
					}
					else
					{	// посчитать все стратегии
						SpisokSdelok.first = element.first; // скопировать список точек входа
						SpisokSdelok.second = element.second; // 
					}
					for (Tochka& Sdelka : SpisokSdelok.second)	// взять из текущего списка отдельную сделку
					{
						Sdelka.PokKill = (float)sniat / 10;	// записать в текущую сделку показатель снятия
						Sdelka.PokOpen = (float)otkr / 10;	// открытия
						Sdelka.PokClose = (float)ubit / 10;	// закрытия с убытком
						sn = floor(sniat * (Sdelka.Close - Sdelka.KoefPerS) / 1000 / ShagCeni) * ShagCeni; // расчитать величину движения рынка для снятия сделки
						ot = floor(otkr * (Sdelka.Close - Sdelka.KoefPerS) / 1000 / ShagCeni) * ShagCeni; // для открытия сделки
						ub = floor(ubit * (Sdelka.Close - Sdelka.KoefPerS) / 1000 / ShagCeni) * ShagCeni; // для закрытия с убытком
						if (Sdelka.Oper == 'B')	// если операция покупка
						{
							Sdelka.PriceOpen = Sdelka.Close + ot + ShagCeni; // расчитать цену открытия
							Sdelka.PriceKill = Sdelka.Close - sn - ShagCeni; // снятия
							Sdelka.PriceCloseU = Sdelka.Close - ub - ShagCeni; // закрытия с убытком
						}
						else if (Sdelka.Oper == 'S')	// если операция продажа
						{
							Sdelka.PriceOpen = Sdelka.Close - ot - ShagCeni; // 
							Sdelka.PriceKill = Sdelka.Close + sn + ShagCeni; // 
							Sdelka.PriceCloseU = Sdelka.Close + ub + ShagCeni; // 
						}
						FindOpen(TorgilList, Sdelka); // найти время открытия
						FindKill(TorgilList, Sdelka); // найти время снятия
						Sdelka.OprSostoyanie(); // определить состояние сделки (открыта, если снята позже открытия)
						if (Sdelka.Sostoyanie == 'I')
						{
							FindClose(TorgilList, Sdelka); // если сделка открыта, найти время закрытия с убытком
						}
					}

					Sort_Po_Otkr(SpisokSdelok); // отсортировать по дате открытия
					TekSdelka = NULL; // текущая сделка - пусто

					for (Tochka& Sdelka : SpisokSdelok.second)	// снова пролистать список сделок
					{
						if (Sdelka.Sostoyanie == 'I')	// проверять только исполненные
						{
							if (TekSdelka)	// если есть текущая сделка
							{
								if (TekSdelka->Oper == Sdelka.Oper)	// если операции текущей сделки и новой совпадают
								{	// проверить превышение позиции
									if (TekSdelka->DateCloseU != 0 && TekSdelka->GetVremia('Т', 'U') <= Sdelka.GetVremia('Т', 'O')) // текущая закрыта с убытком раньше открытия новой
									{
										TekSdelka->CalcRezult();	// подбить результат у текущей
										TekSdelka->Koment = "Убыток";	// 
										TekSdelka = &Sdelka;	// указать новую сделку как текущую
									}
									else	// текущая закрыта с убытком позже открытия новой, пропустить новую, превышена позиция
									{
										Sdelka.Sostoyanie = 'K'; // 
										Sdelka.Koment = "Превышена позиция D-"; // 
										Sdelka.Koment += to_string(TekSdelka->DateOpen); // 
										Sdelka.Koment += " T-"; // 
										Sdelka.Koment += to_string(TekSdelka->TimeOpen); // 
									}
								}
								else	// если операции текущей сделки и новой не совпадают
								{	// проверить закрытие текущей с прибылью
									switch (TekSdelka->Oper)
									{
									case 'B':	// если текущая сделка на покупку
										if (TekSdelka->DateCloseU != 0 && TekSdelka->GetVremia('Т', 'U') < Sdelka.GetVremia('Т', 'O')) // текущая закрыта с убытком раньше открытия новой
										{
											TekSdelka->CalcRezult();	// подбить результат у текущей
											TekSdelka->Koment = "Убыток"; // 
											TekSdelka = &Sdelka;	// указать новую сделку как текущую
										}
										else	// текущая закрыта с убытком позже открытия новой
										{
											if (TekSdelka->GetVremia('Т', 'U') == Sdelka.GetVremia('Т', 'O'))	// если текущая закрыта с убытком одновременно с открытием новой
											{	// проверить закрытие по цене
												if (abs(TekSdelka->PriceCloseU - Sdelka.PriceOpen) > 0.000001 && TekSdelka->PriceCloseU > Sdelka.PriceOpen) // текущая закрыта раньше новой
												{
													TekSdelka->CalcRezult();	// подбить результат у текущей
													TekSdelka->Koment = "Время сделок совпадает, убыток"; // 
													TekSdelka = &Sdelka;	// указать новую сделку как текущую
													break; // 
												}
											}
											if (abs(TekSdelka->PriceOpen - Sdelka.PriceOpen) < 0.000001 || TekSdelka->PriceOpen < Sdelka.PriceOpen)
											{	// текущая покупка открыта ниже открытия новой продажи, покупка закрыта с прибылью, записать показатели
												// новая сделка игнорируется, если она открыта в пределах цен открытия и закрытия текущей
												TekSdelka->TikerCloseP = Sdelka.TikerOpen; // 
												TekSdelka->DateCloseP = Sdelka.DateOpen; // 
												TekSdelka->TimeCloseP = Sdelka.TimeOpen; // 
												TekSdelka->PriceCloseP = Sdelka.PriceOpen; // 
												TekSdelka->KoefPerCloseP = Sdelka.KoefPerOpen; // 
												TekSdelka->PoprPriceCloseP = Sdelka.PoprPriceOpen;
												TekSdelka->CalcRezult(); // 
												TekSdelka->Koment = "Прибыль"; // 
												TekSdelka = &Sdelka;	// указать новую сделку как текущую
											}
											else	// текущая покупка открыта выше открытия новой продажи (в пределах открытие/закрытие новой), пропустить новую
											{
												Sdelka.Sostoyanie = 'K'; // 
												Sdelka.Koment = "Снята активной позицией D-"; // 
												Sdelka.Koment += to_string(TekSdelka->DateOpen); // 
												Sdelka.Koment += " T-"; // 
												Sdelka.Koment += to_string(TekSdelka->TimeOpen); // 
											}
										}
										break; // 
									case 'S':	// если текущая сделка на продажу (все наоборот)
										if (TekSdelka->DateCloseU != 0 && TekSdelka->GetVremia('Т', 'U') < Sdelka.GetVremia('Т', 'O')) // текущая закрыта раньше новой
										{
											TekSdelka->CalcRezult();	// подбить результат у текущей
											TekSdelka->Koment = "Убыток"; // 
											TekSdelka = &Sdelka;	// указать новую сделку как текущую
										}
										else	// текущая закрыта позже открытия новой
										{
											if (TekSdelka->GetVremia('Т', 'U') == Sdelka.GetVremia('Т', 'O'))
											{
												if (abs(TekSdelka->PriceCloseU - Sdelka.PriceOpen) > 0.000001 && TekSdelka->PriceCloseU < Sdelka.PriceOpen) // текущая закрыта раньше новой
												{
													TekSdelka->CalcRezult();	// подбить результат у текущей
													TekSdelka->Koment = "Время сделок совпадает, убыток"; // 
													TekSdelka = &Sdelka;	// указать новую сделку как текущую
													break; // 
												}
											}
											if (abs(TekSdelka->PriceOpen - Sdelka.PriceOpen) < 0.000001 || TekSdelka->PriceOpen > Sdelka.PriceOpen)
											{	// текущая продажа открыта выше открытия новой покупки, продажа закрыта с прибылью, записать показатели
												TekSdelka->TikerCloseP = Sdelka.TikerOpen; // 
												TekSdelka->DateCloseP = Sdelka.DateOpen; // 
												TekSdelka->TimeCloseP = Sdelka.TimeOpen; // 
												TekSdelka->PriceCloseP = Sdelka.PriceOpen; // 
												TekSdelka->KoefPerCloseP = Sdelka.KoefPerOpen; // 
												TekSdelka->PoprPriceCloseP = Sdelka.PoprPriceOpen;
												TekSdelka->CalcRezult(); // 
												TekSdelka->Koment = "Прибыль"; // 
												TekSdelka = &Sdelka;	// указать новую сделку как текущую
											}
											else	// текущая продажа открыта ниже открытия новой покупки (в пределах открытие/закрытие новой), пропустить новую
											{
												Sdelka.Sostoyanie = 'K'; // 
												Sdelka.Koment = "Снята активной позицией D-"; // 
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
								TekSdelka = &Sdelka;	// указать новую сделку как текущую
							}
						}
					}
					if (TekSdelka)	// когда пролистаны все сделки, если есть текущая сделка
					{
						if (TekSdelka->DateCloseU == 0)	// если у текущей не найдена дата закрытия с убытком
						{	// указать закрытие по последней свечке списка торгов
							TekSdelka->TikerCloseP = TorgilList.back().next.back().next.back().next.back().next.back().Tiker; // 
							TekSdelka->DateCloseP = TorgilList.back().next.back().next.back().next.back().next.back().Date; // 
							TekSdelka->TimeCloseP = TorgilList.back().next.back().next.back().next.back().next.back().Time; // 
							TekSdelka->PriceCloseP = TorgilList.back().next.back().next.back().next.back().next.back().Close; // 
							TekSdelka->KoefPerCloseP = TorgilList.back().next.back().next.back().next.back().next.back().KoefPer; // 
							TekSdelka->Koment = "Сделка не закрыта"; // 
						}
						else	// если закрытие с убытком определено, то закрыта с убытком
						{
							TekSdelka->Koment = "Убыток"; // 
						}
						TekSdelka->CalcRezult(); // подбить результат
						if (Strategi == 'y')	// если нужно вывести конкретную стратегию
						{
							Print(SpisokSdelok); // вывести все сделки по стратегии
							PokStrat(SpisokSdelok); // вывести общие показатели стратегии
							return; // 
						}
						else
						{
							PokStrat(SpisokSdelok);	// вывести общие показатели по всем стратегиям
						}
					}
				}
			}
		}
	}
}

int main()
{
	setlocale(LC_ALL, "Russian");	// числа с плавающей точкой stof обрабатывает некорректно
	// указать параметры работы программы
	cout << "Вывести сделки по стратегии? "; // 
	do
	{
		cin >> Strategi; // 
		if (Strategi != 'n' && Strategi != 'y')
		{
			cout << "Некорректный символ! "; // 
		}
		else
		{
			if (Strategi == 'y')
			{
				cout << "Введите показатель количества объемов: "; // 
				cin >> Polz_kolVol; // 
				cout << "Введите показатель снятия: "; // 
				cin >> Polz_Sniat; // 
				cout << "Введите показатель открытия: "; // 
				cin >> Polz_otkr; // 
				cout << "Введите показатель убытка: "; // 
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

	setlocale(LC_ALL, "C"); // для корректной работы с типом float
	list<Svecha> Torgi; // 
	map<const int, list<Tochka>> Tochki; // 
	Nastroika(); // настройка работы программы из файла
	ContTorgi(Torgi); // создать список торгов
	ContTochki(Torgi, Tochki); // 
	Strategiya(Tochki, Torgi); // 

	PS.close(); // 
	VseSdelki.close(); // 
	return 0; // 
}