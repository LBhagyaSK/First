

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
	//srand((unsigned) glfwGetTime());
	float random1 = ((rand() % 100) / 100.0f)-0.5 ;
	float random2 = ((rand() % 100) / 100.0f)-0.5;
	float random3 = ((rand() % 100) / 100.0f) - 0.5;

	Particle::direction = glm::vec3(random1, random2,random3);
	return Particle::direction;
}

static const size_t MaxQuadCount = 50000;
static const size_t MaxVertexCount = MaxQuadCount * 4;
static const size_t MaxIndexCount = MaxQuadCount * 6;
void Particle::Draw()
{
	
	uint32_t count = 0;
	static std::array<Vertex, 200000> vertices;

	Vertex* buffer = vertices.data();

	float r = 0.001f;
	float i = 0.0f;
	
			for (Vertex particle : this->particles)
			{
				float z = particle.Color.g;
				//z -= 0.008;
				this->shader.use();
				if (particle.life > 0.0f)
				{
					
					if (r < 0.1f)
					{

						float a = ((i * 3.14159) / 180.0);
						
						float x = r * cos(a);
						float y = r * sin(a);

						float xpos = x *10.0 * ((float)((rand() % 100) / 100.0f)-0.5);
						float ypos = y * y/2 *  ((float)((rand() % 100) / 100.0f)-0.5);
						float zpos = ((float)((rand() % 100) / 100.0f) - 0.5);

						

						if (xpos > 0.01 )
						{ 
							if (z < 0.01)
							{
								
									buffer = Particle::CreateQuad(buffer, particle.size, glm::vec3(0, 0, 0), particle.Position, z, 0.0f, 0.0f);
									count += 6;
									r = r + 0.1f;
									i = i + 10.0f;
							}
							else
							{
								z = 0.0f;
							}
							
						
						}
						else if (xpos < 0.015)
						{
							//float w = (1 / 3) * 3.14159 * sqrt(r) * x;
							
								//float z = particle.Color.g;


								buffer = Particle::CreateQuad(buffer, particle.size, glm::vec3(xpos/1.5, ypos/10.0f, zpos), particle.Position*0.5f, particle.Color.g*0.8, 0.0f, 0.0f);
								count += 6;
								r = r + 0.0001f;
								i = i + 1.0f;

						}
						else
						{
							i = 0.0f;
						}
						

					}

					else {
						r = 0.001;
					}
					

				}

				
			}
	
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());

	
			glClear(GL_COLOR_BUFFER_BIT);
			this->shader.use();

			
				
			glBindVertexArray(this->VAO);
			glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
	
}




void Particle::Update() {
	

	
	float x = 0.0f;
	//float y = 0.0f;
	float spred = 3.5f;
	float speedIncreaze = 0.000001;
	for (int  i = 0; i < this->amount; i++)
	{
		
		Vertex& p = this->particles[i];
		p.life -= this->dt;
		if (p.life > 0.0f)
		{

			glm::vec3 k = Particle::setDirection();
			p.Position -=glm::vec3(0.0f + x, (float)(((rand() % 100) / 100.0f) - 0.5) / 2.0f, 0.0f) * this->dt * (20.0f + speedIncreaze);// glm::vec3(0.0f + x * spred, ((rand() % 100) / 100.0f) - 0.5f, ((rand() % 100) / 100.0f) * spred);// //glm::vec3(0.0f+x, 0.0f+y, 0.0f+z) * this->dt* (float)(((rand() % 100) / 100.0f) - 0.5)*5.0f;//p.Direction * this->dt;//+ (float)glfwGetTime()/100);
			p.Color.g += 0.0008f;
			p.size -= speedIncreaze;
			
			x += 0.12f;
			//y += 0.1f;
			//z += 0.0001f;
			x = sqrt(x)*x/2.0f;
			//y = (sqrt(y)*y) / sqrt(2.0f);
			
		}

		else {

			p.Position = glm::vec3(0.0f, 0.0f, 0.0f);// Particle::setDirection(); //glm::vec3( 0.0f, 0.0f, 0.0f );
			p.Color =  glm::vec4(1.0f, 0.0f, 0.0f, 1.0f );
			p.life = (((rand() % 100) / 100.0f));
			p.size = 0.0008f;
			
		}
	}
}



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
	glBufferData(GL_ARRAY_BUFFER, MaxVertexCount * sizeof(Vertex),nullptr, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Position));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Color));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Offset));
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
	
	particles.Position = glm::vec3(0.0f, 0.0f, 0.0f); //Particle::setDirection();//glm::vec3(0.0f, 0.0f, 0.0f);
	particles.Color = glm::vec4( 1.0f, 0.0f, 0.0f, 1.0f );
	particles.size = 0.0008f;
	//particles.Color.g = 1.0f;

}



Vertex* Particle::CreateQuad(Vertex* target,float size,glm::vec3 particle,glm::vec3 direction,float z,float x, float y) {
		

		/*for (Vertex particle : this->particles)
		{

			//this->shader.use();
			if (particle.life > 0.0f)
			{*/

				//float size = 0.01f;

				target->Position = glm::vec3(x, y, 0.0f);
				target->Color = glm::vec4(1.0,z,0.0, 1.0);
				target->Offset = particle+direction;
				target++;

				target->Position = glm::vec3( x + size,y,0.0f );
				target->Color = glm::vec4(1.0,z, 0.0, 1.0);
				target->Offset = particle+direction;
				target++;

				target->Position =glm::vec3( x + size,y + size,0.0f );
				target->Color = glm::vec4(1.0,z, 0.0, 1.0);
				target->Offset = particle+direction;
				target++;

				target->Position = glm::vec3( x ,y +size,0.0f );
				target->Color = glm::vec4(1.0,z, 0.0, 1.0);
				target->Offset = particle+direction;
				target++;
			//}
		//}

				return target;
			
		
	
}






