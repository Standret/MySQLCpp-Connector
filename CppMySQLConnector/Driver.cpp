#include "Driver.h"

namespace stt
{
	FieldSettings::FieldSettings(int type, int varchar_length) : type(type), varchar_length(varchar_length) { }

	bool FieldSettings::has_type(FieldType type) const
	{
		return this->type & type;
	}

	Field::Field(const std::string & name, const FieldSettings &settings) : label_name(name), settings(settings) { }

	FieldData::FieldData() { }
	FieldData::FieldData(int int_value) : int_value(int_value) { }
	FieldData::FieldData(long double double_value) : double_value(double_value) { }
	FieldData::FieldData(const std::string & string_value) : string_value(string_value) { }
	FieldData::FieldData(time_t time_value) : time_value(time_value) { }

	SQLCustomDriver::SQLCustomDriver(const char * address, const char * user_name, const char * password)
	{
		driver = sql::mysql::get_mysql_driver_instance();
		con = driver->connect(address, user_name, password);
	}
	void SQLCustomDriver::use_db(const char * name)
	{
		con->setSchema(name);
		stmt = con->createStatement();
	}
}