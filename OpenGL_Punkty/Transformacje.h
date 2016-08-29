#pragma once
#include <glm/vec3.hpp> 
#include <glm/vec4.hpp> 
#include <glm/mat4x4.hpp> 
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/glm.hpp>
// klasa odpowiada za obroty, skalowanie
using namespace glm;

class Transformacje {
private:
	float angle, promien;
	vec3 kamera_przod;
	vec3 kamera_prawo;
	vec3 pozycja_kamery;
	vec3 skladowa_kamerXYZ;
	mat4 view_matrix;

public:

	void funkcja_Po_Okregu(float, float, float &, float &);
	void funkcja_Jakas_tam(float, float, float &, float &);
	mat4 przesun(float, float, float);
	mat4 obroc(mat4, float, bool, bool, bool);
	void ustawKamere();
	void zerujKamere();
	void kamera(mat4 &, mat4 &);
	void KamerawLewo(float);
	void KamerawPrawo(float);
	void KamerawPrzod(float);
	void KamerawTyl(float);
};