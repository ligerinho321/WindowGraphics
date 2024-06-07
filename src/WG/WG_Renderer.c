#include <WG/WG_Renderer.h>

#include <WG/OpenGL/glad.h>
#include <WG/OpenGL/glad_wgl.h>

#define STB_IMAGE_IMPLEMENTATION
#include <WG/STB/stb_image.h>

#define STB_TRUETYPE_IMPLEMENTATION
#include <WG/STB/stb_truetype.h>

#include <WG/DefaultShaders.h>
#include <WG/InlineFunctions.h>

#define M_PI 3.14159265358979323846

#define CircleSegmentAngleFactor 0.625f
#define GapExpandSize 2000
#define MAX_BUFFER 260

#define UnTextureAttributeSize 6
#define UnTexturePositionOffset 0
#define UnTextureColorOffset 2

#define TextureAttributeSize 8
#define TexturePositionOffset 0
#define TextureCoordOffset 2
#define TextureColorOffset 4

typedef enum _ShaderProgram{
    SHADER_PROGRAM_NONE    = 0x0000,
    SHADER_PROGRAM_SHAPE   = 0x0001,
    SHADER_PROGRAM_TEXTURE = 0x0002,
}ShaderProgram;


typedef struct _ViewState{
    Rect viewport;
    Rect cliprect;
    BlendMode blend_mode;
    bool use_blending;
    bool use_cliprect;
}ViewState;

typedef struct _ShapeShaderData{
    uint32_t vertex_shader;
    uint32_t fragment_shader;
    uint32_t program;
    int vertex_position;
    int vertex_color;
    int projection;
}ShapeShaderData;

typedef struct _TextureShaderData{
    uint32_t vertex_shader;
    uint32_t fragment_shader;
    uint32_t program;
    int vertex_position;
    int vertex_texture_coord;
    int vertex_color;
    int projection;
}TextureShaderData;

typedef struct _CharData{
    int   x,y,w,h;
    int   xadvance,glyph;
    int   xoffset,yoffset;
    float s0,t0,s1,t1;
    bool  isValid;
}CharData;


struct _WG_FontAtlas{
    WG_Texture *texture;
    CharData *char_data;
    uint32_t first_char;
    uint32_t num_chars;
    int baseLine;
    int height;
    float scale;
};

struct _WG_Font{
    unsigned char *data;
    void *info;
    int height;
    float scale;
    int ascent;
    int descent;
    int lineGap;
    int baseLine;
    bool allow_kerning;
};

struct _WG_Texture{
    WG_Renderer *renderer;
    uint8_t *pixels;
    Rect locked_rect;
    int pitch;
    int width;
    int height;
    int bytes_per_pixel;
    ViewState view;
    Color color;
    PixelFormat format;

    uint32_t wrap_x;
    uint32_t wrap_y;

    uint32_t min_filter;
    uint32_t mag_filter;

    bool use_mipmap;
    bool use_framebuffer;

    uint32_t texture_handle;
    uint32_t framebuffer_handle;
};

struct _WG_Renderer{

    WG_Window *window;
    HDC hdc;
    HGLRC hglrc;

    int window_width;
    int window_height;

    WG_Texture *current_target;
    ViewState *current_view;
    ShaderProgram current_shader_program;

    ViewState main_view;

    ShapeShaderData shape_shader_data;
    TextureShaderData texture_shader_data;

    uint32_t blit_VBO;
    uint32_t blit_VAO;
    uint32_t blit_EBO;

    GapBuffer *blit_buffer;
    GapBuffer *blit_index;

    uint32_t blit_buffer_max_size;
    uint32_t blit_index_max_size;
    uint32_t max_texture_size;

    uint32_t last_shape;
    WG_Texture *last_texture;
    Rect last_viewport;
    BlendMode last_blend_mode;
    bool last_use_blending;
    bool vsync;
};

static char buffer[MAX_BUFFER];


static void LoadFile(const uint16_t *fileName,uint8_t **data,uint32_t *len){
    if(!data) return;

    FILE *file = _wfopen(fileName,L"rb");

    if(!file){fprintf(stderr,"%s\n",strerror(errno)); return;}

    fseek(file,0,SEEK_END);
    uint32_t pos = ftell(file);
    fseek(file,0,SEEK_SET);

    *data = malloc(sizeof(uint8_t) * pos);

    fread(*data,sizeof(unsigned char),pos,file);

    if(len) *len = pos;

    fclose(file);
}

/////Debug Functions/////

void APIENTRY WG_DebugMessageCallback(GLenum source,GLenum type,GLuint id,GLenum severity,GLsizei length,const char *message,const void *userParam){
    fprintf(stderr,"-----------------------------------------------------------------------\n");
    
    switch(source){
        case GL_DEBUG_SOURCE_API:
            fprintf(stderr,"Source: GL_DEBUG_SOURCE_API\n");
            break;
        case GL_DEBUG_SOURCE_APPLICATION:
            fprintf(stderr,"Source: GL_DEBUG_SOURCE_APPLICATION\n");
            break;
        case GL_DEBUG_SOURCE_OTHER:
            fprintf(stderr,"Source: GL_DEBUG_SOURCE_OTHER\n");
            break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER:
            fprintf(stderr,"Source: GL_DEBUG_SOURCE_SHADER_COMPILER\n");
            break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:
            fprintf(stderr,"Source: GL_DEBUG_SOURCE_THIRD_PARTY\n");
            break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
            fprintf(stderr,"Source: GL_DEBUG_SOURCE_WINDOW_SYSTEM\n");
            break;
    }

    switch(type){
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
            fprintf(stderr,"Type: GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR\n");
            break;
        case GL_DEBUG_TYPE_ERROR:
            fprintf(stderr,"Type: GL_DEBUG_TYPE_ERROR\n");
            break;
        case GL_DEBUG_TYPE_MARKER:
            fprintf(stderr,"Type: GL_DEBUG_TYPE_MARKER\n");
            break;
        case GL_DEBUG_TYPE_OTHER:
            fprintf(stderr,"Type: GL_DEBUG_TYPE_OTHER\n");
            break;
        case GL_DEBUG_TYPE_PERFORMANCE:
            fprintf(stderr,"Type: GL_DEBUG_TYPE_PERFORMANCE\n");
            break;
        case GL_DEBUG_TYPE_POP_GROUP:
            fprintf(stderr,"Type: GL_DEBUG_TYPE_POP_GROUP\n");
            break;
        case GL_DEBUG_TYPE_PORTABILITY:
            fprintf(stderr,"Type: GL_DEBUG_TYPE_PORTABILITY\n");
            break;
        case GL_DEBUG_TYPE_PUSH_GROUP:
            fprintf(stderr,"Type: GL_DEBUG_TYPE_PUSH_GROUP\n");
            break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
            fprintf(stderr,"Type: GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR\n");
            break;
    }

    switch(severity){
        case GL_DEBUG_SEVERITY_HIGH:
            fprintf(stderr,"Severity: GL_DEBUG_SEVERITY_HIGH\n");
            break;
        case GL_DEBUG_SEVERITY_LOW:
            fprintf(stderr,"Severity: GL_DEBUG_SEVERITY_LOW\n");
            break;
        case GL_DEBUG_SEVERITY_MEDIUM:
            fprintf(stderr,"Severity: GL_DEBUG_SEVERITY_MEDIUM\n");
            break;
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            fprintf(stderr,"Severity: GL_DEBUG_SEVERITY_NOTIFICATION\n");
            break;
    }

    fprintf(stderr,"Message: %s\n",message);

    fprintf(stderr,"-----------------------------------------------------------------------\n");
}

void WG_SetDebugMessageCallback(GLDEBUGPROC callback,void *userparam){
    
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

    glDebugMessageCallback(callback,userparam);
}


/////Blend Functions/////

BlendMode WG_GetBlendModeByPreset(BlendModePreset preset){

    switch(preset){

        case BLEND_NORMAL:

            return (BlendMode){FUNC_SRC_ALPHA, FUNC_ONE_MINUS_SRC_ALPHA, FUNC_SRC_ALPHA, FUNC_ONE_MINUS_SRC_ALPHA, EQ_ADD, EQ_ADD};
        
        case BLEND_PREMULTIPLIED_ALPHA:
            
            return (BlendMode){FUNC_ONE, FUNC_ONE_MINUS_SRC_ALPHA, FUNC_ONE, FUNC_ONE_MINUS_SRC_ALPHA, EQ_ADD, EQ_ADD};
        
        case BLEND_MULTIPLY:
            
            return (BlendMode){FUNC_DST_COLOR, FUNC_ZERO, FUNC_SRC_ALPHA, FUNC_ONE_MINUS_SRC_ALPHA, EQ_ADD, EQ_ADD};
        
        case BLEND_ADD:
        
            return (BlendMode){FUNC_SRC_ALPHA, FUNC_ONE, FUNC_SRC_ALPHA, FUNC_ONE, EQ_ADD, EQ_ADD};
            
        case BLEND_SUBTRACT:
        
            return (BlendMode){FUNC_ONE, FUNC_ONE, FUNC_ONE, FUNC_ONE, EQ_SUBTRACT, EQ_SUBTRACT};
        
        case BLEND_MOD_ALPHA:
            
            return (BlendMode){FUNC_ZERO, FUNC_ONE, FUNC_ZERO, FUNC_SRC_ALPHA, EQ_ADD, EQ_ADD};
        
        case BLEND_SET_ALPHA:
            
            return (BlendMode){FUNC_ZERO, FUNC_ONE, FUNC_ONE, FUNC_ZERO, EQ_ADD, EQ_ADD};
        
        case BLEND_SET:
           
           return (BlendMode){FUNC_ONE, FUNC_ZERO, FUNC_ONE, FUNC_ZERO, EQ_ADD, EQ_ADD};
        
        case BLEND_NORMAL_KEEP_ALPHA:
           
           return (BlendMode){FUNC_SRC_ALPHA, FUNC_ONE_MINUS_SRC_ALPHA, FUNC_ZERO, FUNC_ONE, EQ_ADD, EQ_ADD};
        
        case BLEND_NORMAL_ADD_ALPHA:
           
           return (BlendMode){FUNC_SRC_ALPHA, FUNC_ONE_MINUS_SRC_ALPHA, FUNC_ONE, FUNC_ONE, EQ_ADD, EQ_ADD};
        
        case BLEND_NORMAL_FACTOR_ALPHA:
           
           return (BlendMode){FUNC_SRC_ALPHA, FUNC_ONE_MINUS_SRC_ALPHA, FUNC_ONE_MINUS_DST_ALPHA, FUNC_ONE, EQ_ADD, EQ_ADD};
        
        default:
           
           return (BlendMode){FUNC_SRC_ALPHA, FUNC_ONE_MINUS_SRC_ALPHA, FUNC_SRC_ALPHA, FUNC_ONE_MINUS_SRC_ALPHA, EQ_ADD, EQ_ADD};
    
    }
}

void WG_SetBlendFunction(WG_Renderer *renderer,WG_Texture *target,BlendFunction srcRGB,BlendFunction dstRGB,BlendFunction srcAlpha,BlendFunction dstAlpha){
    if(target != NULL){
        target->view.blend_mode.srcRGB = srcRGB;
        target->view.blend_mode.dstRGB = dstRGB;
        target->view.blend_mode.srcAlpha = srcAlpha;
        target->view.blend_mode.dstAlpha = dstAlpha;
    }
    else{
        renderer->main_view.blend_mode.srcRGB = srcRGB;
        renderer->main_view.blend_mode.dstRGB = dstRGB;
        renderer->main_view.blend_mode.srcAlpha = srcAlpha;
        renderer->main_view.blend_mode.dstAlpha = dstAlpha;
    }
}

void WG_SetBlendEquation(WG_Renderer *renderer,WG_Texture *target,BlendEquation modeRGB,BlendEquation modeAlpha){
    if(target != NULL){
        target->view.blend_mode.modeRGB = modeRGB;
        target->view.blend_mode.modeAlpha = modeAlpha;
    }
    else{
        renderer->main_view.blend_mode.modeRGB = modeRGB;
        renderer->main_view.blend_mode.modeAlpha = modeAlpha;
    }
}

void WG_SetBlendModeByPreset(WG_Renderer *renderer,WG_Texture *target,BlendModePreset preset){
    if(target != NULL){
        target->view.blend_mode = WG_GetBlendModeByPreset(preset);
    }
    else{
        renderer->main_view.blend_mode = WG_GetBlendModeByPreset(preset);
    }
}

void WG_SetBlendMode(WG_Renderer *renderer,WG_Texture *target,BlendMode blend_mode){
    if(target != NULL){
        target->view.blend_mode = blend_mode;
    }
    else{
        renderer->main_view.blend_mode = blend_mode;
    }
}

void WG_SetBlending(WG_Renderer *renderer,WG_Texture *target,bool blending){
    if(target != NULL){
        target->view.use_blending = blending;
    }
    else{
        renderer->main_view.use_blending = blending;
    }
}


void WG_SetClip(WG_Renderer *renderer,WG_Texture *target,Rect *rect){

    if(target == renderer->current_target){
        WG_FlushBlitBuffer(renderer);
    }

    if(target != NULL){
        target->view.use_cliprect = true;
        if(rect != NULL){
            target->view.cliprect = *rect;
        }
        else{
            target->view.cliprect = (Rect){0,0,target->width,target->height};
        }
    }
    else{
        renderer->main_view.use_cliprect = true;
        if(rect != NULL){
            renderer->main_view.cliprect = *rect;
        }
        else{
            renderer->main_view.cliprect = (Rect){0,0,renderer->window_width,renderer->window_height};
        }
    }
}

void WG_UnSetClip(WG_Renderer *renderer,WG_Texture *target){
    
    if(target == renderer->current_target){
        WG_FlushBlitBuffer(renderer);
    }

    if(target != NULL){
        target->view.use_cliprect = false;
    }
    else{
        renderer->main_view.use_cliprect = false;
    }
}


/////Texture Functions/////

PixelFormat WG_GetPixelFormat(int bytes_per_pixel){
    switch(bytes_per_pixel){
        case 1:  return PIXEL_FORMAT_LUMINANCE;
        case 2:  return PIXEL_FORMAT_LUMINANCE_ALPHA;
        case 3:  return PIXEL_FORMAT_RGB;
        case 4:  return PIXEL_FORMAT_RGBA;
        default: return 0;
    }
}

int WG_GetBytesPerPixel(PixelFormat format){
    switch(format){
        case PIXEL_FORMAT_RED:             return 1;
        case PIXEL_FORMAT_GREEN:           return 1;
        case PIXEL_FORMAT_BLUE:            return 1;
        case PIXEL_FORMAT_ALPHA:           return 1;
        case PIXEL_FORMAT_LUMINANCE:       return 1;
        case PIXEL_FORMAT_LUMINANCE_ALPHA: return 2;
        case PIXEL_FORMAT_RG:              return 2;
        case PIXEL_FORMAT_RGB:             return 3;
        case PIXEL_FORMAT_RGBA:            return 4;
        case PIXEL_FORMAT_BGB:             return 3;
        case PIXEL_FORMAT_BGBA:            return 4;
        default:                           return 0;
    }
}

void WG_BindTexture(WG_Renderer *renderer,WG_Texture *texture){
    if(texture != renderer->last_texture){
        WG_FlushBlitBuffer(renderer);
        glBindTexture(GL_TEXTURE_2D,texture->texture_handle);
        renderer->last_texture = texture;
    }
}

void WG_UploadNewTexture(void *pixels,PixelFormat format,int width,int height,int alignment,int row_length){
    glPixelStorei(GL_UNPACK_ALIGNMENT,alignment);
    glPixelStorei(GL_UNPACK_ROW_LENGTH,row_length);
    glTexImage2D(GL_TEXTURE_2D,0,format,width,height,0,format,GL_UNSIGNED_BYTE,pixels);
    glPixelStorei(GL_UNPACK_ROW_LENGTH,0);
    glPixelStorei(GL_UNPACK_ALIGNMENT,4);
}

void WG_UploadTexture(void *pixels,PixelFormat format,Rect rect,int alignment,int row_length){
    glPixelStorei(GL_UNPACK_ALIGNMENT,alignment);
    glPixelStorei(GL_UNPACK_ROW_LENGTH,row_length);
    glTexSubImage2D(GL_TEXTURE_2D,0,rect.x,rect.y,rect.w,rect.h,format,GL_UNSIGNED_BYTE,pixels);
    glPixelStorei(GL_UNPACK_ROW_LENGTH,0);
    glPixelStorei(GL_UNPACK_ALIGNMENT,4);
}

WG_Texture* WG_CreateUninitializedTexture(WG_Renderer *renderer,int width,int height,PixelFormat format,bool mipmap,bool framebuffer){

    if(width > renderer->max_texture_size || height > renderer->max_texture_size){
        fprintf(stderr,"Error: The size of the texture exceeds the maximum size\n");
        return NULL;
    }

    WG_Texture *texture = malloc(sizeof(WG_Texture));

    texture->renderer = renderer;
    texture->pixels = NULL;
    texture->locked_rect = (Rect){0};
    texture->width = width;
    texture->height = height;
    texture->bytes_per_pixel = WG_GetBytesPerPixel(format);
    texture->pitch = texture->width * texture->bytes_per_pixel;
    texture->color = (Color){255,255,255,255};
    texture->format = format;
    texture->use_mipmap = mipmap;
    texture->use_framebuffer = framebuffer;

    texture->view.viewport = (Rect){0,0,texture->width,texture->height};
    texture->view.cliprect = texture->view.viewport;
    texture->view.blend_mode = WG_GetBlendModeByPreset(BLEND_NORMAL_KEEP_ALPHA);
    texture->view.use_cliprect = false;
    texture->view.use_blending = true;
    
    glGenTextures(1,&texture->texture_handle);

    WG_BindTexture(renderer,texture);

    texture->min_filter = GL_NEAREST;
    texture->mag_filter = GL_NEAREST;
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,texture->min_filter);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,texture->mag_filter);

    texture->wrap_x = GL_CLAMP_TO_BORDER;
    texture->wrap_y = GL_CLAMP_TO_BORDER;
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,texture->wrap_x);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,texture->wrap_y);

    return texture;
}

bool WG_FilterIsValid(Filter filter){
    if(
        filter != FILTER_NEAREST &&
        filter != FILTER_LINEAR && 
        filter != FILTER_NEAREST_MIPMAP_NEAREST && 
        filter != FILTER_LINEAR_MIPMAP_NEAREST &&
        filter != FILTER_NEAREST_MIPMAP_LINEAR &&
        filter != FILTER_LINEAR_MIPMAP_LINEAR
    ){return false;}
    
    return true;
}

bool WG_WrapIsValid(Wrap wrap){
    if(
        wrap != WRAP_REPEAT &&
        wrap != WRAP_MIRRORED_REPEAT &&
        wrap != WRAP_CLAMP_TO_EDGE
    ){return false;}
    
    return true;
}


WG_Texture* WG_CreateTexture(WG_Renderer *renderer,int width,int height,PixelFormat format,bool mipmap,bool framebuffer){
    
    WG_Texture *texture = WG_CreateUninitializedTexture(renderer,width,height,format,mipmap,framebuffer);

    if(!texture) return NULL;

    unsigned int pixels_storage_size = width * height * texture->bytes_per_pixel;
    unsigned char *pixels = malloc(sizeof(unsigned char) * pixels_storage_size);
    memset(pixels,0,pixels_storage_size);

    texture->pixels = pixels;

    WG_UploadNewTexture(texture->pixels,texture->format,texture->width,texture->height,1,texture->width);

    if(texture->use_mipmap) glGenerateMipmap(GL_TEXTURE_2D);

    if(texture->use_framebuffer) WG_GenerateFrameBuffer(renderer,texture);

    return texture;
}

WG_Texture* WG_CreateTextureFromTexture(WG_Renderer *renderer,WG_Texture *texture,bool mipmap,bool framebuffer){
    if(!texture) return NULL;

    WG_Texture *new_texture = WG_CreateUninitializedTexture(renderer,texture->width,texture->height,texture->format,mipmap,framebuffer);

    if(!new_texture) return NULL;

    new_texture->pixels = malloc(sizeof(unsigned char) * (texture->pitch * texture->height));
    memcpy(new_texture->pixels,texture->pixels,(sizeof(unsigned char) * (texture->pitch * texture->height)));

    WG_UploadNewTexture(new_texture->pixels,new_texture->format,new_texture->width,new_texture->height,1,new_texture->width);

    if(new_texture->use_mipmap) glGenerateMipmap(GL_TEXTURE_2D);

    if(new_texture->use_framebuffer) WG_GenerateFrameBuffer(renderer,texture);

    return new_texture;
}

WG_Texture* WG_LoadTextureFromData(WG_Renderer *renderer,const uint8_t *data,uint32_t len,bool mipmap,bool framebuffer){
    
    if(!data) return NULL;

    int channels = 0;
    int width = 0;
    int height = 0;
    uint8_t *pixels = stbi_load_from_memory(data,len,&width,&height,&channels,0);

    WG_Texture *texture = WG_CreateUninitializedTexture(renderer,width,height,WG_GetPixelFormat(channels),mipmap,framebuffer);

    if(!texture){free(pixels);return NULL;}

    texture->pixels = pixels;

    WG_BindTexture(renderer,texture);

    WG_UploadNewTexture(texture->pixels,texture->format,texture->width,texture->height,1,texture->width);

    if(texture->use_mipmap) glGenerateMipmap(GL_TEXTURE_2D);

    if(texture->use_framebuffer) WG_GenerateFrameBuffer(renderer,texture);

    return texture;
}

WG_Texture* WG_LoadTextureFromFile(WG_Renderer *renderer,const uint16_t *fileName,bool mipmap,bool framebuffer){
    uint8_t *data = NULL;
    uint32_t len = 0;
    LoadFile(fileName,&data,&len);
    WG_Texture *texture = WG_LoadTextureFromData(renderer,data,len,mipmap,framebuffer);
    free(data);
    return texture;
}

void WG_DestroyTexture(WG_Texture *texture){
    if(!texture) return;

    WG_Renderer *renderer = texture->renderer;

    if(renderer->current_target == texture){
        WG_RendererSetTarget(renderer,NULL);
    }
    if(renderer->last_texture == texture){
        WG_FlushBlitBuffer(renderer);
        renderer->last_texture = NULL;
    }

    free(texture->pixels);

    glDeleteTextures(1,&texture->texture_handle);
    glDeleteFramebuffers(1,&texture->framebuffer_handle);
    
    free(texture);
}

void WG_SetTextureColor(WG_Texture *texture,Color color){
    texture->color = color;
}

void WG_SetTextureFilter(WG_Texture *texture,Filter min_filter,Filter mag_filter){
    
    if(!texture) return;

    if(!WG_FilterIsValid(min_filter)){
        fprintf(stderr,"Error: min_filter invalid value: %#x\n",min_filter);
        return;
    }

    if(!WG_FilterIsValid(mag_filter)){
        fprintf(stderr,"Error: mag_filter invalid value: %#x\n",mag_filter);
        return;
    }

    if(texture == texture->renderer->last_texture || texture == texture->renderer->current_target){
        WG_FlushBlitBuffer(texture->renderer);
    }
    
    WG_BindTexture(texture->renderer,texture);

    texture->min_filter = min_filter;
    texture->mag_filter = mag_filter;

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,min_filter);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,mag_filter);

}

void WG_SetTextureWrap(WG_Texture *texture,Wrap wrap_x,Wrap wrap_y){
    if(!texture);

    if(!WG_WrapIsValid(wrap_x)){
        fprintf(stderr,"Error: wrap_x invalid value: %#x\n",wrap_x);
        return;
    }

    if(!WG_WrapIsValid(wrap_y)){
        fprintf(stderr,"Error: wrap_y invalid value: %#x\n",wrap_y);
        return;
    }

    if(texture == texture->renderer->last_texture || texture == texture->renderer->current_target){
        WG_FlushBlitBuffer(texture->renderer);
    }
    
    WG_BindTexture(texture->renderer,texture);

    texture->wrap_x = wrap_x;
    texture->wrap_y = wrap_y;

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,wrap_x);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,wrap_y);
}

void WG_GenerateFrameBuffer(WG_Renderer *renderer,WG_Texture *texture){
    
    if(texture->use_framebuffer) return;
    
    glGenFramebuffers(1,&texture->framebuffer_handle);

    glBindFramebuffer(GL_FRAMEBUFFER,texture->framebuffer_handle);

    glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,texture->texture_handle,0);

    if(renderer->current_target != NULL){
        glBindFramebuffer(GL_FRAMEBUFFER,renderer->current_target->framebuffer_handle);
    }
    else{
        glBindFramebuffer(GL_FRAMEBUFFER,0);
    }

    texture->use_framebuffer = true;
}

void WG_GenearateMipmap(WG_Renderer *renderer,WG_Texture *texture){
    if(texture == renderer->last_texture || texture == renderer->current_target){
        WG_FlushBlitBuffer(renderer);
    }
    WG_BindTexture(renderer,texture);
    glGenerateMipmap(GL_TEXTURE_2D);
    texture->use_mipmap = true;
}

void WG_LockTexture(WG_Texture *texture,Rect *rect,void **pixels,int *pitch){
    if(rect != NULL) texture->locked_rect = *rect;
    else             texture->locked_rect = (Rect){0,0,texture->width,texture->height};

    *pixels = texture->pixels + (texture->locked_rect.y * texture->pitch) + (texture->locked_rect.x * texture->bytes_per_pixel);
    *pitch = texture->pitch;
}

void WG_UnlockTexture(WG_Texture *texture){
    WG_BindTexture(texture->renderer,texture);
    void *pixels = texture->pixels + (texture->locked_rect.y * texture->pitch) + (texture->locked_rect.x * texture->bytes_per_pixel);
    WG_UploadTexture(pixels,texture->format,texture->locked_rect,1,texture->width);
}

void WG_TextureSize(WG_Texture *texture,int *width,int *height){
    if(!texture) return;
    if(width)  *width  = texture->width;
    if(height) *height = texture->height;
}

void WG_TextureFormat(WG_Texture *texture,PixelFormat *format){
    if(format) *format = texture->format;
}

bool WG_TextureHasFrameBuffer(WG_Texture *texture){
    return texture->use_framebuffer;
}

bool WG_TextureHasMipmap(WG_Texture *texture){
    return texture->use_mipmap;
}

void WG_TextureWrap(WG_Texture *texture,Wrap *wrap_x,Wrap *wrap_y){
    if(wrap_x) *wrap_x = texture->wrap_x;
    if(wrap_y) *wrap_y = texture->wrap_y;
}

void WG_TextureFilter(WG_Texture *texture,Filter *min_filter,Filter *mag_filter){
    if(min_filter) *min_filter = texture->min_filter;
    if(mag_filter) *mag_filter = texture->mag_filter;
}



uint32_t WG_CompilerShader(const char *const source,uint32_t type){    
    GLuint shaderID = glCreateShader(type);
    glShaderSource(shaderID,1,&source,NULL);
    glCompileShader(shaderID);
    GLint success;
    glGetShaderiv(shaderID,GL_COMPILE_STATUS,&success);
    if(!success){
        glGetShaderInfoLog(shaderID,MAX_BUFFER,NULL,buffer);
        fprintf(stderr,"%s\n",buffer);
    }
    return shaderID;
}

void WG_ActivateShaderProgram(WG_Renderer *renderer,ShaderProgram shader_program){
    
    if(shader_program == renderer->current_shader_program) return;

    WG_FlushBlitBuffer(renderer);

    switch(shader_program){
        case SHADER_PROGRAM_SHAPE:
            glUseProgram(renderer->shape_shader_data.program);
            break;
        case SHADER_PROGRAM_TEXTURE:
            glUseProgram(renderer->texture_shader_data.program);
            break;
        case SHADER_PROGRAM_NONE:
            glUseProgram(0);
            break;
        default:
            fprintf(stderr,"shader program %d is invalid\n",shader_program);
            return;
    }

    renderer->current_shader_program = shader_program;
}

void WG_UpdateWindowSize(void *void_renderer,WG_Event *event){

    if(event->type == WG_WINDOWRESIZE){

        WG_Renderer *renderer = void_renderer;
        
        WG_GetWindowClientSize(renderer->window,&renderer->window_width,&renderer->window_height);

        WG_RendererSetViewport(renderer,NULL,NULL);
    }
}

ShapeShaderData WG_LoadShapeShaderData(){
    ShapeShaderData data;
    
    data.vertex_shader = WG_CompilerShader(SHAPE_VERTEX_SHADER_SOURCE,GL_VERTEX_SHADER);
    data.fragment_shader = WG_CompilerShader(SHAPE_FRAGMENT_SHADER_SOURCE,GL_FRAGMENT_SHADER);
    
    data.program = glCreateProgram();
    glAttachShader(data.program,data.vertex_shader);
    glAttachShader(data.program,data.fragment_shader);
    glLinkProgram(data.program);

    data.vertex_position = glGetAttribLocation(data.program,"vertex_position");
    data.vertex_color = glGetAttribLocation(data.program,"vertex_color");

    data.projection = glGetUniformLocation(data.program,"projection");

    return data;
}

void WG_ShapeShaderDataFree(ShapeShaderData data){
    glDeleteShader(data.vertex_shader);
    glDeleteShader(data.fragment_shader);
    glDeleteProgram(data.program);
}

TextureShaderData WG_LoadTextureShaderData(){
    TextureShaderData data;

    data.vertex_shader = WG_CompilerShader(TEXTURE_VERTEX_SHADER_SOURCE,GL_VERTEX_SHADER);
    data.fragment_shader = WG_CompilerShader(TEXTURE_FRAGMENT_SHADER_SOURCE,GL_FRAGMENT_SHADER);
    
    data.program = glCreateProgram();
    glAttachShader(data.program,data.vertex_shader);
    glAttachShader(data.program,data.fragment_shader);
    glLinkProgram(data.program);

    data.vertex_position = glGetAttribLocation(data.program,"vertex_position");
    data.vertex_texture_coord = glGetAttribLocation(data.program,"vertex_texture_coord");
    data.vertex_color = glGetAttribLocation(data.program,"vertex_color");
    
    data.projection = glGetUniformLocation(data.program,"projection");

    return data;
}

void WG_TextureShaderDataFree(TextureShaderData data){
    glDeleteShader(data.vertex_shader);
    glDeleteShader(data.fragment_shader);
    glDeleteProgram(data.program);
}


WG_Renderer* WG_CreateRenderer(WG_Window *window,WG_Renderer *share_renderer){

    WG_Renderer *renderer = malloc(sizeof(WG_Renderer));

    renderer->window = window;

    renderer->hdc = GetDC(WG_GetWindowHandle(renderer->window));

    PIXELFORMATDESCRIPTOR pfd = {0};
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.cColorBits = 32;
    pfd.cDepthBits = 24;
    pfd.cStencilBits = 8;

    int pixelFormat = ChoosePixelFormat(renderer->hdc,&pfd);
    SetPixelFormat(renderer->hdc,pixelFormat,&pfd);

    renderer->hglrc = wglCreateContext(renderer->hdc);
    WG_SetCurrentRenderer(renderer);

    if(!gladLoadGLLoader((GLADloadproc)wglGetProcAddress) || !gladLoadWGLLoader((GLADloadproc)wglGetProcAddress,renderer->hdc)){
        printf("Failed to load GLAD\n");
    }

    if(share_renderer){
        if(!wglShareLists(share_renderer->hglrc,renderer->hglrc)){
            printf("Failed to share lists between contexts\n");
        }
    }

    WG_GetWindowClientSize(window,&renderer->window_width,&renderer->window_height);

    WG_SetEventCallback(window,WG_UpdateWindowSize,renderer);

    glGetIntegerv(GL_MAX_TEXTURE_SIZE,&renderer->max_texture_size);

    renderer->main_view.viewport = (Rect){0,0,renderer->window_width,renderer->window_height};
    renderer->main_view.cliprect = renderer->main_view.viewport;
    renderer->main_view.blend_mode = WG_GetBlendModeByPreset(BLEND_NORMAL_KEEP_ALPHA);
    renderer->main_view.use_cliprect = false;
    renderer->main_view.use_blending = true;

    renderer->current_view = &renderer->main_view;
    renderer->current_target = NULL;
    renderer->current_shader_program = SHADER_PROGRAM_NONE;

    renderer->last_texture = NULL;
    renderer->last_shape = GL_TRIANGLES;
    renderer->last_viewport = renderer->current_view->viewport;
    renderer->last_blend_mode = renderer->current_view->blend_mode;
    renderer->last_use_blending = false;

    glViewport(renderer->last_viewport.x,renderer->last_viewport.y,renderer->last_viewport.w,renderer->last_viewport.h);

    glBlendFuncSeparate(renderer->last_blend_mode.srcRGB,renderer->last_blend_mode.dstRGB,renderer->last_blend_mode.srcAlpha,renderer->last_blend_mode.dstAlpha);
    glBlendEquationSeparate(renderer->last_blend_mode.modeRGB,renderer->last_blend_mode.modeAlpha);

    glDisable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);

    CreateGapBuffer(renderer->blit_buffer,float,GapExpandSize);
    renderer->blit_buffer_max_size = 0;

    CreateGapBuffer(renderer->blit_index,uint32_t,GapExpandSize);
    renderer->blit_index_max_size = 0;

    renderer->shape_shader_data = WG_LoadShapeShaderData();
    renderer->texture_shader_data = WG_LoadTextureShaderData();

    glGenBuffers(1,&renderer->blit_VBO);
    glBindBuffer(GL_ARRAY_BUFFER,renderer->blit_VBO);

    glGenBuffers(1,&renderer->blit_EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,renderer->blit_EBO);

    glGenVertexArrays(1,&renderer->blit_VAO);
    glBindVertexArray(renderer->blit_VAO);

    return renderer;
}

void WG_SetCurrentRenderer(WG_Renderer *renderer){
    if(renderer){
        HGLRC current = wglGetCurrentContext();
        if(current != renderer->hglrc){
            wglMakeCurrent(renderer->hdc,renderer->hglrc);
        }
    }
    else{
        wglMakeCurrent(NULL,NULL);
    }
}

void WG_DestroyRenderer(WG_Renderer *renderer){
    
    if(!renderer) return;

    WG_ShapeShaderDataFree(renderer->shape_shader_data);
    WG_TextureShaderDataFree(renderer->texture_shader_data);

    glDeleteBuffers(1,&renderer->blit_VBO);
    glDeleteBuffers(1,&renderer->blit_EBO);
    glDeleteVertexArrays(1,&renderer->blit_VAO);

    GapBufferFree(renderer->blit_buffer);
    GapBufferFree(renderer->blit_index);

    if(wglGetCurrentContext() == renderer->hglrc){
        wglMakeCurrent(NULL,NULL);
    }

    wglDeleteContext(renderer->hglrc);
    
    ReleaseDC(WG_GetWindowHandle(renderer->window),renderer->hdc);

    free(renderer);
}

void WG_RendererSetTarget(WG_Renderer *renderer,WG_Texture *target){

    if(renderer->current_target == target || (target && !target->use_framebuffer)) return;

    WG_FlushBlitBuffer(renderer);

    if(target){
        glBindFramebuffer(GL_FRAMEBUFFER,target->framebuffer_handle);
        renderer->current_view = &target->view;
    }
    else{
        glBindFramebuffer(GL_FRAMEBUFFER,0);
        renderer->current_view = &renderer->main_view;
    }

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
        fprintf(stderr,"Error: failed to bind framebuffer\n");
    }

    renderer->current_target = target;
}

void WG_RendererSetVsync(WG_Renderer *renderer,bool enable){
    if(!wglSwapIntervalEXT){
        printf("Vsync not supported\n");
        return;
    }
    wglSwapIntervalEXT(enable);
}

void WG_RendererSetViewport(WG_Renderer *renderer,WG_Texture *target,Rect *rect){
    
    if(target == renderer->current_target) WG_FlushBlitBuffer(renderer);

    if(target != NULL){
        if(rect != NULL){
            target->view.viewport = *rect;
        }
        else{
            target->view.viewport.x = 0;
            target->view.viewport.y = 0;
            target->view.viewport.w = target->width;
            target->view.viewport.h = target->height;
        }
    }
    else{
        if(rect != NULL){
            renderer->main_view.viewport = *rect;
        }
        else{
            renderer->main_view.viewport.x = 0;
            renderer->main_view.viewport.y = 0;
            renderer->main_view.viewport.w = renderer->window_width;
            renderer->main_view.viewport.h = renderer->window_height;
        }
    }
}


void WG_SetProjection(uint32_t projection,float right,float bottom){

    right += 0.005f;
    bottom += 0.005f;
    
    float top = -0.005f;
    float left = -0.005f;
    float n = 0.0f;
    float f = 1.0f;

    float projection_matrix[16] = {
        2.0f / (right - left),  0.0f,                   0.0f,                -(right + left) / (right - left),
        0.0f,                   2.0f / (top - bottom),  0.0f,                -(top + bottom) / (top - bottom),
        0.0f,                   0.0f,                  -2.0f / (f - n),      -(f + n) / (f - n),
        0.0f,                   0.0f,                   0.0f,                 1.0f
    };

    glUniformMatrix4fv(projection,1,GL_TRUE,projection_matrix);
}

void WG_SubmitBufferData(WG_Renderer *renderer){
    uint32_t blit_buffer_size = ContentSize(renderer->blit_buffer,float);
    uint32_t blit_index_size = ContentSize(renderer->blit_index,uint32_t);

    if(blit_buffer_size > renderer->blit_buffer_max_size){
        glNamedBufferData(renderer->blit_VBO,blit_buffer_size * sizeof(float),renderer->blit_buffer->buffer,GL_DYNAMIC_DRAW);
        renderer->blit_buffer_max_size = blit_buffer_size;
    }
    else{
        glNamedBufferSubData(renderer->blit_VBO,0,blit_buffer_size * sizeof(float),renderer->blit_buffer->buffer);
    }

    if(blit_index_size > renderer->blit_index_max_size){
        glNamedBufferData(renderer->blit_EBO,blit_index_size * sizeof(uint32_t),renderer->blit_index->buffer,GL_DYNAMIC_DRAW);
        renderer->blit_index_max_size = blit_index_size;
    }
    else{
        glNamedBufferSubData(renderer->blit_EBO,0,blit_index_size * sizeof(uint32_t),renderer->blit_index->buffer);
    }
}


void WG_FlushBlitBuffer(WG_Renderer *renderer){

    if(!ContentSize(renderer->blit_buffer,float)) return;

    WG_SubmitBufferData(renderer);

    ChangeViewport(renderer,renderer->current_view->viewport);

    SetClipRect(renderer);

    switch(renderer->current_shader_program){

        case SHADER_PROGRAM_SHAPE:

            WG_SetProjection(renderer->shape_shader_data.projection,renderer->current_view->viewport.w,renderer->current_view->viewport.h);

            glEnableVertexAttribArray(renderer->shape_shader_data.vertex_position);
            glVertexAttribPointer(renderer->shape_shader_data.vertex_position,2,GL_FLOAT,GL_FALSE,(UnTextureAttributeSize * sizeof(GLfloat)),(GLvoid*)UnTexturePositionOffset);
            
            glEnableVertexAttribArray(renderer->shape_shader_data.vertex_color);
            glVertexAttribPointer(renderer->shape_shader_data.vertex_color,4,GL_FLOAT,GL_FALSE,(UnTextureAttributeSize * sizeof(GLfloat)),(GLvoid*)(UnTextureColorOffset * sizeof(GLfloat)));

            glDrawElements(renderer->last_shape,ContentSize(renderer->blit_index,uint32_t),GL_UNSIGNED_INT,renderer->blit_index->buffer);
            
            glDisableVertexAttribArray(renderer->shape_shader_data.vertex_position);
            glDisableVertexAttribArray(renderer->shape_shader_data.vertex_color);
            
            break;

        case SHADER_PROGRAM_TEXTURE:
            
            WG_SetProjection(renderer->texture_shader_data.projection,renderer->current_view->viewport.w,renderer->current_view->viewport.h);

            glEnableVertexAttribArray(renderer->texture_shader_data.vertex_position);
            glVertexAttribPointer(renderer->texture_shader_data.vertex_position,2,GL_FLOAT,GL_FALSE,(TextureAttributeSize * sizeof(GLfloat)),(GLvoid*)(TexturePositionOffset * sizeof(GLfloat)));
            
            glEnableVertexAttribArray(renderer->texture_shader_data.vertex_texture_coord);
            glVertexAttribPointer(renderer->texture_shader_data.vertex_texture_coord,2,GL_FLOAT,GL_FALSE,(TextureAttributeSize * sizeof(GLfloat)),(GLvoid*)(TextureCoordOffset * sizeof(GLfloat)));
            
            glEnableVertexAttribArray(renderer->texture_shader_data.vertex_color);
            glVertexAttribPointer(renderer->texture_shader_data.vertex_color,4,GL_FLOAT,GL_FALSE,(TextureAttributeSize * sizeof(GLfloat)),(GLvoid*)(TextureColorOffset * sizeof(GLfloat)));

            glDrawElements(renderer->last_shape,ContentSize(renderer->blit_index,uint32_t),GL_UNSIGNED_INT,renderer->blit_index->buffer);

            glDisableVertexAttribArray(renderer->texture_shader_data.vertex_position);
            glDisableVertexAttribArray(renderer->texture_shader_data.vertex_texture_coord);
            glDisableVertexAttribArray(renderer->texture_shader_data.vertex_color);

            break;
    }

    UnSetClipRect(renderer);

    GapBufferClear(renderer->blit_index);
    GapBufferClear(renderer->blit_buffer);
}

void WG_ClearColor(WG_Renderer *renderer,Color color){
    glClearColor(color.r / 225.0f,color.g / 255.0f,color.b / 255.0f,color.a / 255.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void WG_ClearRGBA(WG_Renderer *renderer,uint8_t r,uint8_t g,uint8_t b,uint8_t a){
    glClearColor(r / 255.0f,g / 255.0f,b / 255.0f,a / 255.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void WG_Flip(WG_Renderer *renderer){
    WG_FlushBlitBuffer(renderer);
    SwapBuffers(renderer->hdc);
}


/////Draw Shapes and Textures Functions/////

void WG_PrepareToRenderShape(WG_Renderer *renderer,GLenum shape){
    if(renderer->last_shape != shape){
        WG_FlushBlitBuffer(renderer);
        renderer->last_shape = shape;
    }
    ChangeBlending(renderer,(renderer)->current_view->use_blending);
    ChangeBlendMode(renderer,(renderer)->current_view->blend_mode);
    WG_ActivateShaderProgram(renderer,SHADER_PROGRAM_SHAPE);
}

void WG_PrepareToRenderTexture(WG_Renderer *renderer){
    if(renderer->last_shape != GL_TRIANGLES){
        WG_FlushBlitBuffer(renderer);
        renderer->last_shape = GL_TRIANGLES;
    }
    ChangeBlending(renderer,renderer->current_view->use_blending);
    ChangeBlendMode(renderer,renderer->current_view->blend_mode);
    WG_ActivateShaderProgram(renderer,SHADER_PROGRAM_TEXTURE);
}


void WG_DrawFilledRectangleF(WG_Renderer *renderer,RectF *rect,Color color){
    _DrawFilledRectangle(renderer,rect,color);
}

void WG_DrawFilledRectangle(WG_Renderer *renderer,Rect *rect,Color color){
    _DrawFilledRectangle(renderer,rect,color);
}


void WG_DrawFilledRectanglesF(WG_Renderer *renderer,RectF *rects,uint32_t count,Color color){
    _DrawFilledRectangles(renderer,rects,count,color);
}

void WG_DrawFilledRectangles(WG_Renderer *renderer,Rect *rects,uint32_t count,Color color){
    _DrawFilledRectangles(renderer,rects,count,color);
}


void WG_DrawRectangleF(WG_Renderer *renderer,RectF *rect,Color color){
    _DrawRectangle(renderer,rect,color);
}

void WG_DrawRectangle(WG_Renderer *renderer,Rect *rect,Color color){
    _DrawRectangle(renderer,rect,color);
}


void WG_DrawRectanglesF(WG_Renderer *renderer,RectF *rects,uint32_t count,Color color){
    _DrawRectangles(renderer,rects,count,color);
}

void WG_DrawRectangles(WG_Renderer *renderer,Rect *rects,uint32_t count,Color color){
    _DrawRectangles(renderer,rects,count,color);
}


void WG_DrawFilledTriangleF(WG_Renderer *renderer,Vector2F vertex1,Vector2F vertex2,Vector2F vertex3,Color color){
    _DrawFilledTriangle(renderer,vertex1,vertex2,vertex3,color);
}

void WG_DrawFilledTriangle(WG_Renderer *renderer,Vector2 vertex1,Vector2 vertex2,Vector2 vertex3,Color color){
    _DrawFilledTriangle(renderer,vertex1,vertex2,vertex3,color);
}


void WG_DrawTriangleF(WG_Renderer *renderer,Vector2F vertex1,Vector2F vertex2,Vector2F vertex3,Color color){
    _DrawTriangle(renderer,vertex1,vertex2,vertex3,color);
}

void WG_DrawTriangle(WG_Renderer *renderer,Vector2 vertex1,Vector2 vertex2,Vector2 vertex3,Color color){
    _DrawTriangle(renderer,vertex1,vertex2,vertex3,color);
}


void WG_DrawFilledCircleF(WG_Renderer *renderer,Vector2F center,float radius,Color color){
    _DrawFilledCircle(renderer,center,radius,color);
}

void WG_DrawFilledCircle(WG_Renderer *renderer,Vector2 center,int radius,Color color){
    _DrawFilledCircle(renderer,center,radius,color);
}


void WG_DrawCircleF(WG_Renderer *renderer,Vector2F center,float radius,Color color){
    _DrawCircle(renderer,center,radius,color);
}

void WG_DrawCircle(WG_Renderer *renderer,Vector2 center,int radius,Color color){
    _DrawCircle(renderer,center,radius,color);
}


void WG_DrawFilledArcF(WG_Renderer *renderer,Vector2F center,float radius,float startAngle,float endAngle,Color color){
    _DrawFilledArc(renderer,center,radius,startAngle,endAngle,color);
}

void WG_DrawFilledArc(WG_Renderer *renderer,Vector2 center,int radius,int startAngle,int endAngle,Color color){
    _DrawFilledArc(renderer,center,radius,startAngle,endAngle,color);
}


void WG_DrawArcF(WG_Renderer *renderer,Vector2F center,float radius,float startAngle,float endAngle,Color color){
    _DrawArc(renderer,center,radius,startAngle,endAngle,color);
}

void WG_DrawArc(WG_Renderer *renderer,Vector2 center,int radius,int startAngle,int endAngle,Color color){
    _DrawArc(renderer,center,radius,startAngle,endAngle,color);
}


void WG_DrawPixelF(WG_Renderer *renderer,Vector2F point,Color color){
    _DrawPixel(renderer,point,color);
}

void WG_DrawPixel(WG_Renderer *renderer,Vector2 point,Color color){
    _DrawPixel(renderer,point,color);
}


void WG_DrawLineF(WG_Renderer *renderer,Vector2F point1,Vector2F point2,Color color){
    _DrawLine(renderer,point1,point2,color);
}

void WG_DrawLine(WG_Renderer *renderer,Vector2 point1,Vector2 point2,Color color){
    _DrawLine(renderer,point1,point2,color);
}


void WG_DrawLinesF(WG_Renderer *renderer,Vector2F *points,uint32_t points_count,Color color){
    _DrawLines(renderer,points,points_count,color);
}

void WG_DrawLines(WG_Renderer *renderer,Vector2 *points,uint32_t points_count,Color color){
    _DrawLines(renderer,points,points_count,color);
}


void WG_DrawPolygonF(WG_Renderer *renderer,Vector2F *vertices,uint32_t vertices_count,Color color){
    _DrawPolygon(renderer,vertices,vertices_count,color);
}

void WG_DrawPolygon(WG_Renderer *renderer,Vector2 *vertices,uint32_t vertices_count,Color color){
    _DrawPolygon(renderer,vertices,vertices_count,color);
}


void WG_DrawFilledPolygonF(WG_Renderer *renderer,Vector2F *vertices,uint32_t vertices_count,Color color){
    _DrawFilledPolygon(renderer,vertices,vertices_count,color);
}

void WG_DrawFilledPolygon(WG_Renderer *renderer,Vector2 *vertices,uint32_t vertices_count,Color color){
    _DrawFilledPolygon(renderer,vertices,vertices_count,color);
}


void WG_DrawTextF(WG_Renderer *renderer,WG_FontAtlas *atlas,uint16_t *text,Vector2F pos,Color color){
    _DrawText(renderer,atlas,text,pos,color);
}

void WG_DrawText(WG_Renderer *renderer,WG_FontAtlas *atlas,uint16_t *text,Vector2 pos,Color color){
    _DrawText(renderer,atlas,text,pos,color);
}


void WG_DrawNTextF(WG_Renderer *renderer,WG_FontAtlas *atlas,uint16_t *text,uint32_t n,Vector2F pos,Color color){
    _DrawNText(renderer,atlas,text,n,pos,color);
}

void WG_DrawNText(WG_Renderer *renderer,WG_FontAtlas *atlas,uint16_t *text,uint32_t n,Vector2 pos,Color color){
    _DrawNText(renderer,atlas,text,n,pos,color);
}


void WG_DrawTextByGapBufferF(WG_Renderer *renderer,WG_FontAtlas *atlas,GapBuffer *gap_buffer,Vector2F pos,Color color){
    _DrawTextByGapBuffer(renderer,atlas,gap_buffer,pos,color);
}

void WG_DrawTextByGapBuffer(WG_Renderer *renderer,WG_FontAtlas *atlas,GapBuffer *gap_buffer,Vector2 pos,Color color){
    _DrawTextByGapBuffer(renderer,atlas,gap_buffer,pos,color);
}


void WG_BlitColorF(WG_Renderer *renderer,WG_Texture *texture,RectF *src_rect,RectF *dst_rect,Color color){
    texture->color = color;
    _Blit(renderer,texture,src_rect,dst_rect);
}

void WG_BlitColor(WG_Renderer *renderer,WG_Texture *texture,Rect *src_rect,Rect *dst_rect,Color color){
    texture->color = color;
    _Blit(renderer,texture,src_rect,dst_rect);
}


void WG_BlitF(WG_Renderer *renderer,WG_Texture *texture,RectF *src_rect,RectF *dst_rect){
    _Blit(renderer,texture,src_rect,dst_rect);
}

void WG_Blit(WG_Renderer *renderer,WG_Texture *texture,Rect *src_rect,Rect *dst_rect){
    _Blit(renderer,texture,src_rect,dst_rect);
}


void WG_BlitExF(WG_Renderer *renderer,WG_Texture *texture,RectF *src_rect,RectF *dst_rect,float angle,Vector2F *center,FlipFlag flags){
    _BlitEx(renderer,texture,src_rect,dst_rect,angle,center,flags);
}

void WG_BlitEx(WG_Renderer *renderer,WG_Texture *texture,Rect *src_rect,Rect *dst_rect,float angle,Vector2 *center,FlipFlag flags){
    _BlitEx(renderer,texture,src_rect,dst_rect,angle,center,flags);
}


void WG_Geometry(WG_Renderer *renderer,WG_Texture *texture,Vertex *vertices,uint32_t vertices_count,uint32_t *indices,uint32_t indices_count){

    if(!vertices || vertices_count < 3 || (!indices && vertices_count % 3 != 0) || (indices && indices_count % 3 != 0)) return;

    Vertex vertex;
    uint32_t start_index;
    Vector2F texcoord;

    if(indices){
        for(int i = 0; i<indices_count; ++i){
            if(indices[i] >= vertices_count) return;
        }
    }

    if(texture){
        for(int i = 0; i<vertices_count; ++i){
            texcoord = vertices[i].texcoord;
            if(texcoord.x < 0.0f || texcoord.y < 0.0f || texcoord.x > 1.0f || texcoord.y > 1.0f) return;
        }
    }

    if(texture != NULL){

        WG_PrepareToRenderTexture(renderer);

        WG_BindTexture(renderer,texture);

        start_index = ContentSize(renderer->blit_buffer,float) / TextureAttributeSize;

        for(unsigned int i = 0; i < vertices_count; ++i){
            vertex = vertices[i];
            SetTextureVertex(renderer->blit_buffer,vertex.position.x,vertex.position.y,vertex.texcoord.x,vertex.texcoord.y,vertex.color);
        }
    }
    else{

        WG_PrepareToRenderShape(renderer,GL_TRIANGLES);

        start_index = ContentSize(renderer->blit_buffer,float) / UnTextureAttributeSize;

        for(unsigned int i = 0; i < vertices_count; ++i){
            vertex = vertices[i];
            SetUnTextureVertex(renderer->blit_buffer,vertex.position.x,vertex.position.y,vertex.color);
        }
    }

    if(indices != NULL && indices_count >= 3){
        for(unsigned int i = 0; i < indices_count; ++i){
            GapBufferInsertValue(renderer->blit_index,uint32_t,indices[i] + start_index);
        }
    }
    else{
        for(unsigned int i = 0; i < vertices_count; ++i){
            GapBufferInsertValue(renderer->blit_index,uint32_t,i + start_index);
        }
    }
}


/////Font Functions/////


int Length(const uint16_t *text){
    int count = 0;
    while(*text){++count;++text;};
    return count;
}


bool WG_GlyphIsProvided(WG_Font *font,int char_code){
    int glyph_index = stbtt_FindGlyphIndex(font->info,char_code);
    return ((stbtt_IsGlyphEmpty(font->info,glyph_index)) ? false : true);
}

void WG_GlyphMetrics(WG_Font *font,int char_code,int *advance,int *minx,int *maxx,int *miny,int *maxy){
    stbtt_GetCodepointHMetrics(font->info,char_code,advance,NULL);
    stbtt_GetCodepointBox(font->info,char_code,minx,maxx,miny,maxy);
}

void WG_SetFontKerning(WG_Font *font,bool allowed){
    font->allow_kerning = allowed;
}

int WG_GetFontHeight(WG_Font *font){
    return font->height;
}

void WG_SetFontHeight(WG_Font *font,int height){
    font->height = height;
    font->scale = stbtt_ScaleForPixelHeight(font->info,font->height);
    font->baseLine = font->ascent * font->scale;
}

WG_Font *WG_OpenFontFromData(uint8_t *data,int height){
    if(!data) return NULL;

    stbtt_fontinfo font_info;

    if(!stbtt_InitFont(&font_info,data,0)){
        free(data);
        return NULL;
    }

    WG_Font *font = malloc(sizeof(WG_Font));
    font->data = data;

    font->info = malloc(sizeof(stbtt_fontinfo));
    memcpy(font->info,&font_info,sizeof(stbtt_fontinfo));

    stbtt_GetFontVMetrics(font->info,&font->ascent,&font->descent,&font->lineGap);
    WG_SetFontHeight(font,height);
    WG_SetFontKerning(font,true);

    return font;
}

WG_Font* WG_OpenFontFromFile(const uint16_t *fileName,int height){
    unsigned char *data = NULL;
    LoadFile(fileName,&data,NULL);
    WG_Font *font = WG_OpenFontFromData(data,height);
    if(!font){free(data);return NULL;}
    return font;
}

void WG_CloseFont(WG_Font *font){
    free(font->data);
    free(font->info);
    free(font);
}

void WG_SizeText(WG_Font *font,const uint16_t *text,int *width,int *height){
    if(width){
        *width = 0;
        int advance,glyph;
        while(*text){
            glyph = stbtt_FindGlyphIndex(font->info,*text++);
            stbtt_GetGlyphHMetrics(font->info,glyph,&advance,NULL);
            (*width) += roundf(advance * font->scale);
        }
    }
    if(height) *height = font->height;
}

WG_Texture* WG_RenderGlyph(WG_Renderer *renderer,WG_Font *font,uint16_t char_code){
    int advance,x0,y0,x1,y1;

    int glyph = stbtt_FindGlyphIndex(font->info,char_code);

    stbtt_GetGlyphHMetrics(font->info,glyph,&advance,NULL);
    advance = roundf(advance * font->scale);

    stbtt_GetGlyphBitmapBox(font->info,glyph,font->scale,font->scale,&x0,&y0,&x1,&y1);
    int gw = x1-x0;
    int gh = y1-y0;

    int w = Max(gw,advance);
    int h = font->height;
    unsigned char *bitmap = calloc(w*h,sizeof(unsigned char));

    int gx = x0;
    int gy = font->baseLine + y0;

    if(gy < 0){gy = 0;}
    if(gx < 0){gx = 0;}

    if(gh <= h){
        stbtt_MakeGlyphBitmap(font->info,bitmap+gx+gy*w,gw,gh,w,font->scale,font->scale,glyph);
    }

    FlipBitmap(bitmap,w,h);

    uint8_t *pixels = malloc(sizeof(uint8_t) * (w * 4 * h));

    MakePixelsRGBA(bitmap,w,h,pixels);

    free(bitmap);

    WG_Texture *texture = WG_CreateUninitializedTexture(renderer,w,h,PIXEL_FORMAT_ALPHA,false,false);
    
    if(!texture){free(pixels);return NULL;}

    texture->pixels = pixels;

    WG_UploadNewTexture(texture->pixels,texture->format,texture->width,texture->height,1,texture->width);

    return texture;
}

WG_Texture* WG_RenderText(WG_Renderer *renderer,WG_Font *font,const uint16_t *text){
    int w,h,glyph,x0,y0,x1,y1,advance,offset;

    WG_SizeText(font,text,&w,&h);
    unsigned char *bitmap = calloc(w*h,sizeof(unsigned char));

    int xpos = 0;
    int gx,gy,gw,gh;
    while(*text){
        glyph = stbtt_FindGlyphIndex(font->info,*text++);
        stbtt_GetGlyphBitmapBox(font->info,glyph,font->scale,font->scale,&x0,&y0,&x1,&y1);
        stbtt_GetGlyphHMetrics(font->info,glyph,&advance,NULL);
        gx = xpos + x0;
        gy = font->baseLine + y0;
        gw = x1-x0;
        gh = y1-y0;
        if(gy < 0)gy = 0;
        if(gx < 0)gx = 0;
        if(gh <= h){
            stbtt_MakeGlyphBitmap(font->info,bitmap+gx+gy*w,gw,gh,w,font->scale,font->scale,glyph);
        }
        xpos += roundf(advance * font->scale);
    }

    FlipBitmap(bitmap,w,h);

    uint8_t *pixels = malloc(sizeof(uint8_t) * (w * 4 * h));

    MakePixelsRGBA(bitmap,w,h,pixels);

    free(bitmap);

    WG_Texture *texture = WG_CreateUninitializedTexture(renderer,w,h,PIXEL_FORMAT_RGBA,false,false);
    
    if(!texture){free(pixels);return NULL;}

    texture->pixels = pixels;
    WG_UploadNewTexture(texture->pixels,texture->format,texture->width,texture->height,1,texture->width);

    return texture;
}


WG_FontAtlas* WG_RenderFontAtlas(WG_Renderer *renderer,WG_Font *font,int height,int first_char,int num_chars){

    WG_FontAtlas *font_atlas = malloc(sizeof(WG_FontAtlas));

    if(!font_atlas){
        fprintf(stderr,"%s\n",strerror(errno));
        return NULL;
    }

    font_atlas->scale = stbtt_ScaleForPixelHeight(font->info,height);
    font_atlas->baseLine = font->ascent * font_atlas->scale;
    font_atlas->height = height;
    font_atlas->first_char = first_char;
    font_atlas->num_chars = num_chars;

    font_atlas->char_data = malloc(sizeof(CharData) * num_chars);
    if(!font_atlas->char_data){
        fprintf(stderr,"%s\n",strerror(errno));
        return NULL;
    }

    int w = 0;
    int h = 0;
    int xpos = 0;
    int ypos = 0;
    int x0,y0,x1,y1,gw,gh,advance;

    for(int i=0; i<num_chars; ++i){
        font_atlas->char_data[i].glyph = stbtt_FindGlyphIndex(font->info,first_char+i);
        
        stbtt_GetGlyphBitmapBox(font->info,font_atlas->char_data[i].glyph,font_atlas->scale,font_atlas->scale,&x0,&y0,&x1,&y1);
        
        gw = x1-x0;
        gh = y1-y0;
        
        if(xpos + gw > renderer->max_texture_size){
            w = Max(w,xpos);
            xpos = 0;
            ypos = h;
        }

        font_atlas->char_data[i].x = xpos;
        font_atlas->char_data[i].y = ypos;
        font_atlas->char_data[i].w = gw;
        if(gh > font_atlas->height){
            font_atlas->char_data[i].h = 0;
            font_atlas->char_data[i].isValid = false;
        }
        else{
            font_atlas->char_data[i].h = gh;
            font_atlas->char_data[i].isValid = true;
        }
        font_atlas->char_data[i].xoffset = (x0 >= 0) ? x0 : 0;
        font_atlas->char_data[i].yoffset = (font_atlas->baseLine+y0 >= 0) ? font_atlas->baseLine+y0 : 0;

        stbtt_GetGlyphHMetrics(font->info,font_atlas->char_data[i].glyph,&advance,NULL);
        font_atlas->char_data[i].xadvance = roundf(advance * font_atlas->scale);

        xpos += font_atlas->char_data[i].w;

        if(ypos + font_atlas->char_data[i].h > h){
            
            h = ypos + font_atlas->char_data[i].h;

            if(h > renderer->max_texture_size){
                free(font_atlas->char_data);
                free(font_atlas);
                return NULL;
            }
        }
    }
    w = Max(w,xpos);

    uint8_t *bitmap = calloc(w*h,sizeof(uint8_t));

    if(!bitmap){
        fprintf(stderr,"%s\n",strerror(errno));
        free(font_atlas->char_data);
        free(font_atlas);
        return NULL;
    }

    for(int i=0; i<num_chars; ++i){

        if(!font_atlas->char_data[i].isValid) continue;
        
        stbtt_MakeGlyphBitmap(
            font->info,
            bitmap+font_atlas->char_data[i].y*w+font_atlas->char_data[i].x,
            font_atlas->char_data[i].w,
            font_atlas->char_data[i].h,
            w,
            font_atlas->scale,
            font_atlas->scale,
            font_atlas->char_data[i].glyph
        );

        font_atlas->char_data[i].s0 = (float)font_atlas->char_data[i].x / (float)w;
        font_atlas->char_data[i].t0 = (float)font_atlas->char_data[i].y / (float)h;
        font_atlas->char_data[i].s1 = (float)(font_atlas->char_data[i].x + font_atlas->char_data[i].w) / (float)w;
        font_atlas->char_data[i].t1 = (float)(font_atlas->char_data[i].y + font_atlas->char_data[i].h) / (float)h;
    }

    uint8_t *pixels = malloc(sizeof(uint8_t) * (w * 4 * h));
    
    MakePixelsRGBA(bitmap,w,h,pixels);

    free(bitmap);

    WG_Texture *texture = WG_CreateUninitializedTexture(renderer,w,h,PIXEL_FORMAT_RGBA,false,false);

    if(!texture){
        free(font_atlas->char_data);
        free(font_atlas);
        free(pixels);
        return NULL;
    }

    texture->pixels = pixels;

    WG_UploadNewTexture(texture->pixels,texture->format,texture->width,texture->height,1,texture->width);

    font_atlas->texture = texture;

    return font_atlas;
}

int WG_GetFontAtlasHeight(WG_FontAtlas *atlas){
    return atlas->height;
}

void WG_SizeTextByFontAtlas(WG_FontAtlas *atlas,const uint16_t *text,int *width,int *height){
    if(width){
        *width = 0;
        int index;
        while(*text){
            index = *text++;
            if(index < 0 || index >= atlas->num_chars) index = 0;
            *width += atlas->char_data[index].xadvance;
        }
    }
    if(height) *height = atlas->height;
}

void WG_FontAtlasFree(WG_FontAtlas *atlas){
    free(atlas->char_data);
    WG_DestroyTexture(atlas->texture);
    free(atlas);
}