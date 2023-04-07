#pragma once
#include <unordered_map>
#include <unordered_set>
#include <mutex>
namespace KDebugger
{
class UnionFind
{
    // std::mutex locker;
public:
    // 如果使用vector，则必须连续编号
    std::unordered_map<int, int> parent_;
    std::unordered_map<int, int> rank_;
    std::vector<std::pair<int, std::unordered_set<int>>> final_set_;
    int count_;

public:
    UnionFind(int n) :
        count_(n)
    {
        // O(N)
        for (int a = 0; a < n; ++a)
        {
            parent_[a] = a;
        }
    }

    int find(int _num)
    {
        if (_num != parent_[_num])
        {
            parent_[_num] = find(parent_[_num]);
        }
        return parent_[_num];
    }

    void merge(int _a, int _b)
    {
        int roota = find(_a);
        int rootb = find(_b);
        if (roota != rootb)
        {
            // 按秩合并
            if (rank_[roota] < rank_[rootb])
                std::swap(roota, rootb);
            parent_[rootb] = roota;
            count_--;
            // 如果秩相等，将父节点rootx秩 + 1
            if (rank_[roota] == rank_[rootb])
                rank_[roota]++;
        }
    }

    void merge(UnionFind &rhs)
    {
        // O(N)
        for (int i = 0; i < parent_.size(); ++i)
        {
            merge(this->find(i), rhs.find(i));
        }
    }
    void printUnion()
    {
        std::cout << std::endl;
        for (int i = 0; i < parent_.size(); ++i)
        {
            std::cout << "##:" << this->find(i);
        }
    }
    int isConnected(int _a, int _b)
    {
        return find(_a) == find(_b);
    }

    void update()
    {
        std::unordered_map<int, std::unordered_set<int>> final_set;
        // O(N)
        for (auto it = parent_.begin(); it != parent_.end(); ++it)
        {
            final_set[find((*it).first)].insert((*it).first);
        }
        for (auto sit = final_set.begin(); sit != final_set.end(); ++sit)
        {
            final_set_.emplace_back((*sit).first, (*sit).second);
        }
    }
};
} // namespace KDebugger