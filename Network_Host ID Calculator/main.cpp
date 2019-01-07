#include <stdio.h>
#include <iostream>
#include <locale>
using namespace std;


unsigned long long addressToNumber(char *address);
bool isMaskCorrect(unsigned long long mask);
bool isInputCorrect(char* ip_);
void calculateNetHostID(unsigned long long ip, unsigned long long mask, char* ip_, char* mask_);


int main(int argc, char *argv[])
{
	setlocale(LC_ALL, "rus");

	unsigned long long ip, mask;
	char *ip_, *mask_;
	ip_ = new char[16];
	mask_ = new char[16];
	
	cout << "Введить IP адрес сети: " << endl;
	cin >> ip_;
	cout << "Введить Маску сети: " << endl;
	cin >> mask_;

	if (!isInputCorrect(ip_) || !!isInputCorrect(mask_))
	{
		cout << "Неверно введён адрес или маска сети!" << endl;
		return -1;
	}
	
	ip = addressToNumber(ip_);
	mask = addressToNumber(mask_);

	if (!isMaskCorrect(mask))
	{
		cout << "В маске нарушена непрерывность битов!";
		return -1;
	}


	calculateNetHostID(ip, mask, ip_, mask_);
	return 0;
}



unsigned long long addressToNumber(char *address)
{
	int count = 3;
	char buf[3];       // октет
	unsigned long long resultNumber = 0;
	for (int j = 0, i = 0; address[i]; i++, j++)
	{
		if (address[i] >= '0' && address[i] <= '9')
		{
			count--;
			buf[j] = address[i];
		}
		if (address[i] == '.' || address[i + 1] == '\0')
		{
			for (int ii = 2; ii; ii--)
			{
				buf[ii] = buf[ii - count];
			}
			if (count)
				for (; count > 0;)
				{
					count--;
					buf[count] = '0';
				}
			resultNumber *= 1000;
			resultNumber += (unsigned long long)atoi(buf);
			j = -1;
			count = 3;
		}
	}
	return resultNumber;
}

bool isMaskCorrect(unsigned long long mask)
{
	bool result = true;
	bool one = false;
	short buf;
	int check = mask / 100000000000;
	if (!check)
		return false;
	do
	{
		buf = mask % 1000;
		mask /= 1000;
		for (short unit = 1; unit <= 255; unit <<= 1)	// <<	Операция левого сдвига.
		{
			short ff = unit &buf;	// &	Операция получения адреса операнда.
			if (ff)
				one = true;
			if (!ff && one)
			{
				result = false;
				return result;
			}
		}
	} while (mask);
	return result;
}

bool isInputCorrect(char* ip_)
{
	int points = 0;				// количество точек
	int	numbers = 0;			// значение октета
	char* buff;					// буфер для одного октета
	buff = new char[3];
	for (int i = 0; ip_[i] != '\0'; i++)
	{ // для строки IP-адреса
		if (ip_[i] <= '9'&& ip_[i] >= '0')		// если цифра
		{
			if (numbers > 3) return false;
			// если больше трех чисел в октете – ошибка
			buff[numbers++] = ip_[i];
			// скопировать в буфер
		}
		else
			if (ip_[i] == '.')		// если точка
			{
				if (atoi(buff) > 255)
					// проверить диапазон октета
					return false;
				if (numbers == 0)
					// если числа нет - ошибка
					return false;
				numbers = 0;
				points++;
				delete[]buff;
				buff = new char[3];
			}
			else return false;
	}
	if (points != 3)
		// если количество точек в IP-адресе не 3 - ошибка
		return false;
	if (numbers == 0 || numbers > 3)
		return false;
	return true;
}

void calculateNetHostID(unsigned long long ip, unsigned long long mask, char* ip_, char* mask_)
{
	unsigned long long maskCopy = mask;
	unsigned long long ipCopy = ip;
	unsigned long long maskCopy2 = mask;
	unsigned long long ipCopy2 = ip;
	unsigned char ipRes, maskRes;
	int oktet[4], i = 3;
	cout << "IP: " << ip_ << endl;
	cout << "Маска: " << mask_ << endl;
	do
	{
		ipRes = ip % 1000;
		maskRes = mask % 1000;
		ip /= 1000;
		mask /= 1000;
		oktet[i] = ipRes & maskRes;
		i--;
	} while (mask);
	cout << "Адрес сети: ";
	for (int j = 0; j < 4; j++)
	{
		cout << oktet[j];
		if (j < 3)
			cout << '.';
	}
	cout << endl;
	i = 3;

	do
	{
		ipRes = ipCopy % 1000;
		maskRes = maskCopy % 1000;
		ipCopy /= 1000;
		maskCopy /= 1000;
		oktet[i] = ipRes & ~maskRes;
		i--;
	} while (maskCopy);
	cout << "Адрес хоста: ";
	for (int j = 0; j < 4; j++)
	{
		cout << oktet[j];
		if (j < 3)
			cout << '.';
	}
	cout << endl;
	i = 3;

	do
	{
		ipRes = ipCopy2 % 1000;
		maskRes = ~(maskCopy2 % 1000);	// ~ Операция инвертирования или побитового отрицания.
		ipCopy2 /= 1000;
		maskCopy2 /= 1000;
		oktet[i] = ipRes | maskRes;		// | Поразрядная дизъюнкция битовых представлений
		// значений целочисленных операндов.
		i--;
	} while (maskCopy2);
	cout << "Широковещательный адрес (broadcast): ";
	for (int j = 0; j < 4; j++)
	{
		cout << oktet[j];
		if (j < 3)
			cout << '.';
	}
	cout << endl;
}