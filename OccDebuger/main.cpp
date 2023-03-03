#include "MainWindowOcc.h"
#include <QtWidgets/QApplication>
#include <qprocess.h>
#include <iostream>


#include <vector>
#include <unordered_map>
#include <map>
#include <algorithm>
#include <list>
#include "test_datastructure.h"

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    KDebugger::MainWindowOcc w;
    w.show();

    return a.exec();

}
//#include <iostream>
//#include <algorithm>   // 使用find函数需引用该库
//#include <list>
//using namespace std;
//
//void print(std::list<int>& p_list)
//{
//    for (auto i : p_list) { cout << i << ", "; }
//    cout << "\b\b " << endl;
//}
//
//int main()
//{
//    int c(5);
//    list<int> a{ 1,2,3,4,5,6,7,8,9 };
//    list<int> b;
//    list<int>::iterator it;
//    cout << "a(before splice): ";
//    print(a);
//    // find函数返回一个迭代器，
//    // 包含3个参数：前两个参数限定查找范围，均为迭代器类型，第三个参数为需查找的对象
//    it = std::find(a.begin(), a.end(), c);
//    // splice函数将a截成两段，并将后段部分拼接到b中指定位置之后，a和b均为list类型
//    // 包含4个参数：第一个参数类型为迭代器，表示拼接的位置；第二个参数为list类型的引用；
//    // 后两个参数均为迭代器类型，用于限定截断范围，其中第三个参数指向截断处的元素，该元素包含在截取范围内
//    a.splice(b.end(), b, it, a.end());
//    cout << "a(after splice): ";
//    print(a);
//    cout << "b: ";
//    print(b);
//
//    return 0;
//}