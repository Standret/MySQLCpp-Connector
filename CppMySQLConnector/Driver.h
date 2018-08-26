#pragma once

#include <string>
#include <ctime>

#include <string>
#include <sstream>
#include <map>
#include <vector>

#include <mysql_connection.h>
#include <mysql_driver.h>
#include <mysql.h>

#include <cppconn/resultset.h>
#include <cppconn/resultset_metadata.h>
#include <cppconn/statement.h>

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

#include <boost/format.hpp>

namespace stt
{
	class FieldSettings
	{
	public:
		enum FieldType
		{
			intType = 1 << 0,
			doubleType = 1 << 1,
			dateType = 1 << 2,
			varchar = 1 << 3,
			primaryKey = 1 << 4
		};

		int varchar_length = -1;

		FieldSettings(int type, int varchar_length = -1);

		bool has_type(FieldType type) const;
		
	private:	
		int type;
	};

	class Field
	{
	public:
		std::string label_name;
		FieldSettings settings;

		Field(const std::string &name, const FieldSettings &settings);
	};

	struct FieldData
	{
		int int_value;
		long double double_value;
		std::string string_value;
		time_t time_value;

		FieldData();
		FieldData(int int_value);
		FieldData(long double double_value);
		FieldData(const std::string &string_value);
		FieldData(time_t time_value);
	};

	class Storable
	{
	public:
		virtual std::vector<stt::Field> get_all_field(bool include_id = true) const = 0;
		virtual std::string get_value(const stt::Field &field) const = 0;
		virtual const char *get_unique_id() const = 0;
		virtual ~Storable() = default;
	};

	class SQLCustomDriver
	{
	public:
		SQLCustomDriver(const char *address, const char *user_name, const char *password);
		void use_db(const char *name);

		template<class T>
		std::vector<T> get_object(const char *query, const std::vector<Field> &fields) {
			std::vector<std::map<std::string, FieldData>> sql_result;

			auto res_ptr = stmt->executeQuery(query);

			while (res_ptr->next()) {
				std::map<std::string, FieldData> local_result;
				for (int i = 0; i < fields.size(); ++i) {
					if (fields[i].settings.has_type(FieldSettings::dateType) || fields[i].settings.has_type(FieldSettings::varchar)) {
						std::istream *ptr = res_ptr->getBlob(fields[i].label_name.c_str());
						char *tempArr = new char[fields[i].settings.varchar_length]; // todo: think about const buffer
						ptr->getline(tempArr, fields[i].settings.varchar_length);
						local_result[fields[i].label_name] = FieldData(std::string(tempArr));
						delete[] tempArr;
					}
					if (fields[i].settings.has_type(FieldSettings::intType))
						local_result[fields[i].label_name] = FieldData(res_ptr->getInt(fields[i].label_name.c_str()));
					else if (fields[i].settings.has_type(FieldSettings::doubleType))
						local_result[fields[i].label_name] = FieldData(res_ptr->getDouble(fields[i].label_name.c_str()));
				}
				sql_result.push_back(local_result);
			}

			std::vector<T> result;
			for (auto item : sql_result) {
				result.push_back(T(item));
			}

			return result;
		}

		bool set_object(std::vector<Storable*> models) const
		{
			if (!models.empty()) {
				auto all_target_field = models[0]->get_all_field(false);
				if (all_target_field.empty())
					throw std::exception("all_target_field is empty");

				std::stringstream query;
				query << "INSERT INTO " << models[0]->get_unique_id() << " (" << all_target_field[0].label_name;
				for (int i = 1; i < all_target_field.size(); ++i)
					query << ", " + all_target_field[i].label_name;
				query << ") VALUES ";

				bool isInsertDot = true;
				for (int i = 0; i < models.size(); ++i) {
					query << "(";
					for (int j = 0; j < all_target_field.size(); ++j) {
						bool is_str = all_target_field[j].settings.has_type(stt::FieldSettings::varchar) || all_target_field[j].settings.has_type(stt::FieldSettings::dateType);
						query << (is_str ? "'" : "") <<  models[i]->get_value(all_target_field[j]) << (is_str ? "'" : "") << (j < all_target_field.size() - 1 ? ", " : "");
					}
					query << ")" << (i < models.size() - 1 ? ", " : "");
				}
				
				auto str = query.str();
				auto res_ptr = stmt->executeUpdate(query.str().c_str());
				return true;
			}
			return false;
		}

	private:
		sql::mysql::MySQL_Driver *driver;
		sql::Connection *con;
		sql::Statement *stmt;
	};
}