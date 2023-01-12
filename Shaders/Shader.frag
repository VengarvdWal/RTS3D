#version 300 es                              
precision mediump float; 

in vec4 vCol;
in vec2 texCoord;
in vec3 normal;
in vec3 fragPos;

out vec4 fragColor;

const int MAX_POINT_LIGHTS = 3;
const int MAX_SPOT_LIGHTS = 3;

struct Light
{
	vec3 color;
	float ambientIntensity;
	float diffuseIntensity;
};

struct DirectionalLight
{
	Light base;
	vec3 direction;	
};

struct PointLight
{
	Light base;
	vec3 position;
	float constant;
	float linear;
	float quadratic;
};

struct SpotLight
{
	PointLight base;
	vec3 direction;
	float edge;
};

struct Material
{
	float specularIntensity;
	float roughness;
};

uniform int pointLightCount;
uniform int spotLightCount;

uniform DirectionalLight dLight;
uniform PointLight pLights[MAX_POINT_LIGHTS];
uniform SpotLight sLights[MAX_SPOT_LIGHTS];

uniform sampler2D aTexture;
uniform Material material;

uniform vec3 eyePosition;

vec4 CalculateLightByDir(Light light, vec3 direction)
{
	vec4 ambientColor = vec4(light.color, 1.0f) * light.ambientIntensity;

	float diffuseFactor = max(dot(normalize(normal), normalize(direction)), 0.0f);
	vec4 diffuseColor = vec4(light.color * light.diffuseIntensity * diffuseFactor, 1.0f);

	vec4 specularColor = vec4(0, 0, 0, 0);

	if(diffuseFactor > 0.0f)
	{
		vec3 fragToEye = normalize(eyePosition - fragPos);
		vec3 reflectedVertex = normalize(reflect(direction, normalize(normal)));

		float specularFactor = dot(fragToEye, reflectedVertex);
		if(specularFactor > 0.0f)
		{
			specularFactor = pow(specularFactor, material.roughness);
			specularColor = vec4(light.color * material.specularIntensity * specularFactor, 1.0f);
		}
	}

	return (ambientColor + diffuseColor + specularColor);
}

vec4 CalculateDirectionalLight()
{
	return CalculateLightByDir(dLight.base, dLight.direction);
}

vec4 CalculatePointLight(PointLight pLight)
{
	vec3 direction = fragPos - pLight.position;
		float distance = length(direction);
		direction = normalize(direction);

		vec4 color = CalculateLightByDir(pLight.base, direction);
		float attenuation = pLight.quadratic * distance * distance + 
							pLight.linear * distance + 
						    pLight.constant;

		return (color / attenuation);
}

vec4 CalculateSpotLight(SpotLight sLight)
{
	vec3 rayDirection = normalize(fragPos - sLight.base.position);
	float slFactor = dot(rayDirection, sLight.direction);

	if(slFactor > sLight.edge)
	{
		vec4 color = CalculatePointLight(sLight.base);

		return color * (1.0f - (1.0f - slFactor) * (1.0f/(1.0f - sLight.edge)));
	}else{
		return vec4(0, 0, 0, 0);
	}
}

vec4 CalculatePointLights()
{
	vec4 totalColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);
	for(int i = 0; i < pointLightCount; i++)
	{
		totalColor += CalculatePointLight(pLights[i]);
	}

	return totalColor;
}

vec4 CalculateSpotLights()
{
	vec4 totalColor = vec4(0, 0, 0, 0);
	for(int i = 0; i < spotLightCount; i++)
	{
		totalColor += CalculateSpotLight(sLights[i]);
	}
	return totalColor;
}

void main()
{
	vec4 finalColor = CalculateDirectionalLight();
	finalColor += CalculatePointLights();
	finalColor += CalculateSpotLights();
	
	fragColor = texture(aTexture, texCoord) * finalColor;
}