#include <iostream>
#include <string>
#include <unordered_map>
#include <stdio.h>


using namespace std;


class Solution {
    public:
        int numTilePossibilities(string tiles) {
            unordered_map<char, int> nm;

            for (const auto ch: tiles) {
                if (nm.find(ch) == nm.end())
                {
                    nm[ch] = 1;
                }
                else
                    nm[ch]++;
            } 

            return dfs(nm);
        }

        int dfs(unordered_map<char, int> & nm)
        {
            int count = 0;


            for (auto ii: nm) {
                //printf("before1 size:%d count:%d, %c, %d\n", nm.size(), count, ii.first, ii.second);
                if (ii.second == 0) {
                    continue;
                }
                count++;
                //ii.second--;  error
                nm[ii.first]--;
                printf("before2 size:%d count:%d, %c, %d\n", nm.size(), count, ii.first, ii.second);
                count += dfs(nm);
                nm[ii.first]++;
                //ii.second += 1; //error
                printf("after size:%d count:%d, %c, %d\n", nm.size(), count, ii.first, ii.second);
            }

            return count;
        }
};


int main(int argc, char *argv[])
{
    Solution s;

    cout << s.numTilePossibilities("AAABBC") << '\n';

    return 0;
}
