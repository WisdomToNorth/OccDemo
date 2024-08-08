#include "DataObserver.h"

#include "DataGenerator.h"

namespace KDebugger
{
DataObserver::DataObserver(DataGenerator *generator) :
    data_generator_(generator)
{
    generator->addToObserverList(this);
}

} // namespace KDebugger