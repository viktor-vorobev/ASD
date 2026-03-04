#include "itable.h"

TableException::TableException(const std::string& message)
    : std::runtime_error("Table error: " + message) {}