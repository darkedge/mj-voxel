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
