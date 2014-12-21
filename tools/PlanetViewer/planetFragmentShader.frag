varying vec2 tex_coord;
uniform sampler2D cloud;
uniform sampler2D texture;
uniform float phase;
uniform float cloudPhase;
uniform bool cloudEnabled;
uniform bool bumpMapping;
uniform float solarAngle;
uniform vec3 ambientColor;
uniform vec2 texPixelSize;
uniform float pixelSize;

float PI = 3.14159265358979323846264;
float ATMOSPHERE_SIZE = 3.0;
float ANTIALIASING_SIZE = 1.0;
float tr = sqrt((tex_coord.x - 0.5) * (tex_coord.x - 0.5) + (tex_coord.y - 0.5) * (tex_coord.y - 0.5));

vec4 blend(vec4 dst, vec4 src)
{
   vec4 color;
   color.a = src.a + dst.a * (1.0 - src.a);
   color.rgb = (src.rgb * src.a + dst.rgb * dst.a * (1.0 - src.a)) / color.a;
   return color;
}

void getSpherePosition(in float radius, out vec3 pos)
{
    pos.x = ((tex_coord.x - 0.5)) / radius;
    pos.y = (0.5 - tex_coord.y) / radius;
    pos.z = sqrt(1.0 - pos.x * pos.x - pos.y * pos.y);
}

void mapSphereTexture(in vec3 pos, in float phase, out vec2 texCoord)
{
    texCoord.x = 0.5 + (atan(pos.x, pos.z) + phase) / (2.0 * PI);
    texCoord.y = 0.5 + pos.y / 2.0;
}

void calcLight(in sampler2D tex, in vec2 texCoord, in vec3 pos, in bool bump, in vec3 ambient, out vec4 light)
{
    float k;

    vec3 l = normalize(vec3(cos(solarAngle), -sin(solarAngle), 0.5));
    vec3 n = normalize(pos);

    if (bump)
    {
        float h_p10 = texture2D(tex, texCoord + vec2(texPixelSize.x, 0)).a;
        float h_m10 = texture2D(tex, texCoord + vec2(-texPixelSize.x, 0)).a;
        float h_0p1 = texture2D(tex, texCoord + vec2(0, texPixelSize.y)).a;
        float h_0m1 = texture2D(tex, texCoord + vec2(0, -texPixelSize.y)).a;

        vec3 t1 = vec3(1, 0, h_p10 - h_m10);
        vec3 t2 = vec3(0, 1, h_0p1 - h_0m1);
        vec3 texn = normalize(cross(t1, t2));

        vec3 t = normalize(vec3(sqrt(1.0 - pos.x * pos.x), 0.0, -pos.x));
        vec3 b = cross(n, t);

        vec3 v;
        v.x = dot(l, t);
        v.y = dot(l, b);
        v.z = dot(l, n);
        v = normalize(v);

        k = max(dot(v, texn), 0.0);
    }
    else
    {
        k = max(dot(l, n), 0.0);
    }

    light.rgb = vec3(1.0, 1.0, 1.0) * k * 0.7 + 0.3 * ambient;
    light.a = k * 0.6 + 0.4;
}

vec4 getPlanetColor()
{
    vec3 pos;
    vec4 light, planetColor;
    vec2 texCoord;
    float planetRadius = 0.5 - pixelSize * ATMOSPHERE_SIZE;

    if(tr > planetRadius)
        return vec4(0.0, 0.0, 0.0, 0.0);

    getSpherePosition(planetRadius, pos);
    mapSphereTexture(pos, phase, texCoord);
    calcLight(texture, texCoord, pos, bumpMapping, ambientColor, light);
    planetColor = vec4(texture2D(texture, texCoord).rgb * light.rgb, 1.0);
    planetColor.a = clamp((planetRadius - tr) / (pixelSize * ANTIALIASING_SIZE), 0.0, 1.0);
    return planetColor;
}

vec4 getAtmosphereColor()
{
    vec3 pos;
    vec4 light, cloudColor = vec4(0.0, 0.0, 0.0, 0.0);
    vec2 texCoord;
    float pixelAtmSize = pixelSize * ATMOSPHERE_SIZE;

    if(tr > 0.5)
        return vec4(0.0, 0.0, 0.0, 0.0);

    getSpherePosition(0.5, pos);
    calcLight(texture, vec2(0.0, 0.0), pos, false, ambientColor, light);
    light.a += 0.1;

    if (cloudEnabled)
    {
        mapSphereTexture(pos, cloudPhase, texCoord);
        vec4 texColor = texture2D(cloud, texCoord);
        cloudColor = texColor * light;
        cloudColor.a *= clamp((0.5 - tr) / (pixelSize * ANTIALIASING_SIZE), 0.0, 1.0);
    }
    if (tr > 0.5 - 2.0 * pixelAtmSize)
    {
        float atmAlpha = 0.8 * clamp(1.0 - abs((tr - 0.5 + pixelAtmSize) / (pixelAtmSize)), 0.0, 1.0);
        vec4 atmColor = light;
        atmColor.a *= atmAlpha;
        cloudColor = blend(cloudColor, atmColor);
    }
    return cloudColor;
}

void calcColor(out vec4 color)
{
    color = getPlanetColor();
    color = blend(color, getAtmosphereColor());
}

void main()
{
    calcColor(gl_FragColor);

    if (tr > 0.5)
        gl_FragColor = vec4(0.0, 0.0, 0.0, 0.0);
}
