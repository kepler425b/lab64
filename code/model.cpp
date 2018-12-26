
struct Model {
    bool did_import_fail;
	vector<vec3> vertices_data;
	vector<unsigned int> indices;
	Transform transform;
	vec3 pos;
	size_t vertices_size;
	shader material;
	unsigned int index_buffer;
	unsigned int vertex_buffer;
    
	void init(size_t size)
	{
		this->vertices_size = size;
		transform.t = mat4(1.0f);
		transform.r = mat4(1.0f);
		pos = vec3(0);
	}
	void init_gl_buffers()
	{
		glGenBuffers(1, &index_buffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*indices.size(), &indices[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glGenBuffers(1, &vertex_buffer);
	}
};

struct JModel {
    bool did_import_fail;
	vector<object> data;
	size_t vertices_size;
	vec3 line_color;
	Transform transform;
	vec3 local_position;
	unsigned int vertex_buffer;
    
	void init_gl_buffers()
	{
		for (int osz = 0; osz < data.size(); osz++)
		{
			for (int bsz = 0; bsz < data[osz].beams.size(); bsz++)
			{
				if (data[osz].beams[bsz].beams.size() > 0)
				{
					glGenBuffers(1, &data[osz].beams[bsz].index_buffer);
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, data[osz].beams[bsz].index_buffer);
					glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*data[osz].beams[bsz].indices.size(), &data[osz].beams[bsz].indices[0], GL_STATIC_DRAW);
				}
			}
		}
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glGenBuffers(1, &vertex_buffer);
	}
};

JModel init_jbeam(vector<object> &file)
{
	JModel model;
    
	std::string err;
	model.data = file;
    
	model.init_gl_buffers();
	model.local_position = vec3(0);
	assert(model.vertices_size != 0);
    model.did_import_fail = false;
    return model;
}
