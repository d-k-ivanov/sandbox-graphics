#pragma once

#include "Camera2D.h"

#include <glm/glm.hpp>

class CameraController
{
    Camera2D m_Cam;

    double m_VelDrag;
    glm::dvec2 m_Vel{};

    bool m_IsDragging;
    glm::dvec2 m_DragStart{}; //NDC on (-1,1)
    glm::dvec2 m_DragCurr{};
    glm::dvec2 m_MouseZoomPos{};

    double m_ScrollDrag;
    double m_ScrollVel;
public:
    CameraController(double xpos = 0, double ypos = 0, double scale = 0, double aspect = 1);
    void update(double timestep);
    void drag(double x, double y);
    void enddrag();
    void mouseZoom(double amount);
    void setMouseZoomPos(double x, double y);

    void applyCameraTransform() const;
    [[nodiscard]] Camera2D getCam() const;
};
