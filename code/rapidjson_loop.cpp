rg_line lines_to_render;
rg_line selected_lines_to_render;
rg_point points_to_render;
rg_point selected_points_to_render;
bool button_pushed = false;

void rapidjson_loop(Document &data, Input &input_state)
{
	map<string, unsigned int>::iterator e;
	map <string, unsigned int>::iterator ait;
	map <string, unsigned int>::iterator bit;
	int ai = 0;
	int bi = 0;
	Value::ValueIterator nodes = 0;
	for (Value::MemberIterator itr = data.MemberBegin();
		 itr != data.MemberEnd(); ++itr)
	{
		if (itr->value.IsObject())
		{
			unsigned int node_index = 0;
			o.name = itr->name.GetString();
			Value::MemberIterator c = itr->value.FindMember("nodes");
			
			if (c != data.MemberEnd() && c->name == "nodes")
			{
				for (nodes = c->value.Begin(); nodes != c->value.End(); nodes++)
				{
					if (nodes->IsArray())
					{
						for (Value::ValueIterator j = nodes->Begin(); j != nodes->End(); j++)
							if (j[0].IsString() && j[1].IsFloat() && j[2].IsFloat() && j[3].IsFloat())
						{
							jvertex v;
							string id = j[0].GetString();
							/*int a;
 const char *idc = id.c_str();
 unsigned char j, k;
 j = 0;
 k = 0;
 for (unsigned char i = 0; i < strlen(idc); i++)
 {
 if (isdigit(idc[i])) j++;
 
 else k++;
 }
 if (j > 0) a = atoi(idc + k);*/
							
							temp_map.insert(pair<string, unsigned int>(id, node_index));
							
							v.id = id;
							v.index = node_index;
							v.pos.x = j[1].GetFloat();
							v.pos.y = j[2].GetFloat();
							v.pos.z = j[3].GetFloat();
							float dis = distance(input_state.mouse_w, v.pos);
							//cout << dis << endl;
							if ((dis < 0.1f) && input_state.m_left)
							{
								push_point(v.pos, GREEN + RED, 16);
								button_pushed = true;
							}
							
							
							if (1)
							{
								kpl_draw_text(text_info, v.id + ":" + to_string(v.active),  v.pos, 0.25f, glm::vec4(0.0, 0.2f, 0.65f, 1.0f), false);
							}
							
							
							o.nodes.push_back(v);
							//cout << "node: " << id << endl;
							push_point(v.pos, GREEN, 8);
							node_index++;
						}
					}
				}
				//node_beam_maps.push_back(temp_map);
			}
			bool new_group = false;
			bool beam_found = false;
			beam_group b_group = {};
			Value::MemberIterator m = itr->value.FindMember("beams");
			if (m != data.MemberEnd() && m->name == "beams")
			{
				for (int i = 0; i < m->value.Size(); i++)
				{
					if (m->value[i].IsObject())
					{
						if (beam_found)
						{
							new_group = true;
							beam_found = false;
							b_group.id = o.beams.size();
							o.beams.push_back(b_group);
							b_group = {};
						}
						for (Value::MemberIterator itr = m->value[i].MemberBegin();
							 itr != m->value[i].MemberEnd(); ++itr)
						{
							if (itr->name == "beamSpring")
							{
								if (itr->value.IsInt())
								{
									b_group.beam_spring = itr->value.GetInt();
								}
								else if (itr->value.IsFloat())
								{
									b_group.beam_spring = itr->value.GetFloat();
								}
								else
								{
									b_group.beam_spring = FLT_MAX;
								}
							}
							if (itr->name == "beamDamp")
							{
								if (itr->value.IsInt())
								{
									b_group.beam_damp = itr->value.GetInt();
								}
								else if (itr->value.IsFloat())
								{
									b_group.beam_damp = itr->value.GetFloat();
								}
								else
								{
									b_group.beam_damp = FLT_MAX;
								}
							}
							if (itr->name == "beamStrength")
							{
								if (itr->value.IsInt())
								{
									b_group.beam_strength = itr->value.GetInt();
								}
								else if (itr->value.IsFloat())
								{
									b_group.beam_strength = itr->value.GetFloat();
								}
								else
								{
									b_group.beam_strength = FLT_MAX;
								}
							}
							if (itr->name == "beamDeform")
							{
								{
									if (itr->value.IsInt())
									{
										b_group.beam_deform = itr->value.GetInt();
									}
									else if (itr->value.IsFloat())
									{
										b_group.beam_deform = itr->value.GetFloat();
									}
									else
									{
										b_group.beam_deform = FLT_MAX;
									}
								}
							}
							if (itr->name == "beamPrecompression")
							{
								{
									{
										if (itr->value.IsInt())
										{
											b_group.bounded.precompression = itr->value.GetInt();
										}
										else if (itr->value.IsFloat())
										{
											b_group.bounded.precompression = itr->value.GetFloat();;
										}
										else
										{
											b_group.bounded.precompression = FLT_MAX;
										}
									}
								}
							}
							if (itr->name == "beamType") b_group.bounded.type = itr->value.GetString();
							if (itr->name == "beamLongBound") b_group.bounded.long_bound = itr->value.GetFloat();
							if (itr->name == "beamShortBound") b_group.bounded.short_bound = itr->value.GetFloat();
							
						}
					}
					if (m->value[i].IsArray())
					{
						if (m->value[i][0].IsString() && m->value[i][1].IsString())
						{
							string sa, sb;
							unsigned int alen, blen;
							sa = m->value[i][0].GetString();
							sb = m->value[i][1].GetString();
							alen = m->value[i][0].GetStringLength();
							blen = m->value[i][1].GetStringLength();
							
							if (sa.at(alen - 1) && sb.at(blen - 1) != ':')
							{
								beam_found = true;
								beam_pair b;
								b.a = sa;
								b.b = sb;
								
								e = temp_map.end();
								
								ait = temp_map.find(sa);
								bit = temp_map.find(sb);
								if (ait != e && bit != e)
								{
									bi = bit->second;
									ai = ait->second;
									//o.nodes[ai].weight = rand();
									//o.nodes[bi].weight = rand();
									
									push_line(o.nodes[ai].pos, o.nodes[bi].pos, RED);
								}
								//b_group.beams.push_back(b);
							}
							else beam_found = false;
						}
					}
					/*else if (i >= m->value.Size())
  {
  o.beams.push_back(b_group);
  }*/
				}
			}
			b_group.id = o.beams.size();
			o.beams.push_back(b_group);
			
		}
		//printf("Type of member %s and has %d members.\n",
		
		//	itr->name.GetString(), itr->value.MemberCount());
		objects.push_back(o);
		o = {};
		temp_map = {};
		
		if (input_state.k_enter)
		{
			Value::MemberIterator c = itr->value.FindMember("nodes");
			Value a(kArrayType);
			a.PushBack("new node", data.GetAllocator());
			a.PushBack(input_state.mouse_w.x, data.GetAllocator());
			a.PushBack(input_state.mouse_w.y, data.GetAllocator());
			a.PushBack(input_state.mouse_w.z, data.GetAllocator());
			if (c->value.IsArray())
			{
				c->value.PushBack(a, data.GetAllocator());
				jvertex v;
				v.id = "new node";
				v.index = o.nodes.size() + 1;
				v.pos = vec3(input_state.mouse_w.x, input_state.mouse_w.y, input_state.mouse_w.z);
				o.nodes.push_back(v);
			}
		}
	}
	button_pushed = false;
	
	lines_to_render.p.insert(lines_to_render.p.end(), selected_lines_to_render.p.begin(), selected_lines_to_render.p.end());
	lines_to_render.color.insert(lines_to_render.color.end(), selected_lines_to_render.color.begin(), selected_lines_to_render.color.end());
	
	points_to_render.p.insert(points_to_render.p.end(), selected_points_to_render.p.begin(), selected_points_to_render.p.end());
	points_to_render.color.insert(points_to_render.color.end(), selected_points_to_render.color.begin(), selected_points_to_render.color.end());
	render_line_group(lines_to_render, &default_shader, &camera);
	render_point_group(points_to_render, &default_shader, &camera);
	selected_lines_to_render = {};
	selected_points_to_render = {};
	
}