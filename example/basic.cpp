/**
 * example.cpp
 * Shows simple example usage of GetOpt::getopt.
 * Authors: Erich Gubler, erichdongubler@gmail.com
 */
#include "../include/getopt.h"

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;
#include <string>
using std::string;
#include <vector>
using std::vector;
#include <tuple>
using std::get;

int main(int argc, char** argv)
{
	// Typical idiom, as Andrei says, is to declare with defaults first
	string data = "default";
	int length = 0;
	int verbosity = false;
	bool usePrettyOutput = false;
	string color = "none";
	
	// C++-friend version of interface returns a tuple with the result and a vector of remaining args
	// The tuple type is aliased to "GetOptResultTuple"
	GetOpt::GetOptResultAndArgs results;
	try
	{
		results = GetOpt::getopt(argc, argv

		// Define short opt "l" and long opt "length" for int length. Note the following:
			// * accepts args of the forms:
			//   * ["-l24"]
			//   * ["-l", "24"]
			//   * ["--length=24"]
			//   * ["--length", "24"]
			// * opt specifications are delimited by '|'
			// * The value given in args must be parseable to an int, obviously!
			// * Repeat definitions of an opt will override the previous value, so ["-l", "20", "-l300"] would result in length == 300.
		, "l|length", &length

		// Define short opt "f" and long opt "file" for string data
		, "file|f", &data

		// Uncomment this config value to allow unrecognized opts on the command line starting from this point.
		// config::passThrough is useful for creating modular getopt calls.

		/*, GetOpt::config::passThrough*/

		// Notice: if I pass config::passThrough in here, unrecognized args found previous to it will still
		//     cause getopt to throw an exception. If I passed in passThrough here, it would still expect
		//     length and file to come before any unrecognized args.

		// Define incremental opt for verbosity; long is "verbose", short is "v"
		// Integral types can be used as incremental opts, which count the number of occurences of the flag a la bool.
		// For instance, "-v --verbose -v" at the command line would set verbosity to 3.
		, "verbose|v+", &verbosity 

		// Define short opt "c" and two long opts "color" and "colour" for string color
		, "color|c|colour", &color

		// This is like the others, except I *won't* need to provide a value normally.
		// Bools are accepted of the following forms:
		// * ["--pretty"]
		// * ["-p"]
		// * ["-ptrue"]
		// * ["-pfalse"]
		// * ["--pretty=true"]
		// * ["--pretty=false"]

		// However, the following forms skip the second arg:
		// * ["--pretty", "true"]
		// * ["-p", "true"]
		, "pretty|p", &usePrettyOutput);
	}
	catch(GetOpt::GetOptException e)
	{
		cerr << e.what() << endl;
		return 1;
	}

	// Print variables; they've been mutated based upon arg input from before!
	cout << "length: " << length << endl; 
	cout << "data: " << data << endl;
	cout << "verbosity: " << verbosity << endl;
	cout << "color: " << color << endl;
	cout << "usePrettyOutput: " << (usePrettyOutput ? "true" : "false") << endl;

	// Check to see if the user wanted help; if so, print out automagic help
	if(results.result.helpWanted)
		defaultGetoptPrinter("Here's valid args for this program:", results.result.options);

	// Print remaining args (which were removed during args parsing for convenience)
	// Comment out this line and the cpputils include if you want to quickly 
	cout << "Args remaining: ";
	auto begin = results.args.begin();
	auto end = results.args.end();
	cout << '[';
	if(begin != end)
		cout << *begin++;
	while(begin != end)
		cout << ", " << *begin++;
	cout << ']' << endl;
	return 0;
}