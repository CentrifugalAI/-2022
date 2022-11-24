#ifndef DEFINE_H
#define DEFINE_H
typedef int Rank;
#define INT_MAX 0x7fffffff
//====================顶点定义========================//

enum class VStatus /*定义顶点状态*/
{
    UNDISCOVERD,
    DISCOVERD,
    VISITED
};
template <class Tv>
struct Vertex
{
    Tv data;                 // 数据域
    int inDegree, outDegree; // 入度，出度
    VStatus status;          // 如上三种状态
    int dTime, fTime;
    Rank parent;  // 遍历树中的父节点
    int priority; // 遍历树中的优先级
    Vertex(Tv const& d) : data(d), inDegree(0), outDegree(0), status(VStatus::UNDISCOVERD), dTime(-1), fTime(-1), parent(-1), priority(INT_MAX) {}
};

//====================边定义========================//
enum class EType
{
    UNDETERMINED,
    TREE,
    CROSS,
    FORWARD,
    BACKWARD
};

template <class Te>
struct Edge
{
    Te data;
    int weight;
    EType type;
    Rank dest; // 边的终点/边首（该变量仅在邻接表表示法中需要初始化）
    Edge(Te const& d, int w, Rank dest = -1) : data(d), weight(w), type(EType::UNDETERMINED), dest(dest) {}
};
#endif