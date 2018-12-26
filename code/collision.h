
void ResolveRigidBodyCollision(Entity *e);

void resolve_plane_collisions(RigidBody &t, Collider_Plane &plane);

Plane ComputePlane(vec3 a, vec3 b, vec3 c);

vec3 ClosestPtPointPlane(vec3 q, Plane p);

struct RigidBody_Sphere;

void resolve_collision(RigidBody &t);

struct Ray_Info;

bool cast_ray(RigidBody_Sphere &target, Transform *transform, Ray_Info &info, vec3 origin, vec3 direction, float length);

bool resolve_rect_collisions(Collider_Rect &a, Collider_Rect &b);

bool resolve_sphere_collisions(RigidBody_Sphere &a, RigidBody_Sphere &b);

bool col(vec3 &pA, vec3 &pB, jvertex &Target, bool &lshift);

vec3 col_area(vec3 &pA, vec3 &pB);

bool col_edge(vec3 &pA, vec3 &pB, jvertex &Target);

bool col_beam(vec3 &pA, vec3 &pB, beam_pointer *beam, bool &lshift);

bool col_beam_ishowered(vec3 &pA, vec3 &pB, beam_pointer *beam);

bool col2(vec3 &pA, vec3 &pB, vec3 &Target);





