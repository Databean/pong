#include <GL/gl.h>
#include #include <boost/gil/extension/io/png_io.hpp>

//workaround for bug in GIL
#define png_infopp_NULL (png_infopp)NULL
#define int_p_NULL (int*)NULL
#define png_bytep_NULL (png_bytep)NULL

using std::string;

GLuint loadImage(const string& path) {
	downloadImageIfNecessary(host, path);
	string localPath = "." + host + "/" + path;
	
	GLuint ret;
	glGenTextures(1, &ret);
	glBindTexture(GL_TEXTURE_2D, ret);
	
	rgba8_image_t image;
	png_read_and_convert_image(localPath, image);
	
	const GLvoid* data = reinterpret_cast<const GLvoid*>(interleaved_view_get_raw_data(view(image)));
	
	// when texture area is large, bilinear filter the original
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	
	// when texture area is large, bilinear filter the original
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	
	glTexImage2D(GL_TEXTURE_2D, /* levels */ 0, GL_RGBA, image.width(), image.height(), /* border */ 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	
	
	
	return ret;
}