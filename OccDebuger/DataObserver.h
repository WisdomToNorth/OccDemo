#pragma once

namespace KDebugger
{
class DataGenerator;

class DataObserver
{
public:

    DataObserver(DataGenerator* generator);
    virtual void updateData() = 0;

protected:
    DataGenerator* data_generator_;
};
}