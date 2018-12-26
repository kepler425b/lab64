//arrays, buffers, size
#define asize(array) sizeof(array)/(sizeof(array[0]))
#define buffer_offset(i) ((void*)(i))
#define kiloBytes(value) value * 1024
#define megaBytes(value) value * kiloBytes(1024)
#define gigaBytes(value) value * megaBytes(1024)
#define teraBytes(value) value * gigaBytes(1024)

//colors
#define RED vec4(1, 0, 0, 1)
#define GREEN vec4(0, 1, 0, 1)
#define BLUE vec4(0, 0, 1, 1)
#define TRAN vec4(0.97f, 0.83f, 0.17, 1.0f)
#define WHITE vec4(1, 1, 1, 1)
//vectors
#define vec3_zero vec3(0, 0, 0)
#define vec3_up vec3(0, 1, 0)
#define vec3_right vec3(1, 0, 0)
#define vec3_forward vec3(0, 0, -1)
#define vec2_zero vec2(0, 0)
#define fvec2_zero fvec2(0, 0, 0)

//math functions
#define rand_range() (double)rand() / (double)RAND_MAX;
#define degrees_to_radians(angle_degrees) (angle_degrees * M_PI / 180.0)
#define clip(n, lower, upper) if (n < lower) n= lower; else if (n > upper) n= upper

//set bit flags
#define set_active(button, flag) button |= flag; 
#define set_inactive(button, flag) button &= ~flag;

uc16 IMGF = 1 << 0;
uc16 IMGV3 = 1 << 1;
uc16 IMGI = 1 << 2;
uc16 IMGV4 = 1 << 3;

struct imgui_text_list {
	uc16 type = 0x0;
	string tag;
	vec3 v3;
	vec4 v4;
	float f;
	int i;
};

vector<imgui_text_list> imgui_text_group;

void imgpushv3f(string tag, vec3 v)
{
	imgui_text_list t;
	set_active(t.type, IMGV3);
	t.tag = tag;
	t.v3 = v;
	imgui_text_group.push_back(t);
}

void imgpushv4f(string tag, vec4 v)
{
	imgui_text_list t;
	set_active(t.type, IMGV4);
	t.tag = tag;
	t.v4 = v;
	imgui_text_group.push_back(t);
}

void imgpushf(string tag, float f)
{
	imgui_text_list t;
	set_active(t.type, IMGF);
	t.tag = tag;
	t.f = f;
	imgui_text_group.push_back(t);
}

void imgpushi(string tag, int i)
{
	imgui_text_list t;
	set_active(t.type, IMGI);
	t.tag = tag;
	t.i = i;
	imgui_text_group.push_back(t);
}

void imgui_display_list()
{
	if (imgui_text_group.size() > 0)
	{
		for(ui32 i = 0; i < imgui_text_group.size(); i++)
		{
			imgui_text_list *it = &imgui_text_group[i];
			if(it->type & IMGV3)
			{
				string s = it->tag + ": %.3f, %.3f, %.3f";
				ImGui::Text(s.c_str(), it->v3.x, it->v3.y, it->v3.z);
			}
			else if(it->type & IMGV4)
			{
				string s = it->tag + ": %.3f, %.3f, %.3f, %.3f";
				ImGui::Text(s.c_str(), it->v4.x, it->v4.y, it->v4.z, it->v4.w);
			}
			else if(it->type & IMGF)
			{
				string s = it->tag + ": %.3f";
				ImGui::Text(s.c_str(), it->f);
			}
			else if(it->type &IMGI)
			{
				string s = it->tag + ": %d";
				ImGui::Text(s.c_str(), it->i);
			}
		}
	}
	imgui_text_group.clear();
}

float rand_frange(float min, float max)
{
	float r = (float)rand() / (float)RAND_MAX;
	return min + r * (max - min);
}

float rand_irange(ui32 min, ui32 max)
{
	float r = (ui32)rand() / (ui32)RAND_MAX;
	return min + r * (max - min);
}


vec3 rand_vec3(float min, float max)
{
	return vec3(rand_frange(min, max), rand_frange(min, max), rand_frange(min, max));
}

vec3 rand_ivec3(ui32 min, ui32 max)
{
	return vec3(rand_irange(min, max), rand_irange(min, max), rand_irange(min, max));
}

vec2 rand_vec2(float min, float max)
{
	return vec2(rand_frange(min, max), rand_frange(min, max));
}

vec4 rand_color(float min, float max, float amin)
{
	return vec4(rand_frange(min, max), rand_frange(min, max), rand_frange(min, max), rand_frange(amin, 1.0f));
}
