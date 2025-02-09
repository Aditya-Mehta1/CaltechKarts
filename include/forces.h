#ifndef __FORCES_H__
#define __FORCES_H__

#include "collision.h"
#include "scene.h"

/**
 * A grouping of a force creator and its auxillary info.
 */
typedef struct force_info force_info_t;

/**
 * Initializes a force info type with params given
 * Takes ownership of the force creator and auxillary info
 * @param info: the auxillary info for the force
 * @param force: the force creator
 * @param bodies: the bodies the force acts on
 * @return the force info type
 */
force_info_t *force_info_init(void *info, force_creator_t force,
                              list_t *bodies);

/**
 * Frees a force info type
 * Frees the force creator and auxillary info
 * @param force_info: the force info type to free returned by force_info_init
 */
void force_info_free(force_info_t *force_info);

/**
 * Gets the force creator from a force info type
 * @param f_inf: the force info type to get the force creator from
 * @return the force creator
 */
force_creator_t f_info_get_f_creator(force_info_t *f_inf);

/**
 * Gets the auxillary info from a force info type
 * @param f_inf: the force info type to get the auxillary info from
 * @return the auxillary info
 */
void *f_info_get_aux(force_info_t *f_inf);

/**
 * Gets the bodies the force acts on
 * @param f_inf: the force info type to get the auxillary info from
 * @return the bodies
 */
list_t *f_info_get_bodies(force_info_t *f_inf);

/**
 * A function called when a collision occurs.
 * @param body1 the first body passed to create_collision()
 * @param body2 the second body passed to create_collision()
 * @param axis a unit vector pointing from body1 towards body2
 *   that defines the direction the two bodies are colliding in
 * @param aux the auxiliary value passed to create_collision()
 * @param force_const the force constant passed to create_collision()
 */
typedef void (*collision_handler_t)(body_t *body1, body_t *body2, vector_t axis,
                                    void *aux, double force_const);

/**
 * Adds a force creator to a scene that applies gravity between two bodies.
 * The force creator will be called each tick
 * to compute the Newtonian gravitational force between the bodies.
 * See
 * https://en.wikipedia.org/wiki/Newton%27s_law_of_universal_gravitation#Vector_form.
 * The force should not be applied when the bodies are very close,
 * because its magnitude blows up as the distance between the bodies goes to 0.
 *
 * @param scene the scene containing the bodies
 * @param G the gravitational proportionality constant
 * @param body1 the first body
 * @param body2 the second body
 */
void create_newtonian_gravity(scene_t *scene, double G, body_t *body1,
                              body_t *body2);

/**
 * Adds a force creator to a scene that acts like a spring between two bodies.
 * The force creator will be called each tick
 * to compute the Hooke's-Law spring force between the bodies.
 * See https://en.wikipedia.org/wiki/Hooke%27s_law.
 *
 * @param scene the scene containing the bodies
 * @param k the Hooke's constant for the spring
 * @param body1 the first body
 * @param body2 the second body
 */
void create_spring(scene_t *scene, double k, body_t *body1, body_t *body2);

/**
 * Adds a force creator to a scene that applies a drag force on a body.
 * The force creator will be called each tick
 * to compute the drag force on the body proportional to its velocity.
 * The force points opposite the body's velocity.
 *
 * @param scene the scene containing the bodies
 * @param gamma the proportionality constant between force and velocity
 *   (higher gamma means more drag)
 * @param body the body to slow down
 */
void create_drag(scene_t *scene, double gamma, body_t *body);

/**
 * Adds a force creator to a scene that calls a given collision handler
 * function each time two bodies collide.
 * This generalizes create_destructive_collision() from last week,
 * allowing different things to happen on a collision.
 * The handler is passed the bodies, the collision axis, and an auxiliary value.
 * It should only be called once while the bodies are still colliding.
 *
 * @param scene the scene containing the bodies
 * @param body1 the first body
 * @param body2 the second body
 * @param handler a function to call whenever the bodies collide
 * @param aux an auxiliary value to pass to the handler
 * @param force_const a constant to pass to the handler
 */
void create_collision(scene_t *scene, body_t *body1, body_t *body2,
                      collision_handler_t handler, void *aux,
                      double force_const);

/**
 * Adds a force creator to a scene that destroys two bodies when they collide.
 * The bodies should be destroyed by calling body_remove().
 * This should be represented as an on-collision callback
 * registered with create_collision().
 *
 * @param scene the scene containing the bodies
 * @param body1 the first body
 * @param body2 the second body
 */
void create_destructive_collision(scene_t *scene, body_t *body1, body_t *body2);

/**
 * The collision handler for for physics collisions. Applies impulses to
 * bodies according to the elasticity in `aux`.
 */
void physics_collision_handler(body_t *body1, body_t *body2, vector_t axis,
                               void *aux, double force_const);

/**
 * Adds a force creator to a scene that applies impulses
 * to resolve collisions between two bodies in the scene.
 * This should be represented as an on-collision callback
 * registered with create_collision().
 *
 * You may remember from project01 that you should avoid applying impulses
 * multiple times while the bodies are still colliding.
 * You should also have a special case that allows either body1 or body2
 * to have mass INFINITY, as this is useful for simulating walls.
 *
 * @param scene the scene containing the bodies
 * @param body1 the first body
 * @param body2 the second body
 * @param elasticity the "coefficient of restitution" of the collision;
 * 0 is a perfectly inelastic collision and 1 is a perfectly elastic collision
 */
void create_physics_collision(scene_t *scene, body_t *body1, body_t *body2,
                              double elasticity);

#endif // #ifndef __FORCES_H__
