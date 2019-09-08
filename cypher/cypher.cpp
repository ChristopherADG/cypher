#include <iostream>

using namespace std;

const int P10[10] = {3, 5, 2, 7, 4, 10, 1, 9, 8, 6};
const int P8[8] = {6, 3, 7, 4, 8, 5, 10, 9};
const int IP[8] = {2, 6, 3, 1, 4, 8, 5, 7};
const int IP1[8] = {4, 1, 3, 5, 7, 2, 8, 6};
const int EP[8] = {4, 1, 2, 3, 2, 3, 4, 1};
const int P4[4] = {2, 4, 3, 1};

int[10] aplyP10(int before[10])
{
    int after[10];
    for (int i = 0; i < 10; i++)
    {
        after[i] = before[P10[i]];
    }

    return after;
}

int main()
{
    int key[10] = {1, 1, 0, 0, 0, 1, 1, 1, 1, 0};

    key = aplyP10(key);

    for
        each(int i in key)
        {
            cout << i << "\n";
        }

    return 0;
}
