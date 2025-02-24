//
// Created by mdm31 on 2/14/2025.
// provides classes for simple Objects to be placed in the scene, as well as the base Object interface
//

#ifndef OBJECT_H
#define OBJECT_H

#include "../../glm/vec4.hpp"
#include "../../glm/mat4x4.hpp"
#include "../../glm/geometric.hpp"

#include "../Utils.h"

// base Object interface, with purely virtual methods
class Object {
public:
    class Material {
    public:
        glm::vec3 specularCoeffs;
        glm::vec3 diffuseCoeffs;
        glm::vec3 ambientCoeffs;
        glm::vec3 reflectionCoeffs;
        float shininess;

        static const Material WhitePlastic;
        static const Material RedPlastic;
        static const Material GreenPlastic;
        static const Material BluePlastic;
        static const Material Metal;
    };


    virtual ~Object() = default;

    static unsigned int getType() { return -1; }
    virtual void getMatrices(glm::mat4x4 &WorldToObj, glm::mat3x3 &ObjectToWorldNoTrans, glm::vec3 &scaleFactors) { }
    virtual Material objMaterial() const { return {}; }

    virtual void transform(const glm::vec3 &translation, const glm::vec3 &rotation, const glm::vec3 &scaleFactors) { }
    virtual void move(const glm::vec3 &position, const glm::vec3 &rotation, const glm::vec3 &scaleFactors) { }
};

// a sphere, with a position, radius, and uniform color
class Ball : public Object {
private:
    glm::vec3 pos, radii, rotation;

    Material material;
public:
    Ball(const glm::vec3 &pos, const float radius, const Material &material) :
        pos(pos), radii(glm::vec3(radius, radius, radius)), rotation(glm::vec3(0, 0, 0)), material(material) { }

    static unsigned int getType() { return 0; }

    void transform(const glm::vec3 &translation, const glm::vec3 &rot, const glm::vec3 &scaleFactors) override {
        pos += translation;
        radii *= scaleFactors;
        rotation += rot;
    }

    void move(const glm::vec3 &translation, const glm::vec3 &rot, const glm::vec3 &scaleFactors) override {
        pos = translation;
        radii = scaleFactors;
        rotation = rot;
    }

    void getMatrices(glm::mat4x4 &WorldToObj, glm::mat3x3 &ObjectToWorldNoTrans, glm::vec3 &scaleFactors) override {
        const glm::mat4x4 invT = glm::translate(glm::identity<glm::mat4x4>(), -pos);
        const glm::mat4x4 R = Utils::rotate(glm::identity<glm::mat4x4>(), rotation);
        const glm::mat4x4 invR = Utils::rotateInverse(glm::identity<glm::mat4x4>(), rotation);
        const glm::mat4x4 invS = glm::scale(glm::identity<glm::mat4x4>(), glm::vec3(1, 1, 1) / radii);
        const glm::mat4x4 S = glm::scale(glm::identity<glm::mat4x4>(), radii);

        WorldToObj = invS * invR * invT;
        ObjectToWorldNoTrans = R * S;
        scaleFactors = radii;
    }

    Material objMaterial() const override { return material; }
};
/**
// a finite plane defined by three points which bound its corners
class Plane : public Object {
private:
    glm::dvec4 center; //point the plane passes through
    glm::dmat3x3 basisT; //transpose of the basis vectors e1, e2, and normal
    glm::dvec3 halfSideLength;

    Material material;
public:
    Plane(glm::dvec3 p1, glm::dvec3 p2, glm::dvec3 p3, Material material);

    double surfaceDist(glm::dvec4 point) override;

    glm::dvec3 surfaceNormal(glm::dvec4 point) override;

    void transform(glm::dvec3 translation, glm::dvec3 rotation, glm::dvec3 scaleFactors) override;

    Material objMaterial() const override { return material; }
};

//a box at position pos, with side lengths scale, and rotated around each axis by rot
// boxes have uniform color
class Box : public Object {
private:
    Transformation ObjToWorld;

    Material material;
public:
    Box(const glm::dvec3 &pos, const glm::dvec3 &rot, const glm::dvec3 &sideLengths, const Material &material);

    double surfaceDist(glm::dvec4 point) override;
    glm::dvec3 surfaceNormal(glm::dvec4 point) override;

    void transform(glm::dvec3 translation, glm::dvec3 rotation, glm::dvec3 scaleFactors) override;

    Material objMaterial() const override { return material; }
};
**/
class Light {
public:
    class LightColor {
    public:
        glm::vec3 specularColor;
        glm::vec3 diffuseColor;

        static const LightColor White;
        static const LightColor DimWhite;
        static const LightColor VeryDimWhite;
        static const LightColor Red;
        static const LightColor Green;
        static const LightColor Blue;
        static const LightColor Cyan;
    };

private:
    glm::vec4 pos;
    LightColor color;
public:

    Light(const glm::dvec3 &pos, const LightColor col) : pos(Utils::toHomoCoord(pos)), color(col) { }
    ~Light() = default;

    glm::dvec4 getPos() const { return pos; }
    LightColor getColor() const { return color; }
};

//----------------------------
// Color / Material Constants
//----------------------------

#define PlasticSpecCoeff 0.5f
#define PlasticDiffCoeff 0.5f
#define PlasticAmbCoeff 1.f
#define PlasticReflecCoeff 0.25f
#define PlasticShininess 3.f

#define WHITE glm::vec3(1, 1, 1)
#define BLACK glm::vec3(0, 0, 0)
#define RED glm::vec3(1, 0, 0)
#define GREEN glm::vec3(0, 1, 0)
#define BLUE glm::vec3(0, 0, 1)
#define GREY glm::vec3(0.5, 0.5, 0.5)

inline const Object::Material Object::Material::WhitePlastic = {PlasticSpecCoeff * WHITE, PlasticDiffCoeff * WHITE, PlasticAmbCoeff * WHITE, PlasticReflecCoeff * WHITE, PlasticShininess};
inline const Object::Material Object::Material::RedPlastic = {PlasticSpecCoeff * RED, PlasticDiffCoeff * RED, PlasticAmbCoeff * RED, PlasticReflecCoeff * RED, PlasticShininess};
inline const Object::Material Object::Material::GreenPlastic = {PlasticSpecCoeff * GREEN, PlasticDiffCoeff * GREEN, PlasticAmbCoeff * GREEN, PlasticReflecCoeff * GREEN, PlasticShininess};
inline const Object::Material Object::Material::BluePlastic = {PlasticSpecCoeff * BLUE, PlasticDiffCoeff * BLUE, PlasticAmbCoeff * BLUE, PlasticReflecCoeff * BLUE, PlasticShininess};

inline const Object::Material Object::Material::Metal = {0.5f * GREY, 0.25f * GREY, GREY, 0.75f * GREY, 50};


inline const Light::LightColor Light::LightColor::White = {WHITE, WHITE};
inline const Light::LightColor Light::LightColor::Red = {RED, RED};
inline const Light::LightColor Light::LightColor::Green = {GREEN, GREEN};
inline const Light::LightColor Light::LightColor::Blue = {BLUE, BLUE};
inline const Light::LightColor Light::LightColor::DimWhite = {0.5f * WHITE, 0.5f * WHITE};
inline const Light::LightColor Light::LightColor::VeryDimWhite = {0.25f * WHITE, 0.25f * WHITE};
inline const Light::LightColor Light::LightColor::Cyan = {{0, 1, 1}, {0, 1, 1}};
#endif //OBJECT_H
