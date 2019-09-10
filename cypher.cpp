#include <iostream>
#include <string>
#include <fstream>
#include <bitset>
#include <sstream>

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

string brutalKey = "0000000000";

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

string leftShift(string before, int pos)
{
    int length = before.length();
    return before.substr(pos, length) + before.substr(0, pos);
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

    half1 = leftShift(half1, 1);
    half2 = leftShift(half2, 1);

    string join = half1 + half2;

    string key1 = apply8(join, P8);

    half1 = leftShift(half1, 2);
    half2 = leftShift(half2, 2);

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

string halfEncrypt(string left, string right, string key, int brutal)
{

    string ExHalf2 = apply8(right, EP); //EP con la segunda mitad (8 bits)

    if (!brutal)
        cout << "EP: " << ExHalf2 << endl;

    ExHalf2 = XOR(ExHalf2, key); //XOR con la primera key (8 bits)

    if (!brutal)
        cout << "XOR: " << ExHalf2 << endl;

    string leftExHalf2 = applyS(firstHalf(ExHalf2), S0); //(2 bits)
    if (!brutal)
        cout << "S0: " << leftExHalf2 << endl;

    string rightExHalf2 = applyS(secondHalf(ExHalf2), S1); //(2 bits)
    if (!brutal)
        cout << "S1: " << rightExHalf2 << endl;

    string join = applyP4(leftExHalf2 + rightExHalf2); //(4 bits)
    if (!brutal)
        cout << "P4: " << join << endl;

    left = XOR(join, left); //(4 bits)
    if (!brutal)
        cout << "XOR: " << left << endl;

    return left + right; //(8 bits)
}

string encrypt(string key, string plainText, int decrypt, int brutal)
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

    if (!brutal)
        cout << "IP: " << plainText << endl;

    string half1 = firstHalf(plainText);
    string half2 = secondHalf(plainText);

    string firstStep = halfEncrypt(half1, half2, key1, brutal);

    half1 = secondHalf(firstStep); //Switch
    half2 = firstHalf(firstStep);  //Switch

    if (!brutal)
        cout << "Switch: " << half1 + half2 << endl;

    string secondStep = halfEncrypt(half1, half2, key2, brutal);

    string encryptedText = apply8(secondStep, IP_1); //IP

    return encryptedText;
}

void brutalForce(string plaintext, string encrypted, int interacion)
{
    int find = 0;
    for (int i = stoi(brutalKey, nullptr, 2); i <= 1023; i++)
    {
        bitset<10> key(i);
        stringstream ss;
        ss << key;
        string s;
        ss >> s;
        if (encrypt(s, plaintext, 0, 1).compare(encrypted) == 0)
        {
            find = 1;
            //cout << interacion << "- " << key << endl;
            if (brutalKey.compare(s) != 0)
            {
                brutalKey = s;
            }

            break;
        }
    }

    if (!find)
    {
        cout << interacion << "- No encontrado" << endl;
    }
}

//1001100100

int main()
{
    string key, text;
    int decrypt, brutal;

    /*cout << "Select mode:" << endl;
    cout << "0- Normal: " << endl;
    cout << "1- Brutal: " << endl;
    cin >> brutal;
    */
    brutal = 1;

    if (!brutal)
    {
        cout << "Input a 10-bit key: " << endl;
        cin >> key;

        cout << "Input a 8-bit text: " << endl;
        cin >> text;

        cout << "select Encrypt or Decrypt: " << endl;
        cout << "0- Encrypt: " << endl;
        cout << "1- Decrypt: " << endl;
        cin >> decrypt;
        string result = encrypt(key, text, decrypt, brutal);

        cout << "10-bit key: " << key << endl;
        cout << "8-bit text: " << text << endl;
        if (decrypt)
        {
            cout << "Decrypted Text: " << result << endl;
        }
        else
        {
            cout << "Encrypted Text: " << result << endl;
        }
    }
    else
    {
        ifstream file("brutalFile.txt");
        string str;
        int interacion = 1;
        while (std::getline(file, str))
        {
            brutalForce(str.substr(0, 8), str.substr(9), interacion);

            interacion++;
        }
        cout << "Brutal Key: " << brutalKey << endl;
    }

    return 0;
}
