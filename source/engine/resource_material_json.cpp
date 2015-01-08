#include "resource_material_json.h"
#include "loco.h"
#include "jsonxx/jsonxx.h"
#include "debug.h"

namespace loco
{
namespace resource
{

	bool load_material(const Memory* mem, MaterialData* mat, const DefaultResources& default_resources)
	{
		const char* vs_param_name("vs_shader");
		const char* ps_param_name("ps_shader");

		std::string vs_param_value;
		std::string ps_param_value;

		// create input for jsonxx
		std::string input;
		input.append((char*)(mem->data), mem->size);

		// json parsing
		jsonxx::Object o;
		o.parse(input);

		// check vertex shader parameter existance
		if (!o.has<jsonxx::String>(vs_param_name))
		{
			log.error(LOCO_LOG_RESOURCE_MANAGER, "Material parsing error, \"%s\" parameter is missing", vs_param_name);
			return false;
		}

		// check pixel shader parameter existance
		if (!o.has<jsonxx::String>(ps_param_name))
		{
			log.error(LOCO_LOG_RESOURCE_MANAGER, "Material parsing error, \"%s\" parameter is missing", ps_param_name);
			return false;
		}

		// Assign shaders
		Shader vs_shader = loco::resource_manager.get<Shader>(o.get<jsonxx::String>(vs_param_name).c_str());
		Shader ps_shader = loco::resource_manager.get<Shader>(o.get<jsonxx::String>(ps_param_name).c_str());
		if (vs_shader == Shader::invalid || ps_shader == Shader::invalid)
		{
			vs_shader = default_resources.vertex_shader;
			ps_shader = default_resources.pixel_shader;
			log.error(LOCO_LOG_RESOURCE_MANAGER, "Use of default vertex and pixel shaders", ps_param_name);
		}

		mat->set_shader(vs_shader, ps_shader);

		// Set material parameters
		std::vector<float> data;
		Texture texture;
		Renderer::UniformType::Enum uniform_type;
		uint8_t uniform_array_size;

		auto i = o.kv_map().begin();
		for (i; i != o.kv_map().end(); i++)
		{
			if (strcmp(i->first.c_str(), vs_param_name) == 0 || strcmp(i->first.c_str(), ps_param_name) == 0)
				continue;

			switch (i->second->type_)
			{
				data.clear();

			case jsonxx::Value::NUMBER_:
				data.push_back((float)i->second->number_value_);
				mat->set(i->first.c_str(), Renderer::UniformType::Float, data.data(), 1);
				break;

			case jsonxx::Value::BOOL_:
				data.push_back((float)i->second->bool_value_);
				mat->set(i->first.c_str(), Renderer::UniformType::Float, data.data(), 1);
				break;

			case jsonxx::Value::STRING_:
				texture = loco::resource_manager.get<Texture>(i->second->string_value_->c_str());
				mat->set(i->first.c_str(), texture);
				break;

			case jsonxx::Value::ARRAY_:
				switch (i->second->array_value_->size())
				{
				case 2:
					uniform_type = Renderer::UniformType::Vector2;
					uniform_array_size = 1;
					break;

				case 3:
					uniform_type = Renderer::UniformType::Vector3;
					uniform_array_size = 1;
					break;

				case 4:
					uniform_type = Renderer::UniformType::Vector4;
					uniform_array_size = 1;
					break;

				case 9:
					uniform_type = Renderer::UniformType::Matrix3x3;
					uniform_array_size = 1;
					break;

				case 16:
					uniform_type = Renderer::UniformType::Matrix4x4;
					uniform_array_size = 1;
					break;

				default:
					uniform_type = Renderer::UniformType::Float;
					uniform_array_size = (uint8_t)i->second->array_value_->size();
					break;
				}

				for (uint32_t j = 0; j < i->second->array_value_->size(); j++)
				{
					if (!i->second->array_value_->has<jsonxx::Number>(j))
					{
						log.error(LOCO_LOG_RESOURCE_MANAGER, "Material parsing error, parameter \"%s\" is an array of unsupported types", i->first.c_str());
						break;
					}
					data.push_back((float)i->second->array_value_->get<jsonxx::Number>(j));
				}

				mat->set(i->first.c_str(), uniform_type, data.data(), uniform_array_size);
				break;

			default:
				log.error(LOCO_LOG_RESOURCE_MANAGER, "Material parsing error, parameter \"%s\" type is not supported ", i->first.c_str());
				break;
			}
		}

		return true;
	}
} // resource
} // loco

