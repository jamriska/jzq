// This software is in the public domain. Where that dedication is not
// recognized, you are granted a perpetual, irrevocable license to copy
// and modify this file as you see fit.

#ifndef JZQ_GL_H_
#define JZQ_GL_H_

#include <vector>
#include "jzq.h"

template<GLenum TARGET,typename GLTexture>
class GLTextureBase
{
public:
  GLTexture& operator=(GLTextureBase&& t);

  GLTextureBase(const GLTextureBase& t) = delete;
  GLTexture& operator=(const GLTextureBase& t) = delete;

  GLTexture& bind(GLint unit=0);
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
  GLTextureBase(GLTextureBase&& t);
  ~GLTextureBase();

  GLint   getTexLevelParameteri(GLint level,GLenum pname);
  GLfloat getTexLevelParameterf(GLint level,GLenum pname);

  static GLenum formatFor(GLint internalFormat);
  static GLenum typeFor(GLint internalFormat);

private:
  GLuint _id;
  void init();
};

class GLTexture1D : public GLTextureBase<GL_TEXTURE_1D,GLTexture1D>
{
public:
  GLTexture1D();
  GLTexture1D(GLTexture1D&& t);
  GLTexture1D& operator=(GLTexture1D&& t);

  GLTexture1D(const GLTexture1D& t) = delete;
  GLTexture1D& operator=(const GLTexture1D& t) = delete;

  GLTexture1D(GLint internalFormat,int width,void* data=0);
  GLTexture1D(GLint internalFormat,int width,GLenum format,void* data);
  GLTexture1D(GLint internalFormat,int width,GLenum format,GLenum type,void* data);

  template<typename T> explicit GLTexture1D(const std::vector<T>& image);
  template<typename T> GLTexture1D(GLint internalFormat,const std::vector<T>& image);
  template<typename T> GLTexture1D(GLint internalFormat,GLenum format,const std::vector<T>& image);

  GLTexture1D& setWrap(GLint wrapS);

  GLint width(GLint level=0);

private:
  void init1D(GLint internalFormat,int width,GLenum format,GLenum type,void* data);
};

class GLTexture2D : public GLTextureBase<GL_TEXTURE_2D,GLTexture2D>
{
public:
  GLTexture2D();
  GLTexture2D(GLTexture2D&& t);
  GLTexture2D& operator=(GLTexture2D&& t);

  GLTexture2D(const GLTexture2D& t) = delete;
  GLTexture2D& operator=(const GLTexture2D& t) = delete;

  GLTexture2D(GLint internalFormat,int width,int height,void* data=0);
  GLTexture2D(GLint internalFormat,int width,int height,GLenum format,void* data);
  GLTexture2D(GLint internalFormat,int width,int height,GLenum format,GLenum type,void* data);

  template<typename T> explicit GLTexture2D(const Array2<T>& image);
  template<typename T> GLTexture2D(GLint internalFormat,const Array2<T>& image);
  template<typename T> GLTexture2D(GLint internalFormat,GLenum format,const Array2<T>& image);

  GLTexture2D& setWrap(GLint wrapST);
  GLTexture2D& setWrap(GLint wrapS,GLint wrapT);

  GLint width(GLint level=0);
  GLint height(GLint level=0);

private:
  void init2D(GLint internalFormat,int width,int height,GLenum format,GLenum type,void* data);
};

class GLTexture3D : public GLTextureBase<GL_TEXTURE_3D,GLTexture3D>
{
public:
  GLTexture3D();
  GLTexture3D(GLTexture3D&& t);
  GLTexture3D& operator=(GLTexture3D&& t);

  GLTexture3D(const GLTexture3D& t) = delete;
  GLTexture3D& operator=(const GLTexture3D& t) = delete;

  GLTexture3D(GLint internalFormat,int width,int height,int depth,void* data=0);
  GLTexture3D(GLint internalFormat,int width,int height,int depth,GLenum format,void* data);
  GLTexture3D(GLint internalFormat,int width,int height,int depth,GLenum format,GLenum type,void* data);

  template<typename T> explicit GLTexture3D(const Array3<T>& image);
  template<typename T> GLTexture3D(GLint internalFormat,const Array3<T>& image);
  template<typename T> GLTexture3D(GLint internalFormat,GLenum format,const Array3<T>& image);

  GLTexture3D& setWrap(GLint wrapSTR);
  GLTexture3D& setWrap(GLint wrapS,GLint wrapT,GLint wrapR);

  GLint width(GLint level=0);
  GLint height(GLint level=0);
  GLint depth(GLint level=0);

private:
  void init3D(GLint internalFormat,int width,int height,int depth,GLenum format,GLenum type,void* data);
};

class GLShader
{
public:
  GLShader();
  GLShader(GLShader&& shader);
  GLShader& operator=(GLShader&& shader);
  ~GLShader();

  GLShader(const GLShader& shader) = delete;
  GLShader& operator=(const GLShader& shader) = delete;

  GLShader(const std::string& vertexShaderSource,
           const std::string& fragmentShaderSource);

  GLShader& use();
  GLint linkStatus();
  std::string infoLog();

  GLShader& setUniform(const std::string& name,GLint value);
  GLShader& setUniform(const std::string& name,GLfloat value);

  GLShader& bindTexture(const std::string& samplerName,GLTexture1D& texture);
  GLShader& bindTexture(const std::string& samplerName,GLTexture2D& texture);
  GLShader& bindTexture(const std::string& samplerName,GLTexture3D& texture);

private:
  GLuint _id;
  GLint _linkStatus;
  std::string _infoLog;
  GLuint compileShader(GLenum type,const std::string& source);

  struct Sampler
  {
    std::string name;
    GLenum type;
    GLint unit;

    Sampler() {}
    Sampler(const std::string& name,GLenum type,GLint unit) : name(name), type(type), unit(unit) {}
  };

  std::vector<Sampler> _samplers;
  int samplerIndexForName(const std::string& name);

  template<typename GLTexture,GLenum SAMPLER_TYPE>
  GLShader& _bindTexture(const std::string& samplerName,GLTexture& texture);
};

GLShader GLShaderFromFile(const std::string& vertexShaderFileName,
                          const std::string& fragmentShaderFileName);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<GLenum TARGET,typename GLTexture>
GLTextureBase<TARGET,GLTexture>::GLTextureBase() : _id(0) {}

template<GLenum TARGET,typename GLTexture>
GLTextureBase<TARGET,GLTexture>::GLTextureBase(GLTextureBase<TARGET,GLTexture>&& t)
{
  _id = t._id;
  t._id = 0;
}

template<GLenum TARGET,typename GLTexture>
GLTexture& GLTextureBase<TARGET,GLTexture>::operator=(GLTextureBase<TARGET,GLTexture>&& t)
{
  if (_id!=0) { glDeleteTextures(1,&_id); }
  _id = 0;
  _id = t._id;
  t._id = 0;
  return static_cast<GLTexture&>(*this);
}

template<GLenum TARGET,typename GLTexture>
GLTextureBase<TARGET,GLTexture>::~GLTextureBase()
{
  if (_id!=0) { glDeleteTextures(1,&_id); }
}

template<GLenum TARGET,typename GLTexture>
void GLTextureBase<TARGET,GLTexture>::init()
{
  glGenTextures(1,&_id);
}

template<GLenum TARGET,typename GLTexture>
GLuint GLTextureBase<TARGET,GLTexture>::id()
{
  if (_id==0) { init(); }
  return _id;
}

template<GLenum TARGET,typename GLTexture>
GLTexture& GLTextureBase<TARGET,GLTexture>::bind(GLint unit)
{
  if (_id==0) { init(); }
  glActiveTexture(GL_TEXTURE0 + unit);
  glBindTexture(TARGET,_id);
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

    case GL_DEPTH_COMPONENT16:  return GL_UNSIGNED_SHORT;
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

GLTexture2D::GLTexture2D() {}
GLTexture2D::GLTexture2D(GLTexture2D&& t) : GLTextureBase(std::move(t)) {}
GLTexture2D& GLTexture2D::operator=(GLTexture2D&& t) { return GLTextureBase::operator=(std::move(t)); }

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

GLShader::GLShader() : _id(0),_linkStatus(GL_FALSE),_infoLog("") {}

GLShader::GLShader(GLShader&& shader)
{
  _id = shader._id;
  _linkStatus = shader._linkStatus;
  _infoLog = std::move(shader._infoLog);
  _samplers = std::move(shader._samplers);
  shader._id = 0;
  shader._linkStatus = GL_FALSE;
}

GLShader& GLShader::operator=(GLShader&& shader)
{
  if (_id!=0) { glDeleteProgram(_id); }
  _id = 0;
  _id = shader._id;
  _linkStatus = shader._linkStatus;
  _infoLog = std::move(shader._infoLog);
  _samplers = std::move(shader._samplers);
  shader._id = 0;
  shader._linkStatus = GL_FALSE;
  return *this;
}

GLShader::~GLShader()
{
  if (_id!=0) { glDeleteProgram(_id); }
}

GLuint GLShader::compileShader(GLenum type,const std::string& source)
{
  const GLuint shader = glCreateShader(type);

  const GLchar* shaderSource = (const GLchar*)source.c_str();
  glShaderSource(shader,1,&shaderSource,0);

  glCompileShader(shader);

  return shader;
}

GLShader::GLShader(const std::string& vertexShaderSource,
                   const std::string& fragmentShaderSource)
: _id(0),_linkStatus(GL_FALSE),_infoLog("")
{
  const GLuint vertexShader = compileShader(GL_VERTEX_SHADER,vertexShaderSource);
  const GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER,fragmentShaderSource);

  const GLuint program = glCreateProgram();

  glAttachShader(program,vertexShader);
  glAttachShader(program,fragmentShader);

  glLinkProgram(program);

  glGetProgramiv(program,GL_LINK_STATUS,&_linkStatus);

  GLint logLength = 0;
  glGetProgramiv(program,GL_INFO_LOG_LENGTH,&logLength);

  if (logLength>0)
  {
    _infoLog.resize(logLength);
    glGetProgramInfoLog(program,logLength,&logLength,&_infoLog[0]);
    _infoLog.pop_back();
  }

  glDetachShader(program,vertexShader);
  glDetachShader(program,fragmentShader);

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  {
    GLint numUniforms = 0;
    glGetProgramiv(program,GL_ACTIVE_UNIFORMS,&numUniforms);

    GLint maxLength = 0;
    glGetProgramiv(program,GL_ACTIVE_UNIFORM_MAX_LENGTH,&maxLength);

    GLint unit = 0;
    for(int index=0;index<numUniforms;index++)
    {
      std::string name(maxLength+1,0); // XXX
      GLint arraySize = 0;
      GLenum type = 0;
      GLsizei actualLength = 0;
      glGetActiveUniform(program,index,maxLength,&actualLength,&arraySize,&type,&name[0]);
      name.resize(actualLength);
      if (type==GL_SAMPLER_1D ||
          type==GL_SAMPLER_2D ||
          type==GL_SAMPLER_3D ||
          type==GL_SAMPLER_CUBE)
      {
        _samplers.push_back(Sampler(name,type,unit));
        unit++;
      }
    }
  }

  _id = program;
}

GLShader& GLShader::use()
{
  glUseProgram(_id);
  return *this;
}

GLint GLShader::linkStatus()
{
  return _linkStatus;
}

std::string GLShader::infoLog()
{
  return _infoLog;
}

int GLShader::samplerIndexForName(const std::string& name)
{
  for(int i=0;i<_samplers.size();i++)
  {
    if (_samplers[i].name==name) { return i; }
  }
  return -1;
}

GLShader& GLShader::setUniform(const std::string& name,GLint value)
{
  use();
  // XXX: check uniform exists & type matches
  glUniform1i(glGetUniformLocation(_id,name.c_str()),value);
  return *this;
}

GLShader& GLShader::setUniform(const std::string& name,GLfloat value)
{
  use();
  // XXX: check uniform exists & type matches
  glUniform1f(glGetUniformLocation(_id,name.c_str()),value);
  return *this;
}

template<typename GLTexture,GLenum SAMPLER_TYPE>
GLShader& GLShader::_bindTexture(const std::string& samplerName,GLTexture& texture)
{
  const int samplerIndex = samplerIndexForName(samplerName);

  if (samplerIndex==-1) { printf("sampler %s does not exist!\n",samplerName.c_str()); return *this; }

  const Sampler& sampler = _samplers[samplerIndex];

  if (sampler.type!=SAMPLER_TYPE) { printf("wrong sampler type\n"); }

  texture.bind(sampler.unit);
  setUniform(samplerName,sampler.unit);

  return *this;
}

GLShader& GLShader::bindTexture(const std::string& samplerName,GLTexture1D& texture) { return _bindTexture<GLTexture1D,GL_SAMPLER_1D>(samplerName,texture); }
GLShader& GLShader::bindTexture(const std::string& samplerName,GLTexture2D& texture) { return _bindTexture<GLTexture2D,GL_SAMPLER_2D>(samplerName,texture); }
GLShader& GLShader::bindTexture(const std::string& samplerName,GLTexture3D& texture) { return _bindTexture<GLTexture3D,GL_SAMPLER_3D>(samplerName,texture); }

namespace jzq_detail
{
  inline std::string stringFromFile(const std::string& fileName)
  {
    FILE* f = jzq_fopen(fileName.c_str(),"rb");
    if (!f) { return std::string(); }
    fseek(f,0,SEEK_END);
    const int size = ftell(f);
    rewind(f);
    std::string content(size,'\0');
    if (fread(&content[0],sizeof(char),size,f)!=size) { content.clear(); }
    fclose(f);
    return content;
  }
}

GLShader GLShaderFromFile(const std::string& vertexShaderFileName,
                          const std::string& fragmentShaderFileName)
{
  return GLShader(jzq_detail::stringFromFile(vertexShaderFileName),
                  jzq_detail::stringFromFile(fragmentShaderFileName));
}

#endif
