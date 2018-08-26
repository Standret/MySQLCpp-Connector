#include "Model.h"

Soundtrack::Soundtrack() { }

Soundtrack::Soundtrack(std::map<std::string, stt::FieldData> data)
{
	id = data["id"].int_value;
	name = data["name"].string_value;
	owner = data["owner"].string_value;
	duration = data["duration"].int_value;
	rating = data["rating"].double_value;
	publish = data["publish"].string_value;
}

Soundtrack::Soundtrack(const char *name, const char *owner, int duration, long double rating, const char *publish) : name(name), owner(owner), duration(duration), rating(rating), publish(publish) { }

std::vector<stt::Field> FieldFactory::get_soundtrack_field(bool include_id)
{
	std::vector<stt::Field> result {
		stt::Field("id", stt::FieldSettings(stt::FieldSettings::intType | stt::FieldSettings::primaryKey)),
		stt::Field("name", stt::FieldSettings(stt::FieldSettings::varchar, 50)),
		stt::Field("owner", stt::FieldSettings(stt::FieldSettings::varchar, 50)),
		stt::Field("duration", stt::FieldSettings(stt::FieldSettings::intType)),
		stt::Field("rating", stt::FieldSettings(stt::FieldSettings::doubleType)),
		stt::Field("publish", stt::FieldSettings(stt::FieldSettings::dateType, 20))
	};

	int targetInclude = -1;
	if (!include_id) {
		for (int i = 0; i < result.size(); ++i) {
			if (result[i].settings.has_type(stt::FieldSettings::primaryKey)) {
				result.erase(result.begin() + i);
			}
		}
	}
	return result;
}

inline const char *Soundtrack::get_unique_id() const
{
	return "Soundtrack";
}

std::vector<stt::Field> Soundtrack::get_all_field(bool include_id) const
{
	return FieldFactory::get_soundtrack_field(false);
}

std::string Soundtrack::get_value(const stt::Field &field) const
{
	if (field.label_name == "id")
		return std::to_string(id);
	else if (field.label_name == "name")
		return name;
	else if (field.label_name == "owner")
		return owner;
	else if (field.label_name == "duration")
		return std::to_string(duration);
	else if (field.label_name == "rating")
		return std::to_string(rating);
	else if (field.label_name == "publish")
		return publish;

	throw std::exception("field not found");
}
