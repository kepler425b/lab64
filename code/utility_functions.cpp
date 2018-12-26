
struct shader;

inline UINT32 SafeTruncateUINT64(UINT64 value)
{
	UINT32 Result = (UINT32)value;
	return Result;
}

inline UINT32 BitSwap32(UINT32 value)
{
	value = ((value << 8) & 0xFF00FF00) | ((value >> 8) & 0xFF00FF);
	return (value << 16) | (value >> 16);
}

void debug_line(vec3 a, vec3 b, vec4 color, shader *s, Camera *camera);

float map_to_range(float value, float leftMin, float leftMax, float rightMin, float rightMax)
{
	float leftSpan = leftMax - leftMin;
	float rightSpan = rightMax - rightMin;
	float valueScaled = float(value - leftMin) / float(leftSpan);
	return rightMin + (valueScaled * rightSpan);
}

float map_to_unit_range(float value, float max)
{
	return 1.0f - ((max - value) / max);
}


bool
write_file(char* fileName, string data)
{
	bool error = false;
    
	HANDLE fileHandle = CreateFileA(fileName, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, 0, 0);
	if (fileHandle != INVALID_HANDLE_VALUE)
	{
		DWORD BytesWritten;
		if (WriteFile(fileHandle, data.c_str(), data.length(), &BytesWritten, 0) && (data.length() == BytesWritten))
		{
			error = 1;
			CloseHandle(fileHandle);
		}
		else
		{
			error = 0;
		}
	}
	else
	{
		fileHandle = 0;
	}
	return error;
}

float clipf(float n, float lower, float upper)
{
	return std::fmax(lower, std::fmin(n, upper));
}

size_t* my_malloc(size_t s, unsigned int bytes)
{
	size_t *ret = (size_t*)malloc(bytes * s);
	*ret = s;
	return &ret[1];
}

void my_free(void *ptr)
{
	free((size_t*)ptr - 1);
}

size_t allocated_size(void *ptr)
{
	return ((size_t*)ptr)[-1];
}

vec3 rotate_vec3(vec3 v, float a, vec3 n)
{
	float cos = cosf(a);
	float sin = sinf(a);
	vec3 result;
	//result = v + n;
	result.x = n.x * cos - n.y * sin;
	result.y = n.x * sin + n.y * cos;
	return result * v;
}

vec3 vec3_midpoint(vec3 a, vec3 b)
{
	vec3 t;
	t.x = (a.x + b.x) / 2;
	t.y = (a.y + b.y) / 2;
	t.z = (a.z + b.z) / 2;
	return t;
}

vec3 midpoint_4v(vec3 *a, vec3 *b, vec3 *c, vec3 *d)
{
	vec3 t;
	t.x = (a->x + b->x + c->x + d->x) / 2;
	t.y = (a->y + b->y + c->y + d->y) / 2;
	t.z = (a->z + b->z + c->z + d->z) / 2;
	return t;
}

vec3 get_mouse_3d(float depth, Camera camera)
{
	vec3 result;
	glReadBuffer(GL_FRONT);
	ivec4 viewport;
	glGetIntegerv(GL_VIEWPORT, &viewport[0]);
	int mx, my;
	SDL_GetMouseState(&mx, &my);
	int win_x, win_y;
	float win_z;
	win_x = mx;
	win_y = viewport[3] - my;
	glReadPixels(win_x, win_y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &win_z);
	double pos_x, pos_y, pos_z;
	return result = glm::unProject(vec3(win_x, win_y, depth), camera.get_mat(), camera.mat_projection, viewport);
}

vec3 mouse_position(Camera *camera)
{
	vec3 result;
	glReadBuffer(GL_FRONT);
	ivec4 viewport;
	glGetIntegerv(GL_VIEWPORT, &viewport[0]);
	int mx, my;
	SDL_GetMouseState(&mx, &my);
	int win_x, win_y;
	float win_z;
	win_x = mx;
	win_y = viewport[3] - my;
	glReadPixels(win_x, win_y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &win_z);
	GLfloat clip_z = (win_z - 0.5f) * 2.0f;
	GLfloat world_z = ((camera->z_far - camera->z_near) / 2)* clip_z + (camera->z_far + camera->z_near) / 2;
	cout << "z : " << win_z << endl;
	cout << "w z : " << world_z << endl;
    
	return result = glm::unProject(vec3(win_x, win_y, win_z), camera->get_mat(), camera->mat_projection, viewport);
}


vec2 get2dPoint(vec3 point3D, mat4 viewMatrix,
                mat4 projectionMatrix, int width, int height) {
    
	mat4 viewProjectionMatrix = projectionMatrix * viewMatrix;
	//transform world to clipping coordinates
	point3D = viewProjectionMatrix * vec4(point3D, 1.0f);
	int winX = (int)std::round(((point3D.x + 1) / 2.0) *
                               width);
	//we calculate -point3D.getY() because the screen Y axis is
	//oriented top->down 
	int winY = (int)std::round(((1 - point3D.y) / 2.0) *
                               height);
	return vec2(winX, winY);
}


char* file_to_buf(char *file)
{
	FILE *fptr;
	long length;
	char *buf;
    
	fptr = fopen(file, "rb"); /* Open file for reading */
	if (!fptr) /* Return NULL on failure */
		return NULL;
	fseek(fptr, 0, SEEK_END); /* Seek to the end of the file */
	length = ftell(fptr); /* Find out how many bytes into the file we are */
	buf = (char*)malloc(length + 1); /* Allocate a buffer for the entire length of the file and a null terminator */
	fseek(fptr, 0, SEEK_SET); /* Go back to the beginning of the file */
	fread(buf, length, 1, fptr); /* Read the contents of the file in to the buffer */
	fclose(fptr); /* Close the file */
	buf[length] = 0; /* Null terminator */
    
	return buf; /* Return the buffer */
}
