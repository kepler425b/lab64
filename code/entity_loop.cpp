Entity *selected_entity = 0;
bool skip = 0;
void select_entity(Entity *e)
{
	float d = distance(e->transform.position(), input_state.mouse_w);
	if (input_state.mouse_left.state & kON && !skip)
	{
		if(d < 2.0f)
		{
			if(!selected_entity)
			{
				skip = 1;
				selected_entity = e;
			}
		}
		else 
		{
			selected_entity = 0;
		}
		
	}
}

static float bf, af;
bool dragging = 0;
RigidBody *rg = 0;
vec4 color;
float xoffset, yoffset;
float hold_distance = 16.0f;
float grab_force = 500.0f;

void process_entities()
{
	for(ui32 eid = 0; eid < entities.size(); eid++)
	{
		Entity *e = &entities[eid];
		
		e->RB.added_force = {};
		
		if(e->id == -1)
		{
			cout << "skipped entity id: " << e->id << endl;
			continue;
		}
		if(e->mesh_components.size() > 0)
		{
			//e->transform.translate(vec3(sinf(time_state.seconds_passed)*eid, cosf(time_state.seconds_passed)*eid, 0.0f) * (eid * time_state.dt));
			ui32 index = e->mesh_components[e->mesh_components.size()-1].data_id;
			//render_model(model_mesh_memory[index], e->transform, &camera);
		}
		if(e->mover_components.size() == 1)
		{
			ui32 index = e->mover_components[e->mover_components.size()-1].data_id;
			update_movers(e->transform, 1.0f, input_state, time_state);
		}
		
		select_entity(e);
		
		if(selected_entity && input_state.k_delete)
		{
			delete_entity(selected_entity->id);
			selected_entity = 0;
		}
		push_text(&render_group_text, to_string(e->id), e->transform.position(), 0.5f, BLUE);
		
		
		
		/*
  float l0, l1;
  l0 = e->collider.t.position().y;
  l1 = 2.0f;
  
  float fdelta = (e->collider.mass * VAR_G) / 0.5f; 
  float f = -0.5f * e->collider.mass * (l0 - l1);
  cout << "force" << f << endl;
  
  e->collider.add_force(vec3(0.0f, f, 0.0f));
  */
		
		if(e->LogicContainer.size() > 0)
		{
			ui32 size = e->LogicContainer.size();
			for(ui32 i = 0; i < size; i++)
			{
				if(e->LogicContainer[i].enabled && e->LogicContainer[i].FuncPtr != 0)
				{
					e->LogicContainer[i].FuncPtr(e);
				}
			}
		}
		
		color =vec4(0.5f, abs(e->RB.velocity.x), abs(e->RB.velocity.z), 0.8f);
		
		RB_UpdatePhysics(e, time_state);
		Entity *a;
		Entity *b;
		for(ui32 i = 0; i < RectIDS.size(); i++)
		{
			a = get_entity(RectIDS[i]);
			for(ui32 j = 0; j < RectIDS.size(); j++)
			{
				b = get_entity(RectIDS[j]);
				if(a == b) continue;
				ResolveRectCollision(a, b);
				CorrectPenetration(a, b);
			}
			ResolveRigidBodyCollision(a);
		}
		
		if(e->tag == COMPONENT_COLLIDER_RECT)
		{
			DrawRect(e->transform.position(), e->ColliderRect.max.x,
					 e->ColliderRect.max.y, &e->transform, &default_shader, e->ColliderRect.color, &camera, 0);
			
			vec3 p = e->transform.position() - (vec3_up + vec3_right)*0.2f;
			imgpushv3f(to_string(e->id) + " ID Entity pos:", e->transform.position());
			imgpushv3f(to_string(e->id) + " ID ColliderRect pos:", e->ColliderRect.min);
		}
		
		if(e->id != 2 && e->tag != COMPONENT_COLLIDER_RECT){
			//RB_UpdatePhysics(e, time_state);
			//ResolveRigidBodyCollision(e);
			push_cube(&render_group_cubes, e->transform.position(), color, 1.0f);
		}
		else
		{
			Ray_Info info;
			cast_ray(e->sphere, &e->transform, info, camera.pos, input_state.mouse_w - camera.pos, 60.f);
			if(input_state.mouse_left.state & kON)
			{
				e->sphere.apply_force(info.point);
			}
		}
		
		if(display_info.show_entity_bases)
		{
			show_basis(e->transform);
		}
		e->ColliderRect.min = e->transform.position();
	}
	if(selected_entity)
	{
		imgpushf("m", selected_entity->RB.mass);
		imgpushv3f("a", selected_entity->RB.acceleration);
		imgpushf("f", VAR_G);
		imgpushv3f("v", selected_entity->RB.velocity);
		imgpushv3f("v/t", selected_entity->RB.velocity * time_state.dt);
		imgpushv4f("color", color);
		push_cube(&render_group_cubes, selected_entity->transform.position(), vec4(0.5f, 0.7f, 0.1f, 0.5f), 1.25f);
		push_text(&render_group_text, to_string(selected_entity->id), selected_entity->transform.position(), 0.5f, GREEN);
		kpl_draw_text(text_info, to_string(selected_entity->id), selected_entity->transform.position()+vec3_up*0.25f, 0.75f, GREEN, 1);
		
		if(input_state.m_right)
		{
			float xmax, zmax;
			xmax = 20.0f;
			zmax = 4.0f;
			vec3 result;
			vec3 mp;
			vec3 cf = camera.forward;
			vec3 cp = camera.pos;
			vec3 ep = selected_entity->transform.position();
			
			float d = distance(cp, ep);
			imgpushf("d", d);
			
			/*
   if(input_state.k_x)
   {
 result = cp + vec3_right * (hold_distance + input_state.mwheel);
   }
   else if(input_state.k_y)
   {
 result = cp + vec3_up * (hold_distance + input_state.mwheel);
   }
   else
   */
			
			result = cp + cf * (hold_distance + input_state.mwheel);
			result.z = 0;
			
			selected_entity->transform.set_position(result);
			vec3 offset = vec3(0, 2, 0);
		}
		if(input_state.m_left && input_state.lctrl)
		{
			vec3 cf = camera.forward;
			vec3 cp = camera.pos;
			vec3 ep = selected_entity->transform.position();
			vec3 d = cp + cf * 4.0f;
			vec3 result = d - ep;
			result.z = 0;
			if(input_state.mouse_wheel.state & kAxisY) grab_force -= 25.0f; 
			if(input_state.mouse_wheel.state & kAxisX) grab_force += 25.0f;
			
			push_line(ep, d, WHITE);
			selected_entity->RB.added_force += result * grab_force; 
			RB_UpdatePhysics(selected_entity, time_state);
		}
		/*
  vec3 mp = input_state.mouse_w;
  vec3 cp = camera.pos;
  float speed = 0.5f;
  
  if(input_state.a_up) yoffset += speed;
  if(input_state.a_down) yoffset -= speed;
  if(input_state.a_right) xoffset += speed;
  if(input_state.a_left) xoffset -= speed;
  
  p = vec3(xoffset, yoffset, mp.z); 
  
  selected_entity->RB.t.set_position(p);
  selected_entity->transform.set_position(p);
  */
	}
	
	vec3 mouse_noz = input_state.mouse_w;
	mouse_noz.z = 0.0f;
	push_text(&render_group_text, "num of entities:" + to_string(entities.size()),  mouse_noz + vec3_up * 0.25f, 0.5f, GREEN);
	
	push_text(&render_group_text, to_string(input_state.mouse_w.x) + ", " + to_string(input_state.mouse_w.y) + ", " + to_string(input_state.mouse_w.z), vec3(512, 512, 0), 0.5f, GREEN);
	
	
	LARGE_INTEGER tick_after_loop, tick_before_loop;
	QueryPerformanceCounter(&tick_before_loop);
	__int64 interval;
	
	render_text_group(render_group_text, text_info, &text_shader, &camera, 0);
	render_line_group(render_list_lines, &default_shader, &camera);
	render_cube_group(render_group_cubes, &default_shader, &camera);
	render_point_group(points, &default_shader, &camera);
	
	draw_rect(&default_shader, vec3(-50, -2, -50), 100, 100, vec3_right, -80, 1, vec4(0.1f,0.1f,0.1f,1.0f), &camera, 1); 
	QueryPerformanceCounter(&tick_after_loop);
	interval = tick_after_loop.QuadPart - tick_before_loop.QuadPart;
	
	bf = interval * second_per_tick;
	render_group_text.clear();
	skip = 0;
}
