#include "readData.hpp"
#include "execute.hpp"

int main()
{
    ReadData readData;
    vector<vector<string>>* data = readData();
    Airports* airports = new Airports(data);
    delete data;
    Execute execute;
    execute(airports);
    delete airports;
    while (1)
    {
    }
    return 0;
}