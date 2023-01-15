#include "MainWindowOcc.h"
#include <QtWidgets/QApplication>
#include <qprocess.h>
#include <iostream>


#include <vector>
#include <unordered_map>
#include <map>
#include <algorithm>

#include "publicClass.h"

using namespace std;

class Solution
{
public:
    vector<vector<int>> merge(vector<vector<int>>& intervals)
    {
        size_t n = intervals.size();
        sort(intervals.begin(), intervals.end(), [](vector<int>& fir, vector<int>& sec)
            {
                return fir.front() < sec.front();
            });
        vector<vector<int>> res;
        size_t i = 0;
        while (i < n)
        {
            int left = intervals[i].front();
            int right = intervals[i].back();
            while (i + 1 < n && intervals[i + 1].front() <= right)
            {
                right = max(right, intervals[i + 1].back());
                i += 1;
            }
            std::vector<int> temp{ left,right };
            res.emplace_back(temp);
            i += 1;
        }
        return res;
    }
};



int main(int argc, char* argv[])
{
    Solution s;
    vector<vector<int>> vec{ {1,3},{2,6},{8,10},{15,18} };
    vector<vector<int>> vec2{ {1,4},{0,4} };
    printvecvec(s.merge(vec));
    printvecvec(s.merge(vec2));

    //QApplication a(argc, argv);
    //MainWindowOcc w;
    //w.show();

    //return a.exec();
}
