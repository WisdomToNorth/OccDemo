#pragma once

#include "K_box.h"

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
    bool w_h_same;
    bool rand_loc;
    bool rand_size;

};

class DataGenerator
{
public:
    DataGenerator(CadView* view) :viewer_(view) { }

    void reGenerateData(const DataParameter& param);

    void viewData();

    const std::vector<KBox>& getData() { return buf_; }
    void getPtData(std::vector<KPt>&);

    void addToObserverList(DataObserver* obs);
    void notifyAll();

private:
    void generateData(std::vector<KBox>& buffer,
        const DataParameter& param);
    bool checkParam(const DataParameter& param) { return true; }
private:
    CadView* viewer_;
    std::vector<KBox> buf_;
    std::vector<DataObserver*> obs_list_;
};
}
