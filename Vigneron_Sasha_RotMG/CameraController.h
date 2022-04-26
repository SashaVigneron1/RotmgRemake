#pragma once

struct Window;
class Character;

class CameraController final
{
public:
	CameraController(float width, float height);
	CameraController(const CameraController& other) = delete;
	CameraController& operator=(const CameraController& other) = delete;
	CameraController(CameraController&& other) = delete;
	CameraController& operator=(CameraController&& other) = delete;
	~CameraController();
	
	void Follow(const Point2f& target, float elapsedSec);
	void ZoomIn();
	void ZoomOut();
	void ResetZoom();
	void OpenDrawingMatrix(float menuOffset) const;
	void CloseDrawingMatrix() const;


private:
	const float m_Width;
	const float m_Height;

	const float m_FadeInTime;
	bool m_HasFadedIn;
	float m_AccFadeTime;

	Point2f m_Position;

	//Zoom
	const float m_InitialZoom;
	float m_Zoom;
	float m_TargetZoom;
	float m_ZoomSpeedMultiplier;
	const float m_ZoomStep;
	const float m_MaxZoom;
	const float m_MinZoom;

	void FadeIn(float elapsedSec);
	void ClampZoom();
};

