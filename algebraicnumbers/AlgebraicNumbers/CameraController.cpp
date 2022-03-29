#include "CameraController.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SDL_opengl.h>

CameraController::CameraController(const double xpos, const double ypos, const double scale, const double aspect) :
    m_VelDrag(.9), m_IsDragging(false), m_ScrollDrag(.87), m_ScrollVel(0)
{
    m_Cam = Camera2D(xpos, ypos, scale, aspect);
    m_MouseZoomPos = m_DragCurr = m_DragStart = m_Vel = glm::vec2(0, 0);
}

void CameraController::update(const double timestep)
{
    m_Cam.addPos(m_Vel * timestep);
    m_Vel *= m_VelDrag;
    m_Cam.addScroll(m_ScrollVel * timestep);
    m_ScrollVel *= m_ScrollDrag;
}

void CameraController::setMouseZoomPos(const double x, const double y)
{
    m_MouseZoomPos = glm::dvec2(x, y);
}

void CameraController::drag(const double x, const double y)
{
    if (m_IsDragging)
    {
        m_DragStart = m_DragCurr;
        m_DragCurr = glm::dvec2(x, y);
        // std::cout << cam.getAspect() / cam.getScaleAbsolute() * (dragstart-dragcurr).x;
        m_Cam.addPos(
        glm::dvec2(
                m_Cam.getAspect() * m_Cam.getScaleAbsolute() * (m_DragStart - m_DragCurr).x,
                (m_DragStart - m_DragCurr).y * m_Cam.getScaleAbsolute()
            )
        );
    }
    else
    {
        m_IsDragging = true;
        m_DragStart = m_DragCurr = glm::dvec2(x, y);
    }
}

void CameraController::enddrag()
{
    if (m_IsDragging)
    {
        m_IsDragging = false;
        m_Vel -= (m_DragCurr - m_DragStart) * m_VelDrag * m_Cam.getScaleAbsolute();
        m_DragCurr = m_DragStart = glm::dvec2(0, 0);
    }
}

void CameraController::mouseZoom(const double amount)
{
    /*
     * double factor = (1 - exp(2.30258509 * zamount)) / cam.getScaleAbsolute();
     * cam.addPos(
     *     glm::dvec2(
     *         cam.getAspect()*mousezoompos.x*factor,
     *         mousezoompos.y*factor
     *     )
     * );
     */

    m_Cam.addScroll(amount);
    m_ScrollVel += amount * m_ScrollDrag;
}

void CameraController::applyCameraTransform() const
{
    glLoadMatrixd(glm::value_ptr(m_Cam.getNDCTransform()));
}

Camera2D CameraController::getCam() const
{
    return m_Cam;
}
