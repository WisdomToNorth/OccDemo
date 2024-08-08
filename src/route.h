#include "Kbox.h"
#include "KPnt.h"
#include <list>
#include <algorithm>
#include <deque>

namespace KDebugger
{
struct Obj;
struct ViaFix;
struct Segment
{
    KPt ps_;
    KPt pe_;
};
struct PLine
{
    std::list<Segment *> pnts;
    Obj *parent_ = nullptr; // if have parent, it's obj's clearance
    PLine *getClr();
    std::vector<Obj> getObj();
    std::vector<Interction> getIntersex(const PLine *rhs) const;
    KPt getCenterPt() const
    {
        return KPt();
    } // center of all segment length

    enum ShovePiror // TODO: logically, this should be defined in segment
    {
        extrahigh = 3,  // fixed obj and clearance
        alwayshigh = 2, // via and clearance
        high = 1,       // edit pline
        normal = 0      // pline and pline clearance
    };
    ShovePiror piror_ = normal;
};

struct Interction
{
    Segment *seg; // segment of variable
    KPt cross_pt_;
};
struct ViaFix
{
    PLine *v_;
    PLine *f_;
    std::vector<Interction> interxs_;
    void solveVF() // v_ is changed
    {
    }
};
struct Obj
{
    PLine *clr_; // clr_->piror_ should be set
    // bool is_connected_ = false; // if false, Obj can be hug
    bool fixed_;
    ViaFix getIntersex(PLine *edit) const
    {
        ViaFix res;
        if (clr_->piror_ >= edit->piror_) // hug
        {
            res = ViaFix(edit, clr_, clr_->getIntersex(edit));
            edit->piror_ = PLine::ShovePiror::high;
            if (clr_->piror_ == edit->piror_) // hug edit and break
            {
                edit->piror_ = PLine::ShovePiror::normal;
            }
        }
        else
        {
            res = ViaFix(clr_, edit, clr_->getIntersex(edit));
            clr_->piror_ = PLine::ShovePiror::high;
        }
    }
};

void sortViaFixVec(const PLine *edit_, std::deque<ViaFix> &VFs)
{
    const KPt &midpt = edit_->getCenterPt();
    std::sort(VFs.begin(), VFs.end(), [&](const ViaFix &vf1, const ViaFix &vf2) {
        double D1 = UINT8_MAX, D2 = UINT8_MAX;
        for (const auto &interx : vf1.interxs_) D1 = std::min(D1, interx.cross_pt_.distance(midpt));
        for (const auto &interx : vf2.interxs_) D2 = std::min(D2, interx.cross_pt_.distance(midpt));
        return D1 < D2;
    });
}
void updateVFs(const std::vector<Obj> &to_check, PLine *edit_, std::deque<ViaFix> &VFs)
{
    for (auto &obj : to_check)
    {
        // if obj is in VFs
        // update intersex
        // else, new obj
        VFs.emplace_back(obj.getIntersex(edit_));
    }
    sortViaFixVec(edit_, VFs);
}

void commitToDb(){};
void solve(PLine *const edit_) // shove prefer
{
    std::vector<Obj> objs = edit_->getClr()->getObj();
    std::deque<ViaFix> VFs;
    updateVFs(objs, edit_, VFs);
    // contents will not too many.
    // todo: consider if needed to sort each time appendNew

    while (!VFs.empty())
    {
        auto VF = VFs.front();
        VFs.pop_front();
        VF.solveVF();
        // commitToDb();

        if (VF.f_ == edit_) // edit shove others
        {
            solve(VF.v_); // 这里的VF.v可以shove其他，但是不可以再次被shove
        }
        else {} // VF.v_ ==edit // edit_ meet fixed obj, hug, VFs changed;

        if (edit_->piror_ == PLine::ShovePiror::normal) return; // TODO: confirm it's right
        auto changed_new_obj = edit_->getClr()->getObj();
        updateVFs(changed_new_obj, edit_, VFs);
    }
}

void test()
{
    PLine *newline = new PLine();
    newline->piror_ = PLine::ShovePiror::high;
    solve(newline);
}
} // namespace KDebugger