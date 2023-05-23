#pragma once

#include "KBox.h"

namespace KDebugger
{
class CadView;
class DataObserver;

struct HighlevelDataParameter
{
    double rand_loc_min = 0.6;
    double rand_loc_max = 0.95;
    double rand_size_min = 0.8;
    double rand_size_max = 1.2;

    double base_size = 1.0;
};

struct DataParameter
{
    HighlevelDataParameter defaultpar;
    double dis;
    int rowcnt;
    int colcnt;
    int precision;
    int seed = 1;
    bool w_h_same;
    bool rand_loc;
    bool rand_size;
};

class DataGenerator
{
public:
    DataGenerator(CadView *view) : cadview_(view)
    {
    }

    void reGenerateData(const DataParameter &param, bool _view);

    bool viewData();

    const std::vector<KBox> &getData()
    {
        return buf_;
    }
    void getPtData(std::vector<KPt> &);

    void addToObserverList(DataObserver *obs);
    void notifyAll();
    std::vector<int> getIntNumbers(int min, int max, int count);
    std::vector<double> getFourNumber(double min, double max);

private:
    void generateData(std::vector<KBox> &buffer, const DataParameter &param);
    bool checkParam(const DataParameter &param)
    {
        return true;
    }

private:
    CadView *cadview_;

    std::vector<KBox> buf_;
    std::vector<DataObserver *> obs_list_;
};
} // namespace KDebugger
