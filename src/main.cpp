#include <iostream>
#include <memory>
#include <cassert>
#include "ThreadSafeVar.hpp"

using namespace ThreadSafe;

int	main()
{
	/*
	** Example with a simple int
	*/
	// Creation
	Var<int> safe_int = 6;

	// get a copy of the value with load
	assert(safe_int.load() == 6);

	// Assigment
	safe_int = 9;

	// Get a copy of the value with the implicit cast operator
	assert(safe_int == 9);

	/*
	** Example with a non copyable but movable object
	*/
	// Creation
	Var<std::unique_ptr<int>> safe_ptr(std::make_unique<int>(8));

	// We can't get a copy of the pointer but we can access the pointed value with the method "access"
	// The return value of the callback can't be a pointer or a reference
	// because it would expose the protected value to be accessed without using the mutex
	// be carefull to not create access methods of "safe_ptr" only the value parameter in the callback it could create a deadlock
	int test_value = safe_ptr.access(
		[](const std::unique_ptr<int>& ptr) -> int
		{
			return *ptr;
		}
	);
	assert(test_value == 8);

	// This does not compile because the return type of the callback is checked at compile time
	/* auto test2 = b.access(
		[](const int& value) -> const int&
		{
			return value;
		}
	); */

	// Assigment also works with movable object
	safe_ptr = std::make_unique<int>(9);

	// We can also use "access_compliant" to access safely to the data
	// the difference with a access is it accepts callback that return pointer or reference
	// but it alters the return value and remove the reference or pointer from the signature of the callback
	auto test_value_bis = safe_ptr.access_compliant(
		[](const std::unique_ptr<int>& ptr) -> int&
		{
			return *ptr;
		}
	);
	assert(test_value_bis == 9);

	// We can check that the test_value_bis is really an int and not a int&
	bool is_it_same = std::is_same<int, decltype(test_value_bis)>();
	assert(is_it_same == true);

	// We can also swap the the content
	auto ptr_to_swap = std::make_unique<int>(12);
	safe_ptr.swap(ptr_to_swap);
	assert(*ptr_to_swap == 9);


	// If we want to extract the value we can do it
	// but the Var object can't be used after because the protected value was moved during extraction
	std::unique_ptr<int> extracted_ptr = safe_ptr.extract();
	assert(*extracted_ptr == 12);

	std::cout << "Well apparently there's no bug! :D" << std::endl;

	return 0;
}
