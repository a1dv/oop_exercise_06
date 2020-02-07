#include <algorithm>
#include <iostream>
#include <map>
#include "point.h"
#include "pentagon.h"
#include "list.h"
#include "allocator.h"

int main()
{
    containers::list<double> l;
    pentagon <double> p(std::cin);
    p.print(std::cout);
    double i;
    std::cin >> i;
    l.insert_by_number(0, i);
    l.insert_by_number(1, i);
    for (double elem : l) {
        std::cout << elem << elem << elem;
    }
}
