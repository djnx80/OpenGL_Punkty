#include "stdafx.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h> 
#include <assimp/scene.h>
#include <assimp/postprocess.h> 
#include <assimp/cimport.h>
#include <vector>
#include <iostream> 
#include <fstream>
#include <string>
#include "Ekran.h"

using namespace std;

int window_height = 600;
int window_width = 800;

void window_size_callback(GLFWwindow* okno, int width, int height) {
	window_width = width;
	window_height = height;
}
static void key_callback(GLFWwindow* okno, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(okno, GL_TRUE);
	}
}
static void error_callback(int error, const char* description) { cerr << "Error: " << description; }

Ekran::Ekran() {	}
Ekran::~Ekran() {
	glfwDestroyWindow(okno);
	glfwTerminate();
}

GLint Ekran::Znajdz_Zmienna(GLuint t_shaders, const char *nazwa_zmiennej){
GLint zmienna = glGetUniformLocation(t_shaders, nazwa_zmiennej);
if (zmienna == -1) {
	cout << "nie znalaziono zmiennej " << nazwa_zmiennej << endl;
}	else {
	cout << "zmienna znaleziona:" << nazwa_zmiennej << endl; 
}
return zmienna;
}
GLuint Ekran::Init_VBO(GLfloat *punkty, int ile_p) {
	GLuint t_vbo;
	glGenBuffers(1, &t_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, t_vbo);
	glBufferData(GL_ARRAY_BUFFER, ile_p, punkty, GL_STATIC_DRAW);
	return t_vbo;
}
GLuint Ekran::Init_VAO(GLuint t_vbo) {
	// VAO dla jednej zmiennej, np wspó³rzêdnych
	GLuint t_vao;
	glGenVertexArrays(1, &t_vao);
	glBindVertexArray(t_vao);
	glBindBuffer(GL_ARRAY_BUFFER, t_vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);
	return t_vao;
}
GLuint Ekran::Init_VAO(GLuint t_vbo1, int ile_pkt1, GLuint t_vbo2, int ile_pkt2 ) {
	// VAO dla dwóch zmiennych, wspó³rzêdnych i kolorów
	GLuint t_vao;
	glGenVertexArrays(1, &t_vao);
	glBindVertexArray(t_vao);
	glBindBuffer(GL_ARRAY_BUFFER, t_vbo1);
	glVertexAttribPointer(0, ile_pkt1, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, t_vbo2);
	glVertexAttribPointer(1, ile_pkt2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	return t_vao;
}
GLuint Ekran::Init_VAO(GLuint t_vbo1, int ile_pkt1, GLuint t_vbo2, int ile_pkt2, GLuint t_vbo3, int ile_pkt3) {
	// VAO dla dwóch zmiennych, wspó³rzêdnych i kolorów
	GLuint t_vao;
	glGenVertexArrays(1, &t_vao);
	glBindVertexArray(t_vao);
	glBindBuffer(GL_ARRAY_BUFFER, t_vbo1);
	glVertexAttribPointer(0, ile_pkt1, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, t_vbo2);
	glVertexAttribPointer(1, ile_pkt2, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, t_vbo3);
	glVertexAttribPointer(2, ile_pkt3, GL_FLOAT, GL_FALSE, 0, NULL);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	return t_vao;
}

void Ekran::cls(float r, float g, float b) {
	// czysci ekran na podany kolor
	glClearColor(r,g,b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, window_width, window_height);
}
void Ekran::swapBuffers() {
	glfwSwapBuffers(okno);
	glfwPollEvents();
}
void Ekran::rysuj(GLuint &shaders, GLuint &vao1, const char *tryb, int ile_wierzcholkow) {
	glUseProgram(shaders);
	glBindVertexArray(vao1);
	glPointSize(2.0);	// grubosc punktu
	if (tryb == "Trojkaty") { glDrawArrays(GL_TRIANGLES, 0, 3); }
	if (tryb == "Linie") { glDrawArrays(GL_LINE_LOOP, 0, 3); }
	if (tryb == "Punkty") { glDrawArrays(GL_POINTS, 0, ile_wierzcholkow); }

}
double Ekran::Czas() {
	static double poprzedni_czas = glfwGetTime();
	double aktualny_czas = glfwGetTime();
	double ile_minelo = aktualny_czas - poprzedni_czas;
	poprzedni_czas = aktualny_czas;
	return ile_minelo;
}
int Ekran::Inicjacja(GLFWwindow *&temp_okno) {
	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
		return 1;

	glfwWindowHint(GLFW_SAMPLES, 4);		// wyg³adzanie krawêdzi
	okno = glfwCreateWindow(800, 600, "My GLFW Window", NULL, NULL);
	if (!okno) {
		glfwTerminate();
		return 1;
	}

	glfwMakeContextCurrent(okno);

	GLenum error_code = glewInit();
	if (error_code != GLEW_OK) {
		cerr << "GLEW init error: " << glewGetErrorString(error_code);
	}
	glfwSetWindowSizeCallback(okno, window_size_callback);
	glfwSetKeyCallback(okno, key_callback);
	temp_okno = okno;

	// sprawdzic do czego - jezeli cos nie dziala to moze tu...
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glFrontFace(GL_CCW);

	return 0;
}
void Ekran::Wyswietl_info() {
	const GLubyte* vendor = glGetString(GL_VENDOR);
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* version = glGetString(GL_VERSION);
	const GLubyte* shading = glGetString(GL_SHADING_LANGUAGE_VERSION);
	cout << vendor << endl << renderer << endl << version << endl << shading << endl;
}
void Ekran::LicznikFPS() {
	static double poprzedni_czas = glfwGetTime();
	double aktualny_czas = glfwGetTime();
	static int licznik_klatek = 0;
	double ile_minelo_czasu = aktualny_czas - poprzedni_czas;
	if (ile_minelo_czasu >= 1.0) {
		poprzedni_czas = aktualny_czas;
		fps = static_cast<double>(licznik_klatek) / ile_minelo_czasu;
		licznik_klatek = 0;
	}
	licznik_klatek++;
	string title = "Klatek na sek: " + to_string(fps);
	glfwSetWindowTitle(okno, title.c_str());
}


GLint Ekran::WczytajShadery(const char * vertex_shader, const char * fragment_shader) {

	GLuint vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);

	string vertex_shader_data;
	ifstream vertex_shader_file(vertex_shader, ios::in);
	if (vertex_shader_file.is_open())
	{
		string line;
		while (getline(vertex_shader_file, line))
			vertex_shader_data += "\n" + line;
		vertex_shader_file.close();
	}
	else
	{
		cout << "nie ma pliku vertex" << endl;
	}

	string fragment_shader_data;
	ifstream fragment_shader_file(fragment_shader, ios::in);
	if (fragment_shader_file.is_open())
	{
		string line;
		while (getline(fragment_shader_file, line))
			fragment_shader_data += "\n" + line;
		fragment_shader_file.close();
	}
	else
	{
		cout << "nie ma pliku fragment" << endl;
	}

	const char * vertex_ptr = vertex_shader_data.c_str();
	const char * fragment_ptr = fragment_shader_data.c_str();

	glShaderSource(vertex_shader_id, 1, &vertex_ptr, NULL);
	glShaderSource(fragment_shader_id, 1, &fragment_ptr, NULL);

	glCompileShader(vertex_shader_id);
	glCompileShader(fragment_shader_id);

	int vertex_shader_status = -1; 
	glGetShaderiv(vertex_shader_id, GL_COMPILE_STATUS, &vertex_shader_status); 
	if (vertex_shader_status != GL_TRUE) {
		cout << "Vertex shader ZLE SKOMPILOWANY!!!" << endl; 
		return -1; }
	else
		cout << "Vertex shader zostal skompilowany." << endl;
	int fragment_shader_status = -1;
	glGetShaderiv(fragment_shader_id, GL_COMPILE_STATUS, &fragment_shader_status);
	if (fragment_shader_status != GL_TRUE) {
		cout << "Fragment shader ZLE SKOMPILOWANY!!!" <<endl; 
		return -1; }
	else
		cout << "Fragment shader zostal skompilowany." << endl;


	GLuint shader_programme = glCreateProgram();
	glAttachShader(shader_programme, vertex_shader_id);
	glAttachShader(shader_programme, fragment_shader_id);

	glLinkProgram(shader_programme);
	glUseProgram(shader_programme);

	glDeleteShader(vertex_shader_id);
	glDeleteShader(fragment_shader_id);
	return shader_programme;
}


GLint Ekran::WczytajShadery(const char * vertex_shader, const char * geometry_shader, const char * fragment_shader) {
	
	GLuint vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
	GLuint geometry_shader_id = glCreateShader(GL_GEOMETRY_SHADER);
	GLuint fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);

	string vertex_shader_data;
	ifstream vertex_shader_file(vertex_shader, ios::in);
	if (vertex_shader_file.is_open())
	{
		string line;
		while (getline(vertex_shader_file, line))
			vertex_shader_data += "\n" + line;
		vertex_shader_file.close();
	}
	else
	{
		cout << "nie ma pliku vertex" << endl;
	}

	string geometry_shader_data;
	ifstream geometry_shader_file(geometry_shader, ios::in);
	if (geometry_shader_file.is_open())
	{
		string line;
		while (getline(geometry_shader_file, line))
			geometry_shader_data += "\n" + line;
		geometry_shader_file.close();
	}
	else
	{
		cout << "nie ma pliku vertex" << endl;
	}

	string fragment_shader_data;
	ifstream fragment_shader_file(fragment_shader, ios::in);
	if (fragment_shader_file.is_open())
	{
		string line;
		while (getline(fragment_shader_file, line))
			fragment_shader_data += "\n" + line;
		fragment_shader_file.close();
	}
	else
	{
		cout << "nie ma pliku fragment" << endl;
	}

	const char * vertex_ptr = vertex_shader_data.c_str();
	const char * geometry_ptr = geometry_shader_data.c_str();
	const char * fragment_ptr = fragment_shader_data.c_str();
	
	glShaderSource(vertex_shader_id, 1, &vertex_ptr, NULL);
	glShaderSource(geometry_shader_id, 1, &geometry_ptr, NULL);
	glShaderSource(fragment_shader_id, 1, &fragment_ptr, NULL);

	glCompileShader(vertex_shader_id);
	glCompileShader(geometry_shader_id);
	glCompileShader(fragment_shader_id);

	int vertex_shader_status = -1;
	glGetShaderiv(vertex_shader_id, GL_COMPILE_STATUS, &vertex_shader_status);
	if (vertex_shader_status != GL_TRUE) {
		cout << "Vertex shader ZLE SKOMPILOWANY!!!" << endl;
		return -1;
	}
	else
		cout << "Vertex shader zostal skompilowany." << endl;
	int geometry_shader_status = -1;
	glGetShaderiv(geometry_shader_id, GL_COMPILE_STATUS, &geometry_shader_status);
	if (geometry_shader_status != GL_TRUE) {
		cout << "Geometry shader ZLE SKOMPILOWANY!!!" << endl;
		return -1;
	}
	else
		cout << "Geometry shader zostal skompilowany." << endl;
	int fragment_shader_status = -1;
	glGetShaderiv(fragment_shader_id, GL_COMPILE_STATUS, &fragment_shader_status);
	if (fragment_shader_status != GL_TRUE) {
		cout << "Fragment shader ZLE SKOMPILOWANY!!!" << endl;
		return -1;
	}
	else
		cout << "Fragment shader zostal skompilowany." << endl;


	GLuint shader_programme = glCreateProgram();
	glAttachShader(shader_programme, vertex_shader_id);
	glAttachShader(shader_programme, geometry_shader_id);
	glAttachShader(shader_programme, fragment_shader_id);

	glLinkProgram(shader_programme);
	glUseProgram(shader_programme);

	glDeleteShader(vertex_shader_id);
	glDeleteShader(geometry_shader_id);
	glDeleteShader(fragment_shader_id);
	return shader_programme;
}
GLuint Ekran::Wczytaj_Teksture(const char * imagepath) {

	printf("Odczyt pliku %s\n", imagepath);

	// Data read from the header of the BMP file
	unsigned char header[54];
	unsigned int dataPos;
	unsigned int imageSize;
	unsigned int width, height;
	// Actual RGB data
	unsigned char * data;

	// Open the file
	FILE * file;
	fopen_s(&file, imagepath, "rb");
	if (!file) { printf("%s Nie ma takiego pliku\n", imagepath); return 0; }

	// Read the header, i.e. the 54 first bytes

	// If less than 54 byes are read, problem
	if (fread(header, 1, 54, file) != 54) {
		printf("Nieprawid³owy plik BMP\n");
		return 0;
	}
	// A BMP files always begins with "BM"
	if (header[0] != 'B' || header[1] != 'M') {
		printf("Nieprawid³owy plik BMP\n");
		return 0;
	}
	// Make sure this is a 24bpp file
	if (*(int*)&(header[0x1E]) != 0) { printf("Nieprawid³owy plik BMP\n");    return 0; }
	if (*(int*)&(header[0x1C]) != 24) { printf("Nieprawid³owy plik BMP\n");    return 0; }

	// Read the information about the image
	dataPos = *(int*)&(header[0x0A]);
	imageSize = *(int*)&(header[0x22]);
	width = *(int*)&(header[0x12]);
	height = *(int*)&(header[0x16]);

	cout << "dane: " << width << ", " << height << endl;

	// Some BMP files are misformatted, guess missing information
	if (imageSize == 0)    imageSize = width*height * 3; // 3 : one byte for each Red, Green and Blue component
	if (dataPos == 0)      dataPos = 54; // The BMP header is done that way

										 // Create a buffer
	data = new unsigned char[imageSize];

	// Read the actual data from the file into the buffer
	fread(data, 1, imageSize, file);

	// Everything is in memory now, the file wan be closed
	fclose(file);

	// Create one OpenGL texture
	GLuint textureID;
	glActiveTexture(GL_TEXTURE0); // czy potrzebne?
	glGenTextures(1, &textureID);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Give the image to OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

	// OpenGL has now copied the data. Free our own version
	delete[] data;

	// Poor filtering, or ...
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 

	// ... nice trilinear filtering.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	// Return the ID of the texture we just created
	glGenerateMipmap(GL_TEXTURE_2D);


	return textureID;
}
GLuint Ekran::WczytajObjekt(std::string file_name, vector<GLfloat> &liczba_wierzcholkow, vector<GLfloat> &poczatkowy_wierzcholek) {

	int calkowita_liczba_wierzcholkow = 0;
	GLuint vao;
	float wsp = 0.001;

	// cialo funkcji ... 
	const aiScene* scene = aiImportFile(file_name.c_str(), aiProcess_Triangulate);
	aiMesh* mesh = NULL;
	//		int total_vertices_count = 0;
	std::vector<GLfloat> buffer_vbo_data;


	for (int i = 0; i != scene->mNumMeshes; i++) {
		mesh = scene->mMeshes[i];
		int mesh_vertices = 0;
		for (int j = 0; j != mesh->mNumFaces; j++) {
			const aiFace* face = &mesh->mFaces[j];
			for (int k = 0; k != 3; k++) {
				aiVector3D vertex_position{ 0, 0, 0 };
				aiVector3D vertex_normal{ 0, 0, 0 };
				aiVector3D vertex_texture_coord{ 0, 0, 0 };
				if (mesh->HasPositions())
					vertex_position = mesh->mVertices[face->mIndices[k]];
				if (mesh->HasNormals())
					vertex_normal = mesh->mNormals[face->mIndices[k]];
				if (mesh->HasTextureCoords(0))
					vertex_texture_coord = mesh->mTextureCoords[0][face->mIndices[k]];
				buffer_vbo_data.push_back(vertex_position.x*wsp);
				buffer_vbo_data.push_back(vertex_position.y*wsp);
				buffer_vbo_data.push_back(vertex_position.z*wsp);
				buffer_vbo_data.push_back(vertex_normal.x);
				buffer_vbo_data.push_back(vertex_normal.y);
				buffer_vbo_data.push_back(vertex_normal.z);
				buffer_vbo_data.push_back(vertex_texture_coord.x);
				buffer_vbo_data.push_back(vertex_texture_coord.y);
				//	cout << "x:" << vertex_position.x << " y:" << vertex_position.y << " z:" << vertex_position.z << endl;	
				mesh_vertices++;
			}
		}
		liczba_wierzcholkow.push_back(mesh_vertices);
		poczatkowy_wierzcholek.push_back(calkowita_liczba_wierzcholkow);
		calkowita_liczba_wierzcholkow += mesh_vertices;
		cout << "vert: " << poczatkowy_wierzcholek.size() << "  ver2: " << calkowita_liczba_wierzcholkow << endl;
	}


		GLuint vbo_buffer = 0;
		glGenBuffers(1, &vbo_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_buffer);
		glBufferData(GL_ARRAY_BUFFER, buffer_vbo_data.size() * sizeof(GLfloat), buffer_vbo_data.data(), GL_STATIC_DRAW);
		int single_vertex_size = 2 * 3 * sizeof(GLfloat) + 2 * sizeof(GLfloat);
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, single_vertex_size, 0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, single_vertex_size, reinterpret_cast<void*>(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, single_vertex_size, reinterpret_cast<void*>(2 * 3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(2);
	

	return vao;
}