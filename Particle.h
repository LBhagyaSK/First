#ifndef PARTICLE
#define PARTICLE

#include <vector>

#include "Shader.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <array>

struct vec3
{
	float x, y, z;

};

struct vec4
{
	float x, y, z, t;
};
struct Vertex
{
	glm::vec3 Position,Direction;
	vec4 Color;
	glm::vec3 Offset,direction;
	float life;
};
struct Particles
{
	glm::vec3 origin, direction, position;
	//glm::vec2 velocity;
	float life;
	glm::vec4 color;

};



class  Particle
{
	glm::vec3  origin;
	glm::vec3 direction;
	vec3 Direction;
	

	

public:
	Particle(Shader shader, int amount, float dt);
	std::vector<Vertex> particles;
	Shader shader;
	int amount;
	float dt;
	unsigned int VAO, VBO;
	glm::vec3 setOrigin();
	glm::vec3 setDirection();
	vec3 setD();
	void Init();
	//void Generate(Particles &particles);
	void Generate(Vertex& particles);
	void Draw();
	//float gV = 1.0f;
	void Update();

	Vertex* CreateQuad(Vertex* target, glm::vec3 particle, float x, float y);
	Vertex* CreateQuad1(Vertex* target, float x, float y);

	


	


};
#endif // !PARTICLE





