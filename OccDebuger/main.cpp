#include "MainWindowOcc.h"
#include <QtWidgets/QApplication>
#include <qprocess.h>
#include <iostream>


#include <vector>
#include <unordered_map>
#include <map>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    MainWindowOcc w;
    w.show();

    return a.exec();


    std::vector<std::pair<int, std::unordered_set<int>>> te;
    std::vector<std::pair<int, std::unordered_set<int>>> tt;
    auto b = te.begin();
    auto c = tt.begin();
    b;
    c += 1;
    //auto testfuc = [](const std::vector<int>& vec, std::vector<int>::iterator it,
    //    int cnt, const std::string& name)
    //{

    //    while (cnt--)
    //    {
    //        if (it == vec.end())break;
    //        std::string info = "\nname: " + name + ':' + std::to_string(*it) + '\n';
    //        std::cout << info;
    //        ++it;
    //    }
    //};
    //std::vector<int> test_vec{ 1,2,3,4,5,6,7,8,9,10,11,12,13,14 };
    //for (std::vector<int>::iterator it = test_vec.begin(); it != test_vec.end(); ++it)
    //{
    //    testfuc(test_vec, it, 3, std::to_string(*it + 1));
    //}

}
