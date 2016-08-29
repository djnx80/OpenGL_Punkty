#pragma once
#include <string>
#include <vector>

using namespace std;

class Ekran {
private:
	GLFWwindow* okno;		// uchwyt do mojego okna
	double fps;

public:
	Ekran();
	~Ekran();	// destruktor zamyka okno i czysci wszystko

	int Inicjacja(GLFWwindow*&);	// inicjuje okno i pobiera uchwyt do niego
	GLint WczytajShadery(const char *, const char *);
	GLint WczytajShadery(const char *, const char *, const char *);
	GLuint Ekran::WczytajObjekt(string, vector<GLfloat> &, vector<GLfloat> &);
	GLuint Init_VBO(GLfloat *, int);
	GLuint Init_VAO(GLuint);
	GLuint Init_VAO(GLuint, int, GLuint, int);
	GLuint Init_VAO(GLuint, int, GLuint, int, GLuint, int);
	GLint Znajdz_Zmienna(GLuint, const char *);
	GLuint Wczytaj_Teksture(const char *);

	void Wyswietl_info();
	void LicznikFPS();
	double Czas();

	// funkcje graficzne
	void cls(float, float, float);
	void swapBuffers();
//	void rysuj(GLuint , GLuint , int); // nie jestem pewny czy przez referencje czy nie
	void rysuj(GLuint &, GLuint &, const char*, int);
	
};

