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
	
	cout << "������� IP ����� ����: " << endl;
	cin >> ip_;
	cout << "������� ����� ����: " << endl;
	cin >> mask_;

	if (!isInputCorrect(ip_) || !!isInputCorrect(mask_))
	{
		cout << "������� ����� ����� ��� ����� ����!" << endl;
		return -1;
	}
	
	ip = addressToNumber(ip_);
	mask = addressToNumber(mask_);

	if (!isMaskCorrect(mask))
	{
		cout << "� ����� �������� ������������� �����!";
		return -1;
	}


	calculateNetHostID(ip, mask, ip_, mask_);
	return 0;
}



unsigned long long addressToNumber(char *address)
{
	int count = 3;
	char buf[3];       // �����
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
		for (short unit = 1; unit <= 255; unit <<= 1)	// <<	�������� ������ ������.
		{
			short ff = unit &buf;	// &	�������� ��������� ������ ��������.
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
	int points = 0;				// ���������� �����
	int	numbers = 0;			// �������� ������
	char* buff;					// ����� ��� ������ ������
	buff = new char[3];
	for (int i = 0; ip_[i] != '\0'; i++)
	{ // ��� ������ IP-������
		if (ip_[i] <= '9'&& ip_[i] >= '0')		// ���� �����
		{
			if (numbers > 3) return false;
			// ���� ������ ���� ����� � ������ � ������
			buff[numbers++] = ip_[i];
			// ����������� � �����
		}
		else
			if (ip_[i] == '.')		// ���� �����
			{
				if (atoi(buff) > 255)
					// ��������� �������� ������
					return false;
				if (numbers == 0)
					// ���� ����� ��� - ������
					return false;
				numbers = 0;
				points++;
				delete[]buff;
				buff = new char[3];
			}
			else return false;
	}
	if (points != 3)
		// ���� ���������� ����� � IP-������ �� 3 - ������
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
	cout << "�����: " << mask_ << endl;
	do
	{
		ipRes = ip % 1000;
		maskRes = mask % 1000;
		ip /= 1000;
		mask /= 1000;
		oktet[i] = ipRes & maskRes;
		i--;
	} while (mask);
	cout << "����� ����: ";
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
	cout << "����� �����: ";
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
		maskRes = ~(maskCopy2 % 1000);	// ~ �������� �������������� ��� ���������� ���������.
		ipCopy2 /= 1000;
		maskCopy2 /= 1000;
		oktet[i] = ipRes | maskRes;		// | ����������� ���������� ������� �������������
		// �������� ������������� ���������.
		i--;
	} while (maskCopy2);
	cout << "����������������� ����� (broadcast): ";
	for (int j = 0; j < 4; j++)
	{
		cout << oktet[j];
		if (j < 3)
			cout << '.';
	}
	cout << endl;
}