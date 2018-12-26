vector<Model> model_mesh_memory;
vector<Mover> mover_memory;
vector<ui32> RectIDS;

enum Entity_Component_Type {
	COMPONENT_MESH,
	COMPONENT_PRIMITIVE,
	COMPONENT_MOVER,
	COMPONENT_COLLIDER_RECT
};

struct component_mesh_renderer {
	ui32 data_id;
};

struct component_mover {
	ui32 data_id;
};

struct component_logic {
	ui32 data_id;
};

struct Entity;

#define MACRODefineEntityLogic(name, arg) void *name(arg)

void ResetPositions(Entity *e);
void sphere_collider(Entity *e);

struct EntityLogic {
	string name;
	bool enabled;
	void (*FuncPtr)(Entity *);
};

struct Entity {
	ui32 id;
	ui32 tag;
	Transform transform;
	vector<component_mesh_renderer> mesh_components;
	vector<component_mover> mover_components;
	RigidBody RB;
	Collider_Rect ColliderRect;
	vector<EntityLogic> LogicContainer;
	RigidBody_Sphere sphere;
};

void push_logic(Entity *e, void (*func)(Entity *))
{
	EntityLogic t;
	t.enabled = 1;
	t.FuncPtr = func;
	e->LogicContainer.push_back(t);
}

void ResetPositions(Entity *e)
{
	if(input_state.k_enter)
	{
		vec3 p = rand_vec3(2.0f, 16.0f);
		p.y = 6.0f;
		p.z = 0;
		
		e->transform.set_position(p);
		//e->collider.add_force(normalize(input_state.mouse_w - e->collider.t.position())*100.0f);
	}
}

void update_sphere(Entity *e)
{
	e->sphere.update_physics(&e->transform, time_state, &plane1); 
	e->sphere.update_collisions(&e->transform, &plane1);
	//e->sphere.apply_torque(input_state.mouse_w.x *0.00001f);
	kplDrawSphere(e->transform.position(), 0, vec3_forward, 1.0f, 16, 16, vec4(1, 0, 0, 0.25f), 1, 1);
	if(input_state.k_enter)
	{
		e->transform.set_position(vec3_zero);
	}
}

void player_controller(Entity *e)
{
	Input *i = &input_state;
	float scale = 5000.0f;
	if(i->a_up) e->RB.added_force += vec3_up * scale; 
	if(i->a_down) e->RB.added_force += -vec3_up * scale; 
	if(i->a_right) e->RB.added_force += vec3_right * scale; 
	if(i->a_left) e->RB.added_force += -vec3_right * scale; 
}

void mouse_follower(Entity *e)
{
	float dist = distance(e->transform.position(), camera.pos);
	if(dist > 0.5f)
	{
		vec3 d;
		d = normalize(camera.pos - e->transform.position());
		e->transform.translate(d / (dist));
	}
}

void assemble_entity(Entity &e)
{
	//put needed entities
}

vector<Entity> entities;
void attach_component(Entity &e, Entity_Component_Type type, Model &mesh_data)
{
	switch(type)
	{
		case COMPONENT_MESH: 
		{
			component_mesh_renderer t;
			t.data_id = e.mesh_components.size();
			e.mesh_components.push_back(t);
			mesh_data.transform.r = mat4(1.0f);
			model_mesh_memory.push_back(mesh_data);
			
			//assert(t.data_id == model_mesh_memory.size()-1);
		} break;
		
		case COMPONENT_MOVER:
		{
			component_mover t;
			t.data_id = e.mover_components.size();
			e.mover_components.push_back(t);
			Mover m = {};
			mover_memory.push_back(m);
			
			//assert(t.data_id == mover_memory.size()-1);
		} break;
		
		case COMPONENT_PRIMITIVE:
		{
			
		} break;
		
		default: cout << "no type was recognized" << endl; e.id = -1;
	}
}

void push_entity(Entity &e)
{
	e.id = entities.size();
	entities.push_back(e);
	if(e.tag == COMPONENT_COLLIDER_RECT)
	{
		RectIDS.push_back(e.id);
	}
}

void delete_entity(ui32 id)
{
	if(id > entities.size()) 
	{
		cout << "trying to delete non existant entity at: " << id << endl;
		return;
	}
	else if(entities.size() != 0)
	{
		entities.erase(entities.begin() + index);
	}
}

void delete_component(ui32 entity_index, Entity_Component_Type type)
{
	switch(type)
	{
		case COMPONENT_MESH: 
		{
			if(entity_index < entities.size())
			{
				if(entities[entity_index].mesh_components.size() > 0)
				{
					int index = entities[entity_index].mesh_components.size();
					entities[entity_index].mesh_components.erase(entities[entity_index].mesh_components.begin() + index);
					model_mesh_memory.erase(model_mesh_memory.begin() + index);
				}
			}
		} break;
		
		case COMPONENT_MOVER:
		{
			if(entity_index < entities.size())
			{
				if (entities[entity_index].mover_components.size() > 0)
				{
					ui32 index = entities[entity_index].mover_components.size();
					entities[entity_index].mover_components.erase(entities[entity_index].mover_components.begin() + index);
					mover_memory.erase(mover_memory.begin() + index);
				}
			}
		} break;
		
		case COMPONENT_PRIMITIVE:
		{
			
		} break;
		
		default: cout << "no type was recognized." << endl;
	}
}

Entity* get_entity(int id)
{
	if(id <= entities.size() && id >= 0)
	{
		return &entities[id];
	}
	else 
	{
		Entity e;
		return &e;
	}
}

Model* get_mesh_component(int entity_id)
{
	if(entity_id <= entities.size())
	{
		int index = entities[entity_id].mesh_components[entity_id].data_id;
		return &model_mesh_memory[index];
	}
	else 
	{
		Model m;
		return &m; 
	}
}

void RB_AddForce(RigidBody *rb, vec3 f)
{
	rb->added_force = f / rb->InvMass;
}
void RB_UpdatePhysics(Entity *e, Time &time_state)
{
	float dt = time_state.dt;
	vec3 g = vec3(0.0f, VAR_G, 0.0f);
	vec3 force, added_force, acceleration, velocity;
	float mass;
	
	float InvMass = e->RB.InvMass;
	force = g / InvMass;
	added_force = e->RB.added_force;
	acceleration = e->RB.acceleration;
	velocity = e->RB.velocity;
	
	
	//vec3 an = added_force/mass * dt;
	//vec3 fn = mass * an;
	
	e->RB.acceleration = 0.9f * (force+added_force) * InvMass * dt;
	//acceleration += an;
	
	
	e->RB.velocity += acceleration * dt;
	vec3 result = velocity;
	
	e->transform.translate(result);
	
	added_force = {};
}

void RB_Update()
{
	
}




