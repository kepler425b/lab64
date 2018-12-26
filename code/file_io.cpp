void JBEAM_FixCommas_NextChar(string &sample, char &nextchar, int &nextchar_i)
{
	//Find the next character other than comma, space, or line change
	for (int i = nextchar_i; i<sample.length(); i++)
	{
		if (sample.at(i) == ' ');
		else if (sample.at(i) == '\n');
		else if (sample.at(i) == '	');
		else
		{
			nextchar = sample.at(i);
			nextchar_i = i;
			break;
		}
	}
}

void JBEAM_FixCommas_PrevChar(string &sample, char &prevchar, int &prevchar_i)
{
	//Find the previous character other than space, or line change
	for (int i = prevchar_i; i > 0; i--)
	{
		//qDebug() << "checking p " << sample[i];
		if (sample.at(i) == ' ');
		else if (sample.at(i) == '\n');
		else if (sample.at(i) == '	');
		else
		{
			prevchar = sample.at(i);
			prevchar_i = i;
			break;
		}
	}
}

string JBEAM_FixCommas(string JbeamText)
{
	string temp1;
	bool checking = 0;
    
	string JbeamTextSTR = JbeamText.data();
    
	vector <int> CommaMissingPos;
    
	bool Checking = 0;
	bool CommaFound = 0;
	bool InsideString = 0;
	bool token_found = true;
	unsigned char char_a = 'ö';
	unsigned char char_b = 'ö';
	unsigned char closing_char;
	unsigned char char_a_offset;
    
	bool space = 0;
	int index = 0;
	int dirty_index = 0;
	int gap_n = 0;
	int gap_start = 0;
	int gap_end = 0;
    
	smatch result;
	int missingPos = result.position();
	int result_length = 0;
    
	regex regex_comment("\/\/.*");
	for (std::sregex_iterator i = std::sregex_iterator(JbeamTextSTR.begin(), JbeamTextSTR.end(), regex_comment);
         i != std::sregex_iterator();
         ++i)
	{
		std::smatch m = *i;
		//std::cout << m.str() << " at position " << m.position() << '\n';
		JbeamTextSTR.replace(m.position(), m.length(), m.length(), ' ');
	}
    
	vector <ui16> espos;
	regex regex_empty_string("(?:(?=\\s*?)\\\")(?:(?=\\s*?\\\"))");
	for (std::sregex_iterator i = std::sregex_iterator(JbeamTextSTR.begin(), JbeamTextSTR.end(), regex_empty_string);
         i != std::sregex_iterator();
         ++i)
	{
		std::smatch m = *i;
		//std::cout << m.str() << " at position " << m.position() << '\n';
		missingPos = m.position();
		espos.push_back(missingPos);
	}
    
	ui16 offset = 0;
	for (ui16 i = 0; i < espos.size(); i++)
	{
		JbeamTextSTR.insert(espos[i] + offset + 1, "NULL");
		offset += 4;
	}
    
	vector <ui16> mcnumpos;
	regex regex_missing_comma_number("(?=\"[^\"]*\"\\s*\\:[^\"]*(\\d)\\s*\")");
	for (std::sregex_iterator i = std::sregex_iterator(JbeamTextSTR.begin(), JbeamTextSTR.end(), regex_missing_comma_number);
         i != std::sregex_iterator();
         ++i)
	{
		std::smatch m = *i;
		if (m.ready())
		{
			missingPos = m.position(1);
		}
        
		//std::cout << m.str() << " at position " << m.position() << '\n';
		mcnumpos.push_back(missingPos);
	}
    
	offset = 0;
	for (ui16 i = 0; i < mcnumpos.size(); i++)
	{
		JbeamTextSTR.insert(mcnumpos[i] + offset + 1, ",");
		offset += 1;
	}
    
	vector <ui16> mcbpos;
	regex regex_missing_comma_numbrackets("\\d(?=\\s*[\\{\\[])");
	for (std::sregex_iterator i = std::sregex_iterator(JbeamTextSTR.begin(), JbeamTextSTR.end(), regex_missing_comma_numbrackets);
         i != std::sregex_iterator();
         ++i)
	{
		std::smatch m = *i;
		missingPos = m.position();
		mcbpos.push_back(missingPos);
	}
    
	offset = 0;
	for (ui16 i = 0; i < mcbpos.size(); i++)
	{
		JbeamTextSTR.insert(mcbpos[i] + offset + 1, ",");
		offset += 1;
	}
    
	vector <ui16> mcpos;
	//@kepler: god forgive me..
	regex regex_missing_comma("\\](?=\\s*?\\[)|\"(?=\\s*?\\{)|\\}(?=\\s*?\\{)|\"(?=\\s*?\")|\\}(?=\\s*?\")|\\](?=\\s*?\")|\\}(?=\\s*?\\[)");
	for (std::sregex_iterator i = std::sregex_iterator(JbeamTextSTR.begin(), JbeamTextSTR.end(), regex_missing_comma);
         i != std::sregex_iterator();
         ++i)
	{
		std::smatch m = *i;
		//std::cout << m.str() << " at position " << m.position() << '\n';
		mcpos.push_back(m.position());
	}
    
	offset = 0;
	for (ui16 i = 0; i < mcpos.size(); i++)
	{
		JbeamTextSTR.insert(mcpos[i] + offset + 1, ",");
		offset += 1;
	}
    
	regex regex_trailing_comma("\\,(?=\\s*?[\\}\\]])");
    
	for (std::sregex_iterator i = std::sregex_iterator(JbeamTextSTR.begin(), JbeamTextSTR.end(), regex_trailing_comma);
         i != std::sregex_iterator();
         ++i)
	{
		std::smatch m = *i;
		//std::cout << m.str() << " at position " << m.position() << '\n';
		missingPos = m.position();
		JbeamTextSTR.replace(missingPos, 1, " ");
	}
    
	string output = JbeamTextSTR.data();
	return output;
}

void export_jbeam(vector <jvertex> *vertices_data, char* filename)
{
	std::string filename_out = filename;
	filename_out += ".jbeam";
	std::ofstream file(filename_out);
    
	file << "";
}

Model import_model(char *file)
{
	Model model;
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string err;
    
	bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, file);
    
	if (err.empty()) {
		std::cerr << err << std::endl;
        model.did_import_fail = true;
        return model;
	}
    
    // Loop over shapes
    for (size_t s = 0; s < shapes.size(); s++) {
        // Loop over faces(polygon)
        size_t index_offset = 0;
        for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
            int fv = shapes[s].mesh.num_face_vertices[f];
            
            // Loop over vertices in the face.
            for (size_t v = 0; v < fv; v++) {
                // access to jvertex
                tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
                tinyobj::real_t vx = attrib.vertices[3 * idx.vertex_index + 0];
                tinyobj::real_t vy = attrib.vertices[3 * idx.vertex_index + 1];
                tinyobj::real_t vz = attrib.vertices[3 * idx.vertex_index + 2];
                tinyobj::real_t nx = attrib.normals[3 * idx.normal_index + 0];
                tinyobj::real_t ny = attrib.normals[3 * idx.normal_index + 1];
                tinyobj::real_t nz = attrib.normals[3 * idx.normal_index + 2];
                //tinyobj::real_t tx = attrib.texcoords[2 * idx.texcoord_index + 0];
                //tinyobj::real_t ty = attrib.texcoords[2 * idx.texcoord_index + 1];
                // Optional: jvertex colors
                // tinyobj::real_t red = attrib.colors[3*idx.vertex_index+0];
                // tinyobj::real_t green = attrib.colors[3*idx.vertex_index+1];
                // tinyobj::real_t blue = attrib.colors[3*idx.vertex_index+2];
                //model.vertices_data.push_back(vec3(vx, vy, vz));
            }
            index_offset += fv;
            
            // per-face material
            shapes[s].mesh.material_ids[f];
        }
    }
	
    model.init(attrib.vertices.size() / 3);
    
    for (int i = 0; i < shapes[0].mesh.indices.size(); i++)
    {
        model.indices.push_back(shapes[0].mesh.indices[i].vertex_index);
    }
    
	vec3 temp;
	for (int i = 0; i < model.vertices_size; i++)
    {
        temp.x = attrib.vertices[0 + i * 3];
        temp.y = attrib.vertices[1 + i * 3];
        temp.z = attrib.vertices[2 + i * 3];
        model.vertices_data.push_back(temp);
    }
	
	
    model.init_gl_buffers();
    
    assert(model.vertices_size != 0);
    model.did_import_fail = false;
    return model;
}


struct log_text_list {
	uc16 type = 0x0;
	string tag;
	vec3 v3;
	vec4 v4;
	float f;
	int i;
};

vector<log_text_list> log_text_group;
string log_file;

void logpushv3f(string tag, vec3 v)
{
	log_text_list t;
	set_active(t.type, IMGV3);
	t.tag = tag;
	t.v3 = v;
	log_text_group.push_back(t);
}


void logpushv4f(string tag, vec4 v)
{
	log_text_list t;
	set_active(t.type, IMGV4);
	t.tag = tag;
	t.v4 = v;
	log_text_group.push_back(t);
}

void logpushf(string tag, float f)
{
	log_text_list t;
	set_active(t.type, IMGF);
	t.tag = tag;
	t.f = f;
	log_text_group.push_back(t);
}

void logpushi(string tag, int i)
{
	log_text_list t;
	set_active(t.type, IMGI);
	t.tag = tag;
	t.i = i;
	log_text_group.push_back(t);
}

void log_dump_list()
{
	if (log_text_group.size() > 0)
	{
		for(ui32 i = 0; i < log_text_group.size(); i++)
		{
			log_text_list *it = &log_text_group[i];
			if(it->type & IMGV3)
			{
				log_file += it->tag.c_str();
				log_file += ": ";
				log_file += to_string(it->v3.x);
				log_file += ", ";
				log_file += to_string(it->v3.y);
				log_file += ", ";
				log_file += to_string(it->v3.z);
				log_file += "\n";
			}
			else if(it->type & IMGV4)
			{
				log_file += it->tag.c_str();
				log_file += ": ";
				log_file += to_string(it->v4.x);
				log_file += ", ";
				log_file += to_string(it->v4.y);
				log_file += ", ";
				log_file += to_string(it->v4.z);
				log_file += ", ";
				log_file += to_string(it->v4.w);
				log_file += "\n";
			}
			else if(it->type & IMGF)
			{
				log_file += it->tag.c_str();
				log_file += ": ";
				log_file += to_string(it->f);
				log_file += "\n";
			}
			else if(it->type &IMGI)
			{
				log_file += it->tag.c_str();
				log_file += ": ";
				log_file += to_string(it->i);
				log_file += "\n";
			}
		}
	}
	write_file("log.txt", log_file);
	log_file.clear();
}

