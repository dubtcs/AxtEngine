#pragma once

#include <axt/Core.h>

#include <axt/events/Event.h>
#include <axt/events/WindowEvent.h>

#include <glm/glm.hpp>

namespace axt
{

	class AXT_API EditorCamera
	{
	public:
		EditorCamera(float fieldOfView = 90.f);
		void OnUpdate(float dt);
		bool OnEvent(Event& ev);
	public:
		const glm::vec3& GetLookVector() const;
		glm::vec3 GetUpVector() const;
		glm::vec3 GetRightVector() const;
		const glm::mat4& GetViewMatrix() const { return mView; }
		const glm::mat4& GetProjectionMatrix() const { return mProjection; }
		const glm::mat4& GetViewProjectionMatrix() const { return mViewProjection; }
		bool IsPerspective() const { return mIsPerspective; }
	protected:
		void Translate(float dt);
		void Rotate(float dt, const glm::vec2& mouseDelta);
		void BuildProjectionMatrix();
		void BuildMatrices();
		bool OnWindowResize(WindowResizeEvent& ev);
	protected:
		bool mIsPerspective{ true };

		float mAspectRatio;
		bool mStart{ true };

		float mSensitivity{ 1.f };
		float mMovementSpeed{ 1.f };

		float mZoom{ 1.f };
		float mFOV{ 90.f };
		glm::vec3 mPosition{ 0.f, 0.f, 1.f };
		glm::vec3 mDirection;
		glm::vec3 mScale{ 1.f };
		glm::vec2 mMousePosition;

		float mYaw{ -90.f };
		float mPitch{ 0.f };

		glm::mat4 mView;
		glm::mat4 mProjection;
		glm::mat4 mViewProjection;

		friend class ELayer;
	};

}
