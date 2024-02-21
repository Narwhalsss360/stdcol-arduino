#ifndef TestFunctionWrapper_h
#define TestFunctionWrapper_h

#include "TestPlatform.h"

#define test_fail false
#define test_pass true

#define fail_if(expr) if (expr) return test_fail
#define fail_if_msg(expr, msg) if (!(expr)) { std::cout << msg; return test_fail; }

#define pass_if(expr) if (expr) return test_pass
#define pass_if_msg(expr, msg) if (expr) { std::cout << msg; return test_pass; }

const char resulString[][5] = { "Fail", "Pass" };

struct TesterFunction;

using test_func = bool (*) (TesterFunction&);

struct TesterFunction {
	Platform::string name = "Unnamed | Uninitialized";
	test_func f = nullptr;
	bool result = false;

	TesterFunction() {}

	TesterFunction(Platform::string name, test_func f) : name(name), f(f) {

	}

	bool operator()() {
		if (f) result = f(*this);
		return result;
	}

	operator const bool() const {
		return result;
	}
};

#ifndef plat_arduino
class test_exception : public std::exception {
public:
	test_exception() : std::exception() {}

	const char* what() const noexcept override { return "A thrown exception that should be caught, and logged."; }
};
#endif

Platform::ostream& operator<<(Platform::ostream& stream, const TesterFunction& test) {
	stream << '[' << test.name << ']';
	return stream;
}

#endif
