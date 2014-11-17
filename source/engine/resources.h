#ifndef RESOURCES_H_HEADER_GUARD
#define RESOURCES_H_HEADER_GUARD

namespace loco
{
	namespace resources
	{
		void init(char* root_folder_path);

		/// Load all resources in path (recursive) + create the equivalent gpu resources
		/// Return the number of resources loaded
		unsigned load_folder(char* folder_path);

		//Mesh get_mesh(char* resource_path);
		//Texture get_texture(char* resource_path);
		//VertexShader get_vertex_shader(char* resource_path);
		//PixelShader get_pixel_shader(char* resource_path);
		//ComputeShader get_compute_shader(char* resource_path);
		//Material get_material(char* resource_path);
	};
}

#endif // RESOURCES_H_HEADER_GUARD