#pragma once

#include <GL/glew.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

namespace Lab6
{
	class opengl_utils
	{

	public:
		opengl_utils()
		{}

		~opengl_utils()
		{}

		// Shader attribute locations
		GLuint loc1;
		GLuint loc2;
		GLuint loc3;
		GLuint loc4;
		GLuint loc5;
		GLuint loc6;
		GLuint loc7;
		GLuint loc8;
		GLuint loc9;
		GLuint loc10;

		GLuint loc11;
		GLuint loc12;

		GLuint loc13;

		// Shaders
		Shader CubeMapID;
		Shader ShaderWithTextureID;
		Shader lightingID;
		
		// Buffers
		GLuint VBO;
		GLuint IBO;
		GLuint TBO;
		GLuint BTBO;

		// Uniform locations
		GLint model_mat_location;
		GLint view_mat_location;
		GLint proj_mat_location;
		GLint objectColorLoc;
		GLint lightColorLoc;
		GLint lightPosLoc;
		GLint viewPosLoc;

		// Uniform - cubemap
		GLint view_mat_location2;
		GLint proj_mat_location2;
		GLuint cubeLocation2;

		// Reflection
		GLint model_mat_location3;
		GLint view_mat_location3;
		GLint proj_mat_location3;
		GLint model_view_matrix3_3;
		GLint objectColorLoc3;
		GLint useSolidColorUniform3;
		GLint useNormalMapUniform3;
		GLint useSpecularMapUniform3;
		GLint camera_position3;
		GLint lightPosLoc3;
		GLint lightColorLoc3;
		GLint viewPosLoc3;
		GLint cubeLocation3;
		GLint texture3;
		GLint normalMap3;
		GLint specularMap3;

		// Refraction
		GLint model_mat_location4;
		GLint view_mat_location4;
		GLint proj_mat_location4;
		GLint objectColorLoc4;
		GLint camera_position4;
		GLint lightPosLoc4;
		GLint viewPosLoc4;
		GLint cubeLocation4;

		// Uniform - light box
		GLint view_mat_location5;
		GLint proj_mat_location5;
		GLint model_mat_location5;

		unsigned int lightVAO;

		static GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path) {

			// Create the shaders
			GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
			GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

			// Read the Vertex Shader code from the file
			std::string VertexShaderCode;
			std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
			if (VertexShaderStream.is_open()) {
				std::stringstream sstr;
				sstr << VertexShaderStream.rdbuf();
				VertexShaderCode = sstr.str();
				VertexShaderStream.close();
			}
			else {
				printf("impossible to open %s. are you in the right directory ? don't forget to read the faq !\n", vertex_file_path);
				getchar();
				return 0;
			}

			// Read the Fragment Shader code from the file
			std::string FragmentShaderCode;
			std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
			if (FragmentShaderStream.is_open()) {
				std::stringstream sstr;
				sstr << FragmentShaderStream.rdbuf();
				FragmentShaderCode = sstr.str();
				FragmentShaderStream.close();
			}

			GLint Result = GL_FALSE;
			int InfoLogLength;

			// Compile Vertex Shader
			printf("Compiling shader : %s\n", vertex_file_path);
			char const * VertexSourcePointer = VertexShaderCode.c_str();
			glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
			glCompileShader(VertexShaderID);

			// Check Vertex Shader
			glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
			glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
			if (InfoLogLength > 0) {
				std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
				glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
				printf("%s\n", &VertexShaderErrorMessage[0]);
			}

			// Compile Fragment Shader
			printf("Compiling shader : %s\n", fragment_file_path);
			char const * FragmentSourcePointer = FragmentShaderCode.c_str();
			glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
			glCompileShader(FragmentShaderID);

			// Check Fragment Shader
			glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
			glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
			if (InfoLogLength > 0) {
				std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
				glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
				printf("%s\n", &FragmentShaderErrorMessage[0]);
			}

			// Link the program
			printf("Linking program\n");
			GLuint ProgramID = glCreateProgram();
			glAttachShader(ProgramID, VertexShaderID);
			glAttachShader(ProgramID, FragmentShaderID);
			glLinkProgram(ProgramID);
			printf("Finished linking\n");


			// Check the program
			glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
			glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
			if (InfoLogLength > 0) {
				std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
				glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
				printf("%s\n", &ProgramErrorMessage[0]);
			}

			glDetachShader(ProgramID, VertexShaderID);
			glDetachShader(ProgramID, FragmentShaderID);

			glDeleteShader(VertexShaderID);
			glDeleteShader(FragmentShaderID);

			return ProgramID;
		}

		void createShaders()
		{
			CubeMapID = Shader("../Lab6/shaders/cubeMap.vs", "../Lab6/shaders/cubeMap.fs");
			ShaderWithTextureID = Shader("../Lab6/shaders/BlinnPhongMultipleLights.vs", "../Lab6/shaders/BlinnPhongMultipleLights.fs"); 
			lightingID = Shader("../Lab6/shaders/lighting.vs", "../Lab6/shaders/lighting.fs");			
		}

		void setupLightBox()
		{
			glGenVertexArrays(1, &lightVAO);
			glBindVertexArray(lightVAO);
			// we only need to bind to the VBO, the container's VBO's data already contains the correct data.
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			// set the vertex attributes (only position data for our lamp)
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);
		}

		void createVBO(int numVertices)
		{
			// Create VBO
			glGenBuffers(1, &VBO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, numVertices * 8 * sizeof(float), NULL, GL_STATIC_DRAW);  // Vertex contains 8 floats: position (vec3), normal (vec3), texture (vec2)
		}

		void createIBO(int numVertices)
		{
			// Create IBO
			glGenBuffers(1, &IBO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, numVertices * sizeof(unsigned int), NULL, GL_STATIC_DRAW);
		}

		void createTBO(int numVertices)
		{
			// Create IBO
			glGenBuffers(1, &TBO);
			glBindBuffer(GL_ARRAY_BUFFER, TBO);
			glBufferData(GL_ARRAY_BUFFER, numVertices * 3 * sizeof(float), NULL, GL_STATIC_DRAW);
		}
		
		void createBTBO(int numVertices)
		{
			// Create IBO
			glGenBuffers(1, &BTBO);
			glBindBuffer(GL_ARRAY_BUFFER, BTBO);
			glBufferData(GL_ARRAY_BUFFER, numVertices * 3 * sizeof(float), NULL, GL_STATIC_DRAW);
		}

		void addVBOBufferSubData(int startIndex, int meshSize, float *meshAddress)
		{
			glBufferSubData(GL_ARRAY_BUFFER, startIndex * 8 * sizeof(GLfloat), meshSize * 8 * sizeof(GLfloat), meshAddress);
		}

		void addIBOBufferSubData(int startIndex, int meshSize, const GLuint *meshAddress)
		{
			glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, startIndex * sizeof(GLuint), sizeof(GLuint) * meshSize, meshAddress);
		}

		void addTBOBufferSubData(int startIndex, int meshSize, float *meshAddress)
		{
			glBufferSubData(GL_ARRAY_BUFFER, startIndex * 3 * sizeof(GLfloat), meshSize * 3 * sizeof(GLfloat), meshAddress);
		}

		void addBTBOBufferSubData(int startIndex, int meshSize, float *meshAddress)
		{
			glBufferSubData(GL_ARRAY_BUFFER, startIndex * 3 * sizeof(GLfloat), meshSize * 3 * sizeof(GLfloat), meshAddress);
		}

		void generateVertexArray(GLuint *array)
		{
			glGenVertexArrays(1, array);
		}

		void setupUniformVariables()
		{	
			view_mat_location2 = glGetUniformLocation(CubeMapID, "view");
			proj_mat_location2 = glGetUniformLocation(CubeMapID, "projection");
			cubeLocation2 = glGetUniformLocation(CubeMapID, "skybox");

			model_mat_location3 = glGetUniformLocation(ShaderWithTextureID, "model");
			view_mat_location3 = glGetUniformLocation(ShaderWithTextureID, "view");
			proj_mat_location3 = glGetUniformLocation(ShaderWithTextureID, "projection");
			model_view_matrix3_3 = glGetUniformLocation(ShaderWithTextureID, "modelView3x3");
			objectColorLoc3 = glGetUniformLocation(ShaderWithTextureID, "objectColor");
			useSolidColorUniform3 = glGetUniformLocation(ShaderWithTextureID, "useSolidColor");
			useNormalMapUniform3 = glGetUniformLocation(ShaderWithTextureID, "useNormalMap");
			useSpecularMapUniform3 = glGetUniformLocation(ShaderWithTextureID, "useSpecularMap");
			camera_position3 = glGetUniformLocation(ShaderWithTextureID, "cameraPos");

			lightPosLoc3 = glGetUniformLocation(ShaderWithTextureID, "lightPos");
			lightColorLoc3 = glGetUniformLocation(ShaderWithTextureID, "lightColor");
			viewPosLoc3 = glGetUniformLocation(ShaderWithTextureID, "viewPos");

			cubeLocation3 = glGetUniformLocation(ShaderWithTextureID, "skybox");
			texture3 = glGetUniformLocation(ShaderWithTextureID, "diffuseTexture");
			normalMap3 = glGetUniformLocation(ShaderWithTextureID, "normalTexture");
			specularMap3 = glGetUniformLocation(ShaderWithTextureID, "specularTexture");
			
			model_mat_location5 = glGetUniformLocation(lightingID, "model");
			view_mat_location5 = glGetUniformLocation(lightingID, "view");
			proj_mat_location5 = glGetUniformLocation(lightingID, "projection");
		}

		void getAttributeLocations()
		{			
			loc4 = glGetAttribLocation(CubeMapID, "aPos");

			loc5 = glGetAttribLocation(ShaderWithTextureID, "position");
			loc6 = glGetAttribLocation(ShaderWithTextureID, "normal");
			loc7 = glGetAttribLocation(ShaderWithTextureID, "texture");
			loc11 = glGetAttribLocation(ShaderWithTextureID, "tangent");
			loc12 = glGetAttribLocation(ShaderWithTextureID, "bitangent");

			loc13 = glGetAttribLocation(lightingID, "aPos");
		}

		void bindVertexAttribute(int location, int locationSize, int startVBO, int offsetVBO)
		{
			glEnableVertexAttribArray(location);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glVertexAttribPointer(location, locationSize, GL_FLOAT, GL_TRUE, 8 * sizeof(float), (void*)(startVBO * 8 * sizeof(float) + BUFFER_OFFSET(offsetVBO * sizeof(GLfloat))));
		}

		void linkCurrentBuffertoShaderWithTexture(GLuint VAOindex, int startVBO, int startIBO)
		{
			glBindVertexArray(VAOindex);

			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			bindVertexAttribute(loc5, 3, startVBO, 0);
			bindVertexAttribute(loc6, 3, startVBO, 3);
			bindVertexAttribute(loc7, 2, startVBO, 6);

			glBindBuffer(GL_ARRAY_BUFFER, TBO);
			bindVertexAttribute(loc11, 3, startVBO, 0);
			
			glBindBuffer(GL_ARRAY_BUFFER, BTBO);
			bindVertexAttribute(loc12, 3, startVBO, 0);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		}

		void linkCurrentBuffertoShaderLighting(GLuint VAOindex, int startVBO, int startIBO)
		{
			glBindVertexArray(VAOindex);

			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			bindVertexAttribute(loc13, 3, startVBO, 0);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		}

		void updateUniformVariablesCubeMap(glm::mat4 view, glm::mat4 persp_proj)
		{
			glUniformMatrix4fv(proj_mat_location2, 1, GL_FALSE, &persp_proj[0][0]);
			glUniformMatrix4fv(view_mat_location2, 1, GL_FALSE, &view[0][0]);
		}

		void updateUniformVariablesLighting(glm::mat4 model)
		{
			glUniformMatrix4fv(model_mat_location5, 1, GL_FALSE, &model[0][0]);
		}

		void updateUniformVariablesLighting(glm::mat4 model, glm::mat4 view, glm::mat4 persp_proj)
		{
			glUniformMatrix4fv(proj_mat_location5, 1, GL_FALSE, &persp_proj[0][0]);
			glUniformMatrix4fv(view_mat_location5, 1, GL_FALSE, &view[0][0]);
			updateUniformVariablesLighting(model);
		}

		void updateUniformVariablesReflectance(glm::mat4 model, glm::mat4 view)
		{
			glUniformMatrix4fv(model_mat_location3, 1, GL_FALSE, &model[0][0]);
			glm::mat3 viewModel = glm::mat3 (view * model);
			glUniformMatrix3fv(model_view_matrix3_3, 1, GL_FALSE, &viewModel[0][0]);
		}

		void updateUniformVariablesReflectance(glm::mat4 model, glm::mat4 view, glm::mat4 persp_proj)
		{
			glUniformMatrix4fv(proj_mat_location3, 1, GL_FALSE, &persp_proj[0][0]);
			glUniformMatrix4fv(view_mat_location3, 1, GL_FALSE, &view[0][0]);
			
			updateUniformVariablesReflectance(model, view);
		}

		void deleteVertexArrays()
		{
			//glDeleteVertexArrays(1, &footballbVAO);
			//glDeleteVertexArrays(1, &footballwVAO);	
		}

		void deleteBuffers()
		{
			glDeleteBuffers(1, &VBO);
			glDeleteBuffers(1, &IBO);
		}

		void deletePrograms()
		{
			glDeleteProgram(PhongProgramID);
			glDeleteProgram(CubeMapID);
			glDeleteProgram(ShaderWithTextureID);
			glDeleteProgram(RefractionID);
		}

	};
}