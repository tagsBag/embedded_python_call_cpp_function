#ifndef HELPER_H
#define HELPER_H

#include <iostream>
#include <sstream>
#include <vector>
#include <string>

#include <boost/python.hpp>

#undef slots
#include <Python.h>
#define slots Q_SLOTS

#include "strategy.h"

char const* greet();

class  Context
{
public:
    Context(Strategy * strategy);
    ~Context();

private:
    Strategy * m_strategy;
};

#endif // HELPER_H
