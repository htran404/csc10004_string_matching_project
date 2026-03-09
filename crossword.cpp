#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <chrono>
#include <fstream>
#include <queue>
#include <iomanip>
#include <algorithm>

using namespace std;

// Get the input from the input file
void input(ifstream &fin, int &R, int &C, vector<vector<char>> &grid, int &K, vector<string> &word)
{
    fin >> R >> C;

    grid.assign(R, vector<char>(C));

    for (int i = 0; i < R; ++i)
    {
        for (int j = 0; j < C; ++j)
        {
            fin >> grid[i][j];
        }
    }

    fin >> K;

    word.assign(K, "");

    for (int i = 0; i < K; ++i)
    {
        fin >> word[i];
    }
}

namespace bruteForce
{
    long long countComparisions = 0;

    vector<int> compare(string T, string P)
    {
        int n = T.length();
        int m = P.length();

        vector<int> res;

        for (int s = 0; s <= n - m; ++s)
        {
            bool match = true;

            for (int i = 0; i < m; ++i)
            {
                ++countComparisions;

                if (P[i] != T[s + i])
                {
                    match = false;
                    break;
                }
            }

            if (match == true)
            {
                res.push_back(s);
            }
        }

        return res;
    }

    void main(int R, int C, const vector<vector<char>> &grid, int K, const vector<string> &word, const string &outputFile)
    {
        countComparisions = 0;

        ofstream fout(outputFile);
        double totalTime = 0;

        vector<vector<pair<pair<int, int>, pair<int, int>>>> answer(K);

        // Horizontal
        // Cut the whole row of characters for comparision
        // T is the main T we will need to find the Pattern (P)
        for (int k = 0; k < K; ++k)
        {
            string P = word[k];
            for (int r = 0; r < R; ++r)
            {
                string T = "";
                for (int c = 0; c < C; ++c)
                {
                    T += grid[r][c];
                }
                auto start = chrono::high_resolution_clock::now();

                vector<int> temp = compare(T, P);

                auto end = chrono::high_resolution_clock::now();

                totalTime += chrono::duration<double, milli>(end - start).count();
                for (int cur : temp)
                {
                    answer[k].push_back(make_pair(make_pair(r, cur), make_pair(r, cur + P.length() - 1)));
                }
            }
        }

        // Vertical
        // Cut the whole column for comparision
        // T is the main T we will need to find the Pattern (P)
        for (int k = 0; k < K; ++k)
        {
            string P = word[k];
            for (int c = 0; c < C; ++c)
            {
                string T = "";
                for (int r = 0; r < R; ++r)
                {
                    T += grid[r][c];
                }
                auto start = chrono::high_resolution_clock::now();

                vector<int> temp = compare(T, P);

                auto end = chrono::high_resolution_clock::now();

                totalTime += chrono::duration<double, milli>(end - start).count();
                for (int cur : temp)
                {
                    answer[k].push_back(make_pair(make_pair(cur, c), make_pair(cur + P.length() - 1, c)));
                }
            }
        }

        for (int k = 0; k < K; ++k)
        {
            fout << word[k] << ": ";
            if (answer[k].empty())
            {
                fout << "not found\n";
            }
            else
            {
                for (int i = 0; i < (int)answer[k].size(); ++i)
                {
                    pair<pair<int, int>, pair<int, int>> p = answer[k][i];
                    fout << "(" << p.first.first << ", "
                         << p.first.second << ") -> ("
                         << p.second.first << ", "
                         << p.second.second << ")" << "; ";
                }
                fout << "\n";
            }
        }

        fout << "\n--------------------------------\n";
        fout << "Algorithm: Brute-Force\n";
        fout << "Comparisons: " << countComparisions << "\n";
        fout << fixed << setprecision(2) << "Execution Time: " << totalTime << " ms\n";

        fout.close();
    }
}

namespace rabinKarp
{
    long long countComparisions = 0;

    vector<int> compare(string T, string P, int d, int q)
    {
        int n = T.length();
        int m = P.length();

        vector<int> res;

        int h = 1;
        for (int i = 0; i < m - 1; ++i)
        {
            h = (1LL * h * d) % q;
        }

        int p = 0;
        int t = 0;

        for (int i = 0; i < m; ++i)
        {
            p = (1LL * d * p + P[i]) % q;
            t = (1LL * d * t + T[i]) % q;
        }

        for (int s = 0; s <= n - m; ++s)
        {
            if (p == t)
            {
                bool match = true;

                for (int j = 0; j < m; ++j)
                {
                    ++countComparisions;

                    if (T[s + j] != P[j])
                    {
                        match = false;
                        break;
                    }
                }

                if (match == true)
                {
                    res.push_back(s);
                }
            }
            if (s < n - m)
            {
                t = (1LL * d * (t - 1LL * T[s] * h) + T[s + m]) % q;
            }
            if (t < 0)
            {
                t += q;
            }
        }

        return res;
    }

    void main(int R, int C, const vector<vector<char>> &grid, int K, const vector<string> &word, const string &outputFile)
    {
        countComparisions = 0;

        ofstream fout(outputFile);
        double totalTime = 0;

        vector<vector<pair<pair<int, int>, pair<int, int>>>> answer(K);

        const int d = 26;
        const int q = 1e9 + 7;

        // Horizontal
        // Cut the whole row of characters for comparision
        // T is the main T we will need to find the Pattern (P)
        for (int k = 0; k < K; ++k)
        {
            string P = word[k];
            for (int r = 0; r < R; ++r)
            {
                string T = "";
                for (int c = 0; c < C; ++c)
                {
                    T += grid[r][c];
                }

                auto start = chrono::high_resolution_clock::now();

                vector<int> temp = compare(T, P, d, q);

                auto end = chrono::high_resolution_clock::now();

                totalTime += chrono::duration<double, milli>(end - start).count();

                for (int cur : temp)
                {
                    answer[k].push_back(make_pair(make_pair(r, cur), make_pair(r, cur + P.length() - 1)));
                }
            }
        }

        // Vertical
        // Cut the whole column for comparision
        // T is the main T we will need to find the Pattern (P)
        for (int k = 0; k < K; ++k)
        {
            string P = word[k];
            for (int c = 0; c < C; ++c)
            {
                string T = "";
                for (int r = 0; r < R; ++r)
                {
                    T += grid[r][c];
                }

                auto start = chrono::high_resolution_clock::now();

                vector<int> temp = compare(T, P, d, q);

                auto end = chrono::high_resolution_clock::now();

                totalTime += chrono::duration<double, milli>(end - start).count();

                for (int cur : temp)
                {
                    answer[k].push_back(make_pair(make_pair(cur, c), make_pair(cur + P.length() - 1, c)));
                }
            }
        }

        for (int k = 0; k < K; ++k)
        {
            fout << word[k] << ": ";
            if (answer[k].empty())
            {
                fout << "not found\n";
            }
            else
            {
                for (int i = 0; i < (int)answer[k].size(); ++i)
                {
                    pair<pair<int, int>, pair<int, int>> p = answer[k][i];
                    fout << "(" << p.first.first << ", "
                         << p.first.second << ") -> ("
                         << p.second.first << ", "
                         << p.second.second << ")" << "; ";
                }
                fout << "\n";
            }
        }

        fout << "\n--------------------------------\n";
        fout << "Algorithm: Rabin-Karp\n";
        fout << "Comparisons: " << countComparisions << "\n";
        fout << fixed << setprecision(2) << "Execution Time: " << totalTime << " ms\n";

        fout.close();
    }
}

namespace knuthMorrisPratt
{
    long long countComparisions = 0;

    vector<int> kmpPrefixGenerator(string P)
    {
        int m = P.length();

        vector<int> pi(m);

        pi[0] = 0;
        int k = 0;
        for (int i = 1; i < m; ++i)
        {
            while (k > 0 && P[k] != P[i])
            {
                k = pi[k - 1];
            }

            if (P[k] == P[i])
            {
                ++k;
            }

            pi[i] = k;
        }

        return pi;
    }

    vector<int> compare(string T, string P)
    {
        int n = T.length();
        int m = P.length();

        vector<int> pi = kmpPrefixGenerator(P);
        vector<int> res;

        int q = 0;

        for (int i = 0; i < n; ++i)
        {
            while (q > 0 && P[q] != T[i])
            {
                ++countComparisions;
                q = pi[q - 1];
            }

            ++countComparisions;

            if (P[q] == T[i])
            {

                ++q;
            }

            if (q == m)
            {
                res.push_back(i - m + 1);
                q = pi[q - 1];
            }
        }

        return res;
    }

    void main(int R, int C, const vector<vector<char>> &grid, int K, const vector<string> &word, const string &outputFile)
    {
        countComparisions = 0;

        ofstream fout(outputFile);
        double totalTime = 0;

        vector<vector<pair<pair<int, int>, pair<int, int>>>> answer(K);

        // Horizontal
        // Cut the whole row of characters for comparision
        // T is the main T we will need to find the Pattern (P)
        for (int k = 0; k < K; ++k)
        {
            string P = word[k];
            for (int r = 0; r < R; ++r)
            {
                string T = "";
                for (int c = 0; c < C; ++c)
                {
                    T += grid[r][c];
                }

                auto start = chrono::high_resolution_clock::now();

                vector<int> temp = compare(T, P);

                auto end = chrono::high_resolution_clock::now();

                totalTime += chrono::duration<double, milli>(end - start).count();

                for (int cur : temp)
                {
                    answer[k].push_back(make_pair(make_pair(r, cur), make_pair(r, cur + P.length() - 1)));
                }
            }
        }

        // Vertical
        // Cut the whole column for comparision
        // T is the main T we will need to find the Pattern (P)
        for (int k = 0; k < K; ++k)
        {
            string P = word[k];
            for (int c = 0; c < C; ++c)
            {
                string T = "";
                for (int r = 0; r < R; ++r)
                {
                    T += grid[r][c];
                }

                auto start = chrono::high_resolution_clock::now();

                vector<int> temp = compare(T, P);

                auto end = chrono::high_resolution_clock::now();

                totalTime += chrono::duration<double, milli>(end - start).count();

                for (int cur : temp)
                {
                    answer[k].push_back(make_pair(make_pair(cur, c), make_pair(cur + P.length() - 1, c)));
                }
            }
        }

        for (int k = 0; k < K; ++k)
        {
            fout << word[k] << ": ";
            if (answer[k].empty())
            {
                fout << "not found\n";
            }
            else
            {
                for (int i = 0; i < (int)answer[k].size(); ++i)
                {
                    pair<pair<int, int>, pair<int, int>> p = answer[k][i];
                    fout << "(" << p.first.first << ", "
                         << p.first.second << ") -> ("
                         << p.second.first << ", "
                         << p.second.second << ")" << "; ";
                }
                fout << "\n";
            }
        }

        fout << "\n--------------------------------\n";
        fout << "Algorithm: Knuth-Morris-Pratt\n";
        fout << "Comparisons: " << countComparisions << "\n";
        fout << fixed << setprecision(2) << "Execution Time: " << totalTime << " ms\n";

        fout.close();
    }
};

namespace boyerMoore
{
    long long countComparisions = 0;
    const int ALPHABET = 26;

    vector<int> compare(string T, string P)
    {
        int n = T.length();
        int m = P.length();

        vector<int> last(ALPHABET, -1);

        for (int i = 0; i < m; ++i)
        {
            last[P[i] - 'a'] = i;
        }

        vector<int> res;

        int s = 0;

        while (s <= n - m)
        {
            int j = m - 1;

            while (j >= 0)
            {
                ++countComparisions;

                if (P[j] == T[s + j])
                {
                    --j;
                }
                else
                {
                    break;
                }
            }

            if (j < 0)
            {
                res.push_back(s);

                if (s + m < n)
                {
                    s += m - last[T[s + m] - 'a'];
                }
                else
                {
                    s += 1;
                }
            }
            else
            {
                s += max(1, j - last[T[s + j] - 'a']);
            }
        }

        return res;
    }

    void main(int R, int C, const vector<vector<char>> &grid, int K, const vector<string> &word, const string &outputFile)
    {
        countComparisions = 0;

        ofstream fout(outputFile);
        double totalTime = 0;

        vector<vector<pair<pair<int, int>, pair<int, int>>>> answer(K);

        // Horizontal
        // Cut the whole row of characters for comparision
        // T is the main T we will need to find the Pattern (P)
        for (int k = 0; k < K; ++k)
        {
            string P = word[k];
            for (int r = 0; r < R; ++r)
            {
                string T = "";
                for (int c = 0; c < C; ++c)
                {
                    T += grid[r][c];
                }

                auto start = chrono::high_resolution_clock::now();

                vector<int> temp = compare(T, P);

                auto end = chrono::high_resolution_clock::now();

                totalTime += chrono::duration<double, milli>(end - start).count();

                for (int cur : temp)
                {
                    answer[k].push_back(make_pair(make_pair(r, cur), make_pair(r, cur + P.length() - 1)));
                }
            }
        }

        // Vertical
        // Cut the whole column for comparision
        // T is the main T we will need to find the Pattern (P)
        for (int k = 0; k < K; ++k)
        {
            string P = word[k];
            for (int c = 0; c < C; ++c)
            {
                string T = "";
                for (int r = 0; r < R; ++r)
                {
                    T += grid[r][c];
                }

                auto start = chrono::high_resolution_clock::now();

                vector<int> temp = compare(T, P);

                auto end = chrono::high_resolution_clock::now();

                totalTime += chrono::duration<double, milli>(end - start).count();

                for (int cur : temp)
                {
                    answer[k].push_back(make_pair(make_pair(cur, c), make_pair(cur + P.length() - 1, c)));
                }
            }
        }

        for (int k = 0; k < K; ++k)
        {
            fout << word[k] << ": ";
            if (answer[k].empty())
            {
                fout << "not found\n";
            }
            else
            {
                for (int i = 0; i < (int)answer[k].size(); ++i)
                {
                    pair<pair<int, int>, pair<int, int>> p = answer[k][i];
                    fout << "(" << p.first.first << ", "
                         << p.first.second << ") -> ("
                         << p.second.first << ", "
                         << p.second.second << ")" << "; ";
                }
                fout << "\n";
            }
        }

        fout << "\n--------------------------------\n";
        fout << "Algorithm: Boyer-Moore\n";
        fout << "Comparisons: " << countComparisions << "\n";
        fout << fixed << setprecision(2) << "Execution Time: " << totalTime << " ms\n";

        fout.close();
    }
};

namespace ahoCorasick
{
    long long countComparisions = 0;

    const int MAX_STATES = 500 * 5 + 10;
    const int MAX_CHARS = 26;

    int Out[MAX_STATES];
    int FF[MAX_STATES];
    int GF[MAX_STATES][MAX_CHARS];

    int BuildMatchingMachine(const vector<string> &words, char lowestChar = 'a', char highestChar = 'z')
    {
        memset(Out, 0, sizeof Out);
        memset(FF, -1, sizeof FF);
        memset(GF, -1, sizeof GF);

        int states = 1;

        for (int i = 0; i < words.size(); ++i)
        {
            const string &keyword = words[i];
            int currentState = 0;

            for (int j = 0; j < keyword.size(); ++j)
            {
                int c = keyword[j] - lowestChar;

                if (GF[currentState][c] == -1)
                {
                    GF[currentState][c] = states++;
                }

                currentState = GF[currentState][c];
            }

            Out[currentState] |= (1 << i);
        }

        for (int c = 0; c < MAX_CHARS; ++c)
        {
            if (GF[0][c] == -1)
            {
                GF[0][c] = 0;
            }
        }

        queue<int> q;

        for (int c = 0; c <= highestChar - lowestChar; ++c)
        {
            if (GF[0][c] != -1 && GF[0][c] != 0)
            {
                FF[GF[0][c]] = 0;
                q.push(GF[0][c]);
            }
        }

        while (!q.empty())
        {
            int state = q.front();
            q.pop();

            for (int c = 0; c <= highestChar - lowestChar; ++c)
            {
                if (GF[state][c] != -1)
                {
                    int failure = FF[state];

                    while (GF[failure][c] == -1)
                    {
                        failure = FF[failure];
                    }

                    failure = GF[failure][c];

                    FF[GF[state][c]] = failure;
                    Out[GF[state][c]] |= Out[failure];

                    q.push(GF[state][c]);
                }
            }
        }

        return states;
    }

    int FindNextState(int currentState, char nextInput, char lowestChar = 'a')
    {
        int answer = currentState;
        int c = nextInput - lowestChar;

        while (GF[answer][c] == -1)
        {
            answer = FF[answer];
        }

        return GF[answer][c];
    }

    vector<pair<int, int>> compare(string T, const vector<string> &words)
    {
        BuildMatchingMachine(words);

        int currentState = 0;

        vector<pair<int, int>> res;

        for (int i = 0; i < T.length(); ++i)
        {
            ++countComparisions;

            currentState = FindNextState(currentState, T[i]);

            if (Out[currentState] == 0)
            {
                continue;
            }

            for (int j = 0; j < words.size(); ++j)
            {
                if (Out[currentState] & (1 << j))
                {
                    int start = i - words[j].length() + 1;
                    res.push_back({j, start});
                }
            }
        }

        return res;
    }

    void main(int R, int C, const vector<vector<char>> &grid, int K, const vector<string> &word, const string &outputFile)
    {
        countComparisions = 0;

        ofstream fout(outputFile);

        double totalTime = 0;

        vector<vector<pair<pair<int, int>, pair<int, int>>>> answer(K);

        // Horizontal
        // Cut the whole column for comparision
        // T is the main T we will need to find the Pattern (P)
        // With Aho-Corasick we should use to find multiple pattern instead using to find only one patter at a time
        for (int r = 0; r < R; ++r)
        {
            string T = "";

            for (int c = 0; c < C; ++c)
            {
                T += grid[r][c];
            }

            auto startTime = chrono::high_resolution_clock::now();

            vector<pair<int, int>> temp = compare(T, word);

            auto endTime = chrono::high_resolution_clock::now();

            totalTime += chrono::duration<double, milli>(endTime - startTime).count();

            for (auto &match : temp)
            {
                int id = match.first;
                int cur = match.second;

                int endPos = cur + word[id].length() - 1;

                answer[id].push_back({{r, cur}, {r, endPos}});
            }
        }

        // Vertical
        // Cut the whole row of characters for comparision
        // T is the main T we will need to find the Pattern (P)
        // With Aho-Corasick we should use to find multiple pattern instead using to find only one patter at a time
        for (int c = 0; c < C; ++c)
        {
            string T = "";

            for (int r = 0; r < R; ++r)
            {
                T += grid[r][c];
            }

            auto startTime = chrono::high_resolution_clock::now();

            vector<pair<int, int>> temp = compare(T, word);

            auto endTime = chrono::high_resolution_clock::now();

            totalTime += chrono::duration<double, milli>(endTime - startTime).count();

            for (auto &match : temp)
            {
                int id = match.first;
                int cur = match.second;

                int endPos = cur + word[id].length() - 1;

                answer[id].push_back({{cur, c}, {endPos, c}});
            }
        }

        for (int k = 0; k < K; ++k)
        {
            fout << word[k] << ": ";

            if (answer[k].empty())
            {
                fout << "not found\n";
            }
            else
            {
                for (auto &p : answer[k])
                {
                    fout << "(" << p.first.first << ", "
                         << p.first.second << ") -> ("
                         << p.second.first << ", "
                         << p.second.second << "); ";
                }

                fout << "\n";
            }
        }

        fout << "\n--------------------------------\n";
        fout << "Algorithm: Aho-Corasick\n";
        fout << "Comparisons: " << countComparisions << "\n";
        fout << fixed << setprecision(2) << "Execution Time: " << totalTime << " ms\n";

        fout.close();
    }
};

int main(int argc, char *argv[])
{
    string algorithm = argv[2];
    string inputFile = argv[4];
    string outputFile = argv[6];

    ifstream fin(inputFile);

    int R, C, K;
    vector<vector<char>> grid;
    vector<string> word;

    input(fin, R, C, grid, K, word);
    fin.close();

    if (algorithm == "bf")
    {
        return bruteForce::main(R, C, grid, K, word, outputFile), 0;
    }
    if (algorithm == "rk")
    {
        return rabinKarp::main(R, C, grid, K, word, outputFile), 0;
    }
    if (algorithm == "kmp")
    {
        return knuthMorrisPratt::main(R, C, grid, K, word, outputFile), 0;
    }
    if (algorithm == "bm")
    {
        return boyerMoore::main(R, C, grid, K, word, outputFile), 0;
    }
    if (algorithm == "ac")
    {
        return ahoCorasick::main(R, C, grid, K, word, outputFile), 0;
    }
    return 0;
}