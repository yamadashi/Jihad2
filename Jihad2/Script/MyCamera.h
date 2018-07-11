#pragma once
# include <Siv3D.hpp>
# include <HamFramework.hpp>


class MyCamera : public ham::BasicCamera2D {
private:
	s3d::Vec2 m_targetPos = BasicCamera2D::m_pos;
	double m_targetScale = BasicCamera2D::m_scale;
	
	s3d::Mat3x2 getMat3x2() const
	{
		return s3d::Mat3x2::Scale(m_scale).translate(s3d::Window::Size() * 0.5 - m_scale * m_pos);
	}

	s3d::RectF getCameraArea() const
	{
		return s3d::RectF(m_pos - s3d::Window::Size() * 0.5 / m_scale, s3d::Window::Size() / m_scale);
	}

public:
	MyCamera()
		:BasicCamera2D({ 1900, 0.5*Window::Size().y }, 1.0),
		m_targetPos(1900, 0.5*Window::Size().y),
		m_targetScale(1.0)
	{}

	~MyCamera() = default;

	void update(double speed = 10.0, double scaleRatio = 1.01, double lerpRatio = 0.2)
	{
		//ÉJÉÅÉâëÄçÏ
		m_targetPos.x += (Input::KeyD.pressed - Input::KeyA.pressed)*speed / m_targetScale;

		if (Input::KeyW.pressed && m_targetScale < 1.0)
		{
			m_targetScale *= scaleRatio;
		}

		if (Input::KeyS.pressed && m_targetScale > 0.39)
		{
			m_targetScale /= scaleRatio;
		}

		//ÉJÉÅÉâîÕàÕÇÃç∂âEêßå¿
		auto&& area = getCameraArea();
		if ((int)area.x < 800) {
			m_targetPos.x = (int)(Window::Width() * 0.5 / m_scale) + 800;
		}
		else if ((int)(area.x + area.w) > 4571) {
			m_targetPos.x = (int)(Window::Width() * 0.5 / m_scale - area.w) + 4571;
		}


		//ï‚äÆ
		m_pos = s3d::Math::Lerp(m_pos, m_targetPos, lerpRatio);

		m_scale = s3d::Math::Lerp(m_scale, m_targetScale, lerpRatio);
	}

	s3d::Transformer2D createTransformer() const
	{
		return s3d::Transformer2D(getMat3x2(), true);
	}

	double getScale() const { return m_targetScale; }
};