#version 430

vec4 finalColor;
in vec3 color;
in vec3 norm;
in vec3 pos;
in vec2 fuv;

// Color change
layout (location = 7) uniform vec3 ObjColor;


// ambient light
vec3 lightColor = vec3(1, 1, 1);

// light source
vec3 lightSource = vec3(10, 10, 10);

// Camera Location
layout (location = 5) uniform vec3 cameraLoc;

// lighting math
vec3 ambience;
const float ambienceStrength = 0.75f;

vec3 diffuse;
const float diffuseStrength = 1.0f;

vec3 specular;
const float specularAlpha = 16.0f;
const float specularStrength = 2.0f;

vec3 brightness;


// unit vectors
vec3 L; //Frag to light
vec3 V; //Frag to camera
vec3 H; //Half way vec between light and camera
vec3 N; //Frag normal



// Texture
uniform sampler2D myTexture;     // This is updated in main, under "Loading objects"


void main()
{
	// Normalize frag normals
	N = normalize(norm);

	// Caclulate unit vectors
	L = normalize(lightSource - pos);
	V = normalize(cameraLoc - pos);
	H = normalize (L + V);

	ambience = color * vec3(1.0f, 1.0f, 1.0f) * ambienceStrength;
	diffuse = color * max(dot(L, N), 0) * diffuseStrength;
	specular = color * pow(max(dot(H, N), 0), specularAlpha) * specularStrength;

	brightness = ambience + diffuse + specular;

	vec4 FinTexture = texture(myTexture, fuv);
	finalColor = vec4(color * ObjColor * brightness, 1.0f) * FinTexture; 
	
	gl_FragColor = finalColor;
}