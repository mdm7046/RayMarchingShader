#version 100

precision mediump float;

/*-------------------------
    Type Declarations
--------------------------*/

struct Material {
    vec3 specularCoeffs, diffuseCoeffs, ambientCoeffs, reflectionCoeffs;
    float shininess;
};

struct LightColor {
    vec3 specularEmit, diffuseEmit;
};

struct Object {
    Material material;
    mat4 WorldToObjMatrix;
    mat3 ObjToWorldMatrixNoTrans;
    vec3 scaleFactors; //note this kind of scaling only works for convex shapes
    int objType;
};

struct Light {
    LightColor col;
    vec3 pos;
};

/*-------------------------
    Constants
--------------------------*/

const int maxSteps = 300;
const float minDist = 1e-3;
const float maxDist = 1e3;

const int MAX_OBJS = 100;

/*-------------------------
    Uniforms
--------------------------*/

uniform vec4 imgPlaneBottomLeft, centerOfProjection, stepX, stepY;

uniform vec3 sunDir;
uniform LightColor sunColor;
uniform vec3 ambientLight;
uniform vec3 skyColor, groundColor;

uniform struct {
    int numObjects;
    Object objects[MAX_OBJS];
} objectBuffer;

uniform struct {
    int numLights;
    Light lights[MAX_OBJS];
} lightsBuffer;

const Material mat = Material(vec3(1), vec3(1), vec3(1), vec3(1), 1.f);

/*-------------------------
    Marching Functions
--------------------------*/

vec3 march(vec4 pos, vec3 direction);

float SDFAt(vec4 pos, out Object nearestObj);
vec3 surfaceNormal(vec4 pos, Object nearestObj);

vec3 phongShading(vec4 pos, vec3 dir, Object nearestObj);
vec3 skybox(vec3 dir);

void main() {
    vec4 pixelPos = imgPlaneBottomLeft + (gl_FragCoord.x * stepX) + (gl_FragCoord.y * stepY);
    gl_FragColor = vec4(march(pixelPos, vec3(normalize(pixelPos - centerOfProjection))), 1);
}

///march one ray, and return the RGB value obtained via ray marching
///@param pos - homogenous coordinate of the starting ray position
///@param direction - normalized direction to march, with w = 0
vec3 march(vec4 pos, vec3 direction) {
    int timeout = 0;
    float totalDist = 0.f;
    while(timeout++ < maxSteps) {
        Object nearest;
        float dist = SDFAt(pos, nearest);
        pos.xyz += dist * direction;
        if(dist < minDist)
        return phongShading(pos, direction, nearest);
        if((totalDist += dist) >= maxDist)
        return skybox(direction);
    }
    return vec3(0);
}

const float INFINITY = 1.0 / 0.0;

float sphereSDF(vec4 point, vec3 scaleFactors);
vec3 sphereNormal(vec4 point);

float SDFAt(vec4 pos, out Object nearestObj) {
    float minDist = INFINITY;
    for(int i = 0; i < objectBuffer.numObjects; i++) {
        float objDist;
        Object focus = objectBuffer.objects[i];

        vec4 posObjCoord = focus.WorldToObjMatrix * pos;
        if(focus.objType == 0)
        objDist = sphereSDF(posObjCoord, focus.scaleFactors);
        else
        objDist = INFINITY;

        if(objDist < minDist) {
            minDist = objDist;
            nearestObj = focus;
        }
    }
    return minDist;
}

vec3 surfaceNormal(vec4 pos, Object nearestObj) {
    vec3 normal;
    pos = nearestObj.WorldToObjMatrix * pos;
    if(nearestObj.objType == 0) {
        normal = sphereNormal(pos);
    } else {
        normal = vec3(0);
    }
    return normalize(nearestObj.ObjToWorldMatrixNoTrans * normal);
}

vec3 phongShading(vec4 pos, vec3 dir, Object nearestObj) {
    return abs(surfaceNormal(pos, nearestObj));
}

vec3 skybox(vec3 dir) {
    return mix(
    mix(groundColor, skyColor, smoothstep(-0.25, 0.25, dir.y)),
    sunColor.diffuseEmit, dot(-sunDir, dir));
}

/*-------------------------------
Object Distance & Normal Functions
--------------------------------*/

float sphereSDF(vec4 point, vec3 scaleFactors) {
    return length(point.xyz * scaleFactors) - 1.;
}

vec3 sphereNormal(vec4 point) {
    return point.xyz;
}