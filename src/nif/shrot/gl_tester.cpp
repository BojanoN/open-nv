#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "camera.h"

#include <iostream>
#include <cmath>

#include "NiMain.hpp"
#include "factory.hpp"

#include <cstdio>
#include <cstdint>
#include <vector>
#include <cstring>
#include <string>
#include <cerrno>
#include <cassert>

unsigned int objectVAO;
float* objectVertices;
unsigned int* indices;
unsigned int verNum;
unsigned int numElements;
unsigned int blocksOffset;

float modelMat[16];

NiObject* loadObject(FILE* file, const char* type) {
	auto factoryIt = NiFunctionMaps::creatorMap.find(type);
	if(factoryIt == NiFunctionMaps::creatorMap.end()) {
			std::printf("Cannot find factory function for: %s\n", type);
			return NULL;
	}

	NiObject* (*factory)(FILE*) = factoryIt->second;
	return factory(file);
}

NiObject* loadObjectFromOffset(FILE* file, const char* type, unsigned int index, const Header& header) {
	unsigned int oldLoc = std::ftell(file) - blocksOffset;

	unsigned int offset = 0;
	for(unsigned int i = 0; i < index; i++) {
		offset += header.blockSize[i];
	}
	std::fseek(file, offset + blocksOffset, SEEK_SET);

	auto factoryIt = NiFunctionMaps::creatorMap.find(type);
	if(factoryIt == NiFunctionMaps::creatorMap.end()) {
			std::printf("Cannot find factory function for: %s\n", type);
			return NULL;
	}

	NiObject* (*factory)(FILE*) = factoryIt->second;

	NiObject* ret = factory(file);
	std::fseek(file, oldLoc + blocksOffset, SEEK_SET);
	return ret;
}

void processNodeGraph(FILE* file, NiNode* root, const Header& header);


void processTriStrips(FILE* file, NiTriStrips* triStrips, const Header& header) {
	std::printf("NiTriStrips: %s\n", header.strings[triStrips->name.index].c_str());
	for(unsigned int i = 0; i < triStrips->numExtraDataList; i++) {
		std::cout << "Extra data node: " << triStrips->extraDataList[i] << std::endl;

		const char* extraDataType = header.blockTypes[header.blockTypeIndex[triStrips->extraDataList[i]]].c_str();
		std::printf("%s\n", extraDataType);

		NiExtraData* extraData = dynamic_cast<NiExtraData*>(loadObjectFromOffset(file, extraDataType, triStrips->extraDataList[i], header));

		std::printf("NiExtraData: %s\n", header.strings[extraData->name.index].c_str());
	}

	NiTriStripsData* data = dynamic_cast<NiTriStripsData*>(loadObjectFromOffset(file, "NiTriStripsData", triStrips->data, header));

	unsigned int size = data->numVertices * 3;
	if(data->hasNormals) {
		size += data->numVertices * 3;
	}
	if(data->hasVertexColors) {
		//size += data->numVertices * 4;
	}
	objectVertices = new float[size];

	for(unsigned int i = 0; i < data->numVertices; i++) {
		objectVertices[6 * i] = data->vertices[i].x;
		objectVertices[6 * i + 1] = data->vertices[i].y;
		objectVertices[6 * i + 2] = data->vertices[i].z;
		objectVertices[6 * i + 5] = data->normals[i].z;
		objectVertices[6 * i + 3] = data->normals[i].x;
		objectVertices[6 * i + 4] = data->normals[i].y;
		/*objectVertices[10 * i + 6] = data->vertexColors[i].r;
		objectVertices[10 * i + 7] = data->vertexColors[i].g;
		objectVertices[10 * i + 8] = data->vertexColors[i].b;
		objectVertices[10 * i + 9] = data->vertexColors[i].a;*/
	}
	verNum = data->numVertices;

	unsigned int numPoints = 0;
	for(unsigned int i = 0; i < data->numStrips; i++) {
		numPoints += data->stripLengths[i];
	}

	std::cout << numPoints;
	numPoints = 176;
	indices = new unsigned int[numPoints];
	for(unsigned int i = 0; i < numPoints; i++) {
		indices[i] = data->points[i];
	}

	numElements = numPoints;

	modelMat[0] = triStrips->rotation.m11;
	modelMat[1] = triStrips->rotation.m21;
	modelMat[2] = triStrips->rotation.m31;

	modelMat[4] = triStrips->rotation.m12;
	modelMat[5] = triStrips->rotation.m22;
	modelMat[6] = triStrips->rotation.m32;

	modelMat[8] = triStrips->rotation.m13;
	modelMat[9] = triStrips->rotation.m23;
	modelMat[10] = triStrips->rotation.m33;

	modelMat[12] = triStrips->translation.x;
	modelMat[13] = triStrips->translation.y;
	modelMat[14] = triStrips->translation.z;
	modelMat[15] = 1;
}

void processAVObject(FILE* file, NiAVObject* avObject, const Header& header, unsigned int index) {
	if(strcmp(header.blockTypes[header.blockTypeIndex[index]].c_str(), "NiNode") == 0) {
		NiNode* node = dynamic_cast<NiNode*>(avObject);
		if(node) {
			processNodeGraph(file, node, header);
		}
	} else if(strcmp(header.blockTypes[header.blockTypeIndex[index]].c_str(), "NiTriStrips") == 0) {
		NiTriStrips* triStrips = dynamic_cast<NiTriStrips*>(avObject);
		if(triStrips) {
			processTriStrips(file, triStrips, header);
		}
	} 
}

void processNodeGraph(FILE* file, NiNode* node, const Header& header) {
	std::printf("NiNode: %s\n", header.strings[node->name.index].c_str());
	for(unsigned int i = 0; i < node->numExtraDataList; i++) {
		std::cout << "Extra data node: " << node->extraDataList[i] << std::endl;

		if(node->extraDataList[i] == 7) {
			std::cout << "aa";
		}

		const char* extraDataType = header.blockTypes[header.blockTypeIndex[node->extraDataList[i]]].c_str();
		std::printf("%s\n", extraDataType);

		NiExtraData* extraData = dynamic_cast<NiExtraData*>(loadObjectFromOffset(file, extraDataType, node->extraDataList[i], header));

		std::printf("NiExtraData: %s\n", header.strings[extraData->name.index].c_str());
	}
	if(node->controller != -1) {
		std::printf("Has controllers!\n");
	}
	if(node->numProperties != 0) {
		std::printf("Has properties!\n");
	}

	if(node->collisionObject != -1) {
		std::printf("Has collision object!\n");
	}

	for(unsigned int i = 0; i < node->numChildren; i++) {
		NiAVObject* child = dynamic_cast<NiAVObject*>(loadObjectFromOffset(file, header.blockTypes[header.blockTypeIndex[node->children[i]]].c_str(), node->children[i], header));
		processAVObject(file, child, header, node->children[i]);
	}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height);  
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);


float lastX = 400, lastY = 300;
//float yaw = -90.f;
//float pitch = 0.f;

Camera* currentCamera;

int main(int argc, char* argv[]) {

	char* errBuffer;
	if(!glfwInit()) {
		glfwGetError((const char**)&errBuffer);
		std::cout << "Failed to initialize GLFW: " << errBuffer << std::endl;
	}
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Benis", NULL, NULL);
	if (window == NULL) {
    	std::cout << "Failed to create GLFW window" << std::endl;
    	glfwTerminate();
    	return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  
	glfwSetCursorPosCallback(window, mouse_callback); 
	glfwSetScrollCallback(window, scroll_callback); 

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    	std::cout << "Failed to initialize GLAD" << std::endl;
    	return -1;
	}  

	float vertices[] = {
    	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
    	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
    	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
    	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
    	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 

    	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

    	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

     	0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     	0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     	0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     	0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     	0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     	0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
    	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
    	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

    	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
    	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
    	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};

	globalStringBuffer = new char[4096];
	std::FILE* file = std::fopen("E:\\Devel\\nifs\\aobeerbottle.nif", "r");
	if(file == NULL) {
		std::cout << std::strerror(errno) << std::endl;
	}
	Header head;
	head.load(file);
	head.print();
	blocksOffset = std::ftell(file);

	const char* type = head.blockTypes[head.blockTypeIndex[0]].c_str();
	assert(strcmp(type, "NiNode") == 0);
	NiNode* node = dynamic_cast<NiNode*>(loadObject(file, type));

	processNodeGraph(file, node, head);

	unsigned int lightVao;

	glGenVertexArrays(1, &objectVAO);
	glBindVertexArray(objectVAO);
	unsigned int VBO, EBO;
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, verNum * sizeof(float), objectVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*) (3*sizeof(float)));
	glEnableVertexAttribArray(1);
	//glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 10*sizeof(float), (void*) (6*sizeof(float)));
	//glEnableVertexAttribArray(2);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numElements * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &lightVao);
	glBindVertexArray(lightVao);
	unsigned int lightVbo;
	glGenBuffers(1, &lightVbo);

	glBindBuffer(GL_ARRAY_BUFFER, lightVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) * sizeof(float), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glViewport(0, 0, 800, 600);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	Camera camera(glm::vec3(0.f, 0.f, 3.f));
	currentCamera = &camera;
	glm::vec3 lightPosition(1.2f, 1.0f, 2.0f);


	Shader shader("./shaders/vertex.glsl", "./shaders/fragment.glsl");
	Shader sourceShader("./shaders/vertex.glsl", "./shaders/source_fragment.glsl");
	shader.use();
	glEnable(GL_DEPTH_TEST);

	shader.setVec3("objectColor", 1.f, 0.5f, 0.31f);
	shader.setVec3("lightColor", 1.f, 1.f, 1.f);
	shader.setVec3("lightPosition", lightPosition.x, lightPosition.y, lightPosition.z);
	float deltaTime = 0.f;
	float lastFrameTime = 0.f;

	//std::cout << verNum;

	/*for(unsigned int i = 0; i < numElements; i++) {
		std::cout << objectVertices[indices[i] * 6] << ' ' << objectVertices[indices[i] * 6 + 1] << ' ' << objectVertices[indices[i] * 6 + 2] << '\n';
	}*/

	while(!glfwWindowShouldClose(window)) {
		processInput(window);

		glClearColor(0.f, 0.f, 0.f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.use();
		glBindVertexArray(objectVAO);
		float time = glfwGetTime();
		float currentFrame = time;
		deltaTime = currentFrame - lastFrameTime;
		lastFrameTime = currentFrame;

		const float cameraSpeed = 2.5f * deltaTime;
    	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        	camera.processKeyboard(FORWARD, deltaTime);
    	}
    	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        	camera.processKeyboard(BACKWARD, deltaTime);
    	}
    	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    		camera.processKeyboard(LEFT, deltaTime);
    	}
    	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    		camera.processKeyboard(RIGHT, deltaTime);
    	}


		glm::mat4 view = camera.getViewMatrix();
		glm::mat4 projection;
		projection = glm::perspective(glm::radians(camera.getZoom()), 4.f/3.f, 0.1f, 100.f);

		shader.setVec3("viewPos", camera.position);

		//std::cout << camera.position.x << ' ' << camera.position.y << ' ' << camera.position.z << '\n';

		unsigned int viewLoc = glGetUniformLocation(shader.id, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		unsigned int projectionLoc = glGetUniformLocation(shader.id, "projection");
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

		//glm::mat4 model = glm::make_mat4(modelMat);
		glm::mat4 model = glm::mat4(1.f);
		unsigned int modelLoc = glGetUniformLocation(shader.id, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		glm::mat4 normal = glm::transpose(glm::inverse(model));
		unsigned int normalLoc = glGetUniformLocation(shader.id, "normalMat");
		glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));

		glDrawElements(GL_TRIANGLE_STRIP, numElements, GL_UNSIGNED_INT, 0);

		sourceShader.use();
		model = glm::mat4(1.f);
		model = glm::translate(model, lightPosition);
		model = glm::scale(model, glm::vec3(0.2f));
		modelLoc = glGetUniformLocation(sourceShader.id, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		viewLoc = glGetUniformLocation(sourceShader.id, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		projectionLoc = glGetUniformLocation(sourceShader.id, "projection");
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
		
		glBindVertexArray(lightVao);
		glDrawArrays(GL_TRIANGLES, 0, 36);

    	glfwSwapBuffers(window);
    	glfwPollEvents();    
	}
	
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}  

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {

	static bool firstMouse = true;
	if(firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xOffset = xpos - lastX;
	float yOffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;
	currentCamera->processMouseMovement(xOffset, yOffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	currentCamera->processMouseScroll(yoffset);
}