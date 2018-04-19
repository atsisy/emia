#include "emulator.hpp"
#include <iostream>

ExceptionTable::ExceptionTable()
{
	exception_table.insert(std::make_pair(INVALID_OPERATION_EXCEPTION, invalid_operation));
}

ExceptionTable::~ExceptionTable()
{}

void ExceptionTable::invalid_operation(Emulator *emulator)
{
	std::cout << "invalid operation exception" << std::endl;
	exit(-1);
}