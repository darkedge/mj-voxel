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
			int32 GetDepth() const { return m_Depth; }
			float GetZNear() const { return m_ZNear; }
			float GetZFar() const { return m_ZFar; }
			float GetFieldOfView() const { return m_FieldOfView; }

		private:
			ClearFlags m_clearFlags;
			math::float3 m_BackGround;
			float m_FieldOfView;
			float m_ZNear = 0.3f;
			float m_ZFar = 1000.0f;
			// A camera with a larger depth is drawn on top of a camera with a smaller depth.
			int32 m_Depth = -1;
			RenderTexture *m_renderTexture = nullptr;
		};
	} // namespace mj::gl
} // namespace mj
