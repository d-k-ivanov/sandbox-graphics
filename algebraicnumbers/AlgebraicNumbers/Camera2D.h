#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera2D
{
    glm::dvec2 m_Pos;
    double m_Scale;
    double m_Aspect;
public:
    Camera2D(double xpos = 0, double ypos = 0, double scale = 0, double aspect = 1);
    [[nodiscard]] glm::dmat4 getTransform() const;
    [[nodiscard]] glm::dmat4 getInvTransform() const;
    [[nodiscard]] glm::dmat4 getNDCTransform() const;
    [[nodiscard]] glm::dmat4 getInvNDCTransform() const;
    [[nodiscard]] double getAspect() const;
    [[nodiscard]] double getXPos() const;
    [[nodiscard]] double getYPos() const;
    [[nodiscard]] double getScale() const;
    [[nodiscard]] double getScaleAbsolute() const; //returns exp(-2.30258509*scale)

    void setXPos(double arg);
    void setYPos(double arg);
    void addPos(glm::dvec2 arg);
    void addScroll(double arg);
    void setScale(double arg);
    void setAspect(double arg);
};
