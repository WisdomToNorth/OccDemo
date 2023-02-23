#pragma once

namespace KDebugger
{
class DataObserver
{
public:

    DataObserver() {}
    virtual void updateData() = 0;

};
}