#include <algorithm>
#include <climits>
#include <deque>
#include <iostream>
#include <numeric>
#include <queue>
#include <sstream>
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "lc_struct.h"
#include "public_function.h"

using namespace std;
using namespace LeetCode;

int main()
{
    std::vector<int> vec{1, 2, 3, 4, 5, 6, 7, 8, 9};
    // remove number which is 7's multiple
    vec.erase(std::remove_if(vec.begin(), vec.end(), [](int i) { return i % 7 == 0; }), vec.end());

    std::vector<int> vec2{1, 2, 3, 4, 5, 6, 7, 8, 9};

    // find numbers both in vec and vec2
    std::vector<int> vec3;
    std::set_intersection(vec.begin(), vec.end(), vec2.begin(), vec2.end(),
                          std::back_inserter(vec3));

    std::vector<int> vec4{1, 2, 3, 4, 5, 6, 7, 8, 9};

    // push vcet4 to vec
    vec.insert(vec.end(), vec4.begin(), vec4.end());

    return 0;
}