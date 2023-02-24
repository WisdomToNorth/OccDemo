#pragma once

#include "K_box.h"

namespace KDebugger
{
class CadView;
class DataObserver;
class DataGenerator
{
public:
    DataGenerator(CadView* view) :viewer_(view) { }

    void reGenerateData(int rowcnt, int colcnt, double dis, int precision, double radius, bool same_radius);

    void viewData();

    const std::vector<KBox>& getData() { return buf_; }
    void getPtData(std::vector<KPt>&);

    void addToObserverList(DataObserver* obs);
    void notifyAll();

private:
    void generateData(std::vector<KBox>& buffer,
        int testrow, int testcol, double distance, int precision,
        double radius, bool same_radius);

private:
    CadView* viewer_;
    std::vector<KBox> buf_;
    std::vector<DataObserver*> obs_list_;
};
}
