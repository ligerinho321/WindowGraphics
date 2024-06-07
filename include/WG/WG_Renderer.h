#ifndef WG_RENDERER_H_
#define WG_RENDERER_H_

#include <WG/WG_Window.h>
#include <WG/GapBuffer.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <math.h>

typedef struct _WG_FontAtlas WG_FontAtlas;
typedef struct _WG_Font      WG_Font;
typedef struct _WG_Texture   WG_Texture;
typedef struct _WG_Renderer  WG_Renderer;

typedef enum _FlipFlag{
    FLIP_NONE       = 0x0000,
    FLIP_HORIZONTAL = 0x0001,
    FLIP_VERTICAL   = 0x0002,
}FlipFlag;

typedef enum _BlendModePreset{
    BLEND_NORMAL,
    BLEND_PREMULTIPLIED_ALPHA,
    BLEND_MULTIPLY,
    BLEND_ADD,
    BLEND_SUBTRACT,
    BLEND_MOD_ALPHA,
    BLEND_SET_ALPHA,
    BLEND_SET,
    BLEND_NORMAL_KEEP_ALPHA,
    BLEND_NORMAL_ADD_ALPHA,
    BLEND_NORMAL_FACTOR_ALPHA,
}BlendModePreset;

typedef enum _BlendFunction{
    FUNC_ZERO                = 0x0000,
    FUNC_ONE                 = 0x0001,
    FUNC_SRC_COLOR           = 0x0300,
    FUNC_ONE_MINUS_SRC_COLOR = 0x0301,
    FUNC_DST_COLOR           = 0x0306,
    FUNC_ONE_MINUS_DST_COLOR = 0x0307,
    FUNC_SRC_ALPHA           = 0x0302,
    FUNC_ONE_MINUS_SRC_ALPHA = 0x0303,
    FUNC_DST_ALPHA           = 0x0304,
    FUNC_ONE_MINUS_DST_ALPHA = 0x0305,
}BlendFunction;

typedef enum _BlendEquation{
    EQ_ADD              = 0x8006,
    EQ_SUBTRACT         = 0x800A,
    EQ_REVERSE_SUBTRACT = 0x800B,
    EQ_MIN              = 0x8007,
    EQ_MAX              = 0x8008,
}BlendEquation;

typedef enum _Filter{
    FILTER_NEAREST                = 0x2600,
    FILTER_LINEAR                 = 0x2601,
    FILTER_NEAREST_MIPMAP_NEAREST = 0x2700,
    FILTER_LINEAR_MIPMAP_NEAREST  = 0x2701,
    FILTER_NEAREST_MIPMAP_LINEAR  = 0x2702,
    FILTER_LINEAR_MIPMAP_LINEAR   = 0x2703,
}Filter;

typedef enum _Wrap{
    WRAP_REPEAT          = 0x2901,
    WRAP_MIRRORED_REPEAT = 0x8370,
    WRAP_CLAMP_TO_EDGE   = 0x812F,
}Wrap;

typedef enum PixelFormat{
    PIXEL_FORMAT_RED             = 0x1903,
    PIXEL_FORMAT_GREEN           = 0x1904,
    PIXEL_FORMAT_BLUE            = 0x1905,
    PIXEL_FORMAT_ALPHA           = 0x1906,
    PIXEL_FORMAT_LUMINANCE       = 0x1909,
    PIXEL_FORMAT_LUMINANCE_ALPHA = 0x190A,
    PIXEL_FORMAT_RG              = 0x8227,
    PIXEL_FORMAT_RGB             = 0x1907,
    PIXEL_FORMAT_RGBA            = 0x1908,
    PIXEL_FORMAT_BGB             = 0x80E0,
    PIXEL_FORMAT_BGBA            = 0x80E1,
}PixelFormat;


typedef struct _Color{
    uint8_t r,g,b,a;
}Color;

typedef struct _RectF{
    float x,y,w,h;
}RectF;

typedef struct _Rect{
    int x,y,w,h;
}Rect;

typedef struct _Vector2F{
    float x,y;
}Vector2F;

typedef struct _Vector2{
    int x,y;
}Vector2;

typedef struct _Vertex{
    Vector2F position;
    Color color;
    Vector2F texcoord;
}Vertex;

typedef struct _BlendMode{
    uint32_t srcRGB;
    uint32_t dstRGB;

    uint32_t srcAlpha;
    uint32_t dstAlpha;

    uint32_t modeRGB;
    uint32_t modeAlpha;
}BlendMode;


BlendMode WG_GetBlendModeByPreset(BlendModePreset preset);

void WG_SetBlendFunction(WG_Renderer *renderer,WG_Texture *target,BlendFunction srcRGB,BlendFunction dstRGB,BlendFunction srcAlpha,BlendFunction dstAlpha);

void WG_SetBlendEquation(WG_Renderer *renderer,WG_Texture *target,BlendEquation modeRGB,BlendEquation modeAlpha);

void WG_SetBlendModeByPreset(WG_Renderer *renderer,WG_Texture *target,BlendModePreset preset);

void WG_SetBlendMode(WG_Renderer *renderer,WG_Texture *target,BlendMode blend_mode);

void WG_SetBlending(WG_Renderer *renderer,WG_Texture *target,bool blending);


void WG_SetClip(WG_Renderer *renderer,WG_Texture *target,Rect *rect);

void WG_UnSetClip(WG_Renderer *renderer,WG_Texture *target);


WG_Texture* WG_CreateTexture(WG_Renderer *renderer,int width,int height,PixelFormat format,bool mipmap,bool framebuffer);

WG_Texture* WG_CreateTextureFromTexture(WG_Renderer *renderer,WG_Texture *texture,bool mipmap,bool framebuffer);

WG_Texture* WG_LoadTextureFromData(WG_Renderer *renderer,const uint8_t *data,uint32_t len,bool mipmap,bool framebuffer);

WG_Texture* WG_LoadTextureFromFile(WG_Renderer *renderer,const uint16_t *fileName,bool mipmap,bool framebuffer);

void WG_DestroyTexture(WG_Texture *texture);

void WG_SetTextureColor(WG_Texture *texture,Color color);

void WG_SetTextureFilter(WG_Texture *texture,Filter min_filter,Filter mag_filter);

void WG_SetTextureWrap(WG_Texture *texture,Wrap wrap_x,Wrap wrap_y);

void WG_GenerateFrameBuffer(WG_Renderer *renderer,WG_Texture *texture);

void WG_GenearateMipmap(WG_Renderer *renderer,WG_Texture *texture);

void WG_LockTexture(WG_Texture *texture,Rect *rect,void **pixels,int *pitch);

void WG_UnlockTexture(WG_Texture *texture);

void WG_TextureSize(WG_Texture *texture,int *width,int *height);

void WG_TextureFormat(WG_Texture *texture,PixelFormat *format);

bool WG_TextureHasFrameBuffer(WG_Texture *texture);

bool WG_TextureHasMipmap(WG_Texture *texture);

void WG_TextureWrap(WG_Texture *texture,Wrap *wrap_x,Wrap *wrap_y);

void WG_TextureFilter(WG_Texture *texture,Filter *min_filter,Filter *mag_filter);



WG_Renderer* WG_CreateRenderer(WG_Window *window,WG_Renderer *share_renderer);

void WG_SetCurrentRenderer(WG_Renderer *renderer);

void WG_DestroyRenderer(WG_Renderer *renderer);

void WG_RendererSetTarget(WG_Renderer *renderer,WG_Texture *target);

void WG_RendererSetVsync(WG_Renderer *renderer,bool enable);

void WG_RendererSetViewport(WG_Renderer *renderer,WG_Texture *target,Rect *rect);


void WG_FlushBlitBuffer(WG_Renderer *renderer);

void WG_ClearColor(WG_Renderer *renderer,Color color);

void WG_ClearRGBA(WG_Renderer *renderer,uint8_t r,uint8_t g,uint8_t b,uint8_t a);

void WG_Flip(WG_Renderer *renderer);



void WG_DrawFilledRectangleF(WG_Renderer *renderer,RectF *rect,Color color);

void WG_DrawFilledRectangle(WG_Renderer *renderer,Rect *rect,Color color);


void WG_DrawFilledRectanglesF(WG_Renderer *renderer,RectF *rects,uint32_t count,Color color);

void WG_DrawFilledRectangles(WG_Renderer *renderer,Rect *rects,uint32_t count,Color color);


void WG_DrawRectangleF(WG_Renderer *renderer,RectF *rect,Color color);

void WG_DrawRectangle(WG_Renderer *renderer,Rect *rect,Color color);


void WG_DrawRectanglesF(WG_Renderer *renderer,RectF *rects,uint32_t count,Color color);

void WG_DrawRectangles(WG_Renderer *renderer,Rect *rects,uint32_t count,Color color);


void WG_DrawFilledTriangleF(WG_Renderer *renderer,Vector2F vertex1,Vector2F vertex2,Vector2F vertex3,Color color);

void WG_DrawFilledTriangle(WG_Renderer *renderer,Vector2 vertex1,Vector2 vertex2,Vector2 vertex3,Color color);


void WG_DrawTriangleF(WG_Renderer *renderer,Vector2F vertex1,Vector2F vertex2,Vector2F vertex3,Color color);

void WG_DrawTriangle(WG_Renderer *renderer,Vector2 vertex1,Vector2 vertex2,Vector2 vertex3,Color color);


void WG_DrawFilledCircleF(WG_Renderer *renderer,Vector2F center,float radius,Color color);

void WG_DrawFilledCircle(WG_Renderer *renderer,Vector2 center,int radius,Color color);


void WG_DrawCircleF(WG_Renderer *renderer,Vector2F center,float radius,Color color);

void WG_DrawCircle(WG_Renderer *renderer,Vector2 center,int radius,Color color);


void WG_DrawFilledArcF(WG_Renderer *renderer,Vector2F center,float radius,float startAngle,float endAngle,Color color);

void WG_DrawFilledArc(WG_Renderer *renderer,Vector2 center,int radius,int startAngle,int endAngle,Color color);


void WG_DrawArcF(WG_Renderer *renderer,Vector2F center,float radius,float startAngle,float endAngle,Color color);

void WG_DrawArc(WG_Renderer *renderer,Vector2 center,int radius,int startAngle,int endAngle,Color color);


void WG_DrawPixelF(WG_Renderer *renderer,Vector2F point,Color color);

void WG_DrawPixel(WG_Renderer *renderer,Vector2 point,Color color);


void WG_DrawLineF(WG_Renderer *renderer,Vector2F point1,Vector2F point2,Color color);

void WG_DrawLine(WG_Renderer *renderer,Vector2 point1,Vector2 point2,Color color);


void WG_DrawLinesF(WG_Renderer *renderer,Vector2F *points,uint32_t points_count,Color color);

void WG_DrawLines(WG_Renderer *renderer,Vector2 *points,uint32_t points_count,Color color);


void WG_DrawPolygonF(WG_Renderer *renderer,Vector2F *vertices,uint32_t vertices_count,Color color);

void WG_DrawPolygon(WG_Renderer *renderer,Vector2 *vertices,uint32_t vertices_count,Color color);


void WG_DrawFilledPolygonF(WG_Renderer *renderer,Vector2F *vertices,uint32_t vertices_count,Color color);

void WG_DrawFilledPolygon(WG_Renderer *renderer,Vector2 *vertices,uint32_t vertices_count,Color color);


void WG_DrawTextF(WG_Renderer *renderer,WG_FontAtlas *atlas,uint16_t *text,Vector2F pos,Color color);

void WG_DrawText(WG_Renderer *renderer,WG_FontAtlas *atlas,uint16_t *text,Vector2 pos,Color color);


void WG_DrawNTextF(WG_Renderer *renderer,WG_FontAtlas *atlas,uint16_t *text,uint32_t n,Vector2F pos,Color color);

void WG_DrawNText(WG_Renderer *renderer,WG_FontAtlas *atlas,uint16_t *text,uint32_t n,Vector2 pos,Color color);


void WG_DrawTextByGapBufferF(WG_Renderer *renderer,WG_FontAtlas *atlas,GapBuffer *gap_buffer,Vector2F pos,Color color);

void WG_DrawTextByGapBuffer(WG_Renderer *renderer,WG_FontAtlas *atlas,GapBuffer *gap_buffer,Vector2 pos,Color color);


void WG_BlitColorF(WG_Renderer *renderer,WG_Texture *texture,RectF *src_rect,RectF *dst_rect,Color color);

void WG_BlitColor(WG_Renderer *renderer,WG_Texture *texture,Rect *src_rect,Rect *dst_rect,Color color);


void WG_BlitF(WG_Renderer *renderer,WG_Texture *texture,RectF *src_rect,RectF *dst_rect);

void WG_Blit(WG_Renderer *renderer,WG_Texture *texture,Rect *src_rect,Rect *dst_rect);


void WG_BlitExF(WG_Renderer *renderer,WG_Texture *texture,RectF *src_rect,RectF *dst_rect,float angle,Vector2F *center,FlipFlag flags);

void WG_BlitEx(WG_Renderer *renderer,WG_Texture *texture,Rect *src_rect,Rect *dst_rect,float angle,Vector2 *center,FlipFlag flags);


void WG_Geometry(WG_Renderer *renderer,WG_Texture *texture,Vertex *vertices,uint32_t vertices_count,uint32_t *indices,uint32_t indices_count);



bool WG_GlyphIsProvided(WG_Font *font,int char_code);

void WG_GlyphMetrics(WG_Font *font,int char_code,int *advance,int *minx,int *maxx,int *miny,int *maxy);

void WG_SetFontKerning(WG_Font *font,bool allowed);

int WG_GetFontHeight(WG_Font *font);

void WG_SetFontHeight(WG_Font *font,int height);

WG_Font *WG_OpenFontFromData(uint8_t *data,int height);

WG_Font* WG_OpenFontFromFile(const uint16_t *fileName,int height);

void WG_CloseFont(WG_Font *font);

void WG_SizeText(WG_Font *font,const uint16_t *text,int *width,int *height);

WG_Texture* WG_RenderGlyph(WG_Renderer *renderer,WG_Font *font,uint16_t char_code);

WG_Texture* WG_RenderText(WG_Renderer *renderer,WG_Font *font,const uint16_t *text);


WG_FontAtlas* WG_RenderFontAtlas(WG_Renderer *renderer,WG_Font *font,int height,int first_char,int num_chars);

int WG_GetFontAtlasHeight(WG_FontAtlas *atlas);

void WG_SizeTextByFontAtlas(WG_FontAtlas *atlas,const uint16_t *text,int *width,int *height);

void WG_FontAtlasFree(WG_FontAtlas *atlas);

#endif
