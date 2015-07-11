#pragma once

namespace mj {
	namespace gl {
		class Program
		{
		public:
			enum ShaderType
			{
				Vertex,
				Fragment,
			};
			Program(const char *vs, const char *fs);
			~Program();

			void Bind();

		private:
			bool ReadShaderFile( const char *file, ShaderType type );
			GLuint handle;
		};
	}
}
