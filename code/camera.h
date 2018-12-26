#ifndef camera_h
#define camera_h

#define pi 3.14159265359f

using namespace glm;

const GLfloat YAW        = -90.0f;
const GLfloat PITCH      = 0.0f;
const GLfloat SPEED      = 24.0f;
const GLfloat SENSITIVTY = 30.0f;
const GLfloat ZOOM       = 45.0f;
const GLfloat ZOOM_SENSITIVITY = 5.0f;

struct Camera {

	vec3 pos;
	vec3 model_center_mass;
	vec3 forward;
	vec3 right;
	vec3 up;
	vec3 world_up;
	vec3 direction;
	vec3 origin;

	float speed;
	float yaw;
	float pitch;
	float pitch_orbit;
	float yaw_orbit;
	float zoom;
	float orbit_distance;
	float mouse_sensitivity;
	float zoom_sensitivity;
	bool free_cam_mode;
	bool orbit_mode;
	bool mouse_look;
	mat4  mat_view;
	mat4  mat_projection;
	mat4  mat_fixed;
	mat4  mat_rotation;
	
	void init_camera(vec3 pos, vec3 center_mass, vec3 forward, vec3 up,
		float yaw = YAW, float pitch = PITCH,
		float speed = SPEED, float mouse_sensitivity = SENSITIVTY, 
		float zoom = ZOOM, float zoom_sensitivity = ZOOM_SENSITIVITY)
	{
		this->zoom				= 75.0f;
		this->speed				= 8.0f;
		this->pos               = pos;
		this->forward           = vec3(0, 0, -1);
		this->right             = vec3(1, 0, 0);
		this->up				= vec3(0, 1, 0);
		this->world_up          = up;
		this->yaw               = yaw;
		this->pitch             = pitch;
		this->yaw_orbit         = yaw;
		this->pitch_orbit       = pitch;
		this->mouse_sensitivity = 0.5f;
		this->zoom_sensitivity  = zoom_sensitivity;
		this->free_cam_mode     = true;
		this->mouse_look        = true;
		this->orbit_distance    = 2.0f;
		this->mat_projection = glm::perspective(glm::radians(this->zoom), 4.0f / 3.0f, 0.1f, 10.0f);
		this->mat_view       = glm::lookAt(vec3(0.0f, 0.0f, 2.0f), this->forward, this->world_up);
		this->model_center_mass = center_mass;
	}

	mat4 get_mat()
	{
		return this->mat_view;
	}

	vec3 f()
	{
		return vec3(get_mat()[2][0], get_mat()[2][1], get_mat()[2][2]);
	}
	vec3 u()
	{
		return vec3(get_mat()[1][0], get_mat()[1][1], get_mat()[1][2]);
	}
	vec3 r()
	{
		return vec3(get_mat()[0][0], get_mat()[0][1], get_mat()[0][2]);
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
	camera->pos.y += input_state->a_up * -camera->speed * dt;
	camera->pos.y += input_state->a_down * camera->speed * dt;
	camera->pos += input_state->a_right * camera->speed * camera->right * dt;
	camera->pos -= input_state->a_left * camera->speed * camera->right * dt;
	camera->pos += input_state->a_forward * camera->speed * camera->forward *  dt;
	camera->pos -= input_state->a_backward  * camera->speed * camera->forward * dt;
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

#endif
