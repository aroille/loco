#ifndef CAMERA_SYSTEM_H_HEADER_GUARD
#define CAMERA_SYSTEM_H_HEADER_GUARD

#include "entity.h"
#include "handle.h"
#include "math_types.h"
#include <unordered_map>

namespace loco
{
	using namespace math;

	struct CameraComponent
	{
		static const CameraComponent null;

		HandleI24G8 handle;
		inline uint32 index() { return handle.index(); }
	};

	/// Manager of camera components
	/// (one cameraSystem per world)
	class CameraSystem
	{
	public:

		struct ProjectionType
		{
			enum Enum
			{
				Perspective,
				Orthographic
			};
		};

	public:

		CameraSystem();
		~CameraSystem();

		/// Attach a new camera component to entity e
		///
		/// @remarks 
		/// Only one camera component per entity is allowed 
		///
		CameraComponent create(Entity e);

		/// Get camera component attached to entity e
		CameraComponent lookup(Entity e) const;

		/// Check if component c is valid
		bool is_valid(CameraComponent c) const;

		/// Free the camera component attached to entity e
		void destroy(Entity e);

		/// Garbage collector
		void gc(const EntityManager& em);

		/// Get projection matrix
		Matrix4x4 projection_matrix(CameraComponent c, float aspect_ratio) const;

		/// Get projection type
		ProjectionType::Enum projection_type(CameraComponent c) const;

		/// Set projection type
		void set_projection_type(CameraComponent c, ProjectionType::Enum p);

		/// Get near distance
		float near_distance(CameraComponent c) const;

		/// Set near distance
		void set_near_distance(CameraComponent c, float d);

		/// Get far distance
		float far_distance(CameraComponent c)const;

		/// Set far distance
		void set_far_distance(CameraComponent c, float d);

		/// Get fov
		float fov(CameraComponent c)const;

		/// Set fov
		void set_fov(CameraComponent c, float fov);

		/// Get ortho size
		float ortho_size(CameraComponent c) const;

		/// Set ortho size
		void set_ortho_size(CameraComponent c, float size);

	private:

		struct CameraParameters
		{
			ProjectionType::Enum proj;
			float near_distance;
			float far_distance;
			union
			{
				float vertical_fov; // perspective camera only
				float size;			// orthographic camera only
			};
		};

		struct ComponentData
		{	
			unsigned size;					///< Number of used instance
			unsigned capacity;				///< Number of allocated instance in arrays
			char* buffer;					///< Buffer with instance data

			CameraParameters*	param;		///< Camera parameters
			Entity*				entity;		///< Entity
			CameraComponent*			component;  ///< Component

			unsigned*			lut;		///< Look up table : Component/Data_index
		};
		ComponentData _data;

		/// Handle manager (Component = handle)
		HandleManagerI24G8	_handle_mgr;

		/// Map an Entity (key) with a Component (value)
		std::unordered_map<unsigned, CameraComponent> _map;

		/// Return a _data index from a component
		inline unsigned data_index(CameraComponent c) const
		{
			return _data.lut[c.index()];
		};

		/// Increase the capicity of the data buffer to an component count of sz
		void allocate(unsigned sz);

		/// Move a component at index 'from' to a new location at index 'to'
		/// The memory at index 'to' shoudn't be used by another component before the move;
		void move_instance(unsigned from, unsigned to);
	};
}

#endif // CAMERA_SYSTEM_H_HEADER_GUARD