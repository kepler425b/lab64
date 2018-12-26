
struct Collider_Rect {
	vec3 min, max;
	vec3 center;
	vec4 color;
};


struct Collider_Plane {
	Transform transform;
	vec3 r[2];
	vec3 n;
};


struct Plane {
	vec3 n; // Plane normal. Points x on the plane satisfy Dot(n,x) = d
	float d; // d = dot(n,p) for a given point p on the plane
};
// Given three noncollinear points (ordered ccw), compute plane equation

void calc_plane_normal(Collider_Plane *collider)
{
	collider->n = normalize(cross(collider->transform.right(), collider->transform.up()));
}

void rotate_plane(Collider_Plane *collider, float angle, vec3 axis)
{
	vec4 r0 = vec4(collider->r[0], 1.0f) * glm::rotate(mat4(1.0f), angle, axis);
	collider->r[0] = vec3(r0.x, r0.y, r0.z); 
	
	vec4 r1 = vec4(collider->r[1], 1.0f) * glm::rotate(mat4(1.0f), angle, axis);
	collider->r[1] = vec3(r1.x, r1.y, r1.z); 
}

vec3 ClosestPtPointPlane(vec3 q, Plane p)
{
	float t = (dot(p.n, q) - p.d) / dot(p.n, p.n);
	vec3 result = q - t * p.n;
	result.z = 0.0f;
	push_line(q, result, TRAN*0.5f);
	push_point(result, TRAN*0.5f, 16);
	return result;
}

struct RigidBody
{
	bool selected;
    vec3 velocity;
    vec3 acceleration;
    vec3 force;
	vec3 added_force;
    vec2 dim;
	float e = 0.8f;
    float mass = 1.0f;
	float InvMass;
};

struct RigidBody_Sphere
{
	float r;
	vec3 p;
	float mass = 1.0f;
	float InvMass;
	vec3 w;
	vec3 a;
	
	float angularAcceleration;
	vec3  velocity;
	vec3  acceleration;
	
	float angularVelocity;
	
	float torque;
	float orientation;
	
	vec3 force, added_force;
	float area()
	{
		float result = r - p.x * p.x + r - p.y * p.y + r - p.z * p.z;
		return result;
	}
	void apply_force(vec3 f)
	{
		added_force = f;
	}
	void apply_torque(float t)
	{
		torque = t;
	}
	void update_physics(Transform *e, Time &time_state, Plane *plane)
    {
		float fcoef = 0.8f;
		float dt = time_state.dt;
		vec3 g = vec3(0.0f, VAR_G, 0.0f);
		float I = 2.5f * mass * r * r;
		force = mass * g;
		
		acceleration = 0.5f * (force + added_force)/mass * dt;
		velocity += acceleration * dt;
		angularVelocity = torque * (mass * I) * dt;
		
		orientation += angularVelocity * dt;
		
		vec3 result = velocity;
		result.x += angularVelocity;
		e->translate(result);
		
		imgpushf("T", torque);
		imgpushf("angular v", angularVelocity);
		imgpushf("orientation", orientation);
	}
	void update_collisions(Transform *e, Plane *plane)
	{
		vec3 pos = e->position(); + vec3(0, r, 0);
		vec3 point = ClosestPtPointPlane(pos, *plane);
		
		imgpushv3f("ClosestPtPointPlane", point);
		imgpushv3f("A.pos", pos);
		if(point.y > pos.y)
		{
			vec3 dir = normalize(velocity);
			vec3 nr = plane->n;
			float l = dot(dir, nr);
			vec3 r = -2.0f*l*nr + dir;
			vec3 n = normalize(r);
			
			e->translate(vec3(0, point.y - pos.y, 0));
			vec3 v = n * (0.4f * length(velocity)); 
			velocity = v;
			
			acceleration = vec3_zero;
			
			velocity = vec3_zero;
		}
	}
};

