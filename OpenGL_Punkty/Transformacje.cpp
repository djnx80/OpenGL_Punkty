#include "stdafx.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h> 
#include <glm/vec3.hpp> 
#include <glm/vec4.hpp> 
#include <glm/mat4x4.hpp> 
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/glm.hpp>

#include "Transformacje.h"
using namespace glm;

void Transformacje::funkcja_Po_Okregu(float t_promien, float t_angle, float &dx, float &dy) {
	dx = t_promien * cos(t_angle * 3.14 / 180.0);
	dy = t_promien * sin(t_angle * 3.14 / 180.0);
}

void Transformacje::funkcja_Jakas_tam(float t_promien, float t_angle, float &dx, float &dy) {
	dx = t_promien * cos(t_angle * 3.14 / 180.0) * sin(t_angle/2 * 3.14 / 160.0);
	dy = t_promien * sin(t_angle * 3.14 / 280.0) + cos(t_angle * 3.14 / 180.0);
}

mat4 Transformacje::przesun(float dx, float dy, float dz)
{
	return translate(mat4(1.0), vec3(dx, dy, dz));
}

mat4 Transformacje::obroc(mat4 przesuniecie, float angle, bool oX, bool oY, bool oZ)	{
	return rotate(przesuniecie, angle, vec3(oX, oY, oZ));
}

void Transformacje::zerujKamere() {
	float verticalAngle = 0.0f;
	float horizontalAngle = 3.14f;

	kamera_przod = vec3(0.0, 0.0, -1.0);
	kamera_prawo = vec3(1.0, 0.0, 0.0);
	pozycja_kamery = vec3(0.0, 0.0, 4.0);
	//	pozycja_kamery = vec3(0.0, 0.0, 6.0);
	//	kamera_przod = vec3(cos(verticalAngle) * sin(horizontalAngle), sin(verticalAngle), cos(verticalAngle) * cos(horizontalAngle));
	//	kamera_prawo = vec3(sin(horizontalAngle - 3.14f / 2.0f), 0, cos(horizontalAngle - 3.14f / 2.0f));

	skladowa_kamerXYZ = cross(kamera_prawo, kamera_przod);
}
void Transformacje::ustawKamere() {
	float verticalAngle = 0.0f;
	float horizontalAngle = 3.14f;

	kamera_przod = vec3(0.0, 0.0, -1.0);
	kamera_prawo = vec3(1.0, 0.0, 0.0);
	pozycja_kamery = vec3(0.0, 0.0, 4.0);
//	pozycja_kamery = vec3(0.0, 0.0, 6.0);
//	kamera_przod = vec3(cos(verticalAngle) * sin(horizontalAngle), sin(verticalAngle), cos(verticalAngle) * cos(horizontalAngle));
//	kamera_prawo = vec3(sin(horizontalAngle - 3.14f / 2.0f), 0, cos(horizontalAngle - 3.14f / 2.0f));

	skladowa_kamerXYZ = cross(kamera_prawo, kamera_przod);
}

double Czas() {
	static double poprzedni_czas = glfwGetTime();
	double aktualny_czas = glfwGetTime();
	double ile_minelo = aktualny_czas - poprzedni_czas;
	poprzedni_czas = aktualny_czas;
	return ile_minelo;
}

void Transformacje::KamerawLewo(float szybkosc) {	pozycja_kamery -= kamera_prawo * (float)Czas() * szybkosc;	}
void Transformacje::KamerawPrawo(float szybkosc) {	pozycja_kamery += kamera_prawo * (float)Czas() * szybkosc;	}
void Transformacje::KamerawPrzod(float szybkosc) { pozycja_kamery += kamera_przod * (float)Czas() * szybkosc; }
void Transformacje::KamerawTyl(float szybkosc) { pozycja_kamery -= kamera_przod * (float)Czas() * szybkosc; }

void Transformacje::kamera(mat4 &perspektywa, mat4 &kamera) {

	float P1 = 0.1f;
	float P2 = 100.0f;
	float FOV = 45.0f;	// jest odpowiedzialne za zblizanie i oddalanie
	float aspect = 4.0f / 3.0f;
	skladowa_kamerXYZ = cross(kamera_prawo, kamera_przod);
	perspektywa = perspective(FOV, aspect, P1, P2);
	kamera =  lookAt(pozycja_kamery, pozycja_kamery + kamera_przod, skladowa_kamerXYZ);
}
