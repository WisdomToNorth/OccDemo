#include "MainWindowOcc.h"
#include <QtWidgets/QApplication>
#include <iostream>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    KDebugger::MainWindowOcc w;
    w.show();

    return a.exec();

}
#include "lc_struct.h"
using namespace KDebugger;
using namespace std;
typedef TreeNode BinaryTree;

BinaryTree* sortedListToBST(ListNode*& list, int start, int end)
{
    if (start > end) return NULL;
    // same as (start+end)/2, avoids overflow
    int mid = start + (end - start) / 2;
    BinaryTree* leftChild = sortedListToBST(list, start, mid - 1);
    BinaryTree* parent = new BinaryTree(list->val);
    parent->left_ = leftChild;
    list = list->next;
    parent->right_ = sortedListToBST(list, mid + 1, end);
    return parent;
}

BinaryTree* sortedListToBST(ListNode* head, int n)
{
    return sortedListToBST(head, 0, n - 1);
}
