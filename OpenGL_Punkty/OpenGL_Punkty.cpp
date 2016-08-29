#include "stdafx.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h> 
#include <iostream> 
#include <string>
//#include <cstdlib>
#include <ctime>
//#include <SOIL.h>
#include "Ekran.h"
#include "Transformacje.h"

using namespace std;
using namespace glm;

// zmienne globalne
GLFWwindow *mojeOkno = NULL;
const int ile_punktow = 10000;
GLfloat punkty[ile_punktow*3];
GLfloat kolor[ile_punktow*3];
float dXYZ[ile_punktow * 3];

int main(void) {


	srand(time(NULL));

	for (int i = 0; i < ile_punktow * 3; i = i + 3) {
		dXYZ[i] = ((rand() % 2000) - 1000) / 80000.0f;
		dXYZ[i+1] = ((rand() % 2000) - 1000) / 80000.0f;
		dXYZ[i + 2] = ((rand() % 2000) / 40000.0f) + 0.01;
		punkty[i] = 0.0f;
		punkty[i + 1] = 0.0f;
		punkty[i + 2] = 0.0f;
		kolor[i] = ((rand() % 200)) / 200.0f;
		kolor[i + 1] = ((rand() % 200)) / 200.0f;
		kolor[i + 2] = ((rand() % 200)) / 200.0f;
//		cout << punkty[i] << " " << punkty[i + 1] << endl;
	}
		
	Ekran ekran;
	Transformacje trans;
	ekran.Inicjacja(mojeOkno);
	
	GLuint vbo = ekran.Init_VBO(punkty, sizeof(punkty));
	GLuint vbo_kolor = ekran.Init_VBO(kolor, sizeof(kolor));
	GLuint vbo_xyz = ekran.Init_VBO(dXYZ, sizeof(dXYZ));
	GLuint vao = ekran.Init_VAO(vbo, 3, vbo_kolor, 3, vbo_xyz, 3);

	GLuint shaders = ekran.WczytajShadery("vertex_shader.glsl", "fragment_shader.glsl");
	GLint transformacja = ekran.Znajdz_Zmienna(shaders, "macierz_przesuniecia");
	GLint viewm = ekran.Znajdz_Zmienna(shaders, "view_matrix");
	GLint perspm = ekran.Znajdz_Zmienna(shaders, "perspective_matrix");
	trans.ustawKamere();

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	mat4 wynik_trans(1.0);
	mat4 perspektywa(1.0);
	mat4 kamera(1.0);

	float dz = 0, dx = 0, dy = 0;
	float move_speed = 1.0f;

	float angle = 0;
	int ruch_kamery=0;
	bool status_kamery = false; // kamera do ty³u
	int d_kamery = 1;

	while (!glfwWindowShouldClose(mojeOkno))
	{
		for (int i = 0; i < ile_punktow * 3; i = i + 3) {
			punkty[i] += dXYZ[i];
			punkty[i + 1] += dXYZ[i + 1];
			punkty[i + 2] += dXYZ[i+2];
			if (punkty[i + 2] > 3.5f) {
				punkty[i] = 0.0f;
				punkty[i + 1] = 0.0f;
				punkty[i + 2] = 0.0f;
			}
		}

		vbo = ekran.Init_VBO(punkty, sizeof(punkty));
		vbo_kolor = ekran.Init_VBO(kolor, sizeof(kolor));
		vbo_xyz = ekran.Init_VBO(dXYZ, sizeof(dXYZ));
		vao = ekran.Init_VAO(vbo, 3, vbo_kolor, 3, vbo_xyz, 3);

		if (glfwGetKey(mojeOkno, GLFW_KEY_LEFT) == GLFW_PRESS) { trans.KamerawLewo(move_speed); }
		if (glfwGetKey(mojeOkno, GLFW_KEY_RIGHT) == GLFW_PRESS) { trans.KamerawPrawo(move_speed); }
		if (glfwGetKey(mojeOkno, GLFW_KEY_DOWN) == GLFW_PRESS) { trans.KamerawTyl(move_speed); }
		if (glfwGetKey(mojeOkno, GLFW_KEY_UP) == GLFW_PRESS) { trans.KamerawPrzod(move_speed); }

		if (glfwGetKey(mojeOkno, GLFW_KEY_A) == GLFW_PRESS) { dx = dx - 0.05f; }
		if (glfwGetKey(mojeOkno, GLFW_KEY_D) == GLFW_PRESS) { dx = dx + 0.05f; }
		if (glfwGetKey(mojeOkno, GLFW_KEY_W) == GLFW_PRESS) { dy = dy + 0.05f; }
		if (glfwGetKey(mojeOkno, GLFW_KEY_S) == GLFW_PRESS) { dy = dy - 0.05f; }
		if (glfwGetKey(mojeOkno, GLFW_KEY_Q) == GLFW_PRESS) { dx = 0;  dy = 0; }
		if (glfwGetKey(mojeOkno, GLFW_KEY_E) == GLFW_PRESS) { trans.zerujKamere(); }

		ruch_kamery+=d_kamery;
		if (ruch_kamery > 200 && status_kamery==false) {
			status_kamery = true;
			d_kamery = -1;
			move_speed = -move_speed; }
		if (ruch_kamery < -200 && status_kamery == true) {
			status_kamery = false;
			d_kamery = 1;
			move_speed = -move_speed;
		}

		trans.KamerawTyl(move_speed);
		cout << ruch_kamery << "  " << move_speed << endl;

		angle += 0.02;
		wynik_trans = trans.przesun(dx, dy, dz);
	//	wynik_trans = trans.obroc(wynik_trans, 11.0f, true, false, false);
		wynik_trans = trans.obroc(wynik_trans, angle, false, false, true);
	//	wynik_trans = mat4(1.0);
		trans.kamera(perspektywa, kamera);	// aktualizuje zmienne perspektywy i kamery
		glUniformMatrix4fv(perspm, 1, GL_FALSE, value_ptr(perspektywa));
		glUniformMatrix4fv(viewm, 1, GL_FALSE, value_ptr(kamera));
		glUniformMatrix4fv(transformacja, 1, GL_FALSE, value_ptr(wynik_trans));

		ekran.cls(0.0f, 0.0f, 0.0f);
		ekran.rysuj(shaders, vao, "Punkty", ile_punktow);
		ekran.swapBuffers();
	}
	// destruktor zamyka okno itp.
	return 0;
}
