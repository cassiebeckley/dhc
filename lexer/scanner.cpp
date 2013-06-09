#include "scanner.hpp"

char dhc::scanner::peek()
{
    index_to_line_number[index] = line_number;
    if (line_number_to_index[line_number].count(index) == 0) {
        line_number_to_index[line_number].insert(index);
    }

    return source.at(index);
}

char dhc::scanner::get()
{
    index_to_line_number[index] = line_number;
    if (line_number_to_index[line_number].count(index) == 0) {
        line_number_to_index[line_number].insert(index);
    }

    return source.at(index++);
}

unsigned int dhc::scanner::get_index()
{
    return index;
}

void dhc::scanner::set_index(int i)
{
    index = i;
    line_number = index_to_line_number[index];
}

bool dhc::scanner::finished()
{
    return index >= source.length();
}

void dhc::scanner::newline()
{
    line_number++;
}

unsigned int dhc::scanner::lineno()
{
    return line_number;
}

unsigned int dhc::scanner::charno()
{
    return index - *(line_number_to_index[index_to_line_number[index]].begin());
}
