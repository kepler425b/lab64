#include "input.h"
#include "random.h"

using namespace glm;

bool process_input(SDL_Window *mainWindow, Input *input_state, display *display_info, Camera *camera)
{
	SDL_Event Event;
	while (SDL_PollEvent(&Event)) {
		if (Event.type == SDL_QUIT) {
			return true;
		}
		if (Event.type == SDL_WINDOWEVENT && Event.window.event == SDL_WINDOWEVENT_CLOSE && Event.window.windowID == SDL_GetWindowID(mainWindow))
			return true;
		switch (Event.type)
		{
			case SDL_WINDOWEVENT:
			switch (Event.window.event)
			{
				case SDL_WINDOWEVENT_RESIZED:
				{
					//resize(Event.window.data1, Event.window.data2);
					display_info->w = Event.window.data1;
					display_info->h = Event.window.data2;
					glViewport(0, 0, display_info->w, display_info->h);
					break;
					default:
					break;
				}
			}
			case SDL_MOUSEMOTION:
			{
				SDL_Keymod kmod = SDL_GetModState();
				if (Event.motion.state & SDL_BUTTON_LMASK)
				{
				}
				break;
			}
			case SDL_MOUSEBUTTONDOWN:
			{
				SDL_Keymod kmod = SDL_GetModState();
				if (Event.button.button & SDL_BUTTON_LMASK)
				{
					input_state->m_left = true;
				}
				if (Event.button.button & SDL_BUTTON_RMASK)
				{
					input_state->m_right = true;
				}
				if ((Event.button.button & SDL_BUTTON_MIDDLE))
				{
					input_state->m_middle = true;
				}
				break;
			}
			case SDL_MOUSEBUTTONUP:
			{
				if (Event.button.button & SDL_BUTTON_LMASK)
				{
					input_state->m_left = false;
					input_state->m_single_click = false;
				}
				if (Event.button.button & SDL_BUTTON_RMASK)
				{
					input_state->m_right = false;
				}
				if ((Event.button.button & SDL_BUTTON_MIDDLE))
				{
					input_state->m_middle = false;
				}
				break;
			}
			
			case SDL_MOUSEWHEEL:
			{
				if (Event.wheel.y > 0)
				{
					camera->zoom -= camera->zoom_sensitivity;
				}
				else if (Event.wheel.y < 0)
				{
					camera->zoom += camera->zoom_sensitivity;
				}
				if (Event.wheel.x > 0)
				{
				}
				else if (Event.wheel.x < 0)
				{
				}
				break;
			}
			case SDL_KEYDOWN:
			{
				switch (Event.key.keysym.sym)
				{
					case SDLK_m:
					display_info->wire_mode = !display_info->wire_mode;
					break;
					
					case SDLK_z:
					input_state->mouse_look = !input_state->mouse_look;
					input_state->mouse_mode_switch = true;
					break;
					
					case SDLK_o:
					camera->free_cam_mode = !camera->free_cam_mode;
					input_state->mouse_mode_switch = true;
					break;
					
					case SDLK_UP:
					input_state->a_up = true;
					//translation_delta_current.z += 1.000 * time_state.dt;
					break;
					
					case SDLK_DOWN:
					input_state->a_down = true;
					//translation_delta_current.z -= 1.000 * time_state.dt;
					break;
					
					case SDLK_RIGHT:
					input_state->a_right = true;
					//translation_delta_current.x += 1.000 * time_state.dt;
					break;
					
					case SDLK_LEFT:
					input_state->a_left = true;
					//translation_delta_current.x -= 1.000 * time_state.dt;
					break;
					
					case SDLK_a:
					input_state->a_left = true;
					break;
					
					case SDLK_d:
					input_state->a_right = true;
					break;
					
					case SDLK_w:
					input_state->a_forward = true;
					break;
					
					case SDLK_s:
					input_state->a_backward = true;
					break;
					
					case SDLK_LSHIFT:
					input_state->lshift = true;
					break;
					
					case SDLK_DELETE:
					//input_state->k_delete = true;
					break;
				}
				break;
			}
			case SDL_KEYUP:
			{
				switch (Event.key.keysym.sym)
				{
					case SDLK_UP:
					input_state->a_up = false;
					break;
					
					case SDLK_DOWN:
					input_state->a_down = false;
					break;
					
					case SDLK_RIGHT:
					input_state->a_right = false;
					break;
					
					case SDLK_LEFT:
					input_state->a_left = false;
					break;
					
					case SDLK_a:
					input_state->a_left = false;
					break;
					
					case SDLK_d:
					input_state->a_right = false;
					break;
					
					case SDLK_w:
					input_state->a_forward = false;
					break;
					
					case SDLK_s:
					input_state->a_backward = false;
					break;
					
					case SDLK_LSHIFT:
					input_state->lshift = false;
					break;
					
					case SDLK_DELETE:
					input_state->k_delete = true;
					break;
					
					case SDLK_RETURN:
					input_state->k_enter = true;
					break;
				}
				break;
			}
		}
	}
}