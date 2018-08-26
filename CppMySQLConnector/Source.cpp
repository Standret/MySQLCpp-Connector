#include <stdlib.h>
#include <iostream>
#include <memory>

#include <string>
#include <map>
#include <vector>
#include <ctime>

#include "Driver.h"
#include "Model.h"

using namespace stt;

int main(int argc, char* argv[])
{
	try
	{
		// create connection
		SQLCustomDriver driver("adress", "username", "password");
		driver.use_db("labwork1_standret");

		// get object (the tabe sgould be created)
		auto res = driver.get_object<Soundtrack>("SELECT * FROM SOUNDTRACK", FieldFactory::get_soundtrack_field());

		Soundtrack moskau("Moskau", "Dschinghis Khan", 267, 4.5, "1979-01-01"), rasputin("RASPUTIN", "The Essential Boney", 265, 5, "1978-01-01");

		// set object recieve Storable object (your class must inherit from Storable and implement all virtual method)
		std::vector<Storable*> obj{ &moskau, &rasputin };

		// true - objects have been set successfully
		// false - object is empty or other error
		// SQL throw error (you need to catch it by yourself)
		auto update = driver.set_object(obj);
	}
	catch (sql::SQLException &e) {
		// default tracker for show error
		std::cout << "# ERR: SQLException in " << __FILE__;
		std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
		std::cout << "# ERR: " << e.what();
		std::cout << " (MySQL error code: " << e.getErrorCode();
		std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
	}


	return 0;
}