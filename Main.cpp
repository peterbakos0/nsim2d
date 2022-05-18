#include <math.h>
#include <array>
#include <vector>
#include <algorithm>
#include <iostream>
#include <SDL2/SDL.h>

using namespace std;

class Point2D
{
public:
	Point2D(int x, int y);

	int x;
	int y;
};

Point2D::Point2D(int x = 0, int y = 0)
{
	this->x = x;
	this->y = y;
}

vector<Point2D*> DrawCircle(int radius, bool fill)
{
	vector<Point2D*> sprite;

	float a = 0;
	float ad = M_PI / radius / 3;

	while(true)
	{
		Point2D* od = new Point2D();

		od->x = sin(a) * radius;
		od->y = cos(a) * radius;

		if(fill)
		{
			for(int xd = -od->x; xd <= od->x; xd++)
			{
				sprite.push_back(new Point2D(xd, od->y));

				if(od->y != 0)
				{
					sprite.push_back(new Point2D(xd, -od->y));
				}
			}
		}
		else
		{
			sprite.insert(sprite.end(), {
				new Point2D(od->x, od->y),
				new Point2D(od->x, -od->y),
				new Point2D(-od->x, od->y),
				new Point2D(-od->x, -od->y)
			});
		}

		delete od;

		if(a > (M_PI / 2))
		{
			break;
		}

		a += ad;
	}

	return sprite;
}

class Object
{
public:
	Object(float mass, Point2D* position);

	float mass;
	Point2D* position;
	Point2D* velocity;
	vector<Point2D*> forces;
	vector<Point2D*> sprite;
};

Object::Object(float mass, Point2D* position)
{
	this->mass = mass;
	this->position = position;
	this->velocity = new Point2D(0, 0);
}

class Ball : public Object
{
public:
	Ball(float mass, Point2D* position, int radius);
};

Ball::Ball(float mass, Point2D* position, int radius) : Object(mass, position)
{
	sprite = DrawCircle(radius, true);
	cout << sprite.size() << endl;
}

class Simulator
{
public:
	Simulator(SDL_Renderer* renderer);

	void Tick();
private:
	SDL_Renderer* renderer;
	array<Object*, 1> objects;
};

Simulator::Simulator(SDL_Renderer* renderer)
{
	this->renderer = renderer;

	objects[0] = new Ball(1, new Point2D(100, 100), 5);
}

void Simulator::Tick()
{
	for(unsigned i = 0; i < objects.size(); i++)
	{
		Object* object = objects[i];

		for(unsigned j = 0; j < object->sprite.size(); j++)
		{
			Point2D* particle = object->sprite[j];

			Point2D* pixel = new Point2D();

			pixel->x = object->position->x + particle->x;
			pixel->y = object->position->y + particle->y;

			SDL_RenderDrawPoint(renderer, pixel->x, pixel->y);

			delete pixel;
		}
	}
}

int main()
{
	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_Window* window = SDL_CreateWindow(
		"Physics Simulation 1",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		512,
		512,
		0
	);

	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

	Simulator* simulator = new Simulator(renderer);

	bool quit = false;

	while(!quit)
	{
		SDL_Event event;

		while(SDL_PollEvent(&event) != 0)
		{
			switch(event.type)
			{
				case SDL_QUIT:
					quit = true;
					break;
				case SDL_KEYUP:
					quit = (event.key.keysym.scancode == 41);
					break;
			}
		}

		SDL_RenderClear(renderer);

		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		simulator->Tick();

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

		SDL_RenderPresent(renderer);
	}

	return 0;
}
