#include "pch.h"

#include <iostream>

#include <ctime>

#include <boost\multiprecision\cpp_int.hpp>

#include <boost/multiprecision/miller_rabin.hpp>

using boost::multiprecision::cpp_int;

using namespace boost::multiprecision;

using namespace std;





//Проверка числа на простоту с помощью теста Миллера-Рабина

bool isSimple(cpp_int number) {

	int count = 0;

	for (int i = 0; i < 4; i++) {

		if (miller_rabin_test(number, rand() % 100) == true) {

			count++;

		}

	}

	if (count == 4)

		return true;

	else return false;

}





//Алоритм Евклида, НОД

cpp_int gcd(cpp_int a, cpp_int b) {
	if (b == 0)
		return a;
	else
		return gcd(b, a % b);
}



//Шифрование

cpp_int encrypt(cpp_int m, cpp_int e, cpp_int n) {

	cpp_int result;
    result = powm(m, e, n);

	return result;

}







//Расшифровка

cpp_int decrypt(cpp_int m, cpp_int d, cpp_int n) {

	cpp_int result;
	result = powm(m, d, n);

	return result;

}



int main()

{

	

	cout << "Please wait... Key generation procces." << endl << endl;

	srand((unsigned)time(NULL));



	// Простые числа p и q

	cpp_int p_simple = 4;

	while (isSimple(p_simple) == false || p_simple < 500) {

		  p_simple = rand() % 10000;

	}



	cpp_int q_simple = 4;

	while (isSimple(q_simple) == false || q_simple < 500) {

		  q_simple = rand() % 10000;

	}





	cout << endl << "p = " << p_simple << " q = " << q_simple << endl;





	//Находим число n.

	cpp_int n = p_simple * q_simple;



	cpp_int t = (p_simple - 1)*(q_simple - 1);


	cpp_int e;

	while (e > t || gcd(e, t) != 1) {

		  e = rand() % 1000;

	}



	//Генерация числа d и проверка его на взаимопростоту

	//с числом ((p_simple-1)*(q_simple-1)).

	cpp_int d = 1000;

	cpp_int k = 1 % t;

	while (((e * d) % t) != k) {

		  d++;

	}


	
	//Сгенерированные ключи.

	cout << '{' << setw(12) << e << ',' << setw(12) << n << '}' << " - Open key" << endl;

	cout << '{' << setw(12) << d << ',' << setw(12) << n << '}' << " - Secret key" << endl << endl;





	string s;   // строка с клавиатуры.  так гораздо удобнее.



	cout << "Enter message to encrypt :" << endl;

	getline(cin, s); //ввод с клавиатуры

	int size = s.length(); // Длина сообщения


	int mod2;
	if (size % 2 == 0) { // Если количество знаков в сообщении четно, то оставляем размер массива

		mod2 = 0;

		size = size;

	}

	else {     // Если количество знаков нечетно, то увеличиваем размер на 1

		mod2 = 1;

		size = size + 1;

	}



	int sizeCrypto = size / 2; // длина шифрованного сообщения. делим на количество символов в блоке
	unsigned char *Text1 = new unsigned  char[size];  // Массив для хранения текста
	if (mod2 == 1) {

		for (int i = 0; i < size - 1; i++) {

			Text1[i] = s[i];

		}

		Text1[size - 1] = ' ';  // Так как пользователь вводит знаков на 1 меньше, чем массив, то последний элемент инициализируем пробелом

	}

	if (mod2 == 0) {

		for (int i = 0; i < size; i++) {

			Text1[i] = s[i];

		}

	}

	cout << "Text1 : ";

	for (int i = 0; i < size; i++) {      // выводим результат

		cout << Text1[i];

	}

	cout << endl;

	cout << "BlockText : ";

	long *BlockText = new long[sizeCrypto];  // группируем в  блоки по 2 байта 

	for (int i = 0; i < sizeCrypto; i++) {

		BlockText[i] = Text1[2 * i] * 256 + Text1[2 * i + 1];

		cout << BlockText[i] << " ";

	}

	cout << endl;


	//Шифрование сообщения

	cpp_int *Crypto_Text = new cpp_int[sizeCrypto];

	cout << "Encrypted Text : ";


	for (long int i = 0; i < sizeCrypto; i++) {

		Crypto_Text[i] = encrypt(BlockText[i], e, n);

		cout << Crypto_Text[i] << " ";

	}

	cout << endl;



	//Расшифровка сообщения

	cout << "Decrypted Text : ";

	cpp_int *Decrypt_Text = new cpp_int[sizeCrypto];



	for (long int i = 0; i < sizeCrypto; i++) {

		Decrypt_Text[i] = decrypt(Crypto_Text[i], d, n);

		cout << Decrypt_Text[i] << " ";

	}

	cout << endl;


	unsigned char *Text2 = new unsigned  char[size];  // Массив для хранения текста

	for (int i = 0; i < sizeCrypto; i++) {      // блоки обратно разбиваем на отдельные символы

		Text2[2 * i] = static_cast<int>(Decrypt_Text[i] / 256); // тут явное преобразование типов из cpp_int в int и затем неявное - из int в unsigned  char

		Text2[2 * i + 1] = static_cast<int>(Decrypt_Text[i] % 256);

	}

	cout << "Text2 : ";

	for (int i = 0; i < size; i++) {      // выводим результат

		cout << Text2[i];

	}

	cout << endl;


	return 0;

}
