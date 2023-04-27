
#include <pch.h>

#include "EditorCamera.h"

#include <axt/Core.h>
#include <axt/core/Input.h>
#include <axt/core/Keycodes.h>

#include <axt/core/Log.h>

#include <algorithm>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

namespace axt
{

	static glm::vec3 gFrontVector{ 0.0f, 0.0f, -1.0f };

	EditorCamera::EditorCamera(float fieldOfView) :
		mFOV{ fieldOfView },
		mMousePosition{ input::GetMousePosition() },
		mDirection{ 0.f }
	{
		Rotate(0.f, { 0.f, 0.f });
		Translate(0.f);
		BuildMatrices();
	}

	void EditorCamera::OnUpdate(float dt, bool usable)
	{
		if (input::IsMouseButtonPressed(Key::MouseRight) && usable)
		{
			glm::vec2 mouseCurrentPosition{ input::GetMousePosition() };
			glm::vec2 mouseDelta{ 0.f };
			if (!mStart)
			{
				mouseDelta = mMousePosition - mouseCurrentPosition;
			}
			mStart = false;
			mMousePosition = mouseCurrentPosition;
			Rotate(dt, mouseDelta);
			Translate(dt);

			BuildMatrices();
		}
		else
		{
			mStart = true;
		}
	}

	void EditorCamera::Rotate(float dt, const glm::vec2& mouseDelta)
	{
		mYaw -= mouseDelta.x * mSensitivity;
		mPitch += mouseDelta.y * mSensitivity;

		mPitch = std::max(-89.0f, std::min(89.0f, mPitch + (mouseDelta.y * mSensitivity)));

		glm::vec3 direction{};

		direction.x = std::cos(glm::radians(mYaw)) * std::cos(glm::radians(mPitch));
		direction.y = std::sin(glm::radians(mPitch));
		direction.z = std::sin(glm::radians(mYaw)) *std::cos(glm::radians(mPitch));

		mDirection = glm::normalize(direction);
	}

	void EditorCamera::Translate(float dt)
	{
		float translationValue{ (mMovementSpeed * mZoom * dt) };
		// X Axis
		if (input::IsKeyPressed(Key::A))
		{
			mPosition -= mMovementSpeed * dt * glm::normalize(glm::cross(GetLookVector(), GetUpVector()));
		}
		else if (input::IsKeyPressed(Key::D))
		{
			mPosition += mMovementSpeed * dt * glm::normalize(glm::cross(GetLookVector(), GetUpVector()));
		}
		// Z Axis
		if (input::IsKeyPressed(Key::W))
		{
			mPosition += mMovementSpeed * dt * GetLookVector();
		}
		else if (input::IsKeyPressed(Key::S))
		{
			mPosition -= mMovementSpeed * dt * GetLookVector();
		}
		// Y Axis
		if (input::IsKeyPressed(Key::E))
		{
			mPosition += mMovementSpeed * dt * GetUpVector();
		}
		else if (input::IsKeyPressed(Key::Q))
		{
			mPosition -= mMovementSpeed * dt * GetUpVector();
		}
	}

	bool EditorCamera::OnEvent(Event& ev)
	{
		EventHandler handler{ ev };
		handler.Fire<WindowResizeEvent>(AXT_BIND_EVENT(EditorCamera::OnWindowResize));
		return false;
	}

	bool EditorCamera::OnWindowResize(WindowResizeEvent& ev)
	{
		float aspectRatio{ static_cast<float>(ev.GetWidth()) / static_cast<float>(ev.GetHeight()) };
		mAspectRatio = aspectRatio;
		BuildProjectionMatrix();
		BuildMatrices();
		return false;
	}

	void EditorCamera::BuildProjectionMatrix()
	{
		if (mIsPerspective)
		{
			mProjection = glm::perspective(mFOV, mAspectRatio, 0.1f, 1000.f);
		}
		else
		{
			float bounds{ mAspectRatio * mZoom };
			mProjection = glm::ortho(-bounds, bounds, -mZoom, mZoom, 0.f, 1000.f);
		}
	}

	void EditorCamera::BuildMatrices()
	{
		glm::mat4 ones{ 1.f };
		//glm::mat4 transform{ glm::translate(ones, mPosition) * glm::toMat4(glm::quat{mRotation}) * glm::scale(ones, mScale) };
		//mView = glm::inverse(transform);
		mView = glm::lookAt(mPosition, mPosition + GetLookVector(), GetUpVector());
		mViewProjection = mProjection * mView;
	}

	const glm::vec3& EditorCamera::GetLookVector() const
	{
		return mDirection;
	}

	glm::vec3 EditorCamera::GetRightVector() const
	{
		return glm::normalize(glm::cross(glm::vec3{ 0.f, 1.f, 0.f }, mDirection));
	}

	glm::vec3 EditorCamera::GetUpVector() const
	{
		return glm::normalize(glm::cross(mDirection, GetRightVector()));
	}

}