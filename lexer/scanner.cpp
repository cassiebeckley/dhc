#include "scanner.hpp"

char dhc::scanner::peek()
{
    return source.at(index);
}

char dhc::scanner::get()
{
    return source.at(index++);
}

int dhc::scanner::get_index()
{
    return index;
}

void dhc::scanner::set_index(int i)
{
    index = i;
}
