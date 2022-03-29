#include "Camera2D.h"


Camera2D::Camera2D(const double xpos, const double ypos, const double scale, const double aspect) :
    m_Pos(glm::dvec2(xpos, ypos)), m_Scale(scale), m_Aspect(aspect)
{
}

glm::dmat4 Camera2D::getTransform() const
{
    const double sc = exp(2.30258509 * m_Scale);
    glm::dmat4 ret = glm::dmat4();
    ret[0].x = sc;
    ret[1].y = sc;
    ret[2].z = 1;
    ret[3] = glm::vec4(-m_Pos.x * sc, -m_Pos.y * sc, 0, 1.0f);
    return ret;
}

glm::dmat4 Camera2D::getInvTransform() const
{
    const double sc = exp(-2.30258509 * m_Scale);
    glm::dmat4 ret = glm::dmat4();
    ret[0].x = sc;
    ret[1].y = sc;
    ret[2].z = 1;
    ret[3] = glm::vec4(m_Pos.x, m_Pos.y, 0, 1.0f);
    return ret;
}

glm::dmat4 Camera2D::getNDCTransform() const
{
    const double sc = exp(2.30258509 * m_Scale);
    glm::dmat4 ret = glm::dmat4();
    ret[0].x = sc / m_Aspect;
    ret[1].y = sc;
    ret[2].z = 1;
    ret[3] = glm::vec4(-m_Pos.x * sc / m_Aspect, -m_Pos.y * sc, 0, 1.0f);
    return ret;
}

glm::dmat4 Camera2D::getInvNDCTransform() const
{
    const double sc = exp(-2.30258509 * m_Scale);
    glm::dmat4 ret = glm::dmat4();
    ret[0].x = sc * m_Aspect;
    ret[1].y = sc;
    ret[2].z = 1;
    ret[3] = glm::vec4(m_Pos.x, m_Pos.y, 0, 1.0f);
    return ret;
}

double Camera2D::getAspect() const
{
    return m_Aspect;
}

double Camera2D::getXPos() const
{
    return m_Pos.x;
}

double Camera2D::getYPos() const
{
    return m_Pos.y;
}

double Camera2D::getScale() const
{
    return m_Scale;
}

double Camera2D::getScaleAbsolute() const
{
    return exp(-2.30258509 * m_Scale);
}

void Camera2D::setXPos(const double arg)
{
    m_Pos += glm::vec2(arg, 0);
}

void Camera2D::setYPos(const double arg)
{
    m_Pos += glm::vec2(0, arg);
}

void Camera2D::setScale(const double arg)
{
    m_Scale = arg;
}

void Camera2D::setAspect(const double arg)
{
    m_Aspect = arg;
}

void Camera2D::addPos(const glm::dvec2 arg)
{
    m_Pos += arg;
}

void Camera2D::addScroll(const double arg)
{
    m_Scale += arg;
}
