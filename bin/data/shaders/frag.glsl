#version 150

const int MAX_MARCHING_STEPS = 164;
const float EPSILON = 0.0015;
const float NEAR_CLIP = 0.0;
const float FAR_CLIP = 80.00;

out vec4 outputColor;

uniform float u_time;
uniform vec2 u_resolution;
uniform float u_value1;
uniform float u_value2;

vec3 lightDirection = vec3(0.702, 0.1686, 0.6745);

float sdSphere(vec3 pos, float radius){
    return length(pos) - radius;
}

float sdTorus( vec3 p, vec2 t ){
    vec2 q = vec2(length(p.xz)-t.x,p.y);
    return length(q)-t.y;
}

vec2 rotate(vec2 pos, float angle){
    float c = cos(angle);
    float s = sin(angle);
    return mat2(c, s, -s, c) * pos;
}

float smin( float a, float b, float k ){
    float h = clamp( 0.5+0.5*(b-a)/k, 0.0, 1.0 );
    return mix( b, a, h ) - k*h*(1.0-h);
}

float map(vec3 pos){
    float freqOnYZ = .1;
    float freqOnXZ = .4;

    pos.xz = rotate(pos.xz, sin(u_time* 0.4) *.7);
    pos.yz = rotate(pos.yz, cos(u_time* 0.1) *.7);

    vec3 posSphere = pos;
    posSphere.y -= 5.0 * u_value2;
    vec3 posTorus = pos;

    float torus = sdTorus(posTorus,vec2(4.0* u_value1, 0.48)); //2.5
    float sph = sdSphere(posSphere, 2.0);

    return smin(sph,torus, 3.0);
}

vec2 squareFrame(vec2 res, vec2 coord){
    vec2 uv = 2.0 * coord.xy / res.xy - 1.0;
    uv.x *= res.x / res.y;
    return uv;
}

float raymarching(vec3 eye, vec3 marchingDirection){
    float depth = NEAR_CLIP;
    for (int i = 0; i < MAX_MARCHING_STEPS; i++) {
        float dist = map(eye + depth * marchingDirection);
        if (dist < EPSILON){
            return depth;
        }

        depth += dist;

        if (depth >= FAR_CLIP) {
            return FAR_CLIP;
        }
    }
    return FAR_CLIP;
}

vec3 computeNormal(vec3 pos){
    vec2 eps = vec2(0.01, 0.);
    return normalize(vec3(
                          map(pos + eps.xyy) - map(pos - eps.xyy),
                          map(pos + eps.yxy) - map(pos - eps.yxy),
                          map(pos + eps.yyx) - map(pos - eps.yyx)
                          ));
}

float diffuse(vec3 normal){
    float ambient = 0.3;
    return clamp( dot(normal, lightDirection) * ambient + ambient, 0.0, 1.0 );
}

float specular(vec3 normal, vec3 dir){
    vec3 h = normalize(normal - dir);
    float specularityCoef = 100.;
    return clamp( pow(max(dot(h, normal), 0.), specularityCoef), 0.0, 1.0);
}

mat3 setCamera( in vec3 ro, in vec3 ta, float cr ){
    vec3 cw = normalize(ta-ro);
    vec3 cp = vec3(sin(cr), cos(cr),0.0);
    vec3 cu = normalize( cross(cw,cp) );
    vec3 cv = normalize( cross(cu,cw) );
    return mat3( cu, cv, cw );
}

void main(void){
    vec2 uv = squareFrame(u_resolution.xy, gl_FragCoord.xy);
    vec3 eye = vec3( 0.0, 3.0, 11.0);
    vec3 ta = vec3(0.0, 0.0, 0.0 );

    mat3 camera = setCamera( eye, ta, 0.0 );
    float fov = 2.0;
    vec3 dir = camera * normalize(vec3(uv, fov));
    float shortestDistanceToScene = raymarching(eye, dir);

    vec3 color;
    vec3 bgColor = vec3(0.922, 0.763, 0.0);

    if (shortestDistanceToScene < FAR_CLIP - EPSILON) {
        vec3 collision = (eye += (shortestDistanceToScene*0.995) * dir );
        float lightDistance = sdSphere(collision, 1.0);
        vec3 normal = computeNormal(collision);
        float diffLight = diffuse(normal);
        float specLight = specular(normal, dir);
        vec3 matCol = vec3(0.744, 0.113, 0.989);
        color = (diffLight + specLight ) * matCol;
    } else {
        color = bgColor;
    }

    outputColor = vec4(clamp(color,0.0,1.0) , 1.0);
}
