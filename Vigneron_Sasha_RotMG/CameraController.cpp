#include "pch.h"
#include "CameraController.h"
#include "Character.h"
#include "utils.h"

CameraController::CameraController(float width, float height)
	: m_Width{ width }
	, m_Height{ height }
	, m_AccFadeTime{}
	, m_FadeInTime{ 1.f }
	, m_HasFadedIn{false}
	, m_Zoom{0.001f}
	, m_ZoomStep{0.3f}
	, m_TargetZoom{ 1.f }
	, m_MinZoom{0.5f}
	, m_MaxZoom{2.f}
	, m_InitialZoom{1.f}
	, m_ZoomSpeedMultiplier{3.f}
{
}

CameraController::~CameraController()
{
}

void CameraController::Follow(const Point2f& target, float elapsedSec)
{
	if (!m_HasFadedIn) 
	{
		FadeIn(elapsedSec);
	}
	else 
	{
		float precision{ 0.05f };
		if (abs(m_Zoom - m_TargetZoom) >= precision)
		{
			if (m_Zoom < m_TargetZoom) m_Zoom += elapsedSec * m_ZoomSpeedMultiplier;
			if (m_Zoom > m_TargetZoom) m_Zoom -= elapsedSec * m_ZoomSpeedMultiplier;
			ClampZoom();
		}
	}

	Point2f targetPos{ target };
	targetPos.x *= m_Zoom;
	targetPos.y *= m_Zoom;
	targetPos.x -= m_Width / 2.0f;
	targetPos.y -= m_Height / 2.0f;
	m_Position = targetPos;
}


void CameraController::ZoomIn()
{
	if (m_HasFadedIn) 
	{
		m_TargetZoom += m_ZoomStep;
	}
}

void CameraController::ZoomOut()
{
	if (m_HasFadedIn)
	{
		m_TargetZoom -= m_ZoomStep;
	}
}

void CameraController::ResetZoom()
{
	m_TargetZoom = m_InitialZoom;
}

void CameraController::FadeIn(float elapsedSec)
{
	m_AccFadeTime += elapsedSec;
	m_Zoom += elapsedSec;
	if (m_AccFadeTime >= m_FadeInTime)
	{
		ResetZoom();
		m_HasFadedIn = true;
	}
}

void CameraController::ClampZoom()
{
	if (m_Zoom < m_MinZoom) m_Zoom = m_MinZoom;
	if (m_TargetZoom < m_MinZoom) m_TargetZoom = m_MinZoom;
	if (m_Zoom > m_MaxZoom) m_Zoom = m_MaxZoom;
	if (m_TargetZoom > m_MaxZoom) m_TargetZoom = m_MaxZoom;
}

void CameraController::OpenDrawingMatrix(float menuOffset) const
{
	glPushMatrix();
	{
		glTranslatef(-m_Position.x  - menuOffset, -m_Position.y, 0.0f);
		glScalef(m_Zoom, m_Zoom, 1);
	}
}

void CameraController::CloseDrawingMatrix() const
{
	glPopMatrix();
}



