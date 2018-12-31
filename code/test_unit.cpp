
struct unit {
	Collider_Rect collider;
	RigidBody RB;
	Transform transform;
};

vector <unit> unit_vector;

ui32 SIZE = 2;
for(ui32 i = 0; i < SIZE; i++)
{
	for(ui32 j = 0; j < SIZE; j++)
	{
		unit t;
		vec4 color = rand_color(0.3f, 1.0f, 0.7f);
		t.ColliderRect.color = color;
		t.RB.mass = 40.0f;
		t.RB.InvMass = 1.0f / RectA.RB.mass;
		vec3 dim = rand_ivec3(1, 8);
		dim.z = 0;
		vec3 p = vec3(i+dim.x*2, j*dim.y, 0);
		p.z = 0.0f;
		t.transform.set_position(p);
		t.ColliderRect.center = p;
		t.ColliderRect.max = dim;
		
		unit_vector.push_back(t);
	}
}

void test_unit_loop()
{
	
	unit *a;
	unit *b;
	for(ui32 i = 0; i < unit_vector.size(); i++)
	{
		a = &unit_vector[i];
		for(ui32 j = 0; j < unit_vector.size(); j++)
		{
			b = &unit_vector[j];
			if(a == b) continue;
			ResolveRectCollision(a, b);
			CorrectPenetration(a, b);
		}
		ResolveRigidBodyCollision(a);
	}
	
}
