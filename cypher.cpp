#include <iostream>
#include <string>

using namespace std;

const int P10[10] = {3, 5, 2, 7, 4, 10, 1, 9, 8, 6};
const int P8[8] = {6, 3, 7, 4, 8, 5, 10, 9};
const int IP[8] = {2, 6, 3, 1, 4, 8, 5, 7};
const int EP[8] = {4, 1, 2, 3, 2, 3, 4, 1};
const string S0[4][4] = {
    {"01", "00", "11", "10"},
    {"11", "10", "01", "00"},
    {"00", "10", "01", "11"},
    {"11", "01", "11", "10"}};
const string S1[4][4] = {
    {"00", "01", "10", "11"},
    {"10", "00", "01", "11"},
    {"11", "00", "01", "00"},
    {"10", "01", "00", "11"}};
const int P4[4] = {2, 4, 3, 1};
const int IP_1[8] = {4, 1, 3, 5, 7, 2, 8, 6};

string keys[2];

string applyP10(string before)
{
    string after;
    for (int i = 0; i < 10; i++)
    {
        after.push_back(before[P10[i] - 1]);
    }
    return after;
}

string apply8(string before, const int constant[8])
{
    string after;
    for (int i = 0; i < 8; i++)
    {
        after.push_back(before[constant[i] - 1]);
    }
    return after;
}

string applyP4(string before)
{
    string after;
    for (int i = 0; i < 4; i++)
    {
        after.push_back(before[P4[i] - 1]);
    }
    return after;
}

string leftShift(string before)
{
    int length = before.length();
    return before.substr(1, length) + before.substr(0, 1);
}

string leftShift2(string before)
{
    int length = before.length();
    return before.substr(2, length) + before.substr(0, 2);
}

string firstHalf(string before)
{
    int length = before.length() / 2;
    return before.substr(0, length);
}

string secondHalf(string before)
{
    int length = before.length() / 2;
    return before.substr(length);
}

void getKeys(string key)
{
    //Start get keys
    key = applyP10(key);

    string half1 = firstHalf(key);
    string half2 = secondHalf(key);

    half1 = leftShift(half1);
    half2 = leftShift(half2);

    string join = half1 + half2;

    string key1 = apply8(join, P8);

    half1 = leftShift2(half1);
    half2 = leftShift2(half2);

    join = half1 + half2;

    string key2 = apply8(join, P8);

    keys[0] = key1;
    keys[1] = key2;
}

string XOR(string first, string second)
{
    int length = first.length();
    string result;
    for (int i = 0; i < length; i++)
    {
        if (first[i] == second[i])
        {
            result.push_back('0');
        }
        else
        {
            result.push_back('1');
        }
    }
    return result;
}

int getNumber(string binary)
{
    int res = 0;
    for (int i = 1; i >= 0; i--)
    {
        if (i == 1)
        {
            if (binary[i] == '1')
            {
                res += 1;
            }
        }
        else
        {
            if (binary[i] == '1')
            {
                res += 2;
            }
        }
    }
    return res;
}

string applyS(string before, const string S[4][4])
{
    string num1;
    num1.push_back(before[0]);
    num1.push_back(before[3]);
    string num2;
    num2.push_back(before[1]);
    num2.push_back(before[2]);

    int pos1 = getNumber(num1);
    int pos2 = getNumber(num2);

    return S[pos1][pos2];
}

string halfEncrypt(string left, string right, string key)
{

    string ExHalf2 = apply8(right, EP); //EP con la segunda mitad (8 bits)

    ExHalf2 = XOR(ExHalf2, key); //XOR con la primera key (8 bits)

    string leftExHalf2 = applyS(firstHalf(ExHalf2), S0); //(2 bits)
    cout << "S0: " << leftExHalf2 << endl;

    string rightExHalf2 = applyS(secondHalf(ExHalf2), S1); //(2 bits)
    cout << "S1: " << rightExHalf2 << endl;

    string join = applyP4(leftExHalf2 + rightExHalf2); //(4 bits)

    left = XOR(join, left); //(4 bits)

    return left + right; //(8 bits)
}

string encrypt(string key, string plainText, int decrypt)
{
    //Getting keys
    getKeys(key);

    string key1;
    string key2;
    if (decrypt)
    {
        key1 = keys[1];
        key2 = keys[0];
    }
    else
    {
        key1 = keys[0];
        key2 = keys[1];
    }

    //Start encryption

    plainText = apply8(plainText, IP); //IP

    string half1 = firstHalf(plainText);
    string half2 = secondHalf(plainText);

    string firstStep = halfEncrypt(half1, half2, key1);

    half1 = secondHalf(firstStep); //Switch
    half2 = firstHalf(firstStep);  //Switch

    string secondStep = halfEncrypt(half1, half2, key2);

    string encryptedText = apply8(secondStep, IP_1); //IP

    return encryptedText;
}

int main()
{
    string key, text;
    int decrypt;

    cout << "Input a 10-bit key: " << endl;
    cin >> key;

    cout << "Input a 8-bit text: " << endl;
    cin >> text;

    cout << "select Encrypt or Decrypt: " << endl;
    cout << "0- Encrypt: " << endl;
    cout << "1- Decrypt: " << endl;
    cin >> decrypt;

    string result = encrypt(key, text, decrypt);

    cout << "10-bit key: " << key << endl;
    cout << "8-bit text: " << text << endl;

    cout << "Encrypted Text: " << result << endl;

    return 0;
}