

#include "Particle.h"

//#include <synchapi.h>


Particle::Particle(Shader shader, int amount, float dt) :shader(shader), amount(amount),dt(dt)
{
	
	this->Init();	
}

glm::vec3 Particle::setOrigin(){
	Particle::origin = glm::vec3(0.0f, 0.0f, 0.0f);
	return Particle::origin;
}

glm::vec3 Particle::setDirection() {
	float random1 = ((rand() % 100) / 100.0f)-0.5 ;
	float random2 = ((rand() % 100) / 100.0f)-0.5;
	float random3 = ((rand() % 100) / 100.0f) - 0.5;

	Particle::direction = glm::vec3(random1, random2,random3);
	return Particle::direction;
}



void Particle::Draw()
{
	//Particle::Update();

	
	
	uint32_t count = 0;
	static std::array<Vertex, 4000000> vertices;

	Vertex* buffer = vertices.data();


	/*for (int y = 0; y < 5; y++)
	{
		for (int x = 0; x < 5; x++)
		{*/
			for (Vertex particle : this->particles)
			{

				this->shader.use();
				if (particle.life > 0.0f)
				{

		
					float z = particle.Color.b;
					buffer = Particle::CreateQuad(buffer, particle.Position, z,0.0f,0.0f);
					count += 6;


				}

			}
	//auto q0 = CreateQuad(-0.75f, -0.5f);
	/*auto q1 = CreateQuad(0.5f, -0.5f);

	Vertex vertices[8];
	memcpy(vertices, q0.data(), q0.size() * sizeof(Vertex));
	memcpy(vertices + q0.size(), q1.data(), q1.size() * sizeof(Vertex));*/
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());

	/*glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);*/
	//Particle::Update();
	/*for (Particles particle : this->particles)
	{
		
		/*this->shader.use();
		if (particle.life > 0.0f)
		{
			this->shader.setVec3("offset", particle.position);
			float timeValue = glfwGetTime();
			float greenValue = (0.5f / timeValue);

			//for (float i = particle.color.b;i >= 0.0f;  )
			/*float i = 1.0f;
			particle.color.b = i;
			while (1)
			{
				particle.color.b -= 0.25f;*/
				/*if (particle.color.b == 0.0f) {
					//if (greenValue == 0.0f) {
					glm::vec4 ourColor = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
					
					this->shader.setVec4("ourColor", ourColor);
					particle.color.b = 1.0f;
				}
				else
				{
					glm::vec4 ourColor = glm::vec4(0.0f, 0.0f, particle.color.b, 1.0f);
					this->shader.setVec4("ourColor", ourColor);
				}
				
				
			//}*/

			glClear(GL_COLOR_BUFFER_BIT);
			this->shader.use();

			
				
			glBindVertexArray(this->VAO);
			glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		//}
		
	//}
	

}


/*void Particle::Generate(Particles& particles)
{
	particles.origin = Particle::setOrigin();
	particles.direction = Particle::setDirection();
	//particles.velocity = glm::vec2(0.2f,0.3f);
	particles.life = (((rand() % 100) / 100.0f));
	particles.position = glm::vec3(0.0f, 0.0f,0.0f);
	particles.color = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);

}*/

void Particle::Update() {
	
	for (int  i = 0; i < this->amount; i++)
	{
		
		Vertex& p = this->particles[i];
		p.life -= this->dt;
		if (p.life > 0.0f)
		{
			p.Position -= p.Direction* this->dt;
			p.Color.b -= 0.1f;
			
			
		}

		else {

			p.Position = glm::vec3( 0.0f, 0.0f, 0.0f );
			p.Color =  glm::vec4(0.0f, 0.0f, 1.0f, 1.0f );
			p.life = (((rand() % 100) / 100.0f));
			
		}
	}
}

static const size_t MaxQuadCount = 1000000;
static const size_t MaxVertexCount = MaxQuadCount * 4;
static const size_t MaxIndexCount = MaxQuadCount * 6;


void Particle::Init()
{


	static uint32_t indices[MaxIndexCount];
	uint32_t offset = 0;
	for (size_t i = 0; i < MaxIndexCount; i += 6)
	{
		indices[i + 0] = 0 + offset;
		indices[i + 1] = 1 + offset;
		indices[i + 2] = 2 + offset;

		indices[i + 3] = 2 + offset;
		indices[i + 4] = 3 + offset;
		indices[i + 5] = 0 + offset;

		offset += 4;

	}


	
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);
	glGenBuffers(1, &this->EBO);

	glBindVertexArray(this->VAO);

	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, MaxVertexCount*sizeof(Vertex),nullptr, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Position));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Color));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Offset));
	glEnableVertexAttribArray(2);

	for (int i = 0; i < this->amount; i++) {
		particles.push_back(Vertex());
		Generate(particles[i]);
	}


	
}
void Particle::Generate(Vertex& particles)
{
	
	particles.Direction = Particle::setDirection();
	//particles.velocity = glm::vec2(0.2f,0.3f);
	particles.life = (((rand() % 100) / 100.0f));
	
	particles.Position = glm::vec3(0.0f, 0.0f, 0.0f);
	particles.Color = glm::vec4( 0.0f, 0.0f, 1.0f, 1.0f );

}



Vertex* Particle::CreateQuad(Vertex* target,glm::vec3 particle,float z,float x, float y) {

		

		/*for (Vertex particle : this->particles)
		{

			//this->shader.use();
			if (particle.life > 0.0f)
			{*/

				float size = 0.01f;

				target->Position = glm::vec3(x, y, 0.0f);
				target->Color = glm::vec4(0.0f, 0.0f,1.0f, 1.0);
				target->Offset = particle;
				target++;

				target->Position = glm::vec3( x + size,y,0.0f );
				target->Color = glm::vec4(0.0f, 0.0f, 1.0f, 1.0);
				target->Offset = particle;

				target->Position =glm::vec3( x + size,y + size,0.0f );
				target->Color = glm::vec4(0.0f, 0.0f, 1.0f, 1.0);
				target->Offset = particle;
				target++;

				target->Position = glm::vec3( x ,y +size,0.0f );
				target->Color = glm::vec4(0.0f, 0.0f, 1.0f, 1.0);
				target->Offset = particle;
				target++;
			//}
		//}

				return target;
			
		
	
}






