#include <SDL.h>
#include <string> 
#include <SDL_image.h>
#include <math.h>
#include <Windows.h>
#include <kepler_math.h>
#include <stdio.h>
#include <SDL_ttf.h>
SDL_Event Event;
SDL_Renderer *Renderer = NULL;
SDL_Window *Window = NULL;
SDL_Surface *Screen = NULL;


bool checkCollisions(SDL_Rect* A, SDL_Rect B[], int i, char *asd)
{
	int leftA, leftB;
	int rightA, rightB;
	int topA, topB;
	int bottomA, bottomB;

	leftA = A->x;
	rightA = A->x + A->w;
	topA = A->y;
	bottomA = A->y + A->h;

	leftB = B[i].x;
	rightB = B[i].x + B[i].w;
	topB = B[i].y;
	bottomB = B[i].y + B[i].h;

	if (leftA >= rightB)
	{
		return false;

	}
	if (rightA <= leftB)
	{
		return false;

	}
	if (topA >= bottomB)
	{
		return false;

	}
	if (bottomA <= topB)
	{
		return false;

	}
	return true;
}



struct Box {
	v2 delta = { 0, 0 };
	v2 pos = { 0,0 };
	v2 velocity = { 0, 0 };
	SDL_Rect body = { 0, 256, 32, 64 };
};

void renderRecangle(Box *rectangle, SDL_Texture* body, float dt)
{
	rectangle->velocity.y += gravity * dt;
	rectangle->pos.y += rectangle->velocity.y * dt;

	SDL_Rect rectPos;
	rectPos.x = rectangle->pos.x;
	rectPos.y = rectangle->pos.y;
	rectPos.h = rectangle->body.h;
	rectPos.w = rectangle->body.w;
	SDL_RenderCopyEx(Renderer, body, &rectangle->body, &rectPos, 0, 0, SDL_FLIP_NONE);
}

bool init()
{
	SDL_LogSetAllPriority(SDL_LOG_PRIORITY_WARN);
	bool success = true;
	if (SDL_Init(SDL_INIT_EVERYTHING))
	{
		printf("Failed to initilize SDL. SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		Window = SDL_CreateWindow("Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCW, SCH, SDL_WINDOW_RESIZABLE);
		if (Window == NULL)
		{
			printf("Failed creating window. SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			Renderer = SDL_CreateRenderer(Window, 0, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (IMG_Init(IMG_INIT_PNG))
			{
				printf("Failed creating window. SDL Error: %s\n", SDL_GetError());
			}
			TTF_Init();
		}
	}
	return success;
}

void close()
{
	SDL_DestroyWindow(Window);
	SDL_DestroyRenderer(Renderer);
	Window = NULL;
	Renderer = NULL;
	SDL_Quit();
	IMG_Quit();
}


SDL_Texture* loadTexture(std::string path)
{
	SDL_Texture* outputTexture = NULL;

	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		SDL_Log("Unable to load image: %s, SDL_Image error: %s", path.c_str(), IMG_GetError());
	}
	else
	{
		//SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 255, 255));
		outputTexture = SDL_CreateTextureFromSurface(Renderer, loadedSurface);
		if (outputTexture == NULL)
		{
			SDL_Log("Unable to create texture from surface: %s, SDL_Image error: %s", path.c_str(), IMG_GetError());
		}
		SDL_FreeSurface(loadedSurface);
	}
	return outputTexture;
}


typedef struct {
	SDL_Rect dim;
} object;

SDL_Rect *objArray[100] = { NULL };


void addObject(int x, int y, int w, int h)
{
	int found = -1;
	for (int i = 0; i < 100; i++)
	{
		if (objArray[i] == NULL)
		{
			found = i;
			break;
		}
	}
	if (found >= 0)
	{
		int i = found;
		objArray[i] = (SDL_Rect *)malloc(sizeof(SDL_Rect));
		objArray[i]->x = x;
		objArray[i]->y = y;
		objArray[i]->w = w;
		objArray[i]->h = h;
	}
}

SDL_Rect colArray[10];

void insertToColArray(SDL_Rect obj)
{
	int found = -1;
	for (int i = 0; i < 10; i++)
	{
		if (&colArray[i] == NULL)
		{
			found = i;
			break;
		}
	}
	if (found >= 0)
	{
		int i = found;
		colArray[i].x = obj.x;
		colArray[i].y = obj.y;
		colArray[i].w = obj.w;
		colArray[i].h = obj.h;
	}
}
