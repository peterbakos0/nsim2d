#include <cmath>
#include <array>
#include <vector>
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

	arr = new bool*[width];

	for(int i = 0; i < width; i++)
	{
		arr[i] = new bool[height]();
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
	Matrix* sprite = new Matrix(((2 * radius) + 3), (2 * radius));

	Point* currentPoint = new Point(0, radius);

	while(currentPoint->y > 0)
	{
		int dist = pow((pow(currentPoint->x, 2) + pow(currentPoint->y, 2)), 0.5);

		if(dist == radius)
		{
			currentPoint->x++;
			currentPoint->y--;
		}
		else if(dist < radius)
		{
			currentPoint->x++;
		}
		else
		{
			currentPoint->y--;
		}

		int x1 = (radius - currentPoint->x + 1);
		int x2 = (radius + currentPoint->x + 1);

		int y1 = (radius - currentPoint->y);
		int y2 = (radius + currentPoint->y);

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
	}

	delete currentPoint;

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
	Simulator();

	array<Object*, 1> objects;

	void Tick();
};

Simulator::Simulator()
{
	objects[0] = new Ball(1, new Point(200, 200), 100);
}

void Simulator::Tick()
{

}

void Render(Simulator* simulator, SDL_Renderer* renderer)
{
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

	for(unsigned i = 0; i < simulator->objects.size(); i++)
	{
		Object* object = simulator->objects[i];

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

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderPresent(renderer);
}

int main()
{
	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_Window* window = SDL_CreateWindow(
		"nsim2d",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		512,
		512,
		0
	);

	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

	Simulator* simulator = new Simulator();

	int ups = 60;
	int upf = 2;

	int uc = 0;

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

		if(uc % upf == 0)
		{
			Render(simulator, renderer);
		}

		simulator->Tick();

		SDL_Delay(1000 / ups);

		uc++;
	}

	return 0;
}
