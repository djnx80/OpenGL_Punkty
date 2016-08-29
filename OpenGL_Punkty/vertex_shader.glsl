#version 450

layout(location = 0) in vec3 pozycja;
layout(location = 1) in vec3 kolor_punktu;
layout(location = 2) in vec3 delta;

uniform mat4 macierz_przesuniecia;
uniform mat4 view_matrix;
uniform mat4 perspective_matrix;

out vec3 kolor;
float wsp;

void main()
{
	wsp = pozycja.x;
	if (wsp < pozycja.y)	{
		wsp = pozycja.y; }
	if (wsp < pozycja.z)	{
		wsp = pozycja.z; }
	wsp = wsp / 1.5f;

	if (wsp<0.05f)	{
		wsp = 0.05f;}
	if (wsp>1.0f)	{
		wsp = 1.0f;}
	kolor = vec3(1.0*wsp, 1.0*wsp, 0.0*wsp);

	gl_Position = perspective_matrix * view_matrix * macierz_przesuniecia*vec4(pozycja, 1.0);
}
