#pragma once

namespace mj {
	namespace gl {
		class Program
		{
		public:
			enum class ShaderType
			{
				Vertex,
				Fragment,
			};
			Program( const char *vs, const char *fs );
			~Program();

			void Bind() const;

			void SetUniform( const char *name, const math::mat4 &mat );
			void SetUniform( const char *name, const math::float3 &f);
			void SetUniform( const char *name, int32 val);

		private:
			void AttachShaderFromFile( GLuint *shader, const char *file, ShaderType type );
			void Link() const;
			void Validate() const;
			bool FindProgramErrors( GLenum type ) const;

			GLuint m_program;
			GLuint m_vertex;
			GLuint m_fragment;
		};
	}
}
