#version 460 core

//-------------DEFINES-------------//
#define MAX_POINT_LIGHTS 10
#define MAX_SPOT_LIGHTS  3

//-------------STRUCTS-------------//
struct PointLight
{
	vec3 pos;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

//-------------VARIABLES-------------//
in vec3 FragPos;
in vec3 Normal;
in vec2 UV;

out vec4 FragColor;

//Integers
uniform int nPointLights;

//Vectors
uniform vec3 viewPos;

//Structs
uniform PointLight pointLights[MAX_POINT_LIGHTS];

//Samplers
uniform sampler2D tex1;


//-------------FUNCTIONS-------------//
vec3 calculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	//Find direction of light to the fragment
	vec3 lightDir = normalize(light.pos - fragPos);

	//Diffuse intensity
	float diffVal = max(dot(normal, lightDir), 0.0);

	//Find the ray bouncing off the fragment
	// vec3 reflectDir = reflect(-lightDir, normal);
	//Specular intensity
	//Shine fixed for now
	// float specVal = pow(max(dot(viewDir, reflectDir), 0.0), 0);

	//Find distance from the light
	float fragDist = length(light.pos - fragPos);
	//Attenuation based on distance: 1 / (k + l*d + q*d^2)
	float attenuation = 1.0 / (light.constant + light.linear * fragDist + light.quadratic * fragDist * fragDist);

	//Results
	vec3 ambient = light.ambient * vec3(texture(tex1, UV));
	vec3 diffuse = light.diffuse * diffVal * vec3(texture(tex1, UV));
	//vec3 specular = light.specular * specVal * vec3(texture(tex2, UV));

	return (ambient + diffuse) * attenuation;
}


void main()
{
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 result = vec3(0.0);

	//Run directional lights
	
	//TODO: try with dynamic number of point lights
	//Run point lights
	for(int i = 0; i < MAX_POINT_LIGHTS; ++i)
	{
		if(i == nPointLights)
		{
			break;
		}

		//Process point lights
		result += calculatePointLight(pointLights[i], norm, FragPos, viewDir);
	}

    //FragColor = texture(tex1, uv);
    FragColor = vec4(result, 1.0);
}