varying vec2 texCoord;

uniform sampler2D cloud;
uniform sampler2D texture;
uniform bool cloudEnabled;
uniform vec4 ambientColor;
uniform vec2 texPixelSize;
uniform float pixelSize;

float PI = 3.14159265358979323846264;
float tr = sqrt((texCoord.x - 0.5) * (texCoord.x - 0.5) + (texCoord.y - 0.5) * (texCoord.y - 0.5));

vec4 blend(vec4 dst, vec4 src)
{
   vec4 color;
   color.a = src.a + dst.a * (1.0 - src.a);
   color.rgb = (src.rgb * src.a + dst.rgb * dst.a * (1.0 - src.a)) / color.a;
   return color;
}

void getPosition(in float radius, out vec3 pos)
{
    pos.x = ((texCoord.x - 0.5)) / radius;
    pos.y = (0.5 - texCoord.y) / radius;
    pos.z = sqrt(1.0 - pos.x * pos.x - pos.y * pos.y);
}

void mapTexture(in vec3 pos, in float phase, out vec2 coord)
{
    coord.x = 0.5 + (atan(pos.x, pos.z) + phase) / (2.0 * PI);
    coord.y = 0.5 - pos.y / 2.0;
}

void calcLight(in sampler2D tex, in vec2 coord, in vec3 pos, in bool bump, out float light)
{
    float k;

    vec3 l = normalize(vec3(-1.0, 0.0, 0.5));
    vec3 n = normalize(pos);

    if(bump)
    {
        // Bump-mapping
        // Get normal from height map
        float h_p10 = texture2D(tex, coord + vec2(texPixelSize.x, 0)).a;
        float h_m10 = texture2D(tex, coord + vec2(-texPixelSize.x, 0)).a;
        float h_0p1 = texture2D(tex, coord + vec2(0, -texPixelSize.y)).a;
        float h_0m1 = texture2D(tex, coord + vec2(0, texPixelSize.y)).a;
        vec3 t1 = vec3(1, 0, h_p10 - h_m10);
        vec3 t2 = vec3(0, 1, h_0p1 - h_0m1);
        vec3 texn = normalize(cross(t1, t2));

        vec3 t = normalize(vec3(sqrt(1.0 - pos.x * pos.x), 0.0, -pos.x));
        vec3 b = cross(n, t);

        // Map to tangent space
        vec3 v;
        v.x = dot(l, t);
        v.y = dot(l, b);
        v.z = dot(l, n);
        v = normalize(v);

        // Calc diffuse
        light = max(dot(v, texn), 0.0);
    }
    else
    {
        light = max(dot(l, n), 0.0);
    }
}

void calcColor(out vec4 color)
{
    vec3 pos;
    vec2 coord;
    float light;

    float planetRadius = 0.5 - pixelSize;
    getPosition(planetRadius, pos);
    mapTexture(pos, 0.0, coord);
    
    vec3 planetTexColor = texture2D(texture, coord).rgb;
    calcLight(texture, coord, pos, true, light);
    color = vec4(planetTexColor * (light * 0.7 + 0.3), 1.0);
    
    if (cloudEnabled)
    {
        mapTexture(pos, 0.0, coord);
        vec4 texColor = texture2D(cloud, coord);
        vec4 cloudColor = vec4(texColor.rgb * ambientColor.rgb * (light + 0.3), texColor.a);
        color = blend(color, cloudColor);
    }

    getPosition(0.5, pos);
    calcLight(texture, vec2(0.0, 0.0), pos, false, light);
    vec3 atmColor = ambientColor.rgb;

    //TODO: Move atmosphere to uniforms.
    if ((tr > planetRadius - 2.0 * pixelSize) && tr <= planetRadius)
    {
        float aaAlpha = clamp((planetRadius - tr) / pixelSize, 0.0, 1.0);
        color.a *= aaAlpha;
        float atmAlpha = clamp((1.0 - (planetRadius - tr) / (2.0 * pixelSize)) * (light + 0.2), 0.0, 0.8);
        color = blend(color, vec4(atmColor, atmAlpha));
    }
    else if (tr > planetRadius && tr <= 0.5)
    {
        float atmAlpha = clamp((0.5 - tr) / (1.0 * pixelSize) * (light + 0.2), 0.0, 0.8);
        color = vec4(atmColor, atmAlpha);
    }
    
    float aaAlpha = clamp((0.5 - tr) / pixelSize, 0.0, 1.0);
    color.a *= aaAlpha;
}

void main()
{
    calcColor(gl_FragColor);
    //gl_FragColor = vec4(pow(gl_FragColor.rgb, vec3(1.0/1.8)), gl_FragColor.a);

    if (tr > 0.5)
        gl_FragColor = vec4(0.0, 0.0, 0.0, 0.0);
} 
