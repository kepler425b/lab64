#define pi 3.14159265359f

const GLfloat YAW = -90.0f;
const GLfloat PITCH = 0.0f;
const GLfloat SPEED = 24.0f;
const GLfloat SENSITIVTY = 30.0f;
const GLfloat ZOOM = 45.0f;
const GLfloat ZOOM_SENSITIVITY = 5.0f;


typedef enum {
    CMODE_ORTHOGRAPHIC= 0, 
    CMODE_FREE= 1,
    CMODE_ORBIT= 3
} CAMERA_MODE;


struct Camera {
    
	vec3 pos;
	vec3 model_center_mass;
	vec3 forward;
	vec3 right;
	vec3 up;
	vec3 world_up;
	vec3 direction;
	vec3 origin;
    
    CAMERA_MODE view_mode = CMODE_FREE;
	
	float z_near;
	float z_far;
	float speed;
	float yaw;
	float pitch;
	float pitch_orbit;
	float yaw_orbit;
	float zoom;
	float orbit_distance;
	float mouse_sensitivity;
	float zoom_sensitivity;
	bool mouse_look;
	mat4  mat_view;
	mat4  mat_projection;
	mat4  mat_fixed;
	mat4  mat_rotation;
    
	void init_camera(vec3 pos, vec3 center_mass, vec3 forward, vec3 up,
                     float yaw, float pitch,
                     float speed, float mouse_sensitivity,
                     float zoom, float zoom_sensitivity, float z_near, float z_far)
	{
		this->zoom = 75.0f;
		this->speed = 8.0f;
		this->pos = pos;
		this->forward = vec3(0, 0, -1);
		this->right = vec3(1, 0, 0);
		this->up = vec3(0, 1, 0);
		this->world_up = up;
		this->yaw = yaw;
		this->pitch = pitch;
		this->yaw_orbit = yaw;
		this->pitch_orbit = pitch;
		this->mouse_sensitivity = 0.5f;
		this->zoom_sensitivity = zoom_sensitivity;
		this->mouse_look = true;
		this->orbit_distance = 2.0f;
		this->mat_projection = glm::perspective(glm::radians(this->zoom), 4.0f / 3.0f, z_near, z_far);
		this->mat_view = glm::lookAt(vec3(0.0f, 0.0f, 2.0f), this->forward, this->world_up);
		this->model_center_mass = center_mass;
		this->z_near = z_near;
		this->z_far = z_far;
	}
    
	mat4 get_mat()
	{
		return this->mat_view;
	}
    
	vec3 f()
	{
		return vec3(mat_view[2][0], mat_view[2][1], mat_view[2][2]);
	}
	vec3 u()
	{
		return vec3(get_mat()[1][0], get_mat()[1][1], get_mat()[1][2]);
	}
	vec3 r()
	{
		return vec3(get_mat()[0][0], get_mat()[0][1], get_mat()[0][2]);
	}
	
	vec3 position()
	{
		return vec3(mat_view[3][0], mat_view[3][1], mat_view[3][2]);
	}
};

void camera_update_rotation(Camera *camera, const vec2 *mouse_dt, bool invertY)
{
	camera->yaw += mouse_dt->x * camera->mouse_sensitivity;
	if (invertY)
	{
		camera->pitch += mouse_dt->y * camera->mouse_sensitivity;
	}
	else
	{
		camera->pitch -= mouse_dt->y * camera->mouse_sensitivity;
	}
    
	if (camera->pitch >= 89.0f)
		camera->pitch = 89.0f;
    
	if (camera->pitch <= -89.0f)
		camera->pitch = -89.0f;
    
	glm::vec3 front;
	front.x = cos(radians(camera->yaw)) * cos(radians(camera->pitch));
	front.y = sin(radians(camera->pitch));
	front.z = sin(radians(camera->yaw)) * cos(radians(camera->pitch));
	camera->forward = normalize(front);
	camera->right = cross(camera->forward, camera->world_up);
	camera->up = cross(camera->right, camera->forward);
}

void camera_look_at(Camera *camera, vec3 origin, vec3 look)
{
	camera->mat_view = glm::lookAt(origin, look, camera->world_up);
}

void camera_update_translation(Camera *camera, Input *input_state, float dt)
{
	//camera->pos.y += input_state->k * -camera->speed * dt;
	//camera->pos.y += input_state->k_down * camera->speed * dt;
	camera->pos += input_state->k_right * camera->speed * camera->right * dt;
	camera->pos -= input_state->k_left * camera->speed * camera->right * dt;
	camera->pos += input_state->k_forward * camera->speed * camera->forward *  dt;
	camera->pos -= input_state->k_backward  * camera->speed * camera->forward * dt;
}

void camera_translate(Camera *camera, vec3 direction, float amount)
{
	camera->pos += camera->direction * amount;
}

void camera_rotate(Camera *camera, vec3 axis, float angle)
{
	vec4 f;
	f.x = camera->forward.x;
	f.y = camera->forward.y;
	f.z = camera->forward.z;
	f.w = 1.0f;
    
	camera->forward = f * rotate(mat4(1.0f), angle, axis);
}

Camera camera;
