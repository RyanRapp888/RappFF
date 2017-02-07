
#include <glad.h>
#include "TiledGameBoard.h"
#include "Shader.h"
#include "GameMap.h"
#include "Character.h"
#include "Display.h"
#include "Mesh.h"
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

static void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

GLfloat vertices[] = {
	-1,0,2,
	1,0,2,
	1,0,0,
	-1,0,0,
	0,1,1
};

/*
indices to represent all of the faces (triangles) of the pyramid
*/
GLushort indices[] = {
	0,1,4,
	1,2,4,
	2,3,4,
	3,0,4
};

/*
colors for each pyramid instance rendered
*/
GLfloat colors[] = {
	1,0,0,
	0,1,0,
	0,0,1
};

GLuint VAO;
void display();

int main()
{
	
	glfwSetErrorCallback(error_callback);
	
	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}
	
    Display *display_ptr = new Display(800, 600, "Sandbox");
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	
	GLuint buffers[4];

	Shader program("res\\pyramid_examp");
	program.Bind();
	
	/* generate the needed vertex array so that we can use later when rendering */
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	/* generate buffers needed for the pyramid */
	glGenBuffers(4, buffers);

	/* Locations of buffers in array */
#define POSITION_BUFFER 0
#define INDEX_BUFFER 1
#define COLOR_BUFFER 2
#define TRANS_BUFFER 3

	/* Locations of attributes in vertex shader */
#define POSITION_ATTR_LOC 0
#define COLOR_ATTR_LOC 1
#define TRANS_ATTR_LOC 2

	/* buffer to store vertices */
	glBindBuffer(GL_ARRAY_BUFFER, buffers[POSITION_BUFFER]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(POSITION_ATTR_LOC);
	glVertexAttribPointer(POSITION_ATTR_LOC, 3, GL_FLOAT, GL_FALSE, 0, 0);

	/* index buffer */
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[INDEX_BUFFER]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	/* buffer for the colors of each pyramid */
	glBindBuffer(GL_ARRAY_BUFFER, buffers[COLOR_BUFFER]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
	glVertexAttribPointer(COLOR_ATTR_LOC, 3, GL_FLOAT, GL_FALSE, 0, 0);
	// set change per instance -> every instance will have a new entry from the array buffer
	glVertexAttribDivisor(COLOR_ATTR_LOC, 1);
	glEnableVertexAttribArray(COLOR_ATTR_LOC);

	/*
	create just translate transformations for each pyramid -> 3 pyramids
	*/
	glm::mat4 transf[] = {
		glm::translate(glm::mat4(1.0f), glm::vec3(-4.0f, -3.0f, -14.0f)),
		glm::translate(glm::mat4(1.0f), glm::vec3(3.0f, -1.0f, -17.0f)),
		glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, -2.0f, -9.0f))
	};
	glBindBuffer(GL_ARRAY_BUFFER, buffers[TRANS_BUFFER]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(transf), transf, GL_STATIC_DRAW);

	/*
	set the attrib location and enable it. since we have a 4x4 matrix we have
	4 columns of vectors of length 4.
	*/
	for (int i = 0; i < 4; ++i) {
		glVertexAttribPointer(TRANS_ATTR_LOC + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid *)(i * sizeof(glm::vec4)));
		glVertexAttribDivisor(TRANS_ATTR_LOC + i, 1);
		glEnableVertexAttribArray(TRANS_ATTR_LOC + i);
	}

	// done with VAO
	// unbind
	glBindVertexArray(0);

	/*
	create projection transformation and update the uniform
	*/
	GLint projectionLoc = glGetUniformLocation(program.GetProgramId(), "projection");
	glm::mat4 projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, &projection[0][0]);


	while (!display_ptr->WindowShouldClose())
	{
		display_ptr->Clear(0, 0, .2, 1);
		program.Bind();
		display();
		display_ptr->SwapBuffers();
		glfwPollEvents();

	}

}

void display() 
{
	
	glClear(GL_COLOR_BUFFER_BIT);

	/* bind and draw */
	glBindVertexArray(VAO);

	/* sizeof(indices) / sizeof(GLushort) will return number of elements in the array */
	glDrawElementsInstanced(GL_TRIANGLES, sizeof(indices) / sizeof(GLushort), GL_UNSIGNED_SHORT, NULL, 3);

	glFlush();

}