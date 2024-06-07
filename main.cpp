#include <algorithm>
#include <climits>
#include <deque>
#include <iostream>
#include <numeric>
#include <queue>
#include <sstream>
#include <stack>
#include <string>
#include <thread>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <gtest/gtest.h>

#include "lc_struct.h"
#include "public_function.h"
#include "threadpool.h"

using namespace std;
using namespace LeetCode;

int task1()
{
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    return 1;
}

TEST(TestThreadPool, test1)
{
    std::cout << "hardware_concurrency: " << std::thread::hardware_concurrency() << '\n';
    rfbase::ThreadPool pool;
    for (int i = 0; i < 100; ++i)
    {
        auto res = pool.async(task1);
    }
    std::vector<std::future<int>> res;
    for (int i = 0; i < 100; ++i)
    {
        res.emplace_back(pool.async(task1));
    }
    int sum = 0;
    for (auto &i : res)
    {
        sum += i.get();
    }
    std::cout << "sum: " << sum << '\n';
    EXPECT_EQ(sum, 100);

    for (int i = 0; i < 100; ++i)
    {
        task1();
    }
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
