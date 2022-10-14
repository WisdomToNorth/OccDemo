#pragma once
#include <unordered_map>
#include <unordered_set>

class UnionFind
{
public:
    //如果使用vector，则必须连续编号
    std::unordered_map<int, int> parent_;
    std::unordered_map<int, int> rank_;
    std::unordered_map<int, std::unordered_set<int>> final_set_;
    int count_;

public:
    UnionFind(int n) :count_(n)
    {
        //O(N)
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

    void merge(UnionFind& rhs)
    {
        //O(N)
        ////std::cout << "#############\n";
        for (int i = 0; i < count_; ++i)
        {
            //std::cout << "##:" << this->find(i) << "  ##:" << rhs.find(i) << std::endl;
            merge(this->find(i), rhs.find(i));
        }
    }
    void printUnion()
    {
        std::cout << "#############\n";
        for (int i = 0; i < count_; ++i)
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
        //O(N)
        for (auto it = parent_.begin(); it != parent_.end(); ++it)
        {
            final_set_[find((*it).first)].insert((*it).first);
        }
    }
};