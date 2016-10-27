#pragma once

#include <sstream>
#include <exception>

using namespace std;

class Exception : public exception {
public:
	Exception (const string & what_arg) : what_arg (what_arg) {}

	Exception (const char * what_arg) : what_arg (what_arg) {}

	const char * what () const noexcept override {
		return what_arg.c_str ();
	}
private:
	string what_arg;
};

#define GEDI_API_EXCEPTION(funcName, what) \
	[&] () -> Exception { \
		ostringstream os; \
		os << "[gedi::" << funcName << " @ " << __FILE__ << ':' << __LINE__ \
				<< "] " << what; \
		return move (Exception (move (os.str ()))); \
	} ()
