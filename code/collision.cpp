

//NOTE: I should remove entities that passed out of the world by big distances.

void ResolveRigidBodyCollision(Entity *e)
{
    
	if(e->transform.position().y <= -2.0f)
	{
		vec3 dir = normalize(e->RB.velocity);
		vec3 nr = vec3(0, 1, 0);
		float l = dot(dir, nr);
		vec3 r = -2.0f*l*nr + dir;
		vec3 n = normalize(r);
		e->transform.translate(vec3(0, -2.0f - e->transform.position().y, 0));
		//e->RB.added_force = -n * e->RB.InvMass / 9.8f; 
		vec3 v = n * (0.4f * length(e->RB.velocity)); 
		e->RB.velocity = v;
		
		//tomato.add_force(vec3(16.0f, 0, 0.9f));
		//e->RB.acceleration = vec3_zero;
		//e->RB.velocity = vec3_zero;
		debug_line(n, n + r, RED+GREEN, &default_shader, &camera);
	}
	else if(e->transform.position().y >= 80.0f)
	{
		vec3 dir = normalize(e->RB.velocity);
		vec3 nr = vec3(0, 1.0f, 0);
		float l = dot(dir, nr);
		vec3 r = -2.0f*l*nr + dir;
		vec3 n = normalize(r);
		e->transform.translate(vec3(0, 80.0f - e->transform.position().y, 0));
		vec3 v = n * (0.4f * length(e->RB.velocity)); 
		e->RB.velocity = v;
		//tomato.add_force(vec3(16.0f, 0, 0.9f));
		//e->RB.acceleration = vec3_zero;
		//e->RB.velocity = vec3_zero;
		debug_line(n, n + r, RED+GREEN, &default_shader, &camera);
	}
}

struct AABBColInfo {
	Collider_Rect *a;
	Collider_Rect *b;
	float penetration;
	vec3 points[2];
	vec3 n;
};

bool AABBvsAABB(AABBColInfo *info)
{
	Collider_Rect *a = info->a;
	Collider_Rect *b = info->b;
	
	vec3 amin = a->min;
	vec3 amax = a->min + a->max;
	vec3 bmin = b->min;
	vec3 bmax = b->min + b->max;
	
	vec3 n = a->min - b->min;
#if 0
	vec3 ap = amin + a->max;
	ap.z += 0.025f;
	vec3 bp = bmin + b->max;
	bp.z += 0.025f;
	push_line(amin, ap, TRAN);
	push_line(bmin, bp, TRAN);
#endif
	
	float a_extenct = (amax.x - amin.x)/2;
	float b_extenct = (bmax.x - bmin.x)/2;
	
	float x_overlap = a_extenct + b_extenct - fabs(n.x);
	
	if(x_overlap > 0.0f)
	{
		vec3 ex = amin + vec3_right * x_overlap;
		ex.z += 0.025f;
		push_line(amin, ex, TRAN);
		ex = bmin + vec3_right * x_overlap;
		ex.z += 0.025f;
		push_line(bmin, ex, TRAN);
		
		float a_extenct = (amax.y - amin.y)/2;
		float b_extenct = (bmax.y - bmin.y)/2;
		
		float y_overlap = a_extenct + b_extenct - fabs(n.y);
		
		if(y_overlap > 0.0f)
		{
			vec3 ex = amin + vec3_up * y_overlap;
			ex.z += 0.025f;
			push_line(amin, ex, TRAN);
			ex = bmin + vec3_up * y_overlap;
			ex.z += 0.025f;
			push_line(bmin, ex, TRAN);
#if 0
			kpl_draw_text(text_info, "X:" + to_string(x_overlap) + " | Y:" + to_string(y_overlap), amax+vec3_up*0.25f, 0.75f, TRAN, 0);
#endif
			if(x_overlap < y_overlap)
			{
				if(n.x < 0)
					info->penetration = x_overlap;
				info->points[0].x = fmax(amin.x, bmin.x);
				info->points[0].y = fmax(amin.y, bmin.y);
				info->points[1].x = fmin(amax.x, bmax.x);
				info->points[1].y = fmin(amax.y, bmax.y);
				return 1;
			}
			else
			{
				if(n.y < 0)
				{
					info->n = vec3(0, 1, 0);
				}
				else
				{
					info->n = vec3(0, -1, 0);
				}
				info->penetration = y_overlap;
				info->points[0].x = fmax(amin.x, bmin.x);
				info->points[0].y = fmax(amin.y, bmin.y);
				info->points[1].x = fmin(amax.x, bmax.x);
				info->points[1].y = fmin(amax.y, bmax.y);
				return 1;
			}
		}
		else return 0;
	}
	else return 0;
}

bool TestAABBOverlap(AABBColInfo *info)
{
	Collider_Rect *a = info->a;
	Collider_Rect *b = info->b;
	
	vec3 amin = a->min;
	vec3 amax = a->min + a->max;
	vec3 bmin = b->min;
	vec3 bmax = b->min + b->max;
	
    float d1x = bmin.x - amax.x;
    float d1y = bmin.y - amax.y;
    float d2x = amin.x - bmax.x;
    float d2y = amin.y - bmax.y;
	
    if (d1x > 0.0f || d1y > 0.0f)
        return FALSE;
	
    if (d2x > 0.0f || d2y > 0.0f)
        return FALSE;
	
    return TRUE;
}


void CorrectPenetration(Entity *a, Entity *b)
{
	float k_slop = 0.005f;
	float percent = 0.01f;
	AABBColInfo info;
	info.a = &a->ColliderRect;
	info.b = &b->ColliderRect;
	if(AABBvsAABB(&info))
	{
		if(1)
		{
			vec3 correction = (fmax(info.penetration - k_slop, 0.0f) / (a->RB.InvMass + b->RB.InvMass)) * percent * info.n;
			a->transform.translate(-correction * a->RB.InvMass);
			b->transform.translate(correction * b->RB.InvMass);
			imgpushv3f("correction", correction);
		}
	}
}

void ResolveRectCollision(Entity *a, Entity *b)
{
	AABBColInfo info;
	info.a = &a->ColliderRect;
	info.b = &b->ColliderRect;
	
	if(AABBvsAABB(&info))
	{
		imgpushf("COLLIDES", 0);
		
		vec3 pa = a->transform.position();
		vec3 pb = b->transform.position();
		vec3 n = info.n;
		if(n.x != 0)
		{
			vec3 p1, p2;
			p1 = pa; 
			p2 = pa + n * info.penetration;
			p1.z += 0.025f;
			p2.z += 0.025f;
			push_line(p1, p2, WHITE);
		}
		else if(info.n.y != 0)
		{
			vec3 p1, p2;
			p1 = pa; 
			p2 = pa + n * info.penetration;
			p1.z += 0.025f;
			p2.z += 0.025f;
			push_line(p1, p2, WHITE);
		}
		
		//draw_rect(&default_shader, vec3(info.points[0].x, info.points[0].y, 0.05f), info.points[1].x,
		//info.points[1].y, vec3(1), 0, 1.0f, TRAN * 1.5f, &camera, 0);
		
		imgpushv3f("min", info.points[0]);
		imgpushv3f("max", info.points[1]);
		imgpushv3f("n", n);
#if 1
		vec3 RelativeVelocity = a->RB.velocity - b->RB.velocity;
		
		float Magnitude = dot(RelativeVelocity, n);
		float e = fmin(a->RB.e, b->RB.e);
		
		float j = -(1.0f + e) * Magnitude;
		//if(j == 0.0f) return;
		j /= a->RB.InvMass + b->RB.InvMass;
		
		vec3 Impulse = j * n;
		
		logpushv3f("Impulse", Impulse);
		
		a->RB.velocity += a->RB.InvMass * Impulse;
		b->RB.velocity -= b->RB.InvMass * Impulse;
		
		RelativeVelocity = a->RB.velocity - b->RB.velocity;
		vec3 t = (dot(RelativeVelocity, n) * n);
		vec3 fn = RelativeVelocity - t;
		
		vec3 TangentVector = normalize(fn);
		
		float fj = -dot(RelativeVelocity, TangentVector);
		//if(fj == 0.0f) return;
		
		fj /= a->RB.InvMass + b->RB.InvMass;
		
		vec3 FrictionImpulse = fj * fn;
		imgpushv3f("fn", FrictionImpulse);
		vec3 fp = a->ColliderRect.min + a->ColliderRect.max*0.5f;
		fp.z += 0.05f;
		float m = length(FrictionImpulse) / a->ColliderRect.max.x;
		push_line(fp, fp + TangentVector * m * 10.0f, WHITE);
		
		a->RB.velocity += a->RB.InvMass * FrictionImpulse;
		b->RB.velocity -= b->RB.InvMass * FrictionImpulse;
	}
#endif
}

Plane ComputePlane(vec3 a, vec3 b, vec3 c)
{
	Plane p;
	p.n = normalize(cross(b - a, c - a));
	vec3 end, start, edge;
	float dend, dstart, dedge;
	end = normalize(b - a);
	dend = distance(b, a);
	start = normalize(c - a);
	dstart = distance(c, a);
	edge = normalize(b - c);
	dedge = distance(b, c);
	p.d = dot(p.n, a);
	push_point(a, RED, 16);
	push_point(b, GREEN, 16);
	push_point(c, BLUE, 16);
	
	push_line(a, a + end * dend, RED);
	push_line(a, a + start * dstart, GREEN);
	push_line(c, c + edge * dedge, BLUE);
	
	push_line(a, a + end * 100.0f, RED);
	push_line(a, a + start * 100.f, GREEN);
	push_line(c, c + edge * 100.f, BLUE);
	vec3 mid = vec3_midpoint(a, c);
	push_line(mid, mid - p.n, TRAN);
	
	return p;
}

struct Ray_Info {
	vec3 point;
	vec3 direction;
};

bool cast_ray(RigidBody_Sphere &target, Transform *transform, Ray_Info &info, vec3 origin, vec3 direction, float length)
{
	vec3 d = normalize(direction);
	vec3 m = origin - transform->position();
	float b = dot(m, d);
	float c = dot(m, m) - target.r * target.r;
	debug_line(origin, origin + d * length, GREEN, &default_shader, &camera);
	cout << "c: " << c << endl;
	cout << "b: " << b << endl;
	if(c > 0.0f && b > 0.0f) return false;
	
	float discr = b*b - c;
	
	cout << "discr: " << discr << endl;
	if (discr < 0.0f) return false;
	debug_line(origin, origin + d * discr, GREEN, &default_shader, &camera);
	
	float t = -b - sqrt(discr);
	
	info.point = origin + t * d;
	
	cout << "RAY COLLISION" << endl;
	//info.contact_point = origin + (dn * ray_distance - t);
	//info.direction = dn;
	
	debug_point(info.point, GREEN, 16, &default_shader);
	debug_line(origin, d * t, RED + GREEN, &default_shader, &camera);
	
	if (t < 0.0f) 
	{
		t = 0.0f;
		info.point = origin + t * d;
		
		cout << "RAY STARTED INSIDE SPHERE" << endl;
		//info.contact_point = origin + (dn * ray_distance - t);
		//info.direction = dn;
		
		debug_point(info.point, GREEN, 16, &default_shader);
		debug_line(origin, d * t, RED + GREEN, &default_shader, &camera);
		return true;
	}
}

bool resolve_rect_collisions(Collider_Rect &a, Collider_Rect &b)
{
	if (a.max.x < b.min.x || a.min.x > b.max.x) return 0;
	if (a.max.y < b.min.y || a.min.y > b.max.y) return 0;
	//if (a.origin.z + a.r[2] < b.origin.z || a.origin.z > b.origin.z + b.r[2]) return 0;
	return 1;
}

bool resolve_sphere_collisions(RigidBody_Sphere &a, RigidBody_Sphere &b)
{
	vec3 tmp = a.p - b.p;
	float d = dot(tmp, tmp);
	float r = a.r + b.r;
	if (d <= r * r) return true;
	else return false;
}

bool col(vec3 &pA, vec3 &pB, jvertex &Target, bool &lshift)
{
	vec3 target_vec3 = { Target.pos.x, Target.pos.y, Target.pos.z };
	vec3 closest_p;
	vec3 p_dir = pB - pA;
	vec3 p_dir_n = normalize(p_dir);
	vec3 t_dir = target_vec3 - pA;
	float t_dis = distance(pB, pA);
	float t = dot(t_dir, p_dir_n);
	float result_dis;
	
	
	if (t <= 0.0)
	{
		closest_p = pA;
	}
	else if (t >= t_dis)
	{
		closest_p = pB;
	}
	else
	{
		closest_p = pA + p_dir_n * t;
	}
	
	result_dis = distance(target_vec3, closest_p);
	
	
	if (result_dis <= 0.02f) {
		Target.active = true;
		return true;
	}
	else
	{
		if (!lshift) Target.active = false;
		
		return false;
	}
}

vec3 col_area(vec3 &pA, vec3 &pB)
{
	vec3 closest_p;
	vec3 p_dir = pB - pA;
	vec3 p_dir_n = normalize(p_dir);
	float t_dis = distance(pB, pA);
	float result_dis;
	
	return vec3(pA.x * t_dis * pA.y);
}


bool col_edge(vec3 &pA, vec3 &pB, jvertex &Target)
{
	vec3 target_vec3 = { Target.pos.x, Target.pos.y, Target.pos.z };
	vec3 closest_p;
	vec3 p_dir = pB - pA;
	vec3 p_dir_n = normalize(p_dir);
	vec3 t_dir = target_vec3 - pA;
	float t_dis = distance(pB, pA);
	float t = dot(t_dir, p_dir_n);
	float result_dis;
	
	if (t <= 0.0)
	{
		closest_p = pA;
	}
	else if (t >= t_dis)
	{
		closest_p = pB;
	}
	else
	{
		closest_p = pA + p_dir_n * t;
	}
	
	result_dis = distance(target_vec3, closest_p);
	
	
	if (result_dis <= 0.8f) {
		Target.active = true;
		return true;
	}
	return false;
}

bool col_beam(vec3 &pA, vec3 &pB, beam_pointer *beam, bool &lshift)
{
	vec3 target_vec3 = vec3_midpoint(beam->a->pos, beam->b->pos);
	vec3 closest_p;
	vec3 p_dir = pB - pA;
	vec3 p_dir_n = normalize(p_dir);
	vec3 t_dir = target_vec3 - pA;
	float t_dis = distance(pB, pA);
	float t = dot(t_dir, p_dir_n);
	float result_dis;
	
	if (t <= 0.0)
	{
		closest_p = pA;
	}
	else if (t >= t_dis)
	{
		closest_p = pB;
	}
	else
	{
		closest_p = pA + p_dir_n * t;
	}
	
	result_dis = distance(target_vec3, closest_p);
	
	if (result_dis <= 0.05f) {
		beam->a->active = true;
		beam->b->active = true;
		return true;
	}
	else
	{
		if (!lshift)
		{
			beam->a->active = false;
			beam->b->active = false;
		}
		return false;
	}
}

bool col_beam_ishowered(vec3 &pA, vec3 &pB, beam_pointer *beam)
{
	vec3 target_vec3 = vec3_midpoint(beam->a->pos, beam->b->pos);
	vec3 closest_p;
	vec3 p_dir = pB - pA;
	vec3 p_dir_n = normalize(p_dir);
	vec3 t_dir = target_vec3 - pA;
	float t_dis = distance(pB, pA);
	float t = dot(t_dir, p_dir_n);
	float result_dis;
	
	if (t <= 0.0)
	{
		closest_p = pA;
	}
	else if (t >= t_dis)
	{
		closest_p = pB;
	}
	else
	{
		closest_p = pA + p_dir_n * t;
	}
	
	result_dis = distance(target_vec3, closest_p);
	
	if (result_dis <= 0.05f) {
		return true;
	}
	return false;
}

bool col2(vec3 &pA, vec3 &pB, vec3 &Target)
{
	vec3 target_vec3 = { Target.x, Target.y, Target.z };
	vec3 closest_p;
	vec3 p_dir = pB - pA;
	vec3 p_dir_n = normalize(p_dir);
	vec3 t_dir = target_vec3 - pA;
	float t_dis = distance(pB, pA);
	float t = dot(t_dir, p_dir_n);
	float result_dis;
	
	if (t <= 0.0)
	{
		closest_p = pA;
	}
	else if (t >= t_dis)
	{
		closest_p = pB;
	}
	else
	{
		closest_p = pA + p_dir_n * t;
	}
	
	result_dis = distance(target_vec3, closest_p);
	
	if (result_dis <= 0.05f) {
		return true;
	}
	return false;
}




