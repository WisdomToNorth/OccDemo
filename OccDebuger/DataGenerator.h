#pragma once

#include "K_box.h"

namespace KDebugger
{
class CadView;
class DataObserver;
class DataGenerator
{
public:
    DataGenerator(CadView* view) :viewer_(view) {}
    void reGenerateData(int rowcnt, int colcnt, double dis);
    void generateTestData(std::vector<KBox>& buffer,
        int testrow, int testcol, double distance);
    void viewData();

    const std::vector<KBox>& getData() { return buf_; }
    void addToObserverList(DataObserver* obs);
    void notifyAll();

private:
    CadView* viewer_;
    std::vector<KBox> buf_;
    std::vector<DataObserver*> obs_list_;
};
}
