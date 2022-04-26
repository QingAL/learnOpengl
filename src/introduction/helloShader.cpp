#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <cmath>

//Uniform��һ�ִ�CPU�е�Ӧ����GPU�е���ɫ���������ݵķ�ʽ����uniform�Ͷ���������Щ��ͬ��
//���ȣ�uniform��ȫ�ֵ�(Global)��ȫ����ζ��uniform����������ÿ����ɫ����������ж��Ƕ�һ�޶��ģ�
//���������Ա���ɫ�������������ɫ��������׶η��ʡ��ڶ����������uniformֵ���ó�ʲô��uniform��һֱ�������ǵ����ݣ�
//ֱ�����Ǳ����û���¡�

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

//����
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char* vertexShaderSource = "#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"void main()\n"
	"{\n"
	"   gl_Position = vec4(aPos, 1.0);\n"
	"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
	"out vec4 FragColor;\n"
	"uniform vec4 ourColor;\n"
	"void main()\n"
	"{\n"
	"   FragColor = ourColor;\n"
	"}\n\0";

int main()
{
	//glfw��ʼ��
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//��������
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "learn shader", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "GLFW���ڴ���ʧ��" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "��ʼ��GLADʧ��" << std::endl;
		return -1;
	}

	//������ɫ��
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
	glCompileShader(vertexShader);

	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
		std::cout << "������ɫ������ʧ��" << std::endl;
		return -1;
	}

	//Ƭ����ɫ��
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
		std::cout << "Ƭ����ɫ������ʧ��" << std::endl;
		return -1;
	}

	//shader����
	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	//shader�������
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
		std::cout << "����ʧ��" << std::endl;
		return -1;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	//���ö�����Ϣ
	float vertices[] = {
		 0.5f, -0.5f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  // bottom left
		 0.0f,  0.5f, 0.0f   // top 
	};

	//����VAO��VBO
	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	//��VAO��VBO
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//glBindVertexArray(VAO);

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);

		//��������ͨ��glfwGetTime()��ȡ���е�������Ȼ������ʹ��sin��������ɫ��0.0��1.0֮��ı䣬��󽫽�����浽greenValue�
		//���ţ�������glGetUniformLocation��ѯuniform ourColor��λ��ֵ��
		//����Ϊ��ѯ�����ṩ��ɫ�������uniform�����֣���������ϣ����õ�λ��ֵ����Դ����
		//���glGetUniformLocation���� - 1�ʹ���û���ҵ����λ��ֵ��������ǿ���ͨ��glUniform4f��������uniformֵ��
		//ע�⣬��ѯuniform��ַ��Ҫ����֮ǰʹ�ù���ɫ�����򣬵��Ǹ���һ��uniform֮ǰ�������ʹ�ó��򣨵���glUseProgram)��
		//��Ϊ�����ڵ�ǰ�������ɫ������������uniform�ġ�

		double timeValue = glfwGetTime();
		//float greenValue = static_cast<float>(sin(timeValue) / 2.0f + 0.5f);
		float greenValue = static_cast<float>(sin(timeValue));
		int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
		glUniform4f(vertexColorLocation, greenValue, 0.0f, 0.0f, 1.0f);

		glDrawArrays(GL_TRIANGLES, 0, 3);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);

	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}