#ifndef _ODE_H
#define _ODE_H
#include "../ode.h"

/*=
Integrated Physics (ODE)
3900
**/

// Introduction
/**
Raydium allows you to build applications with full physics, using ODE (Open
Dynamics Engine). ODE is "an open source, high performance library for 
simulating rigid body dynamics", and is fully integrated into Raydium, with
the usual abstraction. You can build cars, ragdolls, rockets, ... with
only few lines of code. Physics are linked to sound API, particles engine,
network layer, ... so you've almost nothing else to do but setting up objects.

Raydium's website provides tutorials for building physics ready applications.
**/

// Vocabulary
/**
Raydium physics use a simple vocabulary, with a few entities :
- **Objects:**
Objects are containers, with no direct visual appearance. An object contains
elements and joints (see below). By default, all elements in an object
doesn't collide each others. "Car", "Player", "Crane" are good object examples.

- **Elements:**
Elements are the main thing you will play with. An element is rendered using
an associated 3D mesh, is configured with a density, a size, collides with
others elements, ...
An element **must** be owned by an object.
For now, there is 3 element types (standard, satic, fixing). Static elements
are unmovable, they just collide with other elements, usefull for very
big elements, or externally controlled elements (motion capture, network, 
haptic interface, ...), for example.


- **Joints:**
Joints are dedicated to elements linking. A joint **must** be linked with two
elements or unwanted behaviors may happen.
For now, Raydium supports 4 joint types (hinge, hinge2, universal, fixed), and 
you will find more informations with suitable functions documentation, below.
On a joint, you could setup limits (min and max for all axes) and a maximum
force before joint breaks, if needed.


- **Motors:**
A motor is linked to joints, and may powering an unlimited amount of joints.
For now, 3 motor types are available: engine, angular and rocket.

**Engine** type works the same way as a car's engine: it will try to 
make "something" turning, at the desired **speed**. You can link a 
gearbox to this type (and only this one).

**Angular** type will try to rotate "something" to the desired **angle**,
usefull for car's front wheels, for example.

**Rocket type** is very simple: give a force and an orientation. Usefull for
creating copters, rockets, and for elements "pushing", for example. 
Special rocket is avaiblable for FPS style player controls.
Warning, a rocket is linked to an element ! (not a joint)


- **Explosions:**
Explosions are not directly related to rigid body physics, but consider it
as a high level primitive. 
With Raydium, you have two different ways to create an explosion.

First, you can create a "blowing explosion", generating a spherical blow. Any
element in this growing sphere will be ejected. 
Use this for very consequent explosions only !

Next, you can create an instantaneous explosion, with a degressive blowing
effect. A force is applied to every body found inside the blowing radius,
proportional to distance from the explosion's center. Usefull for smaller
explosions.

- **Launchers:**
Launchers are not real entities, but "only" tools. Obviously, they are
allowing you to launch an element (you must provice force and orientation)
from another element (relatively). More informations about launchers below.

Raydium provides some other functions for advanced uses, and you can
access directly to ODE API for very experienced users.
**/

extern void raydium_ode_name_auto (char *prefix, char *dest);
extern void raydium_ode_init_object (int i);
extern void raydium_ode_init_element (int i);
extern void raydium_ode_init_joint (int i);
extern void raydium_ode_init_motor (int i);
extern void raydium_ode_init_explosion (int e);
extern void raydium_ode_init (void);
extern char raydium_ode_object_isvalid (int i);
extern char raydium_ode_element_isvalid (int i);
extern char raydium_ode_joint_isvalid (int i);
extern char raydium_ode_motor_isvalid (int i);
extern char raydium_ode_explosion_isvalid (int i);
extern void raydium_ode_ground_dTriArrayCallback (dGeomID TriMesh, dGeomID RefObject, const int *TriIndices, int TriCount);
extern int raydium_ode_ground_dTriCallback (dGeomID TriMesh, dGeomID RefObject, int TriangleIndex);
extern void raydium_ode_ground_set_name (char *name);
extern int raydium_ode_object_find (char *name);
extern int raydium_ode_element_find (char *name);
extern int raydium_ode_joint_find (char *name);
extern int raydium_ode_motor_find (char *name);
extern int raydium_ode_explosion_find (char *name);
extern int raydium_ode_object_create (char *name);
extern char raydium_ode_object_rename (int o, char *newname);
extern char raydium_ode_object_rename_name (char *o, char *newname);
extern char raydium_ode_object_colliding (int o, char colliding);
extern char raydium_ode_object_colliding_name (char *o, char colliding);
extern void raydium_ode_object_linearvelocity_set (int o, dReal * vect);
extern void raydium_ode_object_linearvelocity_set_name (char *o, dReal * vect);
extern void raydium_ode_object_linearvelocity_set_name_3f (char *o, dReal vx, dReal vy, dReal vz);
extern void raydium_ode_object_addforce (int o, dReal * vect);
extern void raydium_ode_object_addforce_name (char *o, dReal * vect);
extern void raydium_ode_object_addforce_name_3f (char *o, dReal vx, dReal vy, dReal vz);
extern void raydium_ode_element_addforce (int e, dReal * vect);
extern void raydium_ode_element_addforce_name (char *e, dReal * vect);
extern void raydium_ode_element_addforce_name_3f (char *e, dReal vx, dReal vy, dReal vz);
extern void raydium_ode_element_addtorque (int e, dReal * vect);
extern void raydium_ode_element_addtorque_name (char *e, dReal * vect);
extern void raydium_ode_element_addtorque_name_3f (char *e, dReal vx, dReal vy, dReal vz);
extern char raydium_ode_element_material (int e, dReal erp, dReal cfm);
extern char raydium_ode_element_material_name (char *name, dReal erp, dReal cfm);
extern char raydium_ode_element_slip (int e, dReal slip);
extern char raydium_ode_element_slip_name (char *e, dReal slip);
extern char raydium_ode_element_rotfriction (int e, dReal rotfriction);
extern char raydium_ode_element_rotfriction_name (char *e, dReal rotfriction);
extern dReal *raydium_ode_element_linearvelocity_get (int e);
extern dReal *raydium_ode_element_linearvelocity_get_name (char *e);
extern void raydium_ode_element_OnBlow (int e, void *OnBlow);
extern void raydium_ode_element_OnBlow_name (char *e, void *OnBlow);
extern void raydium_ode_element_OnDelete (int e, void *OnDelete);
extern void raydium_ode_element_OnDelete_name (char *e, void *OnDelete);
extern void raydium_ode_element_gravity (int e, char enable);
extern void raydium_ode_element_gravity_name (char *e, char enable);
extern void raydium_ode_element_ttl_set (int e, int ttl);
extern void raydium_ode_element_ttl_set_name (char *e, int ttl);
extern char raydium_ode_element_aabb_get (int element, dReal * aabb);
extern char raydium_ode_element_aabb_get_name (char *element, dReal * aabb);
extern int raydium_ode_element_touched_get (int e);
extern int raydium_ode_element_touched_get_name (char *e);
extern char raydium_ode_element_player_set (int e, char isplayer);
extern char raydium_ode_element_player_set_name (char *name, char isplayer);
extern char raydium_ode_element_player_get (int e);
extern char raydium_ode_element_player_get_name (char *name);
extern char raydium_ode_element_player_angle (int e, dReal angle);
extern char raydium_ode_element_player_angle_name (char *e, dReal angle);
extern int raydium_ode_element_ground_texture_get (int e);
extern int raydium_ode_element_ground_texture_get_name (char *e);
extern int raydium_ode_element_object_get (int e);
extern int raydium_ode_element_object_get_name (char *e);
extern int raydium_ode_object_sphere_add (char *name, int group, dReal mass, dReal radius, char type, int tag, char *mesh);
extern int raydium_ode_object_box_add (char *name, int group, dReal mass, dReal tx, dReal ty, dReal tz, char type, int tag, char *mesh);
extern int raydium_ode_element_fix (char *name, int *elem, int nelems, char keepgeoms);
extern void raydium_ode_element_unfix (int e);
extern void raydium_ode_element_move (int elem, dReal * pos);
extern void raydium_ode_element_move_3f(int elem, dReal x,dReal y, dReal z);
extern void raydium_ode_element_move_name (char *name, dReal * pos);
extern void raydium_ode_element_move_name_3f (char *name, dReal x, dReal y, dReal z);
extern void raydium_ode_element_rotate (int elem, dReal * rot);
extern void raydium_ode_element_rotate_3f (int elem, dReal rx, dReal ry, dReal rz);
extern void raydium_ode_element_rotate_name (char *name, dReal * rot);
extern void raydium_ode_element_rotateq (int elem, dReal * rot);
extern void raydium_ode_element_rotateq_name (char *name, dQuaternion rot);
extern void raydium_ode_element_rotate_name_3f (char *name, dReal rx, dReal ry, dReal rz);
extern void raydium_ode_object_rotate(int obj, dReal *rot);
extern void raydium_ode_object_rotate_name(char *obj, dReal *rot);
extern void raydium_ode_object_rotate_name_3f(char *obj, dReal rx, dReal ry, dReal rz);
extern void raydium_ode_element_rotate_direction (int elem, char Force0OrVel1);
extern void raydium_ode_element_rotate_direction_name (char *e, char Force0OrVel1);
extern void raydium_ode_element_data_set (int e, void *data);
extern void raydium_ode_element_data_set_name (char *e, void *data);
extern void *raydium_ode_element_data_get (int e);
extern void *raydium_ode_element_data_get_name (char *e);
extern int raydium_ode_element_tag_get (int e);
extern int raydium_ode_element_tag_get_name (char *e);
extern void raydium_ode_object_move (int obj, dReal * pos);
extern void raydium_ode_object_move_name (char *name, dReal * pos);
extern void raydium_ode_object_move_name_3f (char *name, dReal x, dReal y, dReal z);
extern void raydium_ode_object_rotateq (int obj, dReal * rot);
extern void raydium_ode_object_rotateq_name (char *obj, dReal * rot);
extern void raydium_ode_joint_suspension (int j, dReal erp, dReal cfm);
extern void raydium_ode_joint_suspension_name (char *j, dReal erp, dReal cfm);
extern int raydium_ode_joint_attach_hinge2 (char *name, int elem1, int elem2, dReal axe1x, dReal axe1y, dReal axe1z, dReal axe2x, dReal axe2y, dReal axe2z);
extern int raydium_ode_joint_attach_hinge2_name (char *name, char *elem1, char *elem2, dReal axe1x, dReal axe1y, dReal axe1z, dReal axe2x, dReal axe2y, dReal axe2z);
extern int raydium_ode_joint_attach_universal (char *name, int elem1, int elem2, dReal posx, dReal posy, dReal posz, dReal axe1x, dReal axe1y, dReal axe1z, dReal axe2x, dReal axe2y, dReal axe2z);
extern int raydium_ode_joint_attach_universal_name (char *name, char *elem1, char *elem2, dReal posx, dReal posy, dReal posz, dReal axe1x, dReal axe1y, dReal axe1z, dReal axe2x, dReal axe2y, dReal axe2z);
extern int raydium_ode_joint_attach_hinge (char *name, int elem1, int elem2, dReal posx, dReal posy, dReal posz, dReal axe1x, dReal axe1y, dReal axe1z);
extern int raydium_ode_joint_attach_hinge_name (char *name, char *elem1, char *elem2, dReal posx, dReal posy, dReal posz, dReal axe1x, dReal axe1y, dReal axe1z);
extern int raydium_ode_joint_attach_fixed (char *name, int elem1, int elem2);
extern int raydium_ode_joint_attach_fixed_name (char *name, char *elem1, char *elem2);
extern void raydium_ode_joint_hinge_limits (int j, dReal lo, dReal hi);
extern void raydium_ode_joint_hinge_limits_name (char *j, dReal lo, dReal hi);
extern void raydium_ode_joint_universal_limits (int j, dReal lo1, dReal hi1, dReal lo2, dReal hi2);
extern void raydium_ode_joint_universal_limits_name (char *j, dReal lo1, dReal hi1, dReal lo2, dReal hi2);
extern void raydium_ode_joint_hinge2_block (int j, char block);
extern void raydium_ode_joint_hinge2_block_name (char *name, char block);
extern void raydium_ode_joint_delete_callback (int j, void (*f) (int));
extern void raydium_ode_joint_delete_callback_name (char *name, void (*f) (int));
extern void raydium_ode_joint_break_force (int j, dReal maxforce);
extern void raydium_ode_joint_break_force_name (char *name, dReal maxforce);
extern void raydium_ode_joint_elements_get (int j, int *e1, int *e2);
extern void raydium_ode_joint_elements_get_name (char *j, int *e1, int *e2);
extern void raydium_ode_motor_update_joints_data_internal (int j);
extern void raydium_ode_motor_speed (int j, dReal force);
extern void raydium_ode_motor_speed_name (char *name, dReal force);
extern void raydium_ode_motor_power_max (int j, dReal power);
extern void raydium_ode_motor_power_max_name (char *name, dReal power);
extern void raydium_ode_motor_angle (int j, dReal angle);
extern void raydium_ode_motor_angle_name (char *motor, dReal angle);
extern void raydium_ode_motor_gears_set (int m, dReal * gears, int n_gears);
extern void raydium_ode_motor_gears_set_name (char *m, dReal * gears, int n_gears);
extern void raydium_ode_motor_gear_change (int m, int gear);
extern void raydium_ode_motor_gear_change_name (char *m, int gear);
extern dReal *raydium_ode_element_pos_get (int j);
extern dReal *raydium_ode_element_pos_get_name (char *name);
extern char raydium_ode_element_rotq_get (int j, dQuaternion res);
extern char raydium_ode_element_rotq_get_name (char *name, dQuaternion res);
extern char raydium_ode_element_rot_get (int e, dReal * rx, dReal * ry, dReal * rz);
extern char raydium_ode_element_rot_get_name (char *e, dReal * rx, dReal * ry, dReal * rz);
extern void raydium_ode_element_sound_update (int e, int source);
extern void raydium_ode_element_sound_update_name (char *e, int source);
extern void raydium_ode_element_RelPointPos (int e, dReal px, dReal py, dReal pz, dReal * res);
extern void raydium_ode_element_RelPointPos_name (char *e, dReal px, dReal py, dReal pz, dReal * res);
extern int raydium_ode_motor_create (char *name, int obj, char type);
extern void raydium_ode_motor_attach (int motor, int joint, int joint_axe);
extern void raydium_ode_motor_attach_name (char *motor, char *joint, int joint_axe);
extern dReal raydium_ode_motor_speed_get (int m, int gears);
extern dReal raydium_ode_motor_speed_get_name (char *name, int gears);
extern void raydium_ode_motor_rocket_set (int m, int element, dReal x, dReal y, dReal z);
extern void raydium_ode_motor_rocket_set_name (char *motor, char *element, dReal x, dReal y, dReal z);
extern void raydium_ode_motor_rocket_orientation (int m, dReal rx, dReal ry, dReal rz);
extern void raydium_ode_motor_rocket_orientation_name (char *name, dReal rx, dReal ry, dReal rz);
extern void raydium_ode_motor_rocket_playermovement (int m, char isplayermovement);
extern void raydium_ode_motor_rocket_playermovement_name (char *m, char isplayermovement);
extern char raydium_ode_motor_delete (int e);
extern char raydium_ode_motor_delete_name (char *name);
extern char raydium_ode_joint_delete (int joint);
extern char raydium_ode_joint_delete_name (char *name);
extern char raydium_ode_element_delete (int e, char deletejoints);
extern char raydium_ode_element_delete_name (char *name, char deletejoints);
extern char raydium_ode_object_delete (int obj);
extern char raydium_ode_object_delete_name (char *name);
extern char raydium_ode_explosion_delete (int e);
extern char raydium_ode_element_moveto (int element, int object, char deletejoints);
extern char raydium_ode_element_moveto_name (char *element, char *object, char deletejoints);
extern void raydium_ode_joint_break (int j);
extern char raydium_ode_launcher (int element, int from_element, dReal * rot, dReal force);
extern char raydium_ode_launcher_name (char *element, char *from_element, dReal * rot, dReal force);
extern char raydium_ode_launcher_name_3f (char *element, char *from_element, dReal rx, dReal ry, dReal rz, dReal force);
extern char raydium_ode_launcher_simple (int element, int from_element, dReal * lrot, dReal force);
extern char raydium_ode_launcher_simple_name (char *element, char *from_element, dReal * rot, dReal force);
extern char raydium_ode_launcher_simple_name_3f (char *element, char *from_element, dReal rx, dReal ry, dReal rz, dReal force);
extern void raydium_ode_explosion_blow (dReal radius, dReal max_force, dReal * pos);
extern void raydium_ode_explosion_blow_3f (dReal radius, dReal max_force, dReal px, dReal py, dReal pz);
extern int raydium_ode_explosion_create (char *name, dReal final_radius, dReal propag, dReal * pos);
extern void raydium_ode_element_camera_inboard (int e, dReal px, dReal py, dReal pz, dReal lookx, dReal looky, dReal lookz);
extern void raydium_ode_element_camera_inboard_name (char *name, dReal px, dReal py, dReal pz, dReal lookx, dReal looky, dReal lookz);
extern void raydium_ode_draw_all (char names);
extern void raydium_ode_near_callback (void *data, dGeomID o1, dGeomID o2);
extern void raydium_ode_callback (void);
extern void raydium_ode_time_change (GLfloat perc);
extern void raydium_ode_element_particle (int elem, char *filename);
extern void raydium_ode_element_particle_name (char *elem, char *filename);
extern void raydium_ode_element_particle_offset (int elem, char *filename, dReal * offset);
extern void raydium_ode_element_particle_offset_name (char *elem, char *filename, dReal * offset);
extern void raydium_ode_element_particle_offset_name_3f (char *elem, char *filename, dReal ox, dReal oy, dReal oz);
extern void raydium_ode_element_particle_point (int elem, char *filename);
extern void raydium_ode_element_particle_point_name (char *elem, char *filename);
extern void raydium_camera_smooth_path_to_element (char *path, int element, GLfloat path_step, GLfloat smooth_step);
extern void raydium_camera_smooth_path_to_element_name (char *path, char *element, GLfloat path_step, GLfloat smooth_step);
extern void raydium_camera_smooth_element_to_path_offset (int element, GLfloat offset_x, GLfloat offset_y, GLfloat offset_z, char *path, GLfloat path_step, GLfloat smooth_step);
extern void raydium_camera_smooth_element_to_path_offset_name (char *element, GLfloat offset_x, GLfloat offset_y, GLfloat offset_z, char *path, GLfloat path_step, GLfloat smooth_step);
extern void raydium_camera_smooth_element_to_path_name (char *element, char *path, GLfloat path_step, GLfloat smooth_step);
extern int raydium_ode_capture_3d(char *filename);
extern int raydium_ode_orphans_check(void);
#include "ode_net.h"
#endif
