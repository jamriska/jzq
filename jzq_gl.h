// This software is in the public domain. Where that dedication is not
// recognized, you are granted a perpetual, irrevocable license to copy
// and modify this file as you see fit.

#ifndef JZQ_GL_H_
#define JZQ_GL_H_

#include "jzq.h"

template<GLenum TARGET,typename GLTexture>
class GLTextureBase
{
public:
  GLTexture& operator=(const GLTextureBase& t);

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
 
  GLint internalFormat(GLint level=0);

protected:
  GLTextureBase();
  GLTextureBase(const GLTexture& t);
  ~GLTextureBase();

  GLint   getTexLevelParameteri(GLint level,GLenum pname);
  GLfloat getTexLevelParameterf(GLint level,GLenum pname);
 
  static GLenum formatFor(GLint internalFormat);
  static GLenum typeFor(GLint internalFormat);

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
  
  GLTexture2D(GLint internalFormat,int width,int height);
  GLTexture2D(GLint internalFormat,int width,int height,void* data);
  GLTexture2D(GLint internalFormat,int width,int height,GLenum format,void* data);
  GLTexture2D(GLint internalFormat,int width,int height,GLenum format,GLenum type,void* data);
  
  template<typename T> GLTexture2D(const Array2<T>& image);
  template<typename T> GLTexture2D(GLint internalFormat,const Array2<T>& image);
  template<typename T> GLTexture2D(GLint internalFormat,GLenum format,const Array2<T>& image);
  
  GLTexture2D& setWrap(GLint wrapST);
  GLTexture2D& setWrap(GLint wrapS,GLint wrapT);

  GLint width(GLint level=0);
  GLint height(GLint level=0);

private:
  void init2D(GLint internalFormat,int width,int height,GLenum format,GLenum type,void* data);
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
GLTexture& GLTextureBase<TARGET,GLTexture>::operator=(const GLTextureBase<TARGET,GLTexture>& t)
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

  return static_cast<GLTexture&>(*this);
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

template<GLenum TARGET,typename GLTexture>
GLint GLTextureBase<TARGET,GLTexture>::getTexLevelParameteri(GLint level,GLenum pname)
{
  bind();
  GLint param = GL_INVALID_VALUE;
  glGetTexLevelParameteriv(TARGET,level,pname,&param);
  return param;
}

template<GLenum TARGET,typename GLTexture>
GLfloat GLTextureBase<TARGET,GLTexture>::getTexLevelParameterf(GLint level,GLenum pname)
{
  bind();
  GLfloat param = GL_INVALID_VALUE;
  glGetTexLevelParameterfv(TARGET,level,pname,&param);
  return param;
}

template<GLenum TARGET,typename GLTexture>
GLint GLTextureBase<TARGET,GLTexture>::internalFormat(GLint level)
{
  return getTexLevelParameteri(level,GL_TEXTURE_INTERNAL_FORMAT);
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

template<GLenum TARGET,typename GLTexture>
GLenum GLTextureBase<TARGET,GLTexture>::formatFor(GLint internalFormat)
{
  switch(internalFormat)
  {
    case GL_R8:                 return GL_RED;
    case GL_RG8:                return GL_RG;
    case GL_RGB8:               return GL_RGB;
    case GL_RGBA8:              return GL_RGBA;

    case GL_R8I:                return GL_RED;
    case GL_RG8I:               return GL_RG;
    case GL_RGB8I:              return GL_RGB;
    case GL_RGBA8I:             return GL_RGBA;

    case GL_R8UI:               return GL_RED;
    case GL_RG8UI:              return GL_RG;
    case GL_RGB8UI:             return GL_RGB;
    case GL_RGBA8UI:            return GL_RGBA;

    case GL_R16I:               return GL_RED;
    case GL_RG16I:              return GL_RG;
    case GL_RGB16I:             return GL_RGB;
    case GL_RGBA16I:            return GL_RGBA;

    case GL_R16UI:              return GL_RED;
    case GL_RG16UI:             return GL_RG;
    case GL_RGB16UI:            return GL_RGB;
    case GL_RGBA16UI:           return GL_RGBA;
    
    case GL_R32I:               return GL_RED;
    case GL_RG32I:              return GL_RG;
    case GL_RGB32I:             return GL_RGB;
    case GL_RGBA32I:            return GL_RGBA;

    case GL_R32UI:              return GL_RED;
    case GL_RG32UI:             return GL_RG;
    case GL_RGB32UI:            return GL_RGB;
    case GL_RGBA32UI:           return GL_RGBA;

    case GL_R32F:               return GL_RED;
    case GL_RG32F:              return GL_RG;
    case GL_RGB32F:             return GL_RGB;
    case GL_RGBA32F:            return GL_RGBA;

    case GL_SRGB8:              return GL_RGB;
    case GL_SRGB8_ALPHA8:       return GL_RGBA;

    case GL_DEPTH_COMPONENT16:  return GL_DEPTH_COMPONENT;
    case GL_DEPTH_COMPONENT24:  return GL_DEPTH_COMPONENT;
    case GL_DEPTH_COMPONENT32:  return GL_DEPTH_COMPONENT;
    case GL_DEPTH_COMPONENT32F: return GL_DEPTH_COMPONENT;
  }

  return GL_INVALID_VALUE;
}

template<GLenum TARGET,typename GLTexture>
GLenum GLTextureBase<TARGET,GLTexture>::typeFor(GLint internalFormat)
{
  switch(internalFormat)
  {
    case GL_R8:                 return GL_UNSIGNED_BYTE;
    case GL_RG8:                return GL_UNSIGNED_BYTE;
    case GL_RGB8:               return GL_UNSIGNED_BYTE;
    case GL_RGBA8:              return GL_UNSIGNED_BYTE;
    
    case GL_R8I:                return GL_BYTE;
    case GL_RG8I:               return GL_BYTE;
    case GL_RGB8I:              return GL_BYTE;
    case GL_RGBA8I:             return GL_BYTE;

    case GL_R8UI:               return GL_UNSIGNED_BYTE;
    case GL_RG8UI:              return GL_UNSIGNED_BYTE;
    case GL_RGB8UI:             return GL_UNSIGNED_BYTE;
    case GL_RGBA8UI:            return GL_UNSIGNED_BYTE;

    case GL_R16I:               return GL_SHORT;
    case GL_RG16I:              return GL_SHORT;
    case GL_RGB16I:             return GL_SHORT;
    case GL_RGBA16I:            return GL_SHORT;

    case GL_R16UI:              return GL_UNSIGNED_SHORT;
    case GL_RG16UI:             return GL_UNSIGNED_SHORT;
    case GL_RGB16UI:            return GL_UNSIGNED_SHORT;
    case GL_RGBA16UI:           return GL_UNSIGNED_SHORT;

    case GL_R32I:               return GL_INT;
    case GL_RG32I:              return GL_INT;
    case GL_RGB32I:             return GL_INT;
    case GL_RGBA32I:            return GL_INT;

    case GL_R32UI:              return GL_UNSIGNED_INT;
    case GL_RG32UI:             return GL_UNSIGNED_INT;
    case GL_RGB32UI:            return GL_UNSIGNED_INT;
    case GL_RGBA32UI:           return GL_UNSIGNED_INT;
    
    case GL_R32F:               return GL_FLOAT;
    case GL_RG32F:              return GL_FLOAT;
    case GL_RGB32F:             return GL_FLOAT;
    case GL_RGBA32F:            return GL_FLOAT;

    case GL_SRGB8:              return GL_UNSIGNED_BYTE;
    case GL_SRGB8_ALPHA8:       return GL_UNSIGNED_BYTE;

    case GL_DEPTH_COMPONENT16:  return GL_UNSIGNED_INT;
    case GL_DEPTH_COMPONENT24:  return GL_UNSIGNED_INT;
    case GL_DEPTH_COMPONENT32:  return GL_UNSIGNED_INT;
    case GL_DEPTH_COMPONENT32F: return GL_FLOAT;
  }

  return GL_INVALID_VALUE;
}

template<typename T>
struct GLInternalFormatFor { };

template<> struct GLInternalFormatFor<unsigned char> { static const GLint value = GL_R8;      };
template<> struct GLInternalFormatFor<Vec2uc>        { static const GLint value = GL_RG8;     };
template<> struct GLInternalFormatFor<Vec3uc>        { static const GLint value = GL_RGB8;    };
template<> struct GLInternalFormatFor<Vec4uc>        { static const GLint value = GL_RGBA8;   };

template<> struct GLInternalFormatFor<int>           { static const GLint value = GL_R32I;    };
template<> struct GLInternalFormatFor<Vec2i>         { static const GLint value = GL_RG32I;   };
template<> struct GLInternalFormatFor<Vec3i>         { static const GLint value = GL_RGB32I;  };
template<> struct GLInternalFormatFor<Vec4i>         { static const GLint value = GL_RGBA32I; };

template<> struct GLInternalFormatFor<float>         { static const GLint value = GL_R32F;    };
template<> struct GLInternalFormatFor<Vec2f>         { static const GLint value = GL_RG32F;   };
template<> struct GLInternalFormatFor<Vec3f>         { static const GLint value = GL_RGB32F;  };
template<> struct GLInternalFormatFor<Vec4f>         { static const GLint value = GL_RGBA32F; };

//GLTexture1D::GLTexture1D() : GLTextureBase<GL_TEXTURE_1D>() {}
//GLTexture1D::GLTexture1D(const GLTextureBase<GL_TEXTURE_1D>& t) : GLTextureBase<GL_TEXTURE_1D>(t) {}

GLTexture2D::GLTexture2D() : GLTextureBase<GL_TEXTURE_2D,GLTexture2D>() {}
GLTexture2D::GLTexture2D(const GLTexture2D& t) : GLTextureBase<GL_TEXTURE_2D,GLTexture2D>(t) {}

void GLTexture2D::init2D(GLint internalFormat,int width,int height,GLenum format,GLenum type,void* data)
{
  bind();
  glPixelStorei(GL_UNPACK_SWAP_BYTES,GL_FALSE);
  glPixelStorei(GL_UNPACK_LSB_FIRST,GL_FALSE);
  glPixelStorei(GL_UNPACK_ROW_LENGTH,0);
  glPixelStorei(GL_UNPACK_IMAGE_HEIGHT,0);
  glPixelStorei(GL_UNPACK_SKIP_ROWS,0);
  glPixelStorei(GL_UNPACK_SKIP_PIXELS,0);
  glPixelStorei(GL_UNPACK_SKIP_IMAGES,0);
  glPixelStorei(GL_UNPACK_ALIGNMENT,1);
  glTexImage2D(GL_TEXTURE_2D,0,internalFormat,width,height,0,format,type,data);
  setWrap(GL_CLAMP_TO_EDGE);
  setMinFilter(GL_NEAREST);
  setMagFilter(GL_NEAREST);
}

GLTexture2D::GLTexture2D(GLint internalFormat,int width,int height)
{
  init2D(internalFormat,width,height,formatFor(internalFormat),typeFor(internalFormat),0);
}

GLTexture2D::GLTexture2D(GLint internalFormat,int width,int height,void* data)
{
  init2D(internalFormat,width,height,formatFor(internalFormat),typeFor(internalFormat),data);
}

GLTexture2D::GLTexture2D(GLint internalFormat,int width,int height,GLenum format,void* data)
{
  init2D(internalFormat,width,height,format,typeFor(internalFormat),data);
}

GLTexture2D::GLTexture2D(GLint internalFormat,int width,int height,GLenum format,GLenum type,void* data)
{
  init2D(internalFormat,width,height,format,type,data);
}

template<typename T>
GLTexture2D::GLTexture2D(const Array2<T>& image)
{
  init2D(GLInternalFormatFor<T>::value,image.width(),image.height(),formatFor(GLInternalFormatFor<T>::value),typeFor(GLInternalFormatFor<T>::value),(void*)image.data());
}

template<typename T>
GLTexture2D::GLTexture2D(GLint internalFormat,const Array2<T>& image)
{
  init2D(internalFormat,image.width(),image.height(),formatFor(GLInternalFormatFor<T>::value),typeFor(GLInternalFormatFor<T>::value),(void*)image.data());
}

template<typename T>
GLTexture2D::GLTexture2D(GLint internalFormat,GLenum format,const Array2<T>& image)
{
  init2D(internalFormat,image.width(),image.height(),format,typeFor(GLInternalFormatFor<T>::value),(void*)image.data());
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

GLint GLTexture2D::width(GLint level)
{
  return getTexLevelParameteri(level,GL_TEXTURE_WIDTH);
}

GLint GLTexture2D::height(GLint level)
{
  return getTexLevelParameteri(level,GL_TEXTURE_HEIGHT);
}

//GLTexture3D::GLTexture3D() : GLTextureBase<GL_TEXTURE_3D>() {}
//GLTexture3D::GLTexture3D(const GLTextureBase<GL_TEXTURE_3D>& t) : GLTextureBase<GL_TEXTURE_3D>(t) {}

#endif
