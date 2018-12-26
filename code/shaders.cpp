struct shader {
	ui32 buffer_id;
	bool is_inited = false;
	GLuint id;
	int u_view_location, u_model_location, u_projection_location, u_color_location, u_state, u_draw_outline, u_text_color, u_is_billboard, u_velocity;
	vector<vec4> rand_colors;
	vec4 color;
};

unsigned int compile_shader(unsigned int type, char *filepath)
{
	std::string shader_buffer = file_to_buf(filepath);
	unsigned int id = glCreateShader(type);
	const char* src = shader_buffer.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);
	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Shader error % " << message << endl;
		glDeleteShader(id);
		return 0;
	}
	return id;
}

shader create_shader(char* vertexShader, char* fragmentShader)
{
	shader result;
	unsigned int program = glCreateProgram();
	unsigned int vs = compile_shader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = compile_shader(GL_FRAGMENT_SHADER, fragmentShader);
	
	assert(vs && fs > 0);
	
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);
	glDeleteShader(vs);
	glDeleteShader(fs);
	
	result.id = program;
	glUseProgram(result.id);
	result.u_view_location = glGetUniformLocation(result.id, "m_view");
	//assert(result.u_view_location != -1);
	
	result.u_model_location = glGetUniformLocation(result.id, "m_model");
	//assert(result.u_model_location != -1);
	
	result.u_projection_location = glGetUniformLocation(result.id, "m_projection");
	//assert(result.u_projection_location != -1);
	
	result.u_color_location = glGetUniformLocation(result.id, "u_color");
	//assert(result.u_color_location != -1);
	
	result.u_draw_outline = glGetUniformLocation(result.id, "u_draw_outline");
	//assert(result.u_draw_outline != -1);
	
	result.u_state = glGetUniformLocation(result.id, "u_state");
	//assert(result.u_state != -1);
	
	result.u_is_billboard = glGetUniformLocation(result.id, "u_is_billboard");
	//assert(result.u_is_billboard != -1);
	
	result.u_text_color = glGetUniformLocation(result.id, "u_text_color");
	//assert(result.u_text_color != -1);
	
	result.u_velocity = glGetUniformLocation(result.id, "u_velocity");
	
	result.is_inited = true;
	
	glUseProgram(0);
	
	return result;
}

shader default_shader;
shader cursor_shader;
shader text_shader;
shader billboard_shader;
shader texture_shader;
shader circle_shader;
shader cube_shader;

