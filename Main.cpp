#include <math.h>
#include <array>
#include <vector>
#include <algorithm>
#include <iostream>
#include <SDL2/SDL.h>

using namespace std;

class Point
{
public:
	Point(int x, int y);

	int x;
	int y;
};

Point::Point(int x = 0, int y = 0)
{
	this->x = x;
	this->y = y;
}

class Matrix
{
public:
	Matrix(int width, int height);

	int width;
	int height;

	void SetPoint(Point* point, bool value);
	bool GetPoint(Point* point);
private:
	bool** arr;
};

Matrix::Matrix(int width, int height)
{
	this->width = width;
	this->height = height;

	arr = new bool*[height];

	for(int i = 0; i < height; i++)
	{
		arr[i] = new bool[width];
	}
}

void Matrix::SetPoint(Point* point, bool value)
{
	arr[point->x][point->y] = value;
}

bool Matrix::GetPoint(Point* point)
{
	return arr[point->x][point->y];
}

Matrix* DrawCircle(int radius, bool fill)
{
	Matrix* sprite = new Matrix(((2 * radius) + 10), ((2 * radius) + 10));

	float a = 0;
	float ad = M_PI / 180;

	while(true)
	{
		Point* od = new Point();

		od->x = sin(a) * radius;
		od->y = cos(a) * radius;

		int x1 = (radius - od->x);
		int x2 = (radius + od->x);

		int y1 = (radius - od->y);
		int y2 = (radius + od->y);

		delete od;

		if(fill)
		{
			for(int x = x1; x <= x2; x++)
			{
				Point* p1 = new Point(x, y1);
				Point* p2 = new Point(x, y2);

				sprite->SetPoint(p1, true);
				sprite->SetPoint(p2, true);

				delete p1;
				delete p2;
			}
		}
		else
		{
			Point* p1 = new Point(x1, y1);
			Point* p2 = new Point(x1, y2);
			Point* p3 = new Point(x2, y1);
			Point* p4 = new Point(x2, y2);

			sprite->SetPoint(p1, true);
			sprite->SetPoint(p2, true);
			sprite->SetPoint(p3, true);
			sprite->SetPoint(p4, true);

			delete p1;
			delete p2;
			delete p3;
			delete p4;
		}

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
	Object(float mass, Point* position);

	float mass;
	Point* position;
	Point* velocity;
	Matrix* sprite;
	vector<Point*> forces;
};

Object::Object(float mass, Point* position)
{
	this->mass = mass;
	this->position = position;
	this->velocity = new Point(0, 0);
}

class Ball : public Object
{
public:
	Ball(float mass, Point* position, int radius);
};

Ball::Ball(float mass, Point* position, int radius) : Object(mass, position)
{
	sprite = DrawCircle(radius, true);
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

	objects[0] = new Ball(1, new Point(100, 100), 5);
}

void Simulator::Tick()
{
	for(unsigned i = 0; i < objects.size(); i++)
	{
		Object* object = objects[i];

		for(int j = 0; j < object->sprite->width; j++)
		{
			for(int k = 0; k < object->sprite->height; k++)
			{
				Point* particle = new Point(j, k);

				bool isThereAParticle = object->sprite->GetPoint(particle);

				if(isThereAParticle)
				{
					Point* pixel = new Point();

					pixel->x = object->position->x - (object->sprite->width / 2) + particle->x;
					pixel->y = object->position->y - (object->sprite->height / 2) + particle->y;

					SDL_RenderDrawPoint(renderer, pixel->x, pixel->y);

					delete pixel;
				}

				delete particle;
			}
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
