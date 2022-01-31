#include <chrono>
#include "clock_callable.h"

using namespace lox;
ValueType ClockCallable::operator()([[maybe_unused]] Interpreter& interpreter,
                                    [[maybe_unused]] std::vector<ValueType>& arguments) const {
    const auto now = std::chrono::system_clock::now();
    return static_cast<double>(std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count());
}
