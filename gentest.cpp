#include <iostream>
#include <random>
#include <chrono>
#include <fstream>
#include <cassert>
using namespace std;

// This is a function return a random number by using the time in the pc
mt19937 rd((uint32_t)chrono::steady_clock::now().time_since_epoch().count());

// Return a interger in range [l; r]
int randInt(int l, int r)
{
    assert(l <= r);
    return uniform_int_distribution<int>(l, r)(rd);
}

void gentest(void)
{
    ofstream fout("random_R_100_C_100_K_500_len_5.input");

    int R = 100;
    int C = 100;
    int K = 500;
    fout << R << ' ' << C << '\n';
    for (int i = 1; i <= R; ++i)
    {
        for (int j = 1; j <= C; ++j)
        {
            fout << (char)randInt('a', 'z') << ' ';
        }
        fout << '\n';
    }
    fout << K << '\n';
    for (int i = 1; i <= K; ++i)
    {
        int len = randInt(1, 5);
        for (int j = 1; j <= len; ++j)
        {
            fout << (char)randInt('a', 'z');
        }
        fout << '\n';
    }
    fout.close();
}

int main()
{
    gentest();
    return 0;
}