#include "define.hpp"
#include <vector>
#include <list>
#include <queue>
using namespace std;

template <class Tv, class Te>
class GraphList
{
protected:
    int _vcnt, _ecnt;                 // 顶点数，边数
    vector</**/ Vertex<Tv> /**/> V; // 一维顶点集
    vector</**/ list<Edge<Te>*> /**/> E;

public:
    GraphList();
    ~GraphList();
    void reset();
    /*顶点读写*/
    Tv& vertex(Rank i);
    Tv& operator[](Rank i);
    int inDegree(Rank i);
    VStatus& status(Rank i);
    int& dTime(Rank i);
    int& fTime(Rank i);
    int& parent(Rank i);
    int& priority(Rank i);
    int vcnt();
    int ecnt();
    /*边读写*/
    bool exists(Rank i, Rank j);
    Te& edge(Rank i, Rank j);
    EType& type(Rank i, Rank j);
    int& weight(Rank i, Rank j);
    /*动态操作*/
    void insert(Te const& edge, int w, Rank src, Rank dest); /*边的添加*/
    Te remove(Rank i, Rank j);
    void insert(Tv const& vertex); /*顶点的添加*/
    Tv remove(Rank i);
    void single_BFS(Rank curVertex, int clock/*时钟*/, void VST(Tv const& vdata));
    void BFS(Rank startVertex, void (*VST)(Tv const& vdata));
    void single_DFS(Rank curVertex, int clock/*时钟*/, void VST(Tv const& vdata));
    void DFS(Rank startVertex, void (*VST)(Tv const& vdata));
};

template <class Tv, class Te>
GraphList<Tv, Te>::GraphList() { _vcnt = _ecnt = 0; }

template <class Tv, class Te>
GraphList<Tv, Te>::~GraphList()
{
    for (Rank j = 0; j < _vcnt; ++j)
        for (auto ele : E[j]) // E[j] 是第 j 个顶点的邻居节点表，它的元素是边指针
            if (ele != NULL)
                delete ele;
}

template <class Tv, class Te>
void GraphList<Tv, Te>::reset()
{
    for (Rank i = 0; i < _vcnt; ++i)
    {
        this->status(i) = VStatus::UNDISCOVERD;
        this->dTime(i) = this->fTime(i) = -1;
        this->parent(i) = -1;
        this->priority(i) = __INT_MAX__;
        for (Rank j = 0; j < _vcnt; ++j)
            if (this->exists(i, j))
                this->type(i, j) = EType::UNDETERMINED;
    }
}

template <class Tv, class Te>
Tv& GraphList<Tv, Te>::vertex(Rank i) { return V[i].data; }

template <class Tv, class Te>
Tv& GraphList<Tv, Te>::operator[](Rank i) { return V[i].data; }

template <class Tv, class Te>
int GraphList<Tv, Te>::inDegree(Rank i) { return V[i].inDegree; }

template <class Tv, class Te>
VStatus& GraphList<Tv, Te>::status(Rank i) { return V[i].status; }

template <class Tv, class Te>
int& GraphList<Tv, Te>::dTime(Rank i) { return V[i].dTime; }

template <class Tv, class Te>
int& GraphList<Tv, Te>::fTime(Rank i) { return V[i].fTime; }

template <class Tv, class Te>
int& GraphList<Tv, Te>::parent(Rank i) { return V[i].parent; }

template <class Tv, class Te>
int& GraphList<Tv, Te>::priority(Rank i) { return V[i].priority; }

template <class Tv, class Te>
int GraphList<Tv, Te>::vcnt() { return this->_vcnt; }

template <class Tv, class Te>
int GraphList<Tv, Te>::ecnt() { return this->_ecnt; }

template <class Tv, class Te>
bool GraphList<Tv, Te>::exists(Rank i, Rank j)
{
    if (i >= _vcnt)
        return false;
    for (auto ele : E[i])   // 遍历顶点 i 的所有边
        if (ele->dest == j) // 如果存在以 j 为终点的边
            return true;
    return false;
}

template <class Tv, class Te>
Te& GraphList<Tv, Te>::edge(Rank i, Rank j)
{
    for (auto ele : E[i])      // 遍历顶点 i 的所有边
        if (ele->dest == j)    // 如果存在以 j 为终点的边
            return ele->data;  // 返回数据域
    return E[i].front()->data; // 如果找不到则默认返回该顶点的第一条边的数据域
}

template <class Tv, class Te>
EType& GraphList<Tv, Te>::type(Rank i, Rank j)
{
    for (auto ele : E[i])      // 遍历顶点 i 的所有边
        if (ele->dest == j)    // 如果存在以 j 为终点的边
            return ele->type;  // 返回边类型
    return E[i].front()->type; // 如果找不到则默认返回该顶点的第一条边的类型
}

template <class Tv, class Te>
int& GraphList<Tv, Te>::weight(Rank i, Rank j)
{
    for (auto ele : E[i])        // 遍历顶点 i 的所有边
        if (ele->dest == j)      // 如果存在以 j 为终点的边
            return ele->weight;  // 返回权重
    return E[i].front()->weight; // 如果找不到则默认返回该顶点的第一条边的权重
}

template <class Tv, class Te>
void GraphList<Tv, Te>::insert(Te const& edge, int w, Rank src, Rank dest) /*添加一条边*/
{
    Edge<Te>* new_edge = new Edge<Te>(edge, w, dest);
    this->E[src].push_back(new_edge);
    this->_ecnt++;
    this->V[src].outDegree++;
    this->V[dest].inDegree++;
}

template <class Tv, class Te>
Te GraphList<Tv, Te>::remove(Rank i, Rank j) // 删除一条边
{
    Te res = E[i].front()->data;                         // 不存在边（i,j）则默认返回第一条边
    for (auto it = E[i].begin(); it != E[i].end(); ++it) // 遍历顶点 i 的边
        if ((*it)->dest == j)                            // 找到目的为 j 的边，*it 是一个边指针
        {
            res = (*it)->data;
            delete (*it);   // 删除 *it 指针对应的数据域
            E[i].erase(it); // 从边列表中移除该边
            this->_ecnt--;
            return res; // 直接返回
        }
    return res;
}

template <class Tv, class Te>
void GraphList<Tv, Te>::insert(Tv const& vertex) // 插入一个节点
{
    this->V.push_back(vertex); // 在节点域中插入
    this->_vcnt++;
    list<Edge<Te>*>* new_list = new list<Edge<Te>*>(); // 为新节点构造一个空列表
    this->E.push_back(*new_list);
}

template <class Tv, class Te>
Tv GraphList<Tv, Te>::remove(Rank i) // 删除一个节点
{
    Tv data = V[i].data;
    V.erase(i); // 删除顶点域中的相应顶点
    this->_vcnt--;
    for (auto ele : E[i]) // E[i] 是节点 i 对应的边列表，遍历这个列表
        delete ele;       // 凡有初始化的边均应该删除
    E.erase(i);           // 删除边域中的相关边队列
    return data;
}

template <class Tv, class Te>
void GraphList<Tv, Te>::single_BFS(Rank curVertex, int clock/*时钟*/, void (*VST)(Tv const& vdata))
{
    queue<Rank> Q;
    status(curVertex) = VStatus::DISCOVERD;
    Q.push(curVertex);  // 初始化
    while (!Q.empty())
    {
        Rank curVertex = Q.front();
        Q.pop(); // 队首取出 curVertex
        dTime(curVertex) = ++clock;
        for (auto edge : E[curVertex])     // E[curVertex] 是 curVertex 的边链表，存放 Edge<Te>*
            if (status(edge->dest) == VStatus::UNDISCOVERD)
            {
                status(edge->dest) = VStatus::DISCOVERD;
                Q.push(edge->dest);
                type(curVertex, edge->dest) = EType::TREE;
                parent(edge->dest) = curVertex;
            }
            else
                type(curVertex, edge->dest) = EType::CROSS;
        VST(V[curVertex].data);
        status(curVertex) = VStatus::VISITED;
    }   // BFS 没有 fTime 的概念，它的 fTime 就是 dTime
}

template <class Tv, class Te>
void GraphList<Tv, Te>::BFS(Rank startVertex, void (*VST)(Tv const& vdata))
{
    reset();
    int clock = 0;
    Rank curVertex = startVertex;
    do if (status(curVertex) == VStatus::UNDISCOVERD) // 已经遍历过的点不会启用新一轮 bfs
        single_BFS(curVertex, clock, VST);
    while (startVertex != (curVertex = ((curVertex+1) % _vcnt)));   // 遍历所有节点
}

template <class Tv, class Te>
void GraphList<Tv, Te>::single_DFS(Rank curVertex, int clock/*时钟*/, void (*VST)(Tv const& vdata))
{
    dTime(curVertex) = ++clock; status(curVertex) = VStatus::DISCOVERD;
    VST(V[curVertex].data);
    for (auto edge : E[curVertex])     // E[curVertex] 是 curVertex 的边链表，存放 Edge<Te>*
        switch (status(edge->dest))
        {
        case VStatus::UNDISCOVERD:  // 如果下一个点是未发现的点，将边的性质设置为 TREE，表示遍历树的树枝
            type(curVertex, edge->dest) = EType::TREE;
            single_DFS(edge->dest, clock, VST); break;
        case VStatus::DISCOVERD:    // 如果下一个点是已发现的点，将点的性质设置为 BACKWARD，表示从遍历树子孙到父母的树枝
            type(curVertex, edge->dest) = EType::BACKWARD;
        case VStatus::VISITED:
            type(curVertex, edge->dest) = dTime(curVertex) < dTime(edge->dest) ? EType::FORWARD : EType::CROSS; break;
        default: break;
        }
    status(curVertex) = VStatus::VISITED;
    fTime(curVertex) = ++clock;
}

template <class Tv, class Te>
void GraphList<Tv, Te>::DFS(Rank startVertex, void (*VST)(Tv const& vdata))
{
    reset();
    int clock = 0;
    Rank curVertex = startVertex;
    do if (status(curVertex) == VStatus::UNDISCOVERD) // 已经遍历过的点不会启用新一轮 dfs
        single_DFS(curVertex, clock, VST);
    while (startVertex != (curVertex = ((curVertex+1) % _vcnt)));   // 遍历所有节点
}