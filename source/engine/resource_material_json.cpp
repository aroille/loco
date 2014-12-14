#include "resource_material_json.h"
#include "loco.h"
#include "jsonxx/jsonxx.h"
#include "debug.h"

namespace loco
{
	void load_material(const Memory* mem, Material* mat)
	{		
		const char* vs_param("vs_shader");
		const char* ps_param("ps_shader");
		
		// create input for jsonxx
		std::string input;
		input.append((char*)(mem->data), mem->size);
		
		// json parsing
		jsonxx::Object o;
		o.parse(input);
		
		// check shader parameter existance
		LOCO_ASSERTF(o.has<jsonxx::String>(vs_param), "Material parsing error, \"%s\" parameter is missing", vs_param);
		LOCO_ASSERTF(o.has<jsonxx::String>(ps_param), "Material parsing error, \"%s\" parameter is missing", ps_param);

		// Assign shaders
		Shader vs_shader = loco::resources.get<Shader>(o.get<jsonxx::String>(vs_param).c_str());
		Shader ps_shader = loco::resources.get<Shader>(o.get<jsonxx::String>(ps_param).c_str());
		mat->set_shader(vs_shader, ps_shader);

		// Set material parameters
		std::vector<float> data;
		Texture texture;
		renderer::UniformType::Enum uniform_type;
		uint8_t uniform_array_size;

		auto i = o.kv_map().begin();
		for (i; i != o.kv_map().end(); i++)
		{
			if (strcmp(i->first.c_str(), vs_param)==0 || strcmp(i->first.c_str(), ps_param)==0)
				continue;

			switch (i->second->type_)
			{
				data.clear();

				case jsonxx::Value::NUMBER_:
					data.push_back((float)i->second->number_value_);
					mat->set(i->first.c_str(), renderer::UniformType::Float, data.data(), 1);
					break;

				case jsonxx::Value::BOOL_:
					data.push_back((float)i->second->bool_value_);
					mat->set(i->first.c_str(), renderer::UniformType::Float, data.data(), 1);
					break;

				case jsonxx::Value::STRING_:
					texture = loco::resources.get<Texture>(i->second->string_value_->c_str());
					mat->set(i->first.c_str(), texture);
					break;

				case jsonxx::Value::ARRAY_:
					switch (i->second->array_value_->size())
					{
						case 2:
							uniform_type = renderer::UniformType::Vector2;
							uniform_array_size = 1;
							break;

						case 3:
							uniform_type = renderer::UniformType::Vector3;
							uniform_array_size = 1;
							break;

						case 4:
							uniform_type = renderer::UniformType::Vector4;
							uniform_array_size = 1;
							break;

						case 9:
							uniform_type = renderer::UniformType::Matrix3x3;
							uniform_array_size = 1;
							break;

						case 16:
							uniform_type = renderer::UniformType::Matrix4x4;
							uniform_array_size = 1;
							break;

						default:
							uniform_type = renderer::UniformType::Float;
							uniform_array_size = i->second->array_value_->size();
							break;
					}

					for (uint32_t j = 0; j < i->second->array_value_->size(); j++)
					{
						LOCO_ASSERTF(i->second->array_value_->has<jsonxx::Number>(j), "Material parsing error, parameter \"%s\" is an array of unsupported types", i->first.c_str());
						data.push_back((float)i->second->array_value_->get<jsonxx::Number>(j));
					}

					mat->set(i->first.c_str(), uniform_type, data.data(), uniform_array_size);
					break;

				default:
					LOCO_ASSERTF(false, "Material parsing error, parameter \"%s\" type is not supported ", i->first.c_str());
					break;
			}
		}

		return;
	}
}

