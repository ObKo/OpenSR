varying vec2 tex_coord;
uniform sampler2D cloud;
uniform sampler2D texture;
uniform float phase;
uniform float cloudPhase;
uniform bool cloudEnabled;
uniform bool colorCorrection;
uniform float solarAngle;
uniform vec3 ambientColor;

float PI = 3.14159265358979323846264;
float tr = (tex_coord.x - 0.5f) * (tex_coord.x - 0.5f) + (tex_coord.y - 0.5f) * (tex_coord.y - 0.5f);

void getColor(in sampler2D tex, in float phase, in float radius, in bool cloud, out vec4 color)
{
    float x = sqrt(radius * radius - (tex_coord.x - 0.5f) * (tex_coord.x - 0.5f) - (tex_coord.y - 0.5f) * (tex_coord.y - 0.5f));
    float r = sqrt(x * x + (tex_coord.x - 0.5f) * (tex_coord.x - 0.5f) + (tex_coord.y - 0.5f) * (tex_coord.y - 0.5f));
    float lambda = atan((tex_coord.x - 0.5f) / x) + phase;
    float theta = acos((tex_coord.y - 0.5f) / r);
    vec2 result;
    result.x = 0.5f + (1.0f / (2.0f * PI) * lambda);
    result.y = 0.5f - cos(theta) / 2.0f;

    if (tr <= radius * radius)
    {
        color = texture2D(tex, result);

        if (!cloud && colorCorrection)
        {
            float y = 0.299 * color.r + 0.587 * color.g + 0.114 * color.b;
            float u = -0.14713 * color.r - 0.28886 * color.g + 0.436 * color.b;
            float v = 0.615 * color.r - 0.51499 * color.g - 0.10001 * color.b;
            y = y * (0.5f + color.a / 2.0f);
            color.r = y + 1.13983 * v;
            color.g = y - 0.39465 * u - 0.58060 * v;
            color.b = y + 2.03211 * u;
        }

        if (!cloud)
            color.a = 1.0f;
    }
    else
        color = vec4(0.0f, 0.0f, 0.0f, 0.0f);
}

void main()
{
    vec4 t0, t1;
    if (cloudEnabled)
    {
        getColor(texture, phase, 0.49f, false, t0);
        getColor(cloud, cloudPhase, 0.5f, true, t1);
        gl_FragColor = mix(t0, t1, t1.a);

        if (tr >= 0.23)
            gl_FragColor.a = (0.25f - tr) * 50.0f;
        else
            gl_FragColor.a = 1.0f;
    }
    else
    {
        getColor(texture, phase, 0.5f, false, t0);
        gl_FragColor = t0;

        if (tr >= 0.23)
            gl_FragColor.a = (0.25f - tr) * 50.0f;
    }
    float x = sqrt(0.5f * 0.5f - (tex_coord.x - 0.5f) * (tex_coord.x - 0.5f) - (tex_coord.y - 0.5f) * (tex_coord.y - 0.5f));
    vec3 normal = normalize(vec3(x, tex_coord.x - 0.5f, tex_coord.y - 0.5f));
    float diffuse = max(dot(normal, vec3(0.5f, cos(solarAngle), sin(solarAngle))), 0.0);
    gl_FragColor.rgb = gl_FragColor.rgb * 0.3 * ambientColor + gl_FragColor.rgb * diffuse * vec3(1.0, 1.0f, 1.0f);
}
