#ifndef EXECUTE_H
#define EXECUTE_H

#include "airport.hpp"
#include <iostream>
#include <string>
using namespace std;
class Execute
{
private:
    void welcome();
public:
    void operator()(Airports* airport);
};

void Execute::welcome()
{
    string outs = "Welcome to the flight system !!!";                               cout << outs << endl;
    outs = "The following functions are available: ";                               cout << outs << endl;
    outs = "1 - Traversal from any airport";                                        cout << outs << endl;
    outs = "2 - The connectivity judgment of any two airports";                     cout << outs << endl;
    outs = "3 - The number of routes between any two airports";                     cout << outs << endl;
    outs = "4 - The shortest flight time between any two airports";                 cout << outs << endl;
    outs = "5 - A route with no more than k connections between any two airports";  cout << outs << endl;
    outs = "6 - Minimum fare path between two airports under constraint conditions";cout << outs << endl;
    outs = "7 - An alternative route for two airports under constraint conditions"; cout << outs << endl;
    outs = "8 - Minimum fare path of two airports under constraint conditions";     cout << outs << endl;
}

void Execute::operator()(Airports* airports)
{
    welcome();
    while (1)
    {
        string outs, ins;
        outs = "Please enter the function you want to call";
        cout << endl << ">>> " << outs << endl << "<<< ";
        int fun = 0;
        cin >> fun; // 读取功能
        switch (fun)
        {
        case 1:
        {
            outs = "Please enter the number of the airport to be queried";
            cout << ">>> " << outs << endl << "<<< ";
            int port_id;
            cin >> port_id;
            cout << ">>> ";
            airports->solver1(PortInfo(port_id));
            break;
        }
        case 2:
        {
            outs = "Please enter the maximum number of transfers allowed";
            cout << ">>> " << outs << endl << "<<< ";
            int max_transfer_time;
            cin >> max_transfer_time;
            cout << ">>> ";
            airports->solver2(max_transfer_time);
            break;
        }
        case 3:
        {
            outs = "Please enter the ID of the start airport and the end airport";
            cout << ">>> " << outs << endl << "<<< ";
            int port_id1, port_id2, max_transfer_time;
            cin >> port_id1 >> port_id2;
            outs = "Please enter a limit on the number of transfers";
            cout << ">>> " << outs << endl << "<<< ";
            cin >> max_transfer_time;
            cout << ">>> ";
            airports->solver3(PortInfo(port_id1), PortInfo(port_id2), max_transfer_time);
            break;
        }
        case 4:
        {
            outs = "Please enter the ID of the start airport and the end airport";
            cout << ">>> " << outs << endl << "<<< ";
            int port_id1, port_id2;
            cin >> port_id1 >> port_id2;
            cout << ">>> ";
            airports->solver4(PortInfo(port_id1), PortInfo(port_id2));
            break;
        }
        case 5:
        {
            outs = "Please enter the ID of the start airport and the end airport";
            cout << ">>> " << outs << endl << "<<< ";
            int port_id1, port_id2;
            cin >> port_id1 >> port_id2;

            int year;

            outs = "Please enter the upper limit of departure time (if there is no requirement, enter 0 for year)";
            cout << ">>> " << outs << endl << "<<< ";
            Time srcTime_ub;
            getline(cin, ins);
            sscanf(ins.c_str(), "%d/%d/%d %d:%d", &srcTime_ub.month, &srcTime_ub.day, &year, &srcTime_ub.hour, &srcTime_ub.minute);
            if (year == 0)
                srcTime_ub.month = srcTime_ub.day = srcTime_ub.hour = srcTime_ub.minute = 0;

            outs = "Please enter the lower limit of departure time (if not, enter 9999 for the year)";
            cout << ">>> " << outs << endl << "<<< ";
            Time srcTime_lb;
            getline(cin, ins);
            sscanf(ins.c_str(), "%d/%d/%d %d:%d", &srcTime_lb.month, &srcTime_lb.day, &year, &srcTime_lb.hour, &srcTime_lb.minute);
            if (year == 0)
                srcTime_lb.month = srcTime_lb.day = srcTime_lb.hour = srcTime_lb.minute = 0;

            outs = "Please enter the upper limit of arrival time (if there is no requirement, enter 0 for year)";
            cout << ">>> " << outs << endl << "<<< ";
            Time destTime_ub;
            getline(cin, ins);sscanf(ins.c_str(), "%d/%d/%d %d:%d", &destTime_ub.month, &destTime_ub.day, &year, &destTime_ub.hour, &destTime_ub.minute);
            if (year == 0)
                destTime_ub.month = destTime_ub.day = destTime_ub.hour = destTime_ub.minute = 0;

            outs = "Please enter the lower limit of arrival time (if there is no requirement, enter 9999 for year)";
            cout << ">>> " << outs << endl << "<<< ";
            Time destTime_lb;
            getline(cin, ins);
            sscanf(ins.c_str(), "%d/%d/%d %d:%d", &destTime_lb.month, &destTime_lb.day, &year, &destTime_lb.hour, &destTime_lb.minute);
            if (year == 9999)
                destTime_lb.month = destTime_lb.day = destTime_lb.hour = destTime_lb.minute = 9999;

            outs = "Please enter the model requirements (if there is no requirement, enter -1)";
            cout << ">>> " << outs << endl << "<<< ";
            int p_model;
            cin >> p_model;

            outs = "Please enter the maximum number of transfers (9999 if not required)";
            cout << ">>> " << outs << endl << "<<< ";
            int max_transfer_time;
            cin >> max_transfer_time;

            cout << ">>> ";
            airports->solver5(PortInfo(port_id1), PortInfo(port_id2), srcTime_ub, srcTime_lb, destTime_ub, destTime_lb, p_model, max_transfer_time);
            break;
        }
        case 6:
        {
            outs = "Please enter the ID of the start airport and the end airport";
            cout << ">>> " << outs << endl << "<<< ";
            int port_id1, port_id2;
            cin >> port_id1 >> port_id2;

            int year;

            outs = "Please enter the upper limit of departure time (if there is no requirement, enter 0 for year)";
            cout << ">>> " << outs << endl << "<<< ";
            Time srcTime_ub;
            getline(cin, ins);
            sscanf(ins.c_str(), "%d/%d/%d %d:%d", &srcTime_ub.month, &srcTime_ub.day, &year, &srcTime_ub.hour, &srcTime_ub.minute);
            if (year == 0)
                srcTime_ub.month = srcTime_ub.day = srcTime_ub.hour = srcTime_ub.minute = 0;

            outs = "Please enter the lower limit of departure time (if not, enter 9999 for the year)";
            cout << ">>> " << outs << endl << "<<< ";
            Time srcTime_lb;
            cin >> srcTime_lb.month >> srcTime_lb.day >> year >> srcTime_lb.hour >> srcTime_lb.minute;
            if (year == 0)
                srcTime_lb.month = srcTime_lb.day = srcTime_lb.hour = srcTime_lb.minute = 0;

            outs = "Please enter the upper limit of arrival time (if there is no requirement, enter 0 for year)";
            cout << ">>> " << outs << endl << "<<< ";
            Time destTime_ub;
            getline(cin, ins);
            sscanf(ins.c_str(), "%d/%d/%d %d:%d", &destTime_ub.month, &destTime_ub.day, &year, &destTime_ub.hour, &destTime_ub.minute);
            if (year == 0)
                destTime_ub.month = destTime_ub.day = destTime_ub.hour = destTime_ub.minute = 0;

            outs = "Please enter the lower limit of arrival time (if there is no requirement, enter 9999 for year)";
            cout << ">>> " << outs << endl << "<<< ";
            Time destTime_lb;
            getline(cin, ins);
            sscanf(ins.c_str(), "%d/%d/%d %d:%d", &destTime_lb.month, &destTime_lb.day, &year, &destTime_lb.hour, &destTime_lb.minute);
            if (year == 9999)
                destTime_lb.month = destTime_lb.day = destTime_lb.hour = destTime_lb.minute = 9999;

            outs = "Please enter the model requirements (if there is no requirement, enter -1)";
            cout << ">>> " << outs << endl << "<<< ";
            int p_model;
            cin >> p_model;

            cout << ">>> ";
            airports->solver6(PortInfo(port_id1), PortInfo(port_id2), srcTime_ub, srcTime_lb, destTime_ub, destTime_lb, p_model);
            break;
        }
        case 7:
        {
            outs = "Please enter the ID of the start airport and the end airport";
            cout << ">>> " << outs << endl << "<<< ";
            int port_id1, port_id2;
            cin >> port_id1 >> port_id2;

            outs = "Please enter the maximum transfer time";
            cout << ">>> " << outs << endl << "<<< ";
            int year;  Time max_time;
            getline(cin, ins);
            sscanf(ins.c_str(), "%d/%d/%d %d:%d", &max_time.month, &max_time.day, &year, &max_time.hour, &max_time.minute);

            outs = "Please enter the maximum number of transfers";
            cout << ">>> " << outs << endl << "<<< ";
            int max_transfer_time;
            cin >> max_transfer_time;

            cout << ">>> ";
            airports->solver7(PortInfo(port_id1), PortInfo(port_id2), max_time.toNum(), max_transfer_time);
            break;
        }
        case 8:
        {
            outs = "Please enter the ID of the start airport and the end airport";
            cout << ">>> " << outs << endl << "<<< ";
            int port_id1, port_id2;
            cin >> port_id1 >> port_id2;

            outs = "Please enter the maximum transfer time";
            cout << ">>> " << outs << endl << "<<< ";
            int year;  Time max_time;
            getline(cin, ins);
            sscanf(ins.c_str(), "%d/%d/%d %d:%d", &max_time.month, &max_time.day, &year, &max_time.hour, &max_time.minute);

            cout << ">>> ";
            airports->solver8(PortInfo(port_id1), PortInfo(port_id2), max_time.toNum());
            break;
        }
        default: break;
        }
    }
}

#endif