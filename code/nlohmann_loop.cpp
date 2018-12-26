rg_line lines_to_render;
rg_line selected_lines_to_render;
rg_point points_to_render;
rg_point selected_points_to_render;

ui32 beam_group_index = 0;
ui32 node_start_offset = 0;

void nlohmann_loop(nlohmann::json &data, Input &input_state)
{
	using namespace nlohmann;
	map<string, unsigned int>::iterator e;
	map <string, unsigned int>::iterator ait;
	map <string, unsigned int>::iterator bit;
	int ai = 0;
	int bi = 0;
	for (json::iterator itr = data.begin();
		itr != data.end(); ++itr)
	{
		if (itr->is_object())
		{
			unsigned int node_index = 0;
			o.name = itr.key();

			json::iterator c = data.find("nodes");

			if (c.key() == "nodes")
			{
				for (json::iterator ak = c->begin(); c != c->end(); ak++)
				{
					if (ak[0].is_number_float() && ak[1].is_number_float() && ak[2].is_number_float() && ak[3].is_number_float())
					{
						jvertex v;
						string id = c[0].get<string>();
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
						v.pos.x = c[1].get<float>();
						v.pos.y = c[2].get<float>();
						v.pos.z = c[3].get<float>();
						o.nodes.push_back(v);

						if (input_state.k_enter)
						{
							//Value a(kArrayType);
							//a.PushBack(0.0f, document.GetAllocator());
							//a.PushBack(0.0f, document.GetAllocator());
							//a.PushBack(0.0f, document.GetAllocator());
							////j->PushBack(a, document.GetAllocator());

							push_point(selected_points_to_render, v.pos, GREEN, 8);
						}

						push_point(selected_points_to_render, v.pos, GREEN, 8);
						node_index++;
					}
				}
				//node_beam_maps.push_back(temp_map);
			}
			bool new_group = false;
			bool beam_found = false;
			beam_group b_group = {};
			json::iterator m = itr->find("beams");
			if (m != data.end() && m.key() == "beams")
			{
				for (int i = 0; i < m->size(); i++)
				{
					if (m[i].is_object())
					{
						if (beam_found)
						{
							new_group = true;
							beam_found = false;
							b_group.id = o.beams.size();
							o.beams.push_back(b_group);
							b_group = {};
						}
						for (json::iterator itr = m[i].begin();
							itr != m[i].end(); ++itr)
						{
							if (itr.key() == "beamSpring")
							{
								if (itr->is_number_integer())
								{
									b_group.beam_spring = itr->get<int>();
								}
								else if (itr->is_number_float())
								{
									b_group.beam_spring = itr->get<float>();
								}
								else
								{
									b_group.beam_spring = FLT_MAX;
								}
							}
							if (itr.key() == "beamDamp")
							{
								if (itr->is_number_integer())
								{
									b_group.beam_damp = itr->get<int>();
								}
								else if (itr->is_number_float())
								{
									b_group.beam_damp = itr->get<float>();
								}
								else
								{
									b_group.beam_damp = FLT_MAX;
								}
							}
							if (itr.key() == "beamStrength")
							{
								if (itr->is_number_integer())
								{
									b_group.beam_strength = itr->get<int>();
								}
								else if (itr->is_number_float())
								{
									b_group.beam_strength = itr->get<float>();
								}
								else
								{
									b_group.beam_strength = FLT_MAX;
								}
							}
							if (itr.key() == "beamDeform")
							{
								{
									if (itr->is_number_integer())
									{
										b_group.beam_deform = itr->get<int>();
									}
									else if (itr->is_number_float())
									{
										b_group.beam_deform = itr->get<float>();
									}
									else
									{
										b_group.beam_deform = FLT_MAX;
									}
								}
							}
							if (itr.key() == "beamPrecompression")
							{
								{
									{
										if (itr->is_number_integer())
										{
											b_group.bounded.precompression = itr->get<int>();
										}
										else if (itr->is_number_float())
										{
											b_group.bounded.precompression = itr->get<float>();;
										}
										else
										{
											b_group.bounded.precompression = FLT_MAX;
										}
									}
								}
							}
							if (itr.key() == "beamType") b_group.bounded.type = itr->get<string>();
							if (itr.key() == "beamLongBound") b_group.bounded.long_bound = itr->get<float>();
							if (itr.key() == "beamShortBound") b_group.bounded.short_bound = itr->get<float>();

						}
					}
					if (m[i].is_array())
					{
						if (m[i][0].is_string() && m[i][1].is_string())
						{
							string sa, sb;
							unsigned int alen, blen;
							sa = m[i][0].get<string>();
							sb = m[i][1].get<string>();
							alen = m[i][0].size();
							blen = m[i][1].size();

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

									push_lines(lines_to_render, o.nodes[ai].pos, o.nodes[bi].pos, RED);
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
		printf("Type of member %s and has %d members.\n");

		//itr->name.get<string>(), itr->MemberCount());
		objects.push_back(o);
		o = {};
		temp_map = {};
	}
}