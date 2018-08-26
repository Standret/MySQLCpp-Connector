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
		SQLCustomDriver driver("tcp://127.0.0.1:3306", "root", "Zasada_19sqlroot");
		driver.use_db("labwork1_standret");

		auto res = driver.get_object<Soundtrack>("SELECT * FROM SOUNDTRACK", FieldFactory::get_soundtrack_field());
		//int i = res[0].get_value<int>(FieldFactory::get_soundtrack_field()[0]);
		Soundtrack ss;
		ss.duration = 999;
		ss.name = "999";
		ss.owner = "999";
		ss.publish = "1999-12-12";
		ss.rating = 4.5;
		std::vector<Storable*> obj{ &ss };
		auto update = driver.set_object(obj);
	}
	catch (sql::SQLException &e) {
		std::cout << "# ERR: SQLException in " << __FILE__;
		std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
		std::cout << "# ERR: " << e.what();
		std::cout << " (MySQL error code: " << e.getErrorCode();
		std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
	}


	return 0;
}