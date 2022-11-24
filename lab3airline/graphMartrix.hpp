#include "define.hpp"
#include <vector>
#include <queue>
using namespace std;

//====================图定义========================//

template <class Tv, class Te>
class GraphMatrix
{
protected:
    int _m_vcnt, _m_ecnt;                         // 顶点数，边数
    vector</**/ Vertex<Tv> /**/> m_V;         // 一维顶点集
    vector</**/ vector<Edge<Te>*> /**/> m_E; // 二维邻接矩阵（边集）
    void m_reset();       // 重置所有相关值（以便反复调用遍历接口）

public:
    GraphMatrix();
    ~GraphMatrix();
    /*顶点读写*/
    Tv& m_vertex(Rank i);
    int m_inDegree(Rank i);
    VStatus& m_status(Rank i);
    int& m_dTime(Rank i);
    int& m_fTime(Rank i);
    int& m_parent(Rank i);
    int& m_priority(Rank i);
    int m_vcnt();
    int m_ecnt();
    /*边读写*/
    bool m_exists(Rank i, Rank j);
    Te& m_edge(Rank i, Rank j);
    EType& m_type(Rank i, Rank j);
    int& m_weight(Rank i, Rank j);
    /*动态操作*/
    void m_insert(Te const& edge, int w, Rank i, Rank j); /*边的添加*/
    Te m_remove(Rank i, Rank j);
    void m_insert(Tv const& vertex);
    Tv m_remove(Rank i);
    void m_single_BFS(Rank curVertex, int clock/*时钟*/, void VST(Tv const& vdata));
    void m_BFS(Rank startVertex, void (*VST)(Tv const& vdata));
    void m_single_DFS(Rank curVertex, int clock/*时钟*/, void VST(Tv const& vdata));
    void m_DFS(Rank startVertex, void (*VST)(Tv const& vdata));
};

template <class Tv, class Te>
GraphMatrix<Tv, Te>::GraphMatrix() { _m_vcnt = _m_ecnt = 0; }

template <class Tv, class Te>
GraphMatrix<Tv, Te>::~GraphMatrix()
{
    for (Rank j = 0; j < _m_vcnt; j++)
        for (Rank k = 0; k < _m_vcnt; k++)
            if (m_E[j][k] != NULL)
                delete m_E[j][k];
}

template <class Tv, class Te>
void GraphMatrix<Tv, Te>::m_reset()
{
    for (Rank i = 0; i < _m_vcnt; ++i)
    {
        m_status(i) = VStatus::UNDISCOVERD;
        m_dTime(i) = m_fTime(i) = -1;
        m_parent(i) = -1; m_priority(i) = __INT_MAX__;
        for (Rank j = 0; j < _m_vcnt; ++j)
            if (m_exists(i, j))
                m_type(i, j) = EType::UNDETERMINED;
    }
}

template <class Tv, class Te>
inline Tv& GraphMatrix<Tv, Te>::m_vertex(Rank i) { return m_V[i].data; }

template <class Tv, class Te>
inline int GraphMatrix<Tv, Te>::m_inDegree(Rank i) { return m_V[i].inDegree; }

template <class Tv, class Te>
inline VStatus& GraphMatrix<Tv, Te>::m_status(Rank i) { return m_V[i].status; }

template <class Tv, class Te>
inline int& GraphMatrix<Tv, Te>::m_dTime(Rank i) { return m_V[i].dTime; }

template <class Tv, class Te>
inline int& GraphMatrix<Tv, Te>::m_fTime(Rank i) { return m_V[i].fTime; }

template <class Tv, class Te>
inline int& GraphMatrix<Tv, Te>::m_parent(Rank i) { return m_V[i].parent; }

template <class Tv, class Te>
inline int& GraphMatrix<Tv, Te>::m_priority(Rank i) { return m_V[i].priority; }

template <class Tv, class Te>
int GraphMatrix<Tv, Te>::m_vcnt() { return this->_m_vcnt; }

template <class Tv, class Te>
int GraphMatrix<Tv, Te>::m_ecnt() { return this->_m_ecnt; }

template <class Tv, class Te>
inline bool GraphMatrix<Tv, Te>::m_exists(Rank i, Rank j) { return (i < _m_vcnt) && (j < _m_vcnt) && m_E[i][j] != NULL; }

template <class Tv, class Te>
inline Te& GraphMatrix<Tv, Te>::m_edge(Rank i, Rank j) { return m_E[i][j]->data; }

template <class Tv, class Te>
inline EType& GraphMatrix<Tv, Te>::m_type(Rank i, Rank j) { return m_E[i][j]->type; }

template <class Tv, class Te>
inline int& GraphMatrix<Tv, Te>::m_weight(Rank i, Rank j) { return m_E[i][j]->weight; }

template <class Tv, class Te>
void GraphMatrix<Tv, Te>::m_insert(Te const& edge, int w, Rank i, Rank j) /*边的添加*/
{
    if (this->m_exists(i, j)) // 该边已经存在
        return;       // 不再插入
    m_E[i][j] = new Edge<Te>(edge, w);
    this->_m_ecnt++;
    this->m_V[i].outDegree++;
    this->m_V[j].inDegree++;
}

template <class Tv, class Te>
Te GraphMatrix<Tv, Te>::m_remove(Rank i, Rank j)
{
    Te oldEdge = this->edge(i, j);
    delete m_E[i][j];
    m_E[i][j] = NULL;
    this->_m_ecnt--;
    this->m_V[i].outDegree--;
    this->m_V[j].inDegree++;
}

template <class Tv, class Te>
void GraphMatrix<Tv, Te>::m_insert(Tv const& vertex)
{
    for (Rank i = 0; i < _m_vcnt; ++i)
        m_E[i].push_back(NULL);
    _m_vcnt++;
    m_E.push_back(vector<Edge<Te>*>(_m_vcnt, NULL));
    m_V.push_back(Vertex<Tv>(vertex));
}

template <class Tv, class Te>
Tv GraphMatrix<Tv, Te>::m_remove(Rank i)
{
    // 删除顶点 i 的所有的出边
    for (Rank j = 0; j < _m_vcnt; ++j)
        if (this->exists(i, j))
        {
            delete m_E[i][j]; // 释放空间
            m_V[j].inDegree--;
            _m_ecnt--;
        }
    m_E.erase(m_E.begin() + i); // 删除边矩阵的第 i 行
    _m_vcnt--;
    Tv oldVertex = m_V[i];    // 备份第 i 个节点
    m_V.erase(m_V.begin() + i); // 删除第 i 个节点
    // 删除顶点 i 的所有入边
    for (Rank j = 0; j < _m_vcnt; ++j)
    {
        if (m_E[j][i] != NULL) // 对空间进行释放
            delete m_E[j][i];
        m_E[j].erase(m_E[j].begin() + i); // 在边矩阵中删除每一行的第 i 列
        m_V[j].outDegree--;
        _m_ecnt--;
    }
}

template <class Tv, class Te>
void GraphMatrix<Tv, Te>::m_single_BFS(Rank curVertex, int clock/*时钟*/, void (*VST)(Tv const& vdata))
{
    queue<Rank> Q;
    m_status(curVertex) = VStatus::DISCOVERD;
    Q.push(curVertex);  // 初始化
    while (!Q.empty())
    {
        Rank curVertex = Q.front();
        Q.pop(); // 队首取出 curVertex
        m_dTime(curVertex) = ++clock;
        for (int nbrVertex = 0; nbrVertex < _m_vcnt; ++nbrVertex)
            if (m_E[curVertex][nbrVertex] != NULL)
                if (m_status(nbrVertex) == VStatus::UNDISCOVERD)
                {
                    m_status(nbrVertex) = VStatus::DISCOVERD;
                    Q.push(nbrVertex);
                    m_type(curVertex, nbrVertex) = EType::TREE;
                    m_parent(nbrVertex) = curVertex;
                }
                else
                    m_type(curVertex, nbrVertex) = EType::CROSS;
        VST(m_V[curVertex].data);
        m_status(curVertex) = VStatus::VISITED;
    }   // BFS 没有 fTime 的概念，它的 fTime 就是 dTime
}

template <class Tv, class Te>
void GraphMatrix<Tv, Te>::m_BFS(Rank startVertex, void (*VST)(Tv const& vdata))
{
    m_reset();
    int clock = 0;
    Rank curVertex = startVertex;
    do if (m_status(curVertex) == VStatus::UNDISCOVERD) // 已经遍历过的点不会启用新一轮 bfs
        m_single_BFS(curVertex, clock, VST);
    while (startVertex != (curVertex = ((curVertex+1) % _m_vcnt)));   // 遍历所有节点
}

template <class Tv, class Te>
void GraphMatrix<Tv, Te>::m_single_DFS(Rank curVertex, int clock/*时钟*/, void (*VST)(Tv const& vdata))
{
    m_dTime(curVertex) = ++clock; m_status(curVertex) = VStatus::DISCOVERD;
    for (Rank nbrVertex = curVertex; nbrVertex < _m_vcnt; nbrVertex++)
        if (m_E[curVertex][nbrVertex] != NULL)
            switch (m_status(nbrVertex))
            {
            case VStatus::UNDISCOVERD:  // 如果下一个点是未发现的点，将边的性质设置为 TREE，表示遍历树的树枝
                m_type(curVertex, nbrVertex) = EType::TREE;
                m_single_DFS(nbrVertex, clock); break;
            case VStatus::DISCOVERD:    // 如果下一个点是已发现的点，将点的性质设置为 BACKWARD，表示从遍历树子孙到父母的树枝
                m_type(curVertex, nbrVertex) = EType::BACKWARD;
            case VStatus::VISITED:
                m_type(curVertex, nbrVertex) = m_dTime(curVertex) < m_dTime(nbrVertex) ? EType::FORWARD : EType::CROSS; break;
            default: break;
            }
    VST(m_V[curVertex].data);
    m_status(curVertex) = VStatus::VISITED;
    m_fTime(curVertex) = ++clock;
}

template <class Tv, class Te>
void GraphMatrix<Tv, Te>::m_DFS(Rank startVertex, void (*VST)(Tv const& vdata))
{
    m_reset();
    int clock = 0;
    Rank curVertex = startVertex;
    do if (m_status(curVertex) == VStatus::UNDISCOVERD) // 已经遍历过的点不会启用新一轮 dfs
        m_single_DFS(curVertex, clock, VST);
    while (startVertex != (curVertex = ((curVertex+1) % _m_vcnt)));   // 遍历所有节点
}