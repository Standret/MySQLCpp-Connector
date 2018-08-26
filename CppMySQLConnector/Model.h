#pragma once

#include <string>
#include <vector>
#include <map>

#include "Driver.h"

class Soundtrack : public stt::Storable
{
public:
	int id;
	std::string name;
	std::string owner;
	int duration;
	long double rating;
	std::string publish;

	Soundtrack();
	Soundtrack(std::map <std::string, stt::FieldData> data);
	Soundtrack(const char *name, const char *owner, int duration, long double rating, const char *publish);

	// Inherited via Storable
	virtual std::vector<stt::Field> get_all_field(bool include_id = true) const override;
	virtual std::string get_value(const stt::Field & field) const override;
	virtual const char *get_unique_id() const override;
};

class FieldFactory
{
public:
	static std::vector<stt::Field> get_soundtrack_field(bool include_id = true);
};
