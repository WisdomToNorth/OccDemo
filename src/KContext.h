#pragma once

#include <AIS_InteractiveContext.hxx>
#include <vector>

namespace KDebugger
{
class K_Context : public AIS_InteractiveContext
{
public:
    K_Context(const Handle(V3d_Viewer) & MainViewer) :
        AIS_InteractiveContext(MainViewer)
    {}
    void addToSelected(Handle(SelectMgr_EntityOwner) obj)
    {
        selected_.emplace_back(obj);
    }
    void clearSelectedVec()
    {
        selected_.clear();
    }
    Handle(SelectMgr_EntityOwner) getSelected()
    {
        if (selected_.empty())
            return nullptr;
        else
            return selected_.front();
    }

private:
    std::vector<Handle(SelectMgr_EntityOwner)> selected_;
};
} // namespace KDebugger