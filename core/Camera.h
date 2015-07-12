#pragma once
namespace mj {
	namespace gl {
		class RenderTexture;
		class Camera
		{
		public:
			enum class ClearFlags
			{
				Skybox,
				SolidColor,
				DepthOnly,
				Nothing,
			};

			enum class Projection
			{
				Perspective,
				Orthographic,
			};

		public:
			Camera();
			~Camera();

			void Bind();

			// Getters/Setters
			int32 GetDepth() const { return m_depth; }
			float GetZNear() const { return m_zNear; }
			float GetZFar() const { return m_zFar; }
			float GetFieldOfView() const { return m_fieldOfView; }
			const math::mat4 & GetProjectionMatrix() const { return m_projectionMatrix; }

		private:
			Projection m_projection = Projection::Perspective;
			ClearFlags m_clearFlags = ClearFlags::Skybox;
			math::float3 m_backGround;
			float m_fieldOfView = 60.0f; // Degrees
			float m_zNear = 0.3f;
			float m_zFar = 1000.0f;
			// A camera with a larger depth is drawn on top of a camera with a smaller depth.
			int32 m_depth = -1;
			RenderTexture *m_renderTexture = nullptr;

			math::mat4 m_projectionMatrix;
		};
	} // namespace mj::gl
} // namespace mj
