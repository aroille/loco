#ifndef SAMPLE_COMMON_H_HEADER_GUARD
#define SAMPLE_COMMON_H_HEADER_GUARD

#include "loco.h"
#include "world.h"
#include "entry.h"

loco::Entity create_axis(loco::World& world, float length, float thickness);

void camera_update(loco::Entity camera, loco::World& world, loco::GameInput* input, float delta_time);

#endif // SAMPLE_COMMON_H_HEADER_GUARD

