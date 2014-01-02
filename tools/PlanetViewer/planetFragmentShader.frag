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
float tr = sqrt((tex_coord.x - 0.5) * (tex_coord.x - 0.5) + (tex_coord.y - 0.5) * (tex_coord.y - 0.5));

void getPosition(in float radius, out vec3 pos)
{
    pos.x = ((tex_coord.x - 0.5)) / radius;
    pos.y = (0.5 - tex_coord.y) / radius;
    pos.z = sqrt(1.0 - pos.x * pos.x - pos.y * pos.y);
}

void mapTexture(in vec3 pos, in float phase, out vec2 texCoord)
{
    texCoord.x = 0.5 + (atan(pos.x, pos.z) + phase) / (2.0 * PI);
    texCoord.y = 0.5 + pos.y / 2.0;
}

void calcLight(in sampler2D tex, in vec2 texCoord, in vec3 pos, in bool bump, in vec3 ambient, out vec3 light)
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

        vec3 t = normalize(vec3(-sqrt(1.0 - pos.x * pos.x), 0.0, pos.x));
        vec3 b = cross(t, n);

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

    light = vec3(1.0, 1.0, 1.0) * k * 0.8 + 0.4 * ambient;
}

void calcColor(out vec4 color)
{
    vec3 pos, light;
    vec2 texCoord;

    float planetRadius = 0.5 - pixelSize * 2.0;

    getPosition(planetRadius, pos);
    mapTexture(pos, phase, texCoord);
    calcLight(texture, texCoord, pos, bumpMapping, ambientColor, light);

    vec4 planetColor = vec4(texture2D(texture, texCoord).rgb * light, 1.0);

    getPosition(0.5, pos);
    calcLight(texture, vec2(0.0, 0.0), pos, false, ambientColor, light);

    //TODO: Move atmosphere to uniforms.
    if ((tr > planetRadius - 2.0 * pixelSize) && tr <= 0.5)
    {
        float atmAlpha = clamp(1.0 - abs((tr - planetRadius) / (pixelSize * 4.0)), 0.0, 1.0);
        vec3 atmColor = light;
        planetColor = mix(planetColor, vec4(atmColor, atmAlpha), 0.8 * atmAlpha);
    }

    if (cloudEnabled)
    {
        vec4 texColor = texture2D(cloud, texCoord);
        vec4 cloudColor = vec4(texColor.rgb * light, texColor.a * 0.7);
        color = mix(planetColor, cloudColor, cloudColor.a);
    }
    else
    {
        color = planetColor;
    }

    //Anti-aliasing
    float aaAlpha = clamp((0.5 - tr) / pixelSize, 0.0, 1.0);
    color.a *= aaAlpha;
}

void main()
{
    calcColor(gl_FragColor);

    if (tr > 0.5)
        gl_FragColor = vec4(0.0, 0.0, 0.0, 0.0);
}
