#ifndef AIRPORT_H
#define AIRPORT_H

#include "graphList.hpp"
#include "graphMartrix.hpp"
#include <string>
#include <iostream>
#include <stack>

#define FLIGHT_CNT 2346

struct Time
{
    int month, day, hour, minute;
    Time(int mo = -1, int d = -1, int h = -1, int mi = -1);
    Time(Time const& time);
    bool operator>(Time const& t);
    bool operator>=(Time const& t);
    bool operator<(Time const& t);
    bool operator<=(Time const& t);
    bool operator==(Time const& t);
    int toNum();
};

// 注：信息的分配：对 PortInfo 和 FlightInfo 分配不同的信息——取决于对时序关系是否需要考虑
struct PortInfo
{
    Rank port_id;
    PortInfo(Rank p_id = -1);
    PortInfo& operator--(int i);
    PortInfo& operator--();
};

struct FlightInfo
{

    Rank srcPortID, destPortID;  // 起始/终止航站
    Rank flight_id;              // 序号
    Rank plane_id, plane_model;  // 执行任务的飞机
    Time departTime, arrivalTime; // 出发时间和抵达时间
    int fare;  // 费用
    int priority;                 // 迪杰斯特拉算法意义下的优先级 
    int guard;                    // 警戒值
    FlightInfo* lastFlight;          // 迪杰斯特拉算法意义下的上一趟航班
    list<FlightInfo*> nextFlights;    // 下一趟航班构成的队列
    FlightInfo(Rank sp = -1, Rank dp = -1, Rank f_id = -1, Rank p_id = -1, Rank p_model = -1, Time dt = Time(), Time at = Time(), int f = -1); /**/
    FlightInfo(Time const& at);
    FlightInfo(FlightInfo const& f);
    // ~FlightInfo();
};

struct FlightEle
{
    Rank ID;
    int priority;
    int guard;
    FlightEle(int ID = -1, int p = -1, int g = -1) : ID(ID), priority(p), guard(g) {}
};


class Airports : GraphList<PortInfo, FlightInfo*>, GraphMatrix<PortInfo, FlightInfo*>
{
protected:
    vector<FlightInfo*>* flights;
    void builtPorts();      // 初始化所有 portInfo 的航空站编号
    void initFlights(vector<vector<string>>* data); // 初始化所有航班信息
    void initNextFlight();  // 初始化所有航班的下一趟航班
    static void VST_print(PortInfo const& port);
    void resetFlights();     // 重新设置 flight 信息

public:
    Airports(vector<vector<string>>* data);
    // 根据机场 ID 进行
    void solver1(PortInfo const& start_port);     // solution of 2.1
    void solver2(int max_transfer_time);
    void solver3(PortInfo const& src_port, PortInfo const& dest_port, int transfer_time);
    void solver4(PortInfo const& src_port, PortInfo const& dest_port);
    void solver5(PortInfo const& src_port, PortInfo const& dest_port, Time const& srcTime_ub, Time const& srcTime_lb, Time const& destTime_ub, Time const& destTime_lb, int p_model, int max_transfer_time);
    void solver6(PortInfo const& src_port, PortInfo const& dest_port, Time const& srcTime_ub, Time const& srcTime_lb, Time const& destTime_ub, Time const& destTime_lb, int p_model);
    void solver7(PortInfo const& src_port, PortInfo const& dest_port, int max_time, int max_transfer_time);
    void solver8(PortInfo const& src_port, PortInfo const& dest_port, int max_time);
};

#define MAX_PORT_CNT 80

Time::Time(int mo, int d, int h, int mi) : month(mo), day(d), hour(h), minute(mi) {}
Time::Time(Time const& time)
{
    this->month = time.month;
    this->day = time.day;
    this->hour = time.hour;
    this->minute = time.minute;
}
bool Time::operator>(Time const& t) { return month * 43200 + day * 1440 + hour * 60 + minute * 1 > t.month * 43200 + t.day * 1440 + t.hour * 60 + t.minute * 1; }
bool Time::operator>=(Time const& t) { return month * 43200 + day * 1440 + hour * 60 + minute * 1 >= t.month * 43200 + t.day * 1440 + t.hour * 60 + t.minute * 1; }
bool Time::operator<(Time const& t) { return month * 43200 + day * 1440 + hour * 60 + minute * 1 < t.month * 43200 + t.day * 1440 + t.hour * 60 + t.minute * 1; }
bool Time::operator<=(Time const& t) { return month * 43200 + day * 1440 + hour * 60 + minute * 1 <= t.month * 43200 + t.day * 1440 + t.hour * 60 + t.minute * 1; }
bool Time::operator==(Time const& t) { return month * 43200 + day * 1440 + hour * 60 + minute * 1 == t.month * 43200 + t.day * 1440 + t.hour * 60 + t.minute * 1; }
int Time::toNum() { return month * 43200 + day * 1440 + hour * 60 + minute * 1; }


PortInfo::PortInfo(Rank p_id) : port_id(p_id) {}

PortInfo& PortInfo::operator--(int i)
{
    return *this;
}

PortInfo& PortInfo::operator--()
{
    return *this;
}

FlightInfo::FlightInfo(Rank sp, Rank dp, Rank f_id, Rank p_id, Rank p_model, Time dt, Time at, int f) /**/
    : srcPortID(sp), destPortID(dp), flight_id(f_id), plane_id(p_id), plane_model(p_model), departTime(dt), arrivalTime(at), fare(f)
{
    this->priority = __INT_MAX__;
    this->guard = __INT_MAX__;
    this->lastFlight = NULL;
}

FlightInfo::FlightInfo(FlightInfo const& f) : srcPortID(f.srcPortID), destPortID(f.destPortID), flight_id(f.flight_id), plane_id(f.plane_id),
plane_model(f.plane_model), departTime(f.departTime), arrivalTime(f.arrivalTime), fare(f.fare), priority(f.priority), guard(f.guard),
lastFlight(f.lastFlight), nextFlights(f.nextFlights)
{}

FlightInfo::FlightInfo(Time const& at)
{
    srcPortID = -1;
    destPortID = -1;
    flight_id = -1;
    plane_id = -1;
    plane_model = -1;
    priority = __INT_MAX__;
    guard = __INT_MAX__;
    arrivalTime.month = at.month;
    arrivalTime.day = at.day;
    arrivalTime.hour = at.hour;
    arrivalTime.minute = at.minute;
    departTime = Time();
    fare = -1;
    lastFlight = NULL;
}

// FlightInfo::~FlightInfo() { if (lastFlight) delete lastFlight; }

bool operator < (const FlightInfo& fi1, const FlightInfo& fi2) { return fi1.priority < fi2.priority; }

bool operator < (const FlightEle& ele1, const FlightEle& ele2) { return ele1.priority < ele2.priority; }

void Airports::builtPorts()
{
    for (int i = 0; i < MAX_PORT_CNT; ++i)
    {
        this->insert(PortInfo(i));
        this->m_insert(PortInfo(i));
    }
}

void Airports::initFlights(vector<vector<string>>* data)
{
    // data 的结构是：第一维就是表格的每一行，第二维就是表格的 i 行 j 列的内容
    bool have_jumped_first_line = false;
    for (auto line : *data)
    {
        if (!have_jumped_first_line)
        {
            have_jumped_first_line = true;
            continue;
        }
        int srcPortID = atoi(line[4].c_str()), destPortID = atoi(line[5].c_str());
        int flightID = atoi(line[0].c_str()), planeID = atoi(line[8].c_str()), planeModel = atoi(line[9].c_str()), fare = atoi(line[10].c_str());
        string depart_time_str = line[6], arrival_time_str = line[7];
        // cout << "line[6] is " << line[6].c_str() << endl;
        // cout << "line[7] is " << line[7].c_str() << endl;
        int d_day, d_month, d_year, d_hour, d_minute;
        int a_day, a_month, a_year, a_hour, a_minute;
        sscanf(depart_time_str.c_str(), "%d/%d/%d %d:%d", &d_month, &d_day, &d_year, &d_hour, &d_minute);
        sscanf(arrival_time_str.c_str(), "%d/%d/%d %d:%d", &a_month, &a_day, &a_year, &a_hour, &a_minute);
        Time depart_time(d_month, d_day, d_hour, d_minute), arrival_time(a_month, a_day, a_hour, a_minute);
        FlightInfo* new_flight = new FlightInfo(srcPortID, destPortID, flightID, planeID, planeModel, depart_time, arrival_time, fare);
        this->insert(new_flight, -1, srcPortID, destPortID);
        if (new_flight->flight_id == 1) cout << new_flight << endl;
        this->m_insert(new_flight, -1, srcPortID, destPortID);
        (*flights)[new_flight->flight_id] = new_flight;      // 整合到向量中
        if (new_flight->flight_id == 1) cout << (*flights)[new_flight->flight_id] << endl;
    }
}

Airports::Airports(vector<vector<string>>* data)
{
    GraphList();
    GraphMatrix();
    cout << "The airport is building now......" << endl;
    builtPorts();
    this->flights = new vector<FlightInfo*>(FLIGHT_CNT);
    initFlights(data);
    initNextFlight();
    cout << "The airport has been built successfully!!!" << endl;
}

// 第一重循环和第二重循环实际上枚举了所有边，时间复杂度合并统计为 O(e)
// 第三重循环讨论了每一条边的所有邻接边，时间复杂度实际上远小于 O(e)
// 该函数的时间复杂度远小于 O(e^2) ，但是在 O(e) 以上
void Airports::initNextFlight()
{
    for (auto edge_list : this->E)
        for (auto edge : edge_list)     // 讨论每一个航班站（顶点）的每一趟航班（边）
            for (auto next_edge : E[edge->data->destPortID])     // 每一条边讨论终点起飞的所有航班（边）
                if (edge->data->arrivalTime <= next_edge->data->departTime)  // 如果上一趟航班起飞时间比上一趟终点出发的下一趟航班抵达时间晚
                    edge->data->nextFlights.push_back(next_edge->data);   // 认为该航班可以作为下一趟航班，并载入前一趟航班的 nextFlight
}

inline void Airports::VST_print(PortInfo const& port)
{
    cout << port.port_id << " ";
}

void Airports::resetFlights()
{
    for (auto edgelist : E)
        for (auto flight : edgelist)
        {
            flight->data->priority = __INT_MAX__;
            flight->data->guard = __INT_MAX__;
            if (flight->data->lastFlight)
                delete flight->data->lastFlight;
            flight->data->lastFlight = NULL;
        }
}

void Airports::solver1(PortInfo const& start_port)
{
    this->reset();
    cout << "BFS: ";
    this->single_BFS(start_port.port_id, 0, VST_print);
    cout << endl;

    this->reset();
    cout << "DFS: ";
    this->single_DFS(start_port.port_id, 0, VST_print);
    cout << endl;
}

void Airports::solver2(int max_trasfer_time)
{
    Rank arrivable[this->m_vcnt()][this->m_vcnt()];
    // 对 arrivable 数组进行初始化
    for (int i = 0; i < this->m_vcnt(); ++i)
        for (int j = 0; j <this->m_vcnt(); ++j)
            arrivable[i][j] = 0;
    // 对每个点进行一次限制深度的 BFS 
    for (Rank startVertex = 0; startVertex < this->m_vcnt(); ++startVertex)
    {
        m_reset();          // 对于每个新的点的bfs都需要先清除所有点状态
        queue<Rank> Q;
        m_status(startVertex) = VStatus::DISCOVERD;
        Q.push(startVertex);  // 初始化
        m_dTime(startVertex) = 0;     // dTIme 此时用于表示遍历层次
        while (!Q.empty())
        {
            Rank curVertex = Q.front();
            Q.pop(); // 队首取出 curVertex
            // 扩展邻居结点
            if (m_dTime(curVertex) > max_trasfer_time)
                continue;
            for (int nbrVertex = 0; nbrVertex < m_vcnt(); ++nbrVertex)
                if (m_E[curVertex][nbrVertex] != NULL)
                    if (m_status(nbrVertex) == VStatus::UNDISCOVERD)
                    {
                        m_status(nbrVertex) = VStatus::DISCOVERD;
                        m_dTime(nbrVertex) = m_dTime(curVertex) + 1;
                        Q.push(nbrVertex);
                        arrivable[startVertex][nbrVertex] = 1;
                    }
            m_status(curVertex) = VStatus::VISITED;
        }   // BFS 没有 fTime 的概念，它的 fTime 就是 dTime
    }
    cout << endl;
    for (int i = 0; i < m_vcnt(); ++i)
    {
        for (int j = 0; j < m_vcnt(); ++j)
            cout << arrivable[i][j] << " ";
        cout << endl;
    }
}

void Airports::solver3(PortInfo const& src_post, PortInfo const& dest_port, int transfer_time)
{
    vector<int> arrivable;
    for (auto edge : E[src_post.port_id])
    {
        arrivable.push_back(edge->dest);
        if (transfer_time >= 1)
            for (auto flight : edge->data->nextFlights)
                arrivable.push_back(flight->destPortID);
    }
    cout << endl;
    for (auto ele : arrivable)
        cout << ele << " ";
    cout << endl << "total path count is " << arrivable.size();
    cout << endl;
}

/**
 * 关于 priority 的语义规约：
 * priority 的表示的是到达本趟航班开始前的相关属性（比如时间、停机次数、）的累加
*/

void Airports::solver4(PortInfo const& src_port, PortInfo const& dest_port)
{
    priority_queue<FlightEle> unadoptEle;    // 尚未采取的边 
    /** TODO: 这里还需要自行实现一个二叉堆，先使用 priority_queue 糊弄一下）
     *  这里考虑的优先级 priority 就是抵达该航班起点时已经使用的时间, 越小越优
     */
    bool isAdopted[10000] = { false };
    resetFlights();                          // 初始化所有边
    // 先将 src_port 为起点的边全部放入 priority_queue 中
    for (auto edge : E[src_port.port_id])
    {
        FlightInfo* begin_flight = new FlightInfo(Time(edge->data->departTime)); // 哨兵
        edge->data->priority = 0;             // 抵达该航班起点使用的时间均为 0;
        edge->data->lastFlight = begin_flight;
        unadoptEle.push(FlightEle(edge->data->flight_id, edge->data->priority, edge->data->guard));         // 初始时放入起点周围的所有边
    }

    while (!unadoptEle.empty())
    {
        // 算法正式启动：
        FlightEle bestFlightEle = unadoptEle.top();     // 贪心：最优边，采用并放入已采取队列中
        unadoptEle.pop();
        isAdopted[bestFlightEle.ID] = true;
        // 优先级更新   
        FlightInfo* best_flight = (*flights)[bestFlightEle.ID];
        for (auto next_flight : best_flight->nextFlights)
        {
            if (isAdopted[next_flight->flight_id])
                continue;
            // 下一航班（next_flight）抵达起点时使用的时间 > 上一航班（bestFlight）抵达时已经花费的时间 + 
            // std::cout << "the priority of next_flight is " << next_flight->priority << endl;
            if (next_flight->priority > best_flight->priority + best_flight->arrivalTime.toNum() - best_flight->lastFlight->arrivalTime.toNum())
            {
                next_flight->priority = best_flight->priority + best_flight->arrivalTime.toNum() - best_flight->lastFlight->arrivalTime.toNum();
                // std::cout << "the priority of next_flight is " << next_flight->priority << endl;
                next_flight->lastFlight = best_flight;
                unadoptEle.push(FlightEle(next_flight->flight_id, next_flight->priority, next_flight->guard));      // 更新后的 next_flight 加入未采用队列
            }
        }
    }   // 可以得到起点到任何一个点的最短时间

    /** 选出从 dest_port 出发的最优航班（从 dest_port 出发的航班的 priority 不一定一样）
     *  比如 11 点出发的航班飞行 9-11 ，从 21 点出发的航班飞行 21-22，起飞时间不同会影响总飞行时间最优值的不同
     */
    FlightInfo* best_end_flight = E[dest_port.port_id].front()->data;
    for (auto edge : E[dest_port.port_id])    // 遍历所有从 dest_port 出发的航班
        if (edge->data->priority < best_end_flight->priority)
            best_end_flight = edge->data;
    cout << "The shortest flying time is " << best_end_flight->priority << " minutes";
}

void Airports::solver5(PortInfo const& src_port, PortInfo const& dest_port,
    Time const& srcTime_ub, Time const& srcTime_lb, Time const& destTime_ub, Time const& destTime_lb,
    int p_model, int max_transfer_time)
{
    priority_queue<FlightInfo> unadoptEle;    // 尚未采取的边 
    bool isAdopted[1000] = { false };
    resetFlights();                          // 初始化所有边
    // 先将 src_port 为起点的边全部放入 priority_queue 中
    for (auto edge : E[src_port.port_id])
    {
        if (edge->data->departTime >= srcTime_lb && edge->data->departTime <= srcTime_ub &&
            edge->data->arrivalTime >= destTime_lb && edge->data->arrivalTime <= destTime_ub &&
            edge->data->plane_model == p_model)   // 考虑起飞时段和飞机机型
        {
            FlightInfo* begin_flight = new FlightInfo(Time(edge->data->departTime)); // 哨兵
            edge->data->priority = -1;                // 抵达该航班起点使用的中转次数均为 -1;
            edge->data->lastFlight = begin_flight;   // 设置哨兵
            unadoptEle.push(*edge->data);               // 初始时放入起点周围的所有边
        }
    }

    while (!unadoptEle.empty())
    {
        // 算法正式启动：
        FlightInfo* bestFlight = new FlightInfo(unadoptEle.top());     // 贪心：最优边，采用并放入已采取队列中
        unadoptEle.pop();
        isAdopted[bestFlight->flight_id] = true;
        // 优先级更新   
        for (auto next_flight : bestFlight->nextFlights)
        {
            if (isAdopted[next_flight->flight_id] ||
                next_flight->departTime < srcTime_lb || next_flight->departTime > srcTime_ub ||
                next_flight->arrivalTime < destTime_lb || next_flight->arrivalTime > destTime_ub ||
                next_flight->plane_model != p_model)  // 如果一个航班已经被考虑过了则跳过
                continue;
            // 下一航班（next_flight）抵达起点时使用的停机次数 > 上一航班（bestFlight）抵达时使用的停机次数
            if (next_flight->priority > bestFlight->priority + 1)
            {
                next_flight->priority = bestFlight->priority + 1;
                next_flight->lastFlight = bestFlight;
                unadoptEle.push(*next_flight);      // 更新后的 next_flight 加入未采用队列
            }
        }
    }   // 可以得到起点到任何一个点的最短时间

    FlightInfo* best_end_flight = new FlightInfo();    //
    for (auto edge : E[dest_port.port_id])    // 遍历所有从 dest_port 出发的航班
        if (edge->data->priority < best_end_flight->priority)   // 选出最优解
            best_end_flight = edge->data;
    if (best_end_flight->priority > max_transfer_time)  // 如果没有满足的解（更新不到终点目标或中转次数太多）
    {
        cout << -1 << endl;
        return;
    }
    cout << "The flight ID list: " << endl;
    while (best_end_flight->priority != __INT_MAX__)   // 溯源直到哨兵
    {
        cout << best_end_flight->flight_id << " ";
        best_end_flight = best_end_flight->lastFlight;
    }
}

void Airports::solver6(PortInfo const& src_port, PortInfo const& dest_port,
    Time const& srcTime_ub, Time const& srcTime_lb, Time const& destTime_ub, Time const& destTime_lb, int p_model)
{
    priority_queue<FlightInfo> unadoptEle;    // 尚未采取的边 
    bool isAdopted[1000] = { false };
    resetFlights();                          // 初始化所有边
    // 先将 src_port 为起点的边全部放入 priority_queue 中
    for (auto edge : E[src_port.port_id])
    {
        if (edge->data->departTime >= srcTime_lb && edge->data->departTime <= srcTime_ub &&
            edge->data->arrivalTime >= destTime_lb && edge->data->arrivalTime <= destTime_ub &&
            edge->data->plane_model == p_model)   // 考虑起飞时段和飞机机型
        {
            FlightInfo* begin_flight = new FlightInfo(Time(edge->data->departTime)); // 哨兵
            edge->data->priority = 0;                // 抵达该航班起点使用的航费花销均为 0;
            edge->data->lastFlight = begin_flight;   // 设置哨兵（哨兵的 priority 初始化为 __INT_MAX__）
            unadoptEle.push(*edge->data);               // 初始时放入起点周围的所有边
        }
    }

    while (!unadoptEle.empty())
    {
        // 算法正式启动：
        FlightInfo* bestFlight = new FlightInfo(unadoptEle.top());     // 贪心：最优边，采用并放入已采取队列中
        unadoptEle.pop();
        isAdopted[bestFlight->flight_id] = true;
        // 优先级更新   
        for (auto next_flight : bestFlight->nextFlights)
        {
            if (isAdopted[next_flight->flight_id] ||
                next_flight->departTime < srcTime_lb || next_flight->departTime > srcTime_ub ||
                next_flight->arrivalTime < destTime_lb || next_flight->arrivalTime > destTime_ub ||
                next_flight->plane_model != p_model)  // 如果一个航班已经被考虑过了则跳过
                continue;
            // 下一航班（next_flight）抵达起点时使用的费用 > 上一航班（bestFlight）抵达时使用的费用
            if (next_flight->priority > bestFlight->priority + bestFlight->fare)
            {
                next_flight->priority = bestFlight->priority + bestFlight->fare;
                next_flight->lastFlight = bestFlight;
                unadoptEle.push(*next_flight);      // 更新后的 next_flight 加入未采用队列
            }
        }
    }   // 可以得到起点到任何一个点的最短时间

    FlightInfo* best_end_flight = new FlightInfo();    //
    for (auto edge : E[dest_port.port_id])    // 遍历所有从 dest_port 出发的航班
        if (edge->data->priority < best_end_flight->priority)   // 选出最优解
            best_end_flight = edge->data;
    if (best_end_flight->priority == __INT_MAX__)      // 如果没有满足的解（更新不到目标）
    {
        cout << -1 << endl;
        return;
    }
    // int least_transfer_time = best_end_flight->priority - 1;
    cout << "The flight ID list: " << endl;
    while (best_end_flight->priority != __INT_MAX__)   // 溯源直到哨兵
    {
        cout << best_end_flight->flight_id << " ";
        best_end_flight = best_end_flight->lastFlight;
    }
}

/**
 * 2.7 要求同时考虑中转时间和中转次数，要求二者均符合条件的航线
 * 采取的思路是：将 priority 值设置为：当前的中转时间总和，设置一个 guard 警戒值变量，用于衡量累计的中转时间总和
 * 如果某航班的累积中转次数已经超过 guard 值，则不对该航班优先级进行更新，而是视为一条无法满足条件的航班
*/
void Airports::solver7(PortInfo const& src_port, PortInfo const& dest_port, int max_time, int max_transfer_time)
{
    priority_queue<FlightInfo> unadoptEle;    // 尚未采取的边 
    bool isAdopted[1000] = { false };
    resetFlights();                          // 初始化所有边
    // 先将 src_port 为起点的边全部放入 priority_queue 中
    for (auto edge : E[src_port.port_id])
    {
        FlightInfo* begin_flight = new FlightInfo(Time(edge->data->departTime)); // 哨兵
        edge->data->priority = 0;                // 抵达初始航班起点使用的中转时间均为 0;
        edge->data->guard = -1;                   // 抵达初始航班起点使用的中转次数均为 -1
        edge->data->lastFlight = begin_flight;   // 设置哨兵（哨兵的 priority 初始化为 __INT_MAX__）
        unadoptEle.push(*edge->data);               // 初始时放入起点周围的所有边
    }

    while (!unadoptEle.empty())
    {
        // 算法正式启动：
        FlightInfo* bestFlight = new FlightInfo(unadoptEle.top());     // 贪心：最优边，采用并放入已采取队列中
        unadoptEle.pop();
        isAdopted[bestFlight->flight_id] = true;
        // 优先级更新   
        for (auto next_flight : bestFlight->nextFlights)
        {
            if (bestFlight->guard >= max_transfer_time)
                break;
            if (isAdopted[next_flight->flight_id])  // 如果一个航班已经被考虑过了则跳过或者中转次数已经达到警戒值
                continue;
            // 下一航班（next_flight）抵达起点时使用的停机次数 > 上一航班（bestFlight）抵达时使用的停机次数
            if (next_flight->priority > bestFlight->priority + next_flight->departTime.toNum() - bestFlight->arrivalTime.toNum())
            {
                next_flight->priority = bestFlight->priority + next_flight->departTime.toNum() - bestFlight->arrivalTime.toNum();
                next_flight->guard = bestFlight->guard + 1;
                next_flight->lastFlight = bestFlight;
                unadoptEle.push(*next_flight);      // 更新后的 next_flight 加入未采用队列
            }
        }
    }   // 可以得到起点到任何一个点的最短时间

    /** 选出从 dest_port 出发的最优航班（从 dest_port 出发的航班的 priority 不一定一样）
     *  比如 11 点出发的航班飞行 9-11 ，从 21 点出发的航班飞行 21-22，起飞时间不同会影响总飞行时间最优值的不同
     */
    FlightInfo* best_end_flight = new FlightInfo();    //
    for (auto edge : E[dest_port.port_id])    // 遍历所有从 dest_port 出发的航班
        if (edge->data->priority < best_end_flight->priority)   // 选出最优解
            best_end_flight = edge->data;

    if (best_end_flight->priority == __INT_MAX__ || best_end_flight->priority > max_time)      // 如果没有满足的解（中转次数无法达到预期标准或最短路径时间也高于要求时间）
    {
        cout << -1 << endl;
        return;
    }
    cout << "The flight ID list: " << endl;
    while (best_end_flight->priority != __INT_MAX__)   // 溯源直到哨兵
    {
        cout << best_end_flight->flight_id << " ";
        best_end_flight = best_end_flight->lastFlight;
    }
}

void Airports::solver8(PortInfo const& src_port, PortInfo const& dest_port, int max_time)
{
    priority_queue<FlightInfo> unadoptEle;    // 尚未采取的边 
    bool isAdopted[1000] = { false };
    resetFlights();                          // 初始化所有边
    // 先将 src_port 为起点的边全部放入 priority_queue 中
    for (auto edge : E[src_port.port_id])
    {
        FlightInfo* begin_flight = new FlightInfo(Time(edge->data->departTime)); // 哨兵
        edge->data->priority = 0;                // 抵达初始航班起点使用的航行费用均为 0;
        edge->data->guard = 0;                   // 抵达初始航班起点使用的中转时间均为 0
        edge->data->lastFlight = begin_flight;   // 设置哨兵（哨兵的 priority 初始化为 __INT_MAX__）
        unadoptEle.push(*edge->data);               // 初始时放入起点周围的所有边
    }

    while (!unadoptEle.empty())
    {
        // 算法正式启动：
        FlightInfo* bestFlight = new FlightInfo(unadoptEle.top());     // 贪心：最优边，采用并放入已采取队列中
        unadoptEle.pop();
        isAdopted[bestFlight->flight_id] = true;
        // 优先级更新   
        for (auto next_flight : bestFlight->nextFlights)
        {
            if (bestFlight->guard >= max_time)      // 中转时间超过要求
                break;
            if (isAdopted[next_flight->flight_id])  // 如果一个航班已经被考虑过了则跳过或者中转次数已经达到警戒值
                continue;
            // 下一航班（next_flight）抵达起点时使用的费用 > 上一航班（bestFlight）抵达时使用的费用
            if (next_flight->priority > bestFlight->priority + bestFlight->fare)
            {
                next_flight->priority = bestFlight->priority + bestFlight->fare;
                next_flight->guard = bestFlight->guard + next_flight->departTime.toNum() - bestFlight->arrivalTime.toNum();
                next_flight->lastFlight = bestFlight;
                unadoptEle.push(*next_flight);      // 更新后的 next_flight 加入未采用队列
            }
        }
    }   // 可以得到起点到任何一个点的最短时间

    /** 选出从 dest_port 出发的最优航班（从 dest_port 出发的航班的 priority 不一定一样）
     *  比如 11 点出发的航班飞行 9-11 ，从 21 点出发的航班飞行 21-22，起飞时间不同会影响总飞行时间最优值的不同
     */
    FlightInfo* best_end_flight = new FlightInfo();    //
    for (auto edge : E[dest_port.port_id])    // 遍历所有从 dest_port 出发的航班
        if (edge->data->priority < best_end_flight->priority)   // 选出最优解
            best_end_flight = edge->data;

    if (best_end_flight->priority == __INT_MAX__)      // 如果没有满足的解
    {
        cout << -1 << endl;
        return;
    }
    cout << "The flight ID list: " << endl;
    while (best_end_flight->priority != __INT_MAX__)   // 溯源直到哨兵
    {
        cout << best_end_flight->flight_id << " ";
        best_end_flight = best_end_flight->lastFlight;
    }
}

#endif