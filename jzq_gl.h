// This software is in the public domain. Where that dedication is not
// recognized, you are granted a perpetual, irrevocable license to copy
// and modify this file as you see fit.

#ifndef JZQ_GL_H_
#define JZQ_GL_H_

#include "jzq.h"

// * srgb handling
// * pack alignment
// * auto mip?

template<GLenum TARGET,typename GLTexture>
class GLTextureBase
{
public:
  GLTexture& operator=(const GLTexture& t);

  GLTexture& bind();
  GLTexture& setParameter(GLenum name,GLint param);
  GLTexture& setParameter(GLenum name,GLfloat param);
  GLTexture& setMinFilter(GLint param);
  GLTexture& setMagFilter(GLint param);
  GLTexture& setMinLod(GLfloat param);
  GLTexture& setMaxLod(GLfloat param);
  GLTexture& setBaseLevel(GLint param);
  GLTexture& setMaxLevel(GLint param);
  GLTexture& setLodBias(GLfloat param);
  GLTexture& generateMipmap();
  GLuint id();

protected:
  GLTextureBase();
  GLTextureBase(const GLTexture& t);
  ~GLTextureBase();

private:
  GLuint texture;
  int* refCount;
  void init();
  static void destroy(GLuint& texture,int* refCount);
};

/*
class GLTexture1D : public GLTextureBase<GL_TEXTURE_1D,GL>
{
public:
  GLTexture1D();
  GLTexture1D(const GLTextureBase<GL_TEXTURE_1D>& t);
  GLTexture1D(int width);
};
*/

class GLTexture2D : public GLTextureBase<GL_TEXTURE_2D,GLTexture2D>
{
public:
  GLTexture2D();
  GLTexture2D(const GLTexture2D& t);
  //GLTexture2D(GLint internalFormat,int width,int height);
  //GLTexture2D(GLint internalFormat,int width,int height,void* data);
  template<typename T> GLTexture2D(const Array2<T>& image);
  template<typename T> GLTexture2D(GLint internalFormat,const Array2<T>& image);
  
  void setTexImage(GLint internalFormat,int width,int height,GLenum format,GLenum type,void* data);

  GLTexture2D& setWrap(GLint wrapST);
  GLTexture2D& setWrap(GLint wrapS,GLint wrapT);
};

/*
class GLTexture3D : public GLTextureBase<GL_TEXTURE_3D>
{
public:
  GLTexture3D();
  GLTexture3D(const GLTextureBase<GL_TEXTURE_3D>& t);
  GLTexture3D(int width,int height,int depth);
  template<typename T> GLTexture3D(const Array3<T>& image);
};
*/

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<GLenum TARGET,typename GLTexture>
GLTextureBase<TARGET,GLTexture>::GLTextureBase() : texture(0),refCount(0)
{
}

template<GLenum TARGET,typename GLTexture>
GLTextureBase<TARGET,GLTexture>::GLTextureBase(const GLTexture& t)
{
  texture = t.texture;
  refCount = t.refCount;
  if (refCount!=0) { (*refCount)++; }
}

template<GLenum TARGET,typename GLTexture>
void GLTextureBase<TARGET,GLTexture>::destroy(GLuint& texture,int* refCount)
{
  if (refCount!=0)
  {
    (*refCount)--;
    if (*refCount==0)
    {
      delete refCount;
      
      glDeleteTextures(1,&texture);     
      
      texture = 0;
    }
  }
}

template<GLenum TARGET,typename GLTexture>
GLTexture& GLTextureBase<TARGET,GLTexture>::operator=(const GLTexture& t)
{
  if (&t!=this)
  {
    int* orgRefCount = refCount;
    GLuint orgTexture = texture;

    texture = t.texture;
    refCount = t.refCount;
    if (refCount!=0) { (*refCount)++; }

    destroy(orgTexture,orgRefCount);
  }

  return static_cast<GLTexture>(*this);
}

template<GLenum TARGET,typename GLTexture>
GLTextureBase<TARGET,GLTexture>::~GLTextureBase()
{
  destroy(texture,refCount);
}

template<GLenum TARGET,typename GLTexture>
void GLTextureBase<TARGET,GLTexture>::init()
{
  glGenTextures(1,&texture);  
  refCount = new int;
  *refCount = 1;
}

template<GLenum TARGET,typename GLTexture>
GLuint GLTextureBase<TARGET,GLTexture>::id()
{
  if (texture==0) { init(); }
  return texture;
}

template<GLenum TARGET,typename GLTexture>
GLTexture& GLTextureBase<TARGET,GLTexture>::bind()
{
  if (texture==0) { init(); }
  glBindTexture(TARGET,texture);
  return static_cast<GLTexture&>(*this);
}

template<GLenum TARGET,typename GLTexture>
GLTexture& GLTextureBase<TARGET,GLTexture>::setParameter(GLenum name,GLint param)
{
  bind();
  glTexParameteri(TARGET,name,param);
  return static_cast<GLTexture&>(*this);
}

template<GLenum TARGET,typename GLTexture>
GLTexture& GLTextureBase<TARGET,GLTexture>::setParameter(GLenum name,GLfloat param)
{
  bind();
  glTexParameterf(TARGET,name,param);
  return static_cast<GLTexture&>(*this);
}

template<GLenum TARGET,typename GLTexture> GLTexture& GLTextureBase<TARGET,GLTexture>::setMinFilter(GLint param) { return setParameter(GL_TEXTURE_MIN_FILTER,param); }
template<GLenum TARGET,typename GLTexture> GLTexture& GLTextureBase<TARGET,GLTexture>::setMagFilter(GLint param) { return setParameter(GL_TEXTURE_MAG_FILTER,param); }
template<GLenum TARGET,typename GLTexture> GLTexture& GLTextureBase<TARGET,GLTexture>::setMinLod(GLfloat param) { return setParameter(GL_TEXTURE_MIN_LOD,param); }
template<GLenum TARGET,typename GLTexture> GLTexture& GLTextureBase<TARGET,GLTexture>::setMaxLod(GLfloat param) { return setParameter(GL_TEXTURE_MAX_LOD,param); }
template<GLenum TARGET,typename GLTexture> GLTexture& GLTextureBase<TARGET,GLTexture>::setBaseLevel(GLint param) { return setParameter(GL_TEXTURE_BASE_LEVEL,param); }
template<GLenum TARGET,typename GLTexture> GLTexture& GLTextureBase<TARGET,GLTexture>::setMaxLevel(GLint param) { return setParameter(GL_TEXTURE_MAX_LEVEL,param); }
template<GLenum TARGET,typename GLTexture> GLTexture& GLTextureBase<TARGET,GLTexture>::setLodBias(GLfloat param) { return setParameter(GL_TEXTURE_LOD_BIAS,param); }

template<GLenum TARGET,typename GLTexture> GLTexture& GLTextureBase<TARGET,GLTexture>::generateMipmap()
{
  bind();
  glGenerateMipMap(TARGET);
  return static_cast<GLTexture&>(*this);
}

//GLTexture1D::GLTexture1D() : GLTextureBase<GL_TEXTURE_1D>() {}
//GLTexture1D::GLTexture1D(const GLTextureBase<GL_TEXTURE_1D>& t) : GLTextureBase<GL_TEXTURE_1D>(t) {}

GLTexture2D::GLTexture2D() : GLTextureBase<GL_TEXTURE_2D,GLTexture2D>() {}
GLTexture2D::GLTexture2D(const GLTexture2D& t) : GLTextureBase<GL_TEXTURE_2D,GLTexture2D>(t) {}

// XXX how to hide this?
template<typename T>
struct GLFormat { };

template<> struct GLFormat<unsigned char> { static const GLint internalFormat = GL_R8;      static const GLenum format = GL_RED;  static const GLenum type = GL_UNSIGNED_BYTE; };
template<> struct GLFormat<Vec2uc>        { static const GLint internalFormat = GL_RG8;     static const GLenum format = GL_RG;   static const GLenum type = GL_UNSIGNED_BYTE; };
template<> struct GLFormat<Vec3uc>        { static const GLint internalFormat = GL_RGB8;    static const GLenum format = GL_RGB;  static const GLenum type = GL_UNSIGNED_BYTE; };
template<> struct GLFormat<Vec4uc>        { static const GLint internalFormat = GL_RGBA8;   static const GLenum format = GL_RGBA; static const GLenum type = GL_UNSIGNED_BYTE; };

template<> struct GLFormat<int>           { static const GLint internalFormat = GL_R32I;    static const GLenum format = GL_RED;  static const GLenum type = GL_INT;           };
template<> struct GLFormat<Vec2i>         { static const GLint internalFormat = GL_RG32I;   static const GLenum format = GL_RG;   static const GLenum type = GL_INT;           };
template<> struct GLFormat<Vec3i>         { static const GLint internalFormat = GL_RGB32I;  static const GLenum format = GL_RGB;  static const GLenum type = GL_INT;           };
template<> struct GLFormat<Vec4i>         { static const GLint internalFormat = GL_RGBA32I; static const GLenum format = GL_RGBA; static const GLenum type = GL_INT;           };

template<> struct GLFormat<float>         { static const GLint internalFormat = GL_R32F;    static const GLenum format = GL_RED;  static const GLenum type = GL_FLOAT;         };
template<> struct GLFormat<Vec2f>         { static const GLint internalFormat = GL_RG32F;   static const GLenum format = GL_RG;   static const GLenum type = GL_FLOAT;         };
template<> struct GLFormat<Vec3f>         { static const GLint internalFormat = GL_RGB32F;  static const GLenum format = GL_RGB;  static const GLenum type = GL_FLOAT;         };
template<> struct GLFormat<Vec4f>         { static const GLint internalFormat = GL_RGBA32F; static const GLenum format = GL_RGBA; static const GLenum type = GL_FLOAT;         };

/*
GLTexture2D::GLTexture2D(GLint format,int width,int height)
{
  bind();

  // figure out how to derive compatible format for src
  //glTexImage2D(GL_TEXTURE_2D,0,format,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,0);
  
  setWrapS(GL_CLAMP_TO_EDGE);
  setWrapT(GL_CLAMP_TO_EDGE);
  setMinFilter(GL_NEAREST);
  setMagFilter(GL_NEAREST);  
}
*/

template<typename T>
GLTexture2D::GLTexture2D(const Array2<T>& image)
{
  setTexImage(GLFormat<T>::internalFormat,image.width(),image.height(),GLFormat<T>::format,GLFormat<T>::type,(void*)image.data());
  setWrap(GL_CLAMP_TO_EDGE);
  setMinFilter(GL_NEAREST);
  setMagFilter(GL_NEAREST);
}

template<typename T>
GLTexture2D::GLTexture2D(GLint internalFormat,const Array2<T>& image)
{
  setTexImage(internalFormat,image.width(),image.height(),GLFormat<T>::format,GLFormat<T>::type,(void*)image.data());
  setWrap(GL_CLAMP_TO_EDGE);
  setMinFilter(GL_NEAREST);
  setMagFilter(GL_NEAREST);
}

void GLTexture2D::setTexImage(GLint internalFormat,int width,int height,GLenum format,GLenum type,void* data)
{
  bind();
  glTexImage2D(GL_TEXTURE_2D,0,internalFormat,width,height,0,format,type,data);
}

GLTexture2D& GLTexture2D::setWrap(GLint wrapS,GLint wrapT)
{
  setParameter(GL_TEXTURE_WRAP_S,wrapS);
  setParameter(GL_TEXTURE_WRAP_T,wrapT);
  return *this;
}

GLTexture2D& GLTexture2D::setWrap(GLint wrapST)
{
  return setWrap(wrapST,wrapST);
}

//GLTexture3D::GLTexture3D() : GLTextureBase<GL_TEXTURE_3D>() {}
//GLTexture3D::GLTexture3D(const GLTextureBase<GL_TEXTURE_3D>& t) : GLTextureBase<GL_TEXTURE_3D>(t) {}

#endif
