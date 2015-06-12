#include <iostream>  
using namespace std;  
#include <stdlib.h>  
#include <conio.h>   
#include <time.h>   

#define  red  0  
#define  black 1  
#define  NUM  16  

//声明树节点结构体  
typedef struct TreeNode  
{  
    int m_value;//结点值  
    int color;//结点颜色  

    TreeNode * right;//右子树  
    TreeNode * left;//左子树  
    TreeNode * pre; //父结点  
}*RB_Tree;  

typedef struct QNode  
{  
    RB_Tree tree;  
    QNode *next;  
}*LQueuePtr;  
struct LQueue  
{  
    LQueuePtr front,rear;  
};  

//左旋  
void Left_Rotation(RB_Tree &T,RB_Tree x);  
//右旋  
void Right_Rotation(RB_Tree &T,RB_Tree x);  
//插入算法  
void TreeInsert(RB_Tree &T,RB_Tree z);//循环  
void TreeInsert_(RB_Tree &T,RB_Tree z);//递归  
//插入算法RB_Insert  
void RB_Insert(RB_Tree &T,RB_Tree x);  
//以广义表的形式输出红黑树  
void PreorderLists(RB_Tree T);  
//层序遍历红黑树  
void LevelOrder(RB_Tree T);  
//创建节点  
void CreateNode(RB_Tree &x);  


//主函数  
int main()  
{  
    RB_Tree T;  
    RB_Tree x;  

    int random;  

    T = NULL;  
    time_t   t;    
    srand((unsigned)time(&t));  
    cout<<"随机产生的16个数据为："<<endl;  
    for (int i = 0;i < NUM;i++)  
    {         
        random = rand() %100;  
        CreateNode(x);  
        cout<<random<<"  ";  
        x->m_value = random;  
        x->color = 0;  

        RB_Insert(T,x);  
    }  

    //层序遍历红黑树  
    cout<<endl<<endl<<"宽度优先顺序遍历红黑树:"<<endl;  
    LevelOrder(T);  
    cout<<endl<<endl;  

    //以广义表的形式输出红黑树  
    cout<<"以广义表的形式输出红黑树:"<<endl;  
    PreorderLists(T);  
    cout<<endl;  
}  
void CreateNode(RB_Tree &x)  
{  
    x = new TreeNode;  
    x->left = NULL;  
    x->pre = NULL;  
    x->right = NULL;  
}  
//循环插入建立排序二叉树  
void TreeInsert(RB_Tree &T,RB_Tree z)  
{  
    RB_Tree x,y;  
    y = NULL;  
    x = T;  

    while (x != NULL)  
    {  
        y = x;  
        if(z->m_value > x->m_value)x = x->right;  
        else x = x->left;  
    }  
    z->pre = y;  
    if(y == NULL)T = z;  
    else if (z->m_value < y->m_value)y->left = z;  
    else y->right = z;  
}  
//递归插入建立排序二叉树  
void TreeInsert_(RB_Tree &T,RB_Tree z)  
{  
    if(T == NULL)  
    {  
        T = new TreeNode;  
        T->m_value = z->m_value;  
        T->left = NULL;  
        T->right = NULL;  
        T->pre = NULL;  
    }  
    else if (z->m_value < T->m_value)  
    {  
        TreeInsert_(T->left,z);  
    }  
    else if (z->m_value > T->m_value)  
    {  
        TreeInsert_(T->right,z);  
    }  
}  
//插入算法RB_Insert  
void RB_Insert(RB_Tree &T,RB_Tree x)  
{  
    RB_Tree y;  
    CreateNode(y);  

    TreeInsert(T,x);  
    //  TreeInsert_(T,x);  
    x->color = red;  
    while ((x != T) && (x->pre->color == red))  
    {  
        if (x->pre == x->pre->pre->left)//父节点是祖父节点的左子树  
        {  
            y = x->pre->pre->right;  
            if (y != NULL && y->color == red)  
            {  
                x->pre->color = black;  
                y->color = black;                                                  
                x->pre->pre->color = red;  
                x = x->pre->pre;  
            }  
            else  
            {  
                if (x == x->pre->right)  
                {  
                    x = x->pre;  
                    Left_Rotation(T,x);  
                }  
                x->pre->color = black;  
                x->pre->pre->color = red;  
                Right_Rotation(T,x->pre->pre);  
            }  
        }  
        else  
        {  
            y = x->pre->pre->left;  
            if (y != NULL && y->color == red)  
            {  
                x->pre->color = black;  
                y->color = black;                                                  
                x->pre->pre->color = red;  
                x = x->pre->pre;  
            }  
            else  
            {  
                if (x == x->pre->left)  
                {  
                    x = x->pre;  
                    Right_Rotation(T,x);  
                }  
                x->pre->color = black;  
                x->pre->pre->color = red;  
                Left_Rotation(T,x->pre->pre);  
            }  
        }  
    }  
    T->color = black;  
}  
//左旋  
void Left_Rotation(RB_Tree &T,RB_Tree x)  
{  
    RB_Tree y;  
    CreateNode(y);    

    //把y的左子树作为x的右子树  
    y = x->right;  
    x->right = y->left;  
    if (y->left != NULL)  
        y->left->pre = x;  

    //把x的父节点作为y的节点  
    y->pre = x->pre;  

    if (x->pre == NULL)  
        T = y;  
    else if (x == x->pre->left)  
        x->pre->left = y;  
    else  
        x->pre->right = y;  

    y->left = x;  
    x->pre = y;  
}  
//右旋  
void Right_Rotation(RB_Tree &T,RB_Tree x)  
{  
    RB_Tree y;  
    CreateNode(y);    

    y = x->left;  
    x->left = y->right;  
    if (y->right != NULL)  
        y->right->pre = x;  
    y->pre = x->pre;  

    if (x->pre == NULL)  
        T = y;  
    else if (x == x->pre->right)  
        x->pre->right = y;  
    else  
        x->pre->left = y;  

    y->right = x;   
    x->pre = y;  
}  
//按广义表的格式输出红黑树  
void PreorderLists(RB_Tree T)  
{  
    if(T)  
    {  
        cout<<"<"<<T->m_value<<","<<T->color<<">";  
        //  visit(T->data);/*输出第一个元素*/  
        if(T->left || T->right)/*左右子树中有一个不为空*/  
        {  
            cout<<'{';  
            PreorderLists(T->left);  
            cout<<',';  
            PreorderLists(T->right);  
            cout<<'}';  
        }  
    }  
    else  
        cout<<"#";/*为空则用#代替*/  
}  
//初始化队列  
void QueueInit(LQueue &Q)  
{  
    Q.front = new QNode;  
    Q.front->next = NULL;  
    Q.rear = Q.front;  
}  
//入队  
void Enqueue(LQueue &Q,RB_Tree T)  
{  
    LQueuePtr p;  
    p = new QNode;  
    p->tree = T;  

    Q.rear->next = p;  
    p->next = NULL;  
    Q.rear = p;  
}  
bool Dequeue(LQueue &Q,RB_Tree &T)  
{  
    LQueuePtr p;  
    if(Q.front == Q.rear)return false;  

    p = Q.front;  
    Q.front = p->next;  
    T = Q.front->tree;  

    delete p;  
    return true;  
}  
//层序遍历红黑树  
void LevelOrder(RB_Tree T)  
{  
    LQueue q;  
    RB_Tree nothing = NULL;  
    QueueInit(q);  
    if(T) Enqueue(q,T);  
    //  while (Dequeue(q,T))  
    //  {  
    //      if(T)cout<<"<"<<T->m_value<<","<<T->color<<">";  
    //      else cout<<"#";  
    //   
    //      if(T->left)Enqueue(q,T->left);  
    //      else Enqueue(q,nothing);   
    //      if(T->right)Enqueue(q,T->right);  
    //      else Enqueue(q,nothing);   
    //  }  

    while (1)  
    {  
        if (Dequeue(q,T))  
        {  
            if (T)  
            {  
                cout<<"<"<<T->m_value<<","<<T->color<<">";  

                if(T->left)Enqueue(q,T->left);  
                else Enqueue(q,nothing);   
                if(T->right)Enqueue(q,T->right);  
                else Enqueue(q,nothing);  
                //  if((!T->left && T->right) || (T->left && !T->right))Enqueue(q,nothing);  
            }  
            else  
                cout<<"<#,1>";  
        }  
        else  
            break;  
    }  

    return 0;
}  

