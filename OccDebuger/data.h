#pragma once
#include <iostream>
#include <Windows.h>
#include <unordered_map>

struct KPt
{
    double x;
    double y;
    KPt(double _x, double _y) :x(_x), y(_y) {};
};

class KBox
{
public:
    KBox(double x, double y, double sizex, double sizey) :
        center_(KPt(x, y)), size_x(sizex), size_y(sizey)
    {}
    bool isOut(const KBox& rhs)
    {
        double cx = (size_x + rhs.size_x) * 0.5;
        double cy = (size_y + rhs.size_y) * 0.5;
        //std::cout << "Cx:" << cx<<std::endl;
        //std::cout << "Cy:" << cy << std::endl<<std::endl;
        if (abs(rhs.center_.x - center_.x) > cx || abs(rhs.center_.y - center_.y) > cy)
            return true;
        else return false;
    }
    void mergeTest(const KBox&)
    {
        //Sleep(1);
    }
    double X()const { return center_.x; }
    double Y()const { return center_.y; }
    double size_x;
    double size_y;
private:
    KPt center_;

};


class UnionFind {
public:
    // int n;
    std::unordered_map<int, int> father;
    void init(int n)
    {
        for (int a = 0; a < n; ++a)
        {
            father[a] = a;
        }
    }
    int find(int _num)
    {

        int a = father[_num];
        while (father[a] != a)
        {
            a = father[a];

        }
        //int temp = father[_num];
        while (_num != a)
        {
            int temp = father[_num];
            father[_num] = a;
            _num = father[temp];
        }
        return a;
    }
    int merge(int _a, int _b)
    {
        int a = find(_a);
        int b = find(_b);
        if (a != b)
        {
            father[a] = b;
        }
        return b;//·µ»Ø¸¸½Úµã
    }
    int isConnected(int _a, int _b)
    {
        if (find(_a) == find(_b))return true;
        else return false;
    }
};